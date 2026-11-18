/**
 * ScriptDev3 is an extension for mangos providing enhanced features for
 * area triggers, creatures, game objects, instances, items, and spells beyond
 * the default database scripting in mangos.
 *
 * Copyright (C) 2006-2013 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2014-2026 MaNGOS <https://www.getmangos.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

/**
 * ScriptData
 * SDName:      Silithus
 * SD%Complete: 100
 * SDComment:   Quest support: 8519.
 * SDCategory:  Silithus
 * EndScriptData
 */

/**
 * ContentData
 * npc_anachronos_the_ancient
 * EndContentData
 */

#include "precompiled.h"
#include "ObjectMgr.h"
#include "AqWarEffortRuntime.h"
#include "World.h"
#include "Log.h"

/*###
## npc_anachronos_the_ancient
###*/

enum
{
    // Dragons
    NPC_MERITHRA_OF_THE_DREAM           = 15378,
    NPC_CAELESTRASZ                     = 15379,
    NPC_ARYGOS                          = 15380,
    NPC_ANACHRONOS_THE_ANCIENT          = 15381,
    NPC_ANACHRONOS_QUEST_TRIGGER        = 15454,            // marks some movement for the dragons

    // Elfs
    NPC_FANDRAL_STAGHELM                = 15382,
    NPC_KALDOREI_INFANTRY               = 15423,

    // Qiraji warriors
    NPC_QIRAJI_WASP                     = 15414,
    NPC_QIRAJI_DRONE                    = 15421,
    NPC_QIRAJI_TANK                     = 15422,
    NPC_ANUBISATH_CONQUEROR             = 15424,

    QUEST_A_PAWN_ON_THE_ETERNAL_BOARD   = 8519,

    // Yells -> in chronological order
    SAY_ANACHRONOS_INTRO_1              = -1000740,
    SAY_FANDRAL_INTRO_2                 = -1000741,
    SAY_MERITHRA_INTRO_3                = -1000742,
    EMOTE_ARYGOS_NOD                    = -1000743,
    SAY_CAELESTRASZ_INTRO_4             = -1000744,
    EMOTE_MERITHRA_GLANCE               = -1000745,
    SAY_MERITHRA_INTRO_5                = -1000746,

    SAY_MERITHRA_ATTACK_1               = -1000747,
    SAY_ARYGOS_ATTACK_2                 = -1000748,
    SAY_ARYGOS_ATTACK_3                 = -1000749,
    SAY_CAELESTRASZ_ATTACK_4            = -1000750,
    SAY_CAELESTRASZ_ATTACK_5            = -1000751,

    SAY_ANACHRONOS_SEAL_1               = -1000752,
    SAY_FANDRAL_SEAL_2                  = -1000753,
    SAY_ANACHRONOS_SEAL_3               = -1000754,
    SAY_ANACHRONOS_SEAL_4               = -1000755,
    SAY_ANACHRONOS_SEAL_5               = -1000756,
    SAY_FANDRAL_SEAL_6                  = -1000757,

    EMOTE_FANDRAL_EXHAUSTED             = -1000758,
    SAY_ANACHRONOS_EPILOGUE_1           = -1000759,
    SAY_ANACHRONOS_EPILOGUE_2           = -1000760,
    SAY_ANACHRONOS_EPILOGUE_3           = -1000761,
    EMOTE_ANACHRONOS_SCEPTER            = -1000762,
    SAY_FANDRAL_EPILOGUE_4              = -1000763,
    SAY_FANDRAL_EPILOGUE_5              = -1000764,
    EMOTE_FANDRAL_SHATTER               = -1000765,
    SAY_ANACHRONOS_EPILOGUE_6           = -1000766,
    SAY_FANDRAL_EPILOGUE_7              = -1000767,
    EMOTE_ANACHRONOS_DISPPOINTED        = -1000768,
    EMOTE_ANACHRONOS_PICKUP             = -1000769,
    SAY_ANACHRONOS_EPILOGUE_8           = -1000770,

#if defined (TBC) || defined (WOTLK) || defined (CATA) || defined(MISTS)
    // The transform spell for Anachronos was removed from DBC
    DISPLAY_ID_BRONZE_DRAGON            = 15500,
#endif
    // Spells
    SPELL_GREEN_DRAGON_TRANSFORM        = 25105,
    SPELL_RED_DRAGON_TRANSFORM          = 25106,
    SPELL_BLUE_DRAGON_TRANSFORM         = 25107,
#if defined (CLASSIC)
    SPELL_BRONZE_DRAGON_TRANSFORM       = 25108,
#endif

    SPELL_MERITHRA_WAKE                 = 25145,            // should trigger 25172 on targets
    SPELL_ARYGOS_VENGEANCE              = 25149,
    SPELL_CAELESTRASZ_MOLTEN_RAIN       = 25150,

    SPELL_TIME_STOP                     = 25158,            // Anachronos stops the battle - should trigger 25171
    SPELL_GLYPH_OF_WARDING              = 25166,            // Sends event 9427 - closes GO 176148
    SPELL_PRISMATIC_BARRIER             = 25159,            // Sends event 9425 - closes GO 176146
    SPELL_CALL_ANCIENTS                 = 25167,            // Sends event 9426 - closes GO 176147
    SPELL_SHATTER_HAMMER                = 25182,            // Breaks the scepter at its spell target position

    EVENT_AQ_GATE_PRISMATIC_BARRIER     = 9425,
    EVENT_AQ_GATE_CALL_ANCIENTS         = 9426,
    EVENT_AQ_GATE_GLYPH_OF_WARDING      = 9427,
    GO_AQ_GATE_PRISMATIC_BARRIER        = 176146,
    GO_AQ_GATE_CALL_ANCIENTS            = 176147,
    GO_AQ_GATE_GLYPH_OF_WARDING         = 176148,
    GUID_AQ_GATE_PRISMATIC_BARRIER      = 66334,
    GUID_AQ_GATE_CALL_ANCIENTS          = 66335,
    GUID_AQ_GATE_GLYPH_OF_WARDING       = 66336,
    DISPLAY_SCEPTER_OF_SHIFTING_SANDS   = 33518,

    POINT_ID_DRAGON_ATTACK              = 1,
    POINT_ID_EXIT                       = 2,
    POINT_ID_GATE                       = 3,
    POINT_ID_SCEPTER_1                  = 4,
    POINT_ID_SCEPTER_2                  = 5,
    POINT_ID_EPILOGUE                   = 6,
    DATA_HANDLE_SCEPTER                 = 7,        // dummy members - used in dialogue helper
    DATA_MERITHRA_ATTACK                = 8,
    DATA_CAELASTRASZ_ATTACK             = 9,

    MAX_DRAGONS                         = 4,
};

struct AqArmySpawn
{
    uint32 entry;
    uint32 delayMs;
    float x, y, z, orientation;
};

static const DialogueEntry aEventDialogue[] =
{
    {NPC_ANACHRONOS_THE_ANCIENT,    0,                          2000},  // summon the dragons
    {SAY_ANACHRONOS_INTRO_1,        NPC_ANACHRONOS_THE_ANCIENT, 3000},
    {EMOTE_ONESHOT_SHOUT,           NPC_ANACHRONOS_THE_ANCIENT, 3000},  // make Anachronos shout and summon the warriors
    {SAY_FANDRAL_INTRO_2,           NPC_FANDRAL_STAGHELM,       6000},
    {EMOTE_MERITHRA_GLANCE,         NPC_MERITHRA_OF_THE_DREAM,  2000},
    {SAY_MERITHRA_INTRO_3,          NPC_MERITHRA_OF_THE_DREAM,  3000},
    {EMOTE_ARYGOS_NOD,              NPC_ARYGOS,                 4000},
    {SAY_CAELESTRASZ_INTRO_4,       NPC_CAELESTRASZ,            9000},
    {SAY_MERITHRA_INTRO_5,          NPC_MERITHRA_OF_THE_DREAM,  5000},
    {NPC_ANACHRONOS_QUEST_TRIGGER,  0,                          0},     // send Merithra to fight
    {DATA_MERITHRA_ATTACK,          0,                          5000},  // make Merithra wait
    {SAY_MERITHRA_ATTACK_1,         NPC_MERITHRA_OF_THE_DREAM,  1000},
    {SPELL_GREEN_DRAGON_TRANSFORM,  0,                          6000},
    {SAY_ARYGOS_ATTACK_2,           NPC_ARYGOS,                 5000},
    {NPC_ARYGOS,                    0,                          1000},  // send Arygos to fight
    {POINT_ID_EXIT,                 0,                          4000},  // make Merithra exit
    {SAY_ARYGOS_ATTACK_3,           NPC_ARYGOS,                 4000},
    {SPELL_BLUE_DRAGON_TRANSFORM,   0,                          5000},
    {SPELL_ARYGOS_VENGEANCE,        0,                          7000},
    {POINT_ID_DRAGON_ATTACK,        0,                          1000},  // make Arygos exit
    {SAY_CAELESTRASZ_ATTACK_4,      NPC_CAELESTRASZ,            5000},
    {NPC_CAELESTRASZ,               0,                          0},     // send Caelestrasz to fight
    {DATA_CAELASTRASZ_ATTACK,       0,                          3000},  // make Caelestrasz wait
    {SAY_CAELESTRASZ_ATTACK_5,      NPC_CAELESTRASZ,            5000},
    {SPELL_RED_DRAGON_TRANSFORM,    0,                          4000},  // transform Caelestrasz
    {SPELL_CAELESTRASZ_MOLTEN_RAIN, 0,                          6000},  // Caelestrasz casts molten rain
    {SAY_ANACHRONOS_SEAL_1,         NPC_ANACHRONOS_THE_ANCIENT, 5000},
    {SAY_FANDRAL_SEAL_2,            NPC_FANDRAL_STAGHELM,       3000},
    {SAY_ANACHRONOS_SEAL_3,         NPC_ANACHRONOS_THE_ANCIENT, 1000},
    {POINT_ID_GATE,                 0,                          1000},  // send Anachronos to the gate
    {NPC_FANDRAL_STAGHELM,          0,                          0},     // send Fandral to the gate
    {SPELL_TIME_STOP,               0,                          7000},  // Anachronos casts Time Stop
    {SPELL_PRISMATIC_BARRIER,       0,                          15000},
    {SPELL_GLYPH_OF_WARDING,        0,                          4000},
    {SAY_ANACHRONOS_SEAL_5,         NPC_ANACHRONOS_THE_ANCIENT, 3000},
    {SAY_FANDRAL_SEAL_6,            NPC_FANDRAL_STAGHELM,       9000},
    {EMOTE_FANDRAL_EXHAUSTED,       NPC_FANDRAL_STAGHELM,       1000},
    {SAY_ANACHRONOS_EPILOGUE_1,     NPC_ANACHRONOS_THE_ANCIENT, 6000},
    {SAY_ANACHRONOS_EPILOGUE_2,     NPC_ANACHRONOS_THE_ANCIENT, 5000},
    {SAY_ANACHRONOS_EPILOGUE_3,     NPC_ANACHRONOS_THE_ANCIENT, 15000},
    {DATA_HANDLE_SCEPTER,           NPC_ANACHRONOS_THE_ANCIENT, 3000},  // handle the scepter
    {SAY_FANDRAL_EPILOGUE_4,        NPC_FANDRAL_STAGHELM,       3000},
    {POINT_ID_SCEPTER_2,            0,                          4000},  // make Anachronos stand
    {SAY_FANDRAL_EPILOGUE_5,        NPC_FANDRAL_STAGHELM,       12000},
    {EMOTE_FANDRAL_SHATTER,         NPC_FANDRAL_STAGHELM,       3000},
    {SAY_ANACHRONOS_EPILOGUE_6,     NPC_ANACHRONOS_THE_ANCIENT, 0},
    {SAY_FANDRAL_EPILOGUE_7,        NPC_FANDRAL_STAGHELM,       8000},
    {POINT_ID_EPILOGUE,             0,                          4000}, // move Fandral to Anachronos
    {EMOTE_ANACHRONOS_DISPPOINTED,  NPC_ANACHRONOS_THE_ANCIENT, 1000},
    {POINT_ID_SCEPTER_1,            0,                          0},    // make Anachronos pick the pieces
    {0, 0, 0},
};

struct EventLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
    uint32 m_uiEntry;
};

static EventLocations aEternalBoardNPCs[MAX_DRAGONS] =
{
    { -8029.301f, 1534.612f, 2.609f, 3.121f, NPC_FANDRAL_STAGHELM},
    { -8034.227f, 1536.580f, 2.609f, 6.161f, NPC_ARYGOS},
    { -8031.935f, 1532.658f, 2.609f, 1.012f, NPC_CAELESTRASZ},
    { -8034.106f, 1534.224f, 2.609f, 0.290f, NPC_MERITHRA_OF_THE_DREAM},
};

static EventLocations aEternalBoardMovement[] =
{
    { -8159.951f, 1525.241f, 74.994f, 0.0f, 0 },          // 0 Flight position for dragons
    { -8106.238f, 1525.948f, 2.639f, 0.0f, 0 },           // 1 Anachronos gate location
    { -8103.861f, 1525.923f, 2.677f, 0.0f, 0 },           // 2 Fandral gate location
    { -8107.387f, 1523.641f, 2.609f, 0.0f, 0 },           // 3 Shattered scepter
    { -8100.921f, 1527.740f, 2.871f, 0.0f, 0 },           // 4 Fandral epilogue location
    { -8115.270f, 1515.926f, 3.305f, 0.0f, 0 },           // 5 Anachronos gather broken scepter 1
    { -8116.879f, 1530.615f, 3.762f, 0.0f, 0 },           // 6 Anachronos gather broken scepter 2
    { -7997.790f, 1548.664f, 3.738f, 0.0f, 0 },           // 7 Fandral exit location
    { -8061.933f, 1496.196f, 2.556f, 0.0f, 0 },           // 8 Anachronos launch location
    { -8008.705f, 1446.063f, 44.104f, 0.0f, 0 },          // 9 Anachronos flight location
    { -8085.748f, 1521.484f, 2.624f, 0.0f, 0 }            // 10 Anchor point for the army summoning
};

struct npc_anachronos_the_ancient : public CreatureScript
{
    npc_anachronos_the_ancient() : CreatureScript("npc_anachronos_the_ancient") {}

    struct npc_anachronos_the_ancientAI : public ScriptedAI, private DialogueHelper
    {
        npc_anachronos_the_ancientAI(Creature* pCreature) : ScriptedAI(pCreature),
            DialogueHelper(aEventDialogue)
        {
        }

        uint32 m_uiEventTimer;

        uint8 m_uiEventStage;

        ObjectGuid m_fandralGuid;
        ObjectGuid m_merithraGuid;
        ObjectGuid m_CaelestraszGuid;
        ObjectGuid m_arygosGuid;
        ObjectGuid m_playerGuid;
        ObjectGuid m_triggerGuid;

        GuidList m_lQirajiWarriorsList;
        uint32 m_sceneElapsedMs = 0;
        uint32 m_waveOneIndex = 0;
        uint32 m_waveTwoIndex = 0;
        uint32 m_waveTwoStartMs = 0;
        bool m_waveOneActive = false;
        bool m_waveTwoActive = false;

        void Reset() override
        {
            // We summon the rest of the dragons on timer
            m_uiEventTimer = 100;
            m_uiEventStage = 0;
            m_sceneElapsedMs = 0;
            m_waveOneIndex = 0;
            m_waveTwoIndex = 0;
            m_waveTwoStartMs = 0;
            m_waveOneActive = false;
            m_waveTwoActive = false;
        }

        void JustDidDialogueStep(int32 iEntry) override
        {
            switch (iEntry)
            {
                case NPC_ANACHRONOS_THE_ANCIENT:
                    // Call the other dragons
                    DoSummonDragons();
                    break;
                case EMOTE_ONESHOT_SHOUT:
                    // Summon warriors
                    DoSummonWarriors();
                    m_creature->HandleEmote(EMOTE_ONESHOT_SHOUT);
                    break;
                case SAY_FANDRAL_INTRO_2:
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->SetFacingToObject(m_creature);
                    }
                    break;
                case EMOTE_MERITHRA_GLANCE:
                    if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_merithraGuid))
                    {
                        if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                        {
                            pFandral->SetFacingToObject(pMerithra);
                        }
                    }
                    break;
                case NPC_ANACHRONOS_QUEST_TRIGGER:
                    // The Classic movement script adds its second combatant
                    // wave as the dragons engage. Start its source-relative
                    // schedule at this corresponding scene cue.
                    m_waveTwoStartMs = m_sceneElapsedMs;
                    m_waveTwoActive = true;
                    // Move Merithra to attack
                    if (Creature* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_ANACHRONOS_QUEST_TRIGGER, 35.0f))
                    {
                        m_triggerGuid = pTrigger->GetObjectGuid();
                        if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_merithraGuid))
                        {
                            pMerithra->SetWalk(false);
                            pMerithra->GetMotionMaster()->MovePoint(POINT_ID_DRAGON_ATTACK, pTrigger->Where().X(), pTrigger->Where().Y(), pTrigger->Where().Z());
                        }
                    }
                    break;
                case SPELL_GREEN_DRAGON_TRANSFORM:
                    if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_merithraGuid))
                    {
                        pMerithra->CastSpell(pMerithra, SPELL_GREEN_DRAGON_TRANSFORM, false);
                    }
                    break;
                case SAY_ARYGOS_ATTACK_2:
                    if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_merithraGuid))
                    {
                        pMerithra->CastSpell(pMerithra, SPELL_MERITHRA_WAKE, false);
                    }
                    break;
                case NPC_ARYGOS:
                    // Move Arygos to attack
                    if (Creature* pTrigger = m_creature->GetMap()->GetCreature(m_triggerGuid))
                    {
                        if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_arygosGuid))
                        {
                            pArygos->SetWalk(false);
                            pArygos->GetMotionMaster()->MovePoint(POINT_ID_DRAGON_ATTACK, pTrigger->Where().X(), pTrigger->Where().Y(), pTrigger->Where().Z());
                        }
                    }
                    break;
                case POINT_ID_EXIT:
                    // Move Merithra to the exit point
                    if (Creature* pMerithra = m_creature->GetMap()->GetCreature(m_merithraGuid))
                    {
#if defined (CLASSIC)
                        pMerithra->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND);
#else
                        pMerithra->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_FLY_ANIM);
#endif
                        pMerithra->SetLevitate(true);
                        pMerithra->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[0].m_fX, aEternalBoardMovement[0].m_fY, aEternalBoardMovement[0].m_fZ);
                        pMerithra->ForcedDespawn(9000);
                    }
                    break;
                case SPELL_BLUE_DRAGON_TRANSFORM:
                    if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_arygosGuid))
                    {
                        pArygos->CastSpell(pArygos, SPELL_BLUE_DRAGON_TRANSFORM, false);
                    }
                    break;
                case SPELL_ARYGOS_VENGEANCE:
                    if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_arygosGuid))
                    {
                        pArygos->CastSpell(pArygos, SPELL_ARYGOS_VENGEANCE, false);
                    }
                    break;
                case POINT_ID_DRAGON_ATTACK:
                    // Move Arygos to the exit point
                    if (Creature* pArygos = m_creature->GetMap()->GetCreature(m_arygosGuid))
                    {
#if defined (CLASSIC)
                        pArygos->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND);
#else
                        pArygos->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_FLY_ANIM);
#endif
                        pArygos->SetLevitate(true);
                        pArygos->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[0].m_fX, aEternalBoardMovement[0].m_fY, aEternalBoardMovement[0].m_fZ);
                        pArygos->ForcedDespawn(9000);
                    }
                    break;
                case NPC_CAELESTRASZ:
                    // Move Caelestrasz to attack
                    if (Creature* pTrigger = m_creature->GetMap()->GetCreature(m_triggerGuid))
                    {
                        if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_CaelestraszGuid))
                        {
                            pCaelestrasz->SetWalk(false);
                            pCaelestrasz->GetMotionMaster()->MovePoint(POINT_ID_DRAGON_ATTACK, pTrigger->Where().X(), pTrigger->Where().Y(), pTrigger->Where().Z());
                        }
                    }
                    break;
                case SPELL_RED_DRAGON_TRANSFORM:
                    if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_CaelestraszGuid))
                    {
                        pCaelestrasz->CastSpell(pCaelestrasz, SPELL_RED_DRAGON_TRANSFORM, false);
                    }
                    break;
                case SPELL_CAELESTRASZ_MOLTEN_RAIN:
                    if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_CaelestraszGuid))
                    {
                        pCaelestrasz->CastSpell(pCaelestrasz, SPELL_CAELESTRASZ_MOLTEN_RAIN, false);
                    }
                    break;
                case SAY_ANACHRONOS_SEAL_1:
                    // Send Caelestrasz on flight
                    if (Creature* pCaelestrasz = m_creature->GetMap()->GetCreature(m_CaelestraszGuid))
                    {
#if defined (CLASSIC)
                        pCaelestrasz->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND);
#else
                        pCaelestrasz->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_FLY_ANIM);
#endif
                        pCaelestrasz->SetLevitate(true);
                        pCaelestrasz->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[0].m_fX, aEternalBoardMovement[0].m_fY, aEternalBoardMovement[0].m_fZ);
                        pCaelestrasz->ForcedDespawn(9000);
                    }
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        m_creature->SetFacingToObject(pFandral);
                    }
                    break;
                case SAY_FANDRAL_SEAL_2:
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->SetFacingToObject(m_creature);
                    }
                    break;
                case POINT_ID_GATE:
                    // Send Anachronos to the gate
                    m_creature->SetWalk(false);
                    m_creature->GetMotionMaster()->MovePoint(POINT_ID_GATE, aEternalBoardMovement[1].m_fX, aEternalBoardMovement[1].m_fY, aEternalBoardMovement[1].m_fZ);
                    break;
                case NPC_FANDRAL_STAGHELM:
                    // Send Fandral to the gate
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->SetWalk(false);
                        pFandral->GetMotionMaster()->MovePoint(POINT_ID_GATE, aEternalBoardMovement[2].m_fX, aEternalBoardMovement[2].m_fY, aEternalBoardMovement[2].m_fZ);
                    }
                    break;
                case SPELL_PRISMATIC_BARRIER:
                    DoCastSpellIfCan(m_creature, SPELL_PRISMATIC_BARRIER);
                    break;
                case SPELL_GLYPH_OF_WARDING:
                    DoCastSpellIfCan(m_creature, SPELL_GLYPH_OF_WARDING);
                    break;
                case SAY_FANDRAL_SEAL_6:
                    // Here Anachronos should continue to cast something
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->CastSpell(pFandral, SPELL_CALL_ANCIENTS, false);
                    }
                    break;
                case EMOTE_FANDRAL_EXHAUSTED:
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->SetStandState(UNIT_STAND_STATE_KNEEL);
                        m_creature->SetFacingToObject(pFandral);
                    }
                    break;
                case DATA_HANDLE_SCEPTER:
                    // The scepter is a quest item rather than a hand-equip item.
                    // Set its display directly so the handoff has the expected visual.
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->SetVirtualItemRaw(VIRTUAL_ITEM_SLOT_0, DISPLAY_SCEPTER_OF_SHIFTING_SANDS, 0, 0);
                        DoScriptText(EMOTE_ANACHRONOS_SCEPTER, m_creature, pFandral);
                    }
                    m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                    break;
                case SAY_FANDRAL_EPILOGUE_4:
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->SetStandState(UNIT_STAND_STATE_STAND);
                    }
                    break;
                case POINT_ID_SCEPTER_2:
                    m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                    break;
                case EMOTE_FANDRAL_SHATTER:
                    // Shatter the scepter
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->CastSpell(pFandral, SPELL_SHATTER_HAMMER, false);
                    }
                    break;
                case SAY_ANACHRONOS_EPILOGUE_6:
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->SetWalk(true);
                        pFandral->GetMotionMaster()->MovePoint(POINT_ID_SCEPTER_1, aEternalBoardMovement[3].m_fX, aEternalBoardMovement[3].m_fY, aEternalBoardMovement[3].m_fZ);
                    }
                    break;
                case POINT_ID_EPILOGUE:
                    // Make Fandral leave
                    if (Creature* pFandral = m_creature->GetMap()->GetCreature(m_fandralGuid))
                    {
                        pFandral->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[7].m_fX, aEternalBoardMovement[7].m_fY, aEternalBoardMovement[7].m_fZ);
                    }
                    break;
                case POINT_ID_SCEPTER_1:
                    // Anachronos collects the pieces
                    m_creature->SetWalk(true);
                    m_creature->GetMotionMaster()->MovePoint(POINT_ID_SCEPTER_1, aEternalBoardMovement[5].m_fX, aEternalBoardMovement[5].m_fY, aEternalBoardMovement[5].m_fZ);
                    break;
            }
        }

        Creature* GetSpeakerByEntry(uint32 uiEntry) override
        {
            switch (uiEntry)
            {
                case NPC_ANACHRONOS_THE_ANCIENT:
                    return m_creature;
                case NPC_ARYGOS:
                    return m_creature->GetMap()->GetCreature(m_arygosGuid);
                case NPC_CAELESTRASZ:
                    return m_creature->GetMap()->GetCreature(m_CaelestraszGuid);
                case NPC_MERITHRA_OF_THE_DREAM:
                    return m_creature->GetMap()->GetCreature(m_merithraGuid);
                case NPC_FANDRAL_STAGHELM:
                    return m_creature->GetMap()->GetCreature(m_fandralGuid);

                default:
                    return nullptr;
            }
        }

        void DoSummonDragons()
        {
            for (uint8 i = 0; i < MAX_DRAGONS; ++i)
            {
                m_creature->SummonCreature(aEternalBoardNPCs[i].m_uiEntry, aEternalBoardNPCs[i].m_fX, aEternalBoardNPCs[i].m_fY, aEternalBoardNPCs[i].m_fZ, aEternalBoardNPCs[i].m_fO, TEMPSPAWN_CORPSE_DESPAWN, 0);
            }

        }

        void DoSummonWarriors()
        {
            m_waveOneActive = true;
        }

        void SummonArmySpawn(AqArmySpawn const& spawn)
        {
            m_creature->SummonCreature(spawn.entry, spawn.x, spawn.y, spawn.z, spawn.orientation,
                TEMPSPAWN_TIMED_OOC_OR_DEAD_DESPAWN, 180000);
        }

        void UpdateArmySummoning()
        {
            static AqArmySpawn const waveOne[] = {
                {15424, 10000, -8084.521f, 1534.588f, 2.7344015f, 4.886443138122558593f},
                {15424, 11000, -8093.5166f, 1518.3163f, 2.8594015f, 0.756263673305511474f},
                {15424, 12000, -8095.279f, 1537.7485f, 2.9326189f, 4.624593257904052734f},
                {15414, 13000, -8082.606f, 1530.9467f, 2.7344015f, 3.74396371841430664f},
                {15414, 14000, -8084.1543f, 1534.5504f, 2.7344015f, 4.789569854736328125f},
                {15414, 15000, -8088.1025f, 1518.3938f, 2.964626f, 4.268512725830078125f},
                {15414, 16000, -8091.9814f, 1511.5386f, 3.3312032f, 1.638396978378295898f},
                {15414, 17000, -8093.04f, 1521.0588f, 2.7055414f, 3.159045934677124023f},
                {15414, 18000, -8093.7124f, 1511.7349f, 3.021877f, 1.966364026069641113f},
                {15414, 19000, -8094.366f, 1520.0853f, 2.7344015f, 2.097927093505859375f},
                {15414, 20000, -8093.6265f, 1540.3372f, 2.7925682f, 0.64547586441040039f},
                {15423, 21000, -8096.85f, 1517.41f, 2.62497f, 0.2538f},
                {15423, 22000, -8096.16f, 1520.41f, 2.61089f, 1.3023f},
                {15423, 23000, -8093.92f, 1523.24f, 2.63438f, 0.7211f},
                {15423, 24000, -8089.77f, 1523.89f, 2.61413f, 0.0378f},
                {15423, 25000, -8086.45f, 1522.19f, 2.6211f, 5.7595f},
                {15423, 26000, -8085.0f, 1518.58f, 2.69892f, 4.87201f},
                {15423, 27000, -8085.81f, 1515.03f, 3.1137f, 4.49109f},
                {15423, 28000, -8088.1f, 1512.14f, 3.44917f, 3.82743f},
                {15423, 29000, -8092.39f, 1511.83f, 3.14766f, 3.2619f},
                {15423, 30000, -8095.51f, 1513.85f, 2.67505f, 2.2880f},
                {15423, 31000, -8095.81f, 1532.9f, 2.97344f, 1.45553f},
                {15423, 32000, -8097.73f, 1536.73f, 3.01208f, 2.0445f},
                {15423, 33000, -8098.41f, 1540.29f, 3.03665f, 1.2081f},
                {15423, 34000, -8095.92f, 1543.42f, 2.73728f, 0.6112f},
                {15423, 35000, -8092.01f, 1543.66f, 2.61425f, 0.0339f},
                {15423, 36000, -8088.26f, 1542.46f, 2.61052f, 5.9323f},
                {15423, 37000, -8085.55f, 1537.87f, 2.61052f, 4.8759f},
                {15423, 38000, -8087.66f, 1533.79f, 2.61052f, 3.9923f},
                {15423, 39000, -8090.71f, 1532.95f, 2.71019f, 3.4111f},
                {15423, 40000, -8093.79f, 1532.91f, 2.86282f, 3.0734f},
                {15423, 41000, -8089.39f, 1530.44f, 2.67252f, 0.5798f},
                {15423, 42000, -8086.38f, 1527.78f, 2.60994f, 5.5160f},
                {15423, 43000, -8081.67f, 1528.87f, 2.60994f, 0.3913f},
                {15423, 44000, -8079.11f, 1532.48f, 2.60994f, 1.1610f},
                {15423, 45000, -8079.61f, 1537.63f, 2.60994f, 1.9110f},
                {15423, 46000, -8083.76f, 1540.84f, 2.60994f, 2.7475f},
                {15423, 47000, -8088.28f, 1538.65f, 2.60994f, 3.4740f},
                {15423, 48000, -8091.51f, 1535.63f, 2.67311f, 4.0826f},
                {15423, 49000, -8092.32f, 1531.82f, 2.78504f, 4.5617f},
                {15423, 50000, -8089.95f, 1528.06f, 2.65737f, 5.5710f},
                {15422, 51000, -8082.1353f, 1529.8281f, 2.7344015f, 3.373809099197387695f},
                {15422, 52000, -8082.596f, 1519.3921f, 2.873776f, 4.001942634582519531f},
                {15422, 53000, -8087.275f, 1519.5825f, 2.8576925f, 2.785885334014892578f},
                {15422, 54000, -8087.958f, 1518.5608f, 2.9446065f, 2.453642129898071289f},
                {15422, 55000, -8089.2812f, 1534.7256f, 2.7344015f, 5.322139739990234375f},
                {15422, 56000, -8091.786f, 1537.9224f, 2.7344015f, 5.328545570373535156f},
                {15422, 57000, -8093.0737f, 1518.0198f, 2.8594015f, 0.898447096347808837f},
                {15422, 58000, -8093.1265f, 1512.537f, 3.191799f, 4.358320236206054687f},
                {15422, 59000, -8092.011f, 1541.1951f, 2.7344015f, 4.814171314239501953f},
                {15421, 60000, -8083.5586f, 1513.2365f, 3.1731222f, 1.970927238464355468f},
                {15421, 61000, -8085.1436f, 1512.9519f, 3.3675802f, 1.528013944625854492f},
                {15421, 62000, -8086.7524f, 1528.6262f, 2.7344015f, 1.267617464065551757f},
                {15421, 63000, -8088.752f, 1517.8536f, 2.9983563f, 2.162188529968261718f},
                {15421, 64000, -8088.8975f, 1517.7604f, 2.9844015f, 0.283090144395828247f},
                {15421, 65000, -8089.0596f, 1532.3508f, 2.7344015f, 5.283954620361328125f},
                {15421, 66000, -8088.9673f, 1539.9233f, 2.7344015f, 5.818657875061035156f},
                {15421, 67000, -8090.7905f, 1517.2306f, 2.9844015f, 3.400722980499267578f},
                {15421, 68000, -8090.5513f, 1509.6268f, 3.2168355f, 5.228849411010742187f},
                {15421, 69000, -8090.644f, 1509.9595f, 3.308448f, 5.330722332000732421f},
                {15421, 70000, -8089.866f, 1540.7572f, 2.7344015f, 6.091379165649414062f}
            };
            static AqArmySpawn const waveTwo[] = {
                {15422, 25001, -8096.1157f, 1516.818f, 2.7167563f, 1.134464025497436523f},
                {15422, 25002, -8080.6895f, 1512.5712f, 2.9955852f, 5.044001579284667968f},
                {15422, 25003, -8099.3203f, 1500.8727f, 2.6927383f, 3.019419670104980468f},
                {15422, 25004, -8077.5063f, 1525.3082f, 2.6927383f, 3.298672199249267578f},
                {15422, 25005, -8103.316f, 1554.271f, 4.1517234f, 1.151917338371276855f},
                {15422, 25006, -8107.975f, 1551.8503f, 4.5801725f, 5.323254108428955078f},
                {15422, 25007, -8091.7734f, 1547.8002f, 2.7820363f, 3.019419670104980468f},
                {15422, 25008, -8091.2344f, 1537.9305f, 2.7091513f, 2.58308720588684082f},
                {15422, 25009, -8106.6147f, 1501.0076f, 2.7671623f, 0.331612557172775268f},
                {15422, 25010, -8108.7397f, 1510.966f, 2.7452133f, 0.209439516067504882f},
                {15422, 25011, -8081.764f, 1529.9165f, 2.6927383f, 0.122173048555850982f},
                {15421, 30000, -8078.816f, 1538.9602f, 2.6927333f, 0.139626339077949523f},
                {15421, 50500, -8078.816f, 1538.9602f, 2.6927333f, 0.139626339077949523f},
                {15421, 51500, -8087.6978f, 1516.2865f, 3.1868932f, 6.056292533874511718f},
                {15421, 52500, -8090.362f, 1503.5001f, 2.7152743f, 3.298672199249267578f},
                {15421, 53500, -8085.387f, 1511.4579f, 3.4270213f, 2.58308720588684082f},
                {15421, 54500, -8085.819f, 1497.9415f, 2.6927474f, 1.326450228691101074f},
                {15421, 55500, -8080.5635f, 1506.1622f, 2.8614993f, 3.473205089569091796f},
                {15421, 56500, -8096.567f, 1542.1442f, 2.8728862f, 0.314159274101257324f},
                {15421, 57500, -8107.0103f, 1535.819f, 3.3525844f, 0.401425719261169433f},
                {15421, 58500, -8100.2603f, 1550.8744f, 3.5356762f, 2.932153224945068359f},
                {15414, 59500, -8084.222f, 1519.9562f, 2.7359202f, 0.92502450942993164f},
                {15414, 60500, -8093.04f, 1521.0588f, 2.7055414f, 3.159045934677124023f},
                {15414, 61500, -8111.078f, 1498.4434f, 3.2436872f, 3.001966238021850585f},
                {15414, 62500, -8087.0923f, 1552.5841f, 3.4185972f, 5.462880611419677734f},
                {15414, 63500, -8093.4497f, 1511.104f, 3.0333064f, 4.136430263519287109f},
                {15414, 64500, -8076.758f, 1533.0376f, 2.6927383f, 4.345870018005371093f},
                {15423, 64501, -8095.92f, 1543.42f, 2.73728f, 0.6112f},
                {15423, 64502, -8092.01f, 1543.66f, 2.61425f, 0.0339f},
                {15423, 64503, -8088.26f, 1542.46f, 2.61052f, 5.9323f},
                {15423, 64504, -8085.55f, 1537.87f, 2.61052f, 4.8759f},
                {15423, 64505, -8087.66f, 1533.79f, 2.61052f, 3.9923f},
                {15423, 64506, -8090.71f, 1532.95f, 2.71019f, 3.4111f},
                {15414, 115000, -8084.144f, 1531.8539f, 2.6927302f, 6.2657318115234375f},
                {15414, 115100, -8088.3696f, 1531.313f, 2.7210321f, 6.230825424194335937f},
                {15414, 115200, -8085.4287f, 1536.4481f, 2.6927342f, 3.420845270156860351f},
                {15421, 115300, -8091.2744f, 1558.0068f, 4.2844644f, 4.817108631134033203f},
                {15421, 115400, -8103.6094f, 1541.0853f, 3.7910812f, 0.296705961227416992f},
                {15421, 115500, -8082.349f, 1548.36f, 2.9380612f, 5.16617441177368164f}
            };
            if (m_waveOneActive && m_waveOneIndex < sizeof(waveOne) / sizeof(waveOne[0]) &&
                m_sceneElapsedMs >= waveOne[m_waveOneIndex].delayMs)
            {
                SummonArmySpawn(waveOne[m_waveOneIndex++]);
                if (m_waveOneIndex == sizeof(waveOne) / sizeof(waveOne[0]))
                    m_waveOneActive = false;
            }
            if (m_waveTwoActive && m_waveTwoIndex < sizeof(waveTwo) / sizeof(waveTwo[0]) &&
                m_sceneElapsedMs - m_waveTwoStartMs >= waveTwo[m_waveTwoIndex].delayMs)
            {
                SummonArmySpawn(waveTwo[m_waveTwoIndex++]);
                if (m_waveTwoIndex == sizeof(waveTwo) / sizeof(waveTwo[0]))
                    m_waveTwoActive = false;
            }
        }

        void DoUnsummonArmy()
        {
            // Do not let either scheduled wave continue after scene cleanup.
            m_waveOneActive = false;
            m_waveTwoActive = false;
            for (GuidList::const_iterator itr = m_lQirajiWarriorsList.begin(); itr != m_lQirajiWarriorsList.end(); ++itr)
            {
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                {
                    pTemp->ForcedDespawn();
                }
            }
        }

        void JustSummoned(Creature* pSummoned) override
        {
            // Also remove npc flags where needed
            switch (pSummoned->GetEntry())
            {
                case NPC_FANDRAL_STAGHELM:
                    m_fandralGuid = pSummoned->GetObjectGuid();
                    break;
                case NPC_MERITHRA_OF_THE_DREAM:
                    m_merithraGuid = pSummoned->GetObjectGuid();
                    pSummoned->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
                    break;
                case NPC_CAELESTRASZ:
                    m_CaelestraszGuid = pSummoned->GetObjectGuid();
                    pSummoned->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
                    break;
                case NPC_ARYGOS:
                    m_arygosGuid = pSummoned->GetObjectGuid();
                    pSummoned->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
                    break;
                case NPC_ANUBISATH_CONQUEROR:
                case NPC_QIRAJI_WASP:
                case NPC_QIRAJI_DRONE:
                case NPC_QIRAJI_TANK:
                case NPC_KALDOREI_INFANTRY:
                    m_lQirajiWarriorsList.push_back(pSummoned->GetObjectGuid());
                    break;
            }
        }

        void MovementInform(uint32 uiType, uint32 uiPointId) override
        {
            if (uiType != POINT_MOTION_TYPE)
            {
                return;
            }

            switch (uiPointId)
            {
                case POINT_ID_GATE:
                    // Cast time stop when he reaches the gate
                    DoCastSpellIfCan(m_creature, SPELL_TIME_STOP);
                    StartNextDialogueText(SPELL_TIME_STOP);
                    break;
                case POINT_ID_SCEPTER_1:
                    // Pickup the pieces
                    DoScriptText(EMOTE_ANACHRONOS_PICKUP, m_creature);
                    m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                    m_uiEventTimer = 2000;
                    break;
                case POINT_ID_SCEPTER_2:
                    // Pickup the pieces
                    DoScriptText(SAY_ANACHRONOS_EPILOGUE_8, m_creature);
                    m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                    m_uiEventTimer = 4000;
                    break;
                case POINT_ID_EXIT:
#if defined (CLASSIC)
                    DoCastSpellIfCan(m_creature, SPELL_BRONZE_DRAGON_TRANSFORM);
#else
                    // Spell was removed, manually change the display
                    m_creature->SetDisplayId(DISPLAY_ID_BRONZE_DRAGON);
#endif
                    m_uiEventTimer = 4000;
                    break;
            }
        }

        void SummonedMovementInform(Creature* pSummoned, uint32 uiType, uint32 uiPointId) override
        {
            if (uiType != POINT_MOTION_TYPE)
            {
                return;
            }

            if (pSummoned->GetEntry() == NPC_FANDRAL_STAGHELM)
            {
                switch (uiPointId)
                {
                    case POINT_ID_EPILOGUE:
                        // Face Anachronos and restart the dialogue
                        pSummoned->SetFacingToObject(m_creature);
                        StartNextDialogueText(SAY_FANDRAL_EPILOGUE_7);
                        DoUnsummonArmy();
                        break;
                    case POINT_ID_SCEPTER_1:
                        pSummoned->GetMotionMaster()->MovePoint(POINT_ID_EPILOGUE, aEternalBoardMovement[4].m_fX, aEternalBoardMovement[4].m_fY, aEternalBoardMovement[4].m_fZ);
                        break;
                    case POINT_ID_EXIT:
                        pSummoned->ForcedDespawn();
                        break;
                }
            }
            else if (uiPointId == POINT_ID_DRAGON_ATTACK)
            {
                switch (pSummoned->GetEntry())
                {
                    case NPC_MERITHRA_OF_THE_DREAM:
                        StartNextDialogueText(DATA_MERITHRA_ATTACK);
                        break;
                    case NPC_CAELESTRASZ:
                        StartNextDialogueText(DATA_CAELASTRASZ_ATTACK);
                        break;
                }
            }
        }

        void ReceiveAIEvent(AIEventType eventType, Creature* pSender, Unit* pInvoker, uint32 /*uiMiscValue*/) override
        {
            if (eventType == AI_EVENT_CUSTOM_A && pSender == m_creature && pInvoker->GetTypeId() == TYPEID_PLAYER)
            {
                m_playerGuid = pInvoker->GetObjectGuid();
            }
        }

        void UpdateAI(const uint32 uiDiff) override
        {
            m_sceneElapsedMs += uiDiff;
            DialogueUpdate(uiDiff);
            UpdateArmySummoning();

            if (m_uiEventTimer)
            {
                if (m_uiEventTimer <= uiDiff)
                {
                    switch (m_uiEventStage)
                    {
                        case 0:
                            // Start the dialogue
                            StartNextDialogueText(NPC_ANACHRONOS_THE_ANCIENT);
                            m_uiEventTimer = 0;
                            break;
                        case 1:
                            // Do the epilogue movement
                            m_creature->GetMotionMaster()->MovePoint(POINT_ID_SCEPTER_2, aEternalBoardMovement[6].m_fX, aEternalBoardMovement[6].m_fY, aEternalBoardMovement[6].m_fZ);
                            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                            m_uiEventTimer = 0;
                            break;
                        case 2:
                            // Complete quest and despawn gate
                            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                            {
                                pPlayer->GroupEventHappens(QUEST_A_PAWN_ON_THE_ETERNAL_BOARD, m_creature);
                            }
                            m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                            m_uiEventTimer = 4000;
                            break;
                        case 3:
                            // Move to exit
                            m_creature->SetWalk(false);
                            m_creature->GetMotionMaster()->MovePoint(POINT_ID_EXIT, aEternalBoardMovement[8].m_fX, aEternalBoardMovement[8].m_fY, aEternalBoardMovement[8].m_fZ);
                            m_uiEventTimer = 0;
                            break;
                        case 4:
                            // Take off and fly
#if defined (CLASSIC)
                            m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND);
#else
                            m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_FLY_ANIM);
#endif
                            m_creature->SetLevitate(true);
                            m_creature->GetMotionMaster()->MovePoint(0, aEternalBoardMovement[9].m_fX, aEternalBoardMovement[9].m_fY, aEternalBoardMovement[9].m_fZ);
                            m_creature->ForcedDespawn(10000);
                            m_uiEventTimer = 0;
                            break;
                    }
                    ++m_uiEventStage;
                }
                else
                {
                    m_uiEventTimer -= uiDiff;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) override
    {
        return new npc_anachronos_the_ancientAI(pCreature);
    }
};

struct go_crystalline_tear : public GameObjectScript
{
    go_crystalline_tear() : GameObjectScript("go_crystalline_tear") {}

    bool OnQuestAccept(Player* pPlayer, GameObject* pGo, const Quest* pQuest) override
    {
        // Summon the controller dragon at GO position (orientation is wrong - hardcoded)
        if (pQuest->GetQuestId() == QUEST_A_PAWN_ON_THE_ETERNAL_BOARD)
        {
            // Check if event is already in progress first
            if (GetClosestCreatureWithEntry(pGo, NPC_ANACHRONOS_THE_ANCIENT, 90.0f))
            {
                return true;
            }

            if (Creature* pAnachronos = pPlayer->SummonCreature(NPC_ANACHRONOS_THE_ANCIENT, pGo->Where().X(), pGo->Where().Y(), pGo->Where().Z(), 3.75f, TEMPSPAWN_CORPSE_DESPAWN, 0))
            {
                // Send the player's guid in order to handle the quest complete
                if (CreatureAI* pAnachronosAI = pAnachronos->AI())
                {
                    pAnachronosAI->SendAIEvent(AI_EVENT_CUSTOM_A, pPlayer, pAnachronos);
                    //pAnachronosAI->m_playerGuid = pPlayer->GetObjectGuid();
                }
            }
        }

        return true;
    }
};

// The three dragon spells send map events which close the corresponding gate
// for 30 seconds. MaNGOS Zero dispatches these through SD3 MapEventScript.
struct event_aq_gate_closure : public MapEventScript
{
    event_aq_gate_closure() : MapEventScript("event_aq_gate_closure") {}

    bool OnReceived(uint32 eventId, Object* source, Object* /*target*/, bool isStart) override
    {
        WorldObject* worldSource = ToWorldObject(source);
        if (!isStart || !worldSource)
            return false;

        uint32 entry = 0;
        uint32 guid = 0;
        switch (eventId)
        {
            case EVENT_AQ_GATE_PRISMATIC_BARRIER:
                entry = GO_AQ_GATE_PRISMATIC_BARRIER;
                guid = GUID_AQ_GATE_PRISMATIC_BARRIER;
                break;
            case EVENT_AQ_GATE_CALL_ANCIENTS:
                entry = GO_AQ_GATE_CALL_ANCIENTS;
                guid = GUID_AQ_GATE_CALL_ANCIENTS;
                break;
            case EVENT_AQ_GATE_GLYPH_OF_WARDING:
                entry = GO_AQ_GATE_GLYPH_OF_WARDING;
                guid = GUID_AQ_GATE_GLYPH_OF_WARDING;
                break;
            default:
                return false;
        }

        GameObject* gate = worldSource->GetMap()->GetGameObject(ObjectGuid(HIGHGUID_GAMEOBJECT, entry, guid));
        if (gate && gate->GetGoType() == GAMEOBJECT_TYPE_DOOR && gate->GetGoState() != GO_STATE_READY)
            gate->UseDoorOrButton(30);

        return true;
    }
};

enum
{
    SPELL_FOOLS_PLIGHT = 23504,

    SPELL_SOUL_FLAME = 23272,
    SPELL_DREADFUL_FRIGHT = 23275,
    SPELL_CREEPING_DOOM = 23589,
    SPELL_CRIPPLING_CLIP = 23279,

    EMOTE_IMMOBILIZED = -1001252,
    SAY_THE_CLEANER_AGGRO = -1001253,

    SPELL_FROST_TRAP = 13810,

    NPC_NELSON_THE_NICE = 14536,
    GOSSIP_ITEM_NELSON_THE_NICE = -3509002,

    NPC_SOLENOR_THE_SLAYER = 14530,
    NPC_CREEPING_DOOM = 14761,
    NPC_THE_CLEANER = 14503,

    QUEST_STAVE_OF_THE_ANCIENTS = 7636,
};

/*#####
## npc_nelson_the_nice
######*/

/*#####
## npc_solenor_the_slayer
######*/

struct npc_solenor_the_slayer : public CreatureScript
{
    npc_solenor_the_slayer() : CreatureScript("npc_solenor_the_slayer") {}

    bool OnGossipHello(Player* pPlayer, Creature* pCreature) override
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        // Allow to begin the event only for a player (Hunter) who have not completed the quest yet.
        if (pPlayer->GetQuestStatus(QUEST_STAVE_OF_THE_ANCIENTS) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM_ID(0, GOSSIP_ITEM_NELSON_THE_NICE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 /*uiAction*/) override
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        ((npc_solenor_the_slayerAI*)pCreature->AI())->BeginEvent(pPlayer->GetObjectGuid());
        return true;
    }

    struct npc_solenor_the_slayerAI : public ScriptedAI
    {
        npc_solenor_the_slayerAI(Creature* pCreature) : ScriptedAI(pCreature), m_uiDespawn_Timer(0), m_bTransform(false)
        {
            Reset();
        }

        uint32 m_uiTransform_Timer;
        uint32 m_uiTransformEmote_Timer;
        bool m_bTransform;

        ObjectGuid m_hunterGuid;
        uint32 m_uiDreadfulFright_Timer;
        uint32 m_uiCreepingDoom_Timer;
        uint32 m_uiCastSoulFlame_Timer;
        uint32 m_uiDespawn_Timer;

        void Reset() override
        {
            switch (m_creature->GetEntry())
            {
                case NPC_NELSON_THE_NICE:
                {
                    m_creature->SetRespawnDelay(35 * MINUTE);
                    m_creature->SetRespawnTime(35 * MINUTE);
                    m_creature->NearTeleportTo(-7724.21f, 1676.43f, 7.0571f, 4.80044f);
                    if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != WAYPOINT_MOTION_TYPE)
                    {
                        m_creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                        m_creature->GetMotionMaster()->Initialize();
                    }

                    m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    m_uiTransform_Timer = 10000;
                    m_uiTransformEmote_Timer = 5000;
                    m_bTransform = false;
                    m_uiDespawn_Timer = 0;
                    m_uiCastSoulFlame_Timer = 0;
                    break;
                }
                case NPC_SOLENOR_THE_SLAYER:
                {
                    if (!m_uiDespawn_Timer)
                    {
                        m_uiDespawn_Timer = 20 * MINUTE * IN_MILLISECONDS;
                        m_uiCastSoulFlame_Timer = 150;
                        DoCastSpellIfCan(m_creature, SPELL_SOUL_FLAME);
                    }

                    m_hunterGuid.Clear();
                    m_uiDreadfulFright_Timer = urand(10000, 15000);
                    m_uiCreepingDoom_Timer = urand(3000, 6000);
                    break;
                }
            }
        }

        /** Nelson the Nice */
        void Transform()
        {
            m_creature->UpdateEntry(NPC_SOLENOR_THE_SLAYER);
            Reset();
        }

        void BeginEvent(ObjectGuid playerGuid)
        {
            m_hunterGuid = playerGuid;
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
            Player * player = sObjectMgr.GetPlayer(playerGuid);
            m_creature->SetFacingToObject(player);
            m_bTransform = true;
        }

        /** Solenor the Slayer */
        void Aggro(Unit* pWho) override
        {
            if (pWho->getClass() == CLASS_HUNTER && (m_hunterGuid.IsEmpty() || m_hunterGuid == pWho->GetObjectGuid())/*&& pWho->GetQuestStatus(QUEST_STAVE_OF_THE_ANCIENTS) == QUEST_STATUS_INCOMPLETE*/)
            {
                m_hunterGuid = pWho->GetObjectGuid();
            }
            else
            {
                DemonDespawn(pWho);
            }
        }

        void EnterEvadeMode() override
        {
            m_creature->RemoveGuardians();

            ScriptedAI::EnterEvadeMode();
        }

        void JustSummoned(Creature* pSummoned) override
        {
            if (m_creature->getVictim())
            {
                pSummoned->AI()->AttackStart(m_creature->getVictim());
            }
        }

        void JustDied(Unit* /*pKiller*/) override
        {
            uint32 m_respawn_delay_Timer = urand(2,3) * HOUR;
            m_creature->SetRespawnDelay(m_respawn_delay_Timer);
            m_creature->SetRespawnTime(m_respawn_delay_Timer);
            m_creature->SaveRespawnTime();
        }

        void DemonDespawn(Unit * playerFacing = nullptr,  bool triggered = true)
        {
            m_creature->RemoveGuardians();
            uint32 respawnTime = urand(12,15);
            m_creature->SetRespawnDelay(respawnTime * MINUTE);
            m_creature->SetRespawnTime(respawnTime * MINUTE);
            m_creature->SaveRespawnTime();

            if (triggered)
            {
                Creature* creature_the_cleaner = m_creature->SummonCreature(NPC_THE_CLEANER, m_creature->Where().X(), m_creature->Where().Y(), m_creature->Where().Z(), m_creature->Where().BearingTo(playerFacing->Where()), TEMPSPAWN_CORPSE_DESPAWN, 20 * MINUTE * IN_MILLISECONDS);
                if (creature_the_cleaner)
                {
                    DoScriptText(SAY_THE_CLEANER_AGGRO, creature_the_cleaner);
                    ThreatList const& tList = m_creature->GetThreatManager().getThreatList();
                    for (auto itr : tList)
                    {
                        if (Unit* pUnit = m_creature->GetMap()->GetUnit(itr->getUnitGuid()))
                        {
                            if (pUnit->IsAlive())
                            {
                                creature_the_cleaner->AI()->AttackStart(pUnit);
                            }
                        }
                    }
                }
            }

            m_creature->ForcedDespawn();
        }

        void SpellHit(Unit* /*pCaster*/, const SpellEntry* pSpell) override
        {

            if (pSpell && SD3_SpellId(pSpell) == 14268)   // Wing Clip (Rank 3)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_CRIPPLING_CLIP, CAST_TRIGGERED) == CAST_OK)
                {
                    DoScriptText(EMOTE_IMMOBILIZED, m_creature);
                }
            }
        }

        void UpdateAI(const uint32 uiDiff) override
        {

            /** Nelson the Nice */
            if (m_bTransform)
            {
                if (m_uiTransformEmote_Timer)
                {
                    if (m_uiTransformEmote_Timer <= uiDiff)
                    {
                        m_creature->HandleEmote(EMOTE_ONESHOT_LAUGH);
                        m_uiTransformEmote_Timer = 0;
                    }
                    else
                    {
                        m_uiTransformEmote_Timer -= uiDiff;
                    }
                }

                if (m_uiTransform_Timer < uiDiff)
                {
                    m_bTransform = false;
                    Transform();
                }
                else
                {
                    m_uiTransform_Timer -= uiDiff;
                }
            }

            /** Solenor the Slayer */
            if (m_uiDespawn_Timer)
            {
                if (m_uiDespawn_Timer <= uiDiff)
                {
                    if (m_creature->IsAlive() && !m_creature->IsInCombat())
                    {
                        DemonDespawn(nullptr, false);
                    }
                }
                else
                {
                    m_uiDespawn_Timer -= uiDiff;
                }
            }

            if (m_uiCastSoulFlame_Timer)
            {
                if (m_uiCastSoulFlame_Timer <= uiDiff)
                {
                    // delay this cast so spell animation is visible to the player
                    DoCastSpellIfCan(m_creature, SPELL_SOUL_FLAME);
                    m_uiCastSoulFlame_Timer = 0;
                }
                else
                {
                    m_uiCastSoulFlame_Timer -= uiDiff;
                }
            }

            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            {
                return;
            }

            if (m_creature->HasAura(SPELL_SOUL_FLAME) && m_creature->HasAura(SPELL_FROST_TRAP))
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SOUL_FLAME);
            }

            if (m_creature->GetThreatManager().getThreatList().size() > 1 /*|| pHunter->IsDead()*/)
            {
                DemonDespawn(m_creature->getVictim());
            }

            if (m_uiCreepingDoom_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_CREEPING_DOOM);
                m_uiCreepingDoom_Timer = 15000;
            }
            else
            {
                m_uiCreepingDoom_Timer -= uiDiff;
            }

            if (m_uiDreadfulFright_Timer < uiDiff)
            {
                if (Unit* pUnit = m_creature->getVictim())
                {

                    if (m_creature->Where().DistanceTo(pUnit->Where(), false) > 5.0f)
                    {
                        if (DoCastSpellIfCan(pUnit, SPELL_DREADFUL_FRIGHT) == CAST_OK)
                        {
                            m_uiDreadfulFright_Timer = urand(15000, 20000);
                        }
                    }
                }
            }
            else
            {
                m_uiDreadfulFright_Timer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) override
    {
        return new npc_solenor_the_slayerAI(pCreature);
    }
};

// Ahn'Qiraj war effort colossi. The reference creature spell list gives all
// three bosses Colossal Smash (26167), initially after 15-30 seconds and then
// every 60-90 seconds. The event controller owns the persistent kill state.
struct npc_aq_war_colossus : public CreatureScript
{
    npc_aq_war_colossus() : CreatureScript("npc_aq_war_colossus") {}

    struct npc_aq_war_colossusAI : public ScriptedAI
    {
        explicit npc_aq_war_colossusAI(Creature* creature) : ScriptedAI(creature), m_uiColossalSmashTimer(0) {}

        uint32 m_uiColossalSmashTimer;

        void Reset() override
        {
            m_uiColossalSmashTimer = urand(15000, 30000);
        }

        void UpdateAI(const uint32 uiDiff) override
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (m_uiColossalSmashTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), 26167) == CAST_OK)
                    m_uiColossalSmashTimer = urand(60000, 90000);
                else
                    m_uiColossalSmashTimer = 1000;
            }
            else
                m_uiColossalSmashTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }

        void JustRespawned() override
        {
            ScriptedAI::JustRespawned();
            if (!sWorld.getConfig(CONFIG_BOOL_AQ_WAR_EFFORT))
                return;

            switch (m_creature->GetEntry())
            {
                case 15740: DoScriptText(-1999923, m_creature); break;
                case 15741: DoScriptText(-1999924, m_creature); break;
                case 15742: DoScriptText(-1999925, m_creature); break;
                default: break;
            }
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (!sWorld.getConfig(CONFIG_BOOL_AQ_WAR_EFFORT))
                return;

            AqWarEffort::Colossus boss;
            uint32 deathSpawnGuid = 0;
            switch (m_creature->GetEntry())
            {
                // Source Classic DB creature GUIDs 155123, 155124, and 155122
                // are remapped to 800055, 800056, and 800054 in migration 006.
                case 15742: boss = AqWarEffort::Colossus::Ashi; deathSpawnGuid = 800055; break;
                case 15741: boss = AqWarEffort::Colossus::Regal; deathSpawnGuid = 800056; break;
                case 15740: boss = AqWarEffort::Colossus::Zora; deathSpawnGuid = 800054; break;
                default: return;
            }

            CreatureData const* spawnData = sObjectMgr.GetCreatureData(deathSpawnGuid);
            if (!spawnData || spawnData->mapid != m_creature->GetMap()->GetId())
            {
                sLog.outError("AQ colossus %u has no matching death spawn GUID %u on map %u",
                    m_creature->GetEntry(), deathSpawnGuid, m_creature->GetMap()->GetId());
            }
            else
            {
                // Match CMaNGOS WorldObject::SpawnCreature: materialize the
                // static database spawn immediately at its authored location.
                Creature* deathSpawn = new Creature;
                if (!deathSpawn->LoadFromDB(deathSpawnGuid, m_creature->GetMap()))
                    delete deathSpawn;
            }

            auto const result = AqWarEffort::GetRuntime().ReportColossusDeath(boss);
            if (result == AqWarEffort::Result::StorageError || result == AqWarEffort::Result::Busy)
                sLog.outError("Failed to persist AQ war effort colossus death (%u); queued for retry.", m_creature->GetEntry());
        }
    };

    CreatureAI* GetAI(Creature* creature) override
    {
        return new npc_aq_war_colossusAI(creature);
    }
};

void AddSC_silithus()
{
    Script* s;
    s = new npc_anachronos_the_ancient();
    s->RegisterSelf();
    s = new go_crystalline_tear();
    s->RegisterSelf();
    s = new event_aq_gate_closure();
    s->RegisterSelf();

    s = new npc_solenor_the_slayer();
    s->RegisterSelf();
    s = new npc_aq_war_colossus();
    s->RegisterSelf();
}
