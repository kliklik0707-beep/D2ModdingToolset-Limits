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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>

namespace game {
enum class BattleAction : int;
}

namespace hooks {

struct Settings
{
    int unitMaxDamage;
    int unitMaxArmor;
    int stackScoutRangeMax;
    int shatteredArmorMax;
    int shatterDamageMax;
    int drainAttackHeal;
    int drainOverflowHeal;
    std::uint8_t criticalHitDamage;
    std::uint8_t criticalHitChance;
    std::uint8_t mageLeaderAttackPowerReduction;
    std::uint8_t disableAllowedRoundMax;
    std::uint8_t shatterDamageUpgradeRatio;
    std::uint8_t splitDamageMultiplier;

    struct AiAttackPowerBonus
    {
        std::int8_t easy;
        std::int8_t average;
        std::int8_t hard;
        std::int8_t veryHard;
        bool absolute;
    } aiAttackPowerBonus;

    bool preserveCapitalBuildings;
    bool buildTempleForWarriorLord;
    bool allowShatterAttackToMiss;
    bool doppelgangerRespectsEnemyImmunity;
    bool doppelgangerRespectsAllyImmunity;
    bool leveledDoppelgangerAttack;
    bool leveledTransformSelfAttack;
    bool leveledTransformOtherAttack;
    bool leveledDrainLevelAttack;
    bool leveledSummonAttack;
    bool missChanceSingleRoll;
    bool unrestrictedBestowWards;
    bool freeTransformSelfAttack;
    bool freeTransformSelfAttackInfinite;
    bool fixEffectiveHpFormula;

    struct AdditionalLordIncome
    {   
        struct Gold
        {
            int warrior = 0;
            int mage = 0;
            int guildmaster = 0;
        } gold;
        
        struct Mana
        {
            int warrior = 0;
            int mage = 0;
            int guildmaster = 0;
        } mana;
    } additionalLordIncome;

    struct AdditionalCityIncome
    {   
        struct Gold
        {
            int capital = 0;
            int tier1 = 0;
            int tier2 = 0;
            int tier3 = 0;
            int tier4 = 0;
            int tier5 = 0;
        } gold;
        
        struct Mana
        {
            int capital = 0;
            int tier1 = 0;
            int tier2 = 0;
            int tier3 = 0;
            int tier4 = 0;
            int tier5 = 0;
        } mana;
    } additionalCityIncome;

    struct Modifiers
    {
        bool cumulativeUnitRegeneration;
        bool notifyModifiersChanged;
        bool validateUnitsOnGroupChanged;
    } modifiers;

    struct AllowBattleItems
    {
        bool onTransformOther;
        bool onTransformSelf;
        bool onDrainLevel;
        bool onDoppelganger;
    } allowBattleItems;

    struct MovementCost
    {
        struct Water
        {
            int dflt;
            int deadLeader;
            int withBonus;
            int waterOnly;
        } water;

        struct Forest
        {
            int dflt;
            int deadLeader;
            int withBonus;
        } forest;

        struct Plain
        {
            int dflt;
            int deadLeader;
            int onRoad;
        } plain;

    } movementCost;

    // Do not expose these settings in public 'settings.lua' template so poor souls won't suffer
    // from their own ignorance
    struct Debug
    {
        std::uint32_t sendObjectsChangesTreshold{0};
        bool logSinglePlayerMessages{false};
    } debug;

    struct Engine
    {
        // This is needed to split single CRefreshInfo into several instances when loading large
        // scenario, because it needs to fit to the network message buffer of 512 KB.
        std::uint32_t sendRefreshInfoObjectCountLimit{0};
    } engine;

    struct Battle
    {
        game::BattleAction fallbackAction;
        bool debugAi{false};
        bool allowRetreatedUnitsToUpgrade{false};
        bool carryXpOverUpgrade{false};
        bool allowMultiUpgrade{false};
    } battle;

    bool debugMode;

    bool instantBuffRemoval;
    int reviveAttacksUsesQtyHeal;
    bool reviveItemsUsesQtyHeal;
    bool advancedCure;

    struct ExtendedBattle
    {
        bool dotDamageCanStack;
        std::string blisterDamageID;
        std::string frostbiteDamageID;
        std::string poisonDamageID;
        int maxDotDamage;

        bool lowerdamageCanAffectHealer;
        bool boostdamageCanAffectHealer;

        bool boostCanAffectDot;
        bool lowerCanAffectDot;

        bool longEffectsUsesPower;

        bool itemExtraTurn;

        bool experimental;

    } extendedBattle;

    std::vector<int> longEffectRemoveChances;

    bool cacheLeaderDataOnTransform{false};
    bool fogSpellHideEnemyVision{false};
    bool trainerCampLowerCost{false};
};

const Settings& baseGameSettings();
const Settings& defaultGameSettings();

const Settings& gameSettings();

} // namespace hooks

#endif // SETTINGS_H
