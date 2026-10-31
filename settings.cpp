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

#include "settings.h"
#include "battlemsgdata.h"
#include "scripts.h"
#include "utils.h"
#include <algorithm>
#include <limits>
#include <spdlog/spdlog.h>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace hooks {

template <typename T>
static T readSetting(const sol::table& table,
                     const char* name,
                     T def,
                     T min = std::numeric_limits<T>::min(),
                     T max = std::numeric_limits<T>::max())
{
    return std::clamp<T>(table.get_or(name, def), min, max);
}

static std::string readSetting(const sol::table& table, const char* name, const std::string& def)
{
    return table.get_or(name, def);
}


static void readAiAttackPowerSettings(const sol::table& table, Settings::AiAttackPowerBonus& value)
{
    const auto& def = defaultGameSettings().aiAttackPowerBonus;

    auto bonuses = table.get<sol::optional<sol::table>>("aiAccuracyBonus");
    if (!bonuses.has_value()) {
        value = def;
        return;
    }

    value.absolute = readSetting(bonuses.value(), "absolute", def.absolute);
    value.easy = readSetting(bonuses.value(), "easy", def.easy);
    value.average = readSetting(bonuses.value(), "average", def.average);
    value.hard = readSetting(bonuses.value(), "hard", def.hard);
    value.veryHard = readSetting(bonuses.value(), "veryHard", def.veryHard);
}

static void readAllowBattleItemsSettings(const sol::table& table, Settings::AllowBattleItems& value)
{
    const auto& def = defaultGameSettings().allowBattleItems;

    auto category = table.get<sol::optional<sol::table>>("allowBattleItems");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.onTransformOther = readSetting(category.value(), "onTransformOther",
                                         def.onTransformOther);
    value.onTransformSelf = readSetting(category.value(), "onTransformSelf", def.onTransformSelf);
    value.onDrainLevel = readSetting(category.value(), "onDrainLevel", def.onDrainLevel);
    value.onDoppelganger = readSetting(category.value(), "onDoppelganger", def.onDoppelganger);
}

static void readModifierSettings(const sol::table& table, Settings::Modifiers& value)
{
    const auto& def = defaultGameSettings().modifiers;

    auto category = table.get<sol::optional<sol::table>>("modifiers");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.cumulativeUnitRegeneration = readSetting(category.value(), "cumulativeUnitRegeneration",
                                                   def.cumulativeUnitRegeneration);
    value.notifyModifiersChanged = readSetting(category.value(), "notifyModifiersChanged",
                                               def.notifyModifiersChanged);
    value.validateUnitsOnGroupChanged = readSetting(category.value(), "validateUnitsOnGroupChanged",
                                                    def.validateUnitsOnGroupChanged);
}


static void readWaterMoveCostSettings(const sol::table& table, Settings::MovementCost::Water& water)
{
    const auto& def = defaultGameSettings().movementCost.water;

    water.dflt = readSetting(table, "default", def.dflt, 1);
    water.deadLeader = readSetting(table, "withDeadLeader", def.deadLeader, 1);
    water.withBonus = readSetting(table, "withBonus", def.withBonus, 1);
    water.waterOnly = readSetting(table, "waterOnly", def.waterOnly, 1);
}

static void readForestMoveCostSettings(const sol::table& table,
                                       Settings::MovementCost::Forest& forest)
{
    const auto& def = defaultGameSettings().movementCost.forest;

    forest.dflt = readSetting(table, "default", def.dflt, 1);
    forest.deadLeader = readSetting(table, "withDeadLeader", def.deadLeader, 1);
    forest.withBonus = readSetting(table, "withBonus", def.withBonus, 1);
}

static void readPlainMoveCostSettings(const sol::table& table, Settings::MovementCost::Plain& plain)
{
    const auto& def = defaultGameSettings().movementCost.plain;

    plain.dflt = readSetting(table, "default", def.dflt, 1);
    plain.deadLeader = readSetting(table, "withDeadLeader", def.deadLeader, 1);
    plain.onRoad = readSetting(table, "onRoad", def.onRoad, 1);
}

static void readMovementCostSettings(const sol::table& table, Settings::MovementCost& value)
{

    auto moveCost = table.get<sol::optional<sol::table>>("movementCost");
    if (!moveCost.has_value()) {
        return;
    }

    auto water = moveCost.value().get<sol::optional<sol::table>>("water");
    if (water.has_value()) {
        readWaterMoveCostSettings(water.value(), value.water);
    }

    auto forest = moveCost.value().get<sol::optional<sol::table>>("forest");
    if (forest.has_value()) {
        readForestMoveCostSettings(forest.value(), value.forest);
    }

    auto plain = moveCost.value().get<sol::optional<sol::table>>("plain");
    if (plain.has_value()) {
        readPlainMoveCostSettings(plain.value(), value.plain);
    }
  
}

static void readDebugSettings(const sol::table& table, Settings::Debug& value)
{
    const auto& def = defaultGameSettings().debug;

    // 'debug' is reserved to Lua standard debug library
    auto category = table.get<sol::optional<sol::table>>("debugging");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.sendObjectsChangesTreshold = readSetting(category.value(), "sendObjectsChangesTreshold",
                                                   def.sendObjectsChangesTreshold);
    value.logSinglePlayerMessages = readSetting(category.value(), "logSinglePlayerMessages",
                                                def.logSinglePlayerMessages);
}

static void readEngineSettings(const sol::table& table, Settings::Engine& value)
{
    const auto& def = defaultGameSettings().engine;

    auto category = table.get<sol::optional<sol::table>>("engine");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.sendRefreshInfoObjectCountLimit = readSetting(category.value(),
                                                        "sendRefreshInfoObjectCountLimit",
                                                        def.sendRefreshInfoObjectCountLimit);
}

static void readBattleSettings(const sol::table& table, Settings::Battle& value)
{
    const auto& def = defaultGameSettings().battle;

    auto category = table.get<sol::optional<sol::table>>("battle");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.allowRetreatedUnitsToUpgrade = readSetting(category.value(),
                                                     "allowRetreatedUnitsToUpgrade",
                                                     def.allowRetreatedUnitsToUpgrade);
    value.carryXpOverUpgrade = readSetting(category.value(), "carryXpOverUpgrade",
                                           def.carryXpOverUpgrade);
    value.allowMultiUpgrade = readSetting(category.value(), "allowMultiUpgrade",
                                          def.allowMultiUpgrade);
    value.debugAi = readSetting(category.value(), "debugAi", def.debugAi);
    value.fallbackAction = readSetting(category.value(), "fallbackAction", def.fallbackAction,
                                       game::BattleAction::Attack, game::BattleAction::UseItem);
}

static void readAdditionalLordIncomeSettings(const sol::table& table,
                                             Settings::AdditionalLordIncome& value)
{
    const auto& def = defaultGameSettings().additionalLordIncome;

    auto income = table.get<sol::optional<sol::table>>("additionalLordIncome");
    if (!income.has_value()) {
        value = def;
        return;
    }

    auto goldTable = income.value().get<sol::optional<sol::table>>("gold");
    if (goldTable.has_value()) {
        value.gold.warrior = readSetting(goldTable.value(), "warrior", def.gold.warrior);
        value.gold.mage = readSetting(goldTable.value(), "mage", def.gold.mage);
        value.gold.guildmaster = readSetting(goldTable.value(), "guildmaster",
                                             def.gold.guildmaster);
    } else {
        value.gold = def.gold;
    }

    auto manaTable = income.value().get<sol::optional<sol::table>>("mana");
    if (manaTable.has_value()) {
        value.mana.warrior = readSetting(manaTable.value(), "warrior", def.mana.warrior);
        value.mana.mage = readSetting(manaTable.value(), "mage", def.mana.mage);
        value.mana.guildmaster = readSetting(manaTable.value(), "guildmaster",
                                             def.mana.guildmaster);
    } else {
        value.mana = def.mana;
    }
}


static void readAdditionalCityIncomeSettings(const sol::table& table,
                                             Settings::AdditionalCityIncome& value)
{
    const auto& def = defaultGameSettings().additionalCityIncome;

    auto income = table.get<sol::optional<sol::table>>("additionalCityIncome");
    if (!income.has_value()) {
        value = def;
        return;
    }

    auto goldTable = income.value().get<sol::optional<sol::table>>("gold");
    if (goldTable.has_value()) {
        value.gold.capital = readSetting(goldTable.value(), "capital", def.gold.capital);
        value.gold.tier1 = readSetting(goldTable.value(), "tier1", def.gold.tier1);
        value.gold.tier2 = readSetting(goldTable.value(), "tier2", def.gold.tier2);
        value.gold.tier3 = readSetting(goldTable.value(), "tier3", def.gold.tier3);
        value.gold.tier4 = readSetting(goldTable.value(), "tier4", def.gold.tier4);
        value.gold.tier5 = readSetting(goldTable.value(), "tier5", def.gold.tier5);

    } else {
        value.gold = def.gold;
    }

    auto manaTable = income.value().get<sol::optional<sol::table>>("mana");
    if (manaTable.has_value()) {
        value.mana.capital = readSetting(manaTable.value(), "capital", def.mana.capital);
        value.mana.tier1 = readSetting(manaTable.value(), "tier1", def.mana.tier1);
        value.mana.tier2 = readSetting(manaTable.value(), "tier2", def.mana.tier2);
        value.mana.tier3 = readSetting(manaTable.value(), "tier3", def.mana.tier3);
        value.mana.tier4 = readSetting(manaTable.value(), "tier4", def.mana.tier4);
        value.mana.tier5 = readSetting(manaTable.value(), "tier5", def.mana.tier5);

    } else {
        value.mana = def.mana;
    }
}

static void readExtandedBattleSettings(const sol::table& table, Settings::ExtendedBattle& value)
{
    const auto& def = defaultGameSettings().extendedBattle;

    auto category = table.get<sol::optional<sol::table>>("extendedBattle");
    if (!category.has_value()) {
        value = def;
        return;
    }

    value.dotDamageCanStack = readSetting(category.value(), "dotDamageCanStack",
                                          def.dotDamageCanStack);
    value.blisterDamageID = readSetting(category.value(), "blisterDamageID", def.blisterDamageID);
    value.frostbiteDamageID = readSetting(category.value(), "frostbiteDamageID",
                                          def.frostbiteDamageID);
    value.poisonDamageID = readSetting(category.value(), "poisonDamageID", def.poisonDamageID);
    value.maxDotDamage = readSetting(category.value(), "maxDotDamage", def.maxDotDamage, 0, 32767);

    value.lowerdamageCanAffectHealer = readSetting(category.value(), "lowerdamageCanAffectHealer",
                                                   def.lowerdamageCanAffectHealer);
    value.boostdamageCanAffectHealer = readSetting(category.value(), "boostdamageCanAffectHealer",
                                                   def.boostdamageCanAffectHealer);
    value.boostCanAffectDot = readSetting(category.value(), "boostCanAffectDot",
                                          def.boostCanAffectDot);
    value.lowerCanAffectDot = readSetting(category.value(), "lowerCanAffectDot",
                                          def.lowerCanAffectDot);
    value.longEffectsUsesPower = readSetting(category.value(), "longEffectsUsesPower", def.longEffectsUsesPower);

    value.itemExtraTurn = readSetting(category.value(), "itemExtraTurn", def.itemExtraTurn);

    value.experimental = readSetting(category.value(), "experimental", def.experimental);
}

static void readSettings(const sol::table& table, Settings& settings)
{
    // clang-format off
    settings.unitMaxDamage = readSetting(table, "unitMaxDamage", defaultGameSettings().unitMaxDamage);
    settings.unitMaxArmor = readSetting(table, "unitMaxArmor", defaultGameSettings().unitMaxArmor);
    settings.stackScoutRangeMax = readSetting(table, "stackMaxScoutRange", defaultGameSettings().stackScoutRangeMax);
    settings.shatteredArmorMax = readSetting(table, "shatteredArmorMax", defaultGameSettings().shatteredArmorMax, 0, baseGameSettings().shatteredArmorMax);
    settings.shatterDamageMax = readSetting(table, "shatterDamageMax", defaultGameSettings().shatterDamageMax, 0, baseGameSettings().shatterDamageMax);
    settings.drainAttackHeal = readSetting(table, "drainAttackHeal", defaultGameSettings().drainAttackHeal);
    settings.drainOverflowHeal = readSetting(table, "drainOverflowHeal", defaultGameSettings().drainOverflowHeal);
    settings.criticalHitDamage = readSetting(table, "criticalHitDamage", defaultGameSettings().criticalHitDamage);
    settings.criticalHitChance = readSetting(table, "criticalHitChance", defaultGameSettings().criticalHitChance, (uint8_t)0, (uint8_t)100);
    settings.mageLeaderAttackPowerReduction = readSetting(table, "mageLeaderAccuracyReduction", defaultGameSettings().mageLeaderAttackPowerReduction);
    settings.disableAllowedRoundMax = readSetting(table, "disableAllowedRoundMax", defaultGameSettings().disableAllowedRoundMax, (uint8_t)1);
    settings.shatterDamageUpgradeRatio = readSetting(table, "shatterDamageUpgradeRatio", defaultGameSettings().shatterDamageUpgradeRatio);
    settings.splitDamageMultiplier = readSetting(table, "splitDamageMultiplier", defaultGameSettings().splitDamageMultiplier, (uint8_t)1, (uint8_t)6);
    settings.preserveCapitalBuildings = readSetting(table, "preserveCapitalBuildings", defaultGameSettings().preserveCapitalBuildings);
    settings.buildTempleForWarriorLord = readSetting(table, "buildTempleForWarriorLord", defaultGameSettings().buildTempleForWarriorLord);
    settings.allowShatterAttackToMiss = readSetting(table, "allowShatterAttackToMiss", defaultGameSettings().allowShatterAttackToMiss);
    settings.doppelgangerRespectsEnemyImmunity = readSetting(table, "doppelgangerRespectsEnemyImmunity", defaultGameSettings().doppelgangerRespectsEnemyImmunity);
    settings.doppelgangerRespectsAllyImmunity = readSetting(table, "doppelgangerRespectsAllyImmunity", defaultGameSettings().doppelgangerRespectsAllyImmunity);
    settings.leveledDoppelgangerAttack = readSetting(table, "leveledDoppelgangerAttack", defaultGameSettings().leveledDoppelgangerAttack);
    settings.leveledTransformSelfAttack = readSetting(table, "leveledTransformSelfAttack", defaultGameSettings().leveledTransformSelfAttack);
    settings.leveledTransformOtherAttack = readSetting(table, "leveledTransformOtherAttack", defaultGameSettings().leveledTransformOtherAttack);
    settings.leveledDrainLevelAttack = readSetting(table, "leveledDrainLevelAttack", defaultGameSettings().leveledDrainLevelAttack);
    settings.leveledSummonAttack = readSetting(table, "leveledSummonAttack", defaultGameSettings().leveledSummonAttack);
    settings.missChanceSingleRoll = readSetting(table, "missChanceSingleRoll", defaultGameSettings().missChanceSingleRoll);
    settings.unrestrictedBestowWards = readSetting(table, "unrestrictedBestowWards", defaultGameSettings().unrestrictedBestowWards);
    settings.freeTransformSelfAttack = readSetting(table, "freeTransformSelfAttack", defaultGameSettings().freeTransformSelfAttack);
    settings.freeTransformSelfAttackInfinite = readSetting(table, "freeTransformSelfAttackInfinite", defaultGameSettings().freeTransformSelfAttackInfinite);
    settings.fixEffectiveHpFormula = readSetting(table, "fixEffectiveHpFormula", defaultGameSettings().fixEffectiveHpFormula);
    settings.instantBuffRemoval = readSetting(table, "instantBuffRemoval", defaultGameSettings().instantBuffRemoval);
    settings.reviveAttacksUsesQtyHeal = readSetting(table, "reviveAttacksUsesQtyHeal", defaultGameSettings().reviveAttacksUsesQtyHeal);
    settings.reviveItemsUsesQtyHeal = readSetting(table, "reviveItemsUsesQtyHeal", defaultGameSettings().reviveItemsUsesQtyHeal);
    settings.advancedCure = readSetting(table, "advancedCure", defaultGameSettings().advancedCure);
    settings.cacheLeaderDataOnTransform = readSetting(table, "cacheLeaderDataOnTransform", defaultGameSettings().cacheLeaderDataOnTransform);
    settings.fogSpellHideEnemyVision = readSetting(table, "fogSpellHideEnemyVision", defaultGameSettings().fogSpellHideEnemyVision);
    settings.trainerCampLowerCost = readSetting(table, "trainerCampLowerCost", defaultGameSettings().trainerCampLowerCost);

    auto chances = table.get<sol::optional<sol::table>>("longEffectRemoveChances");
    if (chances.has_value())
    {
        settings.longEffectRemoveChances.clear();
        for (size_t i = 0; i < chances.value().size(); i++)
            settings.longEffectRemoveChances.push_back(
                std::clamp<int>(chances.value()[i + 1], 0, 100));
    }

    // People keep forgetting to turn this off in release packages
    //settings.debugMode = readSetting(table, "debugHooks", defaultSettings().debugMode);
    // clang-format on

    readAiAttackPowerSettings(table, settings.aiAttackPowerBonus);
    readAllowBattleItemsSettings(table, settings.allowBattleItems);
    readModifierSettings(table, settings.modifiers);
    readMovementCostSettings(table, settings.movementCost);
    readDebugSettings(table, settings.debug);
    readEngineSettings(table, settings.engine);
    readBattleSettings(table, settings.battle);
    readAdditionalLordIncomeSettings(table, settings.additionalLordIncome);
    readAdditionalCityIncomeSettings(table, settings.additionalCityIncome);
    readExtandedBattleSettings(table, settings.extendedBattle);
}

static void readDebugMode(Settings& settings)
{
#ifdef _DEBUG
    settings.debugMode = true;
    return;
#endif

    settings.debugMode = defaultGameSettings().debugMode;

    char commandLine[256];
    strncpy(commandLine, GetCommandLine(), 256);
    commandLine[255] = 0;
    // Similar to built-in -GAMESPY and -NOCRASH
    if (strstr(commandLine, "-DEBUG")) {
        settings.debugMode = true;
    }
}

const Settings& baseGameSettings()
{
    static Settings settings;
    static bool initialized = false;

    if (!initialized) {
        settings.unitMaxDamage = 300;
        settings.unitMaxArmor = 90;
        settings.stackScoutRangeMax = 8;
        settings.shatteredArmorMax = 100;
        settings.shatterDamageMax = 100;
        settings.drainAttackHeal = 50;
        settings.drainOverflowHeal = 50;
        settings.criticalHitDamage = 5;
        settings.criticalHitChance = 100;
        settings.mageLeaderAttackPowerReduction = 10;
        settings.aiAttackPowerBonus.absolute = true;
        settings.aiAttackPowerBonus.easy = -15;
        settings.aiAttackPowerBonus.average = 0;
        settings.aiAttackPowerBonus.hard = 5;
        settings.aiAttackPowerBonus.veryHard = 10;
        settings.disableAllowedRoundMax = 40;
        settings.shatterDamageUpgradeRatio = 100;
        settings.splitDamageMultiplier = 1;
        settings.preserveCapitalBuildings = false;
        settings.buildTempleForWarriorLord = false;
        settings.allowShatterAttackToMiss = false;
        settings.doppelgangerRespectsEnemyImmunity = false;
        settings.doppelgangerRespectsAllyImmunity = false;
        settings.leveledDoppelgangerAttack = false;
        settings.leveledTransformSelfAttack = false;
        settings.leveledTransformOtherAttack = false;
        settings.leveledDrainLevelAttack = false;
        settings.leveledSummonAttack = false;
        settings.missChanceSingleRoll = false;
        settings.unrestrictedBestowWards = false;
        settings.freeTransformSelfAttack = false;
        settings.freeTransformSelfAttackInfinite = false;
        settings.fixEffectiveHpFormula = false;
        settings.modifiers.cumulativeUnitRegeneration = false;
        settings.modifiers.notifyModifiersChanged = false;
        settings.modifiers.validateUnitsOnGroupChanged = false;
        settings.allowBattleItems.onTransformOther = false;
        settings.allowBattleItems.onTransformSelf = false;
        settings.allowBattleItems.onDrainLevel = false;
        settings.allowBattleItems.onDoppelganger = false;
        settings.movementCost.water.dflt = 6;
        settings.movementCost.water.deadLeader = 12;
        settings.movementCost.water.withBonus = 2;
        settings.movementCost.water.waterOnly = 2;
        settings.movementCost.forest.dflt = 4;
        settings.movementCost.forest.deadLeader = 8;
        settings.movementCost.forest.withBonus = 2;
        settings.movementCost.plain.dflt = 2;
        settings.movementCost.plain.deadLeader = 4;
        settings.movementCost.plain.onRoad = 1;
        settings.battle.fallbackAction = game::BattleAction::Defend;
        settings.debugMode = false;

        settings.instantBuffRemoval = false;
        settings.reviveAttacksUsesQtyHeal = 0;
        settings.reviveItemsUsesQtyHeal = false;
        settings.advancedCure = false;

        settings.extendedBattle.dotDamageCanStack = false;
        // Not used, but can be useful
        settings.extendedBattle.blisterDamageID = "g202aa";
        settings.extendedBattle.frostbiteDamageID = "g201aa";
        settings.extendedBattle.poisonDamageID = "g200aa";
        //
        settings.extendedBattle.maxDotDamage = 300;
        settings.extendedBattle.lowerdamageCanAffectHealer = false;
        settings.extendedBattle.boostdamageCanAffectHealer = false;
        settings.extendedBattle.boostCanAffectDot = false;
        settings.extendedBattle.lowerCanAffectDot = false;
        settings.extendedBattle.longEffectsUsesPower = false;
        settings.extendedBattle.itemExtraTurn = false;
        settings.extendedBattle.experimental = false;

        settings.longEffectRemoveChances = {0, 50, 75, 100};

        settings.cacheLeaderDataOnTransform = false;
        settings.fogSpellHideEnemyVision = false;
        settings.trainerCampLowerCost = false;

        initialized = true;
    }

    return settings;
}

const Settings& defaultGameSettings()
{
    static Settings settings;
    static bool initialized = false;

    if (!initialized) {
        settings = baseGameSettings();
        settings.unrestrictedBestowWards = true;
        settings.fixEffectiveHpFormula = true;

        // The default value of 1024 objects provides room for average object size of 512 bytes.
        settings.engine.sendRefreshInfoObjectCountLimit = 1024;

        initialized = true;
    }

    return settings;
}

void initializeGameSettings(Settings& value)
{
    value = defaultGameSettings();

    const auto path{scriptsFolder() / "settings.lua"};
    try {
        const auto env{executeScriptFile(path)};
        if (env) {
            const sol::table& table = (*env)["settings"];
            readSettings(table, value);
        }

        readDebugMode(value);
    } catch (const std::exception& e) {
        showErrorMessageBox(fmt::format("Failed to read script '{:s}'.\n"
                                        "Reason: '{:s}'",
                                        path.string(), e.what()));
    }
}

Settings& getGameSettings()
{
    static Settings settings;
    static bool initialized = false;

    if (!initialized) {
        initializeGameSettings(settings);
        initialized = true;
    }

    return settings;
}

const Settings& gameSettings()
{
    return getGameSettings();
}


} // namespace hooks
