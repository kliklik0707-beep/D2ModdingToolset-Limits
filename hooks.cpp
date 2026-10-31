/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#include "hooks.h"
#include "stealitemhooks.h"
#include "stealmerchantiteminterf.h"
#include "aigiveitemsaction.h"
#include "aigiveitemsactionhooks.h"
#include "attackimpl.h"
#include "attackreachcat.h"
#include "attackutils.h"
#include "autodialog.h"
#include "autodialoghooks.h"
#include "batattackbestowwards.h"
#include "batattackdamage.h"
#include "batattackdefend.h"
#include "batattackrevive.h"
#include "batattackdoppelganger.h"
#include "batattackdrain.h"
#include "batattackdrainlevel.h"
#include "batattackdrainoverflow.h"
#include "batattackgiveattack.h"
#include "batattackgroupupgrade.h"
#include "batattackheal.h"
#include "batattackparalyze.h"
#include "batattackretreat.h"
#include "batattackpetrify.h"
#include "batattackhealhooks.h"
#include "batattackshatter.h"
#include "batattacksummon.h"
#include "batattacktransformother.h"
#include "batattacktransformself.h"
#include "batattackuntransformeffect.h"
#include "batattackusepotion.h"
#include "batattackwait.h"
#include "batattackutils.h"
#include "bagdroptombhooks.h"
#include "batbigface.h"
#include "batlogic.h"
#include "batlogichooks.h"
#include "battleattackinfo.h"
#include "battlemsgdatahooks.h"
#include "battleviewerinterf.h"
#include "battleviewerinterfhooks.h"
#include "bestowwardshooks.h"
#include "giveattackhooks.h"
#include "cureattackhooks.h"
#include "boostdamagehooks.h"
#include "buildingbranch.h"
#include "buildstructinterf.h"
#include "buildstructinterfhooks.h"
#include "buildingtype.h"
#include "button.h"
#include "citystackinterf.h"
#include "citystackinterfhooks.h"
#include "cmdbattlechooseactionmsg.h"
#include "cmdbattleendmsg.h"
#include "cmdbattleresultmsg.h"
#include "cmdbattlestartmsg.h"
#include "commandmsghooks.h"
#include "condinterf.h"
#include "condinterfhooks.h"
#include "customattackhooks.h"
#include "customattacks.h"
#include "customattackutils.h"
#include "custombuildingcategories.h"
#include "customnobleactioncategories.h"
#include "customnobleactionhooks.h"
#include "d2string.h"
#include "dbfaccess.h"
#include "dbtable.h"
#include "dialoginterf.h"
#include "difficultylevel.h"
#include "displayhandlershooks.h"
#include "doppelgangerhooks.h"
#include "drainattackhooks.h"
#include "drainlevelhooks.h"
#include "dynamiccast.h"
#include "dynupgrade.h"
#include "editboxinterf.h"
#include "editboxinterfhooks.h"
#include "editor.h"
#include "effectinterfhooks.h"
#include "effectresulthooks.h"
#include "enclayoutcityhooks.h"
#include "enclayoutruinhooks.h"
#include "enclayoutstack.h"
#include "enclayoutstackhooks.h"
#include "enclayoutunithooks.h"
#include "encparambase.h"
#include "enrollunitinterf.h"
#include "enrollunitinterfhooks.h"
#include "encparambasehooks.h"
#include "eventconditioncathooks.h"
#include "eventeffectcathooks.h"
#include "exchangeinterf.h"
#include "exchangeinterfhooks.h"
#include "fonts.h"
#include "fontshooks.h"
#include "fortcategory.h"
#include "fortification.h"
#include "gameutils.h"
#include "globaldata.h"
#include "globalvariableshooks.h"
#include "groupupgradehooks.h"
#include "idlist.h"
#include "interfmanager.h"
#include "interftexthooks.h"
#include "intvector.h"
#include "isoenginegroundhooks.h"
#include "isoview.h"
#include "isoviewhooks.h"
#include "itembase.h"
#include "itemcategory.h"
#include "itemexpotionboost.h"
#include "itemtransferhooks.h"
#include "itemutils.h"
#include "leaderabilitycat.h"
#include "leadersforhire.h"
#include "leadersforhirehooks.h"
#include "listbox.h"
#include "lordtype.h"
#include "mainview2.h"
#include "mainview2hooks.h"
#include "managestkinterf.h"
#include "managestkinterfhooks.h"
#include "mempool.h"
#include "menuload.h"
#include "menuloadhooks.h"
#include "menumain.h"
#include "menumainhooks.h"
#include "menunewskirmishhotseathooks.h"
#include "menunewskirmishmultihooks.h"
#include "menunewskirmishsingle.h"
#include "menunewskirmishsinglehooks.h"
#include "turnhooks.h"
#include "menuphasehooks.h"
#include "midautodlgimages.h"
#include "midautodlgimageshooks.h"
#include "midcommandqueue2hooks.h"
#include "middatacache.h"
#include "midevconditionhooks.h"
#include "mideveffecthooks.h"
#include "mideventhooks.h"
#include "midgard.h"
#include "midgardhooks.h"
#include "midgardid.h"
#include "strategicspell.h"
#include "midgardmsgbox.h"
#include "midgardscenariomap.h"
#include "midgardstreamenv.h"
#include "midmsgboxbuttonhandlerstd.h"
#include "midmusic.h"
#include "midobjectlock.h"
#include "midobjectlockhooks.h"
#include "midplayer.h"
#include "midscenvariables.h"
#include "midserverlogic.h"
#include "midserverlogichooks.h"
#include "midsite.h"
#include "midstack.h"
#include "midunitdescriptor.h"
#include "midunitdescriptorhooks.h"
#include "midunithooks.h"
#include "midvillage.h"
#include "modifgroup.h"
#include "modifgrouphooks.h"
#include "modifierutils.h"
#include "movepathhooks.h"
#include "musichooks.h"
#include "netmsghooks.h"
#include "netplayerinfo.h"
#include "netsingleplayer.h"
#include "netsingleplayerhooks.h"
#include "objectinterf.h"
#include "objectinterfhooks.h"
#include "originalfunctions.h"
#include "phasegame.h"
#include "MIDITEM.h"
#include "phasegamehooks.h"
#include "playerbuildings.h"
#include "playerincomehooks.h"
#include "racecategory.h"
#include "racetype.h"
#include "restrictions.h"
#include "scenariodata.h"
#include "scenariodataarray.h"
#include "scenarioinfo.h"
#include "scenedit.h"
#include "scenedithooks.h"
#include "scenpropinterfhooks.h"
#include "settings.h"
#include "currency.h"
#include "trainingcostapi.h"
#include "trainingcosthooks.h"
#include "usersettings.h"
#include "sitecategoryhooks.h"
#include "sitemerchantinterf.h"
#include "sitemerchantinterfhooks.h"
#include "smartptr.h"
#include "stackbattleactionmsg.h"
#include "stacktemplatecache.h"
#include "summonhooks.h"
#include "taskobjaddsite.h"
#include "taskobjaddsitehooks.h"
#include "taskobjprop.h"
#include "taskobjprophooks.h"
#include "testconditionhooks.h"
#include "testkillstackhooks.h"
#include "testleaderownitemhooks.h"
#include "testleadertozone.h"
#include "testleadertozonehooks.h"
#include "testownitemhooks.h"
#include "teststackexistshooks.h"
#include "textboxinterf.h"
#include "textids.h"
#include "transformotherhooks.h"
#include "transformselfhooks.h"
#include "umattack.h"
#include "umattackhooks.h"
#include "umunit.h"
#include "umunithooks.h"
#include "unitbranchcat.h"
#include "unitgenerator.h"
#include "unitmodifier.h"
#include "unitmodifierhooks.h"
#include "unitsforhirehooks.h"
#include "unitstovalidate.h"
#include "unitutils.h"
#include "untransformeffecthooks.h"
#include "usracialsoldier.h"
#include "ussoldier.h"
#include "usstackleader.h"
#include "usunitimpl.h"
#include "utils.h"
#include "version.h"
#include "visitorcreatesite.h"
#include "visitorcreatesitehooks.h"
#include "visitors.h"
#include "reviveattackhooks.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iterator>
#include <spdlog/spdlog.h>
#include <string>

#include "battlemsgdataview.h"
#include <sol/sol.hpp>
#include <scripts.h>
#include <batviewer.h>
#include "groupview.h"
#include "midgardobjectmap.h"

#include "batattackblister.h"
#include "batattackblistereffect.h"
#include "batattackfrostbite.h"
#include "batattackfrostbiteeffect.h"
#include "batattackpoison.h"
#include "batattackpoisoneffect.h"
#include "batattacklowerdamage.h"
#include "batattacklowerinitiative.h"
#include "batattackboostdamage.h"
#include "batattackcure.h"
#include "dotattackhooks.h"
#include "batunitanim.h"
#include <midbag.h>

#include <midruin.h>
#include <midgardplan.h>
#include "quicksavehook.h"
#include "stealspellinterf.h"
#include "addstealspellhooks.h"
#include <attackview.h>
#include <attackparams.h>
#include <batattackfear.h>

#include "itempotionhooks.h"
#include "itempotionheal.h"
#include "idvector.h"
#include <batattackusetalisman.h>
#include <batattackuseorb.h>


struct PendingBattleEffect
{
    game::CMidgardID unitId;
    game::AttackEffect effect;
};
static std::vector<PendingBattleEffect> g_pendingBattleEffects;
//static bool g_hasPendingEffect{false};
//static PendingBattleEffect g_pendingEffect{};

namespace hooks {

/** Hooks that used only in game. */
static Hooks getGameHooks()
{
    using namespace game;

    auto& fn = gameFunctions();
    auto& battle = BattleMsgDataApi::get();
    auto& orig = getOriginalFunctions();
    auto& serverLogic{CMidServerLogicApi::get()};
    auto& eventConditions{ITestConditionApi::get()};

    // clang-format off
    Hooks hooks{
        // Quick save and open-selected-object hotkeys on the strategic map
        {(void*)fn.stratInterfKeyHandler, hookedKeyHandler, (void**)&originalKeyHandler},
        // Fix game crash in battles with summoners
        {CMidUnitApi::get().removeModifier, removeModifierHooked},
        // Fix unit transformation to include hp mods into current hp recalculation
        {CMidUnitApi::get().transform, transformHooked},
        // Show buildings with custom branch category on the 'other buildings' tab
        {CBuildingBranchApi::get().constructor, buildingBranchCtorHooked},
        {CBuildStructInterfApi::get().updateBuildingInfo,
         buildStructInterfUpdateBuildingInfoHooked,
         (void**)&orig.buildStructInterfUpdateBuildingInfo},
        {CBuildStructInterfApi::get().updateBuildingPopup,
         buildStructInterfUpdateBuildingPopupHooked,
         (void**)&orig.buildStructInterfUpdateBuildingPopup},
        // Allow alchemists to buff retreating units
        {CBatAttackGiveAttackApi::vftable()->canPerform, giveAttackCanPerformHooked},
        // Prevent rewrite turnsOrder
        {CBatAttackGiveAttackApi::vftable()->onHit, giveAttackOnHitHooked},
        // Random scenario generator
        {CMenuNewSkirmishSingleApi::get().constructor, menuNewSkirmishSingleCtorHooked, (void**)&orig.menuNewSkirmishSingleCtor},
        {CMenuNewSkirmishHotseatApi::get().constructor, menuNewSkirmishHotseatCtorHooked, (void**)&orig.menuNewSkirmishHotseatCtor},
        {CMenuNewSkirmishMultiApi::get().constructor, menuNewSkirmishMultiCtorHooked, (void**)&orig.menuNewSkirmishMultiCtor},
        // Random scenario generator templates
        {CMenuPhaseApi::get().constructor, menuPhaseCtorHooked, (void**)&orig.menuPhaseCtor},
        {CMenuPhaseApi::vftable()->destructor, menuPhaseDtorHooked, (void**)&orig.menuPhaseDtor},
        // Support custom battle attack objects
        {fn.createBatAttack, createBatAttackHooked, (void**)&orig.createBatAttack},
        // Support immunity bitmask in BattleMsgData
        {fn.getAttackClassWardFlagPosition, getAttackClassWardFlagPositionHooked, (void**)&orig.getAttackClassWardFlagPosition},
        // Support custom attack animations?
        {fn.attackClassToString, attackClassToStringHooked, (void**)&orig.attackClassToString},
        // Add items transfer buttons to city interface
        {CCityStackInterfApi::get().constructor, cityStackInterfCtorHooked, (void**)&orig.cityStackInterfCtor},
        // Add items transfer buttons to stack exchange interface
        {CExchangeInterfApi::get().constructor, exchangeInterfCtorHooked, (void**)&orig.exchangeInterfCtor},
        // Add items transfer buttons to pickup drop interface
        {CPickUpDropInterfApi::get().constructor, pickupDropInterfCtorHooked, (void**)&orig.pickupDropInterfCtor},
        // Add sell all valuables button to merchant interface
        {CSiteMerchantInterfApi::get().constructor, siteMerchantInterfCtorHooked, (void**)&orig.siteMerchantInterfCtor},
        // Cities can generate daily income depending on scenario variable settings
        {fn.computePlayerDailyIncome, computePlayerDailyIncomeHooked, (void**)&orig.computePlayerDailyIncome},
        // Vampiric attacks can deal critical damage
        {CBatAttackDrainApi::vftable()->onHit, drainAttackOnHitHooked},
        {CBatAttackDrainOverflowApi::vftable()->onHit, drainOverflowAttackOnHitHooked},
        // Support additional music tracks for battle and capital cities
        {CMidMusicApi::get().playBattleTrack, playBattleTrackHooked},
        {CMidMusicApi::get().playCapitalTrack, playCapitalTrackHooked},
        // Fix game crash with pathfinding on 144x144 maps
        {fn.markMapPosition, markMapPositionHooked, (void**)&orig.markMapPosition},
        // Allow user to tweak power computations
        {fn.getAttackPower, getAttackPowerHooked},
        // Fix game crash when AI controlled unit with transform self attack
        // uses alternative attack with 'adjacent' attack range
        // Fix incorrect calculation of effective HP used by AI for target prioritization
        {fn.computeUnitEffectiveHpForAi, computeUnitEffectiveHpForAiHooked},
        // Allow transform-self attack to not consume a unit turn for transformation
        // Fixes modifiers becoming permanent after modified unit is transformed
        // Support custom attack damage ratios
        {battle.beforeBattleTurn, beforeBattleTurnHooked},
        // Fix free transform-self to properly reset if the same unit has consequent turns in consequent battles
        {battle.beforeBattleRound, beforeBattleRoundHooked, (void**)&orig.beforeBattleRound},
        /**
         * Allows bestow wards to:
         * 1) Grant modifiers even if there are no source wards among them
         * 2) Heal its targets to the ammount specified in QTY_HEAL
         * 3) Heal retreating units
         * 4) Use Revive as a secondary attack
         * 5) Target a unit with a secondary attack even if there are no modifiers that can be
         * applied to this unit
         * 6) Treat modifiers with complete immunity correctly
         */
        {CBatAttackBestowWardsApi::vftable()->canPerform, bestowWardsAttackCanPerformHooked},
        /**
         * Fix bestow wards:
         * 1) Becoming permanent when more than 8 modifiers are applied at once
         * 2) Not resetting attack class wards (when reapplied)
         * 3) Incorrectly resetting attack source ward if its modifier also contains hp, regen or armor element
         */
        // Fixes modifiers getting lost after modified unit is untransformed
        {CBatAttackBestowWardsApi::vftable()->onHit, bestowWardsAttackOnHitHooked},
        // Fix bestow wards with double attack where modifiers granted by first attack are removed
        {battle.afterBattleTurn, afterBattleTurnHooked},
        // Allow any attack with QTY_HEAL > 0 to heal units when battle ends (just like ordinary heal does)
        {fn.getUnitHealAttackNumber, getUnitHealAttackNumberHooked},
        // Fix AI not being able to find target for lower damage/ini attack
        // Fix incorrect AI prioritization of shatter attack targets
        {battle.findAttackTarget, findAttackTargetHooked, (void**)&orig.findAttackTarget},
        // Support custom attack sources
        {fn.getUnitAttackSourceImmunities, getUnitAttackSourceImmunitiesHooked},
        {battle.isUnitAttackSourceWardRemoved, isUnitAttackSourceWardRemovedHooked},
        {battle.removeUnitAttackSourceWard, removeUnitAttackSourceWardHooked},
        {battle.addUnitToBattleMsgData, addUnitToBattleMsgDataHooked, (void**)&orig.addUnitToBattleMsgData},
        // Support custom attack reaches
        {battle.fillTargetsList, fillTargetsListHooked},
        {battle.fillEmptyTargetsList, fillEmptyTargetsListHooked},
        {battle.getTargetsToAttack, getTargetsToAttackHooked},
        {battle.findDoppelgangerAttackTarget, findDoppelgangerAttackTargetHooked},
        {battle.findDamageAttackTargetWithNonAllReach, findDamageAttackTargetWithNonAllReachHooked},
        {BattleViewerInterfApi::get().markAttackTargets, markAttackTargetsHooked},
        {fn.isGroupSuitableForAiNobleMisfit, isGroupSuitableForAiNobleMisfitHooked},
        {fn.isUnitSuitableForAiNobleDuel, isUnitSuitableForAiNobleDuelHooked},
        {fn.isAttackBetterThanItemUsage, isAttackBetterThanItemUsageHooked},
        {fn.getSummonUnitImplIdByAttack, getSummonUnitImplIdByAttackHooked},
        {fn.isSmallMeleeFighter, isSmallMeleeFighterHooked},
        {fn.cAiHireUnitEval, cAiHireUnitEvalHooked},
        {fn.getMeleeUnitToHireAiRating, getMeleeUnitToHireAiRatingHooked},
        {fn.computeTargetUnitAiPriority, computeTargetUnitAiPriorityHooked},
        // Allow users to specify critical hit chance
        // Support custom attack damage ratios
        {fn.computeDamage, computeDamageHooked},
        // Fix occasional crash with incorrect removal of summoned unit info
        // Fix persistent crash with summons when unrestrictedBestowWards is enabled
        {battle.removeUnitInfo, removeUnitInfoHooked},
        // Fix bug where transform-self attack is unable to target self if alt attack is targeting allies
        {CBatAttackTransformSelfApi::vftable()->fillTargetsList, transformSelfAttackFillTargetsListHooked},
        // Allow transform self into leveled units using script logic
        // Allow transform-self attack to not consume a unit turn for transformation
        // Fix bug where transform-self attack is unable to target self if alt attack is targeting allies
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackTransformSelfApi::vftable()->onHit, transformSelfAttackOnHitHooked},
        // Allow transform other into leveled units using script logic
        // Fix bug where transform-other attack selects melee vs ranged transform based on attacker position rather than target position
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackTransformOtherApi::vftable()->onHit, transformOtherAttackOnHitHooked},
        // Allow to drain different number of levels using script logic
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackDrainLevelApi::vftable()->onHit, drainLevelAttackOnHitHooked},
        // Fix possible attack count mismatch (once vs twice) on unit transformation
        {CBatAttackUntransformEffectApi::vftable()->onHit, untransformEffectAttackOnHitHooked},
        // Fix inability to target self for transformation in case of transform-self + summon attack
        // Remove persistent marking of all target units in case of transform-self attack
        {BattleViewerInterfApi::vftable()->update, battleViewerInterfUpdateHooked},
        // Fix missing modifiers of alternative attacks
        {fn.getUnitAttacks, getUnitAttacksHooked},
        // Support custom event conditions
        {eventConditions.create, createTestConditionHooked, (void**)&orig.createTestCondition},
        // Support custom event effects
        //{IEffectResultApi::get().create, createEffectResultHooked, (void**)&orig.createEffectResult},
        // Support additional as well as high tier units in hire list
        {fn.addPlayerUnitsToHireList, addPlayerUnitsToHireListHooked},
        {fn.shouldAddUnitToHire, shouldAddUnitToHireHooked},
        {fn.enableUnitInHireListUi, enableUnitInHireListUiHooked},
        // Fix sideshow always display in enroll UI
        {fn.addSideshowUnitToUI, addSideshowUnitToUIHooked},
        // Allow scenarios with prebuilt buildings in capitals
        // Start with prebuilt temple in capital for warrior lord depending on user setting
        {fn.buildLordSpecificBuildings, buildLordSpecificBuildingsHooked},
        // Allows transformed leaders (doppelganger, drain-level, transform-self/other attacks) to use battle items (potions, orbs and talismans)
        {BattleViewerInterfApi::get().updateBattleItems, battleViewerInterfUpdateBattleItemsHooked},
        {BatBigFaceApi::get().update, batBigFaceUpdateHooked},
        {battle.updateBattleActions, updateBattleActionsHooked},
        // Support race-specific village graphics
        {GameImagesApi::get().getCityPreviewLargeImageNames, getCityPreviewLargeImageNamesHooked, (void**)&orig.getCityPreviewLargeImageNames},
        {GameImagesApi::get().getCityIconImageNames, getCityIconImageNamesHooked, (void**)&orig.getCityIconImageNames},
        // Support grid toggle button
        {CMainView2Api::get().showIsoDialog, mainView2ShowIsoDialogHooked},
        // Reference ground rendering implementation
        // TODO: fix occasional magenta 'triangles' showing up after closing capital window
        //{CGroundTextureApi::vftable()->draw, groundTextureDrawHooked},
        //{CGroundTextureApi::isoEngineVftable()->render, isoEngineGroundRenderHooked},
        // Support native modifiers
        {CMidUnitApi::get().upgrade, upgradeHooked},
        // Fix doppelganger attack using alternative attack when attacker is transformed (by doppelganger, drain-level, transform-self/other attacks)
        {battle.cannotUseDoppelgangerAttack, cannotUseDoppelgangerAttackHooked},
        // Support new menu windows
        {CMenuPhaseApi::get().switchPhase, menuPhaseSwitchPhaseHooked},
        // Support custom modifiers
        {fn.loadScenarioMap, loadScenarioMapHooked, (void**)&orig.loadScenarioMap},
        // Show broken (removed) wards in unit encyclopedia
        {CEncParamBaseApi::get().addUnitBattleInfo, encParamBaseAddUnitBattleInfoHooked},
        // Fix crash on drag&drop when INotify::OnObjectChanged is processed between mouse down and up
        {CManageStkInterfApi::vftable().notify->onObjectChanged, manageStkInterfOnObjectChangedHooked, (void**)&orig.manageStkInterfOnObjectChanged},
        {CExchangeInterfApi::vftable().notify->onObjectChanged, exchangeInterfOnObjectChangedHooked, (void**)&orig.exchangeInterfOnObjectChanged},
        {CCityStackInterfApi::vftable().notify->onObjectChanged, cityStackInterfOnObjectChangedHooked, (void**)&orig.cityStackInterfOnObjectChanged},
        {CSiteMerchantInterfApi::vftable().notify->onObjectChanged, siteMerchantInterfOnObjectChangedHooked, (void**)&orig.siteMerchantInterfOnObjectChanged},
        // Fix inability to use heal potion on transformed unit if its current hp is greater than maximum hp of unit it is transformed to
        // (most common case is a unit transformed to Imp by a Witch while retaining his original hp)
        {fn.canApplyPotionToUnit, canApplyPotionToUnitHooked},
        // Fix crash on AI turn when it tries to exchange items and a source stack is destroyed in battle/event while moving to destination
        {CAiGiveItemsActionApi::vftable().action->execute, aiGiveItemsActionExecuteHooked},
        // Allow foreign race units to upgrade even if its race capital is present in scenario (functions as if the unit type is locked)
        // Allow foreign race units (including neutral) to be upgraded using capital buildings
        // Fix errornous logic that allowed retreated units to upgrade under certain conditions (introduce setting battle.allowRetreatedUnitsToUpgrade)
        {CBatAttackGroupUpgradeApi::get().upgradeGroup, upgradeGroupHooked},
        {fn.getUpgradeUnitImplCheckXp, getUpgradeUnitImplCheckXpHooked},
        {fn.changeUnitXpCheckUpgrade, changeUnitXpCheckUpgradeHooked},
        // Allow player to customize movement cost
        {fn.computeMovementCost, computeMovementCostHooked},
        // Support custom lobby
        {CMidgardApi::get().startMenuMessageCallback, midgardStartMenuMessageCallbackHooked},
        {CMenuPhaseApi::get().transitionToMainOrCloseGame, menuPhaseTransitionToMainOrCloseGameHooked, (void**)&orig.menuPhaseTransitionToMainOrCloseGame},
        {CMenuMainApi::get().createMenu, menuMainCreateMenuHooked},
        {CMenuLoadApi::get().createServer, menuLoadCreateServerHooked, (void**)&orig.menuLoadCreateServer},
        {AutoDialogApi::get().loadAndParseScriptFile, autoDialogLoadAndParseScriptFileHooked, (void**)&orig.autoDialogLoadAndParseScriptFile},
        {MidAutoDlgImagesApi::vftable()->loadImage, midAutoDlgImagesLoadImageHooked},
        // Support custom scripts for AI battle actions
        {battle.aiChooseBattleAction, aiChooseBattleActionHooked, (void**)&orig.aiChooseBattleAction},
        // Profile and speed up events system
        {serverLogic.applyEventEffectsAndCheckMidEventTriggerers, applyEventEffectsAndCheckMidEventTriggerersHooked, (void**)&orig.applyEventEffectsAndCheckMidEventTriggerers},
        {serverLogic.filterAndProcessEventsNoPlayer, filterAndProcessEventsNoPlayerHooked, (void**)&orig.filterAndProcessEventsNoPlayer},
        {serverLogic.filterAndProcessEvents, filterAndProcessEventsHooked, (void**)&orig.filterAndProcessEvents},
        {serverLogic.checkEventConditions, checkEventConditionsHooked, (void**)&orig.checkEventConditions},
        {serverLogic.executeEventEffects, executeEventEffectsHooked, (void**)&orig.executeEventEffects},
        {eventConditions.testFrequency, testFreqHooked, (void**)&orig.testFrequency},
        {eventConditions.testLocation, testLocationHooked, (void**)&orig.testLocation},
        {eventConditions.testLeaderToZone, testLeaderToZoneHooked},
        {eventConditions.testEnterCity, testEnterCityHooked, (void**)&orig.testEnterCity},
        {eventConditions.testLeaderToCity, testLeaderToCityHooked, (void**)&orig.testLeaderToCity},
        {eventConditions.testOwnCity, testOwnCityHooked, (void**)&orig.testOwnCity},
        {eventConditions.testKillStack, testKillStackHooked},
        {eventConditions.testOwnItem, testOwnItemHooked},
        {eventConditions.testLeaderOwnItem, testLeaderOwnItemHooked},
        {eventConditions.testDiplomacy, testDiplomacyHooked, (void**)&orig.testDiplomacy},
        {eventConditions.testAlliance, testAllianceHooked, (void**)&orig.testAlliance},
        {eventConditions.testLootRuin, testLootRuinHooked, (void**)&orig.testLootRuin},
        {eventConditions.testTransformLand, testTransformLandHooked, (void**)&orig.testTransformLand},
        {eventConditions.testVisitSite, testVisitSiteHooked, (void**)&orig.testVisitSite},
        {eventConditions.testItemToLocation, testItemToLocationHooked, (void**)&orig.testItemToLocation},
        {eventConditions.testStackExists, testStackExistsHooked},
        {eventConditions.testVarInRange, testVarInRangeHooked, (void**)&orig.testVarInRange},
        {fn.removeStack, removeStackHooked, (void**)&orig.removeStack},
        {VisitorApi::get().setStackSrcTemplate, setStackSrcTemplateHooked, (void**)&orig.setStackSrcTemplate},
        // Support resource market site
        {CMainView2Api::get().handleCmdStackVisitMsg, mainView2HandleCmdStackVisitMsgHooked, (void**)&orig.handleCmdStackVisitMsg},
        {CMidServerLogicApi::get().constructor, midServerLogicCtorHooked, (void**)&orig.midServerLogicCtor},
        {fn.getSiteSound, getSiteSoundHooked},
        {fn.siteHasSound, siteHasSoundHooked},
        // Support custom noble actions
        {NobleActionsApi::get().create, createNobleActionResultHooked, (void**)&orig.createNobleActionResult},
        {fn.getSiteNobleActions, getSiteNobleActionsHooked, (void**)&orig.getSiteNobleActions},
        {fn.getPossibleNobleActions, getPossibleNobleActionsHooked, (void**)&orig.getPossibleNobleActions},
        {fn.getNobleActionResultDescription, getNobleActionResultDescriptionHooked, (void**)&orig.getNobleActionResultDescription},
        // Fix network desync issues
        {CMidCommandQueue2Api::get().push, midCommandQueue2PushHooked, (void**)&orig.midCommandQueue2Push},
        {CMidCommandQueue2Api::get().netMsgMapConstructor, netMsgMapConstructorHooked, (void**)&orig.netMsgMapConstructor},
        {CCommandMsgApi::get().create, commandMsgCreateHooked, (void**)&orig.commandMsgCreate},
        {CMainView2Api::get().commandQueueCallback, mainView2CommandQueueCallbackHooked, (void**)&orig.mainView2CommandQueueCallback},
        {CMidObjectLockApi::get().constructor, midObjectLockCtorHooked, (void**)&orig.midObjectLockCtor},
        {CMidObjectLockApi::get().notify1Callback, midObjectLockNotify1CallbackHooked},
        {CMidObjectLockApi::get().notify2Callback, midObjectLockNotify2CallbackHooked},
        {CMidObjectLockApi::vftable()->onObjectChanged, midObjectLockOnObjectChangedHooked},
        {CPhaseGameApi::get().checkObjectLock, phaseGameCheckObjectLockHooked},
        {CPhaseGameApi::get().sendStackMoveMsg, phaseGameSendStackMoveMsgHooked},
        {serverLogic.stackMove, stackMoveHooked, (void**)&orig.stackMove},
        {CMidgardApi::get().clearNetworkState, midgardClearNetworkStateHooked, (void**)&orig.midgardClearNetworkState},
        {CMidgardApi::get().clearNetworkStateAndService, midgardClearNetworkStateAndServiceHooked, (void**)&orig.midgardClearNetworkStateAndService},
        // Correctly untransformation at the end of battle for extracted units
        {CBatLogicApi::get().updateGroupsIfBattleIsOver, updateGroupsIfBattleIsOverHooked},
        // Fixed an issue where a unit killed by a DoT effect was considered alive until end next action
        {CBatLogicApi::get().battleTurn, battleTurnHooked, (void**)&orig.battleTurn},
        // Prevent crash when defending side selects empty position as target
        {CBatAttackHealApi::vftable()->canPerform, healAttackCanPerformHooked},
        // On heal can affect boost and lower damage
        {CBatAttackHealApi::vftable()->onHit, healAttackOnHitHooked},
        //Hooks for status effects.
        {battle.setUnitStatus, setUnitStatusHooked, (void**)&orig.setUnitStatus},

        // Allow modify leaders hire list with lua
        {LeadersForHireApi::get().getLeadersHireList, getLeadersHireListHooked},
        {LeadersForHireApi::get().addNobleLeaderToUI, addNobleLeaderToUIHooked},
        {LeadersForHireApi::get().changeStackLeaderInCapital, changeStackLeaderInCapitalHooked, (void**)&orig.changeStackLeaderInCapital},
        {EnrollUnitInterfApi::get().constructor, enrollUnitInterfCtorHooked, (void**)&orig.enrollUnitInterfCtor},
        // Modify map before game starts
        {CMidServerLogicApi::get().processZeroTurn, processZeroTurnHooked, (void**)&orig.processZeroTurn},
        {CMidServerLogicApi::get().createImportedLeader, createImportedLeaderHooked, (void**)&orig.createImportedLeader},
        // SHOWATTACKEFFFECT, requestBattleEffect doen't work correctly. Don't use.
        {BattleViewerInterfApi::vftable()->showAttackEffect, showAttackEffectHooked, (void**)&orig.showAttackEffect},
        //Fixed an issue where a unit with an attack of 3 or more would incorrectly reset the attackCount after using a block.
        {CBatAttackDefendApi::vftable()->onHit, defendOnHitHooked},
        {CBatAttackReviveApi::vftable()->onHit, reviveAttackOnHitHooked},

        // Controlable random for long effects
        {battle.checkLongEffectDuration, checkLongEffectDurationHooked, (void**)&orig.checkLongEffectDuration},

        //Cure attack always work
        {CBatAttackCureApi::vftable()->canPerform, cureAttackCanPerformHooked},
        {CBatAttackCureApi::vftable()->onHit, cureAttackOnHitHooked},

        {CBatAttackDamageApi::vftable()->onHit, damageAttackOnHitHooked},
        {CBatAttackLowerDamageApi::vftable()->onHit, lowerDamageOnHitHooked},
        {CBatAttackLowerInitiativeApi::vftable()->onHit, lowerInitiativeOnHitHooked},

        {CBatAttackParalyzeApi::vftable()->onHit, paralyzeOnHitHooked},
        {CBatAttackPetrifyApi::vftable()->onHit, petrifyOnHitHooked},

        {CBatAttackFearApi::vftable()->onHit, fearAttackOnHitHooked},

        {battle.setUnitFlag5, setUnitFlag5Hooked},

        // WIP
        {battle.updateParalyzePetrifyEffects, updateParalyzePetrifyEffectsHooked},
        {battle.updatePoisonLongEffect, shouldDisablePoisonHooked},
        {battle.updateFrostbiteLongEffect, shouldDisableFrostbiteHooked},
        {battle.updateBlisterLongEffect, shouldDisableBlisterHooked},
        {battle.updateTransformLongEffect, shouldDisableTransformOtherHooked},

        // WIP
        //{CItemBaseApi::get().constructor, itemBaseCtorHooked, (void**)&orig.itemBaseCtor},

        // Get fog spell area image
        {GameImagesApi::get().getSpellAreaFogImage, getSpellAreaFogImageHooked, (void**)&orig.getSpellAreaFogImage},

        // Quick save hotkey for host in multiplayer
        {(void*)fn.stratInterfKeyHandler, hookedKeyHandler, (void**)&originalKeyHandler},

        // Allow drop loot into grave
        {CMidgardPlanApi::get().getObjectsAtPoint, getObjectsAtPointHooked, (void**)&orig.getObjectsAtPoint},

        //Not used
        {BattleViewerInterfApi::vftable()->battleEnd, battleEndHooked, (void**)&orig.battleEnd},
        {battle.decreaseUnitAttacks, decreaseUnitAttacksHooked, (void**)&orig.decreaseUnitAttacks},
        {CBatLogicApi::get().applyCBatAttackUntransformEffect, applyCBatAttackUntransformEffectHooked, (void**)&orig.applyCBatAttackUntransformEffect},
    };
    // clang-format on

    if (gameSettings().trainerCampLowerCost) {
        if (BankApi::get().copy) {
            hooks.emplace_back(
                HookInfo{(void*)BankApi::get().copy, bankCopyHooked, (void**)&orig.bankCopy});
        }
        const auto& trainApi = TrainingCostApi::get();
        if (BankApi::get().copyCtor && trainApi.costCopyReturnTrainUnit) {
            hooks.emplace_back(HookInfo{(void*)BankApi::get().copyCtor, bankCopyCtorHooked,
                                        (void**)&orig.bankCopyCtor});
        }
        if (BankApi::get().subtract) {
            hooks.emplace_back(HookInfo{(void*)BankApi::get().subtract, bankSubtractHooked,
                                        (void**)&orig.bankSubtract});
        }
        if (trainApi.addExperience && trainApi.expReturnTrainUnit) {
            hooks.emplace_back(HookInfo{(void*)trainApi.addExperience, addExperienceHooked,
                                        (void**)&orig.addExperience});
        }
        if (trainApi.trainUnitAtTrainer) {
            hooks.emplace_back(HookInfo{(void*)trainApi.trainUnitAtTrainer, trainUnitAtTrainerHooked,
                                        (void**)&orig.trainUnitAtTrainer});
        }
        if (trainApi.trainUiAction) {
            hooks.emplace_back(HookInfo{(void*)trainApi.trainUiAction, trainUiActionHooked,
                                        (void**)&orig.trainUiAction});
        }
        if (trainApi.canAffordTrainCheck) {
            hooks.emplace_back(HookInfo{(void*)trainApi.canAffordTrainCheck,
                                        canAffordTrainCheckHooked,
                                        (void**)&orig.canAffordTrainCheck});
        }
        if (trainApi.applyTrainAction) {
            hooks.emplace_back(HookInfo{(void*)trainApi.applyTrainAction, applyTrainActionHooked,
                                        (void**)&orig.applyTrainAction});
        }
        const auto& textApi = TrainCampTextApi::get();
        if (textApi.setPartyTrainingText) {
            hooks.emplace_back(HookInfo{(void*)textApi.setPartyTrainingText, trainUiTextHooked,
                                        (void**)&orig.setPartyTrainingText});
        }
        const auto& textBoxApi = CTextBoxInterfApi::get();
        if (textBoxApi.setString) {
            hooks.emplace_back(HookInfo{(void*)textBoxApi.setString, textBoxSetStringHooked,
                                        (void**)&orig.textBoxSetString});
        }
        const auto& dragDropApi = CMidDragDropInterfApi::get();
        if (dragDropApi.destructor) {
            hooks.emplace_back(HookInfo{(void*)dragDropApi.destructor, midDragDropInterfDtorHooked,
                                        (void**)&orig.midDragDropInterfDtor});
        }

        spdlog::info("trainer hooks version={} train={:#x} ui={:#x} afford={:#x} apply={:#x} "
                     "text={:#x} textbox={:#x} exp={:#x} expret={:#x}",
                     static_cast<int>(hooks::gameVersion()),
                     reinterpret_cast<std::uintptr_t>(trainApi.trainUnitAtTrainer),
                     reinterpret_cast<std::uintptr_t>(trainApi.trainUiAction),
                     reinterpret_cast<std::uintptr_t>(trainApi.canAffordTrainCheck),
                     reinterpret_cast<std::uintptr_t>(trainApi.applyTrainAction),
                     reinterpret_cast<std::uintptr_t>(textApi.setPartyTrainingText),
                     reinterpret_cast<std::uintptr_t>(textBoxApi.setString),
                     reinterpret_cast<std::uintptr_t>(trainApi.addExperience),
                     reinterpret_cast<std::uintptr_t>(trainApi.expReturnTrainUnit));
        if (!trainApi.trainUiAction && !textApi.setPartyTrainingText) {
            spdlog::error("trainer camp discount has no addresses for game version {}",
                          static_cast<int>(hooks::gameVersion()));
        }
    }

    if (gameSettings().extendedBattle.boostdamageCanAffectHealer
        != baseGameSettings().extendedBattle.boostdamageCanAffectHealer) {
        // BoostDamage attack can target healers
        hooks.emplace_back( HookInfo{CBatAttackBoostDamageApi::vftable()->canPerform, boostDamageCanPerformHooked});
        hooks.emplace_back( HookInfo{CBatAttackBoostDamageApi::vftable()->onHit, boostDamageOnHitHooked});
    }

    if (gameSettings().extendedBattle.lowerdamageCanAffectHealer
        != baseGameSettings().extendedBattle.lowerdamageCanAffectHealer) {
        hooks.emplace_back( HookInfo{CBatAttackLowerDamageApi::vftable()->canPerform, lowerDamageCanPerformHooked});
    }

    if (gameSettings().extendedBattle.experimental
        != baseGameSettings().extendedBattle.experimental) {
        // clang-format off

        // onHit dot effects
        hooks.emplace_back(HookInfo{CBatAttackBlisterEffectApi::vftable()->onHit, blisterEffectOnHitHooked});
        hooks.emplace_back(HookInfo{CBatAttackFrostbiteEffectApi::vftable()->onHit, frostbiteEffectOnHitHooked});
        hooks.emplace_back( HookInfo{CBatAttackPoisonEffectApi::vftable()->onHit, poisonEffectOnHitHooked});
        // DOT-damages
        hooks.emplace_back(HookInfo{CBatAttackBlisterApi::vftable()->canPerform, blisterAttackCanPerformHooked});
        hooks.emplace_back(HookInfo{CBatAttackBlisterApi::vftable()->onHit, blisterAttackOnHitHooked});
        hooks.emplace_back(HookInfo{CBatAttackBlisterApi::vftable()->onHit, blisterAttackOnHitHooked});
        hooks.emplace_back(HookInfo{CBatAttackFrostbiteApi::vftable()->canPerform, frostbiteAttackCanPerformHooked});
        hooks.emplace_back(HookInfo{CBatAttackFrostbiteApi::vftable()->onHit, frostbiteAttackOnHitHooked});
        hooks.emplace_back(HookInfo{CBatAttackPoisonApi::vftable()->canPerform, poisonAttackCanPerformHooked});
        hooks.emplace_back(HookInfo{CBatAttackPoisonApi::vftable()->onHit, poisonAttackOnHitHooked});
        //
        hooks.emplace_back(HookInfo{CBatAttackWaitApi::vftable()->onHit, waitAttackOnHitHooked});
        hooks.emplace_back(HookInfo{CBatAttackRetreatApi::vftable()->onHit, retreatAttackOnHitHooked});
        hooks.emplace_back(HookInfo{battle.setUnitFlag5, setUnitFlag5Hooked});
        hooks.emplace_back(HookInfo{CItemPotionReviveApi::get().constructor, itemPotionReviveCtorHooked, (void**)&orig.itemPotionReviveCtor});
        hooks.emplace_back(HookInfo{CItemPotionBoostPermApi::get().constructor, itemPotionBoostPermCtorHooked, (void**)&orig.itemPotionBoostPermCtor});
        hooks.emplace_back(HookInfo{CItemPotionBoostTempApi::get().constructor, itemPotionBoostTempCtorHooked, (void**)&orig.itemPotionBoostTempCtor});
        hooks.emplace_back(HookInfo{CItemPotionHealApi::get().constructor, itemPotionHealCtorHooked, (void**)&orig.itemPotionHealCtor});
        hooks.emplace_back(HookInfo{CBatAttackUsePotionApi::vftable()->onHit, usePotionAttackOnHitHooked});
        hooks.emplace_back(HookInfo{CBatAttackUseTalismanApi::vftable()->onHit, useTalismanAttackOnHitHooked});
        hooks.emplace_back(HookInfo{CBatAttackUseOrbApi::vftable()->onHit, useOrbAttackOnHitHooked});
        // clang-format on
    }
    // End extended battle options

    // Advanced cure
    if (gameSettings().advancedCure != baseGameSettings().advancedCure) {
        hooks.emplace_back(
            HookInfo{battle.unitCanBeCured, unitCanBeCuredHooked, (void**)&orig.unitCanBeCured});
    }

    if (gameSettings().engine.sendRefreshInfoObjectCountLimit) {
        // Fix incomplete scenario loading when its object size exceed network message buffer size
        // of 512 KB
        hooks.emplace_back(HookInfo{CMidServerLogicApi::get().sendRefreshInfo,
                                    midServerLogicSendRefreshInfoHooked});
    }

    if (gameSettings().shatteredArmorMax != baseGameSettings().shatteredArmorMax) {
        // Allow users to customize total armor shatter damage
        hooks.emplace_back(
            HookInfo{CBatAttackShatterApi::vftable()->canPerform, shatterCanPerformHooked});
        hooks.emplace_back(HookInfo{battle.setUnitShatteredArmor, setUnitShatteredArmorHooked});
    }

    if (gameSettings().shatterDamageMax != baseGameSettings().shatterDamageMax) {
        // Allow users to customize maximum armor shatter damage per attack
        hooks.emplace_back(HookInfo{CBatAttackShatterApi::vftable()->onHit, shatterOnHitHooked});
    }

    if (userSettings().showBanners != baseUserSettings().showBanners) {
        // Allow users to show banners by default
        hooks.emplace_back(HookInfo{fn.toggleShowBannersInit, toggleShowBannersInitHooked});
    }

    if (userSettings().showResources != baseUserSettings().showResources
        || userSettings().showLandConverted != baseUserSettings().showLandConverted) {
        // Allow users to show resources panel by default
        hooks.emplace_back(HookInfo{fn.respopupInit, respopupInitHooked});
    }

    if (userSettings().carryOverItemsMax != baseUserSettings().carryOverItemsMax) {
        // Change maximum number of items that player can carry between campaign scenarios
        hooks.emplace_back(HookInfo{CDDCarryOverItemsApi::get().constructor,
                                    carryOverItemsCtorHooked, (void**)&orig.carryOverItemsCtor});
    }

    if (gameSettings().doppelgangerRespectsEnemyImmunity
            != baseGameSettings().doppelgangerRespectsEnemyImmunity
        || gameSettings().doppelgangerRespectsAllyImmunity
               != baseGameSettings().doppelgangerRespectsAllyImmunity) {
        // Make Doppelganger attack respect target source/class wards and immunities
        hooks.emplace_back(HookInfo{CBatAttackDoppelgangerApi::vftable()->canPerform,
                                    doppelgangerAttackCanPerformHooked});
        hooks.emplace_back(HookInfo{CBatAttackDoppelgangerApi::vftable()->isImmune,
                                    doppelgangerAttackIsImmuneHooked});
    }

    if (gameSettings().leveledDoppelgangerAttack != baseGameSettings().leveledDoppelgangerAttack) {
        // Allow doppelganger to transform into leveled units using script logic
        hooks.emplace_back(
            HookInfo{CBatAttackDoppelgangerApi::vftable()->onHit, doppelgangerAttackOnHitHooked});
    }

    if (gameSettings().leveledSummonAttack != baseGameSettings().leveledSummonAttack) {
        // Allow summon leveled units using script logic
        hooks.emplace_back(
            HookInfo{CBatAttackSummonApi::vftable()->onHit, summonAttackOnHitHooked});
    }

    if (gameSettings().missChanceSingleRoll != baseGameSettings().missChanceSingleRoll) {
        // Compute attack miss chance using single random value, instead of two
        hooks.emplace_back(HookInfo{fn.attackShouldMiss, attackShouldMissHooked});
    }

    if (gameSettings().unrestrictedBestowWards != baseGameSettings().unrestrictedBestowWards) {
        // Increases total ward limit for bestow-wards attack from 8 to 48
        // clang-format off
        hooks.emplace_back(HookInfo{battle.constructor, battleMsgDataCtorHooked, (void**)&orig.battleMsgDataCtor});
        hooks.emplace_back(HookInfo{battle.copyConstructor, battleMsgDataCopyCtorHooked});
        hooks.emplace_back(HookInfo{battle.copyConstructor2, battleMsgDataCopyCtorHooked});
        hooks.emplace_back(HookInfo{battle.copyAssignment, battleMsgDataCopyAssignHooked});
        hooks.emplace_back(HookInfo{battle.copy, battleMsgDataCopyHooked});
        hooks.emplace_back(HookInfo{battle.destructor, battleMsgDataDtorHooked});
        hooks.emplace_back(HookInfo{CStackBattleActionMsgApi::vftable()->serialize, stackBattleActionMsgSerializeHooked, (void**)&orig.stackBattleActionMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleStartMsgApi::vftable()->serialize, cmdBattleStartMsgSerializeHooked, (void**)&orig.cmdBattleStartMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleChooseActionMsgApi::vftable()->serialize, cmdBattleChooseActionMsgSerializeHooked, (void**)&orig.cmdBattleChooseActionMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleResultMsgApi::vftable()->serialize, cmdBattleResultMsgSerializeHooked, (void**)&orig.cmdBattleResultMsgSerialize});
        hooks.emplace_back(HookInfo{CCmdBattleEndMsgApi::vftable()->serialize, cmdBattleEndMsgSerializeHooked, (void**)&orig.cmdBattleEndMsgSerialize});
        hooks.emplace_back(HookInfo{CCommandMsgApi::get().destructor, commandMsgDtorHooked, (void**)&orig.commandMsgDtor});
        hooks.emplace_back(HookInfo{CNetMsgApi::get().destructor, netMsgDtorHooked, (void**)&orig.netMsgDtor});
        // clang-format on
    }

    if (userSettings().movementDisplay.show) {
        // Show movement cost
        hooks.emplace_back(HookInfo{fn.showMovementPath, showMovementPathHooked});
    }

    bool hookSendObjectsChanges = false;
    if (gameSettings().debugMode) {
        // clang-format off
        // Log all net messages being sent by single player (both client and server) to netMessages<PID>.log
        if (gameSettings().debug.logSinglePlayerMessages) {
            hooks.emplace_back(HookInfo{CNetSinglePlayerApi::vftable()->sendMessage, netSinglePlayerSendMessageHooked, (void**)&orig.netSinglePlayerSendMessage});
        }
        // Log added/changed/erased objects ids being sent by server to netMessages<PID>.log
        if (gameSettings().debug.sendObjectsChangesTreshold) {
            hookSendObjectsChanges = true;
        }
        // clang-format on
    }

    if (gameSettings().modifiers.validateUnitsOnGroupChanged) {
        // Validate current HP / XP of units when their group changes (units added, removed,
        // rearranged, etc.) to resolve issues with custom HP / XP modifiers, that depend on other
        // units (like auras in MNS mod).
        hooks.emplace_back(
            HookInfo{fn.getStackFortRuinGroupForChange, getStackFortRuinGroupForChangeHooked});
        hookSendObjectsChanges = true;
    }


    //
    // build spell list
    //
    hooks.emplace_back(HookInfo{(void*)game::StealSpellInterfApi::get().buildSpellList,

                                hooks::getBuildSpellListHooked(),

                                hooks::getBuildSpellListOrig()});

    //
    // filtered insertion
    //
    hooks.emplace_back(HookInfo{(void*)game::IdSetApi::get().insert,

                                hooks::getSortedIdListInsertHooked(),

                                hooks::getSortedIdListInsertOrig()});

    //
    // Steal spell
    //
    hooks.emplace_back(HookInfo{(void*)game::StealSpellInterfApi::get().constructor,

                                hooks::getStealSpellInterfCtorHooked(),

                                hooks::getStealSpellInterfCtorOrig()});




    //
    // Steal merchant items
    //

     hooks.emplace_back(HookInfo{(void*) game::StealMerchantItemInterfApi::get().constructor,
        hooks::getStealItemCtorHooked(),
        hooks::getStealItemCtorOrig()});

     //
     // Steal merchant items
     //
     hooks.emplace_back(HookInfo{(void*) game::StealMerchantItemInterfApi::get().addStealItem,
            hooks::getAddStealItemHooked(),
            hooks::getAddStealItemOrig()});
      

    if (hookSendObjectsChanges) {
        hooks.emplace_back(HookInfo{CMidServerLogicApi::vftable().midMsgSender->sendObjectsChanges,
                                    midServerLogicSendObjectsChangesHooked,
                                    (void**)&orig.midServerLogicSendObjectsChanges});
    }

    return hooks;
}

/** Hooks that used only in Scenario Editor. */
static Hooks getScenarioEditorHooks()
{
    using namespace game;

    auto& orig = getOriginalFunctions();

    // clang-format off
    Hooks hooks{
        // Check sites placement the same way as ruins, allowing them to be placed on water
        {editorFunctions.canPlaceSite, editorFunctions.canPlaceRuin},
        // Allow editor to set elves race as caster in 'cast spell on location' event effect
        {editorFunctions.radioButtonIndexToPlayerId, radioButtonIndexToPlayerIdHooked},
        // Fix DLG_R_C_SPELL so it shows actual spell info
        {CEncLayoutSpellApi::get().constructor, encLayoutSpellCtorHooked, (void**)&orig.encLayoutSpellCtor},
        // Allow editor to place more than 200 stacks on a map
        {editorFunctions.countStacksOnMap, countStacksOnMapHooked},
        // Support custom event conditions
        {CMidEvConditionApi::get().getInfoString, eventConditionGetInfoStringHooked, (void**)&orig.eventConditionGetInfoString},
        {CMidEvConditionApi::get().getDescription, eventConditionGetDescriptionHooked, (void**)&orig.eventConditionGetDescription},
        {CMidEvConditionApi::get().getBrief, eventConditionGetBriefHooked, (void**)&orig.eventConditionGetBrief},
        {editor::CCondInterfApi::get().createFromCategory, createCondInterfFromCategoryHooked, (void**)&orig.createCondInterfFromCategory},
        {CMidEventApi::get().checkValid, checkEventValidHooked, (void**)&orig.checkEventValid},
        // Support custom event effects
        //{CMidEvEffectApi::get().getInfoString, eventEffectGetInfoStringHooked, (void**)&orig.eventEffectGetInfoString},
        //{CMidEvEffectApi::get().getDescription, eventEffectGetDescriptionHooked, (void**)&orig.eventEffectGetDescription},
        //{CMidEvEffectApi::get().getBrief, eventEffectGetBriefHooked, (void**)&orig.eventEffectGetBrief},
        //{editor::CEffectInterfApi::get().createFromCategory, createEffectInterfFromCategoryHooked, (void**)&orig.createEffectInterfFromCategory},
        {CMidgardScenarioMapApi::get().checkObjects, checkMapObjectsHooked},
        // Support custom modifiers
        {CMidgardScenarioMapApi::get().stream, scenarioMapStreamHooked, (void**)&orig.scenarioMapStream},
        // Allow changing Netrals attitude from scenario properties menu
        {editor::CScenPropInterfApi::get().constructor, scenPropInterfCtorHooked, (void**)&orig.scenPropInterfCtor},
        // Support new sites
        {editor::CVisitorCreateSiteApi::get().canApply, visitorCreateSiteCanApplyHooked},
        {editor::CVisitorCreateSiteApi::get().apply, visitorCreateSiteApplyHooked},
        {editor::CObjectInterfApi::get().createTaskObj, createTaskObjHooked, (void**)&orig.createTaskObj},
        {editor::CTaskObjPropApi::get().doAction, taskObjPropDoActionHooked, (void**)&orig.taskObjPropDoAction},
        {editor::CTaskObjAddSiteApi::get().doAction, taskObjAddSiteDoActionHooked, (void**)&orig.taskObjAddSiteDoAction},
        {CScenEditApi::get().readScenData, readScenDataHooked, (void**)&orig.readScenData},
    };
    // clang-format on

    return hooks;
}


Hooks getHooks()
{
    using namespace game;

    Hooks hooks{executableIsGame() ? getGameHooks() : getScenarioEditorHooks()};

    auto& fn = gameFunctions();
    auto& orig = getOriginalFunctions();

  
    // Called when a player's turn begins.Invoked for all players, including neutral factions.
    if (hooks::executableIsGame() && fn.midServerLogicDataBeginTurn) {
        hooks.emplace_back(HookInfo{fn.midServerLogicDataBeginTurn, getBeginTurnHooked(), getBeginTurnOrig()});
    }

    // Register buildings with custom branch category as unit buildings
    hooks.emplace_back(HookInfo{fn.createBuildingType, createBuildingTypeHooked});
    // Support custom building branch category
    hooks.emplace_back(
        HookInfo{LBuildingCategoryTableApi::get().constructor, buildingCategoryTableCtorHooked});
    // Increase maximum allowed game turn
    hooks.emplace_back(HookInfo{fn.isTurnValid, isTurnValidHooked});
    // Support custom attack class category
    hooks.emplace_back(
        HookInfo{LAttackClassTableApi::get().constructor, attackClassTableCtorHooked});
    // Support custom attack class in CAttackImpl constructor
    // Support custom attack damage ratios
    // Support per-attack crit settings
    hooks.emplace_back(HookInfo{CAttackImplApi::get().constructor, attackImplCtorHooked});
    hooks.emplace_back(HookInfo{CAttackImplApi::get().constructor2, attackImplCtor2Hooked,
                                (void**)&orig.attackImplCtor2});
    hooks.emplace_back(HookInfo{CAttackImplApi::vftable()->getData, attackImplGetDataHooked,
                                (void**)&orig.attackImplGetData});
    /**
     * Display heal/damage value for any attack with qtyHeal/qtyDamage > 0 regardless of its class.
     * This hook is required for detailedAttackDescription.
     */
    hooks.emplace_back(HookInfo{fn.getAttackQtyDamageOrHeal, getAttackQtyDamageOrHealHooked});
    // Support custom attack sources
    hooks.emplace_back(
        HookInfo{LAttackSourceTableApi::get().constructor, attackSourceTableCtorHooked});
    hooks.emplace_back(
        HookInfo{fn.getSoldierAttackSourceImmunities, getSoldierAttackSourceImmunitiesHooked});
    hooks.emplace_back(HookInfo{fn.getSoldierImmunityAiRating, getSoldierImmunityAiRatingHooked,
                                (void**)&orig.getSoldierImmunityAiRating});
    hooks.emplace_back(HookInfo{fn.getAttackSourceText, getAttackSourceTextHooked});
    hooks.emplace_back(HookInfo{fn.appendAttackSourceText, appendAttackSourceTextHooked});
    hooks.emplace_back(HookInfo{fn.getAttackSourceWardFlagPosition,
                                getAttackSourceWardFlagPositionHooked,
                                (void**)&orig.getAttackSourceWardFlagPosition});
    // Support custom attack reaches
    hooks.emplace_back(
        HookInfo{LAttackReachTableApi::get().constructor, attackReachTableCtorHooked});
    hooks.emplace_back(HookInfo{fn.getAttackClassAiRating, getAttackClassAiRatingHooked});
    hooks.emplace_back(HookInfo{fn.getAttackReachAiRating, getAttackReachAiRatingHooked});
    hooks.emplace_back(HookInfo{CMidStackApi::vftable()->initialize, midStackInitializeHooked});
    // Always place melee units at the front lane in groups controlled by non-neutrals AI
    // Support custom attack reaches
    hooks.emplace_back(HookInfo{fn.chooseUnitLane, chooseUnitLaneHooked});
    // Fix missing modifiers of alternative attacks
    hooks.emplace_back(HookInfo{CUmAttackApi::get().constructor, umAttackCtorHooked});
    hooks.emplace_back(HookInfo{CUmAttackApi::get().copyConstructor, umAttackCopyCtorHooked});
    // Fix incorrect representation of alternative attack modifiers in unit encyclopedia
    hooks.emplace_back(
        HookInfo{CMidUnitDescriptorApi::get().getAttack, midUnitDescriptorGetAttackHooked});
    hooks.emplace_back(HookInfo{CMidUnitDescriptorApi::vftable()->getAttackInitiative,
                                midUnitDescriptorGetAttackInitiativeHooked});
    // Fix crash in encyclopedia showing info
    // about dyn leveled (stopped) doppleganger that transformed to stack leader
    hooks.emplace_back(HookInfo{CMidUnitDescriptorApi ::vftable()->isUnitLeader,
                                midUnitDescriptorIsUnitLeaderHooked});

    if (gameSettings().debugMode) {
        // Show and log game exceptions information
        hooks.emplace_back(HookInfo{os_exceptionApi::get().throwException, throwExceptionHooked,
                                    (void**)&orig.throwException});
    }

    if (gameSettings().shatterDamageUpgradeRatio != baseGameSettings().shatterDamageUpgradeRatio) {
        // Allow users to customize shatter damage upgrade ratio
        hooks.emplace_back(
            HookInfo{fn.applyDynUpgradeToAttackData, applyDynUpgradeToAttackDataHooked});
    }

    if (userSettings().unitEncyclopedia.detailedAttackDescription
        != baseUserSettings().unitEncyclopedia.detailedAttackDescription) {
        // Additional display of some stats bonuses, drain, critical hit, custom attack ratios, etc.
        hooks.emplace_back(HookInfo{fn.generateAttackDescription, generateAttackDescriptionHooked});
    }

    // Support custom event conditions
    hooks.emplace_back(
        HookInfo{LEventCondCategoryTableApi::get().constructor, eventCondCategoryTableCtorHooked});

    hooks.emplace_back(HookInfo{CMidEvConditionApi::get().createFromCategory,
                                createEventConditionFromCategoryHooked,
                                (void**)&orig.createEventConditionFromCategory});

    // Support custom event effects
    // hooks.emplace_back(HookInfo{LEventEffectCategoryTableApi::get().constructor,
    //                            eventEffectCategoryTableCtorHooked});

    // hooks.emplace_back(HookInfo{CMidEvEffectApi::get().createFromCategory,
    //                            createEventEffectFromCategoryHooked,
    //                            (void**)&orig.createEventEffectFromCategory});

    // Allow every leader use additional animations on strategic map
    hooks.emplace_back(
        HookInfo{fn.isUnitUseAdditionalAnimation, isUnitUseAdditionalAnimationHooked});

    // Support race-specific village graphics
    hooks.emplace_back(
        HookInfo{DisplayHandlersApi::get().villageHandler, displayHandlerVillageHooked});

    if (gameSettings().modifiers.cumulativeUnitRegeneration) {
        // Allow unit regeneration modifiers to stack
        hooks.emplace_back(HookInfo{CUmUnitApi::get().constructor, umUnitCtorHooked});
        hooks.emplace_back(HookInfo{CUmUnitApi::get().copyConstructor, umUnitCopyCtorHooked});
        hooks.emplace_back(
            HookInfo{CUmUnitApi::vftable().usSoldier->getRegen, umUnitGetRegenHooked});
    }

    // Support custom modifiers
    hooks.emplace_back(HookInfo{LModifGroupTableApi::get().constructor, modifGroupTableCtorHooked});
    hooks.emplace_back(HookInfo{TUnitModifierApi::get().constructor, unitModifierCtorHooked});
    hooks.emplace_back(HookInfo{TUnitModifierApi::vftable()->destructor, unitModifierDtorHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::get().addModifier, addModifierHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::vftable()->stream, midUnitStreamHooked});

    // Support custom modifiers display for unit encyclopedia
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().constructor, encLayoutUnitCtorHooked});
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().constructor2, encLayoutUnitCtor2Hooked});
    hooks.emplace_back(
        HookInfo{CEncLayoutUnitApi::get().dataConstructor, encLayoutUnitDataCtorHooked});
    hooks.emplace_back(
        HookInfo{CEncLayoutUnitApi::get().dataDestructor, encLayoutUnitDataDtorHooked});
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().initialize, encLayoutUnitInitializeHooked,
                                (void**)&orig.encLayoutUnitInitialize});

    // Show effective HP in unit encyclopedia
    hooks.emplace_back(HookInfo{CEncLayoutUnitApi::get().update, encLayoutUnitUpdateHooked,
                                (void**)&orig.encLayoutUnitUpdate});

    // Show total xp-killed in stack/city/ruin encyclopedia
    hooks.emplace_back(HookInfo{CEncLayoutStackApi::get().update, encLayoutStackUpdateHooked,
                                (void**)&orig.encLayoutStackUpdate});
    hooks.emplace_back(HookInfo{CEncLayoutCityApi::get().update, encLayoutCityUpdateHooked,
                                (void**)&orig.encLayoutCityUpdate});
    hooks.emplace_back(HookInfo{CEncLayoutRuinApi::get().update, encLayoutRuinUpdateHooked,
                                (void**)&orig.encLayoutRuinUpdate});

    // Support native modifiers
    hooks.emplace_back(HookInfo{CMidUnitApi::get().getModifiers, getModifiersHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::get().addModifiers, addModifiersHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::get().removeModifiers, removeModifiersHooked});
    hooks.emplace_back(HookInfo{CMidUnitApi::vftable()->initWithSoldierImpl,
                                initWithSoldierImplHooked, (void**)&orig.initWithSoldierImpl});

    // Update city encyclopedia on visiting stack changes
    hooks.emplace_back(HookInfo{CEncLayoutCityApi::vftable()->onObjectChanged,
                                encLayoutCityOnObjectChangedHooked});

    // Fix infamous crash in multiplayer with city encyclopedia when observing other player's cities
    hooks.emplace_back(
        HookInfo{CEncLayoutCityApi::get().updateGroupUi, encLayoutCityUpdateGroupUiHooked});

    // Fix display of required buildings when multiple units have the same upgrade building
    hooks.emplace_back(HookInfo{fn.getUnitRequiredBuildings, getUnitRequiredBuildingsHooked});

    // Allow foreign race units to upgrade even if its race capital is present in scenario
    // (functions as if the unit type is locked) Allow foreign race units (including neutral) to be
    // upgraded using capital buildings
    hooks.emplace_back(HookInfo{fn.isUnitTierMax, isUnitTierMaxHooked});
    hooks.emplace_back(HookInfo{fn.isUnitLevelNotMax, isUnitLevelNotMaxHooked});
    hooks.emplace_back(HookInfo{fn.isUnitUpgradePending, isUnitUpgradePendingHooked});

    // Fixes crash on scenario loading when level of any unit is below its template from
    // `GUnits.dbf`, or above maximum level for generated units (restricted by total count of unit
    // templates)
    hooks.emplace_back(
        HookInfo{CMidUnitApi::get().streamImplIdAndLevel, midUnitStreamImplIdAndLevelHooked});

    // Support custom fonts
    hooks.emplace_back(HookInfo{FontCacheApi::get().loadFontFiles, loadFontFilesHooked});
    hooks.emplace_back(HookInfo{FontCacheApi::get().dataDestructor, fontCacheDataDtorHooked});

    // Fixes incorrect order of building status checks along with missing lordHasBuilding condition
    hooks.emplace_back(HookInfo{fn.getBuildingStatus, getBuildingStatusHooked});

    // Fix input of 'io' (U+0451) and 'IO' (U+0401)
    hooks.emplace_back(HookInfo{CEditBoxInterfApi::get().isCharValid, editBoxIsCharValidHooked});

    // Support password editbox
    hooks.emplace_back(HookInfo{CEditBoxInterfApi::get().editBoxDataCtor, editBoxDataCtorHooked,
                                (void**)&orig.editBoxDataCtor});
    hooks.emplace_back(HookInfo{CEditBoxInterfApi::get().update, editBoxInterfUpdateHooked,
                                (void**)&orig.editBoxInterfUpdate});

    // Support new sites
    hooks.emplace_back(
        HookInfo{LSiteCategoryTableApi::get().constructor, siteCategoryTableCtorHooked});
    hooks.emplace_back(HookInfo{fn.getSiteNameSuffix, getSiteNameSuffixHooked});
    // Support new site on a strategic map
    hooks.emplace_back(HookInfo{ImageLayerListApi::get().getMapElementIsoLayerImages,
                                getMapElementIsoLayerImagesHooked,
                                (void**)&orig.getMapElementIsoLayerImages});
    // Support encyclopedia info for a new sites
    hooks.emplace_back(HookInfo{fn.updateEncLayoutSite, updateEncLayoutSiteHooked});
    // Support custom noble action categories
    hooks.emplace_back(
        HookInfo{LNobleActionCatTableApi::get().constructor, nobleActionCatTableCtorHooked});
    // Support new global variables
    hooks.emplace_back(HookInfo{GlobalVariablesApi::get().constructor, globalVariablesCtorHooked,
                                (void**)&orig.globalVariablesCtor});

    return hooks;
}

Hooks getVftableHooks()
{
    using namespace game;

    auto& orig = getOriginalFunctions();

    Hooks hooks;

    if (CBatAttackBestowWardsApi::vftable()) {
        // Allow bestow wards to target dead units, so it can be coupled with Revive as a secondary
        // attack
        hooks.emplace_back(HookInfo{&CBatAttackBestowWardsApi::vftable()->method14, bestowWardsMethod14Hooked});
        hooks.emplace_back(HookInfo{&CBatAttackBestowWardsApi::vftable()->method15, bestowWardsMethod15Hooked});
        // Extra immune
        hooks.emplace_back(HookInfo{&CBatAttackBestowWardsApi::vftable()->isImmune, bestowWardsAttackIsImmuneHooked});
    }

    if (CBatAttackHealApi::vftable())
    {
        // Extra immune
        hooks.emplace_back(HookInfo{&CBatAttackHealApi::vftable()->isImmune, healAttackIsImmuneHooked});
    }

    if (CBatAttackGiveAttackApi::vftable())
    {
        // Extra immune
        hooks.emplace_back(HookInfo{&CBatAttackGiveAttackApi::vftable()->isImmune, giveAttackIsImmuneHooked});
    }

    if (CBatAttackCureApi::vftable()) 
    {
        // Extra immune
        hooks.emplace_back(HookInfo{&CBatAttackCureApi::vftable()->isImmune, cureAttackIsImmuneHooked});
        // Test
        //hooks.emplace_back(HookInfo{&CBatAttackCureApi::vftable()->method15, cureAttackMethod15Hooked});
    }

    if (CBatAttackBoostDamageApi::vftable()) {
        // Extra immune
        hooks.emplace_back(HookInfo{&CBatAttackBoostDamageApi::vftable()->isImmune, boostDamageIsImmuneHooked});
    }

    if (CBatAttackReviveApi::vftable()) {
        // Extra immune
        hooks.emplace_back(HookInfo{&CBatAttackReviveApi::vftable()->isImmune, reviveAttackIsImmuneHooked});
    }

    if (gameSettings().allowShatterAttackToMiss != baseGameSettings().allowShatterAttackToMiss) {
        if (CBatAttackShatterApi::vftable())
            // Fix an issue where shatter attack always hits regardless of its power value
            hooks.emplace_back(
                HookInfo{&CBatAttackShatterApi::vftable()->canMiss, shatterCanMissHooked});
    }

    if (CEncLayoutUnitApi::vftable()) {
        // Support unit encyclopedia update on shift/ctrl/alt press (for custom unit encyclopedia)
        hooks.emplace_back(HookInfo{&CEncLayoutUnitApi::vftable()->handleKeyboard,
                                    encLayoutUnitHandleKeyboardHooked,
                                    (void**)&orig.encLayoutUnitHandleKeyboard});
    }

    return hooks;
}

void respopupInitHooked(void)
{
    spdlog::debug("Resource popup hook start");

    auto& variables = game::gameVariables();

    *variables.minimapMode = userSettings().showLandConverted;
    *variables.respopup = userSettings().showResources;

    spdlog::debug("Resource popup hook finished");
}

void* __fastcall toggleShowBannersInitHooked(void* thisptr, int /*%edx*/)
{
    spdlog::debug("Show banners hook start");

    char* ptr = (char*)thisptr;
    *ptr = userSettings().showBanners;
    // meaning unknown
    ptr[1] = 0;

    spdlog::debug("Show banners hook finished");
    return thisptr;
}

void __stdcall createBuildingTypeHooked(const game::CDBTable* dbTable,
                                        void* a2,
                                        const game::GlobalData** globalData)
{
    using namespace game;

    auto* buildings = (*globalData)->buildingCategories;
    LBuildingCategory category;
    category.vftable = BuildingCategories::vftable();

    auto& db = CDBTableApi::get();
    db.findBuildingCategory(&category, dbTable, "CATEGORY", buildings);

    auto memAlloc = Memory::get().allocate;
    auto constructor = TBuildingTypeApi::get().constructor;
    TBuildingType* buildingType = nullptr;

    auto& buildingCategories = BuildingCategories::get();
    if (category.id == buildingCategories.unit->id || isCustomBuildingCategory(&category)) {
        // This is TBuildingUnitUpgType constructor
        // without TBuildingTypeData::category validity check
        TBuildingUnitUpgType* unitBuilding = (TBuildingUnitUpgType*)memAlloc(
            sizeof(TBuildingUnitUpgType));

        constructor(unitBuilding, dbTable, globalData);
        unitBuilding->branch.vftable = UnitBranchCategories::vftable();
        unitBuilding->vftable = (IMidObjectVftable*)TBuildingUnitUpgTypeApi::vftable();

        auto* branches = (*globalData)->unitBranches;
        db.findUnitBranchCategory(&unitBuilding->branch, dbTable, "BRANCH", branches);
        db.readUnitLevel(&unitBuilding->level, dbTable, "LEVEL");

        buildingType = unitBuilding;
    } else {
        buildingType = constructor((TBuildingType*)memAlloc(sizeof(TBuildingType)), dbTable,
                                   globalData);
    }

    if (!gameFunctions().addObjectAndCheckDuplicates(a2, buildingType)) {
        db.duplicateRecordException(dbTable, &buildingType->id);
    }
}

game::LBuildingCategoryTable* __fastcall buildingCategoryTableCtorHooked(
    game::LBuildingCategoryTable* thisptr,
    int /*%edx*/,
    const char* globalsFolderPath,
    void* codeBaseEnvProxy)
{
    using namespace game;

    static const char dbfFileName[] = "LBuild.dbf";

    spdlog::debug("Hook started");

    const auto dbfFilePath{std::filesystem::path(globalsFolderPath) / dbfFileName};
    addCustomBuildingCategory(dbfFilePath, BuildingBranchNumber::Fighter, "L_FIGHTER");
    addCustomBuildingCategory(dbfFilePath, BuildingBranchNumber::Mage, "L_MAGE");
    addCustomBuildingCategory(dbfFilePath, BuildingBranchNumber::Archer, "L_ARCHER");
    addCustomBuildingCategory(dbfFilePath, BuildingBranchNumber::Other, "L_CUSTOM");
    addCustomBuildingCategory(dbfFilePath, BuildingBranchNumber::Special, "L_SPECIAL");

    auto& table = LBuildingCategoryTableApi::get();
    auto& categories = BuildingCategories::get();

    thisptr->bgn = nullptr;
    thisptr->end = nullptr;
    thisptr->allocatedMemEnd = nullptr;
    thisptr->allocator = nullptr;
    thisptr->vftable = LBuildingCategoryTableApi::vftable();

    table.init(thisptr, codeBaseEnvProxy, globalsFolderPath, dbfFileName);
    table.readCategory(categories.guild, thisptr, "L_GUILD", dbfFileName);
    table.readCategory(categories.heal, thisptr, "L_HEAL", dbfFileName);
    table.readCategory(categories.magic, thisptr, "L_MAGIC", dbfFileName);
    table.readCategory(categories.unit, thisptr, "L_UNIT", dbfFileName);
    for (auto& custom : getCustomBuildingCategories()) {
        table.readCategory(&custom.second, thisptr, custom.second.text.c_str(), dbfFileName);
    }
    table.initDone(thisptr);

    spdlog::debug("Hook finished");
    return thisptr;
}

static void addUnitBuilding(game::BuildingBranchMap* branchMap,
                            const game::TBuildingType* buildingType,
                            game::BuildingBranchNumber branchNumber,
                            game::CPhaseGame* phaseGame)
{
    using namespace game;

    const auto& buildingBranch = CBuildingBranchApi::get();
    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    const auto& branches = UnitBranchCategories::get();

    const TBuildingUnitUpgType* unitUpg = (const TBuildingUnitUpgType*)
        dynamicCast(buildingType, 0, rtti.TBuildingTypeType, rtti.TBuildingUnitUpgTypeType, 0);

    auto branchId = unitUpg->branch.id;
    if (branchId == branches.sideshow->id) {
        buildingBranch.addSideshowUnitBuilding(branchMap, unitUpg);
        return;
    }

    if (branchId == branches.fighter->id && branchNumber == BuildingBranchNumber::Fighter
        || branchId == branches.mage->id && branchNumber == BuildingBranchNumber::Mage
        || branchId == branches.archer->id && branchNumber == BuildingBranchNumber::Archer
        || branchId == branches.special->id && branchNumber == BuildingBranchNumber::Special) {
        buildingBranch.addUnitBuilding(phaseGame, branchMap, unitUpg);
    }
}

static void addBuilding(game::BuildingBranchMap* branchMap,
                        const game::TBuildingType* buildingType,
                        game::BuildingBranchNumber branchNumber,
                        game::CPhaseGame* phaseGame)
{
    using namespace game;

    const auto& buildingBranch = CBuildingBranchApi::get();
    const auto& categories = BuildingCategories::get();

    const LBuildingCategory* category = &buildingType->data->category;
    if (category->id == categories.unit->id) {
        addUnitBuilding(branchMap, buildingType, branchNumber, phaseGame);
    } else if ((category->id == categories.guild->id || category->id == categories.heal->id
                || category->id == categories.magic->id)
               && branchNumber == BuildingBranchNumber::Other) {
        buildingBranch.addBuilding(phaseGame, branchMap, buildingType);
    } else if (category->id == getCustomBuildingCategoryId(branchNumber)) {
        buildingBranch.addBuilding(phaseGame, branchMap, buildingType);
    }
}

game::CBuildingBranch* __fastcall buildingBranchCtorHooked(game::CBuildingBranch* thisptr,
                                                           int /*%edx*/,
                                                           game::CPhaseGame* phaseGame,
                                                           game::BuildingBranchNumber* branchNumber)
{
    using namespace game;

    spdlog::debug("CBuildingBranchCtor hook started");

    auto memAlloc = Memory::get().allocate;
    CBuildingBranchData* data = (CBuildingBranchData*)memAlloc(sizeof(CBuildingBranchData));

    const auto& buildingBranch = CBuildingBranchApi::get();
    buildingBranch.initData(data);

    thisptr->data = data;
    thisptr->vftable = CBuildingBranchApi::vftable();

    buildingBranch.initBranchMap(&thisptr->data->map);
    thisptr->data->branchNumber = *branchNumber;

    auto* dialogName = &thisptr->data->branchDialogName;
    StringApi::get().free(dialogName);
    dialogName->string = nullptr;
    dialogName->length = 0;
    dialogName->lengthAllocated = 0;

    const auto& phase = CPhaseApi::get();
    auto playerId = phase.getCurrentPlayerId(&phaseGame->phase);
    auto objectMap = phase.getDataCache(&phaseGame->phase);
    auto findScenarioObjectById = objectMap->vftable->findScenarioObjectById;

    auto playerObject = findScenarioObjectById(objectMap, playerId);

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    const CMidPlayer* player = (const CMidPlayer*)dynamicCast(playerObject, 0,
                                                              rtti.IMidScenarioObjectType,
                                                              rtti.CMidPlayerType, 0);

    const LRaceCategory* playerRace = &player->raceType->data->raceType;
    thisptr->data->raceCategory.table = playerRace->table;
    thisptr->data->raceCategory.id = playerRace->id;

    const auto& fn = gameFunctions();
    auto lord = fn.getLordByPlayer(player);
    auto buildList = lord->data->buildList;

    const auto& globalApi = GlobalDataApi::get();
    auto buildings = (*globalApi.getGlobalData())->buildings;

    for (const auto& id : buildList->data) {
        auto buildingType = (const TBuildingType*)globalApi.findById(buildings, &id);
        addBuilding(&thisptr->data->map, buildingType, *branchNumber, phaseGame);
    }

    spdlog::debug("Ctor finished");
    return thisptr;
}

int __stdcall chooseUnitLaneHooked(const game::IUsSoldier* soldier)
{
    using namespace game;

    // Place units with adjacent attack reach at the front lane, despite of their attack class
    IAttack* attack = soldier->vftable->getAttackById(soldier);
    return isMeleeAttack(attack) ? 1 : 0;
}

bool __stdcall isTurnValidHooked(int turn)
{
    return turn >= 0 && turn <= 9999 || turn == -1;
}

game::CMidgardID* __stdcall radioButtonIndexToPlayerIdHooked(game::CMidgardID* playerId,
                                                             game::IMidgardObjectMap* objectMap,
                                                             int index)
{
    using namespace game;
    const auto& races = RaceCategories::get();
    const auto& fn = editorFunctions;

    const CMidPlayer* player{nullptr};
    switch (index) {
    case 0:
        player = fn.findPlayerByRaceCategory(races.human, objectMap);
        break;
    case 1:
        player = fn.findPlayerByRaceCategory(races.heretic, objectMap);
        break;
    case 2:
        player = fn.findPlayerByRaceCategory(races.undead, objectMap);
        break;
    case 3:
        player = fn.findPlayerByRaceCategory(races.dwarf, objectMap);
        break;
    case 4:
        player = fn.findPlayerByRaceCategory(races.elf, objectMap);
        break;
    }

    *playerId = player ? player->id : emptyId;

    return playerId;
}

bool __fastcall shatterCanPerformHooked(game::CBatAttackShatter* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::BattleMsgData* battleMsgData,
                                        game::CMidgardID* unitId)
{
    using namespace game;

    CMidgardID targetGroupId{};
    thisptr->vftable->getTargetGroupId(thisptr, &targetGroupId, battleMsgData);

    auto& fn = gameFunctions();
    CMidgardID unitGroupId{};
    fn.getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, unitId, true);

    if (targetGroupId != unitGroupId) {
        // Can't target allies
        return false;
    }

    auto& battle = BattleMsgDataApi::get();
    if (battle.getUnitStatus(battleMsgData, unitId, BattleStatus::Retreat)) {
        // Can't target retreating units
        return false;
    }

    const int shatteredArmor = battle.getUnitShatteredArmor(battleMsgData, unitId);
    if (shatteredArmor >= gameSettings().shatteredArmorMax) {
        return false;
    }

    CMidUnit* unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);

    int unitArmor{};
    soldier->vftable->getArmor(soldier, &unitArmor);

    const int fortArmor = battle.getUnitFortificationArmor(battleMsgData, unitId);
    const int reducedArmor = unitArmor - shatteredArmor;

    if (reducedArmor > fortArmor) {
        return reducedArmor > 0;
    }

    return false;
}

void __fastcall setUnitShatteredArmorHooked(game::BattleMsgData* thisptr,
                                            int /*%edx*/,
                                            const game::CMidgardID* unitId,
                                            int armor)
{
    using namespace game;

    auto info = BattleMsgDataApi::get().getUnitInfoById(thisptr, unitId);
    if (!info) {
        return;
    }

    info->shatteredArmor = std::clamp(info->shatteredArmor + armor, 0,
                                      gameSettings().shatteredArmorMax);
}

void __fastcall shatterOnHitHooked(game::CBatAttackShatter* thisptr,
                                   int /*%edx*/,
                                   game::IMidgardObjectMap* objectMap,
                                   game::BattleMsgData* battleMsgData,
                                   game::CMidgardID* unitId,
                                   game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const auto& battleApi = BattleMsgDataApi::get();

    CMidUnit* targetUnit = gameFunctions().findUnitById(objectMap, unitId);

    if (!targetUnit)
        return;

    auto attackVftable = (const IAttackVftable*)thisptr->attack->vftable;

    const auto damageMax{gameSettings().shatterDamageMax};
    int shatterDamage = attackVftable->getQtyDamage(thisptr->attack);
    if (shatterDamage > damageMax) {
        shatterDamage = damageMax;
    }

    CMidUnit* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId1);

    UnitInfo* targetInfo = battleApi.getUnitInfoById(battleMsgData, unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.damage = shatterDamage;
    params.attack = thisptr->attack;
    params.attackClass = "Shatter";
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    shatterDamage = params.damage;

    setUnitShatteredArmorHooked(battleMsgData, 0, unitId, shatterDamage);

    const auto unit = gameFunctions().findUnitById(objectMap, unitId);

    addToBattleAttackInfo(*attackInfo, unit, 0, 0, false);
}


bool __fastcall shatterCanMissHooked(game::CBatAttackShatter* thisptr,
                                     int /*%edx*/,
                                     game::BattleMsgData* battleMsgData,
                                     game::CMidgardID* id)
{
    return true;
}

bool __stdcall buildLordSpecificBuildingsHooked(game::IMidgardObjectMap* objectMap,
                                                const game::NetPlayerInfo* playerInfo,
                                                int)
{
    using namespace game;

    auto playerObj = objectMap->vftable->findScenarioObjectById(objectMap, &playerInfo->playerId);

    const auto dynamicCast = RttiApi::get().dynamicCast;
    const auto& rtti = RttiApi::rtti();
    auto player = (const CMidPlayer*)dynamicCast(playerObj, 0, rtti.IMidScenarioObjectType,
                                                 rtti.CMidPlayerType, 0);

    auto& fn = gameFunctions();
    if (!gameSettings().preserveCapitalBuildings) {
        fn.deletePlayerBuildings(objectMap, player);
    }

    auto lordType = fn.getLordByPlayer(player);
    const auto lordCategoryId{lordType->data->lordCategory.id};

    const auto& lordCategories = LordCategories::get();
    const auto& buildingCategories = BuildingCategories::get();

    if (lordCategoryId == lordCategories.diplomat->id) {
        return fn.addCapitalBuilding(objectMap, player, buildingCategories.guild);
    }

    if (lordCategoryId == lordCategories.mage->id) {
        return fn.addCapitalBuilding(objectMap, player, buildingCategories.magic);
    }

    if (gameSettings().buildTempleForWarriorLord && playerInfo->controlledByHuman
        && lordCategoryId == lordCategories.warrior->id) {
        return fn.addCapitalBuilding(objectMap, player, buildingCategories.heal);
    }

    return true;
}

game::CEncLayoutSpell* __fastcall encLayoutSpellCtorHooked(game::CEncLayoutSpell* thisptr,
                                                           int /*%edx*/,
                                                           game::IMidgardObjectMap* objectMap,
                                                           game::CInterface* interf,
                                                           void* a2,
                                                           game::CMidgardID* spellId,
                                                           game::CEncParamBase* encParam,
                                                           const game::CMidgardID* playerId)
{
    using namespace game;

    if (!playerId) {
        forEachScenarioObject(objectMap, IdType::Player,
                              [&playerId](const IMidScenarioObject* obj) {
                                  // Use id of the first found player to show spell info
                                  if (!playerId) {
                                      playerId = &obj->id;
                                  }
                              });
    }

    // Show spell price and casting cost
    encParam->data->statuses = 4;
    return getOriginalFunctions().encLayoutSpellCtor(thisptr, objectMap, interf, a2, spellId,
                                                     encParam, playerId);
}

int __stdcall countStacksOnMapHooked(game::IMidgardObjectMap*)
{
    return 0;
}

game::CDDCarryOverItems* __fastcall carryOverItemsCtorHooked(game::CDDCarryOverItems* thisptr,
                                                             int /*%edx*/,
                                                             game::IMidDropManager* dropManager,
                                                             game::CListBoxInterf* listBox,
                                                             game::CPhaseGame* phaseGame,
                                                             int carryOverItemsMax)
{
    return getOriginalFunctions().carryOverItemsCtor(thisptr, dropManager, listBox, phaseGame,
                                                     userSettings().carryOverItemsMax);
}

void __fastcall markMapPositionHooked(void* thisptr, int /*%edx*/, game::CMqPoint* position)
{
    if (position->x < 0 || position->x >= 144 || position->y < 0 || position->y >= 144)
        return;

    return getOriginalFunctions().markMapPosition(thisptr, position);
}

int __stdcall computeDamageHooked(const game::IMidgardObjectMap* objectMap,
                                  const game::BattleMsgData* battleMsgData,
                                  const game::IAttack* attack,
                                  const game::CMidgardID* attackerUnitId,
                                  const game::CMidgardID* targetUnitId,
                                  bool computeCriticalHit,
                                  int* attackDamage,
                                  int* criticalHitDamage)
{
    using namespace game;

    int armor;
    const auto& fn = gameFunctions();
    fn.computeArmor(&armor, objectMap, battleMsgData, targetUnitId);

    bool isEasyDifficulty = false;
    auto player = getPlayer(objectMap, battleMsgData, attackerUnitId);
    if (player && player->isHuman) {
        const auto& difficulties = DifficultyLevelCategories::get();
        isEasyDifficulty = getScenarioInfo(objectMap)->gameDifficulty.id == difficulties.easy->id;
    }

    int damageMax = fn.computeDamageMax(objectMap, attackerUnitId);
    int damageWithBuffs = fn.computeDamageWithBuffs(attack, damageMax, battleMsgData,
                                                    attackerUnitId, true, isEasyDifficulty);
    int damage = damageWithBuffs * (100 - armor) / 100;

    int critDamage = 0;
    if (computeCriticalHit) {
        bool critHit = attack->vftable->getCritHit(attack);
        if (!critHit) {
            auto unit = fn.findUnitById(objectMap, attackerUnitId);
            critHit = hasCriticalHitLeaderAbility(unit->unitImpl);
        }

        if (critHit) {
            auto customData = getCustomAttackData(attack);
            int critPower = customData.critPower;
            if (!fn.attackShouldMiss(&critPower)) {
                critDamage = damageWithBuffs * customData.critDamage / 100;
            }
        }
    }

    if (getCustomAttacks().damageRatiosEnabled) {
        auto& damageRatios = getCustomDamageRatios(attack);
        auto ratio = damageRatios.find(*targetUnitId);
        if (ratio != damageRatios.end()) {
            damage = applyAttackDamageRatio(damage, ratio->second);
            critDamage = applyAttackDamageRatio(critDamage, ratio->second);
        }
    }

    if (attackDamage)
        *attackDamage = damage;
    if (criticalHitDamage)
        *criticalHitDamage = critDamage;
    return damage + critDamage;
}

void __stdcall getAttackPowerHooked(int* power,
                                    const game::IAttack* attack,
                                    const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* unitId,
                                    const game::BattleMsgData* battleMsgData)
{
    if (!attack) {
        *power = 100;
        return;
    }

    using namespace game;

    const auto& restrictions = game::gameRestrictions();

    const auto attackClass = attack->vftable->getAttackClass(attack);

    if (!attackHasPower(attackClass->id)) {
        *power = 100;
        return;
    }

    int tmpPower{};
    attack->vftable->getPower(attack, &tmpPower);

    const auto& battle = BattleMsgDataApi::get();
    auto groupId = battle.isUnitAttacker(battleMsgData, unitId) ? &battleMsgData->attackerGroupId
                                                                : &battleMsgData->defenderGroupId;

    const auto& fn = gameFunctions();

    if (!fn.isGroupOwnerPlayerHuman(objectMap, groupId)) {
        const auto& difficulties = DifficultyLevelCategories::get();
        const auto difficultyId = getScenarioInfo(objectMap)->gameDifficulty.id;

        const auto& aiAttackPower = gameSettings().aiAttackPowerBonus;
        const std::int8_t* bonus = &aiAttackPower.easy;

        if (difficultyId == difficulties.easy->id) {
            bonus = &aiAttackPower.easy;
        } else if (difficultyId == difficulties.average->id) {
            bonus = &aiAttackPower.average;
        } else if (difficultyId == difficulties.hard->id) {
            bonus = &aiAttackPower.hard;
        } else if (difficultyId == difficulties.veryHard->id) {
            bonus = &aiAttackPower.veryHard;
        }

        if (aiAttackPower.absolute) {
            tmpPower += *bonus;
        } else {
            tmpPower += tmpPower * *bonus / 100;
        }

        tmpPower = std::clamp(tmpPower, restrictions.attackPower->min,
                              restrictions.attackPower->max);
    }

    const auto& attacks = AttackClassCategories::get();
    if (battleMsgData->currentRound > gameSettings().disableAllowedRoundMax
        && (attackClass->id == attacks.paralyze->id || attackClass->id == attacks.petrify->id)) {
        tmpPower = 0;
    }

    tmpPower -= battle.getAttackPowerReduction(battleMsgData, unitId);
    *power = std::clamp(tmpPower, restrictions.attackPower->min, restrictions.attackPower->max);
}

bool __stdcall attackShouldMissHooked(const int* power)
{
    return game::gameFunctions().generateRandomNumber(100) > *power;
}

int __stdcall getUnitHealAttackNumberHooked(const game::IMidgardObjectMap* objectMap,
                                            const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();
    auto unit = fn.findUnitById(objectMap, unitId);
    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);

    auto attack = soldier->vftable->getAttackById(soldier);
    int qtyHeal = attack->vftable->getQtyHeal(attack);
    if (qtyHeal > 0)
        return 1;

    auto attack2 = soldier->vftable->getSecondAttackById(soldier);
    if (attack2) {
        qtyHeal = attack2->vftable->getQtyHeal(attack2);
        if (qtyHeal > 0)
            return 2;
    }

    return 0;
}

int __stdcall getAttackQtyDamageOrHealHooked(const game::IAttack* attack, int damageMax)
{
    int qtyHeal = attack->vftable->getQtyHeal(attack);
    if (qtyHeal)
        return qtyHeal;

    int qtyDamage = attack->vftable->getQtyDamage(attack);
    if (qtyDamage > damageMax)
        qtyDamage = damageMax;

    return qtyDamage;
}

static game::CMidgardID currUnitId = game::emptyId;
bool isItemUsed = false;
void __stdcall afterBattleTurnHooked(game::BattleMsgData* battleMsgData,
                                     const game::CMidgardID* unitId,
                                     const game::CMidgardID* nextUnitId)
{
    using namespace game;

    if (!battleMsgData || !unitId || !nextUnitId)
        return;

    const auto& battle = BattleMsgDataApi::get();
    const bool isSameUnit = (*unitId == *nextUnitId);

    auto* unitInfo = battle.getUnitInfoById(battleMsgData, nextUnitId);

    if (!isSameUnit) {
        battleMsgData->battleStateFlags2.parts.shouldUpdateUnitEffects = true;
        battle.removeFiniteBoostLowerDamage(battleMsgData, unitId);
        UnitInfo* unitInfo = battle.getUnitInfoById(battleMsgData, nextUnitId);
        if (unitInfo)
            unitInfo->unitFlags.parts.attackedOnceOfTwice = false;
    }

    if (isItemUsed)
        isItemUsed = false;

    currUnitId = *unitId;

    static const auto scriptPath = scriptsFolder() / "hooks/hooks.lua";

    std::optional<sol::environment> env;
    auto f = getScriptFunction(scriptPath, "OnAfterBattleTurn", env, false, true);

    if (f) {
        try {
            const auto& fn = gameFunctions();
            auto* objMap = getObjectMap();

            CMidUnit* cMidUnit = fn.findUnitById(objMap, unitId);
            CMidUnit* cMidUnitNext = fn.findUnitById(objMap, nextUnitId);

            const bindings::BattleMsgDataView battleView{battleMsgData, objMap};
            std::optional<bindings::UnitView> unitView;
            std::optional<bindings::UnitView> unitNextView;
            if (cMidUnit)
                unitView.emplace(cMidUnit);
            if (cMidUnitNext)
                unitNextView.emplace(cMidUnitNext);

            (*f)(battleView, unitView ? unitView : std::nullopt,
                 unitNextView ? unitNextView : std::nullopt);
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Lua Error in 'OnAfterBattleTurn':\n{:s}", e.what()));
        }
    }
}

void __stdcall beforeBattleTurnHooked(game::BattleMsgData* battleMsgData,
                                      game::IMidgardObjectMap* objectMap,
                                      const game::CMidgardID* unitId)
{
    using namespace game;

    if (!battleMsgData || !objectMap || !unitId)
        return;

    const auto& battle = BattleMsgDataApi::get();
    auto& custom = getCustomAttacks();
    const auto currentIdVal = *unitId;

    battle.setUnitStatus(battleMsgData, unitId, BattleStatus::Defend, false);

    UnitInfo* unitInfo = nullptr;

    if (currentIdVal != currUnitId) {
        unitInfo = battle.getUnitInfoById(battleMsgData, unitId);
        if (unitInfo) {
            auto modifiedUnitIds = getModifiedUnitIds(unitInfo);
            for (const auto& modId : modifiedUnitIds) {
                removeModifiers(battleMsgData, objectMap, unitInfo, &modId);
            }
            resetModifiedUnitsInfo(unitInfo);
        }
    }

    battle.setAttackPowerReduction(battleMsgData, unitId, 0);
    custom.targets.clear();
    custom.damageRatios.clear();

    auto& freeTransformSelf = custom.freeTransformSelf;
    if (freeTransformSelf.unitId != currentIdVal) {
        freeTransformSelf.unitId = currentIdVal;
        freeTransformSelf.turnCount = 0;
        freeTransformSelf.used = false;
    } else if (freeTransformSelf.used) {
        if (!unitInfo)
            unitInfo = battle.getUnitInfoById(battleMsgData, unitId);

        if (unitInfo) {
            unitInfo->unitFlags.parts.attackedOnceOfTwice = true;
        }
    }
    freeTransformSelf.turnCount++;

    static const auto scriptPath = scriptsFolder() / "hooks/hooks.lua";

    std::optional<sol::environment> env;
    auto f = getScriptFunction(scriptPath, "OnBeforeBattleTurn", env, false, true);

    if (f) {
        try {
            const auto& fn = gameFunctions();
            if (CMidUnit* cMidUnit = fn.findUnitById(objectMap, unitId)) {
                const bindings::BattleMsgDataView battleMsg{battleMsgData, objectMap};
                const bindings::UnitView unit{cMidUnit};

                (*f)(battleMsg, unit);
            }
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Lua Error in 'OnBeforeBattleTurn':\n{:s}", e.what()));
        }
    }
}

void __stdcall throwExceptionHooked(const game::os_exception* thisptr, const void* throwInfo)
{
    // TODO: this is wrong, os_exception is a base class and it does not store message as a char*
    // Instead, there are plenty of child classes that store messages in their own way.
    // There should be proper way to get message, perhaps using vftable.
    // This particular example leads to a crash when there are problems with .dlg files and
    // CAutoDialogException is thrown
    if (thisptr && thisptr->message) {
        showErrorMessageBox(fmt::format("Caught exception '{:s}'.\n"
                                        "The {:s} will probably crash now.",
                                        thisptr->message, executableIsGame() ? "game" : "editor"));
    }

    getOriginalFunctions().throwException(thisptr, throwInfo);
}

int __stdcall computeUnitEffectiveHpForAiHooked(const game::IMidgardObjectMap* objectMap,
                                                const game::CMidUnit* unit,
                                                const game::BattleMsgData* battleMsgData)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (!unit)
        return 0;

    int armor;
    fn.computeArmor(&armor, objectMap, battleMsgData, &unit->id);

    return computeUnitEffectiveHpForAi(unit->currentHp, armor);
}

void __stdcall applyDynUpgradeToAttackDataHooked(const game::CMidgardID* unitImplId,
                                                 game::CUnitGenerator* unitGenerator,
                                                 int unitLevel,
                                                 game::IdType dynUpgradeType,
                                                 const game::CMidgardID* altAttackId,
                                                 game::CAttackData* attackData)
{
    using namespace game;

    CMidgardID leveledUnitImplId{};
    unitGenerator->vftable->generateUnitImplId(unitGenerator, &leveledUnitImplId, unitImplId,
                                               unitLevel);

    CMidgardID globalUnitImplId{};
    unitGenerator->vftable->getGlobalUnitImplId(unitGenerator, &globalUnitImplId,
                                                &leveledUnitImplId);

    CMidgardID leveledAttackId{};
    CMidgardIDApi::get().changeType(&leveledAttackId, &leveledUnitImplId, dynUpgradeType);

    CDynUpgrade* upgrade1 = nullptr;
    CDynUpgrade* upgrade2 = nullptr;
    int upgrade1Count;
    int upgrade2Count;
    gameFunctions().computeUnitDynUpgrade(&globalUnitImplId, unitLevel, &upgrade1, &upgrade2,
                                          &upgrade1Count, &upgrade2Count);

    attackData->attackId = leveledAttackId;
    attackData->altAttack = *altAttackId;

    if (upgrade1)
        attackData->initiative += upgrade1->initiative * upgrade1Count;
    if (upgrade2)
        attackData->initiative += upgrade2->initiative * upgrade2Count;

    if (attackData->power > 0) {
        if (upgrade1)
            attackData->power += upgrade1->power * upgrade1Count;
        if (upgrade2)
            attackData->power += upgrade2->power * upgrade2Count;
    }

    if (attackData->qtyDamage > 0) {
        float ratio = 1.0;
        if (attackData->attackClass->id == AttackClassCategories::get().shatter->id)
            ratio = (float)gameSettings().shatterDamageUpgradeRatio / 100;

        if (upgrade1)
            attackData->qtyDamage += lround(upgrade1->damage * upgrade1Count * ratio);
        if (upgrade2)
            attackData->qtyDamage += lround(upgrade2->damage * upgrade2Count * ratio);
    }

    if (attackData->qtyHeal > 0) {
        if (upgrade1)
            attackData->qtyHeal += upgrade1->heal * upgrade1Count;
        if (upgrade2)
            attackData->qtyHeal += upgrade2->heal * upgrade2Count;
    }
}

void __stdcall getUnitAttacksHooked(const game::IMidgardObjectMap* objectMap,
                                    const game::CMidgardID* unitId,
                                    game::AttackTypePairVector* value,
                                    bool checkAltAttack)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& vectorApi = AttackTypePairVectorApi::get();

    auto unit = fn.findUnitById(objectMap, unitId);

    auto attack = getAttack(unit->unitImpl, true, checkAltAttack);
    AttackTypePair pair{attack, AttackType::Primary};
    vectorApi.pushBack(value, &pair);

    auto attack2 = getAttack(unit->unitImpl, false, checkAltAttack);
    if (attack2) {
        AttackTypePair pair{attack2, AttackType::Secondary};
        vectorApi.pushBack(value, &pair);
    }

    auto item1Attack = fn.getItemAttack(objectMap, unitId, 1);
    if (item1Attack) {
        AttackTypePair pair{item1Attack, AttackType::Item};
        vectorApi.pushBack(value, &pair);
    }

    auto item2Attack = fn.getItemAttack(objectMap, unitId, 2);
    if (item2Attack) {
        AttackTypePair pair{item2Attack, AttackType::Item};
        vectorApi.pushBack(value, &pair);
    }
}

bool __stdcall isUnitUseAdditionalAnimationHooked(const game::CMidgardID*)
{
    return true;
}

void __stdcall getCityPreviewLargeImageNamesHooked(game::List<game::String>* imageNames,
                                                   const void* cityFF,
                                                   const game::LRaceCategory* race,
                                                   int cityTier)
{
    using namespace game;

    const auto& races{RaceCategories::get()};
    const auto raceId{race->id};

    if (cityTier == 0 || raceId == races.neutral->id) {
        getOriginalFunctions().getCityPreviewLargeImageNames(imageNames, cityFF, race, cityTier);
        return;
    }

    char raceSuffix{'?'};
    if (raceId == races.human->id) {
        raceSuffix = 'H';
    } else if (raceId == races.undead->id) {
        raceSuffix = 'U';
    } else if (raceId == races.heretic->id) {
        raceSuffix = 'E';
    } else if (raceId == races.dwarf->id) {
        raceSuffix = 'D';
    } else if (raceId == races.elf->id) {
        raceSuffix = 'F';
    }

    const auto name{fmt::format("ALN{:c}{:d}", raceSuffix, cityTier)};
    const auto length{name.length()};

    GameImagesApi::get().getImageNames(imageNames, cityFF, name.c_str(), length, length);

    if (!imageNames->length) {
        // Fallback to default city image names
        getOriginalFunctions().getCityPreviewLargeImageNames(imageNames, cityFF, race, cityTier);
    }
}

void __stdcall getCityIconImageNamesHooked(game::List<game::String>* imageNames,
                                           const void* iconsFF,
                                           const game::CMidgardID* fortificationId,
                                           const game::IMidgardObjectMap* objectMap)
{
    using namespace game;

    auto obj{objectMap->vftable->findScenarioObjectById(objectMap, fortificationId)};
    if (!obj) {
        return;
    }

    auto fortification{static_cast<const CFortification*>(obj)};
    auto vftable{static_cast<const CFortificationVftable*>(fortification->vftable)};

    auto category{vftable->getCategory(fortification)};

    if (category->id != FortCategories::get().village->id) {
        getOriginalFunctions().getCityIconImageNames(imageNames, iconsFF, fortificationId,
                                                     objectMap);
        return;
    }

    auto village{static_cast<const CMidVillage*>(fortification)};

    const auto& races{RaceCategories::get()};
    const auto neutralRace{races.neutral};
    const LRaceCategory* ownerRace{neutralRace};

    if (village->ownerId != emptyId) {
        auto player{getPlayer(objectMap, &village->ownerId)};
        if (player) {
            ownerRace = &player->raceType->data->raceType;
        }
    }

    const auto ownerRaceId{ownerRace->id};
    if (ownerRaceId == neutralRace->id) {
        getOriginalFunctions().getCityIconImageNames(imageNames, iconsFF, fortificationId,
                                                     objectMap);
        return;
    }

    const char* raceSuffix{"HU"};
    if (ownerRaceId == races.human->id) {
        raceSuffix = "HU";
    } else if (ownerRaceId == races.heretic->id) {
        raceSuffix = "HE";
    } else if (ownerRaceId == races.undead->id) {
        raceSuffix = "UN";
    } else if (ownerRaceId == races.dwarf->id) {
        raceSuffix = "DW";
    } else if (ownerRaceId == races.elf->id) {
        raceSuffix = "EL";
    }

    const char tierLetter{'0' + static_cast<char>(village->tierLevel)};
    const auto name{fmt::format("CITY{:s}{:c}", raceSuffix, tierLetter)};
    const auto length{name.length()};

    GameImagesApi::get().getImageNames(imageNames, iconsFF, name.c_str(), length, length);

    if (!imageNames->length) {
        // Fallback to default city icon image names
        getOriginalFunctions().getCityIconImageNames(imageNames, iconsFF, fortificationId,
                                                     objectMap);
    }
}

bool __fastcall checkMapObjectsHooked(game::CMidgardScenarioMap* scenarioMap, int /*%edx*/)
{
    using namespace game;

    std::memset(scenarioMap->freeIdTypeIndices, 0, sizeof(scenarioMap->freeIdTypeIndices));

    const auto& api{CMidgardScenarioMapApi::get()};

    ScenarioMapDataIterator current{};
    api.begin(scenarioMap, &current);

    ScenarioMapDataIterator end{};
    api.end(scenarioMap, &end);

    const auto& idApi{CMidgardIDApi::get()};

    while (current.foundRecord != end.foundRecord) {
        const auto* objectId{&current.foundRecord->key};

        const auto type{static_cast<int>(idApi.getType(objectId))};
        const auto typeIndex{idApi.getTypeIndex(objectId)};

        if (scenarioMap->freeIdTypeIndices[type] <= typeIndex) {
            scenarioMap->freeIdTypeIndices[type] = typeIndex + 1;
        }

        const auto* object{current.foundRecord->value.data};
        if (!object->vftable->isValid(object, scenarioMap)) {
            spdlog::error("Scenario object {:s} is invalid", idToString(objectId));
            return false;
        }

        api.advance(&current);
    }

    return true;
}

void validateUnits(game::CMidgardScenarioMap* scenarioMap)
{
    using namespace game;

    forEachScenarioObject(scenarioMap, IdType::Unit,
                          [](const IMidScenarioObject* obj) { validateUnit((CMidUnit*)obj); });
}

int __stdcall loadScenarioMapHooked(int a1,
                                    game::CMidStreamEnvFile* streamEnv,
                                    game::CMidgardScenarioMap* scenarioMap)
{
    stackTemplateCacheClear();

    const int result = getOriginalFunctions().loadScenarioMap(a1, streamEnv, scenarioMap);
    // Write-mode validation is done in midUnitStreamHooked
    validateUnits(scenarioMap);

    using namespace game;

    const auto& dynamicCast{RttiApi::get().dynamicCast};
    const auto& rtti{RttiApi::rtti()};

    auto addStackFromTemplateToCache = [&dynamicCast, &rtti](const IMidScenarioObject* obj) {
        auto stack{(const CMidStack*)dynamicCast(obj, 0, rtti.IMidScenarioObjectType,
                                                 rtti.CMidStackType, 0)};
        if (stack->sourceTemplateId != emptyId) {
            stackTemplateCacheAdd(stack->sourceTemplateId, stack->id);
        }
    };

    forEachScenarioObject(scenarioMap, IdType::Stack, addStackFromTemplateToCache);
    return result;
}

bool __fastcall scenarioMapStreamHooked(game::CMidgardScenarioMap* scenarioMap,
                                        int /*%edx*/,
                                        game::IMidgardStreamEnv* streamEnv)
{
    bool result = getOriginalFunctions().scenarioMapStream(scenarioMap, streamEnv);
    if (result && streamEnv->vftable->readMode(streamEnv)) {
        // Write-mode validation is done in midUnitStreamHooked
        validateUnits(scenarioMap);
    }

    return result;
}

void __stdcall getStackFortRuinGroupForChangeHooked(game::IMidgardObjectMap* objectMap,
                                                    const game::CMidgardID* objectId,
                                                    game::CMidUnitGroup** result)
{
    using namespace game;

    auto group = getGroup(objectMap, objectId, true);
    if (group) {
        // The validation itself will be performed in midServerLogicSendObjectsChangesHooked
        auto& unitsToValidate = getUnitsToValidate();
        const auto& units = group->units;
        for (auto it = units.bgn; it != units.end; it++) {
            unitsToValidate.insert(*it);
        }
    }

    *result = group;
}

game::CanApplyPotionResult __stdcall canApplyPotionToUnitHooked(
    const game::IMidgardObjectMap* objectMap,
    const game::CMidgardID* unitId,
    const game::CMidgardID* groupId,
    const game::CMidgardID* itemId)
{
    using namespace game;

    const auto& itemCategories = ItemCategories::get();

    auto inventory = getInventory(objectMap, groupId);
    if (inventory->vftable->getItemIndex(inventory, itemId) == -1) {
        return CanApplyPotionResult::NoItemInInventory;
    }

    auto unit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));

    auto itemBase = getGlobalItemById(objectMap, itemId);
    auto itemCategory = itemBase->vftable->getCategory(itemBase);
    if (itemCategory->id == itemCategories.potionBoost->id
        || itemCategory->id == itemCategories.potionPermanent->id) {
        if (unit->currentHp <= 0) {
            return CanApplyPotionResult::CannotBoostDead;
        }

        auto potionBoost = castItemToPotionBoost(itemBase);
        if (hasModifier(unit->unitImpl, potionBoost->vftable->getModifierId(potionBoost))) {
            return CanApplyPotionResult::AlreadyApplied;
        }
    } else if (itemCategory->id == itemCategories.potionHeal->id) {
        if (unit->currentHp <= 0) {
            return CanApplyPotionResult::CannotHealDead;
        } else if (unit->currentHp >= getUnitHpMax(unit)) {
            return CanApplyPotionResult::AlreadyAtFullHp;
        }
    } else if (itemCategory->id == itemCategories.potionRevive->id) {
        if (unit->currentHp > 0) {
            return CanApplyPotionResult::AlreadyAlive;
        }
    } else {
        return CanApplyPotionResult::NotAPotion;
    }

    return CanApplyPotionResult::Ok;
}

void __stdcall getUnitRequiredBuildingsHooked(const game::IMidgardObjectMap* objectMap,
                                              const game::CMidgardID* playerId,
                                              const game::IUsUnit* unitImpl,
                                              game::Vector<game::TBuildingType*>* result)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& globalDataApi = GlobalDataApi::get();
    const auto& intVectorApi = IntVectorApi::get();

    const GlobalData* globalData = *globalDataApi.getGlobalData();
    auto player = getPlayer(objectMap, playerId);

    const auto& units = globalData->units->map->data;
    const auto& buildings = (*globalData->buildings)->data;
    for (auto building = buildings.bgn; building != buildings.end; ++building) {
        for (auto unit = units.bgn; unit != units.end; ++unit) {
            auto racialSoldier = fn.castUnitImplToRacialSoldier(unit->second);
            if (racialSoldier) {
                auto upgradeBuildingId = racialSoldier->vftable->getUpgradeBuildingId(
                    racialSoldier);
                if (*upgradeBuildingId == building->first) {
                    auto prevUnitImplId = racialSoldier->vftable->getPrevUnitImplId(racialSoldier);
                    if (*prevUnitImplId == unitImpl->id) {
                        if (!player || lordHasBuilding(&player->lordId, &building->first)) {
                            intVectorApi.pushBack((IntVector*)result, (int*)&building->second);
                        }
                    }
                }
            }
        }
    }
}

const game::TUsUnitImpl* __stdcall getUpgradeUnitImplCheckXpHooked(
    const game::IMidgardObjectMap* objectMap,
    const game::CMidUnit* unit)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    if (unit->currentXp < soldier->vftable->getXpNext(soldier)) {
        return nullptr;
    }

    return getUpgradeUnitImpl(objectMap, getPlayerByUnitId(objectMap, &unit->id), unit);
}

bool __stdcall changeUnitXpCheckUpgradeHooked(game::IMidgardObjectMap* objectMap,
                                              const game::CMidgardID* playerId,
                                              const game::CMidgardID* unitId,
                                              int amount)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& visitors = VisitorApi::get();

    auto unit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    int xpNext = soldier->vftable->getXpNext(soldier);

    int xpAmount = amount;
    if (unit->currentXp + xpAmount >= xpNext) {
        if (!getUpgradeUnitImpl(objectMap, getPlayer(objectMap, playerId), unit)) {
            xpAmount = xpNext - unit->currentXp - 1;
        }
    }

    return visitors.changeUnitXp(unitId, xpAmount, objectMap, 1);
}

bool __stdcall isUnitTierMaxHooked(const game::IMidgardObjectMap* objectMap,
                                   const game::CMidgardID* playerId,
                                   const game::CMidgardID* unitId)
{
    using namespace game;

    const auto& fn = gameFunctions();

    if (fn.isPlayerRaceUnplayable(playerId, objectMap)) {
        return true;
    }

    auto unit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));
    if (unit->dynLevel) {
        return true;
    }

    if (!canUnitGainXp(unit->unitImpl)) {
        return true;
    }

    if (hasMaxTierUpgradeBuilding(objectMap, unit->unitImpl)) {
        return true;
    }

    return hasNextTierUnitImpl(unit->unitImpl) == false;
}

bool __stdcall isUnitLevelNotMaxHooked(const game::IMidgardObjectMap* objectMap,
                                       const game::CMidgardID* playerId,
                                       const game::CMidgardID* unitId)
{
    // Originally calls isUnitTierMax, but it is already getting called before this function
    // everywhere in the game code, so the excessive call is removed from here.

    using namespace game;

    const auto& fn = gameFunctions();

    auto unit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));
    if (!canUnitGainXp(unit->unitImpl)) {
        return false;
    }

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    auto soldierLevel = soldier->vftable->getLevel(soldier);
    if (soldierLevel == getGeneratedUnitImplLevelMax()) {
        return false;
    }

    auto stackLeader = fn.castUnitImplToStackLeader(unit->unitImpl);
    if (stackLeader) {
        auto scenarioInfo = getScenarioInfo(objectMap);
        if (!scenarioInfo) {
            return false;
        }

        return soldierLevel < scenarioInfo->leaderMaxLevel;
    }

    return soldierLevel < *gameRestrictions().unitMaxLevel;
}

bool __stdcall isUnitUpgradePendingHooked(const game::CMidgardID* unitId,
                                          const game::IMidgardObjectMap* objectMap)
{
    using namespace game;

    const auto& fn = gameFunctions();

    auto unit = static_cast<const CMidUnit*>(
        objectMap->vftable->findScenarioObjectById(objectMap, unitId));

    auto soldier = fn.castUnitImplToSoldier(unit->unitImpl);
    if (unit->currentXp == soldier->vftable->getXpNext(soldier) - 1) {
        auto playerId = getPlayerIdByUnitId(objectMap, unitId);
        if (fn.isUnitLevelNotMax(objectMap, &playerId, unitId)) {
            return getUpgradeUnitImpl(objectMap, getPlayer(objectMap, &playerId), unit) == nullptr;
        }
    }

    return false;
}

bool __fastcall editBoxIsCharValidHooked(const game::EditBoxData* thisptr, int /*%edx*/, char ch)
{
    using namespace game;

    // clang-format off
    // These characters are language specific and depend on actual code page being used.
    // For example, in cp-1251 they will represent Russian alphabet.
    // They are hardcoded in game and should work fine on different code pages
    // such as cp-1250, cp-1251
    static const unsigned char languageSpecific[] = {
        0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8,
        0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1,
        0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA,
        0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xC0, 0xC1, 0xC2, 0xC3,
        0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC,
        0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5,
        0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,
        0xDF,
        0xA8, 0xB8, // This allows players to enter 'io' (U+0451) and 'IO' (U+0401) in cp-1251 
        0
    };
    // clang-format on

    // Cast to int using unsigned char
    // so extended symbols (>= 127) are handled correctly by isalpha()
    const int a2 = static_cast<unsigned char>(ch);

    if (!a2 || a2 == VK_ESCAPE || !thisptr->original.allowEnter && (a2 == '\n' || a2 == '\r')) {
        return false;
    }

    const int filter = (int)thisptr->filter;

    const int v2 = filter - 1;
    if (!v2) {
        if (isalpha(a2) || isspace(a2)) {
            return true;
        }

        const char* v10 = strchr((const char*)languageSpecific, a2);
        return v10 != nullptr;
    }

    const int v3 = v2 - 1;
    if (!v3) {
        // Remove two symbols from forbidden list.
        // This allows players to enter 'io' (U+0451) and 'IO' (U+0401) in cp-1251
        static const unsigned char forbiddenSymbols[] = {'`', '^', '~', /* 0xA8, 0xB8,*/ 0};

        if (!strchr((const char*)forbiddenSymbols, a2)) {
            if (isalpha(a2) || isdigit(a2) || isspace(a2) || ispunct(a2)) {
                return true;
            }

            const char* v10 = strchr((const char*)languageSpecific, a2);
            return v10 != nullptr;
        }

        return false;
    }

    const int v4 = v3 - 1;
    if (!v4) {
        return isdigit(a2) != 0;
    }

    const int v5 = v4 - 1;
    if (!v5) {
        if (iscntrl(a2)) {
            return false;
        }

        const char* v8 = strchr("/?*\"<>|+'", a2);
        return !v8;
    }

    const int v6 = v5 - 1;
    if (!v6) {
        if (iscntrl(a2)) {
            return false;
        }

        const char* v8 = strchr("/?*\"<>|+':\\", a2);
        return !v8;
    }

    if (v6 == 1) {
        return isalnum(a2) || strchr((const char*)languageSpecific, a2) != nullptr || a2 == ' '
               || a2 == '\'' || a2 == '_' || a2 == '-';
    } else {
        return true;
    }
}

// Conditions should be checked from most critical to less critical to provide correct flow of unit
// upgrades, AI building decisions and UI messages
game::BuildingStatus __stdcall getBuildingStatusHooked(const game::IMidgardObjectMap* objectMap,
                                                       const game::CMidgardID* playerId,
                                                       const game::CMidgardID* buildingId,
                                                       bool ignoreBuildTurnAndCost)
{
    using namespace game;

    auto player = getPlayer(objectMap, playerId);
    if (playerHasBuilding(objectMap, player, buildingId)) {
        return BuildingStatus::AlreadyBuilt;
    }

    auto building = getBuilding(buildingId);
    auto scenarioInfo = getScenarioInfo(objectMap);
    if (getBuildingLevel(building) > scenarioInfo->unitMaxTier) {
        return BuildingStatus::ExceedsMaxLevel;
    }

    if (!lordHasBuilding(&player->lordId, buildingId)) {
        return BuildingStatus::LordHasNoBuilding;
    }

    if (playerHasSiblingUnitBuilding(objectMap, player, building)) {
        return BuildingStatus::PlayerHasSiblingUnitBuilding;
    }

    auto requiredId = building->data->requiredId;
    if (requiredId != emptyId && !playerHasBuilding(objectMap, player, &requiredId)) {
        return BuildingStatus::PlayerHasNoRequiredBuilding;
    }

    if (!ignoreBuildTurnAndCost) {
        if (player->bank < building->data->cost) {
            return BuildingStatus::InsufficientBank;
        }

        const auto& races = RaceCategories::get();
        const auto raceId = player->raceType->data->raceType.id;
        const char* racePrefix{};
        if (raceId == races.human->id) {
            racePrefix = "EMPIRE_";
        } else if (raceId == races.heretic->id) {
            racePrefix = "LEGIONS_";
        } else if (raceId == races.dwarf->id) {
            racePrefix = "CLANS_";
        } else if (raceId == races.undead->id) {
            racePrefix = "HORDES_";
        } else if (raceId == races.elf->id) {
            racePrefix = "ELVES_";
        }

        auto variables{getScenarioVariables(objectMap)};
        if (variables && variables->variables.length) {
            int multipleBuildValue{0}; 
            const auto expectedName{"MULTIPLE_BUILD_CAPITAL"};
            const auto expectedNameRace{fmt::format("{:s}MULTIPLE_BUILD_CAPITAL", racePrefix)};
            for (const auto& variable : variables->variables) {
                const auto& name = variable.second.name;
                if (!strncmp(name, expectedName, sizeof(name))) {
                    multipleBuildValue = variable.second.value;
                } else if (!strncmp(name, expectedNameRace.c_str(), sizeof(name))) {
                    multipleBuildValue = variable.second.value;
                    break;
                }
            }

            if (multipleBuildValue > 0) {
                return BuildingStatus::CanBeBuilt;
            } else if (multipleBuildValue < 0) {
                return BuildingStatus::PlayerAlreadyBuiltThisDay;
            }
        }

        if (player->constructionTurn == scenarioInfo->currentTurn) {
            return BuildingStatus::PlayerAlreadyBuiltThisDay;
        }
    }

    return BuildingStatus::CanBeBuilt;
}

bool __stdcall removeStackHooked(const game::CMidgardID* stackId,
                                 game::CMidgardPlan* plan,
                                 game::IMidgardObjectMap* objectMap,
                                 game::CScenarioVisitor* visitor)
{
    using namespace game;

    const CMidStack* stack{getStack(objectMap, stackId)};
    if (stack && stack->sourceTemplateId != emptyId) {
        stackTemplateCacheRemove(stack->sourceTemplateId, stack->id);
    }

    return getOriginalFunctions().removeStack(stackId, plan, objectMap, visitor);
}

bool __stdcall setStackSrcTemplateHooked(const game::CMidgardID* stackId,
                                         const game::CMidgardID* stackTemplateId,
                                         game::IMidgardObjectMap* objectMap,
                                         int apply)
{
    const bool result{
        getOriginalFunctions().setStackSrcTemplate(stackId, stackTemplateId, objectMap, apply)};

    if (apply == 1 && result) {
        stackTemplateCacheAdd(*stackTemplateId, *stackId);
    }

    return result;
}

const char* __stdcall getSiteNameSuffixHooked(const game::LSiteCategory* siteCategory)
{
    using namespace game;

    const auto& categories = SiteCategories::get();

    const auto id = siteCategory->id;

    if (id == categories.merchant->id) {
        return "MERH";
    }

    if (id == categories.mageTower->id) {
        return "MAGE";
    }

    if (id == categories.mercenaries->id) {
        return "MERC";
    }

    if (customSiteCategories().exists && id == customSiteCategories().resourceMarket.id) {
        return "RMKT";
    }

    return "TRAI";
}

void __stdcall updateEncLayoutSiteHooked(const game::CMidSite* site, game::CTextBoxInterf* textBox)
{
    if (!site) {
        return;
    }

    using namespace game;

    // \hC;\vC;\fLarge;%NAME%\fNormal;\n%TYPE%\n\n%DESC%
    std::string str{getInterfaceText("X005TA0868")};

    const auto& categories{SiteCategories::get()};
    const auto id{site->siteCategory.id};

    const char* textId{};
    if (id == categories.merchant->id) {
        // (Merchant)
        textId = "X005TA0873";
    } else if (id == categories.mageTower->id) {
        // (Magic shop)
        textId = "X005TA0874";
    } else if (id == categories.mercenaries->id) {
        // (Mercenary)
        textId = "X005TA0875";
    } else if (customSiteCategories().exists && id == customSiteCategories().resourceMarket.id) {
        textId = textIds().resourceMarket.encyDesc.c_str();
    } else {
        // (Trainer)
        textId = "X005TA0876";
    }

    const std::string type{getInterfaceText(textId)};

    const char* name{site->title.string ? site->title.string : ""};
    const char* description{site->description.string ? site->description.string : ""};

    replace(str, "%NAME%", name);
    replace(str, "%TYPE%", type);
    replace(str, "%DESC%", description);

    CTextBoxInterfApi::get().setString(textBox, str.c_str() ? str.c_str() : "");
}

game::String* __stdcall getSiteSoundHooked(game::String* soundName, const game::CMidSite* site)
{
    using namespace game;

    const auto& init{StringApi::get().initFromString};

    const SiteId id{site->siteCategory.id};

    if (customSiteCategories().exists && id == customSiteCategories().resourceMarket.id) {
        init(soundName, "RESMARKT");
        return soundName;
    }

    const auto& sites{SiteCategories::get()};

    if (id == sites.mageTower->id) {
        init(soundName, "MAGICTW");
        return soundName;
    }

    if (id == sites.trainer->id) {
        init(soundName, "TRAINCMP");
        return soundName;
    }

    if (id != sites.merchant->id || site->imgIso) {
        init(soundName, "");
        return soundName;
    }

    init(soundName, "WINDMILL");
    return soundName;
}

bool __stdcall siteHasSoundHooked(const game::CMidSite* site)
{
    using namespace game;

    const auto& sites{SiteCategories::get()};
    const SiteId id{site->siteCategory.id};

    if (id == sites.mageTower->id || id == sites.trainer->id
        || id == sites.merchant->id && !site->imgIso) {
        return true;
    }

    if (customSiteCategories().exists && id == customSiteCategories().resourceMarket.id) {
        return true;
    }

    return false;
}

static void callLuaHook(const char* funcName,
                 game::BattleMsgData* data,
                 const game::IMidgardObjectMap* map,
                 const game::CMidgardID* id)
{
    static const auto scriptPath = scriptsFolder() / "hooks/hooks.lua";
    std::optional<sol::environment> env;

    auto f = getScriptFunction(scriptPath, funcName, env, false, true);

    if (f) {
        try {
            auto* unit = game::gameFunctions().findUnitById(map, id);
            (*f)(bindings::BattleMsgDataView{data, map}, bindings::UnitView{unit});
        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Lua Error in {}: {}", funcName, e.what()));
        }
    }
}

void __fastcall setUnitStatusHooked(const game::BattleMsgData* battleMsgData,
                                    int /*%edx*/,
                                    const game::CMidgardID* unitId,
                                    const int status,
                                    const bool enable)
{
    using namespace game;
    
    const auto& battleApi = BattleMsgDataApi::get();

    auto* battle = const_cast<BattleMsgData*>(battleMsgData);
    auto* objectMap = const_cast<IMidgardObjectMap*>(getObjectMap());
    const auto bStatus = static_cast<BattleStatus>(status);

    getOriginalFunctions().setUnitStatus(battleMsgData, unitId, bStatus, enable);

    switch (bStatus) {
    case BattleStatus::Dead: {
        if (enable) {
            if (gameSettings().instantBuffRemoval != baseGameSettings().instantBuffRemoval) {
                auto* unitInfo = battleApi.getUnitInfoById(battle, unitId);
                if (unitInfo) {
                    for (const auto& modId : getModifiedUnitIds(unitInfo)) {
                        removeModifiers(battle, objectMap, unitInfo, &modId);
                    }
                    resetModifiedUnitsInfo(unitInfo);
                }
            }
            callLuaHook("OnUnitDeath", battle, objectMap, unitId);
        } else {
            callLuaHook("OnResurrection", battle, objectMap, unitId);
        }
        break;
    }
    case BattleStatus::Cured: {
        if (enable && gameSettings().advancedCure != baseGameSettings().advancedCure) {
            battleApi.setUnitStatus(battle, unitId, BattleStatus::LowerDamageLvl1, false);
            battleApi.setUnitStatus(battle, unitId, BattleStatus::LowerDamageLvl2, false);
            battleApi.setUnitStatus(battle, unitId, BattleStatus::LowerDamageLong, false);
            battleApi.setUnitStatus(battle, unitId, BattleStatus::LowerInitiative, false);
            battleApi.setUnitStatus(battle, unitId, BattleStatus::LowerInitiativeLong, false);
        }
        break;
    }
    case BattleStatus::Blister:
    case BattleStatus::Frostbite:
    case BattleStatus::Poison: {
        if (!enable) {
            if (auto* info = battleApi.getUnitInfoById(battle, unitId)) {
                if (bStatus == BattleStatus::Blister) {
                    info->blisterAppliedDamage = 0;
                } else if (bStatus == BattleStatus::Frostbite) {
                    info->frostbiteAppliedDamage = 0;
                } else {
                    info->poisonAppliedDamage = 0;
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

void __fastcall battleEndHooked(game::IBatViewer* thisptr,
                                int /*%edx*/,
                                const game::BattleMsgData* battleMsgData,
                                const game::CMidgardID* a3)
{
    resetPreTurnHookState();
    getOriginalFunctions().battleEnd(thisptr, battleMsgData, a3);
}

bool __stdcall unitCanBeCuredHooked(const game::BattleMsgData* battleMsgData,
                                    const game::CMidgardID* unitId)
{
    using namespace game;
    const auto& battle = BattleMsgDataApi::get();

    if (battle.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerDamageLvl1)
        || battle.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerDamageLvl2)
        || battle.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerInitiative))
        return true;

    return getOriginalFunctions().unitCanBeCured(battleMsgData, unitId);
}

// Lower Damage
bool __fastcall lowerDamageCanPerformHooked(game::CBatAttackLowerDamage* thisptr,
                                            int /*%edx*/,
                                            game::IMidgardObjectMap* objectMap,
                                            game::BattleMsgData* battleMsgData,
                                            game::CMidgardID* unitId)
{
    using namespace game;

    auto& battle = BattleMsgDataApi::get();

    if (*unitId == emptyId || *unitId == invalidId) {
        return false;
    }

    CMidgardID unitGroupId{};
    gameFunctions().getAllyOrEnemyGroupId(&unitGroupId, battleMsgData, unitId, true);

    CMidgardID targetGroupId{};
    thisptr->vftable->getTargetGroupId(thisptr, &targetGroupId, battleMsgData);

    if (battle.getUnitStatus(battleMsgData, unitId, BattleStatus::Dead))
        return false;

    if (targetGroupId != unitGroupId)
        return false;

    int curLvl = 0;

    if (battle.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerDamageLvl2)) {
        curLvl = 2;
    } else if (battle.getUnitStatus(battleMsgData, unitId, BattleStatus::LowerDamageLvl1)) {
        curLvl = 1;
    }

    auto* attack = thisptr->attack;
    return curLvl <= attack->vftable->getLevel(attack);
}

void __fastcall lowerDamageOnHitHooked(game::CBatAttackLowerDamage* thisptr,
                                       int /*%edx*/,
                                       game::IMidgardObjectMap* objectMap,
                                       game::BattleMsgData* battleMsgData,
                                       game::CMidgardID* targetUnitId,
                                       game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const BattleMsgDataApi::Api& battleApi = BattleMsgDataApi::get();

    const auto attack = thisptr->attack;

    CMidUnit* targetUnit = gameFunctions().findUnitById(objectMap, targetUnitId);

    int level = thisptr->attack->vftable->getLevel(thisptr->attack);
    bool isLong = attack->vftable->getInfinite(attack);
    CMidUnit* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.attack = attack;
    params.attackClass = "LowerDamage";
    params.attackLevel = level;
    params.isLong = isLong;
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    level = params.attackLevel;
    isLong = params.isLong;

    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerDamageLvl1, false);
    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerDamageLvl2, false);
    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerDamageLong, false);

    auto status = static_cast<BattleStatus>((int)BattleStatus::LowerDamageLvl1 + (level - 1));

    battleApi.setUnitStatus(battleMsgData, targetUnitId, status, true);

    if (isLong)
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerDamageLong, true);

    addToBattleAttackInfo(*attackInfo, targetUnit);
}

void __fastcall lowerInitiativeOnHitHooked(game::CBatAttackLowerInitiative* thisptr,
                                           int /*%edx*/,
                                           game::IMidgardObjectMap* objectMap,
                                           game::BattleMsgData* battleMsgData,
                                           game::CMidgardID* targetUnitId,
                                           game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const BattleMsgDataApi::Api& battleApi = BattleMsgDataApi::get();

    const auto attack = thisptr->attack;

    CMidUnit* targetUnit = gameFunctions().findUnitById(objectMap, targetUnitId);

    bool isLong = attack->vftable->getInfinite(attack);
    CMidUnit* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.attack = attack;
    params.attackClass = "LowerInitiative";
    params.isLong = isLong;
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    isLong = params.isLong;

    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerInitiative, false);
    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerInitiativeLong, false);

    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerInitiative, true);

    if(isLong)
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::LowerInitiativeLong,
                                true);

    addToBattleAttackInfo(*attackInfo, targetUnit);
}

void __fastcall paralyzeOnHitHooked(game::CBatAttackParalyze* thisptr,
                                    int /*%edx*/,
                                    game::IMidgardObjectMap* objectMap,
                                    game::BattleMsgData* battleMsgData,
                                    game::CMidgardID* targetUnitId,
                                    game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const BattleMsgDataApi::Api& battleApi = BattleMsgDataApi::get();
    static const auto& globalApi = GlobalDataApi::get();
    auto* globalData = *globalApi.getGlobalData();

    const auto attack = thisptr->attack;

    CMidUnit* targetUnit = gameFunctions().findUnitById(objectMap, targetUnitId);

    UnitInfo* targetInfo = battleApi.getUnitInfoById(battleMsgData, targetUnitId);

    bool isLong = attack->vftable->getInfinite(attack);
    CMidUnit* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.attack = attack;
    params.attackClass = "Paralyze";
    params.isLong = isLong;
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    isLong = params.isLong;

    targetInfo->disableAppliedRound = battleMsgData->currentRound;

    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::Paralyze, true);
    if (isLong)
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::DisableLong, true);

    if (gameSettings().extendedBattle.longEffectsUsesPower != baseGameSettings().extendedBattle.longEffectsUsesPower) {
        int level = thisptr->attack->vftable->getLevel(thisptr->attack);
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::DisableLong, true);
        targetInfo->disableAppliedRound = level > 0 ? level : 1;
    }

    addToBattleAttackInfo(*attackInfo, targetUnit);
}

void __fastcall petrifyOnHitHooked(game::CBatAttackPetrify* thisptr,
                                    int /*%edx*/,
                                    game::IMidgardObjectMap* objectMap,
                                    game::BattleMsgData* battleMsgData,
                                    game::CMidgardID* targetUnitId,
                                    game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const BattleMsgDataApi::Api& battleApi = BattleMsgDataApi::get();
    static const auto& globalApi = GlobalDataApi::get();
    auto* globalData = *globalApi.getGlobalData();

    const auto attack = thisptr->attack;

    CMidUnit* targetUnit = gameFunctions().findUnitById(objectMap, targetUnitId);

    UnitInfo* targetInfo = battleApi.getUnitInfoById(battleMsgData, targetUnitId);

    bool isLong = attack->vftable->getInfinite(attack);
    CMidUnit* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.attack = attack;
    params.attackClass = "Petrify";
    params.isLong = isLong;
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    isLong = params.isLong;

    targetInfo->disableAppliedRound = battleMsgData->currentRound;
    battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::Petrify, true);
    if (isLong)
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::DisableLong, true);

    if (gameSettings().extendedBattle.longEffectsUsesPower
        != baseGameSettings().extendedBattle.longEffectsUsesPower) {
        int level = thisptr->attack->vftable->getLevel(thisptr->attack);
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::DisableLong, true);
        targetInfo->disableAppliedRound = level > 0 ? level : 1;
    }

    addToBattleAttackInfo(*attackInfo, targetUnit);
}

void __fastcall fearAttackOnHitHooked(game::CBatAttackFear* thisptr,
                                    int /*%edx*/,
                                    game::IMidgardObjectMap* objectMap,
                                    game::BattleMsgData* battleMsgData,
                                    game::CMidgardID* targetUnitId,
                                    game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const auto& battleApi = BattleMsgDataApi::get();

    const auto attack = thisptr->attack;

    CMidUnit* targetUnit = gameFunctions().findUnitById(objectMap, targetUnitId);
    if (!targetUnit)
        return;

    bool canRetreat = false;
    const CMidStack* stack = getStackByUnitId(objectMap, targetUnitId);

    if (stack && stack->insideId == emptyId)
    {
        canRetreat = true;
    }

    CMidUnit* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.attack = attack;
    params.attackClass = "Fear";
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    targetUnitId = &params.target->id;

    if (canRetreat)
    {
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::Retreat, true);
    }
    else
    {
        UnitInfo* targetInfo = battleApi.getUnitInfoById(battleMsgData, targetUnitId);
        battleApi.setUnitStatus(battleMsgData, targetUnitId, BattleStatus::Paralyze, true);
        targetInfo->disableAppliedRound = battleMsgData->currentRound;
    }

    addToBattleAttackInfo(*attackInfo, targetUnit);
}

void __fastcall showAttackEffectHooked(game::IBatViewer* thisptr,
    int /*%edx*/,
    const game::BattleMsgData* battleMsgData,
    const game::BattleAttackInfo** attackInfo,
    const game::LAttackClass* attackClass)
{
    using namespace game;

    if (!g_pendingBattleEffects.empty() && attackInfo && *attackInfo) {
        auto* info = const_cast<BattleAttackInfo*>(*attackInfo);
        for (const auto& pending : g_pendingBattleEffects) {
            info->unitId = pending.unitId;
            info->effect = pending.effect;
        }
        g_pendingBattleEffects.clear();
    }

    getOriginalFunctions().showAttackEffect(thisptr, battleMsgData, attackInfo,
                                                    attackClass);
}

void __fastcall defendOnHitHooked(game::CBatAttackDefend* thisptr,
                                  int /*%edx*/,
                                  game::IMidgardObjectMap* objectMap,
                                  game::BattleMsgData* battleMsgData,
                                  game::CMidgardID* targetUnitId,
                                  game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const BattleMsgDataApi::Api& battleApi = BattleMsgDataApi::get();

    CMidUnit* unit = gameFunctions().findUnitById(objectMap, targetUnitId);

    bindings::AttackHitParamsView params;
    params.attacker = unit;
    params.target = unit;
    params.attack = nullptr;
    params.attackClass = "Defend";
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss)
    {
        auto newAttackInfo = *attackInfo;
        newAttackInfo->effect = AttackEffect::Effect18;
        return;
    }
    
    IUsSoldier* soldier = gameFunctions().castUnitImplToSoldier(unit->unitImpl);
    bool hasDoubleAttack = soldier->vftable->getAttackTwice(soldier);

    for (int i = 0; i < 13; i++) {
        if (battleMsgData->turnsOrder[i].unitId == thisptr->unitId
            && battleMsgData->turnsOrder[i].attackCount > 0) {
            battleMsgData->turnsOrder[i].attackCount = hasDoubleAttack ? 2 : 1;
            break;
        }
    }

    UnitInfo* unitInfo = battleApi.getUnitInfoById(battleMsgData, targetUnitId);
    unitInfo->unitFlags.parts.attackedOnceOfTwice = false;

    battleApi.setUnitStatus(battleMsgData, &thisptr->unitId, BattleStatus::Defend, true);

    BattleAttackUnitInfo info{};
    info.unitId = *targetUnitId;
    info.unitImplId = unit->unitImpl->id;
    info.attackMissed = false;
    info.damage = 0;

    BattleAttackInfoApi::get().addUnitInfo(&(*attackInfo)->unitsInfo, &info);
}

void __fastcall retreatAttackOnHitHooked(game::CBatAttackRetreat* thisptr,
                                         int /*%edx*/,
                                         game::IMidgardObjectMap* objectMap,
                                         game::BattleMsgData* battleMsgData,
                                         game::CMidgardID* targetUnitId,
                                         game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const BattleMsgDataApi::Api& battleApi = BattleMsgDataApi::get();

    const auto& settings = gameSettings();
    UnitInfo* unitInfo = battleApi.getUnitInfoById(battleMsgData, targetUnitId);

    if (unitInfo) {
        if (unitInfo->unitFlags.parts.waited)
            unitInfo->unitFlags.parts.retreatingAfterWait;
    }

    bool fastRetreat = false;
    CMidUnit* midUnit = gameFunctions().findUnitById(objectMap, targetUnitId);
    IUsUnit* unitImpl = midUnit->unitImpl;

    IUsStackLeader* leader = gameFunctions().castUnitImplToStackLeader(unitImpl);
    if (leader)
        fastRetreat = leader->vftable->getFastRetreat(leader);

    bindings::AttackHitParamsView params;
    params.attacker = midUnit;
    params.target = midUnit;
    params.attack = nullptr;
    params.attackClass = "Retreat";
    params.fastRetreat = fastRetreat;
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        (*attackInfo)->effect = AttackEffect::Effect18;
        return;
    }

    fastRetreat = params.fastRetreat;

    if (fastRetreat)
    {
        battleApi.setUnitStatus(battleMsgData, &thisptr->unitId, BattleStatus::Retreated, true);
        (*attackInfo)->effect = AttackEffect::Retreated;
    }
    else
    {
        battleApi.setUnitStatus(battleMsgData, &thisptr->unitId, BattleStatus::Retreat, true);
    }
}

void __fastcall waitAttackOnHitHooked(game::CBatAttackWait* thisptr,
                                      int /*%edx*/,
                                      game::IMidgardObjectMap* objectMap,
                                      game::BattleMsgData* battleMsgData,
                                      game::CMidgardID* targetUnitId,
                                      game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const BattleMsgDataApi::Api& battleApi = BattleMsgDataApi::get();
    
    int curAttackCount = battleMsgData->turnsOrder[0].attackCount;

    CMidUnit* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = unitAttacker;
    params.attack = nullptr;
    params.attackCount = curAttackCount;
    params.attackClass = "Wait";

    callLuaAttackHook(objectMap, battleMsgData, params);

    curAttackCount = params.attackCount;

    for (int i = 0; i < 13; i++)
    {
        if (battleMsgData->turnsOrder[i].unitId == invalidId)
        {
            battleMsgData->turnsOrder[i].unitId = thisptr->unitId;
            battleMsgData->turnsOrder[i].attackCount = curAttackCount;
            break;
        }
    }

    UnitInfo* unit = battleApi.getUnitInfoById(battleMsgData, &thisptr->unitId);
    unit->unitFlags.parts.waited = true;
}

bool __stdcall checkLongEffectDurationHooked(int roundsPassed)
{
    if (roundsPassed == 0)
        return false;

    const auto& chances = gameSettings().longEffectRemoveChances;
    const auto& baseChances = baseGameSettings().longEffectRemoveChances;
    const auto& actual = chances.empty() ? baseChances : chances;

    int index = roundsPassed - 1;

    int chance;
    if (index < (int)actual.size())
        chance = actual[index];
    else
        chance = 100;

    return game::gameFunctions().generateRandomNumber(100) < chance;
}

void __fastcall damageAttackOnHitHooked(game::CBatAttackDamage* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::BattleMsgData* battleMsgData,
                                        game::CMidgardID* targetUnitId,
                                        game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    static const auto& idApi = CMidgardIDApi::get();
    static const auto& battleApi = BattleMsgDataApi::get();
    static const auto& visitor = VisitorApi::get();
    static const auto& fn = gameFunctions();

    CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    if (!targetUnit)
        return;

    auto* unitAttacker = game::gameFunctions().findUnitById(objectMap, &thisptr->unitId);

    int normalDamage = 0;
    int critDamage = 0;
    int totalDamage = fn.computeDamage(objectMap, battleMsgData, thisptr->attack, &thisptr->unitId,
                                       targetUnitId, true, &normalDamage, &critDamage);

    int drainAmount = thisptr->attack->vftable->getDrain(thisptr->attack, totalDamage);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.attack = thisptr->attack;
    params.attackClass = "Damage";
    params.damage = normalDamage;
    params.critDamage = critDamage;
    params.drain = drainAmount;
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss)
    {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    normalDamage = params.damage;
    critDamage = params.critDamage;
    drainAmount = params.drain;
    totalDamage = normalDamage + critDamage;

    int hpBefore = targetUnit->currentHp;
    visitor.changeUnitHp(targetUnitId, -totalDamage, objectMap, 1);
    battleApi.checkUnitDeath(objectMap, battleMsgData, targetUnitId);
    battleApi.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);

    if (drainAmount > 0) {
        CMidUnit* attackerUnit = fn.findUnitById(objectMap, &thisptr->unitId);

        if (attackerUnit) {
            int actualDrain = std::min(drainAmount, hpBefore - targetUnit->currentHp);
            if (actualDrain > 0) {
                visitor.changeUnitHp(&thisptr->unitId, actualDrain, objectMap, 1);
                battleApi.setUnitHp(battleMsgData, &thisptr->unitId, attackerUnit->currentHp);
            }
        }
    }

    addToBattleAttackInfo(*attackInfo, targetUnit, normalDamage, critDamage);

}

//void __fastcall usePotionAttackOnHitHooked(game::CBatAttackUsePotion* thisptr,
//    int /*%edx*/,
//    game::IMidgardObjectMap* objectMap,
//    game::BattleMsgData* battleMsgData,
//    game::CMidgardID* targetUnitId,
//    game::BattleAttackInfo** attackInfo)
//{
//    using namespace game;
//
//    const auto& fn = gameFunctions();
//    const auto& battleApi = BattleMsgDataApi::get();
//    const auto& itemCategories = ItemCategories::get();
//    const auto& visitor = VisitorApi::get();
//
//    CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
//
//    UnitInfo* unitInfo = battleApi.getUnitInfoById(battleMsgData, &thisptr->unitId);
//
//    auto groupId = unitInfo->unitFlags.parts.attacker ? &battleMsgData->attackerGroupId
//                                                      : &battleMsgData->defenderGroupId;
//
//    if (CMidgardIDApi::get().getType(groupId) != IdType::Stack)
//        return;
//
//    auto stack = getStack(objectMap, groupId);
//    if (!stack)
//        return;
//
//    int itemIndex = battleApi.getLeaderEquippedBattleItemIndex(objectMap, &thisptr->unitId, battleMsgData, &thisptr->itemId);
//
//    visitor.unEquipItem(&stack->id, itemIndex, objectMap, 1);
//
//    CMidItem* item = static_cast<CMidItem*>(objectMap->vftable->findScenarioObjectById(objectMap, &thisptr->itemId));
//
//    const auto& global = GlobalDataApi::get();
//    auto globalData = *global.getGlobalData();
//
//    const CItemBase* globalItem = global.findItemById(globalData->itemTypes, &item->globalItemId);
//    const LItemCategory* itemCategory = globalItem->vftable->getCategory(globalItem);
//
//    int totalHeal = 0;
//
//    if (itemCategory == itemCategories.potionHeal)
//    {
//        const CItemPotionHeal* potion = static_cast<const CItemPotionHeal*>(globalItem);
//        visitor.changeUnitHp(targetUnitId, potion->quantityHp, objectMap, 1);
//        totalHeal = potion->quantityHp;
//    }
//    else if (itemCategory == itemCategories.potionRevive)
//    {
//        visitor.reviveUnit(targetUnitId, objectMap, 1);
//    } else if (itemCategory == itemCategories.potionBoost) {
//        auto potionBoost = castItemToPotionBoost(globalItem);
//        const CMidgardID* effectId = potionBoost->vftable->getModifierId(potionBoost);
//
//        auto* info = getScenarioInfo(objectMap);
//        visitor.addSpellEffect(targetUnitId, &item->globalItemId, &stack->ownerId, effectId,
//                               &info->currentTurn, objectMap, 1);
//    }
//    else if (itemCategory == itemCategories.potionPermanent)
//    {
//        auto potionBoost = castItemToPotionBoost(globalItem);
//        const CMidgardID* effect = potionBoost->vftable->getModifierId(potionBoost);
//        hooks::addModifier(targetUnit, effect, true);
//    }
//
//    visitor.equipItem(&stack->id, &thisptr->itemId, itemIndex, objectMap, 1);
//
//    for (int i = 0; i < 4; i++) {
//        if (battleMsgData->usedItemIds[i] == thisptr->itemId)
//            break;
//        if (battleMsgData->usedItemIds[i] == invalidId) {
//            battleMsgData->usedItemIds[i] = thisptr->itemId;
//            break;
//        }
//    }
//
//    addToBattleAttackInfo(*attackInfo, targetUnit, totalHeal, 0);
//
//    battleApi.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
//}

void __fastcall usePotionAttackOnHitHooked(game::CBatAttackUsePotion* thisptr,
                                           int /*%edx*/,
                                           game::IMidgardObjectMap* objectMap,
                                           game::BattleMsgData* battleMsgData,
                                           game::CMidgardID* targetUnitId,
                                           game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& itemCategories = ItemCategories::get();
    const auto& visitor = VisitorApi::get();
    const auto& midgardIdApi = CMidgardIDApi::get();

    CMidUnit* targetUnit = fn.findUnitById(objectMap, targetUnitId);
    CMidUnit* unitAttacker = fn.findUnitById(objectMap, &thisptr->unitId);

    UnitInfo* unitInfo = battleApi.getUnitInfoById(battleMsgData, &thisptr->unitId);

    auto* groupId = unitInfo->unitFlags.parts.attacker ? &battleMsgData->attackerGroupId
                                                       : &battleMsgData->defenderGroupId;

    if (midgardIdApi.getType(groupId) != IdType::Stack)
        return;

    auto* stack = getStack(objectMap, groupId);
    if (!stack)
        return;

    int itemIndex = battleApi.getLeaderEquippedBattleItemIndex(objectMap, &thisptr->unitId,
                                                               battleMsgData, &thisptr->itemId);

    visitor.unEquipItem(&stack->id, itemIndex, objectMap, 1);

    auto* item = static_cast<CMidItem*>(
        objectMap->vftable->findScenarioObjectById(objectMap, &thisptr->itemId));

    const auto& global = GlobalDataApi::get();
    const GlobalData* globalData = *global.getGlobalData();
    const CItemBase* globalItem = global.findItemById(globalData->itemTypes, &item->globalItemId);
    const LItemCategory* itemCategory = globalItem->vftable->getCategory(globalItem);

    bool isRevive = false;
    bool isTemp = false;
    int hpPotion = 0;
    CMidgardID modPotion = invalidId;

    if (itemCategory == itemCategories.potionHeal) {
        const auto* potion = static_cast<const CItemPotionHealPatched*>(globalItem);
        hpPotion = potion->hpPotion;
        modPotion = potion->modPotion;
    } else if (itemCategory == itemCategories.potionRevive) {
        const auto* potion = static_cast<const CItemPotionRevivePatched*>(globalItem);
        hpPotion = potion->hpPotion;
        modPotion = potion->modPotion;
        isRevive = true;
    } else if (itemCategory == itemCategories.potionBoost) {
        const auto* potion = static_cast<const CItemPotionBoostTempPatched*>(globalItem);
        hpPotion = potion->hpPotion;
        modPotion = potion->modPotion;
        isTemp = true;
    } else if (itemCategory == itemCategories.potionPermanent) {
        const auto* potion = static_cast<const CItemPotionBoostPermPatched*>(globalItem);
        hpPotion = potion->hpPotion;
        modPotion = potion->modPotion;
    }

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.itemId = &thisptr->itemId;
    params.attack = nullptr;
    params.attackClass = "UsePotion";
    params.heal = hpPotion;
    params.isRevive = isRevive;
    params.isTemp = isTemp;
    params.miss = false;
    params.modifierIds.push_back(&modPotion);

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    if (params.isRevive)
        visitor.reviveUnit(targetUnitId, objectMap, 1);

    visitor.changeUnitHp(targetUnitId, params.heal, objectMap, 1);

    if (params.isTemp) {
        auto* info = getScenarioInfo(objectMap);
        for (const CMidgardID* modifierId : params.modifierIds) {
            visitor.addSpellEffect(targetUnitId, &item->globalItemId, &stack->ownerId, modifierId,
                                   &info->currentTurn, objectMap, 1);
        }
    } else {
        for (const CMidgardID* modifierId : params.modifierIds) {
            hooks::addModifier(targetUnit, modifierId, true);
        }
    }

    battleApi.checkUnitDeath(objectMap, battleMsgData, targetUnitId);

    visitor.equipItem(&stack->id, &thisptr->itemId, itemIndex, objectMap, 1);

    for (int i = 0; i < 4; ++i) {
        if (battleMsgData->usedItemIds[i] == thisptr->itemId)
            break;
        if (battleMsgData->usedItemIds[i] == invalidId) {
            battleMsgData->usedItemIds[i] = thisptr->itemId;
            break;
        }
    }

    addToBattleAttackInfo(*attackInfo, targetUnit, params.heal, 0);
    battleApi.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);

    bool freeTurn = gameSettings().extendedBattle.itemExtraTurn != baseGameSettings().extendedBattle.itemExtraTurn;
    if (freeTurn && !isItemUsed) {
        battleMsgData->turnsOrder[0].attackCount++;
        isItemUsed = true;
    }
}

void callLuaAttackHook(game::IMidgardObjectMap* objectMap,
                       game::BattleMsgData* battleMsgData,
                       bindings::AttackHitParamsView& params)
{
    static const auto scriptPath = scriptsFolder() / "hooks/hooks.lua";
    std::optional<sol::environment> env;

    auto f = getScriptFunction(scriptPath, "OnAttackHit", env, false, true);

    if (f) {
        try {
            bindings::BattleMsgDataView battle{battleMsgData, objectMap};

            (*f)(battle, params);

        } catch (const std::exception& e) {
            showErrorMessageBox(fmt::format("Lua Error in {}: {}", "OnAttackHit", e.what()));
        }
    }
}

void __fastcall useTalismanAttackOnHitHooked(game::CBatAttackUseTalisman* thisptr,
                                             int /*%edx*/,
                                             game::IMidgardObjectMap* objectMap,
                                             game::BattleMsgData* battleMsgData,
                                             game::CMidgardID* targetUnitId,
                                             game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& midgardIdApi = CMidgardIDApi::get();

    CMidUnit* targetUnit = nullptr;

    CMidgardID summonId;
    midgardIdApi.isSummonUnitId(&summonId, targetUnitId);
    bool isNotSummon = summonId == emptyId;

    if(isNotSummon)
        targetUnit = fn.findUnitById(objectMap, targetUnitId);

    const CMidgardID* attacker = &battleMsgData->turnsOrder[0].unitId;
    CMidUnit* unitAttacker = fn.findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.itemId = &thisptr->itemId;
    params.attack = nullptr;
    params.attackClass = "UseTalisman";
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    IBatAttack* attack = *(IBatAttack**)&thisptr->batAttack;

    attack->vftable->onHit(attack, objectMap, battleMsgData, targetUnitId, attackInfo);

    (*attackInfo)->id = thisptr->itemId;

    for (int i = 0; i < 4; ++i) {
        if (battleMsgData->usedItemIds[i] == thisptr->itemId)
            break;
        if (battleMsgData->usedItemIds[i] == invalidId) {
            battleMsgData->usedItemIds[i] = thisptr->itemId;
            break;
        }
    }

    if (targetUnit)
    {
        battleApi.checkUnitDeath(objectMap, battleMsgData, targetUnitId);
        battleApi.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
    }

    if (battleApi.isAutoBattle(battleMsgData)) {
        return;
    }

    bool freeTurn = gameSettings().extendedBattle.itemExtraTurn != baseGameSettings().extendedBattle.itemExtraTurn;
    if (freeTurn && !isItemUsed)
    {
        battleMsgData->turnsOrder[0].attackCount++;
        isItemUsed = true;
    }
}

void __fastcall useOrbAttackOnHitHooked(game::CBatAttackUseOrb* thisptr,
                                        int /*%edx*/,
                                        game::IMidgardObjectMap* objectMap,
                                        game::BattleMsgData* battleMsgData,
                                        game::CMidgardID* targetUnitId,
                                        game::BattleAttackInfo** attackInfo)
{
    using namespace game;

    const auto& fn = gameFunctions();
    const auto& battleApi = BattleMsgDataApi::get();
    const auto& midgardIdApi = CMidgardIDApi::get();

    CMidUnit* targetUnit = nullptr;

    CMidgardID summonId;
    midgardIdApi.isSummonUnitId(&summonId, targetUnitId);
    bool isNotSummon = summonId == emptyId;

    if (isNotSummon)
        targetUnit = fn.findUnitById(objectMap, targetUnitId);

    const CMidgardID* attacker = &battleMsgData->turnsOrder[0].unitId;
    CMidUnit* unitAttacker = fn.findUnitById(objectMap, &thisptr->unitId);

    bindings::AttackHitParamsView params;
    params.attacker = unitAttacker;
    params.target = targetUnit;
    params.itemId = &thisptr->itemId;
    params.attack = nullptr;
    params.attackClass = "UseOrb";
    params.miss = false;

    callLuaAttackHook(objectMap, battleMsgData, params);

    if (params.miss) {
        addToBattleAttackInfo(*attackInfo, targetUnit, 0, 0, true);
        return;
    }

    IBatAttack* attack = *(IBatAttack**)&thisptr->batAttack;

    attack->vftable->onHit(attack, objectMap, battleMsgData, targetUnitId, attackInfo);

    (*attackInfo)->id = thisptr->itemId;

    for (int i = 0; i < 4; ++i) {
        if (battleMsgData->usedItemIds[i] == thisptr->itemId)
            break;
        if (battleMsgData->usedItemIds[i] == invalidId) {
            battleMsgData->usedItemIds[i] = thisptr->itemId;
            break;
        }
    }

    if (targetUnit) {
        battleApi.checkUnitDeath(objectMap, battleMsgData, targetUnitId);
        battleApi.setUnitHp(battleMsgData, targetUnitId, targetUnit->currentHp);
    }

    bool freeTurn = gameSettings().extendedBattle.itemExtraTurn != baseGameSettings().extendedBattle.itemExtraTurn;
    if (freeTurn && !isItemUsed) {
        battleMsgData->turnsOrder[0].attackCount++;
        isItemUsed = true;
    }
}

//Not used
bool __fastcall decreaseUnitAttacksHooked(game::BattleMsgData* battleMsgData,
                                          int /*%edx*/,
                                          const game::CMidgardID* unitId)
{
    using namespace game;

    if (!unitId || *unitId == game::emptyId || *unitId == game::invalidId)
        return false;

    int index = -1;
    for (int i = 0; i < 13; i++) {
        if (battleMsgData->turnsOrder[i].unitId == *unitId) {
            index = i;
            break;
        }
    }

    // Important fix
    if (index <= -1)
        return false;

    battleMsgData->turnsOrder[index].attackCount--;

    if (battleMsgData->turnsOrder[index].attackCount <= 0) {
        while (index < 12) {
            battleMsgData->turnsOrder[index] = battleMsgData->turnsOrder[index + 1];
            index++;
        }
        battleMsgData->turnsOrder[12].unitId = game::emptyId;
        battleMsgData->turnsOrder[12].attackCount = 0;
    }

    battleMsgData->turnsOrder[12].unitId = game::invalidId;
    battleMsgData->turnsOrder[12].attackCount = 0;

    return true;
}

void __stdcall applyCBatAttackUntransformEffectHooked(game::IMidgardObjectMap* objectMap,
                                                      game::CMidgardID* unitId,
                                                      game::BattleMsgData* battleMsgData,
                                                      game::CResultSender* resultSender,
                                                      char sendResult)
{
    getOriginalFunctions().applyCBatAttackUntransformEffect(objectMap, unitId, battleMsgData,
                                                            resultSender, sendResult);
}

game::IMqImage2* __stdcall getSpellAreaFogImageHooked(bool spellAllowed)
{
    if (gameSettings().fogSpellHideEnemyVision) {
        spellAllowed = true;
    }
    return getOriginalFunctions().getSpellAreaFogImage(spellAllowed);
}

} // namespace hooks
