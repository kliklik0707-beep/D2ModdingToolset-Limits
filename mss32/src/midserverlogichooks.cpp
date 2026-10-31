/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Stanislav Egorov.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "midserverlogichooks.h"
#include <mutex>
#include <atomic>
#include <cstdio>
#include "version.h"
#include "netmsgmapentry.h"
#include "midscenvariables.h"
#include "cmdmovestackendmsg.h"
#include "d2list.h"
#include "dynamiccast.h"
#include "exchangeresourcesmsg.h"
#include "game.h"
#include "gameutils.h"
#include "idset.h"
#include "logutils.h"
#include "midgardplan.h"
#include "midgardscenariomap.h"
#include "midplayer.h"
#include "midruin.h"
#include "midserver.h"
#include "midserverlogic.h"
#include "midsiteresourcemarket.h"
#include "midstack.h"
#include "netmsgcallbacks.h"
#include "netmsgmapentryexchangeresourcesmsg.h"
#include "netplayerinfo.h"
#include "originalfunctions.h"
#include "racetype.h"
#include "refreshinfo.h"
#include "ruinmovement.h"
#include "scenarioinfo.h"
#include "settings.h"
#include <cstdlib>
#include "timer.h"
#include "unitstovalidate.h"
#include "unitutils.h"
#include "utils.h"
#include <chrono>
#include <filesystem>
#include <process.h>
#include "scripts.h"
#include <sol/sol.hpp>
#include <spdlog/spdlog.h>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace hooks {

// spdlog uses const std::string& to get named log so we create singleton of std::string.
// Log name is truncated to 8 characters by the default logger pattern.
extern const std::string eventsPerformanceLog{"evntperf"};

long long conditionsTotalTime = 0;
long long conditionsSystemTime = 0;
long long effectsTime = 0;

static bool __fastcall exchangeResourcesMsgHandler(game::CMidServerLogic* thisptr,
                                                   int /*%edx*/,
                                                   const CExchangeResourcesMsg* netMessage,
                                                   std::uint32_t idFrom)
{
    using namespace game;

    const NetPlayerInfo* playerInfo{CMidServerLogicApi::get().getPlayerInfo(thisptr, idFrom)};
    if (!playerInfo) {
        return false;
    }

    if (!CMidServerLogicApi::get().isCurrentPlayer(thisptr, &playerInfo->playerId)) {
        return true;
    }

    auto objectMap{thisptr->coreData->objectMap};
    if (!exchangeResources(objectMap, netMessage->siteId, netMessage->visitorStackId,
                           netMessage->playerCurrency, netMessage->siteCurrency,
                           netMessage->amount)) {
        return false;
    }

    thisptr->IMidMsgSender::vftable->sendObjectsChanges(thisptr);
    return true;
}

void addValidatedUnitsToChangedObjects(game::CMidgardScenarioMap* scenarioMap)
{
    using namespace game;

    const auto& idSetApi = IdSetApi::get();

    auto& changedObjects = scenarioMap->changedObjects;

    auto& unitsToValidate = getUnitsToValidate();
    for (const auto& unitId : unitsToValidate) {
        auto unit = (CMidUnit*)scenarioMap->vftable->findScenarioObjectById(scenarioMap, &unitId);
        if (unit && validateUnit(unit)) {
            Pair<IdSetIterator, bool> tmp;
            idSetApi.insert(&changedObjects, &tmp, &unitId);
        }
    }
    unitsToValidate.clear();
}

void logObjectsToSend(const game::CMidgardScenarioMap* scenarioMap)
{
    const auto& addedObjects = scenarioMap->addedObjects;
    const auto& changedObjects = scenarioMap->changedObjects;
    const auto& objectsToErase = scenarioMap->objectsToErase;

    spdlog::debug("Sending scenario objects changes, added: {:d}, changed: {:d}, erased: {:d}",
                  addedObjects.length, changedObjects.length, objectsToErase.length);

    auto totalLength = addedObjects.length + changedObjects.length + objectsToErase.length;
    if (totalLength >= gameSettings().debug.sendObjectsChangesTreshold) {
        for (auto obj : addedObjects) {
            spdlog::debug("Added\t{:s}\t{:s}", idToString(&obj), getMidgardIdTypeDesc(&obj));
        }
        for (auto obj : changedObjects) {
            spdlog::debug("Changed\t{:s}\t{:s}", idToString(&obj), getMidgardIdTypeDesc(&obj));
        }
        for (auto obj : objectsToErase) {
            spdlog::debug("Erased\t{:s}\t{:s}", idToString(&obj), getMidgardIdTypeDesc(&obj));
        }
    }
}

bool __fastcall midServerLogicSendObjectsChangesHooked(game::IMidMsgSender* thisptr, int /*%edx*/)
{
    using namespace game;

    const auto serverLogic = castMidMsgSenderToMidServerLogic(thisptr);
    auto scenarioMap = CMidServerLogicApi::get().getObjectMap(serverLogic);

    if (gameSettings().modifiers.validateUnitsOnGroupChanged) {
        addValidatedUnitsToChangedObjects(scenarioMap);
    }

    if (gameSettings().debugMode && gameSettings().debug.sendObjectsChangesTreshold) {
        logObjectsToSend(scenarioMap);
    }

    return getOriginalFunctions().midServerLogicSendObjectsChanges(thisptr);
}

bool __fastcall midServerLogicSendRefreshInfoHooked(const game::CMidServerLogic* thisptr,
                                                    int /*%edx*/,
                                                    const game::Set<game::CMidgardID>* objectsList,
                                                    std::uint32_t playerNetId)
{
    using namespace game;

    const auto& refreshInfoApi = CRefreshInfoApi::get();

    const auto scenarioMap = CMidServerLogicApi::get().getObjectMap(thisptr);
    const auto limit = gameSettings().engine.sendRefreshInfoObjectCountLimit;

    auto it = objectsList->begin();
    const auto end = objectsList->end();
    auto sendRefreshInfo = [&](bool& proceed) {
        CRefreshInfo refreshInfo{};
        refreshInfoApi.constructor2(&refreshInfo, &scenarioMap->scenarioFileId,
                                    thisptr->coreData->isExpansionContent);

        proceed = false;
        std::uint32_t count = 0;
        for (; it != end; ++it) {
            if (++count > limit) {
                proceed = true;
                break;
            }
            refreshInfoApi.addObject(&refreshInfo, scenarioMap, &(*it));
        }

        auto server = thisptr->coreData->server;
        bool result = CMidServerApi::get().sendNetMsg(server, &refreshInfo, playerNetId);

        refreshInfoApi.destructor(&refreshInfo);
        return result;
    };

    for (bool proceed = true; proceed;) {
        if (!sendRefreshInfo(proceed)) {
            return false;
        }
    }

    return true;
}

bool __fastcall applyEventEffectsAndCheckMidEventTriggerersHooked(
    game::CMidServerLogic** thisptr,
    int /*%edx*/,
    game::List<game::IEventEffect*>* effectsList,
    const game::CMidgardID* triggererId,
    const game::CMidgardID* playingStackId)
{
#if 0
    const ScopedTimer timer{"Apply event effects and check triggerers", eventsPerformanceLog};
#endif

    return getOriginalFunctions().applyEventEffectsAndCheckMidEventTriggerers(thisptr, effectsList,
                                                                              triggererId,
                                                                              playingStackId);
}

bool __fastcall stackMoveHooked(game::CMidServerLogic** thisptr,
                                int /*%edx*/,
                                const game::CMidgardID* playerId,
                                game::List<game::Pair<game::CMqPoint, int>>* movementPath,
                                const game::CMidgardID* stackId,
                                const game::CMqPoint* startingPoint,
                                const game::CMqPoint* endPoint)
{
    using namespace game;

#if 0
    char message[256];
    const auto result{fmt::format_to_n(message, sizeof(message) - 1u,
                                       "Stack move from ({:d}, {:d}) to ({:d}, {:d})",
                                       startingPoint->x, startingPoint->y, endPoint->x,
                                       endPoint->y)};
    message[result.size] = 0;

    const ScopedTimer timer{std::string_view{message, result.size}, eventsPerformanceLog};
#endif

    auto objectMap = CMidServerLogicApi::get().getObjectMap(*thisptr);
    const CMidStack* stackBefore = getStack(objectMap, stackId);

    const CMidPlayer* mover = playerId ? getPlayer(objectMap, playerId) : nullptr;
    const bool humanMover = mover && mover->isHuman;

    bool lootedRuinBeforeMove = false;
    if (stackBefore && humanMover) {
        if (auto plan = getMidgardPlan(objectMap)) {
            lootedRuinBeforeMove = isLootedRuinInteraction(objectMap, plan, stackBefore, endPoint,
                                                           startingPoint);
        }
    }

    const bool skipLootedRuinEntry = shouldSkipLootedRuinEntry(
        stackBefore != nullptr, humanMover, lootedRuinBeforeMove);

    bool result = false;
    if (skipLootedRuinEntry) {
        const int mp = stackBefore ? static_cast<int>(stackBefore->movement) : -1;
        const int sx = startingPoint ? startingPoint->x : -1;
        const int sy = startingPoint ? startingPoint->y : -1;
        const int ex = endPoint ? endPoint->x : -1;
        const int ey = endPoint ? endPoint->y : -1;
        spdlog::info("looted ruin skip entry mp={} from=({},{}) to=({},{})", mp, sx, sy, ex, ey);
        result = true;
    } else {
        result = getOriginalFunctions().stackMove(thisptr, playerId, movementPath, stackId,
                                                  startingPoint, endPoint);
    }

    IMidMsgSender* sender = thisptr ? *thisptr : nullptr;
    if (sender && sender->vftable && sender->vftable->sendMessage) {
        CCmdMoveStackEndMsg message;
        if (!sender->vftable->sendMessage(sender, &message, false)) {
            spdlog::error(__FUNCTION__ ": failed to send CCmdMoveStackEndMsg");
        }
    }

    return result;
}

bool __stdcall filterAndProcessEventsNoPlayerHooked(game::IMidgardObjectMap* objectMap,
                                                    game::List<game::CMidEvent*>* eventObjectList,
                                                    game::List<game::IEventEffect*>* effectsList,
                                                    bool* stopProcessing,
                                                    game::IdList* executedEvents,
                                                    const game::CMidgardID* triggererStackId,
                                                    const game::CMidgardID* playingStackId)
{
    using namespace game;

#if 0
    const ScopedTimer timer{"Filter and process events (no player)", eventsPerformanceLog};
#endif

    return CMidServerLogicApi::get().filterAndProcessEvents(objectMap, eventObjectList, effectsList,
                                                            stopProcessing, executedEvents,
                                                            &emptyId, triggererStackId,
                                                            playingStackId);
}

bool __stdcall filterAndProcessEventsHooked(game::IMidgardObjectMap* objectMap,
                                            game::List<game::CMidEvent*>* eventObjectList,
                                            game::List<game::IEventEffect*>* effectsList,
                                            bool* stopProcessing,
                                            game::IdList* executedEvents,
                                            const game::CMidgardID* playerId,
                                            const game::CMidgardID* triggererStackId,
                                            const game::CMidgardID* playingStackId)
{
    using namespace game;

#if 0
    const ScopedTimer timer{"Filter and process events", eventsPerformanceLog};
#endif

    if (*playerId != emptyId && gameFunctions().ignorePlayerEvents(playerId, objectMap)) {
        return false;
    }

    CMidgardID triggererId{*triggererStackId};
    const CMidStack* triggererStack{};

    if (triggererId != emptyId) {
        triggererStack = getStack(objectMap, &triggererId);
        if (!triggererStack && triggererId != emptyId) {
            triggererId = emptyId;
        }
    }

    // Use unordered set for fast lookup of executed events
    std::unordered_set<CMidgardID, CMidgardIDHash> executedEventsSet(executedEvents->length);
    for (const CMidgardID& id : *executedEvents) {
        executedEventsSet.insert(id);
    }

    const auto& rtti = RttiApi::rtti();
    const auto dynamicCast = RttiApi::get().dynamicCast;

    // Cache players so we won't waste time searching them every time
    std::unordered_map<RaceId, CMidgardID /* player id */> racePlayerMap;

    auto cachePlayer = [triggererStack, &racePlayerMap, &rtti, &dynamicCast](const auto* obj) {
        auto* player = (const CMidPlayer*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                      rtti.CMidPlayerType, 0);

        if (!triggererStack || player->id == triggererStack->ownerId) {
            const RaceId raceId{player->raceType->data->raceType.id};
            racePlayerMap[raceId] = player->id;
        }
    };

    forEachScenarioObject(objectMap, IdType::Player, cachePlayer);

    const auto& raceSetFind{RaceSetApi::get().find};
    const auto& checkAndExecuteEvent{CMidServerLogicApi::get().checkAndExecuteEvent};

    for (const CMidEvent* evt : *eventObjectList) {
        if (!evt->enabled) {
            // Event disabled, don't waste time checking anything
            continue;
        }

        if (executedEventsSet.find(evt->id) != executedEventsSet.end()) {
            // Event already executed, skip
            continue;
        }

        const auto end{evt->racesCanTrigger.end()};

        bool eventExecuted = false;
        LRaceCategory raceCategory{};
        SetIterator<LRaceCategory> it{};
        for (const auto& [raceId, playerCanTriggerId] : racePlayerMap) {
            // Only category id is checked during search. Don't bother setting other fields
            raceCategory.id = raceId;
            raceSetFind(&evt->racesCanTrigger, &it, &raceCategory);

            if (it == end) {
                // Race can't trigger
                continue;
            }

#ifdef D2_MEASURE_EVENTS_TIME
            conditionsTotalTime = 0;
            conditionsSystemTime = 0;
            effectsTime = 0;

            char message[256];
            const auto result{fmt::format_to_n(message, sizeof(message) - 1u, "  Event '{:s}'",
                                               evt->name.string ? evt->name.string : "?")};
            message[result.size] = 0;

            {
                const ScopedTimer eventTimer{std::string_view{message, result.size},
                                             eventsPerformanceLog};
#endif

                const bool samePlayer = *playerId == playerCanTriggerId;
                // Player can trigger, check event conditions and execute effects
                if (checkAndExecuteEvent(objectMap, effectsList, stopProcessing, &evt->id,
                                         &playerCanTriggerId, &triggererId, playingStackId,
                                         samePlayer)) {
                    eventExecuted = true;
                }

#ifdef D2_MEASURE_EVENTS_TIME
            }

            const auto overhead{conditionsSystemTime - conditionsTotalTime};
            spdlog::debug("{:s} conditions time {:d} us, "
                          "conditions system time {:d} us (overhead {:d} us), "
                          "effects time {:d} us",
                          message, conditionsTotalTime, conditionsSystemTime, overhead,
                          effectsTime);
#endif
        }

        if (eventExecuted) {
            IdListApi::get().pushBack(executedEvents, &evt->id);
            return true;
        }
    }

    return false;
}

bool __stdcall checkEventConditionsHooked(const game::IMidgardObjectMap* objectMap,
                                          game::List<game::IEventEffect*>* effectsList,
                                          const game::CMidgardID* playerId,
                                          const game::CMidgardID* stackTriggererId,
                                          int samePlayer,
                                          const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    const ScopedValueTimer timer{conditionsSystemTime};
#endif

    return getOriginalFunctions().checkEventConditions(objectMap, effectsList, playerId,
                                                       stackTriggererId, samePlayer, eventId);
}

void __stdcall executeEventEffectsHooked(game::IMidgardObjectMap* objectMap,
                                         game::List<game::IEventEffect*>* effectsList,
                                         bool* stopProcessing,
                                         const game::CMidgardID* eventId,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* stackTriggererId,
                                         const game::CMidgardID* playingStackId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    const ScopedValueTimer timer{effectsTime};
#endif

    getOriginalFunctions().executeEventEffects(objectMap, effectsList, stopProcessing, eventId,
                                               playerId, stackTriggererId, playingStackId);
}

static bool doTestHooked(game::ITestConditionVftable::Test testFunc,
                         const char* name,
                         const game::ITestCondition* thisptr,
                         const game::IMidgardObjectMap* objectMap,
                         const game::CMidgardID* playerId,
                         const game::CMidgardID* eventId)
{
    using namespace game;

#ifdef D2_MEASURE_EVENTS_TIME
    char message[256];
    const auto result{
        fmt::format_to_n(message, sizeof(message) - 1u, "    Test condition '{:s}'", name)};
    message[result.size] = 0;

    const ScopedTimer conditionTimer{std::string_view{message, result.size}, eventsPerformanceLog};
    const ScopedValueTimer timer{conditionsTotalTime};
#endif

    return testFunc(thisptr, objectMap, playerId, eventId);
}

bool __fastcall testFreqHooked(const game::ITestCondition* thisptr,
                               int /*%edx*/,
                               const game::IMidgardObjectMap* objectMap,
                               const game::CMidgardID* playerId,
                               const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testFrequency, "frequency", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testLocationHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testLocation, "location", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testEnterCityHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testEnterCity, "enter city", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testLeaderToCityHooked(const game::ITestCondition* thisptr,
                                       int /*%edx*/,
                                       const game::IMidgardObjectMap* objectMap,
                                       const game::CMidgardID* playerId,
                                       const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testLeaderToCity, "leader to city", thisptr,
                        objectMap, playerId, eventId);
}

bool __fastcall testOwnCityHooked(const game::ITestCondition* thisptr,
                                  int /*%edx*/,
                                  const game::IMidgardObjectMap* objectMap,
                                  const game::CMidgardID* playerId,
                                  const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testOwnCity, "own city", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testDiplomacyHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testDiplomacy, "diplomacy", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testAllianceHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testAlliance, "alliance", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testLootRuinHooked(const game::ITestCondition* thisptr,
                                   int /*%edx*/,
                                   const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testLootRuin, "loot ruin", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testTransformLandHooked(const game::ITestCondition* thisptr,
                                        int /*%edx*/,
                                        const game::IMidgardObjectMap* objectMap,
                                        const game::CMidgardID* playerId,
                                        const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testTransformLand, "transform land", thisptr,
                        objectMap, playerId, eventId);
}

bool __fastcall testVisitSiteHooked(const game::ITestCondition* thisptr,
                                    int /*%edx*/,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* playerId,
                                    const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testVisitSite, "visit site", thisptr, objectMap,
                        playerId, eventId);
}

bool __fastcall testItemToLocationHooked(const game::ITestCondition* thisptr,
                                         int /*%edx*/,
                                         const game::IMidgardObjectMap* objectMap,
                                         const game::CMidgardID* playerId,
                                         const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testItemToLocation, "item to location", thisptr,
                        objectMap, playerId, eventId);
}

bool __fastcall testVarInRangeHooked(const game::ITestCondition* thisptr,
                                     int /*%edx*/,
                                     const game::IMidgardObjectMap* objectMap,
                                     const game::CMidgardID* playerId,
                                     const game::CMidgardID* eventId)
{
    return doTestHooked(getOriginalFunctions().testVarInRange, "var in range", thisptr, objectMap,
                        playerId, eventId);
}

// ZM ETAP34: experimental session-only spell cap (NOT SAVE SAFE)
// The data is intentionally not serialized. A load/restart can reset the quota.
// This mode is available ONLY when explicitly enabled in settings.lua and ONLY
// in regular single-player (never multiplayer or hotseat).
struct Zm34SpellMeter {
    int turn{-1};
    unsigned int confirmedPaid{0};
};
// ZM ETAP35: defer meter reset until the first actual spell message.
// No STL container mutation, no quota mutex acquisition inside server constructor.
// Epoch prevents accidental reuse if the engine recycles a server object address.
static std::mutex zm34MeterMutex;
static std::atomic<game::CMidServerLogic*> zm34ServerOwner{nullptr};
static std::atomic<bool> zm34OrdinarySinglePlayer{false};
static std::atomic<bool> zm38HotseatSession{false}; // ETAP38A: ctor flag, no map mutation.
static std::atomic<unsigned int> zm35SessionEpoch{0};
static unsigned int zm35AppliedEpoch = 0; // Guarded by zm34MeterMutex.
static std::unordered_map<int, Zm34SpellMeter> zm34SessionMeter;

static void zm35RegisterSession(game::CMidServerLogic* owner, bool singlePlayer, bool hotseat)
{
    zm34OrdinarySinglePlayer.store(singlePlayer, std::memory_order_relaxed);
    zm38HotseatSession.store(hotseat, std::memory_order_relaxed);
    zm34ServerOwner.store(owner, std::memory_order_release);
    const auto epoch = zm35SessionEpoch.fetch_add(1, std::memory_order_acq_rel) + 1;
    // No map.clear(), no lock: actual counter reset is deferred to first spell.
    spdlog::warn("[ZM35_LOAD] SESSION_REGISTERED epoch={} singlePlayer={} "
                 "lazyResetPending=true (NO MAP CLEAR DURING LOAD)", epoch, singlePlayer);
}

static void zm35EnsureFreshMeterLocked()
{
    const auto epoch = zm35SessionEpoch.load(std::memory_order_acquire);
    if (zm35AppliedEpoch != epoch) {
        zm34SessionMeter.clear();
        zm35AppliedEpoch = epoch;
        spdlog::warn("[ZM35_SPELL] LAZY_METER_RESET epoch={} (first spell; not map load)",
                     epoch);
    }
}

static bool zm34IsExperimentalEnabled(game::CMidServerLogic* owner, int turn)
{
    const auto& limits = gameSettings().zmLimits;
    if (!(limits.enabled && limits.testSessionSpellBlock && limits.spellsPerTurn > 0
          && gameSettings().maxSpellsPerTurn >= limits.spellsPerTurn
          && owner && turn >= 0 && turn <= 32767))
        return false;
    return zm34ServerOwner.load(std::memory_order_acquire) == owner
        && zm34OrdinarySinglePlayer.load(std::memory_order_relaxed);
}

static unsigned int zm34GetSessionPaidCasts(int playerId, int turn)
{
    std::lock_guard<std::mutex> guard(zm34MeterMutex);
    zm35EnsureFreshMeterLocked();
    auto& meter = zm34SessionMeter[playerId];
    if (meter.turn != turn) {
        meter.turn = turn;
        meter.confirmedPaid = 0;
    }
    return meter.confirmedPaid;
}

static unsigned int zm34RecordPaidCast(int playerId, int turn)
{
    std::lock_guard<std::mutex> guard(zm34MeterMutex);
    zm35EnsureFreshMeterLocked();
    auto& meter = zm34SessionMeter[playerId];
    if (meter.turn != turn) {
        meter.turn = turn;
        meter.confirmedPaid = 0;
    }
    if (meter.confirmedPaid < 65535u)
        ++meter.confirmedPaid;
    return meter.confirmedPaid;
}

// ZM ETAP36: opt-in saved-variable quota (requires pre-existing scenario slot)
// ZM ETAP31: diagnostic wrapper for CSpellCastMsg (NOT a spell quota)
// This is the server-side message handler, not the client UI action.
// Only observe, forward to the game's original callback and log its return.
// A true result indicates handler acceptance, NOT guaranteed successful magic.
static game::CNetMsgMapEntry_member::Callback zm31OriginalCastSpellCallback{};

static bool __fastcall zm31CastSpellMessageObserved(game::CMidServerLogic* thisptr,
                                                    int /*edx*/,
                                                    game::CNetMsg* message,
                                                    std::uint32_t idFrom)
{
    auto callback = zm31OriginalCastSpellCallback;
    if (!callback) {
        spdlog::error("[ZM31_SPELL] no original callback; rejecting to avoid invalid call");
        return false;
    }

    int turn = -1;
    if (thisptr && thisptr->coreData && thisptr->coreData->objectMap) {
        if (const auto* info = getScenarioInfo(thisptr->coreData->objectMap)) {
            turn = info->currentTurn;
        }
    }
    const auto* player = thisptr
                             ? game::CMidServerLogicApi::get().getPlayerInfo(thisptr, idFrom)
                             : nullptr;
    const int playerId = player ? player->playerId.value : 0;
    // ZM ETAP32: separate safe engine ceilings and ZM caps
    // Analyze changes to resource banks; callbackResult=true is not proof of a cast.
    // No spell rejection and NO DLL-memory quota counters in this stage.
    const auto* objectMap = thisptr && thisptr->coreData
                                ? thisptr->coreData->objectMap : nullptr;
    const auto* beforePlayer = (objectMap && player)
        ? getPlayer(objectMap, &player->playerId) : nullptr;
    const game::Bank beforeBank = beforePlayer ? beforePlayer->bank : game::Bank{};
    const game::Bank beforeSpellBank = beforePlayer ? beforePlayer->spellBank : game::Bank{};
    const bool hadPlayer = beforePlayer != nullptr;
    const unsigned int playerIndex = player ? (static_cast<unsigned int>(playerId) & 0xffffu) : 0u;
    char scenarioCounterName[24];
    std::snprintf(scenarioCounterName, sizeof(scenarioCounterName), "ZM_SP_%04X", playerIndex);
    const auto* variables = objectMap ? getScenarioVariables(objectMap) : nullptr;
    const bool persistentSlotFound = variables && hasScenarioVariableByName(variables, scenarioCounterName);
    const int persistentValue = persistentSlotFound
        ? getScenarioVariableByName(variables, scenarioCounterName, -1) : -1;
    spdlog::warn("[ZM32_SPELL] preflight player={} turn={} saveSlot={} present={} value={} "
                 "legacyCap={} zmCap={} zmEnabled={}",
                 playerId, turn, scenarioCounterName, persistentSlotFound, persistentValue,
                 gameSettings().maxSpellsPerTurn, gameSettings().zmLimits.spellsPerTurn,
                 gameSettings().zmLimits.enabled);
    spdlog::warn("[ZM31_SPELL] BEGIN from={} playerId={} turn={} msg={}",
                 idFrom, playerId, turn, static_cast<const void*>(message));

    // ZM ETAP36: opt-in saved-variable quota, for EXISTING map slots ONLY.
    // The saved scenario variable is authoritative after save/load. Stage33 remains
    // the sole writer (after accepted, resource-spending cast). No DLL-RAM mirroring.
    // Do not enable alongside testSessionSpellBlock; fail open if misconfigured.
    // ZM ETAP38A: hotseat session audit (NO hotseat block or scenario writes)
    const auto& zm36Cfg = gameSettings().zmLimits;
    const bool zm38AuditHotseat = zm36Cfg.enabled && zm36Cfg.testPersistedSpellBlock
        && !zm36Cfg.testSessionSpellBlock && hadPlayer && player
        && turn >= 0 && turn <= 32767
        && zm38HotseatSession.load(std::memory_order_relaxed)
        && !zm34OrdinarySinglePlayer.load(std::memory_order_relaxed)
        && zm34ServerOwner.load(std::memory_order_acquire) == thisptr;
    if (zm38AuditHotseat) {
        const auto count = zm34GetSessionPaidCasts(playerId, turn);
        spdlog::warn("[ZM38_HOTSEAT] PRE player={} from={} turn={} "
                     "slot={} slotFound={} slotValue={} sessionPaid={} max={} "
                     "NO_BLOCK NO_SCENARIO_WRITE", playerId, idFrom, turn,
                     scenarioCounterName, persistentSlotFound, persistentValue,
                     count, zm36Cfg.spellsPerTurn);
    }
    const bool zm36Mode = zm36Cfg.enabled && zm36Cfg.testPersistedSpellBlock
        && !zm36Cfg.testSessionSpellBlock && zm36Cfg.spellsPerTurn > 0
        && zm36Cfg.spellsPerTurn <= 65535
        && gameSettings().maxSpellsPerTurn >= zm36Cfg.spellsPerTurn
        && turn >= 0 && turn <= 32767 && hadPlayer && player
        && zm34ServerOwner.load(std::memory_order_acquire) == thisptr
        && zm34OrdinarySinglePlayer.load(std::memory_order_relaxed);
    if (zm36Cfg.testPersistedSpellBlock && !zm36Mode) {
        spdlog::warn("[ZM36_PERSIST] INACTIVE player={} turn={} "
                     "(another mode, invalid cap, multiplayer or unavailable state)",
                     playerId, turn);
    }
    if (zm36Mode) {
        if (!persistentSlotFound) {
            spdlog::warn("[ZM36_PERSIST] SLOT_MISSING player={} slot={} "
                         "(AUDIT_ONLY: never claim save/load protection)",
                         playerId, scenarioCounterName);
        } else {
            const auto packed = static_cast<std::uint32_t>(persistentValue);
            const unsigned int storedTurn = packed >> 16;
            const unsigned int storedCount = packed & 0xffffu;
            const unsigned int used = storedTurn == static_cast<unsigned int>(turn)
                ? storedCount : 0u;
            spdlog::warn("[ZM36_PERSIST] PRECHECK player={} turn={} slot={} "
                         "storedTurn={} used={} max={}",
                         playerId, turn, scenarioCounterName, storedTurn, used,
                         zm36Cfg.spellsPerTurn);
            if (used >= static_cast<unsigned int>(zm36Cfg.spellsPerTurn)) {
                spdlog::warn("[ZM36_PERSIST] DENY_SAVED player={} turn={} "
                             "used={} max={} NO_CALLBACK NO_SPEND",
                             playerId, turn, used, zm36Cfg.spellsPerTurn);
                showErrorMessageBox(fmt::format("Spell limit reached!\nYou have already cast {} spells this turn.", zm36Cfg.spellsPerTurn));
                return true; // As stage34: message handled; original callback not run.
            }
        }
    }

    // ZM ETAP34: check experimental count BEFORE the original cast callback.
    // Return true (message handled) without invoking the original handler: no spell
    // is executed, no bank is debited. Client-side notification is NOT implemented.
    const bool zm34SessionMode = zm34IsExperimentalEnabled(thisptr, turn) && hadPlayer;
    if (gameSettings().zmLimits.testSessionSpellBlock && !zm34SessionMode) {
        spdlog::warn("[ZM34_SPELL] TEST_NOT_ACTIVE player={} turn={} "
                     "(multiplayer, missing player, invalid caps or no server; NO BLOCK)",
                     playerId, turn);
    }
    if (zm34SessionMode) {
        const auto spent = zm34GetSessionPaidCasts(playerId, turn);
        const auto cap = static_cast<unsigned int>(gameSettings().zmLimits.spellsPerTurn);
        if (spent >= cap) {
            spdlog::warn("[ZM34_SPELL] DENY_TEST_SESSION player={} turn={} "
                         "paidCasts={} max={} NO_CALLBACK NO_SPEND "
                         "(NO in-game popup; NOT SAVE SAFE)",
                         playerId, turn, spent, cap);
            showErrorMessageBox(fmt::format("Spell limit reached!\nYou have already cast {} spells this turn.", cap));
            return true;
        }
        spdlog::warn("[ZM34_SPELL] ALLOW_TEST_SESSION player={} turn={} "
                     "paidCasts={} max={}", playerId, turn, spent, cap);
    }

    const bool accepted = callback(thisptr, message, idFrom);
    const auto* postMap = thisptr && thisptr->coreData
        ? thisptr->coreData->objectMap : nullptr;
    const auto* afterPlayer = (postMap && player)
        ? getPlayer(postMap, &player->playerId) : nullptr;
    const game::Bank afterBank = afterPlayer ? afterPlayer->bank : game::Bank{};
    const game::Bank afterSpellBank = afterPlayer ? afterPlayer->spellBank : game::Bank{};
    spdlog::warn("[ZM32_SPELL] result player={} turn={} callback={} playerPresent={} "
                 "bankDelta=({},{},{},{},{},{}) spellBankDelta=({},{},{},{},{},{}) "
                 "(audit only, NO spell block)",
                 playerId, turn, accepted, hadPlayer && afterPlayer != nullptr,
                 static_cast<int>(afterBank.gold) - beforeBank.gold,
                 static_cast<int>(afterBank.infernalMana) - beforeBank.infernalMana,
                 static_cast<int>(afterBank.lifeMana) - beforeBank.lifeMana,
                 static_cast<int>(afterBank.deathMana) - beforeBank.deathMana,
                 static_cast<int>(afterBank.runicMana) - beforeBank.runicMana,
                 static_cast<int>(afterBank.groveMana) - beforeBank.groveMana,
                 static_cast<int>(afterSpellBank.gold) - beforeSpellBank.gold,
                 static_cast<int>(afterSpellBank.infernalMana) - beforeSpellBank.infernalMana,
                 static_cast<int>(afterSpellBank.lifeMana) - beforeSpellBank.lifeMana,
                 static_cast<int>(afterSpellBank.deathMana) - beforeSpellBank.deathMana,
                 static_cast<int>(afterSpellBank.runicMana) - beforeSpellBank.runicMana,
                 static_cast<int>(afterSpellBank.groveMana) - beforeSpellBank.groveMana);
    // ZM ETAP33: bounded potion denial diagnostics and scenario-backed spell meter
    // This is an AUDIT, NOT a quota. Count only accepted casts that spend resources.
    // Use a pre-existing scenario variable named ZM_SP_XXXX where XXXX is player ID low16.
    // Never create Tree/Map nodes in engine memory; never fall back to DLL RAM counters.
    // All mutations must use findScenarioObjectByIdForChange (marks save/network change).
    const bool zm33SpentResources = accepted && hadPlayer && afterPlayer
        && (afterBank.gold < beforeBank.gold
            || afterBank.infernalMana < beforeBank.infernalMana
            || afterBank.lifeMana < beforeBank.lifeMana
            || afterBank.deathMana < beforeBank.deathMana
            || afterBank.runicMana < beforeBank.runicMana
            || afterBank.groveMana < beforeBank.groveMana
            || afterSpellBank.gold < beforeSpellBank.gold
            || afterSpellBank.infernalMana < beforeSpellBank.infernalMana
            || afterSpellBank.lifeMana < beforeSpellBank.lifeMana
            || afterSpellBank.deathMana < beforeSpellBank.deathMana
            || afterSpellBank.runicMana < beforeSpellBank.runicMana
            || afterSpellBank.groveMana < beforeSpellBank.groveMana);
    // ZM ETAP34: a successful original callback is not enough to count.
    // Increment ONLY when callback accepted and a resource balance declined.
    // Free spells are not counted. No added save-state, no touches to units.
    if (zm34SessionMode) {
        if (zm33SpentResources) {
            const auto count = zm34RecordPaidCast(playerId, turn);
            spdlog::warn("[ZM34_SPELL] PAID_CAST player={} turn={} "
                         "count={} max={} (SESSION_ONLY)",
                         playerId, turn, count, gameSettings().zmLimits.spellsPerTurn);
        } else {
            spdlog::warn("[ZM34_SPELL] NOT_COUNTED player={} turn={} callback={} "
                         "(free/failed/unverified; SESSION_ONLY)",
                         playerId, turn, accepted);
        }
    }
    // ZM ETAP38A: record ONLY cost-confirmed casts in an in-DLL RAM audit meter.
    // This counter is not saved; it cannot enforce a quota and never alters the game.
    if (zm38AuditHotseat) {
        if (zm33SpentResources) {
            const auto count = zm34RecordPaidCast(playerId, turn);
            spdlog::warn("[ZM38_HOTSEAT] PAID player={} turn={} sessionPaid={} "
                         "max={} capReached={} NO_BLOCK NO_SCENARIO_WRITE",
                         playerId, turn, count, zm36Cfg.spellsPerTurn,
                         count >= static_cast<unsigned int>(zm36Cfg.spellsPerTurn));
        } else {
            spdlog::warn("[ZM38_HOTSEAT] NOT_COUNTED player={} turn={} "
                         "callback={} spent=false (free/failed/unverified)",
                         playerId, turn, accepted);
        }
    }
    // Format: (turn << 16) | count; zero in editor is the initial value.
    // Runs only for supported turns and named vars already created by the map author.
    const auto* zm33ScenarioInfo = postMap ? getScenarioInfo(postMap) : nullptr;
    const bool zm33TurnStable = zm33ScenarioInfo && zm33ScenarioInfo->currentTurn == turn;
    // ZM ETAP38D15: TEST ONLY. Enable the EXISTING post-accepted-spell
    // scenario-variable writer for VERIFIED local hotseat sessions as well.
    // Other network multiplayer stays excluded. No server-side spell denial.
    // Requires a pre-existing ZM_SP_XXXX scenario variable for each player.
    const bool zm38d15HotseatWriter = zm36Cfg.enabled
        && zm36Cfg.testPersistedSpellBlock
        && !zm36Cfg.testSessionSpellBlock
        && zm38HotseatSession.load(std::memory_order_relaxed)
        && !zm34OrdinarySinglePlayer.load(std::memory_order_relaxed);
    const bool zm38d15WriterMode =
        zm34OrdinarySinglePlayer.load(std::memory_order_relaxed)
        || zm38d15HotseatWriter;
    if (gameSettings().zmLimits.enabled && player && postMap
        && zm38d15WriterMode
        && zm34ServerOwner.load(std::memory_order_acquire) == thisptr
        && turn >= 0 && turn <= 32767 && zm33TurnStable) {
        if (zm38d15HotseatWriter)
            spdlog::warn("[ZM38D15_SAVE] HOTSEAT_WRITE_CHECK player={} turn={} "
                         "slot={} accepted={} spent={}", playerId, turn,
                         scenarioCounterName, accepted, zm33SpentResources);
        const auto* postVars = getScenarioVariables(postMap);
        if (!postVars || !hasScenarioVariableByName(postVars, scenarioCounterName)) {
            spdlog::warn("[ZM33_SPELL] SLOT_MISSING name={} player={} turn={} (audit, NO BLOCK)",
                         scenarioCounterName, playerId, turn);
        } else if (zm33SpentResources) {
            // D16: only accepted, resource-spending callbacks arrive here.
            // Do not infer casts from client-side attempts.
            spdlog::warn("[ZM38D16_SERVER] CONFIRMED_PAID player={} turn={} "
                         "slot={} accepted={} spent={}",
                         playerId, turn, scenarioCounterName, accepted,
                         zm33SpentResources);
            const int prior = getScenarioVariableByName(postVars, scenarioCounterName, 0);
            const auto priorU = static_cast<std::uint32_t>(prior);
            const auto storedTurn = static_cast<unsigned>(priorU >> 16);
            const unsigned currentCount = storedTurn == static_cast<unsigned>(turn)
                ? (priorU & 0xffffu) : 0u;
            if (currentCount >= 0xffffu) {
                spdlog::error("[ZM33_SPELL] COUNT_OVERFLOW player={} turn={}", playerId, turn);
            } else {
                const int next = static_cast<int>((static_cast<unsigned>(turn) << 16)
                                                  | (currentCount + 1u));
                auto* writableMap = const_cast<game::IMidgardObjectMap*>(postMap);
                const auto* changedObj = writableMap->vftable->findScenarioObjectByIdForChange(
                    writableMap, &postVars->id);
                auto* writableVars = const_cast<game::CMidScenVariables*>(
                    static_cast<const game::CMidScenVariables*>(changedObj));
                const bool saved = writableVars
                    && setScenarioVariableByName(writableVars, scenarioCounterName, next);
                spdlog::warn("[ZM33_SPELL] COUNT player={} turn={} count={} slot={} write={} "
                             "(cost-confirmed AUDIT ONLY, NO BLOCK)",
                             playerId, turn, currentCount + 1u, scenarioCounterName, saved);
                if (zm38d15HotseatWriter) {
                    // Read-back is diagnostic; the actual save/load round trip
                    // remains an in-game test. Do not spoof success.
                    const auto* verifyVars = getScenarioVariables(postMap);
                    const int readback = verifyVars
                        ? getScenarioVariableByName(verifyVars, scenarioCounterName, -1) : -1;
                    spdlog::warn("[ZM38D15_SAVE] HOTSEAT_WRITE_RESULT player={} turn={} "
                                 "name={} prior={:#x} next={:#x} write={} readback={:#x}",
                                 playerId, turn, scenarioCounterName,
                                 priorU, static_cast<unsigned>(next), saved,
                                 static_cast<unsigned>(readback));
                }
            }
        } else {
            spdlog::warn("[ZM38D17_SERVER] REJECTED_OR_FREE player={} turn={} "
                         "accepted={} spent={} (NO_INCREMENT; outcome classification "
                         "needs a rejected-cast gameplay test)",
                         playerId, turn, accepted, zm33SpentResources);
            spdlog::warn("[ZM33_SPELL] NOT_COUNTED player={} turn={} callback={} spent={} "
                         "(free/failed/unknown; AUDIT ONLY)",
                         playerId, turn, accepted, zm33SpentResources);
        }
    } else if (gameSettings().zmLimits.enabled) {
        spdlog::warn("[ZM33_SPELL] STATE_UNAVAILABLE player={} turn={} (AUDIT ONLY, NO BLOCK)",
                     playerId, turn);
    }
    // ETAP36: verify read-back of the stage33 writer, not a second mutation.
    // This is an audit and does not assert that a save/load round trip worked.
    if (zm36Mode && persistentSlotFound && zm33SpentResources && postMap) {
        const auto* checkVars = getScenarioVariables(postMap);
        const int checkValue = checkVars
            ? getScenarioVariableByName(checkVars, scenarioCounterName, -1) : -1;
        const auto packed = static_cast<std::uint32_t>(checkValue);
        const bool sameTurn = (packed >> 16) == static_cast<unsigned int>(turn);
        const unsigned int count = sameTurn ? (packed & 0xffffu) : 0u;
        spdlog::warn("[ZM36_PERSIST] READBACK player={} turn={} count={} slot={} "
                     "validTurn={} (still requires save/load test)",
                     playerId, turn, count, scenarioCounterName, sameTurn);
    }
    spdlog::warn("[ZM31_SPELL] END from={} playerId={} turn={} callbackResult={}",
                 idFrom, playerId, turn, accepted);
    return accepted;
}

static void zm31InstallSpellMessageObserver(game::CMidServerLogic* serverLogic)
{
    // Never install a combat/spell observer inside the scenario editor.
    if (!executableIsGame()) return;
    if (!serverLogic || !serverLogic->coreData || !serverLogic->coreData->netMsgEntryData
        || !*serverLogic->coreData->netMsgEntryData) {
        spdlog::warn("[ZM31_SPELL] no server message map");
        return;
    }

    auto* entries = *serverLogic->coreData->netMsgEntryData;
    int found = 0;
    for (auto& node : entries->entries) {
        // Fixed-size class name can be unterminated. Do not call strlen here.
        const std::string_view name{node.name, sizeof(node.name)};
        if (name.find("CSpellCastMsg") == std::string_view::npos) {
            continue;
        }
        ++found;
        auto* base = node.ptr.data;
        if (!base) {
            spdlog::error("[ZM31_SPELL] null message entry");
            continue;
        }
        auto* member = reinterpret_cast<game::CNetMsgMapEntry_member*>(base);
        if (member->callbackThisptr != serverLogic || !member->callback) {
            spdlog::error("[ZM31_SPELL] callback receiver mismatch; not patching");
            continue;
        }
        const auto observer =
            reinterpret_cast<game::CNetMsgMapEntry_member::Callback>(zm31CastSpellMessageObserved);
        if (member->callback == observer) {
            spdlog::warn("[ZM31_SPELL] observer already active");
            continue;
        }
        if (zm31OriginalCastSpellCallback && zm31OriginalCastSpellCallback != member->callback) {
            spdlog::error("[ZM31_SPELL] different original callback, skipping unsafe rewrite");
            continue;
        }
        zm31OriginalCastSpellCallback = member->callback;
        member->callback = observer;
        spdlog::warn("[ZM31_SPELL] INSTALLED on server-side CSpellCastMsg, quota={} (AUDIT ONLY)",
                     gameSettings().maxSpellsPerTurn);
        spdlog::warn("[ZM32_LIMITS] legacy spell={} permanent={} oneday={} "
                     "zm enabled={} spell={} permanent={} oneday={} (spell AUDIT ONLY)",
                     gameSettings().maxSpellsPerTurn,
                     gameSettings().maxPotionBoostsPerUnit,
                     gameSettings().maxPotionOnedayBoostsPerUnit,
                     gameSettings().zmLimits.enabled,
                     gameSettings().zmLimits.spellsPerTurn,
                     gameSettings().zmLimits.permanentPotionsPerUnit,
                     gameSettings().zmLimits.onedayPotionsPerUnit);
        spdlog::warn("[ZM36_PERSIST] config={} (opt-in, EXISTING slot only; "
                     "does not create variables and does not count free spells)",
                     gameSettings().zmLimits.testPersistedSpellBlock);
        spdlog::warn("[ZM34_SPELL] mode={} target={} "
                     "(test_session has NO save/load protection or in-game popup)",
                     gameSettings().zmLimits.testSessionSpellBlock
                         ? "TEST_SESSION" : "AUDIT_ONLY",
                     gameSettings().zmLimits.spellsPerTurn);
    }
    if (found != 1) {
        spdlog::warn("[ZM31_SPELL] unexpected matching entries={} (expected 1)", found);
    }
}

game::CMidServerLogic* __fastcall midServerLogicCtorHooked(game::CMidServerLogic* thisptr,
                                                           int /*%edx*/,
                                                           game::CMidServer* server,
                                                           bool multiplayerGame,
                                                           bool hotseatGame,
                                                           int a5,
                                                           int gameVersion)
{
    using namespace game;

    getOriginalFunctions().midServerLogicCtor(thisptr, server, multiplayerGame, hotseatGame, a5,
                                              gameVersion);

    auto netMsgEntryData{thisptr->coreData->netMsgEntryData};

    auto callback = (CNetMsgMapEntry_member::Callback)exchangeResourcesMsgHandler;
    auto entry{createNetMsgMapEntryExchangeResourcesMsg(thisptr, callback)};

    NetMsgApi::get().addEntry(netMsgEntryData, (CNetMsgMapEntry*)entry);
    zm35RegisterSession(thisptr, !multiplayerGame && !hotseatGame, hotseatGame);
    zm31InstallSpellMessageObserver(thisptr);
    return thisptr;
}

void __fastcall processZeroTurnHooked(game::CMidServerLogic* thisptr,
                                      int /*%edx*/,
                                      std::uint32_t playerNetId,
                                      int a3)
{
    using namespace game;

    auto objectMap{thisptr->coreData->objectMap};
    auto scenarioInfo = getScenarioInfo(objectMap);
    bool isTurnZero = (scenarioInfo->currentTurn == 0);

    getOriginalFunctions().processZeroTurn(thisptr, playerNetId, a3);


    if (!isTurnZero) {
        return;
    }
    
    static std::optional<sol::environment> env;
    static std::optional<sol::function> func;
    const std::filesystem::path path = scriptsFolder() / "turn.lua";

    if (!env && !func) {
        func = getScriptFunction(path, "processTurnZero", env, false, true);
    }

    if (func) {
        try {
            lua_State* L = env->lua_state();
            sol::object resultObj = (*func)();
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Failed to run processTurnZero: {}", e.what()));
        }
    }
}

void __fastcall createImportedLeaderHooked(game::CMidServerLogic* thisptr,
                                           int /*%edx*/,
                                           std::uint32_t playerNetId)
{
    using namespace game;

    auto objectMap{thisptr->coreData->objectMap};
    auto scenarioInfo = getScenarioInfo(objectMap);
    int turn = scenarioInfo->currentTurn;

    if (turn == 0 || turn == 1 && thisptr->currentPlayerIndex != -1) {
        getOriginalFunctions().createImportedLeader(thisptr, playerNetId);
    }
}
} // namespace hooks
