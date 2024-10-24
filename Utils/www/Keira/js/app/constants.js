/*jslint browser: true, eqeq: true, white: true, plusplus: true */
/*global angular, console, alert*/

(function () {
  'use strict';

  var app = angular.module('keira2');

  /* Constants mostly taken from https://github.com/Discover-/SAI-Editor/blob/master/SAI-Editor/Enumerators/PublicEnums.cs */

  /* Init arrays */
  app.constants = [];
  app.saiConstants = [];
  app.modalConstants = [];


  /* UNIT_FLAG constants */
  app.constants.unitFlag = {
    NONE              : 0x00000000,
    SERVER_CONTROLLED : 0x00000001,
    NON_ATTACKABLE    : 0x00000002,
    DISABLE_MOVE      : 0x00000004,
    PVP_ATTACKABLE    : 0x00000008,
    RENAME            : 0x00000010,
    PREPARATION       : 0x00000020,
    UNK_6             : 0x00000040,
    NOT_ATTACKABLE_1  : 0x00000080,
    IMMUNE_TO_PC      : 0x00000100,
    IMMUNE_TO_NPC     : 0x00000200,
    LOOTING           : 0x00000400,
    PET_IN_COMBAT     : 0x00000800,
    PVP               : 0x00001000,
    SILENCED          : 0x00002000,
    UNK_14            : 0x00004000,
    UNK_15            : 0x00008000,
    UNK_16            : 0x00010000,
    PACIFIED          : 0x00020000,
    STUNNED           : 0x00040000,
    IN_COMBAT         : 0x00080000,
    TAXI_FLIGHT       : 0x00100000,
    DISARMED          : 0x00200000,
    CONFUSED          : 0x00400000,
    FLEEING           : 0x00800000,
    PLAYER_CONTROLLED : 0x01000000,
    NOT_SELECTABLE    : 0x02000000,
    SKINNABLE         : 0x04000000,
    MOUNT             : 0x08000000,
    UNK_28            : 0x10000000,
    UNK_29            : 0x20000000,
    SHEATHE           : 0x40000000
  };

  /* NPC_FLAG constants */
  app.constants.npcFlag = {
    NONE               : 0x00000000,
    GOSSIP             : 0x00000001,
    QUESTGIVER         : 0x00000002,
    UNK1               : 0x00000004,
    UNK2               : 0x00000008,
    TRAINER            : 0x00000010,
    TRAINER_CLASS      : 0x00000020,
    TRAINER_PROFESSION : 0x00000040,
    VENDOR             : 0x00000080,
    VENDOR_AMMO        : 0x00000100,
    VENDOR_FOOD        : 0x00000200,
    VENDOR_POISON      : 0x00000400,
    VENDOR_REAGENT     : 0x00000800,
    REPAIR             : 0x00001000,
    FLIGHTMASTER       : 0x00002000,
    SPIRITHEALER       : 0x00004000,
    SPIRITGUIDE        : 0x00008000,
    INNKEEPER          : 0x00010000,
    BANKER             : 0x00020000,
    PETITIONER         : 0x00040000,
    TABARDDESIGNER     : 0x00080000,
    BATTLEMASTER       : 0x00100000,
    AUCTIONEER         : 0x00200000,
    STABLEMASTER       : 0x00400000,
    GUILD_BANKER       : 0x00800000,
    SPELLCLICK         : 0x01000000,
    PLAYER_VEHICLE     : 0x02000000
  };

  /* GameObject Flags */
  app.constants.goFlag = {
    NONE           : 0x00000000,
    IN_USE         : 0x00000001,
    LOCKED         : 0x00000002,
    INTERACT_COND  : 0x00000004,
    TRANSPORT      : 0x00000008,
    NOT_SELECTABLE : 0x00000010,
    NODESPAWN      : 0x00000020,
    TRIGGERED      : 0x00000040,
    DAMAGED        : 0x00000200,
    DESTROYED      : 0x00000400
  };

  /* Dynamic Flags */
  app.constants.dynamicFlag = {
    NONE                      : 0x0000,
    LOOTABLE                  : 0x0001,
    TRACK_UNIT                : 0x0002,
    TAPPED                    : 0x0004,
    TAPPED_BY_PLAYER          : 0x0008,
    SPECIALINFO               : 0x0010,
    DEAD                      : 0x0020,
    REFER_A_FRIEND            : 0x0040,
    TAPPED_BY_ALL_THREAT_LIST : 0x0080
  };

  /* Unit Field Bytes1 Types */
  app.constants.unitFieldBytes1Type = {
    STAND_STAND_STATE_TYPE : 0,
    PET_TALENTS_TYPE       : 1,
    STAND_FLAGS_TYPE       : 2,
    BYTES1_FLAGS_TYPE      : 3
  };

  /* Unit Stand State Types */
  app.constants.unitStandStateType = {
    STAND            : 0,
    SIT              : 1,
    SIT_CHAIR        : 2,
    SLEEP            : 3,
    SIT_LOW_CHAIR    : 4,
    SIT_MEDIUM_CHAIR : 5,
    SIT_HIGH_CHAIR   : 6,
    DEAD             : 7,
    KNEEL            : 8,
    SUBMERGED        : 9
  };

  /* Unit Stand Flags */
  app.constants.unitStandFlags = {
    NONE        : 0x00,
    UNK1        : 0x01,
    CREEP       : 0x02,
    UNTRACKABLE : 0x04,
    UNK4        : 0x08,
    UNK5        : 0x10,
    ALL         : 0xFF
  };

  /* Unit Bytes1 Flags */
  app.constants.unitBytes1Flags = {
    UNIT_BYTE1_FLAG_ALWAYS_STAND : 0x01,
    UNIT_BYTE1_FLAG_HOVER        : 0x02,
    UNIT_BYTE1_FLAG_UNK_3        : 0x04,
    UNIT_BYTE1_FLAG_ALL          : 0xFF
  };

  /*  SAI event constants */
  app.saiConstants.event = {
    UPDATE_IC : 0,
    UPDATE_OOC : 1,
    HEALT_PCT : 2,
    MANA_PCT : 3,
    AGGRO : 4,
    KILL : 5,
    DEATH : 6,
    EVADE : 7,
    SPELLHIT : 8,
    RANGE : 9,
    OOC_LOS : 10,
    RESPAWN : 11,
    TARGET_HEALTH_PCT : 12,
    VICTIM_CASTING : 13,
    FRIENDLY_HEALTH : 14,
    FRIENDLY_IS_CC : 15,
    FRIENDLY_MISSING_BUFF : 16,
    SUMMONED_UNIT : 17,
    TARGET_MANA_PCT : 18,
    ACCEPTED_QUEST : 19,
    REWARD_QUEST : 20,
    REACHED_HOME : 21,
    RECEIVE_EMOTE : 22,
    HAS_AURA : 23,
    TARGET_BUFFED : 24,
    RESET : 25,
    IC_LOS : 26,
    PASSENGER_BOARDED : 27,
    PASSENGER_REMOVED : 28,
    CHARMED : 29,
    CHARMED_TARGET : 30,
    SPELLHIT_TARGET : 31,
    DAMAGED : 32,
    DAMAGED_TARGET : 33,
    MOVEMENTINFORM : 34,
    SUMMON_DESPAWNED : 35,
    CORPSE_REMOVED : 36,
    AI_INIT : 37,
    DATA_SET : 38,
    WAYPOINT_START : 39,
    WAYPOINT_REACHED : 40,
    TRANSPORT_ADDPLAYER_NYI : 41,
    TRANSPORT_ADDCREATURE_NYI : 42,
    TRANSPORT_REMOVE_PLAYER_NYI : 43,
    TRANSPORT_RELOCATE_NYI : 44,
    INSTANCE_PLAYER_ENTER_NYI : 45,
    AREATRIGGER_ONTRIGGER : 46,
    QUEST_ACCEPTED_NYI : 47,
    QUEST_OBJ_COPLETETION_NYI : 48,
    QUEST_COMPLETION_NYI : 49,
    QUEST_REWARDED_NYI : 50,
    QUEST_FAIL_NYI : 51,
    TEXT_OVER : 52,
    RECEIVE_HEAL : 53,
    JUST_SUMMONED : 54,
    WAYPOINT_PAUSED : 55,
    WAYPOINT_RESUMED : 56,
    WAYPOINT_STOPPED : 57,
    WAYPOINT_ENDED : 58,
    TIMED_EVENT_TRIGGERED : 59,
    UPDATE : 60,
    LINK : 61,
    GOSSIP_SELECT : 62,
    JUST_CREATED : 63,
    GOSSIP_HELLO : 64,
    FOLLOW_COMPLETED : 65,
    DUMMY_EFFECT_NYI : 66,
    IS_BEHIND_TARGET : 67,
    GAME_EVENT_START : 68,
    GAME_EVENT_END : 69,
    GO_STATE_CHANGED : 70,
    GO_EVENT_INFORM : 71,
    ACTION_DONE : 72,
    ON_SPELLCLICK : 73,
    FRIENDLY_HEALTH_PCT : 74,
    DISTANCE_CREATURE : 75,
    DISTANCE_GAMEOBJECT : 76,
    MAX : 77
  };

  /* SAI action constants */
  app.saiConstants.action = {
    NONE : 0,
    TALK : 1,
    SET_FACTION : 2,
    MORPH_TO_ENTRY_OR_MODEL : 3,
    SOUND : 4,
    EMOTE : 5,
    FAIL_QUEST : 6,
    ADD_QUEST : 7,
    SET_REACT_STATE : 8,
    ACTIVATE_GOBJECT : 9,
    RANDOM_EMOTE : 10,
    CAST : 11,
    SUMMON_CREATURE : 12,
    THREAT_SINGLE_PCT : 13,
    THREAT_ALL_PCT : 14,
    CALL_AREAEXPLOREDOREVENTHAPPENS : 15,
    UNUSED_16 : 16,
    SET_EMOTE_STATE : 17,
    SET_UNIT_FLAG : 18,
    REMOVE_UNIT_FLAG : 19,
    AUTO_ATTACK : 20,
    COMBAT_MOVEMENT : 21,
    SET_EVENT_PHASE : 22,
    INC_EVENT_PHASE : 23,
    EVADE : 24,
    FLEE_FOR_ASSIST : 25,
    CALL_GROUPEVENTHAPPENS : 26,
    CALL_CASTEDCREATUREORGO : 27,
    REMOVEAURASFROMSPELL : 28,
    FOLLOW : 29,
    RANDOM_PHASE : 30,
    RANDOM_PHASE_RANGE : 31,
    RESET_GOBJECT : 32,
    KILLED_MONSTER : 33,
    SET_INST_DATA : 34,
    SET_INST_DATA64 : 35,
    UPDATE_TEMPLATE : 36,
    DIE : 37,
    SET_IN_COMBAT_WITH_ZONE : 38,
    CALL_FOR_HELP : 39,
    SET_SHEATH : 40,
    FORCE_DESPAWN : 41,
    SET_INVINCIBILITY_HP_LEVEL : 42,
    MOUNT_TO_ENTRY_OR_MODEL : 43,
    SET_PHASE_MASK : 44,
    SET_DATA : 45,
    MOVE_FORWARD : 46,
    SET_VISIBILITY : 47,
    SET_ACTIVE : 48,
    ATTACK_START : 49,
    SUMMON_GO : 50,
    KILL_UNIT : 51,
    ACTIVATE_TAXI : 52,
    WP_START : 53,
    WP_PAUSE : 54,
    WP_STOP : 55,
    ADD_ITEM : 56,
    REMOVE_ITEM : 57,
    INSTALL_AI_TEMPLATE : 58,
    SET_RUN : 59,
    SET_FLY : 60,
    SET_SWIM : 61,
    TELEPORT : 62,
    UNUSED_63 : 63,
    STORE_TARGET_LIST : 64,
    WP_RESUME : 65,
    SET_ORIENTATION : 66,
    CREATE_TIMED_EVENT : 67,
    PLAYMOVIE : 68,
    MOVE_TO_POS : 69,
    RESPAWN_TARGET : 70,
    EQUIP : 71,
    CLOSE_GOSSIP : 72,
    TRIGGER_TIMED_EVENT : 73,
    REMOVE_TIMED_EVENT : 74,
    ADD_AURA : 75,
    OVERRIDE_SCRIPT_BASE_OBJECT : 76,
    RESET_SCRIPT_BASE_OBJECT : 77,
    CALL_SCRIPT_RESET : 78,
    SET_RANGED_MOVEMENT : 79,
    CALL_TIMED_ACTIONLIST : 80,
    SET_NPC_FLAG : 81,
    ADD_NPC_FLAG : 82,
    REMOVE_NPC_FLAG : 83,
    SIMPLE_TALK : 84,
    INVOKER_CAST : 85,
    CROSS_CAST : 86,
    CALL_RANDOM_TIMED_ACTIONLIST : 87,
    CALL_RANDOM_RANGE_TIMED_ACTIONLIST : 88,
    RANDOM_MOVE : 89,
    SET_UNIT_FIELD_BYTES_1 : 90,
    REMOVE_UNIT_FIELD_BYTES_1 : 91,
    INTERRUPT_SPELL : 92,
    SEND_GO_CUSTOM_ANIM : 93,
    SET_DYNAMIC_FLAG : 94,
    ADD_DYNAMIC_FLAG : 95,
    REMOVE_DYNAMIC_FLAG : 96,
    JUMP_TO_POS : 97,
    SEND_GOSSIP_MENU : 98,
    GO_SET_LOOT_STATE : 99,
    SEND_TARGET_TO_TARGET : 100,
    SET_HOME_POS : 101,
    SET_HEALTH_REGEN : 102,
    SET_ROOT : 103,
    SET_GO_FLAG : 104,
    ADD_GO_FLAG : 105,
    REMOVE_GO_FLAG : 106,
    SUMMON_CREATURE_GROUP : 107,
    SET_POWER : 108,
    ADD_POWER : 109,
    REMOVE_POWER : 110,
    GAME_EVENT_STOP : 111,
    GAME_EVENT_START : 112,
    START_CLOSEST_WAYPOINT : 113,
    MAX : 114
  };

  /* SAI target constants */
  app.saiConstants.target = {
    NONE : 0,
    SELF : 1,
    VICTIM : 2,
    HOSTILE_SECOND_AGGRO : 3,
    HOSTILE_LAST_AGGRO : 4,
    HOSTILE_RANDOM : 5,
    HOSTILE_RANDOM_NOT_TOP : 6,
    ACTION_INVOKER : 7,
    POSITION : 8,
    CREATURE_RANGE : 9,
    CREATURE_GUID : 10,
    CREATURE_DISTANCE : 11,
    STORED : 12,
    GAMEOBJECT_RANGE : 13,
    GAMEOBJECT_GUID : 14,
    GAMEOBJECT_DISTANCE : 15,
    INVOKER_PARTY : 16,
    PLAYER_RANGE : 17,
    PLAYER_DISTANCE : 18,
    CLOSEST_CREATURE : 19,
    CLOSEST_GAMEOBJECT : 20,
    CLOSEST_PLAYER : 21,
    ACTION_INVOKER_VEHICLE : 22,
    OWNER_OR_SUMMONER : 23,
    THREAT_LIST : 24,
    CLOSEST_ENEMY : 25,
    CLOSEST_FRIENDLY : 26,
    LOOT_RECIPIENTS : 27,
    MAX : 28
  };

  /* SAI template constants */
  app.saiConstants.templates = {
    BASIC           : 0,
    CASTER          : 1,
    TURRET          : 2,
    PASSIVE         : 3,
    CAGED_GO_PART   : 4,
    CAGED_NPC_PART  : 5
  };

  /* SAI phase masks */
  app.saiConstants.phaseMask = {
    ALWAYS : 0x000,
    1      : 0x001,
    2      : 0x002,
    3      : 0x004,
    4      : 0x008,
    5      : 0x010,
    6      : 0x020,
    7      : 0x040,
    8      : 0x080,
    9      : 0x100
  };

  /* SAI event flags */
  app.saiConstants.eventFlags = {
    NONE           : 0x00,
    NOT_REPEATABLE : 0x01,
    NORMAL_DUNGEON : 0x02,
    HEROIC_DUNGEON : 0x04,
    NORMAL_RAID    : 0x08,
    HEROIC_RAID    : 0x10,
    DEBUG_ONLY     : 0x80
  };

  /* GO constants */
  app.goConstants = {};
  app.goConstants.data0     = [];
  app.goConstants.data0[0]  = "0| startOpen (0/1)";
  app.goConstants.data0[1]  = "0| startOpen (0/1)";
  app.goConstants.data0[2]  = "0| open(Lock.dbc)";
  app.goConstants.data0[3]  = "0| open(Lock.dbc)";
  app.goConstants.data0[5]  = "0| floatingTooltip (0/1)";
  app.goConstants.data0[6]  = "0| open (Lock.dbc)";
  app.goConstants.data0[7]  = "0| chairslots (Nbr joueurs pouvant s'asseoir)";
  app.goConstants.data0[8]  = "0| SpellFocusObject.dbc";
  app.goConstants.data0[9]  = "0| pageID (page_text.entry)";
  app.goConstants.data0[10] = "0| open Lock.dbc-attaque(99)/ouvrir(43)-temp pour ouvrir(1949)";
  app.goConstants.data0[11] = "0| Timeto2ndfloor";
  app.goConstants.data0[12] = "0| open";
  app.goConstants.data0[13] = "0| open (Lock.dbc)";
  app.goConstants.data0[15] = "0| taxiPathID (TaxiPath.dbc)";
  app.goConstants.data0[18] = "0| casters";
  app.goConstants.data0[20] = "0| actionHouseID (AuctionHouse.dbc)";
  app.goConstants.data0[21] = "0| CreatureID";
  app.goConstants.data0[22] = "0| Spell.dbc";
  app.goConstants.data0[23] = "0| minLevel";
  app.goConstants.data0[24] = "0| open (Lock.dbc)";
  app.goConstants.data0[25] = "0| radius (distance)";
  app.goConstants.data0[26] = "0| open (Lock.dbc)";
  app.goConstants.data0[27] = "0| areatrigger_teleport.id";
  app.goConstants.data0[29] = "0| radius (Distance)";
  app.goConstants.data0[30] = "0| startOpen (0/1)";
  app.goConstants.data0[31] = "0| mapID (Map.dbc)";
  app.goConstants.data0[32] = "0| chairheight";
  app.goConstants.data0[33] = "0| intactNumHits";
  app.goConstants.data0[35] = "0| whenToPause";
  app.goConstants.data0[36] = "0| open : Lock_";
  app.goConstants.data0[37] = "0| open : Lock_";
  app.goConstants.data0[38] = "0| open : Lock_";
  app.goConstants.data0[39] = "0| Plot Instance: GarrPlotInstance, NoValue = 0";
  app.goConstants.data0[40] = "0| CreatureDisplayInfo";
  app.goConstants.data0[41] = "0| Item";
  app.goConstants.data0[42] = "0| Capture (ms), 0-2147483647, Default 60000";
  app.goConstants.data0[43] = "0| mapID, NoValue = -1";
  app.goConstants.data0[44] = "0| Trophy Type ID";
  app.goConstants.data0[45] = "0| CharShipmentContainer";
  app.goConstants.data0[46] = "0| Trophy Instance ID";
  app.goConstants.data0[48] = "0| UI Link Type : 10";
  
  app.goConstants.data1     = [];
  app.goConstants.data1[0]  = "1| open (Lock.dbc)";
  app.goConstants.data1[1]  = "1| open (Lock.dbc)";
  app.goConstants.data1[2]  = "1| questList";
  app.goConstants.data1[3]  = "1| gameobject_loot_template";
  app.goConstants.data1[5]  = "1| highlight (0/1)";
  app.goConstants.data1[6]  = "1| level (lvl equivalent d'un NPC)";
  app.goConstants.data1[7]  = "1| chairorientation";
  app.goConstants.data1[8]  = "1| diameter (radius*2)";
  app.goConstants.data1[9]  = "1| language (Languages.dbc)";
  app.goConstants.data1[10] = "1| questID";
  app.goConstants.data1[11] = "1| startOpen";
  app.goConstants.data1[12] = "1| radius";  
  app.goConstants.data1[13] = "1| camera (CinematicCamera.dbc)";
  app.goConstants.data1[15] = "1| moveSpeed";
  app.goConstants.data1[18] = "1| Spell.dbc";
  app.goConstants.data1[21] = "1| unk";
  app.goConstants.data1[22] = "1| charges";
  app.goConstants.data1[23] = "1| maxLevel";
  app.goConstants.data1[24] = "1| pickupSpell (Spell.dbc)";
  app.goConstants.data1[25] = "1| chestLoot (gameobject_loot_template.entry)";
  app.goConstants.data1[26] = "1| eventID";
  app.goConstants.data1[29] = "1| spell (not a spell.dbc ?!)";
  app.goConstants.data1[30] = "1| radius (Distance)";
  app.goConstants.data1[31] = "1| difficulty : 0) 5-10 normal , 1) 5-25 normal , 2) 10 heroic , 3) 25 heroic";
  app.goConstants.data1[32] = "1| HeightOffset";
  app.goConstants.data1[33] = "1| creditProxyCreature";
  app.goConstants.data1[35] = "1| startOpen";
  app.goConstants.data1[36] = "1| pickupSpell : Spell";
  app.goConstants.data1[39] = "1| mapID, NoValue = -1";
  app.goConstants.data1[40] = "1| Anim Kit";
  app.goConstants.data1[42] = "1| Gigantic AOI (0/1)";
  app.goConstants.data1[43] = "1| Area Names,-2147483648 a 2147483647, Default : 0";
  app.goConstants.data1[44] = "1| Trophy Instance ID";
  app.goConstants.data1[45] = "1| Gigantic AOI (0/1)";

  app.goConstants.data2     = [];
  app.goConstants.data2[0]  = "2| autoClose (ms)";
  app.goConstants.data2[1]  = "2| autoClose (ms)";
  app.goConstants.data2[2]  = "2| PageTextMaterial (1 parchemin,2 pierre,3 marbre,4 argent,5 bronze)";
  app.goConstants.data2[3]  = "2| chestRestockTime (en seconde)";
  app.goConstants.data2[5]  = "2| serverOnly (Toujours 0)";
  app.goConstants.data2[6]  = "2| diameter (radius * 2)";
  app.goConstants.data2[7]  = "2| onlyCreatorUse";
  app.goConstants.data2[8]  = "linkedTrap (gameobject_template-Spawned Ty 6)";
  app.goConstants.data2[9]  = "2| pageMaterial (1 parchemin,2 pierre,3 marbre,4 argent,5 bronze)";
  app.goConstants.data2[10] = "2| event_script id / T ouverture si Data0=1949";
  app.goConstants.data2[11] = "2| autoClose";
  app.goConstants.data2[12] = "2| damageMin";
  app.goConstants.data2[13] = "2| eventID";
  app.goConstants.data2[15] = "2| accelRate";
  app.goConstants.data2[18] = "2| animSpell (Spell.dbc)";
  app.goConstants.data2[22] = "2| partyOnly (0/1 doit etre en groupe)";
  app.goConstants.data2[23] = "2| AreaTable.dbc";
  app.goConstants.data2[24] = "2| radius (distance)";
  app.goConstants.data2[25] = "2| minRestock";
  app.goConstants.data2[26] = "2| pickupSpell (Spell.dbc)";
  app.goConstants.data2[29] = "2| worldState1";
  app.goConstants.data2[30] = "2| auraID1 (Spell.dbc)";
  app.goConstants.data2[31] = "2| DifficultyHeroic";
  app.goConstants.data2[32] = "2| SitAnimKit";
  app.goConstants.data2[33] = "2| state1Name";
  app.goConstants.data2[35] = "2| autoClose";
  app.goConstants.data2[36] = "2| openTextID : BroadcastText";
  app.goConstants.data2[40] = "2| creatureID";
  app.goConstants.data2[42] = "2| highlight (0/1)";
  app.goConstants.data2[43] = "2| Primary doodad set, -2147483648 a 2147483647, Default : 0";
  app.goConstants.data2[45] = "2| Large AOI (0/1)";


  app.goConstants.data3     = [];
  app.goConstants.data3[0]  = "3| noDamageImmune (0/1)";
  app.goConstants.data3[1]  = "3| linkedTrap (Spawned GO Ty 6)";
  app.goConstants.data3[2]  = "3| gossip_menu_option.menu_id";
  app.goConstants.data3[3]  = "3| consumable (0/1)";
  app.goConstants.data3[5]  = "3| large (0/1)";
  app.goConstants.data3[6]  = "3| spell (Spell.dbc)";
  app.goConstants.data3[7]  = "3| triggeredEvent";
  app.goConstants.data3[8]  = "3| serverOnly (Toujour 0)";
  app.goConstants.data3[9]  = "3| Monture autorise";
  app.goConstants.data3[10] = "3| Temps en ms avant etat initial";
  app.goConstants.data3[11] = "3| Reached1stfloor";
  app.goConstants.data3[12] = "3| damageMax";
  app.goConstants.data3[13] = "3| openTextID";
  app.goConstants.data3[15] = "3| startEventID";
  app.goConstants.data3[18] = "3| ritualPersistent (0/1)";
  app.goConstants.data3[22] = "3| Monture autorise";
  app.goConstants.data3[24] = "3| returnAura (from Spell.dbc)";
  app.goConstants.data3[25] = "3| maxRestock";
  app.goConstants.data3[26] = "3| noDamageImmune (0/1)";
  app.goConstants.data3[29] = "3| worldstate2";
  app.goConstants.data3[30] = "3| conditionID1 (conditionID1)";
  app.goConstants.data3[31] = "3| DifficultyEpic";
  app.goConstants.data3[33] = "3| intactEvent";
  app.goConstants.data3[35] = "3| BlocksPathsDown";
  app.goConstants.data3[36] = "3| require LOS (0/1)";
  app.goConstants.data3[42] = "3| open";
  app.goConstants.data3[43] = "3| Secondary doodad set, -2147483648 a 2147483647, Default: 0";
  
  app.goConstants.data4     = [];
  app.goConstants.data4[0]  = "4| openTextID";
  app.goConstants.data4[1]  = "4| noDamageImmune (0/1)";
  app.goConstants.data4[2]  = "4| customAnim (1 a 4)";
  app.goConstants.data4[3]  = "4| Min succes loot pour metier";
  app.goConstants.data4[5]  = "4| floatOnWater (0/1)";
  app.goConstants.data4[6]  = "4| type (0 no despawn - 1 despawn apres cast - 2 cast au spawn)";
  app.goConstants.data4[7]  = "4| conditionID1";
  app.goConstants.data4[8]  = "4| questID";
  app.goConstants.data4[9]  = "4| conditionID1";
  app.goConstants.data4[10] = "4| customAnim";
  app.goConstants.data4[11] = "4| Reached2ndfloor";
  app.goConstants.data4[12] = "4| damageSchool";
  app.goConstants.data4[13] = "4| conditionID1";
  app.goConstants.data4[15] = "4| stopEventID";
  app.goConstants.data4[18] = "4| casterTargetSpell (Spell.dbc)";
  app.goConstants.data4[22] = "4| GiganticAOI";
  app.goConstants.data4[24] = "4| returnSpell (from Spell.dbc)";
  app.goConstants.data4[25] = "4| open";
  app.goConstants.data4[26] = "4| openTextID";
  app.goConstants.data4[29] = "4| winEventID1";
  app.goConstants.data4[30] = "4| auraID2";
  app.goConstants.data4[31] = "4| DifficultyLegendary";
  app.goConstants.data4[33] = "4| damagedDisplayId";
  app.goConstants.data4[35] = "4| PathBlockerBump";
  app.goConstants.data4[36] = "4| conditionID1 : PlayerCondition";
  app.goConstants.data4[42] = "4| Assault Broadcast (Horde): BroadcastText";
  
  app.goConstants.data5     = [];
  app.goConstants.data5[0]  = "5| closeTextID";
  app.goConstants.data5[1]  = "5| large (0/1)";
  app.goConstants.data5[2]  = "5| noDamageImmune (0/1)";
  app.goConstants.data5[3]  = "5| Max succes loot pour metier";
  app.goConstants.data5[5]  = "5| questID";
  app.goConstants.data5[6]  = "5| cooldown (en seconde)";
  app.goConstants.data5[8]  = "5| large (0/1)";
  app.goConstants.data5[9]  = "5| non utilisable sur monture";
  app.goConstants.data5[10] = "5| consumable (despawn ou non)";
  app.goConstants.data5[11] = "5| SpawnMap";
  app.goConstants.data5[12] = "5| autoClose";
  app.goConstants.data5[15] = "5| transportPhysics";
  app.goConstants.data5[18] = "5| casterTargetSpellTargets (0/1)";
  app.goConstants.data5[22] = "5| conditionID1";
  app.goConstants.data5[24] = "5| noDamageImmune (0/1)";
  app.goConstants.data5[26] = "5| playerCast";
  app.goConstants.data5[29] = "5| winEventID2";
  app.goConstants.data5[30] = "5| conditionID2";
  app.goConstants.data5[31] = "5| HeroicAttachment";
  app.goConstants.data5[33] = "5| damagedNumHits";
  app.goConstants.data5[36] = "5| Gigantic AOI (0/1)";
  app.goConstants.data5[42] = "5| Capture Broadcast (Horde): BroadcastText";

  app.goConstants.data6     = [];
  app.goConstants.data6[0]  = "6| Ignored by pathfinding";
  app.goConstants.data6[1]  = "6| openTextID";
  app.goConstants.data6[2]  = "6| broadcast_text ID";
  app.goConstants.data6[3]  = "6| event_scripts.ID";
  app.goConstants.data6[5]  = "6| conditionID1";
  app.goConstants.data6[8]  = "6| floatingTooltip (0/1)";
  app.goConstants.data6[10] = "6| cooldown / temp d'ouverture";
  app.goConstants.data6[11] = "6| Timeto3rdfloor";
  app.goConstants.data6[12] = "6| openTextID";
  app.goConstants.data6[15] = "6| SpawnMap";
  app.goConstants.data6[18] = "6| castersGrouped  (0/1)";
  app.goConstants.data6[22] = "6| playerCast";
  app.goConstants.data6[24] = "6| openTextID";
  app.goConstants.data6[26] = "6| ExpireDuration";
  app.goConstants.data6[29] = "6| contestedEventID1";
  app.goConstants.data6[30] = "6| serverOnly (-1)";
  app.goConstants.data6[31] = "6| ChallengeAttachment";
  app.goConstants.data6[33] = "6| empty3";
  app.goConstants.data6[36] = "6| Infinite AOI (0/1)";
  app.goConstants.data6[42] = "6| Defended Broadcast (Horde): BroadcastText";

  app.goConstants.data7     = [];
  app.goConstants.data7[0]  = "7| Conditionid1";  
  app.goConstants.data7[1]  = "7| closeTextID";
  app.goConstants.data7[2]  = "7| losOK (0/1)";
  app.goConstants.data7[3]  = "7| linkedTrap (Spawned GO Ty6)";
  app.goConstants.data7[5]  = "7| LargeAOI";
  app.goConstants.data7[6]  = "7| startDelay (en seconde)";
  app.goConstants.data7[8]  = "7| floatOnWater";
  app.goConstants.data7[10] = "7| pageID (page_text.entry)";
  app.goConstants.data7[11] = "7| Reached3rdfloor";
  app.goConstants.data7[12] = "7| closeTextID";
  app.goConstants.data7[15] = "7| worldState1";
  app.goConstants.data7[18] = "7| ritualNoTargetCheck";
  app.goConstants.data7[22] = "7| Non utilisable sur monture";
  app.goConstants.data7[24] = "7| losOK (0/1)";
  app.goConstants.data7[26] = "7| GiganticAOI";
  app.goConstants.data7[29] = "7| contestedEventID2";
  app.goConstants.data7[31] = "7| DifficultyAnimations";
  app.goConstants.data7[33] = "7| empty4";
  app.goConstants.data7[36] = "7| Expire Duration :0-3600000, Default 10000";
  app.goConstants.data7[42] = "7| Assault Broadcast (Alliance): BroadcastText";

  app.goConstants.data8     = [];
  app.goConstants.data8[0]  = "8| Porte opaque"; 
  app.goConstants.data8[1]  = "8| losOK (0/1)";
  app.goConstants.data8[2]  = "8| Monture permise (0/1)";
  app.goConstants.data8[3]  = "8| questID";
  app.goConstants.data8[5]  = "8| UseGarrisonOwnerGuildColors";
  app.goConstants.data8[6]  = "8| serverOnly (Toujour 0)";
  app.goConstants.data8[8]  = "8| conditionID1";
  app.goConstants.data8[10] = "8| language (Languages.dbc)";
  app.goConstants.data8[11] = "8| Timeto4rdfloor";
  app.goConstants.data8[15] = "8| allowstopping";
  app.goConstants.data8[18] = "8| conditionID1";
  app.goConstants.data8[24] = "8| conditionID1";
  app.goConstants.data8[26] = "8| InfiniteAOI";
  app.goConstants.data8[29] = "8| progressEventID1";
  app.goConstants.data8[31] = "8| LargeAOI";
  app.goConstants.data8[33] = "8| empty5";
  app.goConstants.data8[36] = "8| Respawn Time 0-3600000, Default:20000";
  app.goConstants.data8[42] = "8| Capture Broadcast (Alliance): BroadcastText";

  app.goConstants.data9     = [];
  app.goConstants.data9[0]  = "9| Gigantic AOI";
  app.goConstants.data9[2]  = "9| large (0/1)";
  app.goConstants.data9[3]  = "9| level pour ouvrir";
  app.goConstants.data9[6]  = "9| stealthed (0/1)";
  app.goConstants.data9[10] = "pageMaterial (1 parchemin,2 pierre,3 marbre,4 argent,5 bronze)";
  app.goConstants.data9[11] = "9| Reached4rdfloor";
  app.goConstants.data9[15] = "9| InitStopped";
  app.goConstants.data9[24] = "9| playerCast";
  app.goConstants.data9[26] = "9| cooldown";
  app.goConstants.data9[29] = "9| progressEventID2";
  app.goConstants.data9[31] = "9| GiganticAOI";
  app.goConstants.data9[33] = "9| damagedEvent";
  app.goConstants.data9[36] = "9| Flag Drop : GameObjects";
  app.goConstants.data9[42] = "9| Defended Broadcast (Alliance): BroadcastText";

  app.goConstants.data10     = [];
  app.goConstants.data10[0]  = "10| Infinite AOI";
  app.goConstants.data10[2]  = "10| Conditionid1";
  app.goConstants.data10[3]  = "10| losOK (0/1)";
  app.goConstants.data10[6]  = "10| large (0/1)";
  app.goConstants.data10[10] = "10| spell (Spell.dbc)";
  app.goConstants.data10[11] = "10| Timeto5rdfloor";
  app.goConstants.data10[15] = "10| TrueInfiniteAOI";
  app.goConstants.data10[24] = "10| GiganticAOI";
  app.goConstants.data10[29] = "10| neutralEventID1";
  app.goConstants.data10[31] = "10| Legacy";
  app.goConstants.data10[33] = "10| destroyedDisplayId";
  app.goConstants.data10[36] = "10| Exclusive Category -2147483648 a 2147483647 , defaut 0";
  app.goConstants.data10[42] = "10| worldState1";

  app.goConstants.data11     = [];
  app.goConstants.data11[2]  = "11| Non utilisable sur monture";
  app.goConstants.data11[3]  = "11| leaveLoot (0/1)";
  app.goConstants.data11[6]  = "11| furtif affecté (0/1)";
  app.goConstants.data11[10] = "11| noDamageImmune (0/1)";
  app.goConstants.data11[11] = "11| Reached5rdfloor";
  app.goConstants.data11[24] = "11| InfiniteAOI";
  app.goConstants.data11[29] = "11| neutralEventID2";
  app.goConstants.data11[33] = "11| empty7";
  app.goConstants.data11[36] = "11| worldState1";
  app.goConstants.data11[42] = "11| Contested Event (Horde): GameEvents";

  app.goConstants.data12     = [];
  app.goConstants.data12[3]  = "12| Hors Combat (0/1)";
  app.goConstants.data12[6]  = "12| openTextID";
  app.goConstants.data12[10] = "12| linkedTrap (Spawned GO Ty6)";
  app.goConstants.data12[11]  = "12| Timeto6rdfloor";
  app.goConstants.data12[24]  = "12| cooldown";
  app.goConstants.data12[29] = "12| neutralPercent";
  app.goConstants.data12[33] = "12| empty8";
  app.goConstants.data12[36] = "12| Return on Defender Interact (0/1)";
  app.goConstants.data12[42] = "12| Capture Event (Horde): GameEvents";

  app.goConstants.data13     = [];
  app.goConstants.data13[3]  = "13| log loot";
  app.goConstants.data13[6]  = "13| closeTextID";
  app.goConstants.data13[10] = "13| large (0/1)";
  app.goConstants.data13[33] = "13| Reached6rdfloor";
  app.goConstants.data13[29] = "13| worldstate3";
  app.goConstants.data13[33] = "13| empty9";
  app.goConstants.data13[42] = "13| Defended Event (Horde): GameEvents";

  app.goConstants.data14     = [];
  app.goConstants.data14[3]  = "14| openTextID";
  app.goConstants.data14[6]  = "14| IgnoreTotems";
  app.goConstants.data14[10] = "14| openTextID";
  app.goConstants.data14[11] = "14| Timeto7rdfloor";
  app.goConstants.data14[29] = "14| minSuperiority";
  app.goConstants.data14[33] = "14| destroyedEvent";
  app.goConstants.data14[42] = "14| Contested Event (Alliance): GameEvents";

  app.goConstants.data15     = [];
  app.goConstants.data15[3]  = "15| use group loot rules (0/1)";
  app.goConstants.data15[6] = "15| conditionID1"; 
  app.goConstants.data15[10] = "15| closeTextID";
  app.goConstants.data15[11] = "15| Reached7rdfloor"; 
  app.goConstants.data15[29] = "15| maxSuperiority";
  app.goConstants.data15[33] = "15| empty10";
  app.goConstants.data15[42] = "15| Capture Event (Alliance): GameEvents";

  app.goConstants.data16     = [];
  app.goConstants.data16[3]  = "16| floating tooltip";
  app.goConstants.data16[6]  = "16| playerCast";
  app.goConstants.data16[10] = "16| losOK(0/1)";
  app.goConstants.data16[11] = "16| Timeto8rdfloor";
  app.goConstants.data16[29] = "16| minTime (en seconde)";
  app.goConstants.data16[33] = "16| debuildingTimeSecs";
  app.goConstants.data16[42] = "16| Defended Event (Alliance): GameEvents";

  app.goConstants.data17     = [];
  app.goConstants.data17[3]  = "17| conditionid1";
  app.goConstants.data17[6]  = "17| SummonerTriggered";
  app.goConstants.data17[11]  = "17| Reached8rdfloor";
  app.goConstants.data17[29] = "17| maxTime (en seconde)";
  app.goConstants.data17[33] = "17| empty11";
  app.goConstants.data17[42] = "17| Spell Visual 1";

  app.goConstants.data18     = [];
  app.goConstants.data18[3]  = "18| xplevel";
  app.goConstants.data18[6]  = "18| requireLOS";
  app.goConstants.data18[11] = "18| Timeto9rdfloor";
  app.goConstants.data18[29] = "18| large (0/1)";
  app.goConstants.data18[33] = "18| destructibleData";
  app.goConstants.data18[42] = "18| Spell Visual 2";

  app.goConstants.data19   = [];
  app.goConstants.data19[3]  = "19| xpDifficulty";
  app.goConstants.data19[10] = "19| gossipID (sort s'il est utilise)";
  app.goConstants.data19[11] = "19| Reached9rdfloor";
  app.goConstants.data19[29] = "19| highlight";
  app.goConstants.data19[33] = "19| rebuildingEvent";
  app.goConstants.data19[42] = "19| Spell Visual 3";

  app.goConstants.data20   = [];
  app.goConstants.data20[3]  = "20| lootlevel";
  app.goConstants.data20[10] = "20| Autoriser Multi Interaction";
  app.goConstants.data20[11] = "20| Timeto10rdfloor";
  app.goConstants.data20[29] = "20| startingValue";
  app.goConstants.data20[33] = "20| empty12";
  app.goConstants.data20[42] = "20| Spell Visual 4";

  app.goConstants.data21   = [];
  app.goConstants.data21[3]  = "21| Group Xp";
  app.goConstants.data21[10] = "21| flotter sur l'eau";
  app.goConstants.data21[11] = "21| Reached10rdfloor";
  app.goConstants.data21[29] = "21| unidirectional";
  app.goConstants.data21[33] = "21| empty13";
  app.goConstants.data21[42] = "21| Spell Visual 5";

  app.goConstants.data22   = [];
  app.goConstants.data22[3]  = "22| Damage Immune";
  app.goConstants.data22[10] = "22| conditionID1";
  app.goConstants.data22[11] = "22| onlychargeheightcheck";
  app.goConstants.data22[29] = "22| killbonustime";
  app.goConstants.data22[33] = "22| damageEvent";

  app.goConstants.data23   = [];
  app.goConstants.data23[3]   = "23| trivialSkillLow";
  app.goConstants.data23[10]  = "23| playerCast";
  app.goConstants.data23[11]  = "23| onlychargetimecheck";
  app.goConstants.data23[29]  = "23| speedWorldState1";
  app.goConstants.data23[32]  = "23| empty14";
    
  app.goConstants.data24   = [];
  app.goConstants.data24[3]  = "24| trivialSkillHigh";
  app.goConstants.data24[10] = "24| SpawnVignette";
  app.goConstants.data24[29] = "24| speedWorldState2";
  
  app.goConstants.data25   = [];
  app.goConstants.data25[3]  = "25| Dungeon Encounter";
  app.goConstants.data25[10]  = "25| startOpen";
  app.goConstants.data25[29]  = "25| UncontestedTime";
  
  app.goConstants.data26   = [];
  app.goConstants.data26[3]  = "26| spell";
  app.goConstants.data26[10] = "26| DontPlayOpenAnim";
  app.goConstants.data26[29] = "26| FrequentHeartbeat";
  
  app.goConstants.data27   = [];
  app.goConstants.data27[3]  = "27| GiganticAOI";
  app.goConstants.data27[10]  = "27| IgnoreBoundingBox";

  app.goConstants.data28   = [];
  app.goConstants.data28[3]   = "28| LargeAOI";
  app.goConstants.data28[10]  = "28| NeverUsableWhileMounted";
  
  app.goConstants.data29   = [];
  app.goConstants.data29[3]  = "29| spawnVignette";
  app.goConstants.data29[10] = "29| SortFarZ";
  
  app.goConstants.data30   = [];
  app.goConstants.data30[3]  = "30| chest Personal Loot";
  app.goConstants.data30[10] = "30| SyncAnimationtoObjectLifetime";
  
  app.goConstants.data31   = [];
  app.goConstants.data31[3]  = "31| turn personal loot security off";
  app.goConstants.data31[10] = "31| NoFuzzyHit";
  
  app.goConstants.data32   = [];
  app.goConstants.data32[3]  = "32| Chest properties";

  /* creature family constants */
  app.modalConstants.family = [];
  app.modalConstants.family[1]  = "Loup  -  CREATURE_FAMILY_WOLF - (0.6 1.3 0.5) : 1/3_Mouvement_Contact_Basique , 2/3_Mouvement_Contact_Prudent";
  app.modalConstants.family[2]  = "Felin  -  CREATURE_FAMILY_CAT - (0.6 1.3 0.5) : Mouvement_Contact_Tournant_Aleatoire (invisible)";
  app.modalConstants.family[3]  = "Araignee  -  CREATURE_FAMILY_SPIDER - (0.4 1.0 0.5) : Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance , Doit avoir du mana , Toile_Araignee)";
  app.modalConstants.family[4]  = "Ours  -  CREATURE_FAMILY_BEAR - (0.6 1.3 0.5) : Mouvement_Contact_Basique, Instincts_de_survie (charge)";
  app.modalConstants.family[5]  = "Sanglier  -  CREATURE_FAMILY_BOAR - (0.6 1.3 0.5) : Mouvement_Contact_Charges_Multiples";
  app.modalConstants.family[6]  = "Crocodile  -  CREATURE_FAMILY_CROCOLISK - (0.4 0.8 0.6) : 1/3_Mouvement_Contact_Avance_Recule , 2/3_Mouvement_Contact_Basique, (Ecorce)";
  app.modalConstants.family[7]  = "Oiseau charogniar  -  CREATURE_FAMILY_CARRION_BIRD - (1.75 1.75 2.0) : Mouvement_Contact_Prudent_Volant";
  app.modalConstants.family[8]  = "Crabe  -  CREATURE_FAMILY_CRAB - (0.4 0.8 0.4) : Mouvement_Contact_Avance_Recule - 1/2 Mouvement_Contact_Basique";
  app.modalConstants.family[9]  = "Gorille  -  CREATURE_FAMILY_GORILLA - (0.6 1.3 0.5) : Mouvement_Contact_Bondissant, (Bondir_Guerrier)";
  app.modalConstants.family[10] = "Custom  -  CREATURE_FAMILY_HORSE_CUSTOM - (0.5 1.0 0.5) : ";
  app.modalConstants.family[11] = "Raptor  -  CREATURE_FAMILY_RAPTOR - (0.6 1.3 0.5) : 1/4_Mouvement_Contact_Avance_Recule  , 1/4_Mouvement_Contact_Tournant_Aleatoire, 2/4_Mouvement_Contact_Basique, (Cri_Incapacitant)";
  app.modalConstants.family[12] = "Grand trotteur  -  CREATURE_FAMILY_TALLSTRIDER - (0.5 1.0 0.5) : 1/2 Mouvement_Contact_Avance_Recule , 1/2 Mouvement_Contact_Basique";
  app.modalConstants.family[15] = "Chasseur corrompu (Felhunter)  -  CREATURE_FAMILY_FELHUNTER - AI : 1/2 Mouvement_Contact_Basique , 1/2 Mouvement_Contact_Tournant_Aleatoire";
  app.modalConstants.family[16] = "Marcheur du vide (Voidwalker)  -  CREATURE_FAMILY_VOIDWALKER - AI : Mouvement_Contact_Basique";
  app.modalConstants.family[17] = "Succube  -  CREATURE_FAMILY_SUCCUBUS - (0.75 1.0 0.8): 1/2_Mouvement_Contact_Tournant_Aleatoire - 1/2_Mouvement_Contact_Basique";
  app.modalConstants.family[19] = "Garde funeste (Doomguard)  -  CREATURE_FAMILY_DOOMGUARD - (0.75 1.0 0.8): Mouvement_Contact_Charges_Multiples, (Bondir_Guerrier)";
  app.modalConstants.family[20] = "Scorpion  -  CREATURE_FAMILY_SCORPID - (0.5 1.2 0.5) : Mouvement_Contact_Avance_Recule";
  app.modalConstants.family[21] = "Tortue  -  CREATURE_FAMILY_TURTLE - (0.4 0.8 0.4) : Mouvement_Contact_Basique";
  app.modalConstants.family[23] = "Imp  -  CREATURE_FAMILY_IMP - (0.5 1.0 0.5) : Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance )";
  app.modalConstants.family[24] = "Chauve souris  -  CREATURE_FAMILY_BAT - (1.4 1.6 1.4) : Mouvement_Contact_Prudent_Volant";
  app.modalConstants.family[25] = "Hyene  -  CREATURE_FAMILY_HYENA - (0.5 1.0 0.5) : Mouvement_Contact_Prudent";
  app.modalConstants.family[26] = "Oiseau de proie  -  CREATURE_FAMILY_BIRD_OF_PREY - (1.75 1.75 2.0) : Mouvement_Contact_Basique, (Griffure_bondissante)";
  app.modalConstants.family[27] = "Serpent des vents  -  CREATURE_FAMILY_WIND_SERPENT - (0.75 1.0 0.8) : Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance, (Bouclier_De_Foudre) )";
  app.modalConstants.family[28] = "Telecommande  -  CREATURE_FAMILY_REMOTE_CONTROL - AI : ";
  app.modalConstants.family[29] = "Felguard  -  CREATURE_FAMILY_FELGUARD - AI : Mouvement_Contact_Basique, (Poursuite)";
  app.modalConstants.family[30] = "Faucon dragon  -  CREATURE_FAMILY_DRAGONHAWK - (1.4 1.6 1.4) : Mouvement_Caster";
  app.modalConstants.family[31] = "Ravageur  -  CREATURE_FAMILY_RAVAGER - (0.6 1.3 0.5) : 1/4_Mouvement_Contact_Prudent , 1/4_Mouvement_Contact_Avance_Recule , 1/4_Mouvement_Contact_Tournant_Aleatoire , 1/4_Mouvement_Contact_Charges_Multiples, (Poursuite, Griffure_bondissante) ";
  app.modalConstants.family[32] = "Traqueur de chaine  -  CREATURE_FAMILY_WARP_STALKER - AI : ";
  app.modalConstants.family[33] = "Sporebat  -  CREATURE_FAMILY_SPOREBAT - AI : ";
  app.modalConstants.family[34] = "Raie du Neant  -  CREATURE_FAMILY_NETHER_RAY - (0.75 1.0 0.8): 1/2_Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance ) , 1/2_Mouvement_Caster";
  app.modalConstants.family[35] = "Serpent  -  CREATURE_FAMILY_SERPENT - (0.4 1.0 0.5) : Mouvement_Contact_Basique";
  app.modalConstants.family[37] = "Papillon de nuit  -  CREATURE_FAMILY_MOTH - (1.4 1.6 1.4) :  Mouvement_Caster, Bouclier_De_Terre";
  app.modalConstants.family[38] = "Chimere  -  CREATURE_FAMILY_CHIMAERA - (1.4 1.6 1.4) : Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance, (Epines) )";
  app.modalConstants.family[39] = "Diablosaure  -  CREATURE_FAMILY_DEVILSAUR - AI : Mouvement_Contact_Basique";
  app.modalConstants.family[40] = "Goule  -  CREATURE_FAMILY_GHOUL - (0.4 1.0 0.5) : Mouvement_Contact_Basique (vit 0.4-1-0.5)";
  app.modalConstants.family[41] = "Silithide  -  CREATURE_FAMILY_SILITHID - AI : ";
  app.modalConstants.family[42] = "Ver  -  CREATURE_FAMILY_WORM - (0.4 1.0 0.5) : Mouvement_Caster_Puis_Contact ( spell [2]=spell a distance  - (UPDATE `creature_template_addon` SET `auras` = '58984 164339' WHERE `creature_template_addon`.`entry` = 8603)";
  app.modalConstants.family[43] = "Rhinoceros/Kodo  -  CREATURE_FAMILY_RHINO (0.6 1.3 0.5) - AI : 1/3_Mouvement_Contact_Basique , 2/3_Mouvement_Contact_Charges_Multiples, %Charge_Repousse";
  app.modalConstants.family[44] = "Guepe  -  CREATURE_FAMILY_WASP - (0.75 1.0 0.8): Mouvement_Contact_Prudent_Volant";
  app.modalConstants.family[45] = "Chien de base  -  CREATURE_FAMILY_CORE_HOUND - AI : 1/3_Mouvement_Contact_Prudent , 2/3_Mouvement_Contact_Charges_Multiples";
  app.modalConstants.family[46] = "Esprit de bete  -  CREATURE_FAMILY_SPIRIT_BEAST - AI : ";
  app.modalConstants.family[49] = "Elementaire d'eau  -  CREATURE_FAMILY_WATER_ELEMENTAL - (0.8 1.0 0.7) : 1/3_Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance RAPIDE) , 1/3_Mouvement_Caster , 1/3_Mouvement_Contact_Basique";
  app.modalConstants.family[50] = "Renard  -  CREATURE_FAMILY_FOX - (0.5 1.0 0.5) : 1/3_Mouvement_Contact_Basique , 2/3_Mouvement_Contact_Prudent";
app.modalConstants.family[51] = "Singe  -  CREATURE_FAMILY_MONKEY - (0.4 1.0 0.5) : Mouvement_Caster";
app.modalConstants.family[52] = "Chien  -  CREATURE_FAMILY_DOG - (0.5 1.0 0.5) : 4/5_Mouvement_Contact_Prudent , 1/5_Mouvement_Contact_Basique";
app.modalConstants.family[53] = "Hanneton  -  CREATURE_FAMILY_BEETLE - (0.5 1.0 0.5) : ";
app.modalConstants.family[55] = "Araignee de schiste  -  CREATURE_FAMILY_SHALE_SPIDER - AI : 1/2_Mouvement_Contact_Avance_Recule , 1/2_Mouvement_Contact_Tournant_Aleatoire, %Armure_De_La_Fournaise %Bondir_Guerrier";
app.modalConstants.family[56] = "Zombie  -  CREATURE_FAMILY_ZOMBIE - (0.4 1.0 0.5) : Mouvement_Contact_Basique";
app.modalConstants.family[57] = "A TEST FAMILY   -  CREATURE_FAMILY_BEETLE_OLD - AI : ";
app.modalConstants.family[59] = "CREATURE_FAMILY_SILITHID2 - AI : ";
app.modalConstants.family[66] = "CREATURE_FAMILY_WASP2 - AI : ";
app.modalConstants.family[68] = "Hydre  -  CREATURE_FAMILY_HYDRA - (0.6 1.3 0.5) :  1/2_Mouvement_Contact_Basique , 1/2_Mouvement_Contact_Avance_Recule, %Poison_Tir_Catapulte";
app.modalConstants.family[100] = "Diablotin gangrene  -  CREATURE_FAMILY_FELIMP - AI : 1/2_Mouvement_Contact_Avance_Recule , 1/2_Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance )";
app.modalConstants.family[101] = "Seigneur du Vide  -  CREATURE_FAMILY_VOIDLORD - (0.75 1.0 0.8): ";
app.modalConstants.family[102] = "Shivarra  -  CREATURE_FAMILY_SHIVARA - AI : 1/2 Mouvement_Contact_Basique , 1/2 Mouvement_Contact_Tournant_Aleatoire";
app.modalConstants.family[103] = "Observateur  -  CREATURE_FAMILY_OBSERVER - AI : ";
app.modalConstants.family[104] = "Garde-courroux  -  CREATURE_FAMILY_WRATHGUARD - AI : 1/2_Mouvement_Contact_Basique , 1/2_Mouvement_Contact_Charges_Multiples, %Poursuite";
app.modalConstants.family[108] = "Infernal  -  CREATURE_FAMILY_INFERNAL - (0.6 1.3 0.5) : Mouvement_Contact_Basique";
app.modalConstants.family[116] = "elementaire de feu  -  CREATURE_FAMILY_FIREELEMENTAL - (0.75 1.5) : 1/4_Mouvement_Caster_Puis_Contact ( spell [2] = spell a distance ) , 1/4_Mouvement_Caster , 2/4_Mouvement_Contact_Basique, %Armure_De_La_Fournaise";
app.modalConstants.family[117] = "elementaire de terre  -  CREATURE_FAMILY_EARTHELEMENTAL - (0.7 1.0 0.6) : Mouvement_Contact_Basique, %Ecorce %Lancer_Une_Pierre";
app.modalConstants.family[125] = "Grue  -  CREATURE_FAMILY_CRANE  - (0.4 1.0 0.5) : ";
app.modalConstants.family[126] = "Trotteur aquatique  -  CREATURE_FAMILY_WATERSTRIDER - (0.5 1.0 0.5) : ";
app.modalConstants.family[127] = "Porc-epic  -  CREATURE_FAMILY_PORCUPINE - - (0.5 1.2 0.5) : ";
app.modalConstants.family[128] = "Quilen  -  CREATURE_FAMILY_QUILEN - AI : ";
app.modalConstants.family[129] = "Chevre  -  CREATURE_FAMILY_GOAT - - (0.5 1.2 0.5) : ";
app.modalConstants.family[130] = "Basilic  -  CREATURE_FAMILY_BASILISK - (0.4 1.0 0.5) : 1/2 Mouvement_Contact_Avance_Recule , Mouvement_Contact_Basique , %Ecorce";
app.modalConstants.family[138] = "Navrecorne  -  CREATURE_FAMILY_DIREHORN - AI : ";
app.modalConstants.family[145] = "elem. de tempete  -  CREATURE_FAMILY_STORMELEMENTAL - (0.75 1.5) : ";
app.modalConstants.family[146] = "MT - Water Elemental  -  CREATURE_FAMILY_MTWATERELEMENTAL - (0.8 1.0 0.7) : ";
app.modalConstants.family[147] = "Garde de terreur  -  CREATURE_FAMILY_TORRORGUARD - AI : ";
app.modalConstants.family[148] = "Abyssal  -  CREATURE_FAMILY_ABYSSAL - AI : ";
app.modalConstants.family[149] = "Rylak  -  CREATURE_FAMILY_RYLAK - - (0.5 1.2 0.5) : ";
app.modalConstants.family[150] = "Potamodonte  -  CREATURE_FAMILY_RIVERBEAST - (0.75 1.2 1.25) : ";
app.modalConstants.family[151] = "Cerf  -  CREATURE_FAMILY_STAG - - (0.5 1.2 0.5) : ";
app.modalConstants.family[152] = "Murloc 1.2 - CREATURE_FAMILY_MURLOC - (0.5 1.0 0.8) : 1/4_Mouvement_Contact_Prudent, 1/4_Mouvement_Contact_Avance_Recule, 2/4_Mouvement_Contact_Basique";
app.modalConstants.family[153] = "Naga 2.0 - CREATURE_FAMILY_NAGA - - (0.5 1.0 0.8) : 1/3_Mouvement_Contact_Basique, 1/3_Mouvement_Contact_Tournant_Aleatoire, 1/3_Mouvement_Contact_Prudent_Volant";
app.modalConstants.family[155] = "CREATURE_FAMILY_SENTERRE uniquement si fixe - (0.25 1.0 0.4) : Mouvement_Contact_Basique --- UPDATE `creature_template_addon` SET `auras` = '58984 164339' WHERE `creature_template_addon`.`entry` = XXXXX; ";
app.modalConstants.family[156] = "CREATURE_FAMILY_SE_DETERRE_AU_CONTACT - (0.5 0.9 0.5) : 1/3_Mouvement_Contact_Basique, 1/3_Mouvement_Contact_Tournant_Aleatoire, 1/3_Mouvement_Contact_Avance_Recule --- Sans fumee : UPDATE `creature_template_addon` SET `auras` = '58984 164339' WHERE `creature_template_addon`.`entry` = 43851;)";
app.modalConstants.family[157] = "Rocher (elementaire de terre si fixe)  -  CREATURE_FAMILY_MORPH_ROCHER - (0.7 1.0 0.6) : Mouvement_Contact_Basique";
app.modalConstants.family[158] = "CREATURE_FAMILY_CREATURE_AQUATIQUE - (0.5  1.1  0.5) : 1/4_Mouvement_Contact_Prudent, 1/4_Mouvement_Contact_Prudent_Volant, 2/4_Mouvement_Contact_Basique";
app.modalConstants.family[159] = "CREATURE_FAMILY_SENTERRE_FIXE (ROOT) - (0.1 0.1 0.1) : Mouvement_Fixe - (Asticot 16030)";



  /* faction constants */
   app.modalConstants.faction = [];
  app.modalConstants.faction[1]    = "PLAYER, Humain";
  app.modalConstants.faction[2]    = "PLAYER, Orc";
  app.modalConstants.faction[3]    = "PLAYER, Nain";
  app.modalConstants.faction[4]    = "PLAYER, Elfe de la nuit";
  app.modalConstants.faction[5]    = "PLAYER, Mort-vivant";
  app.modalConstants.faction[6]    = "PLAYER, Tauren ";
  app.modalConstants.faction[7]    = "Creature hostile";
  app.modalConstants.faction[10]   = "Escortee";
  app.modalConstants.faction[11]   = "Hurlevent ";
  app.modalConstants.faction[12]   = "Hurlevent";
  app.modalConstants.faction[14]   = "Monstre";
  app.modalConstants.faction[15]   = "Creature";
  app.modalConstants.faction[16]   = "Monster";
  app.modalConstants.faction[17]   = "Defias Brotherhood";
  app.modalConstants.faction[18]   = "Murloc";
  app.modalConstants.faction[19]   = "Gnoll - Redridge";
  app.modalConstants.faction[20]   = "Gnoll - Riverpaw";
  app.modalConstants.faction[21]   = "Mort-vivant, Scourge";
  app.modalConstants.faction[22]   = "Bete - Araignee";
  app.modalConstants.faction[23]   = "Gnomeregan Exiles";
  app.modalConstants.faction[24]   = "Worgen";
  app.modalConstants.faction[25]   = "Kobold";
  app.modalConstants.faction[26]   = "Kobold";
  app.modalConstants.faction[27]   = "Defias Brotherhood";
  app.modalConstants.faction[28]   = "Troll, Bloodscalp";
  app.modalConstants.faction[29]   = "Orgrimmar";
  app.modalConstants.faction[30]   = "Troll, Skullsplitter";
  app.modalConstants.faction[31]   = "Proie";
  app.modalConstants.faction[32]   = "Bete - Wolf";
  app.modalConstants.faction[33]   = "Escortee";
  app.modalConstants.faction[34]   = "Defias Brotherhood";
  app.modalConstants.faction[35]   = "Amical";
  app.modalConstants.faction[36]   = "Trogg";
  app.modalConstants.faction[37]   = "Troll, Frostmane";
  app.modalConstants.faction[38]   = "Bete - Loup";
  app.modalConstants.faction[39]   = "Gnoll-Shadowhide";
  app.modalConstants.faction[40]   = "Orc,Blackrock";
  app.modalConstants.faction[41]   = "mechant";
  app.modalConstants.faction[42]   = "Victime";
  app.modalConstants.faction[43]   = "mechant";
  app.modalConstants.faction[44]   = "Bete-Ours";
  app.modalConstants.faction[45]   = "Ogre";
  app.modalConstants.faction[46]   = "Kurzen's Mercenaire";
  app.modalConstants.faction[47]   = "Venture Company";
  app.modalConstants.faction[48]   = "Bete-Raptor";
  app.modalConstants.faction[49]   = "Basilisk";
  app.modalConstants.faction[50]   = "Dragonflight,Green";
  app.modalConstants.faction[51]   = "Les disparus";
  app.modalConstants.faction[52]   = "Gizlock's Dummy";
  app.modalConstants.faction[53]   = "Humain,Night Watch";
  app.modalConstants.faction[54]   = "Dark Iron Nains";
  app.modalConstants.faction[55]   = "Ironforge";
  app.modalConstants.faction[56]   = "Humain,Night Watch";
  app.modalConstants.faction[57]   = "Ironforge";
  app.modalConstants.faction[58]   = "Creature";
  app.modalConstants.faction[59]   = "Trogg";
  app.modalConstants.faction[60]   = "Dragonflight,Red";
  app.modalConstants.faction[61]   = "Gnoll-Mosshide";
  app.modalConstants.faction[62]   = "Orc,Dragonmaw";
  app.modalConstants.faction[63]   = "Gnome-Leper";
  app.modalConstants.faction[64]   = "Gnomeregan Exiles";
  app.modalConstants.faction[65]   = "Orgrimmar";
  app.modalConstants.faction[66]   = "Leopard";
  app.modalConstants.faction[67]   = "Scarlet Crusade";
  app.modalConstants.faction[68]   = "Undercity";
  app.modalConstants.faction[69]   = "Ratchet";
  app.modalConstants.faction[70]   = "Gnoll-Rothide";
  app.modalConstants.faction[71]   = "Fossoyeuse";
  app.modalConstants.faction[72]   = "Bete-Gorille";
  app.modalConstants.faction[73]   = "Bete-Oiseau charognard";
  app.modalConstants.faction[74]   = "Naga";
  app.modalConstants.faction[76]   = "Dalaran";
  app.modalConstants.faction[77]   = "Forlorn Spirit";
  app.modalConstants.faction[78]   = "Darkhowl";
  app.modalConstants.faction[79]   = "Darnassus";
  app.modalConstants.faction[80]   = "Darnassus";
  app.modalConstants.faction[81]   = "Grell";
  app.modalConstants.faction[82]   = "Furbolg";
  app.modalConstants.faction[83]   = "Horde Generique";
  app.modalConstants.faction[84]   = "Alliance Generique";
  app.modalConstants.faction[85]   = "Orgrimmar";
  app.modalConstants.faction[86]   = "Gizlock's Charm";
  app.modalConstants.faction[87]   = "Syndicate";
  app.modalConstants.faction[88]   = "Hillsbrad Milice";
  app.modalConstants.faction[89]   = "Scarlet Crusade";
  app.modalConstants.faction[90]   = "Demon";
  app.modalConstants.faction[91]   = "Elementaire";
  app.modalConstants.faction[92]   = "Esprit";
  app.modalConstants.faction[93]   = "Monstre";
  app.modalConstants.faction[94]   = "Tresor";
  app.modalConstants.faction[95]   = "Gnoll-Mudsnout";
  app.modalConstants.faction[96]   = "HIllsbrad,Southshore Mayor";
  app.modalConstants.faction[97]   = "Syndicate";
  app.modalConstants.faction[98]   = "Fossoyeuse";
  app.modalConstants.faction[99]   = "Victime";
  app.modalConstants.faction[100]  = "Tresor";
  app.modalConstants.faction[101]  = "Tresor";
  app.modalConstants.faction[102]  = "Tresor";
  app.modalConstants.faction[103]  = "Dragonflight,Black";
  app.modalConstants.faction[104]  = "Thunder Bluff";
  app.modalConstants.faction[105]  = "Thunder Bluff *";
  app.modalConstants.faction[106]  = "Horde Generique";
  app.modalConstants.faction[107]  = "Troll,Frostmane";
  app.modalConstants.faction[108]  = "Syndicat";
  app.modalConstants.faction[109]  = "Quilboar,Razormane 2";
  app.modalConstants.faction[110]  = "Quilboar,Razormane 2";
  app.modalConstants.faction[111]  = "Quilboar,Bristleback";
  app.modalConstants.faction[112]  = "Quilboar,Bristleback";
  app.modalConstants.faction[113]  = "Escortee ***";
  app.modalConstants.faction[114]  = "Tresor";
  app.modalConstants.faction[115]  = "PLAYER,Gnome";
  app.modalConstants.faction[116]  = "PLAYER,Troll";
  app.modalConstants.faction[118]  = "Fossoyeuse";
  app.modalConstants.faction[119]  = "Bloodsail Boucaniers";
  app.modalConstants.faction[120]  = "Booty Bay";
  app.modalConstants.faction[121]  = "Booty Bay";
  app.modalConstants.faction[122]  = "Ironforge";
  app.modalConstants.faction[123]  = "Hurlevent";
  app.modalConstants.faction[124]  = "Darnassus";
  app.modalConstants.faction[125]  = "Orgrimmar";
  app.modalConstants.faction[126]  = "Sombrelance Trolls";
  app.modalConstants.faction[127]  = "mechant";
  app.modalConstants.faction[128]  = "Blackfathom";
  app.modalConstants.faction[129]  = "Makrura";
  app.modalConstants.faction[130]  = "Centaure,Kolkar";
  app.modalConstants.faction[131]  = "Centaure,Galak";
  app.modalConstants.faction[132]  = "Gelkis Clan Centaure";
  app.modalConstants.faction[133]  = "Magram Clan Centaure";
  app.modalConstants.faction[134]  = "Maraudine";
  app.modalConstants.faction[148]  = "Monstre";
  app.modalConstants.faction[149]  = "Theramore";
  app.modalConstants.faction[150]  = "Theramore";
  app.modalConstants.faction[151]  = "Theramore";
  app.modalConstants.faction[152]  = "Quilboar,Razorfen";
  app.modalConstants.faction[153]  = "Quilboar,Razorfen";
  app.modalConstants.faction[154]  = "Quilboar,Deathshead";
  app.modalConstants.faction[168]  = "Ennemi";
  app.modalConstants.faction[188]  = "Ambient";
  app.modalConstants.faction[189]  = "Creature";
  app.modalConstants.faction[190]  = "Ambient";
  app.modalConstants.faction[208]  = "Nethergarde Caravane";
  app.modalConstants.faction[209]  = "Nethergarde Caravane";
  app.modalConstants.faction[210]  = "Alliance Generique";
  app.modalConstants.faction[230]  = "des mers du Sud Freebooters";
  app.modalConstants.faction[231]  = "Escortee";
  app.modalConstants.faction[232]  = "Escortee";
  app.modalConstants.faction[233]  = "Mort-vivant,Scourge";
  app.modalConstants.faction[250]  = "Escortee";
  app.modalConstants.faction[270]  = "Wailing Caverns";
  app.modalConstants.faction[290]  = "Escortee";
  app.modalConstants.faction[310]  = "Silithid";
  app.modalConstants.faction[311]  = "Silithid";
  app.modalConstants.faction[312]  = "Bete-Araignee";
  app.modalConstants.faction[330]  = "Wailing Caverns";
  app.modalConstants.faction[350]  = "Blackfathom";
  app.modalConstants.faction[370]  = "Armies of C'Thun";
  app.modalConstants.faction[371]  = "Silvermoon Remnant";
  app.modalConstants.faction[390]  = "Booty Bay";
  app.modalConstants.faction[410]  = "Basilisk";
  app.modalConstants.faction[411]  = "Bete-Chauve souris";
  app.modalConstants.faction[412]  = "The Defilers";
  app.modalConstants.faction[413]  = "Scorpide";
  app.modalConstants.faction[414]  = "Timbermaw Hold";
  app.modalConstants.faction[415]  = "Titan";
  app.modalConstants.faction[416]  = "Titan";
  app.modalConstants.faction[430]  = "Taskmaster Fizzule";
  app.modalConstants.faction[450]  = "Wailing Caverns";
  app.modalConstants.faction[470]  = "Titan";
  app.modalConstants.faction[471]  = "Ravenholdt";
  app.modalConstants.faction[472]  = "Syndicat";
  app.modalConstants.faction[473]  = "Ravenholdt";
  app.modalConstants.faction[474]  = "Gadgetzan";
  app.modalConstants.faction[475]  = "Gadgetzan";
  app.modalConstants.faction[494]  = "Gnomeregan Bug";
  app.modalConstants.faction[495]  = "Escortee";
  app.modalConstants.faction[514]  = "Harpy";
  app.modalConstants.faction[534]  = "Alliance Generique";
  app.modalConstants.faction[554]  = "Burning Blade";
  app.modalConstants.faction[574]  = "Shadowsilk Braconnier";
  app.modalConstants.faction[575]  = "Searing Araignee";
  app.modalConstants.faction[594]  = "Trogg";
  app.modalConstants.faction[614]  = "Victime";
  app.modalConstants.faction[634]  = "Monstre";
  app.modalConstants.faction[635]  = "Cenarion Circle";
  app.modalConstants.faction[636]  = "Grumegueule Hold";
  app.modalConstants.faction[637]  = "Ratchet";
  app.modalConstants.faction[654]  = "Troll,Witherbark";
  app.modalConstants.faction[655]  = "Centaure,Kolkar";
  app.modalConstants.faction[674]  = "Dark Iron Nains";
  app.modalConstants.faction[694]  = "Alliance Generique";
  app.modalConstants.faction[695]  = "Hydraxian Waterlords";
  app.modalConstants.faction[714]  = "Horde Generique";
  app.modalConstants.faction[734]  = "Dark Iron Nains";
  app.modalConstants.faction[735]  = "Goblin,Dark Iron Bar Patron";
  app.modalConstants.faction[736]  = "Goblin,Dark Iron Bar Patron";
  app.modalConstants.faction[754]  = "Dark Iron Nains";
  app.modalConstants.faction[774]  = "Escortee";
  app.modalConstants.faction[775]  = "Escortee";
  app.modalConstants.faction[776]  = "Brood of Nozdormu";
  app.modalConstants.faction[777]  = "Might of Kalimdor";
  app.modalConstants.faction[778]  = "Geant";
  app.modalConstants.faction[794]  = "Aube d'argent";
  app.modalConstants.faction[795]  = "Troll,Vilebranch";
  app.modalConstants.faction[814]  = "Aube d'argent";
  app.modalConstants.faction[834]  = "Elementaire";
  app.modalConstants.faction[854]  = "Toujours regarder";
  app.modalConstants.faction[855]  = "Toujours regarder";
  app.modalConstants.faction[874]  = "sabre d'hiver Trainers";
  app.modalConstants.faction[875]  = "Gnomeregan Exiles";
  app.modalConstants.faction[876]  = "Sombrelance Trolls";
  app.modalConstants.faction[877]  = "Sombrelance Trolls";
  app.modalConstants.faction[894]  = "Theramore";
  app.modalConstants.faction[914]  = "Mannequin d'entraînement";
  app.modalConstants.faction[934]  = "Furbolg,Non corrompu";
  app.modalConstants.faction[954]  = "Demon";
  app.modalConstants.faction[974]  = "Mort-vivant,Scourge";
  app.modalConstants.faction[994]  = "Cenarion Circle";
  app.modalConstants.faction[995]  = "Thunder Bluff";
  app.modalConstants.faction[996]  = "Cenarion Circle";
  app.modalConstants.faction[1014] = "Shatterspear Trolls";
  app.modalConstants.faction[1015] = "Shatterspear Trolls";
  app.modalConstants.faction[1034] = "Horde Generique";
  app.modalConstants.faction[1054] = "Alliance Generique";
  app.modalConstants.faction[1055] = "Alliance Generique";
  app.modalConstants.faction[1074] = "Orgrimmar";
  app.modalConstants.faction[1075] = "Theramore";
  app.modalConstants.faction[1076] = "Darnassus";
  app.modalConstants.faction[1077] = "Theramore";
  app.modalConstants.faction[1078] = "Hurlevent";
  app.modalConstants.faction[1080] = "Amical";
  app.modalConstants.faction[1081] = "Elementaire";
  app.modalConstants.faction[1094] = "Bete-Sanglier";
  app.modalConstants.faction[1095] = "Mannequin d'entraînement";
  app.modalConstants.faction[1096] = "Theramore";
  app.modalConstants.faction[1097] = "Darnassus";
  app.modalConstants.faction[1114] = "Dragonflight,Black-Bait";
  app.modalConstants.faction[1134] = "Fossoyeuse";
  app.modalConstants.faction[1154] = "Fossoyeuse";
  app.modalConstants.faction[1174] = "Orgrimmar";
  app.modalConstants.faction[1194] = "Champ de bataille neutre";
  app.modalConstants.faction[1214] = "Clan loup-de-givre";
  app.modalConstants.faction[1215] = "Clan loup-de-givre";
  app.modalConstants.faction[1216] = "Garde foudrepique";
  app.modalConstants.faction[1217] = "Garde foudrepique";
  app.modalConstants.faction[1234] = "Sulfuron Firelords";
  app.modalConstants.faction[1235] = "Sulfuron Firelords";
  app.modalConstants.faction[1236] = "Seigneurs du Feu de Sulfuron";
  app.modalConstants.faction[1254] = "Cenarion Circle";
  app.modalConstants.faction[1274] = "Creature";
  app.modalConstants.faction[1275] = "Creature";
  app.modalConstants.faction[1294] = "Gizlock";
  app.modalConstants.faction[1314] = "Horde Generique";
  app.modalConstants.faction[1315] = "Alliance Generique";
  app.modalConstants.faction[1334] = "Garde foudrepique";
  app.modalConstants.faction[1335] = "Clan loup-de-givre";
  app.modalConstants.faction[1354] = "Shen'dralar";
  app.modalConstants.faction[1355] = "Shen'dralar";
  app.modalConstants.faction[1374] = "Ogre(Captain Kromcrush)";
  app.modalConstants.faction[1375] = "Tresor";
  app.modalConstants.faction[1394] = "Vol draconique,Black";
  app.modalConstants.faction[1395] = "Silithid Attackers";
  app.modalConstants.faction[1414] = "Esprit Guide-Alliance";
  app.modalConstants.faction[1415] = "Esprit Guide-Horde";
  app.modalConstants.faction[1434] = "Jaedenar";
  app.modalConstants.faction[1454] = "Victime";
  app.modalConstants.faction[1474] = "Thorium Brotherhood";
  app.modalConstants.faction[1475] = "Thorium Brotherhood";
  app.modalConstants.faction[1494] = "Horde Generique";
  app.modalConstants.faction[1495] = "Horde Generique";
  app.modalConstants.faction[1496] = "Horde Generique";
  app.modalConstants.faction[1514] = "Sentinelles Ailargent";
  app.modalConstants.faction[1515] = "Cavaliers chanteguerres";
  app.modalConstants.faction[1534] = "Garde foudrepique";
  app.modalConstants.faction[1554] = "Clan loup-de-givre";
  app.modalConstants.faction[1555] = "Darkmoon Faire";
  app.modalConstants.faction[1574] = "Tribu Zandalar";
  app.modalConstants.faction[1575] = "Hurlevent";
  app.modalConstants.faction[1576] = "Reste de Lune-d'argent";
  app.modalConstants.faction[1577] = "The League of Arathor";
  app.modalConstants.faction[1594] = "Darnassus";
  app.modalConstants.faction[1595] = "Orgrimmar";
  app.modalConstants.faction[1596] = "Garde foudrepique";
  app.modalConstants.faction[1597] = "Clan loup-de-givre";
  app.modalConstants.faction[1598] = "The Defilers";
  app.modalConstants.faction[1599] = "La Ligue d'Arathor";
  app.modalConstants.faction[1600] = "Darnassus";
  app.modalConstants.faction[1601] = "Brood of Nozdormu";
  app.modalConstants.faction[1602] = "Lune-d'argent City";
  app.modalConstants.faction[1603] = "Lune-d'argent City";
  app.modalConstants.faction[1604] = "Lune-d'argent City";
  app.modalConstants.faction[1605] = "Vol draconique,Bronze";
  app.modalConstants.faction[1606] = "Creature";
  app.modalConstants.faction[1607] = "Creature";
  app.modalConstants.faction[1608] = "Cenarion Circle";
  app.modalConstants.faction[1610] = "PLAYER,BloodElf";
  app.modalConstants.faction[1611] = "Ironforge";
  app.modalConstants.faction[1612] = "Orgrimmar";
  app.modalConstants.faction[1613] = "Puissance de Kalimdor";
  app.modalConstants.faction[1614] = "Monstre";
  app.modalConstants.faction[1615] = "Steamwheedle Cartel";
  app.modalConstants.faction[1616] = "RC Objects";
  app.modalConstants.faction[1617] = "RC Enemies";
  app.modalConstants.faction[1618] = "Ironforge";
  app.modalConstants.faction[1619] = "Orgrimmar";
  app.modalConstants.faction[1620] = "Ennemi";
  app.modalConstants.faction[1621] = "Blue";
  app.modalConstants.faction[1622] = "Red";
  app.modalConstants.faction[1623] = "Tranquillien";
  app.modalConstants.faction[1624] = "Aube d'argent";
  app.modalConstants.faction[1625] = "Aube d'argent";
  app.modalConstants.faction[1626] = "Mort-vivant,Scourge";
  app.modalConstants.faction[1627] = "Farstriders";
  app.modalConstants.faction[1628] = "Tranquillien";
  app.modalConstants.faction[1629] = "PLAYER,Draenei";
  app.modalConstants.faction[1630] = "Scourge Invaders";
  app.modalConstants.faction[1634] = "Scourge Invaders";
  app.modalConstants.faction[1635] = "Steamwheedle Cartel";
  app.modalConstants.faction[1636] = "Farstriders";
  app.modalConstants.faction[1637] = "Farstriders";
  app.modalConstants.faction[1638] = "Exodar";
  app.modalConstants.faction[1639] = "Exodar";
  app.modalConstants.faction[1640] = "Exodar";
  app.modalConstants.faction[1641] = "Cavaliers chanteguerres";
  app.modalConstants.faction[1642] = "Sentinelles Ailargent";
  app.modalConstants.faction[1643] = "Troll,Forest";
  app.modalConstants.faction[1644] = "The Sons of Lothar";
  app.modalConstants.faction[1645] = "The Sons of Lothar";
  app.modalConstants.faction[1646] = "Exodar";
  app.modalConstants.faction[1647] = "Exodar";
  app.modalConstants.faction[1648] = "The Sons of Lothar";
  app.modalConstants.faction[1649] = "The Sons of Lothar";
  app.modalConstants.faction[1650] = "TheMag'har";
  app.modalConstants.faction[1651] = "TheMag'har";
  app.modalConstants.faction[1652] = "TheMag'har";
  app.modalConstants.faction[1653] = "TheMag'har";
  app.modalConstants.faction[1654] = "Exodar";
  app.modalConstants.faction[1655] = "Exodar";
  app.modalConstants.faction[1656] = "Lune-d'argent City";
  app.modalConstants.faction[1657] = "Lune-d'argent City";
  app.modalConstants.faction[1658] = "Lune-d'argent City";
  app.modalConstants.faction[1659] = "Cenarion Expedition";
  app.modalConstants.faction[1660] = "Cenarion Expedition";
  app.modalConstants.faction[1661] = "Cenarion Expedition";
  app.modalConstants.faction[1662] = "Fel Orc";
  app.modalConstants.faction[1663] = "Fel Orc Ghost";
  app.modalConstants.faction[1664] = "Sons of Lothar Ghosts";
  app.modalConstants.faction[1665] = "None";
  app.modalConstants.faction[1666] = "Prise d'honneur";
  app.modalConstants.faction[1667] = "Prise d'honneur";
  app.modalConstants.faction[1668] = "Thrallmar";
  app.modalConstants.faction[1669] = "Thrallmar";
  app.modalConstants.faction[1670] = "Thrallmar";
  app.modalConstants.faction[1671] = "Prise d'honneur";
  app.modalConstants.faction[1672] = "Test Faction 1";
  app.modalConstants.faction[1673] = "To WoW-Flag";
  app.modalConstants.faction[1674] = "Test Faction 4";
  app.modalConstants.faction[1675] = "Test Faction 3";
  app.modalConstants.faction[1676] = "ToWoW-Flag Trigger Horde(DND)";
  app.modalConstants.faction[1677] = "ToWoW-Flag Trigger Alliance(DND)";
  app.modalConstants.faction[1678] = "Ethereum";
  app.modalConstants.faction[1679] = "Broken";
  app.modalConstants.faction[1680] = "Elementaire";
  app.modalConstants.faction[1681] = "Elementaire de Terre";
  app.modalConstants.faction[1682] = "Fighting Robots";
  app.modalConstants.faction[1683] = "Bon acteur";
  app.modalConstants.faction[1684] = "Acteur maléfique";
  app.modalConstants.faction[1685] = "Stillpine Furbolg";
  app.modalConstants.faction[1686] = "Stillpine Furbolg";
  app.modalConstants.faction[1687] = "Crazed Owlkin";
  app.modalConstants.faction[1688] = "Chess Alliance";
  app.modalConstants.faction[1689] = "Chess Horde";
  app.modalConstants.faction[1690] = "Chess Alliance";
  app.modalConstants.faction[1691] = "Chess Horde";
  app.modalConstants.faction[1692] = "Monstre Spar";
  app.modalConstants.faction[1693] = "Monstre Spar Buddy";
  app.modalConstants.faction[1694] = "Exodar";
  app.modalConstants.faction[1695] = "Lune-d'argent City";
  app.modalConstants.faction[1696] = "The Violet Eye";
  app.modalConstants.faction[1697] = "Fel Orc";
  app.modalConstants.faction[1698] = "Exodar";
  app.modalConstants.faction[1699] = "Exodar";
  app.modalConstants.faction[1700] = "Exodar";
  app.modalConstants.faction[1701] = "Sunhawks";
  app.modalConstants.faction[1702] = "Sunhawks";
  app.modalConstants.faction[1703] = "Mannequin d'entraînement";
  app.modalConstants.faction[1704] = "Fel Orc";
  app.modalConstants.faction[1705] = "Fel Orc";
  app.modalConstants.faction[1706] = "Fungal Geant";
  app.modalConstants.faction[1707] = "Sporeggar";
  app.modalConstants.faction[1708] = "Sporeggar";
  app.modalConstants.faction[1709] = "Sporeggar";
  app.modalConstants.faction[1710] = "Cenarion Expedition";
  app.modalConstants.faction[1711] = "Monstre,Predator";
  app.modalConstants.faction[1712] = "Monstre,Proie";
  app.modalConstants.faction[1713] = "Monstre,Proie";
  app.modalConstants.faction[1714] = "Sunhawks";
  app.modalConstants.faction[1715] = "VoidAnomaly";
  app.modalConstants.faction[1716] = "Hyjal Defenders";
  app.modalConstants.faction[1717] = "Hyjal Defenders";
  app.modalConstants.faction[1718] = "Hyjal Defenders";
  app.modalConstants.faction[1719] = "Hyjal Defenders";
  app.modalConstants.faction[1720] = "Hyjal Invaders";
  app.modalConstants.faction[1721] = "Kurenai";
  app.modalConstants.faction[1722] = "Kurenai";
  app.modalConstants.faction[1723] = "Kurenai";
  app.modalConstants.faction[1724] = "Kurenai";
  app.modalConstants.faction[1725] = "Earthen Ring";
  app.modalConstants.faction[1726] = "Earthen Ring";
  app.modalConstants.faction[1727] = "Earthen Ring";
  app.modalConstants.faction[1728] = "Cenarion Expedition";
  app.modalConstants.faction[1729] = "Thrallmar";
  app.modalConstants.faction[1730] = "Le Consortium";
  app.modalConstants.faction[1731] = "Le Consortium";
  app.modalConstants.faction[1732] = "Alliance Generique";
  app.modalConstants.faction[1733] = "Alliance Generique";
  app.modalConstants.faction[1734] = "Horde Generique";
  app.modalConstants.faction[1735] = "Horde Generique";
  app.modalConstants.faction[1736] = "Monstre Spar Buddy";
  app.modalConstants.faction[1737] = "Prise d'honneur";
  app.modalConstants.faction[1738] = "Arakkoa";
  app.modalConstants.faction[1739] = "Bannière du marais de Zangar(Alliance)";
  app.modalConstants.faction[1740] = "Bannière du marais de Zangar(Horde)";
  app.modalConstants.faction[1741] = "The Sha'tar";
  app.modalConstants.faction[1742] = "Bannière du marais de Zangar(Neutral)";
  app.modalConstants.faction[1743] = "The Aldor";
  app.modalConstants.faction[1744] = "The Scryers";
  app.modalConstants.faction[1745] = "Lune-d'argent City";
  app.modalConstants.faction[1746] = "The Scryers";
  app.modalConstants.faction[1747] = "Cavernes du temps-Thrall";
  app.modalConstants.faction[1748] = "Cavernes du temps-Durnholde";
  app.modalConstants.faction[1749] = "Cavernes du temps-Southshore Guards";
  app.modalConstants.faction[1750] = "Shadow Council Covert";
  app.modalConstants.faction[1751] = "Monstre";
  app.modalConstants.faction[1752] = "Portail sombre Attaquant,Legion";
  app.modalConstants.faction[1753] = "Portail sombre Attaquant,Legion";
  app.modalConstants.faction[1754] = "Portail sombre Attaquant,Legion";
  app.modalConstants.faction[1755] = "Portail sombre Defenseur,Alliance";
  app.modalConstants.faction[1756] = "Portail sombre Defenseur,Alliance";
  app.modalConstants.faction[1757] = "Portail sombre Defenseur,Alliance";
  app.modalConstants.faction[1758] = "Portail sombre Defenseur,Horde";
  app.modalConstants.faction[1759] = "Portail sombre Defenseur,Horde";
  app.modalConstants.faction[1760] = "Portail sombre Defenseur,Horde";
  app.modalConstants.faction[1761] = "Incitateur Trigger";
  app.modalConstants.faction[1762] = "Incitateur Trigger 2";
  app.modalConstants.faction[1763] = "Incitateur Trigger 3";
  app.modalConstants.faction[1764] = "Incitateur Trigger 4";
  app.modalConstants.faction[1765] = "Incitateur Trigger 5";
  app.modalConstants.faction[1766] = "Aube d'argent";
  app.modalConstants.faction[1767] = "Aube d'argent";
  app.modalConstants.faction[1768] = "Demon";
  app.modalConstants.faction[1769] = "Demon";
  app.modalConstants.faction[1770] = "Bon acteur";
  app.modalConstants.faction[1771] = "Acteur maléfique";
  app.modalConstants.faction[1772] = "Mana Creature";
  app.modalConstants.faction[1773] = "Khadgar's Servant";
  app.modalConstants.faction[1774] = "Amical";
  app.modalConstants.faction[1775] = "The Sha'tar";
  app.modalConstants.faction[1776] = "The Aldor";
  app.modalConstants.faction[1777] = "The Aldor";
  app.modalConstants.faction[1778] = "The Scale of the Sands";
  app.modalConstants.faction[1779] = "Keepers of Time";
  app.modalConstants.faction[1780] = "Clan Flèchelame";
  app.modalConstants.faction[1781] = "Clan Bloodmaul";
  app.modalConstants.faction[1782] = "Clan Flèchelame";
  app.modalConstants.faction[1783] = "Clan Bloodmaul";
  app.modalConstants.faction[1784] = "Clan Flèchelame";
  app.modalConstants.faction[1785] = "Clan Bloodmaul";
  app.modalConstants.faction[1786] = "Demon";
  app.modalConstants.faction[1787] = "Monstre";
  app.modalConstants.faction[1788] = "Le Consortium";
  app.modalConstants.faction[1789] = "Les faucons du soleil";
  app.modalConstants.faction[1790] = "Clan Flèchelame";
  app.modalConstants.faction[1791] = "Clan Bloodmaul";
  app.modalConstants.faction[1792] = "FelOrc";
  app.modalConstants.faction[1793] = "Les faucons du soleil";
  app.modalConstants.faction[1794] = "Protectorate";
  app.modalConstants.faction[1795] = "Protectorate";
  app.modalConstants.faction[1796] = "Ethereum";
  app.modalConstants.faction[1797] = "Protectorat";
  app.modalConstants.faction[1798] = "Arcane Annihilator(DNR)";
  app.modalConstants.faction[1799] = "Ethereum Sparbuddy";
  app.modalConstants.faction[1800] = "Ethereum";
  app.modalConstants.faction[1801] = "Horde";
  app.modalConstants.faction[1802] = "Alliance";
  app.modalConstants.faction[1803] = "Ambient";
  app.modalConstants.faction[1804] = "Ambient";
  app.modalConstants.faction[1805] = "The Aldor";
  app.modalConstants.faction[1806] = "Amical";
  app.modalConstants.faction[1807] = "Protectorat";
  app.modalConstants.faction[1808] = "Kirin'Var-Belmara";
  app.modalConstants.faction[1809] = "Kirin'Var-Cohlien";
  app.modalConstants.faction[1810] = "Kirin'Var-Dathric";
  app.modalConstants.faction[1811] = "Kirin'Var-Luminrath";
  app.modalConstants.faction[1812] = "Amical";
  app.modalConstants.faction[1813] = "Servant of Illidan";
  app.modalConstants.faction[1814] = "Monstre Spar Buddy";
  app.modalConstants.faction[1815] = "Bete-Loup";
  app.modalConstants.faction[1816] = "Amical";
  app.modalConstants.faction[1818] = "Lower City";
  app.modalConstants.faction[1819] = "Alliance Generique";
  app.modalConstants.faction[1820] = "Ashtongue Deathsworn";
  app.modalConstants.faction[1821] = "Esprits d'Ombrelune 1";
  app.modalConstants.faction[1822] = "Esprits d'Ombrelune 2";
  app.modalConstants.faction[1823] = "Ethereum";
  app.modalConstants.faction[1824] = "Netherwing";
  app.modalConstants.faction[1825] = "Demon";
  app.modalConstants.faction[1826] = "Servant of Illidan";
  app.modalConstants.faction[1827] = "Wyrmcult";
  app.modalConstants.faction[1828] = "Treant";
  app.modalConstants.faction[1829] = "Leotheras Demon I";
  app.modalConstants.faction[1830] = "Leotheras Demon II";
  app.modalConstants.faction[1831] = "Leotheras Demon III";
  app.modalConstants.faction[1832] = "Leotheras Demon IV";
  app.modalConstants.faction[1833] = "Leotheras Demon V";
  app.modalConstants.faction[1834] = "Azaloth";
  app.modalConstants.faction[1835] = "Horde Generique";
  app.modalConstants.faction[1836] = "Le Consortium";
  app.modalConstants.faction[1837] = "Sporeggar";
  app.modalConstants.faction[1838] = "The Scryers";
  app.modalConstants.faction[1839] = "Rock Flayer";
  app.modalConstants.faction[1840] = "Flayer Hunter";
  app.modalConstants.faction[1841] = "Shadowmoon Shade";
  app.modalConstants.faction[1842] = "Legion Communicator";
  app.modalConstants.faction[1843] = "Servant of Illidan";
  app.modalConstants.faction[1844] = "The Aldor";
  app.modalConstants.faction[1845] = "The Scryers";
  app.modalConstants.faction[1846] = "Ravenswood Ancients";
  app.modalConstants.faction[1847] = "Monstre Spar";
  app.modalConstants.faction[1848] = "Monstre Spar Buddy";
  app.modalConstants.faction[1849] = "Servant of Illidan";
  app.modalConstants.faction[1850] = "Netherwing";
  app.modalConstants.faction[1851] = "Lower City";
  app.modalConstants.faction[1852] = "Chess,Amical to All Chess";
  app.modalConstants.faction[1853] = "Servant of Illidan";
  app.modalConstants.faction[1854] = "The Aldor";
  app.modalConstants.faction[1855] = "The Scryers";
  app.modalConstants.faction[1856] = "Sha'tari Skyguard";
  app.modalConstants.faction[1857] = "Amical";
  app.modalConstants.faction[1858] = "Ashtongue Deathsworn";
  app.modalConstants.faction[1859] = "Maiev";
  app.modalConstants.faction[1860] = "Skettis Shadowy Arakkoa";
  app.modalConstants.faction[1862] = "Skettis Arakkoa";
  app.modalConstants.faction[1863] = "Orc,Dragonmaw";
  app.modalConstants.faction[1864] = "Gueule de dragon Ennemi";
  app.modalConstants.faction[1865] = "Orc,Gueule de dragon";
  app.modalConstants.faction[1866] = "Ashtongue Deathsworn";
  app.modalConstants.faction[1867] = "Maiev";
  app.modalConstants.faction[1868] = "Monstre Spar Buddy";
  app.modalConstants.faction[1869] = "Arakkoa";
  app.modalConstants.faction[1870] = "Sha'tari Garde-ciel";
  app.modalConstants.faction[1871] = "Skettis Arakkoa";
  app.modalConstants.faction[1872] = "Ogri'la";
  app.modalConstants.faction[1873] = "Rock Flayer";
  app.modalConstants.faction[1874] = "Ogri'la";
  app.modalConstants.faction[1875] = "The Aldor";
  app.modalConstants.faction[1876] = "The Scryers";
  app.modalConstants.faction[1877] = "Orc,Gueule de dragon";
  app.modalConstants.faction[1878] = "Frenzy";
  app.modalConstants.faction[1879] = "Garde-ciel Ennemi";
  app.modalConstants.faction[1880] = "Orc,Gueule de dragon";
  app.modalConstants.faction[1881] = "Skettis Arakkoa";
  app.modalConstants.faction[1882] = "Servant of Illidan";
  app.modalConstants.faction[1883] = "Déserteur de Theramore";
  app.modalConstants.faction[1884] = "Tuskarr";
  app.modalConstants.faction[1885] = "Vrykul";
  app.modalConstants.faction[1886] = "Creature";
  app.modalConstants.faction[1887] = "Creature";
  app.modalConstants.faction[1888] = "Northsea Pirates";
  app.modalConstants.faction[1889] = "UNUSED";
  app.modalConstants.faction[1890] = "Troll,Amani";
  app.modalConstants.faction[1891] = "Valiance Expedition";
  app.modalConstants.faction[1892] = "Valiance Expedition";
  app.modalConstants.faction[1893] = "Valiance Expedition";
  app.modalConstants.faction[1894] = "Vrykul";
  app.modalConstants.faction[1895] = "Vrykul";
  app.modalConstants.faction[1896] = "Foire de Sombrelune";
  app.modalConstants.faction[1897] = "The Hand of Vengeance";
  app.modalConstants.faction[1898] = "Valiance Expedition";
  app.modalConstants.faction[1899] = "Valiance Expedition";
  app.modalConstants.faction[1900] = "The Hand of Vengeance";
  app.modalConstants.faction[1901] = "Horde Expedition";
  app.modalConstants.faction[1902] = "Acteur maléfique";
  app.modalConstants.faction[1904] = "Acteur maléfique";
  app.modalConstants.faction[1905] = "Tamed Plaguehound";
  app.modalConstants.faction[1906] = "Spotted Gryphon";
  app.modalConstants.faction[1907] = "Test Faction 1";
  app.modalConstants.faction[1908] = "Test Faction 1";
  app.modalConstants.faction[1909] = "Bete-Raptor";
  app.modalConstants.faction[1910] = "Vrykul(AncientSpirit1)";
  app.modalConstants.faction[1911] = "Vrykul(AncientSiprit2)";
  app.modalConstants.faction[1912] = "Vrykul(AncientSiprit3)";
  app.modalConstants.faction[1913] = "CTF-Flag-Alliance";
  app.modalConstants.faction[1914] = "Vrykul";
  app.modalConstants.faction[1915] = "Test";
  app.modalConstants.faction[1916] = "Maiev";
  app.modalConstants.faction[1917] = "Creature";
  app.modalConstants.faction[1918] = "Horde Expedition";
  app.modalConstants.faction[1919] = "Vrykul Gladiator";
  app.modalConstants.faction[1920] = "Valgarde Combatant";
  app.modalConstants.faction[1921] = "The Taunka";
  app.modalConstants.faction[1922] = "Le Taunka";
  app.modalConstants.faction[1923] = "Le Taunka";
  app.modalConstants.faction[1924] = "Monstre,Zone Force Reaction 1";
  app.modalConstants.faction[1925] = "Monstre";
  app.modalConstants.faction[1926] = "Explorer's League";
  app.modalConstants.faction[1927] = "Explorer's League";
  app.modalConstants.faction[1928] = "La main de la vengeance";
  app.modalConstants.faction[1929] = "La main de la vengeance";
  app.modalConstants.faction[1930] = "belier de Course Powerup DND";
  app.modalConstants.faction[1931] = "belier de Course Trap DND";
  app.modalConstants.faction[1932] = "Elementaire";
  app.modalConstants.faction[1933] = "Amical";
  app.modalConstants.faction[1934] = "Bon acteur";
  app.modalConstants.faction[1935] = "Bon acteur";
  app.modalConstants.faction[1936] = "Craig's Squirrels";
  app.modalConstants.faction[1937] = "Craig's Squirrels";
  app.modalConstants.faction[1938] = "Craig's Squirrels";
  app.modalConstants.faction[1939] = "Craig's Squirrels";
  app.modalConstants.faction[1940] = "Craig's Squirrels";
  app.modalConstants.faction[1941] = "Craig's Squirrels";
  app.modalConstants.faction[1942] = "Craig's Squirrels";
  app.modalConstants.faction[1943] = "Craig's Squirrels";
  app.modalConstants.faction[1944] = "Craig's Squirrels";
  app.modalConstants.faction[1945] = "Craig's Squirrels";
  app.modalConstants.faction[1947] = "Craig's Squirrels";
  app.modalConstants.faction[1948] = "Blue";
  app.modalConstants.faction[1949] = "The Kalu' ak";
  app.modalConstants.faction[1950] = "The Kalu' ak";
  app.modalConstants.faction[1951] = "Darnassus";
  app.modalConstants.faction[1952] = "Holiday-WaterBarrel";
  app.modalConstants.faction[1953] = "Monstre,Predator";
  app.modalConstants.faction[1954] = "Iron Nains";
  app.modalConstants.faction[1955] = "Iron Nains";
  app.modalConstants.faction[1956] = "Offensive du Soleil brisé";
  app.modalConstants.faction[1957] = "Offensive du Soleil brisé";
  app.modalConstants.faction[1958] = "Acteur maléfique";
  app.modalConstants.faction[1959] = "Acteur maléfique";
  app.modalConstants.faction[1960] = "Offensive du Soleil brisé";
  app.modalConstants.faction[1961] = "Fighting Vanity Pet";
  app.modalConstants.faction[1962] = "Mort-vivant,Scourge";
  app.modalConstants.faction[1963] = "Demon";
  app.modalConstants.faction[1964] = "Mort-vivant,Scourge";
  app.modalConstants.faction[1965] = "Monstre Spar";
  app.modalConstants.faction[1966] = "Murloc";
  app.modalConstants.faction[1967] = "Offensive du Soleil brisé";
  app.modalConstants.faction[1968] = "Murloc,Winterfin";
  app.modalConstants.faction[1969] = "Murloc";
  app.modalConstants.faction[1970] = "Monstre";
  app.modalConstants.faction[1971] = "Amical,Force Reaction";
  app.modalConstants.faction[1972] = "Object,Force Reaction";
  app.modalConstants.faction[1973] = "Valiance Expedition";
  app.modalConstants.faction[1974] = "Valiance Expedition";
  app.modalConstants.faction[1975] = "Mort-vivant,Scourge";
  app.modalConstants.faction[1976] = "Valiance Expedition";
  app.modalConstants.faction[1977] = "Valiance Expedition";
  app.modalConstants.faction[1978] = "Warsong Offensive";
  app.modalConstants.faction[1979] = "Warsong Offensive";
  app.modalConstants.faction[1980] = "Warsong Offensive";
  app.modalConstants.faction[1981] = "Warsong Offensive";
  app.modalConstants.faction[1982] = "Mort-vivant,Scourge";
  app.modalConstants.faction[1983] = "Monstre Spar";
  app.modalConstants.faction[1984] = "Monstre Spar Buddy";
  app.modalConstants.faction[1985] = "Monstre";
  app.modalConstants.faction[1986] = "Escortee";
  app.modalConstants.faction[1987] = "Cenarion Expedition";
  app.modalConstants.faction[1988] = "Mort-vivant,Scourge";
  app.modalConstants.faction[1989] = "Braconnier";
  app.modalConstants.faction[1990] = "Ambient";
  app.modalConstants.faction[1991] = "Mort-vivant,Scourge";
  app.modalConstants.faction[1992] = "Monstre";
  app.modalConstants.faction[1993] = "Monstre Spar";
  app.modalConstants.faction[1994] = "Monstre Spar Buddy";
  app.modalConstants.faction[1995] = "CTF-Flag-Alliance";
  app.modalConstants.faction[1997] = "CTF-Flag-Alliance";
  app.modalConstants.faction[1998] = "Holiday Monstre";
  app.modalConstants.faction[1999] = "Monstre,Proie";
  app.modalConstants.faction[2000] = "Monstre,Proie";
  app.modalConstants.faction[2001] = "Furbolg,Redfang";
  app.modalConstants.faction[2003] = "Furbolg,Frostpaw";
  app.modalConstants.faction[2004] = "Valiance Expedition";
  app.modalConstants.faction[2005] = "Mort-vivant,Scourge";
  app.modalConstants.faction[2006] = "Kirin Tor";
  app.modalConstants.faction[2007] = "Kirin Tor";
  app.modalConstants.faction[2008] = "Kirin Tor";
  app.modalConstants.faction[2009] = "Kirin Tor";
  app.modalConstants.faction[2010] = "The Wyrmrest Accord";
  app.modalConstants.faction[2011] = "The Wyrmrest Accord";
  app.modalConstants.faction[2012] = "The Wyrmrest Accord";
  app.modalConstants.faction[2013] = "The Wyrmrest Accord";
  app.modalConstants.faction[2014] = "Azjol-Nerub";
  app.modalConstants.faction[2016] = "Azjol-Nerub";
  app.modalConstants.faction[2017] = "Azjol-Nerub";
  app.modalConstants.faction[2018] = "Mort-vivant,Scourge";
  app.modalConstants.faction[2019] = "Le Taunka";
  app.modalConstants.faction[2020] = "Warsong Offensive";
  app.modalConstants.faction[2021] = "REUSE";
  app.modalConstants.faction[2022] = "Monstre";
  app.modalConstants.faction[2023] = "Scourge Invaders";
  app.modalConstants.faction[2024] = "La main de la vengeance";
  app.modalConstants.faction[2025] = "The Silver Covenant";
  app.modalConstants.faction[2026] = "The Silver Covenant";
  app.modalConstants.faction[2027] = "The Silver Covenant";
  app.modalConstants.faction[2028] = "Ambient";
  app.modalConstants.faction[2029] = "Monstre, Predator";
  app.modalConstants.faction[2030] = "Monstre, Predator";
  app.modalConstants.faction[2031] = "Horde Generique";
  app.modalConstants.faction[2032] = "GrizzlyHills Trapper";
  app.modalConstants.faction[2033] = "Monstre";
  app.modalConstants.faction[2034] = "Offensive chanteguerre";
  app.modalConstants.faction[2035] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2036] = "Amical";
  app.modalConstants.faction[2037] = "Expédition Valiance";
  app.modalConstants.faction[2038] = "Ambient";
  app.modalConstants.faction[2039] = "Monstre";
  app.modalConstants.faction[2040] = "Expédition Valiance";
  app.modalConstants.faction[2041] = "The Wyrmrest Accord";
  app.modalConstants.faction[2042] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2043] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2044] = "Expédition Valiance";
  app.modalConstants.faction[2045] = "Offensive chanteguerre";
  app.modalConstants.faction[2046] = "Escortee";
  app.modalConstants.faction[2047] = "The Kalu' ak";
  app.modalConstants.faction[2048] = "Scourge Invaders";
  app.modalConstants.faction[2049] = "Scourge Invaders";
  app.modalConstants.faction[2050] = "Knights of the EbonBlade";
  app.modalConstants.faction[2051] = "Knights of the EbonBlade";
  app.modalConstants.faction[2052] = "Wrathgate Scourge";
  app.modalConstants.faction[2053] = "Wrathgate Alliance";
  app.modalConstants.faction[2054] = "Wrathgate Horde";
  app.modalConstants.faction[2055] = "Monstre Spar";
  app.modalConstants.faction[2056] = "Monstre Spar Buddy";
  app.modalConstants.faction[2057] = "Monstre, ZoneForceReaction 2";
  app.modalConstants.faction[2058] = "CTF-Flag-Horde";
  app.modalConstants.faction[2059] = "CTF-Flag-Neutral";
  app.modalConstants.faction[2060] = "Tribu Frenecœur";
  app.modalConstants.faction[2061] = "Tribu Frenecœur";
  app.modalConstants.faction[2062] = "Tribu Frenecœur";
  app.modalConstants.faction[2063] = "Les oracles";
  app.modalConstants.faction[2064] = "Les oracles";
  app.modalConstants.faction[2065] = "Les oracles";
  app.modalConstants.faction[2066] = "Les oracles";
  app.modalConstants.faction[2067] = "The Wyrmrest Accord";
  app.modalConstants.faction[2068] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2069] = "Troll, Drakkari";
  app.modalConstants.faction[2070] = "Croisade d'argent";
  app.modalConstants.faction[2071] = "Croisade d'argent";
  app.modalConstants.faction[2072] = "Croisade d'argent";
  app.modalConstants.faction[2073] = "Croisade d'argent";
  app.modalConstants.faction[2074] = "Cavernes du temps-Durnholde";
  app.modalConstants.faction[2075] = "CoT Scourge";
  app.modalConstants.faction[2076] = "CoT Arthas";
  app.modalConstants.faction[2077] = "CoT Arthas";
  app.modalConstants.faction[2078] = "CoT Stratholme Citizen";
  app.modalConstants.faction[2079] = "CoT Arthas";
  app.modalConstants.faction[2080] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2081] = "Freya";
  app.modalConstants.faction[2082] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2083] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2084] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2085] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2086] = "Aube d'argent";
  app.modalConstants.faction[2087] = "Aube d'argent";
  app.modalConstants.faction[2088] = "Acteur maléfique";
  app.modalConstants.faction[2089] = "Scarlet Crusade";
  app.modalConstants.faction[2090] = "Mount-Taxi-Alliance";
  app.modalConstants.faction[2091] = "Mount-Taxi-Horde";
  app.modalConstants.faction[2092] = "Mount-Taxi-Neutral";
  app.modalConstants.faction[2093] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2094] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2095] = "Scarlet Crusade";
  app.modalConstants.faction[2096] = "Scarlet Crusade";
  app.modalConstants.faction[2097] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2098] = "Elementaire, Air";
  app.modalConstants.faction[2099] = "Elementaire, Eau";
  app.modalConstants.faction[2100] = "Mort-vivant, Scourge";
  app.modalConstants.faction[2101] = "Acteur maléfique";
  app.modalConstants.faction[2102] = "Acteur maléfique";
  app.modalConstants.faction[2103] = "Scarlet Crusade";
  app.modalConstants.faction[2104] = "Monstre Spar";
  app.modalConstants.faction[2105] = "Monstre Spar Buddy";
  app.modalConstants.faction[2106] = "Ambient";
  app.modalConstants.faction[2107] = "The Sons of Hodir";
  app.modalConstants.faction[2108] = "Iron Giants";
  app.modalConstants.faction[2109] = "Frost Vrykul";
  app.modalConstants.faction[2110] = "Amical";
  app.modalConstants.faction[2111] = "Monstre";
  app.modalConstants.faction[2112] = "The Sons of Hodir";
  app.modalConstants.faction[2113] = "Frost Vrykul";
  app.modalConstants.faction[2114] = "Vrykul";
  app.modalConstants.faction[2115] = "Bon acteur";
  app.modalConstants.faction[2116] = "Vrykul";
  app.modalConstants.faction[2117] = "Bon acteur";
  app.modalConstants.faction[2118] = "Earthen";
  app.modalConstants.faction[2119] = "Monstre Referee";
  app.modalConstants.faction[2120] = "Monstre Referee";
  app.modalConstants.faction[2121] = "The Sunreavers";
  app.modalConstants.faction[2122] = "The Sunreavers";
  app.modalConstants.faction[2123] = "The Sunreavers";
  app.modalConstants.faction[2124] = "Monstre";
  app.modalConstants.faction[2125] = "Frost Vrykul";
  app.modalConstants.faction[2126] = "Frost Vrykul";
  app.modalConstants.faction[2127] = "Ambient";
  app.modalConstants.faction[2128] = "Hyldsmeet";
  app.modalConstants.faction[2129] = "Les saccageurs du soleil";
  app.modalConstants.faction[2130] = " L'alliance d'argent";
  app.modalConstants.faction[2131] = "Croisade d'argent";
  app.modalConstants.faction[2132] = "Offensive chanteguerre";
  app.modalConstants.faction[2133] = "Vrykul de givre";
  app.modalConstants.faction[2134] = "Croisade d'argent";
  app.modalConstants.faction[2135] = "Amical";
  app.modalConstants.faction[2136] = "Ambient";
  app.modalConstants.faction[2137] = "Amical";
  app.modalConstants.faction[2138] = "Croisade d'argent";
  app.modalConstants.faction[2139] = "ScourgeInvaders";
  app.modalConstants.faction[2140] = "Amical";
  app.modalConstants.faction[2141] = "Amical";
  app.modalConstants.faction[2142] = "Alliance";
  app.modalConstants.faction[2143] = "Expedition Valiance";
  app.modalConstants.faction[2144] = "Chevaliers de la Lame d'ebene";
  app.modalConstants.faction[2145] = "Scourge Invaders";
  app.modalConstants.faction[2148] = "The Kalu'ak";
  app.modalConstants.faction[2150] = "Monstre Spar Buddy";
app.modalConstants.faction[2155] = "Ironforge";
app.modalConstants.faction[2156] = "Monstre, Predator";
app.modalConstants.faction[2176] = "ActorGood";
app.modalConstants.faction[2178] = "ActorGood";
app.modalConstants.faction[2189] = "déteste tout";
app.modalConstants.faction[2190] = "déteste tout";
app.modalConstants.faction[2191] = "déteste tout";
app.modalConstants.faction[2209] = "Mort-vivant, Scourge";
app.modalConstants.faction[2210] = "Lune-d'argent City";
app.modalConstants.faction[2212] = "Mort-vivant,Scourge";
app.modalConstants.faction[2214] = "Chevaliers de la Lame d'ebene";
app.modalConstants.faction[2216] = "Le verdict des cendres";
app.modalConstants.faction[2217] = "Le verdict des cendres";
app.modalConstants.faction[2218] = "Le verdict des cendres";
app.modalConstants.faction[2219] = "Le verdict des cendres";
app.modalConstants.faction[2226] = "Chevaliers de la Lame d'ebene";
app.modalConstants.faction[2230] = "Croisade d'argent";
app.modalConstants.faction[2235] = "CTF-Flag-Horde 2";
app.modalConstants.faction[2236] = "CTF-Flag-Alliance 2";
app.modalConstants.faction[1237]    = "GdT Puits d Eternite - Demons";
app.modalConstants.faction[1240]    = "GdT Puits d Eternite - Balles Demoniaques";
app.modalConstants.faction[1241]    = "GdT Puits d Eternite - Traqueurs";
app.modalConstants.faction[1242]    = "Jinyu de Nageperle";
app.modalConstants.faction[1243]    = "Hozen";
app.modalConstants.faction[1245]    = "Mists of Pandaria";
app.modalConstants.faction[1249]    = "JOUEUR, neutre extension 4";
app.modalConstants.faction[1252]    = "Sha";
app.modalConstants.faction[1253]    = "Gardiens du temple de l Est";
app.modalConstants.faction[1265]    = "Shen dralar";
app.modalConstants.faction[1268]    = "Mogu";
app.modalConstants.faction[1269]    = "Lotus dore";
app.modalConstants.faction[1270]    = "Pandashan";
app.modalConstants.faction[1271]    = "l ordre du Serpent-nuage";
app.modalConstants.faction[1272]    = "Les Laboureurs";
app.modalConstants.faction[1273]    = "Jogu l Ivrogne";
app.modalConstants.faction[1275]    = "Ella";
app.modalConstants.faction[1276]    = "Vieux Patte des Hauts";
app.modalConstants.faction[1277]    = "Chii Chii";
app.modalConstants.faction[1278]    = "Sho";
app.modalConstants.faction[1279]    = "Haohan Griffe de Tourbe";
app.modalConstants.faction[1280]    = "Tina Griffe de Tourbe";
app.modalConstants.faction[1281]    = "Gina Griffe de Tourbe";
app.modalConstants.faction[1282]    = "MarEe Pelage de Roseau";
app.modalConstants.faction[1283]    = "Fermier Fung";
app.modalConstants.faction[1286]    = "Gardiens de la Grue rouge";
app.modalConstants.faction[1302]    = "Les Hameconneurs";
app.modalConstants.faction[1336]    = "l Essaim mantide";
app.modalConstants.faction[1337]    = "Les Klaxxi";
app.modalConstants.faction[1338]    = "Mogu gurthan";
app.modalConstants.faction[1339]    = "Mogu harthak";
app.modalConstants.faction[1340]    = "Mogu korgesh";
app.modalConstants.faction[1341]    = "Les Astres venerables";
app.modalConstants.faction[1345]    = "Les Chroniqueurs";
app.modalConstants.faction[1351]    = "Les Maitres brasseurs";
app.modalConstants.faction[1352]    = "Les pandarens huojin";
app.modalConstants.faction[1353]    = "Les pandarens tushui";
app.modalConstants.faction[1357]    = "Nomi";
app.modalConstants.faction[1358]    = "Nat Pagle";
app.modalConstants.faction[1359]    = "Le prince noir";
app.modalConstants.faction[1364]    = "Bienveillant, Force de reaction 2";
app.modalConstants.faction[1366]    = "Amical, force la reaction 3";
app.modalConstants.faction[1367]    = "Amical, force la reaction 4";
app.modalConstants.faction[1368]    = "Amical, force la reaction 5";
app.modalConstants.faction[1369]    = "Amical, force la reaction 6";
app.modalConstants.faction[1370]    = "Amical, force la reaction 7";
app.modalConstants.faction[1371]    = "Pao don";
app.modalConstants.faction[1372]    = "Rosee-de-Miel";
app.modalConstants.faction[1374]    = "Arene de Castagn ar[saison 1)";
app.modalConstants.faction[1375]    = "Offensive Domination";
app.modalConstants.faction[1376]    = "Operation Bouclier";
app.modalConstants.faction[1387]    = "Offensive du Kirin Tor";
app.modalConstants.faction[1388]    = "Assaut des Saccage-soleil";
app.modalConstants.faction[1416]    = "Confiance d Akama";
app.modalConstants.faction[1419]    = "Bar-Tabasse de Bizmo[saison 1)";
app.modalConstants.faction[1433]    = "Monstre, neutre impose pour forcer reaction";
app.modalConstants.faction[1435]    = "Assaut des Pandashan";
app.modalConstants.faction[1440]    = "Rebellion sombrelance";
app.modalConstants.faction[1443]    = "Kor krons";
app.modalConstants.faction[1444]    = "Warlords of Draenor";
app.modalConstants.faction[1445]    = "Orcs loups-de-givre";
app.modalConstants.faction[1447]    = "Trolls sombrelances";
app.modalConstants.faction[1457]    = "Florelins";
app.modalConstants.faction[1483]    = "Mange-Florelins";
app.modalConstants.faction[1491]    = "Les marcheurs du temps";
app.modalConstants.faction[1492]    = "Empereur Shaohao";
app.modalConstants.faction[1496]    = "Ogres de Givrefeu";
app.modalConstants.faction[1500]    = "Fantome rouge";
app.modalConstants.faction[1508]    = "Orcs ombrelunes";
app.modalConstants.faction[1509]    = "Les Blafards";
app.modalConstants.faction[1514]    = "Adherents de Rukhmar";
app.modalConstants.faction[1515]    = "Parias arakkoa";
app.modalConstants.faction[1520]    = "ExilEs ombrelunes";
app.modalConstants.faction[1521]    = "Conseil des ombres";
app.modalConstants.faction[1591]    = "Draenei, Draenor";
app.modalConstants.faction[1617]    = "Primordial";
app.modalConstants.faction[1618]    = "Briseur";
app.modalConstants.faction[1623]    = "La Horde de Fer";
app.modalConstants.faction[1660]    = "Expedition Cenarienne ( Pour escorte )";
app.modalConstants.faction[1665]    = "Les Sargerei";
app.modalConstants.faction[1674]    = "Spores de Draenor";
app.modalConstants.faction[1679]    = "Operation Fourmilier";
app.modalConstants.faction[1680]    = "Cognefort";
app.modalConstants.faction[1681]    = "Lance de Vol jin";
app.modalConstants.faction[1682]    = "Avant-garde de Wrynn";
app.modalConstants.faction[1687]    = "Gorens de Gorgrond";
app.modalConstants.faction[1690]    = "Arene de Castagn ar";
app.modalConstants.faction[1691]    = "Bar-Tabasse de Bizmo";
app.modalConstants.faction[1708]    = "Orcs du Crane-Ricanant";
app.modalConstants.faction[1710]    = "Defense sha tari";
app.modalConstants.faction[1711]    = "Operation de Conservation de Gentepression";
app.modalConstants.faction[1712]    = "Gar Invasion_IronHorde";
app.modalConstants.faction[1713]    = "Gar Invasion_ShadowCouncil";
app.modalConstants.faction[1714]    = "Gar Invasion_Ogres";
app.modalConstants.faction[1715]    = "Gar Invasion_Primals";
app.modalConstants.faction[1716]    = "Gar Invasion_Breakers";
app.modalConstants.faction[1717]    = "Gar Invasion_ThunderLord";
app.modalConstants.faction[1718]    = "Gar Invasion_Shadowmoon";
app.modalConstants.faction[1731]    = "Conseil des exarques";
app.modalConstants.faction[1732]    = "Expedition de Gentepression en Draenor";
app.modalConstants.faction[1733]    = "Delvar Poing-de-Fer";
app.modalConstants.faction[1735]    = "Gardes du corps de caserne";
app.modalConstants.faction[1736]    = "Tormmok";
app.modalConstants.faction[1737]    = "Pretre de la serre Ishaal";
app.modalConstants.faction[1738]    = "Defenseur Illona";
app.modalConstants.faction[1739]    = "Vivianne";
app.modalConstants.faction[1740]    = "Aeda Brillaube";
app.modalConstants.faction[1741]    = "Leorajh";
app.modalConstants.faction[1746]    = "Gronn de Nagrand";
app.modalConstants.faction[1747]    = "Tresor du colisee";
app.modalConstants.faction[1756]    = "LVA_Looters";
app.modalConstants.faction[1757]    = "LVA_Preservers";
app.modalConstants.faction[1758]    = "LVA_Naturalists";
app.modalConstants.faction[1759]    = "LVA_Machines";
app.modalConstants.faction[1760]    = "Gladiateurs de Cognefort";
app.modalConstants.faction[1847]    = "La Main du prophete";
app.modalConstants.faction[1848]    = "Chasseurs de tetes de Vol jin";
app.modalConstants.faction[1849]    = "Ordre des Eveilles";
app.modalConstants.faction[1850]    = "Les Traquesabres";

  /* creature type constants */
  app.modalConstants.type = [];
  app.modalConstants.type[1]  = "Bete	- CREATURE_TYPE_BEAST - (0.6 1.1 0.5)";
  app.modalConstants.type[2]  = "Dragonkin	- CREATURE_TYPE_DRAGONKIN - (0.8 1.1 0.7 1.5)";
  app.modalConstants.type[3]  = "Demon	- CREATURE_TYPE_DEMON - (0.8 1.1 0.7 1.5)";
  app.modalConstants.type[4]  = "Elementaire - CREATURE_TYPE_ELEMENTAL - (0.5 0.9 0.9)";
  app.modalConstants.type[5]  = "Geant - CREATURE_TYPE_GIANT - (0.8 1.2 0.8)";
  app.modalConstants.type[6]  = "Mort-vivant - CREATURE_TYPE_UNDEAD - (0.5 0.9 0.4)";
  app.modalConstants.type[7]  = "Humanoide - CREATURE_TYPE_HUMANOID - (0.6 1.0 0.6)";
  app.modalConstants.type[8]  = "Bestiole - CREATURE_TYPE_CRITTER - (0.3 1.0 0.5)";
  app.modalConstants.type[9]  = "Mechanique - CREATURE_TYPE_MECHANICAL - (0.65 1.0 0.65)";
  app.modalConstants.type[10] = "Not specified - CREATURE_TYPE_NOT_SPECIFIED - (0.6 1.0 0.6)";
  app.modalConstants.type[11] = "Totem - CREATURE_TYPE_TOTEM";
  app.modalConstants.type[12] = "Familier non combattant - CREATURE_TYPE_NON_COMBAT_PET - (0.3 1.0 0.5)";
  app.modalConstants.type[13] = "Nuage de gaz - CREATURE_TYPE_GAS_CLOUD - (0.3 1.0 0.5)";
  app.modalConstants.type[14] = "Familier sauvage - CREATURE_TYPE_WILD_PET - (0.3 1.0 0.5)";
  app.modalConstants.type[15] = "Aberation - CREATURE_TYPE_ABERRATION - (0.3 0.6 0.6)";
  app.modalConstants.type[16] = "Elementaire Glissant - CREATURE_TYPE_ELEMENTAL_GLISSANT - (0.2 0.5 0.75)";
  app.modalConstants.type[17]  = "Humanoide2 - CREATURE_TYPE_HUMANOID_AUTRE - (0.6 1.0 0.6) - Pas d'emote ou d'armes rangees";
  app.modalConstants.type[18] = "Dirigeable etc CREATURE_TYPE_MACHINE_VOLANTE - (4 4 4 6)";

  /* race constants */
  app.modalConstants.race = [];
  app.modalConstants.race[0]  = "HUMAIN - RACE_HUMAN";
  app.modalConstants.race[1]  = "ORC - RACE_ORC";
  app.modalConstants.race[2]  = "NAIN - RACE_DWARF";
  app.modalConstants.race[3]  = "ELFE DE LA NUIT - RACE_NIGHTELF";
  app.modalConstants.race[4]  = "MORT-VIVANT - RACE_UNDEAD_PLAYER";
  app.modalConstants.race[5]  = "TAUREN - RACE_TAUREN ";
  app.modalConstants.race[6]  = "GNOME - RACE_GNOME ";
  app.modalConstants.race[7]  = "TROLL - RACE_TROLL  ";
  app.modalConstants.race[8]  = "GOBLIN - RACE_GOBLIN ";
  app.modalConstants.race[9]  = "ELFE DE SANG - RACE_BLOODELF";
  app.modalConstants.race[10] = "DRAENEIRACE_DRAENEI";
  app.modalConstants.race[11] = "FEL_ORC (NOT USED) - RACE_FEL_ORC";
  app.modalConstants.race[12] = "NAGA (NOT USED) - RACE_NAGA";
  app.modalConstants.race[13] = "BROKEN (NOT USED) - RACE_BROKEN	";
  app.modalConstants.race[14] = "SKELETON (NOT USED) - RACE_SKELETON";
  app.modalConstants.race[15] = "VRYKUL (NOT USED) - RACE_VRYKUL ";
  app.modalConstants.race[16] = "TUSKARR (NOT USED) - RACE_TUSKARR  ";
  app.modalConstants.race[17] = "FOREST_TROLL (NOT USED) - RACE_FOREST_TROLL ";
  app.modalConstants.race[18] = "TAUNKA (NOT USED) - RACE_TAUNKA ";
  app.modalConstants.race[19] = "NORTHREND_SKELETON (NOT USED) - RACE_NORTHREND_SKELETON";
  app.modalConstants.race[20] = "ICE_TROLL (NOT USED) - RACE_ICE_TROLL   ";
  app.modalConstants.race[21] = "WORGEN - RACE_WORGEN ";
  app.modalConstants.race[22] = "GILNEAN - RACE_GILNEAN ";
  app.modalConstants.race[23] = "PANDAREN_NEUTRAL - RACE_PANDAREN_NEUTRAL";
  app.modalConstants.race[24] = "PANDAREN_ALLIANCE - RACE_PANDAREN_ALLIANCE ";
  app.modalConstants.race[25] = "PANDAREN_HORDE - RACE_PANDAREN_HORDE";
          
  /* creature classes constants */
  app.modalConstants.classes = [];
  app.modalConstants.classes[1]  = "Guerrier - CLASS_WARRIOR";
  app.modalConstants.classes[2]  = "Paladin - CLASS_PALADIN";
  app.modalConstants.classes[4]  = "Voleur - CLASS_ROGUE";
  app.modalConstants.classes[8]  = "Mage/Moine - CLASS_MAGE/CLASS_MONK";
/*    app.modalConstants.classes[9]  = "Unk";*/ 

  app.modalConstants.trainer = [];
  app.modalConstants.trainer[1]  = "Guerrier - CLASS_WARRIOR";
  app.modalConstants.trainer[2]  = "Paladin - CLASS_PALADIN";
  app.modalConstants.trainer[3]  = "Chasseur - CLASS_HUNTER";
  app.modalConstants.trainer[4]  = "Voleur - CLASS_ROGUE";
  app.modalConstants.trainer[5]  = "Pretre - CLASS_PRIEST";
  app.modalConstants.trainer[6]  = "Chevalier de la mort - CLASS_DEATH_KNIGHT";
  app.modalConstants.trainer[7]  = "Shaman - CLASS_SHAMAN";
  app.modalConstants.trainer[5]  = "Mage - CLASS_MAGE";
  app.modalConstants.trainer[9]  = "Demoniste - CLASS_WARLOCK";
  app.modalConstants.trainer[10] = "Moine - CLASS_MONK";
  app.modalConstants.trainer[11] = "Druide - CLASS_DRUID";
 
  /* item classes constants */
  app.modalConstants.item_class = [];
  app.modalConstants.item_class[0]  = "Consommable";
  app.modalConstants.item_class[1]  = "Recipient";
  app.modalConstants.item_class[2]  = "Arme";
  app.modalConstants.item_class[3]  = "Geme";
  app.modalConstants.item_class[4]  = "Armure";
  app.modalConstants.item_class[5]  = "Reactif";
  app.modalConstants.item_class[6]  = "Projectile";
  app.modalConstants.item_class[7]  = "Marchandises";
  app.modalConstants.item_class[8]  = "Generic (OBSOLETE)";
  app.modalConstants.item_class[9]  = "Recette";
  app.modalConstants.item_class[10] = "Money (OBSOLETE)";
  app.modalConstants.item_class[11] = "Quiver";
  app.modalConstants.item_class[12] = "Quest";
  app.modalConstants.item_class[13] = "Key";
  app.modalConstants.item_class[14] = "Permanent (OBSOLETE)";
  app.modalConstants.item_class[15] = "Miscellaneous";
  app.modalConstants.item_class[16] = "Glyphe";

  /* item subclass constants */
  app.modalConstants.item_subclass = [];

  for (var i = 0; i < 17; i++) {
    app.modalConstants.item_subclass[i] = [];
  }

  app.modalConstants.item_subclass[0][0]    = "Consommable (Usability in combat is decided by the spell assigned)";
  app.modalConstants.item_subclass[0][1]    = "Potion";
  app.modalConstants.item_subclass[0][2]    = "Elixir";
  app.modalConstants.item_subclass[0][3]    = "Ballon";
  app.modalConstants.item_subclass[0][4]    = "Parchemin";
  app.modalConstants.item_subclass[0][5]    = "Nourriture & boisson";
  app.modalConstants.item_subclass[0][6]    = "Item Enhancement";
  app.modalConstants.item_subclass[0][7]    = "Bandage";
  app.modalConstants.item_subclass[0][8]    = "Autre";
  app.modalConstants.item_subclass[1][0]    = "Sac";
  app.modalConstants.item_subclass[1][1]    = "Sac d'ame";
  app.modalConstants.item_subclass[1][2]    = "Sac d herboriste";
  app.modalConstants.item_subclass[1][3]    = "Sac D enchanteur";
  app.modalConstants.item_subclass[1][4]    = "Sac d Ingenieur";
  app.modalConstants.item_subclass[1][5]    = "Sac joaillerie";
  app.modalConstants.item_subclass[1][6]    = "Sac de mineur";
  app.modalConstants.item_subclass[1][7]    = "Sac de Travail du cuir";
  app.modalConstants.item_subclass[1][8]    = "Inscription Bag";
  app.modalConstants.item_subclass[2][0]    = "Axe,One handed";
  app.modalConstants.item_subclass[2][1]    = "Axe,Two handed";
  app.modalConstants.item_subclass[2][2]    = "Bow";
  app.modalConstants.item_subclass[2][3]    = "Gun";
  app.modalConstants.item_subclass[2][4]    = "Mace,One handed";
  app.modalConstants.item_subclass[2][5]    = "Mace,Two handed";
  app.modalConstants.item_subclass[2][6]    = "Polearm";
  app.modalConstants.item_subclass[2][7]    = "Sword,One handed";
  app.modalConstants.item_subclass[2][8]    = "Sword,Two handed";
  app.modalConstants.item_subclass[2][9]    = "Obsolete";
  app.modalConstants.item_subclass[2][10]   = "Staff";
  app.modalConstants.item_subclass[2][11]   = "Exotic";
  app.modalConstants.item_subclass[2][12]   = "Exotic";
  app.modalConstants.item_subclass[2][13]   = "Fist Weapon";
  app.modalConstants.item_subclass[2][14]   = "Miscellaneous (Blacksmith Hammer, Mining Pick, etc.)";
  app.modalConstants.item_subclass[2][15]   = "Dagger";
  app.modalConstants.item_subclass[2][16]   = "Thrown";
  app.modalConstants.item_subclass[2][17]   = "Spear";
  app.modalConstants.item_subclass[2][18]   = "Crossbow";
  app.modalConstants.item_subclass[2][19]   = "Wand";
  app.modalConstants.item_subclass[2][20]   = "Fishing Pole";
  app.modalConstants.item_subclass[3][0]    = "Red";
  app.modalConstants.item_subclass[3][1]    = "Blue";
  app.modalConstants.item_subclass[3][2]    = "Yellow";
  app.modalConstants.item_subclass[3][3]    = "Purple";
  app.modalConstants.item_subclass[3][4]    = "Green";
  app.modalConstants.item_subclass[3][5]    = "Orange";
  app.modalConstants.item_subclass[3][6]    = "Meta";
  app.modalConstants.item_subclass[3][7]    = "Simple";
  app.modalConstants.item_subclass[3][8]    = "Prismatic";
  app.modalConstants.item_subclass[4][0]    = "Miscellaneous";
  app.modalConstants.item_subclass[4][1]    = "Cloth";
  app.modalConstants.item_subclass[4][2]    = "Leather";
  app.modalConstants.item_subclass[4][3]    = "Mail";
  app.modalConstants.item_subclass[4][4]    = "Plate";
  app.modalConstants.item_subclass[4][5]    = "Buckler(OBSOLETE)";
  app.modalConstants.item_subclass[4][6]    = "Shield";
  app.modalConstants.item_subclass[4][7]    = "Libram";
  app.modalConstants.item_subclass[4][8]    = "Idol";
  app.modalConstants.item_subclass[4][9]    = "Totem";
  app.modalConstants.item_subclass[4][10]   = "Sigil";
  app.modalConstants.item_subclass[5][0]    = "Reagent";
  app.modalConstants.item_subclass[6][0]    = "Wand(OBSOLETE)";
  app.modalConstants.item_subclass[6][1]    = "Bolt(OBSOLETE)";
  app.modalConstants.item_subclass[6][2]    = "Arrow";
  app.modalConstants.item_subclass[6][3]    = "Bullet";
  app.modalConstants.item_subclass[6][4]    = "Thrown(OBSOLETE)";
  app.modalConstants.item_subclass[7][0]    = "Trade Goods";
  app.modalConstants.item_subclass[7][1]    = "Parts";
  app.modalConstants.item_subclass[7][2]    = "Explosives";
  app.modalConstants.item_subclass[7][3]    = "Devices";
  app.modalConstants.item_subclass[7][4]    = "Jewelcrafting";
  app.modalConstants.item_subclass[7][5]    = "Cloth";
  app.modalConstants.item_subclass[7][6]    = "Leather";
  app.modalConstants.item_subclass[7][7]    = "Metal & Stone";
  app.modalConstants.item_subclass[7][8]    = "Meat";
  app.modalConstants.item_subclass[7][9]    = "Herb";
  app.modalConstants.item_subclass[7][10]   = "Elemental";
  app.modalConstants.item_subclass[7][11]   = "Other";
  app.modalConstants.item_subclass[7][12]   = "Enchanting";
  app.modalConstants.item_subclass[7][13]   = "Materials";
  app.modalConstants.item_subclass[7][14]   = "Armor Enchantment";
  app.modalConstants.item_subclass[7][15]   = "Weapon Enchantment";
  app.modalConstants.item_subclass[8][0]    = "Generic(OBSOLETE)";
  app.modalConstants.item_subclass[9][0]    = "Book";
  app.modalConstants.item_subclass[9][1]    = "Leatherworking";
  app.modalConstants.item_subclass[9][2]    = "Tailoring";
  app.modalConstants.item_subclass[9][3]    = "Engineering";
  app.modalConstants.item_subclass[9][4]    = "Blacksmithing";
  app.modalConstants.item_subclass[9][5]    = "Cooking";
  app.modalConstants.item_subclass[9][6]    = "Alchemy";
  app.modalConstants.item_subclass[9][7]    = "First Aid";
  app.modalConstants.item_subclass[9][8]    = "Enchanting";
  app.modalConstants.item_subclass[9][9]    = "Fishing";
  app.modalConstants.item_subclass[9][10]   = "Jewelcrafting";
  app.modalConstants.item_subclass[10][0]   = "Money(OBSOLETE)";
  app.modalConstants.item_subclass[11][0]   = "Quiver(OBSOLETE)";
  app.modalConstants.item_subclass[11][1]   = "Quiver(OBSOLETE)";
  app.modalConstants.item_subclass[11][2]   = "Quiver,Can hold arrows";
  app.modalConstants.item_subclass[11][3]   = "Ammo Pouch,Can hold bullets";
  app.modalConstants.item_subclass[12][0]   = "Quest";
  app.modalConstants.item_subclass[13][0]   = "Key";
  app.modalConstants.item_subclass[13][1]   = "Lockpick";
  app.modalConstants.item_subclass[14][0]   = "Permanent";
  app.modalConstants.item_subclass[15][0]   = "Junk";
  app.modalConstants.item_subclass[15][1]   = "Reagent";
  app.modalConstants.item_subclass[15][2]   = "Pet";
  app.modalConstants.item_subclass[15][3]   = "Holiday";
  app.modalConstants.item_subclass[15][4]   = "Other";
  app.modalConstants.item_subclass[15][5]   = "Mount";
  app.modalConstants.item_subclass[16][1]   = "Warrior";
  app.modalConstants.item_subclass[16][2]   = "Paladin";
  app.modalConstants.item_subclass[16][3]   = "Hunter";
  app.modalConstants.item_subclass[16][4]   = "Rogue";
  app.modalConstants.item_subclass[16][5]   = "Priest";
  app.modalConstants.item_subclass[16][6]   = "Death Knight";
  app.modalConstants.item_subclass[16][7]   = "Shaman";
  app.modalConstants.item_subclass[16][8]   = "Mage";
  app.modalConstants.item_subclass[16][9]   = "Warlock";
  app.modalConstants.item_subclass[16][11]  = "Druid";


  /* InventoryType constants */
  app.modalConstants.InventoryType = [];
  app.modalConstants.InventoryType[0]  = "Non equipable";
  app.modalConstants.InventoryType[1]  = "Tete";
  app.modalConstants.InventoryType[2]  = "Cou";
  app.modalConstants.InventoryType[3]  = "Epaule";
  app.modalConstants.InventoryType[4]  = "Corps (chemise)";
  app.modalConstants.InventoryType[5]  = "Poitrine";
  app.modalConstants.InventoryType[6]  = "Ceinture";
  app.modalConstants.InventoryType[7]  = "Jambes";
  app.modalConstants.InventoryType[8]  = "Pieds (bottes)";
  app.modalConstants.InventoryType[9]  = "Poignets (brassards)";
  app.modalConstants.InventoryType[10] = "Mains (gants)";
  app.modalConstants.InventoryType[11] = "Doigt (Bague)";
  app.modalConstants.InventoryType[12] = "Bibelot";
  app.modalConstants.InventoryType[13] = "Arme d'une main";
  app.modalConstants.InventoryType[14] = "Arme Main Libre / Bouclier";
  app.modalConstants.InventoryType[15] = "A distance (Arc)";
  app.modalConstants.InventoryType[16] = "Cape";
  app.modalConstants.InventoryType[17] = "Arme a deux mains";
  app.modalConstants.InventoryType[18] = "Sac (y compris les carquois)";
  app.modalConstants.InventoryType[19] = "Tabard";
  app.modalConstants.InventoryType[20] = "Robe";
  app.modalConstants.InventoryType[21] = "Main principale";
  app.modalConstants.InventoryType[22] = "Hors Main (Articles Divers)";
  app.modalConstants.InventoryType[23] = "Tome";
  app.modalConstants.InventoryType[24] = "Munition";
  app.modalConstants.InventoryType[25] = "Jete";
  app.modalConstants.InventoryType[26] = "Ranged Right (Gun)";
  app.modalConstants.InventoryType[27] = "Quiver";
  app.modalConstants.InventoryType[28] = "Relique";

  /* Emote constants */
  app.modalConstants.emote = [];
  app.modalConstants.emote[0]   = "UNIQUE_Aucun";
  app.modalConstants.emote[1]   = "UNIQUE_PARLER(DNR)";
  app.modalConstants.emote[2]   = "UNIQUE_ARC";
  app.modalConstants.emote[3]   = "UNIQUE_VAGUE(DNR)";
  app.modalConstants.emote[4]   = "UNIQUE_ACCLAMATION / Contant";
  app.modalConstants.emote[5]   = "UNIQUE_EXCLAMATION(DNR)";
  app.modalConstants.emote[6]   = "UNIQUE_QUESTION";
  app.modalConstants.emote[7]   = "UNIQUE MANGER";
  app.modalConstants.emote[10]  = "BOUCLE DANCE";
  app.modalConstants.emote[11]  = "UNIQUE_RIRE";
  app.modalConstants.emote[12]  = "BOUCLE_DORMIR";
  app.modalConstants.emote[13]  = "BOUCLE_ASSEOIR";
  app.modalConstants.emote[14]  = "UNIQUE_GROSSIER(DNR)";
  app.modalConstants.emote[15]  = "UNIQUE_RUGIR(DNR)";
  app.modalConstants.emote[16]  = "UNIQUE_S'AGENOUILLER";
  app.modalConstants.emote[17]  = "UNIQUE_EMBRASSER";
  app.modalConstants.emote[18]  = "UNIQUE_Pleurer";
  app.modalConstants.emote[19]  = "UNIQUE_POULET";
  app.modalConstants.emote[20]  = "UNIQUE_MENDIER";
  app.modalConstants.emote[21]  = "UNIQUE_APPLAUDIR";
  app.modalConstants.emote[22]  = "UNIQUE_CRIER(DNR)";
  app.modalConstants.emote[23]  = "UNIQUE_FLÉCHIR";
  app.modalConstants.emote[24]  = "UNIQUE TIMIDE(DNR)";
  app.modalConstants.emote[25]  = "UNIQUE POINT(DNR)";
  app.modalConstants.emote[26]  = "BOUCLE SUPPORTER !";
  app.modalConstants.emote[27]  = "BOUCLE_PRET SANS ARME";
  app.modalConstants.emote[28]  = "BOUCLE_TRAVAIL_SHMANGERHED";
  app.modalConstants.emote[29]  = "BOUCLE_POINT(DNR)";
  app.modalConstants.emote[30]  = "BOUCLE_AUCUN";
  app.modalConstants.emote[33]  = "UNIQUE_BLESSURE";
  app.modalConstants.emote[34]  = "UNIQUE_BLESSURECRITICAL";
  app.modalConstants.emote[35]  = "UNIQUE_ATTAQUE SANS ARME";
  app.modalConstants.emote[36]  = "UNIQUE_ ATTAQUE 1H";
  app.modalConstants.emote[37]  = "UNIQUE_ ATTAQUE 2HTIGHT";
  app.modalConstants.emote[38]  = "UNIQUE_ ATTAQUE 2HLOOSE";
  app.modalConstants.emote[39]  = "UNIQUE_PARE SANS ARME";
  app.modalConstants.emote[43]  = "BOUCLE_PARE AVEC BOUCLIER";
  app.modalConstants.emote[44]  = "UNIQUE_En garde ARME";
  app.modalConstants.emote[45]  = "UNIQUE_En Garde 1H";
  app.modalConstants.emote[48]  = "UNIQUE_PRET ARC !";
  app.modalConstants.emote[50]  = "UNIQUE_SPELLPRECAST !";
  app.modalConstants.emote[51]  = "BOUCLE Lancer sort avec force";
  app.modalConstants.emote[53]  = "BOUCLE CRI DE BATAILLE";
  app.modalConstants.emote[54]  = "UNIQUE_SPECIAL ATTAQUE 1retourne et frape 1H";
  app.modalConstants.emote[60]  = "BOUCLE Coup de pied haut";
  app.modalConstants.emote[61]  = "BOUCLE ATTAQUE JETER";
  app.modalConstants.emote[64]  = "BOUCLE_ETOURDIR";
  app.modalConstants.emote[65]  = "BOUCLE_MORT !";
  app.modalConstants.emote[66]  = "BOUCLE SALUER";
  app.modalConstants.emote[68]  = "BOUCLE_S'AGENOUILLER !";
  app.modalConstants.emote[69]  = "BOUCLE_ Travaille avec les mains";
  app.modalConstants.emote[70]  = "BOUCLE Coucou";
  app.modalConstants.emote[71]  = "UNIQUE ACCLAMATION/Contant";
  app.modalConstants.emote[92]  = "BOUCLE MANGER_NOSHMANGERHE";
  app.modalConstants.emote[93]  = "BOUCLE ETOURDIR";
  app.modalConstants.emote[94]  = "BOUCLE_DANCE";
  app.modalConstants.emote[113] = "BOUCLESALUER/garde a vous";
  app.modalConstants.emote[133] = "BOUCLE Travaille avec les mains";
  app.modalConstants.emote[153] = "BOUCLE RIRE";
  app.modalConstants.emote[173] = "BOUCLE_TRAVAIL , bucheron sans item";
  app.modalConstants.emote[193] = "BOUCLE_SPELLPRECAST !";
  app.modalConstants.emote[213] = "BOUCLE En garde FUSIL (freze) ";
  app.modalConstants.emote[214] = "BOUCLE _En garde FUSIL";
  app.modalConstants.emote[233] = "BOUCLE_TRAVAIL_MINEUR (sans item)";
  app.modalConstants.emote[234] = "BOUCLE_TRAVAIL_BUCHERON (avec item)";
  app.modalConstants.emote[253] = "BOUCLE_APPLAUDIR";
  app.modalConstants.emote[254] = "UNIQUE_DECOLLAGE !";
  app.modalConstants.emote[273] = "BOUCLE OUI(DNR)";
  app.modalConstants.emote[274] = "BOUCLE NON(DNR)";
  app.modalConstants.emote[275] = "BOUCLE TRAIN(DNR)";
  app.modalConstants.emote[293] = "UNIQUE_LAND !";
  app.modalConstants.emote[313] = "BOUCLE_A l'aise !";
  app.modalConstants.emote[333] = "BOUCLE_PRET , fixe 1H";
  app.modalConstants.emote[353] = "BOUCLE_SPELLS'AGENOUILLERSTART";
  app.modalConstants.emote[373] = "BOUCLE_SUBMERGÉ !";
  app.modalConstants.emote[374] = "UNIQUE_SUBMERGER";
  app.modalConstants.emote[375] = "BOUCLE En garde , fixe 2H";
  app.modalConstants.emote[376] = "BOUCLE PRET ARC , fixe , de profil";
  app.modalConstants.emote[377] = "UNIQUE_MONTURE SPECIALE";
  app.modalConstants.emote[378] = "BOUCLE_PARLER";
  app.modalConstants.emote[379] = "BOUCLE_PECHE";
  app.modalConstants.emote[380] = "UNIQUE_PECHE";
  app.modalConstants.emote[381] = "BOUCLE_LOOT";
  app.modalConstants.emote[382] = "BOUCLE_TOURBILLON";
  app.modalConstants.emote[383] = "BOUCLE_NOYER , fixe";
  app.modalConstants.emote[384] = "BOUCLE_TENIR ARC, fixe";
  app.modalConstants.emote[385] = "BOUCLE_TENIR FUSIL,fixe ***";
  app.modalConstants.emote[386] = "BOUCLE_En garde , fixe";
  app.modalConstants.emote[387] = "BOUCLE_NOYER,bof";
  app.modalConstants.emote[388] = "UNIQUE_STOMP";
  app.modalConstants.emote[389] = "BOUCLE ATTAQUE OFF";
  app.modalConstants.emote[390] = "BOUCLE ATTAQUE OFFPIERCE";
  app.modalConstants.emote[391] = "BOUCLE RUGIR";
  app.modalConstants.emote[392] = "BOUCLE_RIRE";
  app.modalConstants.emote[393] = "UNIQUE_CRMANGERURE_SPECIAL !";
  app.modalConstants.emote[394] = "UNIQUE_JUMPLANDRUN";
  app.modalConstants.emote[395] = "BOUCLE_JUMPEND";
  app.modalConstants.emote[396] = "UNIQUE_PARLER NOSHMANGERHE";
  app.modalConstants.emote[397] = "BOUCLE Montre du doigt";
  app.modalConstants.emote[398] = "BOUCLE_CANNIBALIZE / agenouiller";
  app.modalConstants.emote[399] = "UNIQUE_JUMP START/roulade en l'air avec pause";
  app.modalConstants.emote[400] = "BOUCLE_DANCES PECIAL";
  app.modalConstants.emote[401] = "UNIQUE_DANCES PECIAL";
  app.modalConstants.emote[402] = "UNIQUE_CUSTOMSPELL01";
  app.modalConstants.emote[403] = "UNIQUE_CUSTOMSPELL02 !";
  app.modalConstants.emote[404] = "UNIQUE_CUSTOMSPELL03";
  app.modalConstants.emote[405] = "UNIQUE_CUSTOMSPELL04";
  app.modalConstants.emote[406] = "UNIQUE_CUSTOMSPELL05";
  app.modalConstants.emote[407] = "UNIQUE_CUSTOMSPELL06";
  app.modalConstants.emote[408] = "UNIQUE_CUSTOMSPELL07";
  app.modalConstants.emote[409] = "UNIQUE_CUSTOMSPELL08 !";
  app.modalConstants.emote[410] = "UNIQUE_CUSTOMSPELL09 !";
  app.modalConstants.emote[411] = "UNIQUE_CUSTOMSPELL10";
  app.modalConstants.emote[412] = "BOUCLE_EXCLAMER";
  app.modalConstants.emote[413] = "BOUCLE_DANCE_CUSTOM !";
  app.modalConstants.emote[415] = "BOUCLE_ASSEOIR CHAISE MED";
  app.modalConstants.emote[416] = "BOUCLE_CUSTOM_SPELL_01 !";
  app.modalConstants.emote[417] = "BOUCLE_CUSTOM_SPELL_02 !";
  app.modalConstants.emote[418] = "BOUCLE_MANGER";
  app.modalConstants.emote[419] = "BOUCLE_CUSTOM_SPELL_04 !";
  app.modalConstants.emote[420] = "BOUCLE_CUSTOM_SPELL_03 !";
  app.modalConstants.emote[421] = "BOUCLE_CUSTOM_SPELL_05 !";
  app.modalConstants.emote[422] = "BOUCLE_SPELLEFFECT_HOLD !";
  app.modalConstants.emote[423] = "BOUCLE_MANGER,boucle";
  app.modalConstants.emote[424] = "BOUCLE_Monture";
  app.modalConstants.emote[425] = "BOUCLE_En garde,fixe 2HL";
  app.modalConstants.emote[426] = "BOUCLE_ASSEOIR CHAISE HIGH";
  app.modalConstants.emote[427] = "BOUCLE_TOMBER";
  app.modalConstants.emote[428] = "BOUCLE_agenouille/ramasse loot,fixe";
  app.modalConstants.emote[429] = "BOUCLE_SUBMERGÉ_NEW !";
  app.modalConstants.emote[430] = "UNIQUE_RECROQUEVILLER(DNR)";
  app.modalConstants.emote[431] = "BOUCLE_Efraye";
  app.modalConstants.emote[432] = "BOUCLE travaille avec les mains";
  app.modalConstants.emote[433] = "BOUCLE_FURTIF , fixe";
  app.modalConstants.emote[434] = "UNIQUE_OMNICAST_GHOUL (W/SOUND)";
  app.modalConstants.emote[435] = "BOUCLE_ ATTAQUE ARC";
  app.modalConstants.emote[436] = "BOUCLE_ ATTAQUE FUSIL";
  app.modalConstants.emote[437] = "BOUCLE_Nager au ralenti sur place";
  app.modalConstants.emote[438] = "BOUCLE_ ATTAQUE  SANS ARMES";
  app.modalConstants.emote[439] = "BOUCLE_LANCER UN SORT (W/SOUND)";
  app.modalConstants.emote[440] = "BOUCLE_ESQUIVER";
  app.modalConstants.emote[441] = "BOUCLE_PARER 1H";
  app.modalConstants.emote[442] = "BOUCLE_PARER 2H";
  app.modalConstants.emote[443] = "UNIQUE_PARER,fixe 2HL";
  app.modalConstants.emote[444] = "BOUCLE_FLY TOMBER";
  app.modalConstants.emote[445] = "BOUCLE_Tomber a terre";
  app.modalConstants.emote[446] = "BOUCLE_Voler TOMBER";
  app.modalConstants.emote[447] = "BOUCLE_Voler ASSEOIR SOL DOWN";
  app.modalConstants.emote[448] = "BOUCLE_Voler ASSEOIR SOL UP";
  app.modalConstants.emote[449] = "UNIQUE_EMERGE !" ;
  app.modalConstants.emote[450] = "UNIQUE_DRAGONSPIT !";
  app.modalConstants.emote[451] = "BOUCLE_Frape arriere";
  app.modalConstants.emote[452] = "UNIQUE_FLY GRAB 1";
  app.modalConstants.emote[453] = "BOUCLE_FLY GRABCLOSED !";
  app.modalConstants.emote[454] = "UNIQUE_FLY GRABJETER";
  app.modalConstants.emote[455] = "BOUCLE_Assis par terre";
  app.modalConstants.emote[456] = "BOUCLE_Marche en arriere";
  app.modalConstants.emote[457] = "UNIQUE_FLY PARLER";
  app.modalConstants.emote[458] = "BOUCLE_ ATTAQUE 1H";
  app.modalConstants.emote[459] = "BOUCLE_CUSTOMSPELL08 !";
  app.modalConstants.emote[460] = "UNIQUE_Voler DRAGONSPIT !";
  app.modalConstants.emote[461] = "BOUCLE_ASSEOIR CHAISE LOW";
  app.modalConstants.emote[462] = "BOUCLE Stun";
  app.modalConstants.emote[463] = "UNIQUE_SPELLCAST_OMNI";
  app.modalConstants.emote[465] = "BOUCLE_en garde";
  app.modalConstants.emote[466] = "UNIQUE_TRAVAIL_BUCHERON (sans item)";
  app.modalConstants.emote[467] = "UNIQUE_TRAVAIL_MINEUR (sans item)";
  app.modalConstants.emote[468] = "BOUCLE_Canalise sort , bras en l'air";
  app.modalConstants.emote[469] = "BOUCLE_Canalise sort , fixe";
  app.modalConstants.emote[470] = "SUPPORTER_BOUCLE_AUCUN";
  app.modalConstants.emote[471] = "BOUCLE_PRET JOUST !";
  app.modalConstants.emote[473] = "BOUCLE_ETRANGLER , ne touche pas le sol";
  app.modalConstants.emote[474] = "BOUCLE_Canalise sort heal";
  app.modalConstants.emote[475] = "BOUCLE_En garde pour lance ?!";
  app.modalConstants.emote[476] = "UNIQUE_Pleurer";
  app.modalConstants.emote[477] = "UNIQUE_SPECIAL UNARMED";
  app.modalConstants.emote[478] = "BOUCLE_DANCE_NOSHMANGERHE";
  app.modalConstants.emote[479] = "UNIQUE_RENIFLER";
  app.modalConstants.emote[480] = "UNIQUE_DRAGONSTOMP";
  app.modalConstants.emote[482] = "UNIQUE_Tomber a la renverse , en Boucle";
  app.modalConstants.emote[483] = "BOUCLE_Lire carte";
  app.modalConstants.emote[485] = "UNIQUE_Parler";
  app.modalConstants.emote[492] = "BOUCLE_Lire carte";
  app.modalConstants.emote[498] = "BOUCLE_CUSTOM_SPELL_06";
  app.modalConstants.emote[499] = "BOUCLE_CUSTOM_SPELL_07";
  app.modalConstants.emote[500] = "BOUCLE_CUSTOM_SPELL_08";
  app.modalConstants.emote[501] = "BOUCLE_CUSTOM_SPELL_09 !";
  app.modalConstants.emote[502] = "BOUCLE_CUSTOM_SPELL_10 !";
  app.modalConstants.emote[505] = "BOUCLE_En garde , fixe 1H_ALLOW_MOVEMENT";
  app.modalConstants.emote[506] = "BOUCLE_En garde , fixe 2H_ALLOW_MOVEMENT";
  app.modalConstants.emote[507] = "UNIQUE_attaque a main nue , moine";
  app.modalConstants.emote[508] = "UNIQUE_Lance sort / attaque moine sans arme";
  app.modalConstants.emote[509] = "UNIQUE_Pare sans arme , moine";
  app.modalConstants.emote[510] = "BOUCLE_En garde moine";
  app.modalConstants.emote[511] = "UNIQUE_Attaque a mais nue (boucle)";
  app.modalConstants.emote[512] = "BOUCLE_CRANE";
  app.modalConstants.emote[517] = "UNIQUE_Lire carte";
  app.modalConstants.emote[518] = "BOUCLE_READ_CHRISTMAS";
  app.modalConstants.emote[526] = "UNIQUE_Attaque , animation 2HL";
  app.modalConstants.emote[527] = "UNIQUE_Canalise lancer sort , fixe";
  app.modalConstants.emote[528] = "BOUCLE_FLY PRET SPELLDIRECTED";
  app.modalConstants.emote[531] = "BOUCLE_En garde fixe 1H";
  app.modalConstants.emote[533] = "BOUCLE_Meditation bouda en planant";
  app.modalConstants.emote[534] = "BOUCLE_en garde fixe a main nue";
  app.modalConstants.emote[535] = "UNIQUE_a terre";
  app.modalConstants.emote[536] = "UNIQUE_Courir sur place";
  app.modalConstants.emote[537] = "BOUCLE_ ATTAQUE/JETER";
  app.modalConstants.emote[538] = "BOUCLE_SPELL_CHANNEL_DIRECTED_NOSOUND";
  app.modalConstants.emote[539] = "UNIQUE_TRAVAIL (sans item , boucle)";
  app.modalConstants.emote[540] = "BOUCLE_PRET SANS ARME_NOSOUND";
  app.modalConstants.emote[543] = "UNIQUE_MONKOFFENSE_ATTAQUE SANS ARMEOFF";
  app.modalConstants.emote[546] = "RECLINED_assis PASSENGER";
  app.modalConstants.emote[547] = "UNIQUE_QUESTION ";
  app.modalConstants.emote[549] = "UNIQUE_lance sort canalise";
  app.modalConstants.emote[550] = "BOUCLE_S'AGENOUILLER";
  app.modalConstants.emote[551] = "UNIQUE_ATTAQUE SANS ARME";
  app.modalConstants.emote[552] = "UNIQUE_FLY COMBATBLESSURE";
  app.modalConstants.emote[553] = "UNIQUE_MONTURESELFSPECIAL !";
  app.modalConstants.emote[554] = "UNIQUE_ATTAQUE SANS ARME";
  app.modalConstants.emote[555] = "BOUCLE_Combat , encaisse coup";
  app.modalConstants.emote[556] = "UNIQUE_ ATTAQUE 1H ";
  app.modalConstants.emote[557] = "BOUCLE_MONTURE_SELF_IDLE";
  app.modalConstants.emote[558] = "UNIQUE_Marche sur place";
  app.modalConstants.emote[559] = "BOUCLE_OUVERT";
  app.modalConstants.emote[564] = "BOUCLE_CUSTOMSPELL03";
  app.modalConstants.emote[565] = "UNIQUE_Intimidation/Rugir";
  app.modalConstants.emote[567] = "BOUCLE_ ATTAQUE 1H";
  app.modalConstants.emote[568] = "BOUCLE_TRAVAIL_BUCHERON  (sans item)";
  app.modalConstants.emote[569] = "BOUCLE_UTILISATION SUPPLEMENTAIRE_LOOP";
  app.modalConstants.emote[572] = "BOUCLE_Travaille avec les mains";
  app.modalConstants.emote[573] = "UNIQUE_2 bras balancent verticalement";
  app.modalConstants.emote[574] = "UNIQUE_RIRE  ";
  app.modalConstants.emote[575] = "RECLINED_Assis pour conduire";
  app.modalConstants.emote[577] = "UNIQUE_ ATTAQUE 1H";
  app.modalConstants.emote[578] = "BOUCLE_Pleurer NOSOUND";
  app.modalConstants.emote[579] = "UNIQUE_Pleurer NOSOUND";
  app.modalConstants.emote[584] = "UNIQUE_COMBAT Coup reçu";
  app.modalConstants.emote[585] = "BOUCLE_TRAIN";
  app.modalConstants.emote[586] = "BOUCLE_TRAVAIL_BUCHERON (Avec item)";
  app.modalConstants.emote[587] = "UNIQUE_SPECIAL ATTAQUE 2H";
  app.modalConstants.emote[588] = "BOUCLE_Lire carte";
  app.modalConstants.emote[589] = "UNIQUE_SUPPORTER_VAR1 !";
  app.modalConstants.emote[590] = "REXXAR_STRANGLES_GOBLIN - montre du doigt";
  app.modalConstants.emote[591] = "UNIQUE_SUPPORTER_VAR2 !";
  app.modalConstants.emote[592] = "UNIQUE_Alonge au sol";
  app.modalConstants.emote[595] = "BOUCLE_PARLER ONCE";
  app.modalConstants.emote[596] = "BOUCLE_ ATTAQUE 2H (sans itemm)";
  app.modalConstants.emote[598] = "BOUCLE_EMOTE_ASSEOIR_GROUND !";
  app.modalConstants.emote[599] = "BOUCLE_TRAVAIL_BUCHERON (sans item)";
  app.modalConstants.emote[601] = "BOUCLE_CUSTOMSPELL01";
  app.modalConstants.emote[602] = "UNIQUE_COMBAT BLESSURE !";
  app.modalConstants.emote[603] = "UNIQUE_PARLER_EXCLAMATION";
  app.modalConstants.emote[604] = "UNIQUE_QUESTION ";
  app.modalConstants.emote[605] = "BOUCLE_Pleurer";
  app.modalConstants.emote[606] = "BOUCLE_Travaille avec les mains";
  app.modalConstants.emote[613] = "BOUCLE_TRAVAIL_FORGERON  (avec masse)";
  app.modalConstants.emote[614] = "BOUCLE_TRAVAIL_BUCHERON (sans item)";
  app.modalConstants.emote[615] = "BOUCLE_CUSTOMSPELL02";
  app.modalConstants.emote[616] = "BOUCLE_Assis et lire Carte";
  app.modalConstants.emote[619] = "BOUCLE_PARER  SANS ARMES";
  app.modalConstants.emote[620] = "BOUCLE_BLOQUER AVEC BOUCLIER";
  app.modalConstants.emote[621] = "BOUCLE_ASSEOIR_GROUND !";
  
  /* itemset constants */
  app.modalConstants.itemset = [];
  app.modalConstants.itemset[1] = "The Gladiator";
  app.modalConstants.itemset[41] = "Dal'Rend's Arms";
  app.modalConstants.itemset[65] = "Spider's Kiss";
  app.modalConstants.itemset[81] = "The Postmaster";
  app.modalConstants.itemset[121] = "Cadaverous Garb";
  app.modalConstants.itemset[122] = "Necropile Raiment";
  app.modalConstants.itemset[123] = "Bloodmail Regalia";
  app.modalConstants.itemset[124] = "Deathbone Guardian";
  app.modalConstants.itemset[141] = "Volcanic Armor";
  app.modalConstants.itemset[142] = "Stormshroud Armor";
  app.modalConstants.itemset[143] = "Devilsaur Armor";
  app.modalConstants.itemset[144] = "Ironfeather Armor";
  app.modalConstants.itemset[161] = "defias Leather";
  app.modalConstants.itemset[162] = "Embrace of the Viper";
  app.modalConstants.itemset[163] = "Chain of the Scarlet Crusade";
  app.modalConstants.itemset[181] = "Magister's Regalia";
  app.modalConstants.itemset[182] = "Vestments of the Devout";
  app.modalConstants.itemset[183] = "Dreadmist Raiment";
  app.modalConstants.itemset[184] = "Shadowcraft Armor";
  app.modalConstants.itemset[185] = "Wildheart Raiment";
  app.modalConstants.itemset[186] = "Beaststalker Armor";
  app.modalConstants.itemset[187] = "The Elements";
  app.modalConstants.itemset[188] = "Lightforge Armor";
  app.modalConstants.itemset[189] = "Battlegear of Valor";
  app.modalConstants.itemset[201] = "Arcanist Regalia";
  app.modalConstants.itemset[202] = "Vestments of Prophecy";
  app.modalConstants.itemset[203] = "Felheart Raiment";
  app.modalConstants.itemset[204] = "Nightslayer Armor";
  app.modalConstants.itemset[205] = "Cenarion Raiment";
  app.modalConstants.itemset[206] = "Giantstalker Armor";
  app.modalConstants.itemset[207] = "The Earthfury";
  app.modalConstants.itemset[208] = "Lawbringer Armor";
  app.modalConstants.itemset[209] = "Battlegear of Might";
  app.modalConstants.itemset[210] = "Netherwind Regalia";
  app.modalConstants.itemset[211] = "Vestments of Transcendence";
  app.modalConstants.itemset[212] = "Nemesis Raiment";
  app.modalConstants.itemset[213] = "Bloodfang Armor";
  app.modalConstants.itemset[214] = "Stormrage Raiment";
  app.modalConstants.itemset[215] = "Dragonstalker Armor";
  app.modalConstants.itemset[216] = "The Ten Storms";
  app.modalConstants.itemset[217] = "Judgement Armor";
  app.modalConstants.itemset[218] = "Battlegear of Wrath";
  app.modalConstants.itemset[221] = "Garb of Thero-shan";
  app.modalConstants.itemset[241] = "Shard of the Gods";
  app.modalConstants.itemset[261] = "Spirit of Eskhandar";
  app.modalConstants.itemset[281] = "Champion's Battlegear";
  app.modalConstants.itemset[282] = "Lieutenant Commander's Battlegear";
  app.modalConstants.itemset[301] = "Champion's Earthshaker";
  app.modalConstants.itemset[321] = "Imperial Plate";
  app.modalConstants.itemset[341] = "Champion's Regalia";
  app.modalConstants.itemset[342] = "Champion's Raiment";
  app.modalConstants.itemset[343] = "Lieutenant Commander's Regalia";
  app.modalConstants.itemset[344] = "Lieutenant Commander's Raiment";
  app.modalConstants.itemset[345] = "Champion's Threads";
  app.modalConstants.itemset[346] = "Lieutenant Commander's Threads";
  app.modalConstants.itemset[347] = "Champion's Vestments";
  app.modalConstants.itemset[348] = "Lieutenant Commander's Vestments";
  app.modalConstants.itemset[361] = "Champion's Pursuit";
  app.modalConstants.itemset[362] = "Lieutenant Commander's Pursuit";
  app.modalConstants.itemset[381] = "Lieutenant Commander's Sanctuary";
  app.modalConstants.itemset[382] = "Champion's Sanctuary";
  app.modalConstants.itemset[383] = "Warlord's Battlegear";
  app.modalConstants.itemset[384] = "Field Marshal's Battlegear";
  app.modalConstants.itemset[386] = "Warlord's Earthshaker";
  app.modalConstants.itemset[387] = "Warlord's Regalia";
  app.modalConstants.itemset[388] = "Field Marshal's Regalia";
  app.modalConstants.itemset[389] = "Field Marshal's Raiment";
  app.modalConstants.itemset[390] = "Warlord's Raiment";
  app.modalConstants.itemset[391] = "Warlord's Threads";
  app.modalConstants.itemset[392] = "Field Marshal's Threads";
  app.modalConstants.itemset[393] = "Warlord's Vestments";
  app.modalConstants.itemset[394] = "Field Marshal's Vestments";
  app.modalConstants.itemset[395] = "Field Marshal's Pursuit";
  app.modalConstants.itemset[396] = "Warlord's Pursuit";
  app.modalConstants.itemset[397] = "Field Marshal's Sanctuary";
  app.modalConstants.itemset[398] = "Warlord's Sanctuary";
  app.modalConstants.itemset[401] = "Lieutenant Commander's Aegis";
  app.modalConstants.itemset[402] = "Field Marshal's Aegis";
  app.modalConstants.itemset[421] = "Bloodvine Garb";
  app.modalConstants.itemset[441] = "Primal Batskin";
  app.modalConstants.itemset[442] = "Blood Tiger Harness";
  app.modalConstants.itemset[443] = "Bloodsoul Embrace";
  app.modalConstants.itemset[444] = "The Darksoul";
  app.modalConstants.itemset[461] = "The Twin Blades of Hakkari";
  app.modalConstants.itemset[462] = "Zanzil's Concentration";
  app.modalConstants.itemset[463] = "Primal Blessing";
  app.modalConstants.itemset[464] = "Overlord's Resolution";
  app.modalConstants.itemset[465] = "Prayer of the Primal";
  app.modalConstants.itemset[466] = "Major Mojo Infusion";
  app.modalConstants.itemset[467] = "The Highlander's Resolution";
  app.modalConstants.itemset[468] = "The Highlander's Resolve";
  app.modalConstants.itemset[469] = "The Highlander's Determination";
  app.modalConstants.itemset[470] = "The Highlander's Fortitude";
  app.modalConstants.itemset[471] = "The Highlander's Purpose";
  app.modalConstants.itemset[472] = "The Highlander's Will";
  app.modalConstants.itemset[473] = "The Highlander's Intent";
  app.modalConstants.itemset[474] = "Vindicator's Battlegear";
  app.modalConstants.itemset[475] = "Freethinker's Armor";
  app.modalConstants.itemset[476] = "Augur's Regalia";
  app.modalConstants.itemset[477] = "Predator's Armor";
  app.modalConstants.itemset[478] = "Madcap's Outfit";
  app.modalConstants.itemset[479] = "Haruspex's Garb";
  app.modalConstants.itemset[480] = "Confessor's Raiment";
  app.modalConstants.itemset[481] = "Demoniac's Threads";
  app.modalConstants.itemset[482] = "Illusionist's Attire";
  app.modalConstants.itemset[483] = "The Defiler's Determination";
  app.modalConstants.itemset[484] = "The Defiler's Fortitude";
  app.modalConstants.itemset[485] = "The Defiler's Intent";
  app.modalConstants.itemset[486] = "The Defiler's Purpose";
  app.modalConstants.itemset[487] = "The Defiler's Resolution";
  app.modalConstants.itemset[488] = "The Defiler's Will";
  app.modalConstants.itemset[489] = "Black Dragon Mail";
  app.modalConstants.itemset[490] = "Green Dragon Mail";
  app.modalConstants.itemset[491] = "Blue Dragon Mail";
  app.modalConstants.itemset[492] = "Twilight Trappings";
  app.modalConstants.itemset[493] = "Genesis Raiment";
  app.modalConstants.itemset[494] = "Symbols of Unending Life";
  app.modalConstants.itemset[495] = "Battlegear of Unyielding Strength";
  app.modalConstants.itemset[496] = "Conqueror's Battlegear";
  app.modalConstants.itemset[497] = "Deathdealer's Embrace";
  app.modalConstants.itemset[498] = "Emblems of Veiled Shadows";
  app.modalConstants.itemset[499] = "Doomcaller's Attire";
  app.modalConstants.itemset[500] = "Implements of Unspoken Names";
  app.modalConstants.itemset[501] = "Stormcaller's Garb";
  app.modalConstants.itemset[502] = "Gift of the Gathering Storm";
  app.modalConstants.itemset[503] = "Enigma Vestments";
  app.modalConstants.itemset[504] = "Trappings of Vaulted Secrets";
  app.modalConstants.itemset[505] = "Avenger's Battlegear";
  app.modalConstants.itemset[506] = "Battlegear of Eternal Justice";
  app.modalConstants.itemset[507] = "Garments of the Oracle";
  app.modalConstants.itemset[508] = "Finery of Infinite Wisdom";
  app.modalConstants.itemset[509] = "Striker's Garb";
  app.modalConstants.itemset[510] = "Trappings of the Unseen Path";
  app.modalConstants.itemset[511] = "Battlegear of Heroism";
  app.modalConstants.itemset[512] = "Darkmantle Armor";
  app.modalConstants.itemset[513] = "Feralheart Raiment";
  app.modalConstants.itemset[514] = "Vestments of the Virtuous";
  app.modalConstants.itemset[515] = "Beastmaster Armor";
  app.modalConstants.itemset[516] = "Soulforge Armor";
  app.modalConstants.itemset[517] = "Sorcerer's Regalia";
  app.modalConstants.itemset[518] = "Deathmist Raiment";
  app.modalConstants.itemset[519] = "The Five Thunders";
  app.modalConstants.itemset[520] = "Ironweave Battlesuit";
  app.modalConstants.itemset[521] = "Dreamwalker Raiment";
  app.modalConstants.itemset[522] = "Champion's Guard";
  app.modalConstants.itemset[523] = "Dreadnaught's Battlegear";
  app.modalConstants.itemset[524] = "Bonescythe Armor";
  app.modalConstants.itemset[525] = "Vestments of Faith";
  app.modalConstants.itemset[526] = "Frostfire Regalia";
  app.modalConstants.itemset[527] = "The Earthshatterer";
  app.modalConstants.itemset[528] = "Redemption Armor";
  app.modalConstants.itemset[529] = "Plagueheart Raiment";
  app.modalConstants.itemset[530] = "Cryptstalker Armor";
  app.modalConstants.itemset[533] = "Battlegear of Undead Slaying";
  app.modalConstants.itemset[534] = "Undead Slayer's Armor";
  app.modalConstants.itemset[535] = "Garb of the Undead Slayer";
  app.modalConstants.itemset[536] = "Regalia of Undead Cleansing";
  app.modalConstants.itemset[537] = "Champion's Battlearmor";
  app.modalConstants.itemset[538] = "Champion's Stormcaller";
  app.modalConstants.itemset[539] = "Champion's Refuge";
  app.modalConstants.itemset[540] = "Champion's Investiture";
  app.modalConstants.itemset[541] = "Champion's Dreadgear";
  app.modalConstants.itemset[542] = "Champion's Arcanum";
  app.modalConstants.itemset[543] = "Champion's Pursuance";
  app.modalConstants.itemset[544] = "Lieutenant Commander's Redoubt";
  app.modalConstants.itemset[545] = "Lieutenant Commander's Battlearmor";
  app.modalConstants.itemset[546] = "Lieutenant Commander's Arcanum";
  app.modalConstants.itemset[547] = "Lieutenant Commander's Dreadgear";
  app.modalConstants.itemset[548] = "Lieutenant Commander's Guard";
  app.modalConstants.itemset[549] = "Lieutenant Commander's Investiture";
  app.modalConstants.itemset[550] = "Lieutenant Commander's Pursuance";
  app.modalConstants.itemset[551] = "Lieutenant Commander's Refuge";
  app.modalConstants.itemset[552] = "Wrath of Spellfire";
  app.modalConstants.itemset[553] = "Shadow's Embrace";
  app.modalConstants.itemset[554] = "Primal Mooncloth";
  app.modalConstants.itemset[555] = "Netherweave Vestments";
  app.modalConstants.itemset[556] = "Imbued Netherweave";
  app.modalConstants.itemset[557] = "Soulcloth Embrace";
  app.modalConstants.itemset[558] = "Arcanoweave Vestments";
  app.modalConstants.itemset[559] = "Spellstrike Infusion";
  app.modalConstants.itemset[560] = "Fel Iron Plate";
  app.modalConstants.itemset[561] = "Fel Iron Chain";
  app.modalConstants.itemset[562] = "Adamantite Battlegear";
  app.modalConstants.itemset[563] = "Enchanted Adamantite Armor";
  app.modalConstants.itemset[564] = "Flame Guard";
  app.modalConstants.itemset[565] = "Khorium Ward";
  app.modalConstants.itemset[566] = "Burning Rage";
  app.modalConstants.itemset[567] = "Gladiator's Battlegear";
  app.modalConstants.itemset[568] = "Gladiator's Dreadgear";
  app.modalConstants.itemset[569] = "Faith in Felsteel";
  app.modalConstants.itemset[570] = "The Unyielding";
  app.modalConstants.itemset[571] = "Whitemend Wisdom";
  app.modalConstants.itemset[572] = "Battlecast Garb";
  app.modalConstants.itemset[573] = "Fel Skin";
  app.modalConstants.itemset[574] = "Strength of the Clefthoof";
  app.modalConstants.itemset[575] = "Felstalker Armor";
  app.modalConstants.itemset[576] = "Fury of the Nether";
  app.modalConstants.itemset[577] = "Gladiator's Vestments";
  app.modalConstants.itemset[578] = "Gladiator's Earthshaker";
  app.modalConstants.itemset[579] = "Gladiator's Regalia";
  app.modalConstants.itemset[580] = "Gladiator's Thunderfist";
  app.modalConstants.itemset[581] = "Gladiator's Raiment";
  app.modalConstants.itemset[582] = "Gladiator's Aegis";
  app.modalConstants.itemset[583] = "Gladiator's Vindication";
  app.modalConstants.itemset[584] = "Gladiator's Sanctuary";
  app.modalConstants.itemset[585] = "Gladiator's Wildhide";
  app.modalConstants.itemset[586] = "Gladiator's Pursuit";
  app.modalConstants.itemset[587] = "High Warlord's Aegis";
  app.modalConstants.itemset[588] = "High Warlord's Battlegear";
  app.modalConstants.itemset[589] = "Grand Marshal's Aegis";
  app.modalConstants.itemset[590] = "Grand Marshal's Battlegear";
  app.modalConstants.itemset[591] = "Grand Marshal's Dreadgear";
  app.modalConstants.itemset[592] = "High Warlord's Dreadgear";
  app.modalConstants.itemset[593] = "Grand Marshal's Earthshaker";
  app.modalConstants.itemset[594] = "High Warlord's Earthshaker";
  app.modalConstants.itemset[595] = "Grand Marshal's Pursuit";
  app.modalConstants.itemset[596] = "High Warlord's Pursuit";
  app.modalConstants.itemset[597] = "Grand Marshal's Raiment";
  app.modalConstants.itemset[598] = "High Warlord's Raiment";
  app.modalConstants.itemset[599] = "Grand Marshal's Regalia";
  app.modalConstants.itemset[600] = "High Warlord's Regalia";
  app.modalConstants.itemset[601] = "Grand Marshal's Sanctuary";
  app.modalConstants.itemset[602] = "High Warlord's Sanctuary";
  app.modalConstants.itemset[603] = "Grand Marshal's Thunderfist";
  app.modalConstants.itemset[604] = "High Warlord's Thunderfist";
  app.modalConstants.itemset[605] = "Grand Marshal's Vestments";
  app.modalConstants.itemset[606] = "High Warlord's Vestments";
  app.modalConstants.itemset[607] = "Grand Marshal's Vindication";
  app.modalConstants.itemset[608] = "High Warlord's Vindication";
  app.modalConstants.itemset[609] = "Grand Marshal's Wildhide";
  app.modalConstants.itemset[610] = "High Warlord's Wildhide";
  app.modalConstants.itemset[611] = "Felscale Armor";
  app.modalConstants.itemset[612] = "Scaled Draenic Armor";
  app.modalConstants.itemset[613] = "Thick Draenic Armor";
  app.modalConstants.itemset[614] = "Wild Draenish Armor";
  app.modalConstants.itemset[615] = "Gladiator's Felshroud";
  app.modalConstants.itemset[616] = "Netherscale Armor";
  app.modalConstants.itemset[617] = "Netherstrike Armor";
  app.modalConstants.itemset[618] = "Windhawk Armor";
  app.modalConstants.itemset[619] = "Primal Intent";
  app.modalConstants.itemset[620] = "Assassination Armor";
  app.modalConstants.itemset[621] = "Netherblade";
  app.modalConstants.itemset[622] = "Deathmantle";
  app.modalConstants.itemset[623] = "Righteous Armor";
  app.modalConstants.itemset[624] = "Justicar Raiment";
  app.modalConstants.itemset[625] = "Justicar Armor";
  app.modalConstants.itemset[626] = "Justicar Battlegear";
  app.modalConstants.itemset[627] = "Crystalforge Raiment";
  app.modalConstants.itemset[628] = "Crystalforge Armor";
  app.modalConstants.itemset[629] = "Crystalforge Battlegear";
  app.modalConstants.itemset[630] = "Tidefury Raiment";
  app.modalConstants.itemset[631] = "Cyclone Raiment";
  app.modalConstants.itemset[632] = "Cyclone Regalia";
  app.modalConstants.itemset[633] = "Cyclone Harness";
  app.modalConstants.itemset[634] = "Cataclysm Raiment";
  app.modalConstants.itemset[635] = "Cataclysm Regalia";
  app.modalConstants.itemset[636] = "Cataclysm Harness";
  app.modalConstants.itemset[637] = "Moonglade Raiment";
  app.modalConstants.itemset[638] = "Malorne Raiment";
  app.modalConstants.itemset[639] = "Malorne Regalia";
  app.modalConstants.itemset[640] = "Malorne Harness";
  app.modalConstants.itemset[641] = "Nordrassil Harness";
  app.modalConstants.itemset[642] = "Nordrassil Raiment";
  app.modalConstants.itemset[643] = "Nordrassil Regalia";
  app.modalConstants.itemset[644] = "Oblivion Raiment";
  app.modalConstants.itemset[645] = "Voidheart Raiment";
  app.modalConstants.itemset[646] = "Corruptor Raiment";
  app.modalConstants.itemset[647] = "Incanter's Regalia";
  app.modalConstants.itemset[648] = "Aldor Regalia";
  app.modalConstants.itemset[649] = "Tirisfal Regalia";
  app.modalConstants.itemset[650] = "Beast Lord Armor";
  app.modalConstants.itemset[651] = "Demon Stalker Armor";
  app.modalConstants.itemset[652] = "Rift Stalker Armor";
  app.modalConstants.itemset[653] = "Bold Armor";
  app.modalConstants.itemset[654] = "Warbringer Armor";
  app.modalConstants.itemset[655] = "Warbringer Battlegear";
  app.modalConstants.itemset[656] = "Destroyer Armor";
  app.modalConstants.itemset[657] = "Destroyer Battlegear";
  app.modalConstants.itemset[658] = "Mana-Etched Regalia";
  app.modalConstants.itemset[659] = "Wastewalker Armor";
  app.modalConstants.itemset[660] = "Desolation Battlegear";
  app.modalConstants.itemset[661] = "Doomplate Battlegear";
  app.modalConstants.itemset[662] = "Hallowed Raiment";
  app.modalConstants.itemset[663] = "Incarnate Raiment";
  app.modalConstants.itemset[664] = "Incarnate Regalia";
  app.modalConstants.itemset[665] = "Avatar Raiment";
  app.modalConstants.itemset[666] = "Avatar Regalia";
  app.modalConstants.itemset[667] = "The Twin Stars";
  app.modalConstants.itemset[668] = "Slayer's Armor";
  app.modalConstants.itemset[669] = "Gronnstalker's Armor";
  app.modalConstants.itemset[670] = "Malefic Raiment";
  app.modalConstants.itemset[671] = "Tempest Regalia";
  app.modalConstants.itemset[672] = "Onslaught Battlegear";
  app.modalConstants.itemset[673] = "Onslaught Armor";
  app.modalConstants.itemset[674] = "Absolution Regalia";
  app.modalConstants.itemset[675] = "Vestments of Absolution";
  app.modalConstants.itemset[676] = "Thunderheart Harness";
  app.modalConstants.itemset[677] = "Thunderheart Regalia";
  app.modalConstants.itemset[678] = "Thunderheart Raiment";
  app.modalConstants.itemset[679] = "Lightbringer Armor";
  app.modalConstants.itemset[680] = "Lightbringer Battlegear";
  app.modalConstants.itemset[681] = "Lightbringer Raiment";
  app.modalConstants.itemset[682] = "Skyshatter Harness";
  app.modalConstants.itemset[683] = "Skyshatter Raiment";
  app.modalConstants.itemset[684] = "Skyshatter Regalia";
  app.modalConstants.itemset[685] = "Gladiator's Refuge";
  app.modalConstants.itemset[686] = "Gladiator's Wartide";
  app.modalConstants.itemset[687] = "Gladiator's Investiture";
  app.modalConstants.itemset[688] = "Grand Marshal's Refuge";
  app.modalConstants.itemset[689] = "High Warlord's Refuge";
  app.modalConstants.itemset[690] = "Gladiator's Redemption";
  app.modalConstants.itemset[691] = "Grand Marshal's Investiture";
  app.modalConstants.itemset[692] = "High Warlord's Investiture";
  app.modalConstants.itemset[693] = "Grand Marshal's Redemption";
  app.modalConstants.itemset[694] = "High Warlord's Redemption";
  app.modalConstants.itemset[695] = "Grand Marshal's Wartide";
  app.modalConstants.itemset[696] = "High Warlord's Wartide";
  app.modalConstants.itemset[697] = "Champion's Redoubt";
  app.modalConstants.itemset[698] = "Warlord's Aegis";
  app.modalConstants.itemset[699] = "The Twin Blades of Azzinoth";
  app.modalConstants.itemset[700] = "Merciless Gladiator's Aegis";
  app.modalConstants.itemset[701] = "Merciless Gladiator's Battlegear";
  app.modalConstants.itemset[702] = "Merciless Gladiator's Dreadgear";
  app.modalConstants.itemset[703] = "Merciless Gladiator's Earthshaker";
  app.modalConstants.itemset[704] = "Merciless Gladiator's Felshroud";
  app.modalConstants.itemset[705] = "Merciless Gladiator's Investiture";
  app.modalConstants.itemset[706] = "Merciless Gladiator's Pursuit";
  app.modalConstants.itemset[707] = "Merciless Gladiator's Raiment";
  app.modalConstants.itemset[708] = "Merciless Gladiator's Redemption";
  app.modalConstants.itemset[709] = "Merciless Gladiator's Refuge";
  app.modalConstants.itemset[710] = "Merciless Gladiator's Regalia";
  app.modalConstants.itemset[711] = "Merciless Gladiator's Sanctuary";
  app.modalConstants.itemset[712] = "Merciless Gladiator's Thunderfist";
  app.modalConstants.itemset[713] = "Merciless Gladiator's Vestments";
  app.modalConstants.itemset[714] = "Merciless Gladiator's Vindication";
  app.modalConstants.itemset[715] = "Merciless Gladiator's Wartide";
  app.modalConstants.itemset[716] = "Merciless Gladiator's Wildhide";
  app.modalConstants.itemset[717] = "Field Marshal's Earthshaker";
  app.modalConstants.itemset[718] = "Lieutenant Commander's Earthshaker";
  app.modalConstants.itemset[719] = "The Fists of Fury";
  app.modalConstants.itemset[720] = "Vengeful Gladiator's Refuge";
  app.modalConstants.itemset[721] = "Vengeful Gladiator's Sanctuary";
  app.modalConstants.itemset[722] = "Vengeful Gladiator's Wildhide";
  app.modalConstants.itemset[723] = "Vengeful Gladiator's Pursuit";
  app.modalConstants.itemset[724] = "Vengeful Gladiator's Regalia";
  app.modalConstants.itemset[725] = "Vengeful Gladiator's Redemption";
  app.modalConstants.itemset[726] = "Vengeful Gladiator's Vindication";
  app.modalConstants.itemset[727] = "Vengeful Gladiator's Aegis";
  app.modalConstants.itemset[728] = "Vengeful Gladiator's Investiture";
  app.modalConstants.itemset[729] = "Vengeful Gladiator's Raiment";
  app.modalConstants.itemset[730] = "Vengeful Gladiator's Vestments";
  app.modalConstants.itemset[731] = "Vengeful Gladiator's Wartide";
  app.modalConstants.itemset[732] = "Vengeful Gladiator's Earthshaker";
  app.modalConstants.itemset[733] = "Vengeful Gladiator's Thunderfist";
  app.modalConstants.itemset[734] = "Vengeful Gladiator's Dreadgear";
  app.modalConstants.itemset[735] = "Vengeful Gladiator's Felshroud";
  app.modalConstants.itemset[736] = "Vengeful Gladiator's Battlegear";
  app.modalConstants.itemset[737] = "Latro's Flurry";
  app.modalConstants.itemset[738] = "Dreadweave Battlegear";
  app.modalConstants.itemset[739] = "Mooncloth Battlegear";
  app.modalConstants.itemset[740] = "Satin Battlegear";
  app.modalConstants.itemset[741] = "Evoker's Silk Battlegear";
  app.modalConstants.itemset[742] = "Dragonhide Battlegear";
  app.modalConstants.itemset[743] = "Wyrmhide Battlegear";
  app.modalConstants.itemset[744] = "Kodohide Battlegear";
  app.modalConstants.itemset[745] = "Opportunist's Battlegear";
  app.modalConstants.itemset[746] = "Seer's Mail Battlegear";
  app.modalConstants.itemset[747] = "Seer's Ringmail Battlegear";
  app.modalConstants.itemset[748] = "Seer's Linked Battlegear";
  app.modalConstants.itemset[749] = "Stalker's Chain Battlegear";
  app.modalConstants.itemset[750] = "Savage Plate Battlegear";
  app.modalConstants.itemset[751] = "Crusader's Ornamented Battlegear";
  app.modalConstants.itemset[752] = "Crusader's Scaled Battlegear";
  app.modalConstants.itemset[754] = "Borean Embrace";
  app.modalConstants.itemset[755] = "Nerubian Hive";
  app.modalConstants.itemset[756] = "Frostscale Binding";
  app.modalConstants.itemset[757] = "Iceborne Embrace";
  app.modalConstants.itemset[759] = "Raine's Revenge";
  app.modalConstants.itemset[760] = "Gladiator's Dreadplate";
  app.modalConstants.itemset[761] = "Winter Garb";
  app.modalConstants.itemset[762] = "Brewfest Garb";
  app.modalConstants.itemset[763] = "Frostwoven Power";
  app.modalConstants.itemset[764] = "Duskweaver";
  app.modalConstants.itemset[765] = "Gladiator's Battlegear";
  app.modalConstants.itemset[766] = "Gladiator's Vindication";
  app.modalConstants.itemset[767] = "Gladiator's Redemption";
  app.modalConstants.itemset[768] = "Gladiator's Desecration";
  app.modalConstants.itemset[769] = "Gladiator's Thunderfist";
  app.modalConstants.itemset[770] = "Gladiator's Earthshaker";
  app.modalConstants.itemset[771] = "Gladiator's Wartide";
  app.modalConstants.itemset[772] = "Gladiator's Pursuit";
  app.modalConstants.itemset[773] = "Gladiator's Refuge";
  app.modalConstants.itemset[774] = "Gladiator's Wildhide";
  app.modalConstants.itemset[775] = "Gladiator's Sanctuary";
  app.modalConstants.itemset[776] = "Gladiator's Vestments";
  app.modalConstants.itemset[777] = "Gladiator's Investiture";
  app.modalConstants.itemset[778] = "Gladiator's Raiment";
  app.modalConstants.itemset[779] = "Gladiator's Regalia";
  app.modalConstants.itemset[780] = "Gladiator's Felshroud";
  app.modalConstants.itemset[781] = "Blessed Regalia of Undead Cleansing";
  app.modalConstants.itemset[782] = "Undead Slayer's Blessed Armor";
  app.modalConstants.itemset[783] = "Blessed Garb of the Undead Slayer";
  app.modalConstants.itemset[784] = "Blessed Battlegear of Undead Slaying";
  app.modalConstants.itemset[785] = "Midsummer Reveler";
  app.modalConstants.itemset[787] = "Dreadnaught Plate";
  app.modalConstants.itemset[788] = "Dreadnaught Battlegear";
  app.modalConstants.itemset[789] = "Redemption Battlegear";
  app.modalConstants.itemset[790] = "Redemption Regalia";
  app.modalConstants.itemset[791] = "Redemption Plate";
  app.modalConstants.itemset[792] = "Scourgeborne Battlegear";
  app.modalConstants.itemset[793] = "Scourgeborne Plate";
  app.modalConstants.itemset[794] = "Cryptstalker Battlegear";
  app.modalConstants.itemset[795] = "Earthshatter Battlegear";
  app.modalConstants.itemset[796] = "Earthshatter Garb";
  app.modalConstants.itemset[797] = "Earthshatter Regalia";
  app.modalConstants.itemset[798] = "Dreamwalker Battlegear";
  app.modalConstants.itemset[799] = "Dreamwalker Regalia";
  app.modalConstants.itemset[800] = "Dreamwalker Garb";
  app.modalConstants.itemset[801] = "Bonescythe Battlegear";
  app.modalConstants.itemset[802] = "Plagueheart Garb";
  app.modalConstants.itemset[803] = "Frostfire Garb";
  app.modalConstants.itemset[804] = "Regalia of Faith";
  app.modalConstants.itemset[805] = "Garb of Faith";
  app.modalConstants.itemset[812] = "Spring Tuxedo";
  app.modalConstants.itemset[813] = "Eviscerator's Battlegear";
  app.modalConstants.itemset[814] = "Ornate Saronite Battlegear";
  app.modalConstants.itemset[815] = "Overcaster Battlegear";
  app.modalConstants.itemset[816] = "Savage Saronite Battlegear";
  app.modalConstants.itemset[817] = "Stormhide Battlegear";
  app.modalConstants.itemset[818] = "Swiftarrow Battlegear";
  app.modalConstants.itemset[819] = "Frostsavage Battlegear";
  app.modalConstants.itemset[820] = "Aegis Battlegear";
  app.modalConstants.itemset[821] = "Aegis Plate";
  app.modalConstants.itemset[822] = "Aegis Regalia";
  app.modalConstants.itemset[823] = "Worldbreaker Battlegear";
  app.modalConstants.itemset[824] = "Worldbreaker Garb";
  app.modalConstants.itemset[825] = "Worldbreaker Regalia";
  app.modalConstants.itemset[826] = "Terrorblade Battlegear";
  app.modalConstants.itemset[827] = "Nightsong Battlegear";
  app.modalConstants.itemset[828] = "Nightsong Garb";
  app.modalConstants.itemset[829] = "Nightsong Regalia";
  app.modalConstants.itemset[830] = "Siegebreaker Battlegear";
  app.modalConstants.itemset[831] = "Siegebreaker Plate";
  app.modalConstants.itemset[832] = "Sanctification Garb";
  app.modalConstants.itemset[833] = "Sanctification Regalia";
  app.modalConstants.itemset[834] = "Darkruned Battlegear";
  app.modalConstants.itemset[835] = "Darkruned Plate";
  app.modalConstants.itemset[836] = "Kirin Tor Garb";
  app.modalConstants.itemset[837] = "Deathbringer Garb";
  app.modalConstants.itemset[838] = "Scourgestalker Battlegear";
  app.modalConstants.itemset[843] = "Khadgar's Regalia";
  app.modalConstants.itemset[844] = "Sunstrider's Regalia";
  app.modalConstants.itemset[845] = "Gul'dan's Regalia";
  app.modalConstants.itemset[846] = "Kel'Thuzad's Regalia";
  app.modalConstants.itemset[847] = "Velen's Raiment";
  app.modalConstants.itemset[848] = "Zabra's Raiment";
  app.modalConstants.itemset[849] = "Velen's Regalia";
  app.modalConstants.itemset[850] = "Zabra's Regalia";
  app.modalConstants.itemset[851] = "Malfurion's Garb";
  app.modalConstants.itemset[852] = "Runetotem's Garb";
  app.modalConstants.itemset[853] = "Malfurion's Regalia";
  app.modalConstants.itemset[854] = "Runetotem's Regalia";
  app.modalConstants.itemset[855] = "Malfurion's Battlegear";
  app.modalConstants.itemset[856] = "Runetotem's Battlegear";
  app.modalConstants.itemset[857] = "VanCleef's Battlegear";
  app.modalConstants.itemset[858] = "Garona's Battlegear";
  app.modalConstants.itemset[859] = "Windrunner's Battlegear";
  app.modalConstants.itemset[860] = "Windrunner's Pursuit";
  app.modalConstants.itemset[861] = "Nobundo's Garb";
  app.modalConstants.itemset[862] = "Thrall's Garb";
  app.modalConstants.itemset[863] = "Thrall's Regalia";
  app.modalConstants.itemset[864] = "Nobundo's Regalia";
  app.modalConstants.itemset[865] = "Nobundo's Battlegear";
  app.modalConstants.itemset[866] = "Thrall's Battlegear";
  app.modalConstants.itemset[867] = "Wrynn's Battlegear";
  app.modalConstants.itemset[868] = "Hellscream's Battlegear";
  app.modalConstants.itemset[869] = "Wrynn's Plate";
  app.modalConstants.itemset[870] = "Hellscream's Plate";
  app.modalConstants.itemset[871] = "Thassarian's Battlegear";
  app.modalConstants.itemset[872] = "Koltira's Battlegear";
  app.modalConstants.itemset[873] = "Thassarian's Plate";
  app.modalConstants.itemset[874] = "Koltira's Plate";
  app.modalConstants.itemset[875] = "Turalyon's Garb";
  app.modalConstants.itemset[876] = "Liadrin's Garb";
  app.modalConstants.itemset[877] = "Turalyon's Battlegear";
  app.modalConstants.itemset[878] = "Liadrin's Battlegear";
  app.modalConstants.itemset[879] = "Turalyon's Plate";
  app.modalConstants.itemset[880] = "Liadrin's Plate";
  app.modalConstants.itemset[881] = "Purified Shard of the Gods";
  app.modalConstants.itemset[882] = "Shiny Shard of the Gods";
  app.modalConstants.itemset[883] = "Bloodmage's Regalia";
  app.modalConstants.itemset[884] = "Dark Coven's Regalia";
  app.modalConstants.itemset[885] = "Crimson Acolyte's Raiment";
  app.modalConstants.itemset[886] = "Crimson Acolyte's Regalia";
  app.modalConstants.itemset[887] = "Lasherweave Garb";
  app.modalConstants.itemset[888] = "Lasherweave Regalia";
  app.modalConstants.itemset[889] = "Lasherweave Battlegear";
  app.modalConstants.itemset[890] = "Shadowblade's Battlegear";
  app.modalConstants.itemset[891] = "Ahn'Kahar Blood Hunter's Battlegear";
  app.modalConstants.itemset[892] = "Frost Witch's Garb";
  app.modalConstants.itemset[893] = "Frost Witch's Regalia";
  app.modalConstants.itemset[894] = "Frost Witch's Battlegear";
  app.modalConstants.itemset[895] = "Ymirjar Lord's Battlegear";
  app.modalConstants.itemset[896] = "Ymirjar Lord's Plate";
  app.modalConstants.itemset[897] = "Scourgelord's Battlegear";
  app.modalConstants.itemset[898] = "Scourgelord's Plate";
  app.modalConstants.itemset[899] = "Lightsworn Garb";
  app.modalConstants.itemset[900] = "Lightsworn Battlegear";
  app.modalConstants.itemset[901] = "Lightsworn Plate";

  /* item extendend cost */
  app.modalConstants.ExtendedCost       = [];
  app.modalConstants.ExtendedCost[1]    = "13 honor pts, 9 arena pts, Tough Jerky (id=117), 2 Minor Healing Potion (id=118)";
  app.modalConstants.ExtendedCost[2]    = "0";
  app.modalConstants.ExtendedCost[5]    = "40 Halaa Battle Token (id=26045), 2 Halaa Research Token (id=26044)";
  app.modalConstants.ExtendedCost[6]    = "20 Halaa Battle Token (id=26045), Halaa Research Token (id=26044)";
  app.modalConstants.ExtendedCost[7]    = "8 Halaa Research Token (id=26044)";
  app.modalConstants.ExtendedCost[18]   = "100 Halaa Battle Token (id=26045)";
  app.modalConstants.ExtendedCost[21]   = "978 arena pts";
  app.modalConstants.ExtendedCost[22]   = "1630 arena pts";
  app.modalConstants.ExtendedCost[24]   = "1304 arena pts";
  app.modalConstants.ExtendedCost[26]   = "3261 arena pts";
  app.modalConstants.ExtendedCost[53]   = "5 Mark of Honor Hold (id=24579)";
  app.modalConstants.ExtendedCost[54]   = "5 Mark of Thrallmar (id=24581)";
  app.modalConstants.ExtendedCost[55]   = "10 Mark of Thrallmar (id=24581)";
  app.modalConstants.ExtendedCost[56]   = "10 Mark of Honor Hold (id=24579)";
  app.modalConstants.ExtendedCost[65]   = "15 Mark of Thrallmar (id=24581)";
  app.modalConstants.ExtendedCost[66]   = "1 honor pts, 1 arena pts";
  app.modalConstants.ExtendedCost[69]   = "15 Mark of Honor Hold (id=24579)";
  app.modalConstants.ExtendedCost[76]   = "30 Mark of Thrallmar (id=24581)";
  app.modalConstants.ExtendedCost[77]   = "30 Mark of Honor Hold (id=24579)";
  app.modalConstants.ExtendedCost[94]   = "19125 honor pts";
  app.modalConstants.ExtendedCost[95]   = "6885 honor pts";
  app.modalConstants.ExtendedCost[98]   = "22950 honor pts";
  app.modalConstants.ExtendedCost[99]   = "8500 honor pts";
  app.modalConstants.ExtendedCost[100]  = "Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[102]  = "2 Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[103]  = "25 Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[104]  = "15 Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[115]  = "16983 honor pts";
  app.modalConstants.ExtendedCost[125]  = "22950 honor pts, 10 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[127]  = "15300 honor pts, 10 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[129]  = "11934 honor pts, 10 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[131]  = "30600 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[133]  = "2283 arena pts";
  app.modalConstants.ExtendedCost[146]  = "870 arena pts";
  app.modalConstants.ExtendedCost[148]  = "2739 arena pts";
  app.modalConstants.ExtendedCost[165]  = "9435 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[169]  = "9435 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[170]  = "9435 honor pts";
  app.modalConstants.ExtendedCost[171]  = "14280 honor pts, 40 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[172]  = "14280 honor pts, 40 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[173]  = "12750 honor pts";
  app.modalConstants.ExtendedCost[194]  = "18 Spirit Shard (id=28558)";
  app.modalConstants.ExtendedCost[201]  = "16524 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[202]  = "10098 honor pts, 20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[203]  = "16065 honor pts, 30 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[204]  = "16065 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[205]  = "10098 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[293]  = "50 Spirit Shard (id=28558)";
  app.modalConstants.ExtendedCost[298]  = "8 Spirit Shard (id=28558)";
  app.modalConstants.ExtendedCost[348]  = "15300 honor pts, 20 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[359]  = "20400 honor pts";
  app.modalConstants.ExtendedCost[360]  = "20400 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[386]  = "15300 honor pts";
  app.modalConstants.ExtendedCost[388]  = "30 Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[423]  = "30 Alterac Valley Mark of Honor (id=20560), 30 Arathi Basin Mark of Honor (id=20559), 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[427]  = "2805 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[428]  = "2805 honor pts, 20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[444]  = "4335 honor pts, 30 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[460]  = "1 honor pts";
  app.modalConstants.ExtendedCost[463]  = "13770 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[464]  = "13005 honor pts, 30 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[465]  = "8415 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[488]  = "4590 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[489]  = "4590 honor pts, 25 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[490]  = "10200 honor pts, 40 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[491]  = "1530 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[492]  = "1530 honor pts, 10 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[495]  = "4590 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[496]  = "2380 honor pts, 10 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[497]  = "2720 honor pts, 15 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[501]  = "8415 honor pts, 40 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[520]  = "2805 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[532]  = "1530 honor pts, 10 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[533]  = "1530 honor pts, 20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[541]  = "8415 honor pts, 20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[542]  = "13005 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[565]  = "15300 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[567]  = "15300 honor pts, 40 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[634]  = "2805 honor pts";
  app.modalConstants.ExtendedCost[652]  = "4590 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[653]  = "4335 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[701]  = "10200 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[702]  = "4760 honor pts, 25 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[746]  = "8160 honor pts, 40 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[747]  = "5100 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[748]  = "8925 honor pts, 40 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[774]  = "1530 honor pts, 10 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[821]  = "428 honor pts, 40 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[822]  = "65 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[823]  = "65 honor pts, 10 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[826]  = "214 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[837]  = "632 honor pts, 40 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[838]  = "95 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[839]  = "95 honor pts, 10 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[841]  = "175 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[855]  = "175 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[871]  = "316 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[883]  = "938 honor pts, 40 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[884]  = "141 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[885]  = "141 honor pts, 10 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[887]  = "258 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[901]  = "258 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[917]  = "469 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[929]  = "1387 honor pts, 40 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[930]  = "208 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[931]  = "208 honor pts, 10 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[933]  = "382 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[947]  = "382 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[968]  = "694 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[986]  = "208 honor pts, 10 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[989]  = "422 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[990]  = "624 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1000] = "60 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[1002] = "30 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[1003] = "60 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[1005] = "50 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[1006] = "20 Alterac Valley Mark of Honor (id=20560), 20 Arathi Basin Mark of Honor (id=20559), 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1007] = "3 Arathi Basin Mark of Honor (id=20559), 3 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1009] = "60 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1015] = "25 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1027] = "41 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1037] = "33 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1040] = "50 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1050] = "95 honor pts, 10 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[1054] = "258 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1055] = "118 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1061] = "20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[1062] = "20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[1076] = "20 Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[1077] = "45 Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[1110] = "30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[1111] = "30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1184] = "2 Holy Dust (id=29735)";
  app.modalConstants.ExtendedCost[1188] = "8 Holy Dust (id=29735)";
  app.modalConstants.ExtendedCost[1192] = "2 Arcane Rune (id=29736)";
  app.modalConstants.ExtendedCost[1196] = "8 Arcane Rune (id=29736)";
  app.modalConstants.ExtendedCost[1200] = "Chestguard of the Fallen Champion (id=29754)";
  app.modalConstants.ExtendedCost[1203] = "Gloves of the Fallen Champion (id=29757)";
  app.modalConstants.ExtendedCost[1206] = "Helm of the Fallen Champion (id=29760)";
  app.modalConstants.ExtendedCost[1209] = "Leggings of the Fallen Champion (id=29766)";
  app.modalConstants.ExtendedCost[1212] = "Pauldrons of the Fallen Champion (id=29763)";
  app.modalConstants.ExtendedCost[1215] = "Chestguard of the Fallen Hero (id=29755)";
  app.modalConstants.ExtendedCost[1216] = "Gloves of the Fallen Hero (id=29756)";
  app.modalConstants.ExtendedCost[1217] = "Helm of the Fallen Hero (id=29759)";
  app.modalConstants.ExtendedCost[1218] = "Leggings of the Fallen Hero (id=29765)";
  app.modalConstants.ExtendedCost[1219] = "Pauldrons of the Fallen Hero (id=29762)";
  app.modalConstants.ExtendedCost[1235] = "Chestguard of the Fallen Defender (id=29753)";
  app.modalConstants.ExtendedCost[1237] = "Gloves of the Fallen Defender (id=29758)";
  app.modalConstants.ExtendedCost[1239] = "Helm of the Fallen Defender (id=29761)";
  app.modalConstants.ExtendedCost[1241] = "Leggings of the Fallen Defender (id=29767)";
  app.modalConstants.ExtendedCost[1243] = "Pauldrons of the Fallen Defender (id=29764)";
  app.modalConstants.ExtendedCost[1332] = "Chestguard of the Vanquished Defender (id=30237)";
  app.modalConstants.ExtendedCost[1333] = "Gloves of the Vanquished Defender (id=30240)";
  app.modalConstants.ExtendedCost[1334] = "Helm of the Vanquished Defender (id=30243)";
  app.modalConstants.ExtendedCost[1335] = "Leggings of the Vanquished Defender (id=30246)";
  app.modalConstants.ExtendedCost[1336] = "Pauldrons of the Vanquished Defender (id=30249)";
  app.modalConstants.ExtendedCost[1342] = "Chestguard of the Vanquished Champion (id=30236)";
  app.modalConstants.ExtendedCost[1343] = "Gloves of the Vanquished Champion (id=30239)";
  app.modalConstants.ExtendedCost[1344] = "Helm of the Vanquished Champion (id=30242)";
  app.modalConstants.ExtendedCost[1345] = "Leggings of the Vanquished Champion (id=30245)";
  app.modalConstants.ExtendedCost[1346] = "Pauldrons of the Vanquished Champion (id=30248)";
  app.modalConstants.ExtendedCost[1357] = "Chestguard of the Vanquished Hero (id=30238)";
  app.modalConstants.ExtendedCost[1358] = "Gloves of the Vanquished Hero (id=30241)";
  app.modalConstants.ExtendedCost[1359] = "Helm of the Vanquished Hero (id=30244)";
  app.modalConstants.ExtendedCost[1360] = "Leggings of the Vanquished Hero (id=30247)";
  app.modalConstants.ExtendedCost[1361] = "Pauldrons of the Vanquished Hero (id=30250)";
  app.modalConstants.ExtendedCost[1431] = "1875 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[1432] = "1125 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[1435] = "1500 arena pts, 2000 arenarank";
  app.modalConstants.ExtendedCost[1442] = "7905 honor pts, 60 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1452] = "20 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1454] = "30 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1468] = "8160 honor pts, 60 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[1474] = "Gloves of the Forgotten Protector (id=31094)";
  app.modalConstants.ExtendedCost[1476] = "Helm of the Forgotten Protector (id=31095)";
  app.modalConstants.ExtendedCost[1478] = "Chestguard of the Forgotten Protector (id=31091)";
  app.modalConstants.ExtendedCost[1480] = "Leggings of the Forgotten Protector (id=31100)";
  app.modalConstants.ExtendedCost[1482] = "Pauldrons of the Forgotten Protector (id=31103)";
  app.modalConstants.ExtendedCost[1485] = "Gloves of the Forgotten Conqueror (id=31092)";
  app.modalConstants.ExtendedCost[1488] = "Helm of the Forgotten Conqueror (id=31097)";
  app.modalConstants.ExtendedCost[1491] = "Chestguard of the Forgotten Conqueror (id=31089)";
  app.modalConstants.ExtendedCost[1494] = "Leggings of the Forgotten Conqueror (id=31098)";
  app.modalConstants.ExtendedCost[1497] = "Pauldrons of the Forgotten Conqueror (id=31101)";
  app.modalConstants.ExtendedCost[1520] = "Gloves of the Forgotten Vanquisher (id=31093)";
  app.modalConstants.ExtendedCost[1521] = "Helm of the Forgotten Vanquisher (id=31096)";
  app.modalConstants.ExtendedCost[1522] = "Chestguard of the Forgotten Vanquisher (id=31090)";
  app.modalConstants.ExtendedCost[1523] = "Leggings of the Forgotten Vanquisher (id=31099)";
  app.modalConstants.ExtendedCost[1524] = "Pauldrons of the Forgotten Vanquisher (id=31102)";
  app.modalConstants.ExtendedCost[1564] = "8 honor pts";
  app.modalConstants.ExtendedCost[1565] = "5 honor pts";
  app.modalConstants.ExtendedCost[1642] = "15 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1645] = "Silken Thread (id=4291)";
  app.modalConstants.ExtendedCost[1646] = "100 Halaa Battle Token (id=26045), 20 Halaa Research Token (id=26044)";
  app.modalConstants.ExtendedCost[1647] = "70 Halaa Battle Token (id=26045), 15 Halaa Research Token (id=26044)";
  app.modalConstants.ExtendedCost[1648] = "2 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[1649] = "2 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[1652] = "2 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[1653] = "2 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1664] = "3750 arena pts";
  app.modalConstants.ExtendedCost[1670] = "2625 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[1757] = "3150 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[1758] = "1000 arena pts";
  app.modalConstants.ExtendedCost[1765] = "2 Halaa Research Token (id=26044)";
  app.modalConstants.ExtendedCost[1909] = "10 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[1910] = "35 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1911] = "17850 honor pts, 40 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[1923] = "17850 honor pts, 40 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[1935] = "11794 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[1948] = "3 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1949] = "2 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1950] = "4 Apexis Crystal (id=32572), 100 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1952] = "Apexis Crystal (id=32572), 50 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1958] = "10 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1959] = "Mark of the Illidari (id=32897)";
  app.modalConstants.ExtendedCost[1963] = "Coilfang Armaments (id=24368)";
  app.modalConstants.ExtendedCost[1973] = "40 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1979] = "160 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[1981] = "4 Apexis Crystal (id=32572)";
  app.modalConstants.ExtendedCost[1985] = "3 Apexis Crystal (id=32572)";
  app.modalConstants.ExtendedCost[1992] = "2 Spirit Shard (id=28558)";
  app.modalConstants.ExtendedCost[1994] = "10 Glowcap (id=24245)";
  app.modalConstants.ExtendedCost[2028] = "15300 honor pts, 10 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2049] = "75 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2059] = "60 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2060] = "35 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2236] = "4 Halaa Research Token (id=26044)";
  app.modalConstants.ExtendedCost[2237] = "27000 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2238] = "25200 honor pts, 20 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2239] = "18000 honor pts, 20 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2240] = "9000 honor pts, 20 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2241] = "8000 honor pts, 10 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2242] = "15000 honor pts, 20 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2248] = "11934 honor pts, 10 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2249] = "11424 honor pts, 40 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2250] = "11424 honor pts, 40 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2251] = "7548 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[2252] = "9547 honor pts, 10 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2253] = "9547 honor pts, 10 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2254] = "19125 honor pts, 20 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2255] = "12240 honor pts, 20 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2256] = "38250 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2257] = "24480 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2258] = "20655 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2259] = "13219 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2260] = "12622 honor pts, 20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2261] = "8078 honor pts, 20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2262] = "20081 honor pts, 30 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2263] = "12852 honor pts, 30 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2264] = "20081 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[2265] = "12852 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[2266] = "12622 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2267] = "8078 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2268] = "11794 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2269] = "7548 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2270] = "25500 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2271] = "16320 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2272] = "50 Apexis Shard (id=32569)";
  app.modalConstants.ExtendedCost[2273] = "600 Brewfest Prize Ticket (id=37829)";
  app.modalConstants.ExtendedCost[2274] = "50 Brewfest Prize Ticket (id=37829)";
  app.modalConstants.ExtendedCost[2275] = "100 Brewfest Prize Ticket (id=37829)";
  app.modalConstants.ExtendedCost[2276] = "200 Brewfest Prize Ticket (id=37829)";
  app.modalConstants.ExtendedCost[2277] = "10500 honor pts, 20 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2278] = "11250 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2279] = "14500 honor pts, 30 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2280] = "14500 honor pts, 30 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2281] = "14500 honor pts, 30 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[2282] = "3750 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2283] = "1125 arena pts";
  app.modalConstants.ExtendedCost[2284] = "3150 arena pts";
  app.modalConstants.ExtendedCost[2285] = "1875 arena pts";
  app.modalConstants.ExtendedCost[2286] = "1125 arena pts";
  app.modalConstants.ExtendedCost[2287] = "2625 arena pts";
  app.modalConstants.ExtendedCost[2288] = "1500 arena pts";
  app.modalConstants.ExtendedCost[2289] = "30000 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2290] = "19125 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2291] = "12240 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2292] = "Felfury Legplates (id=34180), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2293] = "Legplates of the Holy Juggernaut (id=34167), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2294] = "Chain Links of the Tumultuous Storm (id=34186), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2295] = "Breeches of Natural Aggression (id=34169), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2296] = "Leggings of the Immortal Night (id=34188), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2297] = "Pantaloons of Calming Strife (id=34170), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2298] = "Pauldrons of Perseverance (id=34192), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2299] = "Spaulders of the Thalassian Savior (id=34193), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2300] = "Equilibrium Epaulets (id=34208), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2301] = "Spaulders of Reclamation (id=34209), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2302] = "Shoulderpads of Vehemence (id=34195), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2303] = "Shawl of Wonderment (id=34202), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2304] = "Warharness of Reckless Fury (id=34215), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2305] = "Heroic Judicator's Chestguard (id=34216), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2306] = "Garments of Serene Shores (id=34229), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2307] = "Harness of Carnal Instinct (id=34211), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2308] = "Sunglow Vest (id=34212), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2309] = "Robes of Faltered Light (id=34233), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2310] = "Crown of Anasterian (id=34345), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2311] = "Helm of Burning Righteousness (id=34243), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2312] = "Cowl of Gul'dan (id=34332), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2313] = "Cover of Ursol the Wise (id=34245), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2314] = "Duplicitous Guise (id=34244), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2315] = "Cowl of Light's Purity (id=34339), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2316] = "Handguards of the Dawn (id=34342), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2317] = "Tranquil Majesty Wraps (id=34351), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2318] = "Shadowed Gauntlets of Paroxysm (id=34234), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2319] = "Gauntlets of the Ancient Shadowmoon (id=34350), Sunmote (id=34664)";
  app.modalConstants.ExtendedCost[2320] = "Bracers of the Forgotten Conqueror (id=34848)";
  app.modalConstants.ExtendedCost[2321] = "Bracers of the Forgotten Vanquisher (id=34852)";
  app.modalConstants.ExtendedCost[2322] = "Belt of the Forgotten Protector (id=34854)";
  app.modalConstants.ExtendedCost[2323] = "Bracers of the Forgotten Protector (id=34851)";
  app.modalConstants.ExtendedCost[2324] = "Belt of the Forgotten Conqueror (id=34853)";
  app.modalConstants.ExtendedCost[2325] = "Belt of the Forgotten Vanquisher (id=34855)";
  app.modalConstants.ExtendedCost[2326] = "Boots of the Forgotten Conqueror (id=34856)";
  app.modalConstants.ExtendedCost[2327] = "Boots of the Forgotten Vanquisher (id=34858)";
  app.modalConstants.ExtendedCost[2328] = "Boots of the Forgotten Protector (id=34857)";
  app.modalConstants.ExtendedCost[2329] = "150 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2330] = "125 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2331] = "105 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2332] = "45 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2333] = "100 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2334] = "1875 arena pts, 1650 arenarank";
  app.modalConstants.ExtendedCost[2335] = "1125 arena pts, 1550 arenarank";
  app.modalConstants.ExtendedCost[2336] = "1875 arena pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2337] = "1875 arena pts, 1600 arenarank";
  app.modalConstants.ExtendedCost[2338] = "1125 arena pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2339] = "1000 arena pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2340] = "2150 arena pts, 1700 arenarank";
  app.modalConstants.ExtendedCost[2341] = "1110 arena pts, 1700 arenarank";
  app.modalConstants.ExtendedCost[2342] = "1125 arena pts";
  app.modalConstants.ExtendedCost[2343] = "675 arena pts";
  app.modalConstants.ExtendedCost[2344] = "1575 arena pts, 1700 arenarank";
  app.modalConstants.ExtendedCost[2345] = "675 arena pts, 1700 arenarank";
  app.modalConstants.ExtendedCost[2346] = "1125 arena pts, 1700 arenarank";
  app.modalConstants.ExtendedCost[2347] = "40 Badge of Justice (id=29434)";
  app.modalConstants.ExtendedCost[2348] = "2 Burning Blossom (id=23247)";
  app.modalConstants.ExtendedCost[2349] = "5 Burning Blossom (id=23247)";
  app.modalConstants.ExtendedCost[2350] = "20 Burning Blossom (id=23247)";
  app.modalConstants.ExtendedCost[2351] = "100 Burning Blossom (id=23247)";
  app.modalConstants.ExtendedCost[2352] = "200 Burning Blossom (id=23247)";
  app.modalConstants.ExtendedCost[2353] = "350 Burning Blossom (id=23247)";
  app.modalConstants.ExtendedCost[2354] = "1 honor pts";
  app.modalConstants.ExtendedCost[2355] = "Borean Leather (id=33568)";
  app.modalConstants.ExtendedCost[2356] = "650 arena pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2357] = "650 arena pts";
  app.modalConstants.ExtendedCost[2358] = "1304 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2359] = "1500 arena pts, 2200 arenarank";
  app.modalConstants.ExtendedCost[2360] = "3750 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2361] = "3150 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2362] = "2625 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2363] = "1125 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2364] = "1875 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2365] = "1875 arena pts, 1700 arenarank";
  app.modalConstants.ExtendedCost[2366] = "1875 arena pts, 1550 arenarank";
  app.modalConstants.ExtendedCost[2367] = "2250 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2368] = "1110 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2369] = "1575 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2370] = "1575 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2371] = "675 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2372] = "1125 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2373] = "1304 arena pts, 1950 arenarank";
  app.modalConstants.ExtendedCost[2374] = "600 arena pts";
  app.modalConstants.ExtendedCost[2375] = "650 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2376] = "4 Borean Leather (id=33568)";
  app.modalConstants.ExtendedCost[2377] = "3000 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2378] = "2520 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2379] = "900 arena pts";
  app.modalConstants.ExtendedCost[2380] = "800 arena pts";
  app.modalConstants.ExtendedCost[2381] = "2100 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2382] = "900 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2383] = "1500 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2384] = "1150 arena pts, 1950 arenarank";
  app.modalConstants.ExtendedCost[2385] = "2520 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2386] = "3000 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2387] = "900 arena pts";
  app.modalConstants.ExtendedCost[2388] = "800 arena pts";
  app.modalConstants.ExtendedCost[2389] = "2100 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2390] = "900 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2391] = "1500 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2392] = "1200 arena pts, 1950 arenarank";
  app.modalConstants.ExtendedCost[2393] = "650 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2394] = "200 Venture Coin (id=37836)";
  app.modalConstants.ExtendedCost[2395] = "200 Venture Coin (id=37836)";
  app.modalConstants.ExtendedCost[2396] = "120 Venture Coin (id=37836)";
  app.modalConstants.ExtendedCost[2397] = "200 Brewfest Prize Token (id=37829)";
  app.modalConstants.ExtendedCost[2398] = "2 Brewfest Prize Token (id=37829)";
  app.modalConstants.ExtendedCost[2399] = "20 Brewfest Prize Token (id=37829)";
  app.modalConstants.ExtendedCost[2400] = "17850 honor pts, 40 Eye of the Storm Mark of Honor (id=29024), 1700 arenarank";
  app.modalConstants.ExtendedCost[2401] = "11794 honor pts, 20 Warsong Gulch Mark of Honor (id=20558), 1575 arenarank";
  app.modalConstants.ExtendedCost[2402] = "15300 honor pts, 10 Alterac Valley Mark of Honor (id=20560), 1650 arenarank";
  app.modalConstants.ExtendedCost[2403] = "40000 honor pts";
  app.modalConstants.ExtendedCost[2404] = "8000 honor pts";
  app.modalConstants.ExtendedCost[2405] = "9199 honor pts, 20 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[2406] = "13923 honor pts, 40 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2407] = "500 Ethereal Credit (id=38186)";
  app.modalConstants.ExtendedCost[2408] = "100 Ethereal Credit (id=38186)";
  app.modalConstants.ExtendedCost[2409] = "250 Ethereal Credit (id=38186)";
  app.modalConstants.ExtendedCost[2410] = "1000 Ethereal Credit (id=38186)";
  app.modalConstants.ExtendedCost[2411] = "50 Ethereal Credit (id=38186)";
  app.modalConstants.ExtendedCost[2412] = "25 Ethereal Credit (id=38186)";
  app.modalConstants.ExtendedCost[2413] = "Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2414] = "50 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2415] = "30 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2416] = "5 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2417] = "20 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2418] = "100 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2419] = "60 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2420] = "30 Venture Coin (id=37836)";
  app.modalConstants.ExtendedCost[2421] = "70 Venture Coin (id=37836)";
  app.modalConstants.ExtendedCost[2422] = "50 Venture Coin (id=37836)";
  app.modalConstants.ExtendedCost[2423] = "800 arena pts";
  app.modalConstants.ExtendedCost[2424] = "5 Brewfest Prize Token (id=37829)";
  app.modalConstants.ExtendedCost[2425] = "9500 honor pts, 10 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2426] = "1250 honor pts";
  app.modalConstants.ExtendedCost[2427] = "35000 honor pts, 40 Alterac Valley Mark of Honor (id=20560)";
  app.modalConstants.ExtendedCost[2428] = "20000 honor pts, 20 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2429] = "3 Dalaran Jewelcrafter's Token (id=41596)";
  app.modalConstants.ExtendedCost[2430] = "5 Dalaran Jewelcrafter's Token (id=41596)";
  app.modalConstants.ExtendedCost[2431] = "250 arena pts";
  app.modalConstants.ExtendedCost[2432] = "400 arena pts";
  app.modalConstants.ExtendedCost[2433] = "Dalaran Jewelcrafter's Token (id=41596)";
  app.modalConstants.ExtendedCost[2434] = "2 Dalaran Jewelcrafter's Token (id=41596)";
  app.modalConstants.ExtendedCost[2435] = "Alterac Valley Mark of Honor (id=20560), Arathi Basin Mark of Honor (id=20559), Eye of the Storm Mark of Honor (id=29024), Warsong Gulch Mark of Honor (id=20558), Strand of the Ancients Mark of Honor (id=42425)";
  app.modalConstants.ExtendedCost[2436] = "4 Dalaran Jewelcrafter's Token (id=41596)";
  app.modalConstants.ExtendedCost[2437] = "100 Oshu'gun Relic (id=25634)";
  app.modalConstants.ExtendedCost[2438] = "24000 honor pts, 700 arena pts";
  app.modalConstants.ExtendedCost[2439] = "20000 honor pts, 575 arena pts";
  app.modalConstants.ExtendedCost[2440] = "12000 honor pts, 350 arena pts";
  app.modalConstants.ExtendedCost[2441] = "7200 honor pts, 200 arena pts";
  app.modalConstants.ExtendedCost[2442] = "16800 honor pts, 475 arena pts";
  app.modalConstants.ExtendedCost[2443] = "7200 honor pts, 200 arena pts";
  app.modalConstants.ExtendedCost[2444] = "6400 honor pts, 175 arena pts";
  app.modalConstants.ExtendedCost[2445] = "9600 honor pts, 275 arena pts";
  app.modalConstants.ExtendedCost[2446] = "24800 honor pts";
  app.modalConstants.ExtendedCost[2447] = "15800 honor pts";
  app.modalConstants.ExtendedCost[2448] = "19000 honor pts";
  app.modalConstants.ExtendedCost[2449] = "24000 honor pts, 1400 arena pts, 1705 arenarank";
  app.modalConstants.ExtendedCost[2450] = "20000 honor pts, 1150 arena pts, 1705 arenarank";
  app.modalConstants.ExtendedCost[2451] = "12000 honor pts, 700 arena pts, 1675 arenarank";
  app.modalConstants.ExtendedCost[2452] = "7200 honor pts, 400 arena pts, 1615 arenarank";
  app.modalConstants.ExtendedCost[2453] = "12000 honor pts, 700 arena pts, 1735 arenarank";
  app.modalConstants.ExtendedCost[2454] = "12000 honor pts, 700 arena pts, 1645 arenarank";
  app.modalConstants.ExtendedCost[2455] = "16800 honor pts, 950 arena pts, 1705 arenarank";
  app.modalConstants.ExtendedCost[2456] = "7200 honor pts, 400 arena pts, 1705 arenarank";
  app.modalConstants.ExtendedCost[2457] = "6400 honor pts, 350 arena pts, 1705 arenarank";
  app.modalConstants.ExtendedCost[2458] = "12000 honor pts, 700 arena pts, 1705 arenarank";
  app.modalConstants.ExtendedCost[2459] = "9600 honor pts, 550 arena pts, 1775 arenarank";
  app.modalConstants.ExtendedCost[2460] = "4500 arena pts, 1930 arenarank";
  app.modalConstants.ExtendedCost[2461] = "3750 arena pts, 1930arenarank";
  app.modalConstants.ExtendedCost[2462] = "2250 arena pts, 1870 arenarank";
  app.modalConstants.ExtendedCost[2463] = "1350 arena pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2464] = "2250 arena pts, 1990 arenarank";
  app.modalConstants.ExtendedCost[2465] = "2250 arena pts, 1810 arenarank";
  app.modalConstants.ExtendedCost[2466] = "3150 arena pts, 1930 arenarank";
  app.modalConstants.ExtendedCost[2467] = "1350 arena pts, 1930 arenarank";
  app.modalConstants.ExtendedCost[2468] = "1200 arena pts, 1930 arenarank";
  app.modalConstants.ExtendedCost[2469] = "2250 arena pts, 1930 arenarank";
  app.modalConstants.ExtendedCost[2470] = "1750 arena pts, 2050 arenarank";
  app.modalConstants.ExtendedCost[2471] = "31000 honor pts, 1630 arenarank";
  app.modalConstants.ExtendedCost[2472] = "31000 honor pts, 1720 arenarank";
  app.modalConstants.ExtendedCost[2473] = "19700 honor pts, 1660 arenarank";
  app.modalConstants.ExtendedCost[2474] = "23700 honor pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2475] = "23700 honor pts, 1600 arenarank";
  app.modalConstants.ExtendedCost[2476] = "23700 honor pts, 1690 arenarank";
  app.modalConstants.ExtendedCost[2477] = "31000 honor pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2478] = "3 Dalaran Cooking Award (id=43016)";
  app.modalConstants.ExtendedCost[2479] = "5 Dalaran Cooking Award (id=43016)";
  app.modalConstants.ExtendedCost[2480] = "Dalaran Cooking Award (id=43016)";
  app.modalConstants.ExtendedCost[2481] = "12 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2482] = "24 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2483] = "12 honor pts";
  app.modalConstants.ExtendedCost[2484] = "10 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2485] = "Chestguard of the Lost Vanquisher (id=40612)";
  app.modalConstants.ExtendedCost[2486] = "Gloves of the Lost Vanquisher (id=40615)";
  app.modalConstants.ExtendedCost[2487] = "Helm of the Lost Vanquisher (id=40618)";
  app.modalConstants.ExtendedCost[2488] = "Leggings of the Lost Vanquisher (id=40621)";
  app.modalConstants.ExtendedCost[2489] = "Spaulders of the Lost Vanquisher (id=40624)";
  app.modalConstants.ExtendedCost[2490] = "Helm of the Lost Conqueror (id=40616)";
  app.modalConstants.ExtendedCost[2491] = "Chestguard of the Lost Conqueror (id=40610)";
  app.modalConstants.ExtendedCost[2492] = "Leggings of the Lost Conqueror (id=40619)";
  app.modalConstants.ExtendedCost[2493] = "Spaulders of the Lost Conqueror (id=40622)";
  app.modalConstants.ExtendedCost[2494] = "Gloves of the Lost Conqueror (id=40613)";
  app.modalConstants.ExtendedCost[2495] = "Chestguard of the Lost Protector (id=40611)";
  app.modalConstants.ExtendedCost[2496] = "Gloves of the Lost Protector (id=40614)";
  app.modalConstants.ExtendedCost[2497] = "Helm of the Lost Protector (id=40617)";
  app.modalConstants.ExtendedCost[2498] = "Leggings of the Lost Protector (id=40620)";
  app.modalConstants.ExtendedCost[2499] = "Spaulders of the Lost Protector (id=40623)";
  app.modalConstants.ExtendedCost[2500] = "10 Relic of Ulduar (id=42780)";
  app.modalConstants.ExtendedCost[2501] = "2 Relic of Ulduar (id=42780)";
  app.modalConstants.ExtendedCost[2502] = "250 Relic of Ulduar (id=42780)";
  app.modalConstants.ExtendedCost[2503] = "Gauntlets of the Lost Conqueror (id=40628)";
  app.modalConstants.ExtendedCost[2504] = "Breastplate of the Lost Conqueror (id=40625)";
  app.modalConstants.ExtendedCost[2505] = "Crown of the Lost Conqueror (id=40631)";
  app.modalConstants.ExtendedCost[2506] = "Legplates of the Lost Conqueror (id=40634)";
  app.modalConstants.ExtendedCost[2507] = "Mantle of the Lost Conqueror (id=40637)";
  app.modalConstants.ExtendedCost[2508] = "Breastplate of the Lost Protector (id=40626)";
  app.modalConstants.ExtendedCost[2509] = "Crown of the Lost Protector (id=40632)";
  app.modalConstants.ExtendedCost[2510] = "Gauntlets of the Lost Protector (id=40629)";
  app.modalConstants.ExtendedCost[2511] = "Legplates of the Lost Protector (id=40635)";
  app.modalConstants.ExtendedCost[2512] = "Mantle of the Lost Protector (id=40638)";
  app.modalConstants.ExtendedCost[2513] = "Breastplate of the Lost Vanquisher (id=40627)";
  app.modalConstants.ExtendedCost[2514] = "Crown of the Lost Vanquisher (id=40633)";
  app.modalConstants.ExtendedCost[2515] = "Gauntlets of the Lost Vanquisher (id=40630)";
  app.modalConstants.ExtendedCost[2516] = "Legplates of the Lost Vanquisher (id=40636)";
  app.modalConstants.ExtendedCost[2517] = "Mantle of the Lost Vanquisher (id=40639)";
  app.modalConstants.ExtendedCost[2518] = "15 Necrotic Rune (id=22484)";
  app.modalConstants.ExtendedCost[2519] = "20 Necrotic Rune (id=22484)";
  app.modalConstants.ExtendedCost[2520] = "8 Necrotic Rune (id=22484)";
  app.modalConstants.ExtendedCost[2521] = "30 Necrotic Rune (id=22484)";
  app.modalConstants.ExtendedCost[2522] = "40 Necrotic Rune (id=22484)";
  app.modalConstants.ExtendedCost[2523] = "25 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2524] = "40 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2525] = "15 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2526] = "60 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2527] = "75 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2528] = "45 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2529] = "30 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2530] = "100 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2531] = "35 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2532] = "50 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2533] = "80 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2534] = "25 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2535] = "60 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2536] = "40 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2537] = "60 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2538] = "75 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2539] = "45 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2540] = "30 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2541] = "10 Dalaran Cooking Award (id=43016)";
  app.modalConstants.ExtendedCost[2542] = "200 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2543] = "325 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2544] = "250 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2545] = "20 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2546] = "30 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2547] = "15 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2548] = "100 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2549] = "40 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2550] = "200 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2551] = "65 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2552] = "300 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2553] = "4 Dream Shard (id=34052)";
  app.modalConstants.ExtendedCost[2554] = "10 Dream Shard (id=34052)";
  app.modalConstants.ExtendedCost[2555] = "3 Heavy Borean Leather (id=38425)";
  app.modalConstants.ExtendedCost[2556] = "2 Arctic Fur (id=44128)";
  app.modalConstants.ExtendedCost[2557] = "2500 honor pts";
  app.modalConstants.ExtendedCost[2558] = "6 Dalaran Jewelcrafter's Token (id=41596)";
  app.modalConstants.ExtendedCost[2559] = "49600 honor pts";
  app.modalConstants.ExtendedCost[2560] = "31600 honor pts";
  app.modalConstants.ExtendedCost[2561] = "38000 honor pts";
  app.modalConstants.ExtendedCost[2562] = "62000 honor pts, 1630 arenarank";
  app.modalConstants.ExtendedCost[2563] = "62000 honor pts, 1720 arenarank";
  app.modalConstants.ExtendedCost[2564] = "39400 honor pts, 1660 arenarank";
  app.modalConstants.ExtendedCost[2565] = "47400 honor pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2566] = "47400 honor pts, 1600 arenarank";
  app.modalConstants.ExtendedCost[2567] = "47400 honor pts, 1690 arenarank";
  app.modalConstants.ExtendedCost[2568] = "62000 honor pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2569] = "60000 honor pts";
  app.modalConstants.ExtendedCost[2570] = "50000 honor pts";
  app.modalConstants.ExtendedCost[2571] = "10 Noblegarden Chocolate (id=44791)";
  app.modalConstants.ExtendedCost[2572] = "25 Noblegarden Chocolate (id=44791)";
  app.modalConstants.ExtendedCost[2573] = "50 Noblegarden Chocolate (id=44791)";
  app.modalConstants.ExtendedCost[2574] = "100 Noblegarden Chocolate (id=44791)";
  app.modalConstants.ExtendedCost[2575] = "5 Noblegarden Chocolate (id=44791)";
  app.modalConstants.ExtendedCost[2576] = "9 Wintergrasp Mark of Honor (id=43589)";
  app.modalConstants.ExtendedCost[2577] = "15 Wintergrasp Mark of Honor (id=43589)";
  app.modalConstants.ExtendedCost[2578] = "25 Wintergrasp Mark of Honor (id=43589)";
  app.modalConstants.ExtendedCost[2579] = "40 Wintergrasp Mark of Honor (id=43589)";
  app.modalConstants.ExtendedCost[2580] = "Coin of Ancestry (id=21100)";
  app.modalConstants.ExtendedCost[2581] = "5 Coin of Ancestry (id=21100)";
  app.modalConstants.ExtendedCost[2582] = "Ink of the Sea (id=43126)";
  app.modalConstants.ExtendedCost[2583] = "10 Ink of the Sea (id=43126)";
  app.modalConstants.ExtendedCost[2584] = "5 Abyss Crystal (id=34057)";
  app.modalConstants.ExtendedCost[2585] = "2 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2586] = "2 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2587] = "10 Winterfin Clam (id=34597)";
  app.modalConstants.ExtendedCost[2588] = "10000 honor pts";
  app.modalConstants.ExtendedCost[2589] = "Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2590] = "100 honor pts, 200 arena pts, 2 Emblem of Heroism (id=40752), 2 Emblem of Valor (id=40753), 2 Stone Keeper's Shard (id=43228)";
  app.modalConstants.ExtendedCost[2591] = "Band of the Kirin Tor (id=40586)";
  app.modalConstants.ExtendedCost[2592] = "Band of the Kirin Tor (id=40586)";
  app.modalConstants.ExtendedCost[2593] = "Loop of the Kirin Tor (id=44934)";
  app.modalConstants.ExtendedCost[2594] = "Ring of the Kirin Tor (id=44935)";
  app.modalConstants.ExtendedCost[2595] = "Signet of the Kirin Tor (id=40585)";
  app.modalConstants.ExtendedCost[2596] = "100 arena pts";
  app.modalConstants.ExtendedCost[2597] = "15 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2598] = "50 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2599] = "40 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2600] = "100 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2601] = "25 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2602] = "10 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2603] = "150 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2604] = "18 Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2605] = "30400 honor pts";
  app.modalConstants.ExtendedCost[2606] = "19 Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2607] = "28 Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2608] = "6400 honor pts, 350 arena pts, 700 arenarank";
  app.modalConstants.ExtendedCost[2609] = "7200 honor pts, 400 arena pts, 800 arenarank";
  app.modalConstants.ExtendedCost[2610] = "12000 honor pts, 700 arena pts, 900 arenarank";
  app.modalConstants.ExtendedCost[2611] = "12000 honor pts, 700 arena pts, 1000 arenarank";
  app.modalConstants.ExtendedCost[2612] = "12000 honor pts, 700 arena pts, 1100 arenarank";
  app.modalConstants.ExtendedCost[2613] = "9600 honor pts, 550 arena pts, 1350 arenarank";
  app.modalConstants.ExtendedCost[2614] = "39400 honor pts, 1400 arenarank";
  app.modalConstants.ExtendedCost[2615] = "62000 honor pts, 1450 arenarank";
  app.modalConstants.ExtendedCost[2616] = "62000 honor pts, 1500 arenarank";
  app.modalConstants.ExtendedCost[2617] = "47400 honor pts, 1550 arenarank";
  app.modalConstants.ExtendedCost[2618] = "47400 honor pts, 1650 arenarank";
  app.modalConstants.ExtendedCost[2619] = "47400 honor pts, 2000 arenarank";
  app.modalConstants.ExtendedCost[2620] = "58 Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2621] = "1300 arena pts, 1600 arenarank";
  app.modalConstants.ExtendedCost[2622] = "2150 arena pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2623] = "50000 honor pts, 1400 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2624] = "35000 honor pts, 1200 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2625] = "35000 honor pts, 1200 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2626] = "15000 honor pts, 1100 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2627] = "1066 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2628] = "1950 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2629] = "2150 arena pts, 1900 arenarank";
  app.modalConstants.ExtendedCost[2630] = "39 Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2631] = "46 Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2632] = "4250 arena pts, 2350 arenarank";
  app.modalConstants.ExtendedCost[2633] = "3585 arena pts, 2350 arenarank";
  app.modalConstants.ExtendedCost[2634] = "3000 arena pts, 2350 arenarank";
  app.modalConstants.ExtendedCost[2635] = "1245 arena pts, 2350 arenarank";
  app.modalConstants.ExtendedCost[2636] = "500 arena pts, 2350 arenarank";
  app.modalConstants.ExtendedCost[2637] = "Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2638] = "15 Emblem of Valor (id=40753)";
  app.modalConstants.ExtendedCost[2639] = "Champion's Writ (id=46114)";
  app.modalConstants.ExtendedCost[2640] = "Gloves of the Wayward Conqueror (id=45644)";
  app.modalConstants.ExtendedCost[2641] = "Gauntlets of the Wayward Conqueror (id=45641)";
  app.modalConstants.ExtendedCost[2642] = "Spaulders of the Wayward Conqueror (id=45659)";
  app.modalConstants.ExtendedCost[2643] = "Mantle of the Wayward Conqueror (id=45656)";
  app.modalConstants.ExtendedCost[2644] = "Leggings of the Wayward Conqueror (id=45650)";
  app.modalConstants.ExtendedCost[2645] = "Legplates of the Wayward Conqueror (id=45653)";
  app.modalConstants.ExtendedCost[2646] = "Chestguard of the Wayward Conqueror (id=45635)";
  app.modalConstants.ExtendedCost[2647] = "Breastplate of the Wayward Conqueror (id=45632)";
  app.modalConstants.ExtendedCost[2648] = "Helm of the Wayward Conqueror (id=45647)";
  app.modalConstants.ExtendedCost[2649] = "Crown of the Wayward Conqueror (id=45638)";
  app.modalConstants.ExtendedCost[2650] = "Gloves of the Wayward Protector (id=45645)";
  app.modalConstants.ExtendedCost[2651] = "Gauntlets of the Wayward Protector (id=45642)";
  app.modalConstants.ExtendedCost[2652] = "Spaulders of the Wayward Protector (id=45660)";
  app.modalConstants.ExtendedCost[2653] = "Mantle of the Wayward Protector (id=45657)";
  app.modalConstants.ExtendedCost[2654] = "Leggings of the Wayward Protector (id=45651)";
  app.modalConstants.ExtendedCost[2655] = "Legplates of the Wayward Protector (id=45654)";
  app.modalConstants.ExtendedCost[2656] = "Chestguard of the Wayward Protector (id=45636)";
  app.modalConstants.ExtendedCost[2657] = "Breastplate of the Wayward Protector (id=45633)";
  app.modalConstants.ExtendedCost[2658] = "Helm of the Wayward Protector (id=45648)";
  app.modalConstants.ExtendedCost[2659] = "Crown of the Wayward Protector (id=45639)";
  app.modalConstants.ExtendedCost[2660] = "Gloves of the Wayward Vanquisher (id=45646)";
  app.modalConstants.ExtendedCost[2661] = "Gauntlets of the Wayward Vanquisher (id=45643)";
  app.modalConstants.ExtendedCost[2662] = "Spaulders of the Wayward Vanquisher (id=45661)";
  app.modalConstants.ExtendedCost[2663] = "Mantle of the Wayward Vanquisher (id=45658)";
  app.modalConstants.ExtendedCost[2664] = "Leggings of the Wayward Vanquisher (id=45652)";
  app.modalConstants.ExtendedCost[2665] = "Legplates of the Wayward Vanquisher (id=45655)";
  app.modalConstants.ExtendedCost[2666] = "Chestguard of the Wayward Vanquisher (id=45637)";
  app.modalConstants.ExtendedCost[2667] = "Breastplate of the Wayward Vanquisher (id=45634)";
  app.modalConstants.ExtendedCost[2668] = "Helm of the Wayward Vanquisher (id=45649)";
  app.modalConstants.ExtendedCost[2669] = "Crown of the Wayward Vanquisher (id=45640)";
  app.modalConstants.ExtendedCost[2670] = "100 Dalaran Cooking Award (id=43016)";
  app.modalConstants.ExtendedCost[2671] = "5 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2673] = "60 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2674] = "95 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2675] = "75 Champion's Seal (id=44990)";
  app.modalConstants.ExtendedCost[2676] = "2000 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2677] = "2000 arena pts, 1850 arenarank(1)";
  app.modalConstants.ExtendedCost[2678] = "2000 arena pts, 1850 arenarank(2)";
  app.modalConstants.ExtendedCost[2679] = "Regalia of the Grand Conqueror  (id=47557)";
  app.modalConstants.ExtendedCost[2680] = "Regalia of the Grand Protector (id=47558)";
  app.modalConstants.ExtendedCost[2681] = "Regalia of the Grand Vanquisher (id=47559)";
  app.modalConstants.ExtendedCost[2682] = "25 Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2683] = "75 Emblem of Triumph (id=47241), Trophy of the Crusade (id=47242)";
  app.modalConstants.ExtendedCost[2684] = "45 Emblem of Triumph (id=47241), Trophy of the Crusade (id=47242)";
  app.modalConstants.ExtendedCost[2685] = "35 Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2686] = "50 Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2687] = "30 Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2688] = "2150 arena pts, 1600 arenarank";
  app.modalConstants.ExtendedCost[2689] = "1300 arena pts, 1400 arenarank";
  app.modalConstants.ExtendedCost[2690] = "2150 arena pts, 1900 arenarank";
  app.modalConstants.ExtendedCost[2691] = "2150 arena pts, 1550 arenarank";
  app.modalConstants.ExtendedCost[2692] = "1750 arena pts, 2000 arenarank(1)";
  app.modalConstants.ExtendedCost[2693] = "50000 honor pts, 1400 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2694] = "4250 arena pts, 2200 arenarank(1)";
  app.modalConstants.ExtendedCost[2695] = "35000 honor pts, 1200 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2696] = "3585 arena pts, 2200 arenarank(1)";
  app.modalConstants.ExtendedCost[2697] = "3000 arena pts, 2200 arenarank(1)";
  app.modalConstants.ExtendedCost[2698] = "15000 honor pts, 1100 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2699] = "1245 arena pts, 2200 arenarank(1)";
  app.modalConstants.ExtendedCost[2700] = "1150 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2701] = "300 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2702] = "Inscribed Band of the Kirin Tor (id=45688)";
  app.modalConstants.ExtendedCost[2703] = "Inscribed Loop of the Kirin Tor (id=45689)";
  app.modalConstants.ExtendedCost[2704] = "Inscribed Ring of the Kirin Tor (id=45690)";
  app.modalConstants.ExtendedCost[2705] = "Inscribed Signet of the Kirin Tor (id=45691)";
  app.modalConstants.ExtendedCost[2706] = "20 Emblem of Heroism (id=40752)";
  app.modalConstants.ExtendedCost[2707] = "Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2708] = "75 Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2709] = "45 Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2710] = "10 Titanium Powder (id=46849)";
  app.modalConstants.ExtendedCost[2711] = "75 Emblem of Conquest (id=45624)";
  app.modalConstants.ExtendedCost[2712] = "62000 honor pts, 1250 arenarank";
  app.modalConstants.ExtendedCost[2713] = "62000 honor pts, 1300 arenarank";
  app.modalConstants.ExtendedCost[2714] = "39400 honor pts, 1200 arenarank";
  app.modalConstants.ExtendedCost[2715] = "47400 honor pts, 1750 arenarank";
  app.modalConstants.ExtendedCost[2716] = "47400 honor pts, 1450 arenarank";
  app.modalConstants.ExtendedCost[2717] = "47400 honor pts, 1500 arenarank";
  app.modalConstants.ExtendedCost[2718] = "62000 honor pts, 1650 arenarank";
  app.modalConstants.ExtendedCost[2719] = "1100 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2720] = "300 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2721] = "1100 arena pts, 1850 arenarank";
  app.modalConstants.ExtendedCost[2722] = "500 arena pts, 2350 arenarank";
  app.modalConstants.ExtendedCost[2723] = "15 Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2724] = "10 Heavy Borean Leather (id=38425)";
  app.modalConstants.ExtendedCost[2725] = "Love Token (id=49927)";
  app.modalConstants.ExtendedCost[2726] = "10 Love Token (id=49927)";
  app.modalConstants.ExtendedCost[2727] = "5 Love Token (id=49927)";
  app.modalConstants.ExtendedCost[2728] = "2 Love Token (id=49927)";
  app.modalConstants.ExtendedCost[2729] = "40 Love Token (id=49927)";
  app.modalConstants.ExtendedCost[2730] = "Lovely Charm Bracelet (id=49916)";
  app.modalConstants.ExtendedCost[2731] = "Primordial Saronite (id=49908)";
  app.modalConstants.ExtendedCost[2732] = "30 Love Token (id=49927)";
  app.modalConstants.ExtendedCost[2733] = "20 Love Token (id=49927)";
  app.modalConstants.ExtendedCost[2734] = "Emblem of Triumph (id=47241)";
  app.modalConstants.ExtendedCost[2735] = "Etched Signet of the Kirin Tor (id=48957)";
  app.modalConstants.ExtendedCost[2736] = "Etched Ring of the Kirin Tor (id=48956)";
  app.modalConstants.ExtendedCost[2737] = "Etched Loop of the Kirin Tor (id=48955)";
  app.modalConstants.ExtendedCost[2738] = "Etched Band of the Kirin Tor (id=48954)";
  app.modalConstants.ExtendedCost[2739] = "30 Emblem of Frost (id=49426)";
  app.modalConstants.ExtendedCost[2740] = "60 Emblem of Frost (id=49426)";
  app.modalConstants.ExtendedCost[2741] = "50 Emblem of Frost (id=49426)";
  app.modalConstants.ExtendedCost[2742] = "95 Emblem of Frost (id=49426)";
  app.modalConstants.ExtendedCost[2743] = "Emblem of Frost (id=49426)";
  app.modalConstants.ExtendedCost[2744] = "23 Emblem of Frost (id=49426)";
  app.modalConstants.ExtendedCost[2745] = "Vanquisher's Mark of Sanctification (id=52025), Bloodmage Shoulderpads (id=50279)";
  app.modalConstants.ExtendedCost[2746] = "Vanquisher's Mark of Sanctification (id=52025), Bloodmage Robe (id=50278)";
  app.modalConstants.ExtendedCost[2747] = "Vanquisher's Mark of Sanctification (id=52025), Bloodmage Leggings (id=50277)";
  app.modalConstants.ExtendedCost[2748] = "Vanquisher's Mark of Sanctification (id=52025), Bloodmage Hood (id=50276)";
  app.modalConstants.ExtendedCost[2749] = "Vanquisher's Mark of Sanctification (id=52025), Bloodmage Gloves (id=50275)";
  app.modalConstants.ExtendedCost[2750] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Bloodmage Gloves (id=51159)";
  app.modalConstants.ExtendedCost[2751] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Bloodmage Hood (id=51158)";
  app.modalConstants.ExtendedCost[2752] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Bloodmage Leggings (id=51157)";
  app.modalConstants.ExtendedCost[2753] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Bloodmage Robe (id=51156)";
  app.modalConstants.ExtendedCost[2754] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Bloodmage Shoulderpads (id=51155)";
  app.modalConstants.ExtendedCost[2755] = "Vanquisher's Mark of Sanctification (id=52025), Shadowblade Pauldrons (id=50105)";
  app.modalConstants.ExtendedCost[2756] = "Vanquisher's Mark of Sanctification (id=52025), Shadowblade Legplates (id=50090)";
  app.modalConstants.ExtendedCost[2757] = "Vanquisher's Mark of Sanctification (id=52025), Shadowblade Helmet (id=50089)";
  app.modalConstants.ExtendedCost[2758] = "Vanquisher's Mark of Sanctification (id=52025), Shadowblade Gauntlets (id=50088)";
  app.modalConstants.ExtendedCost[2759] = "Vanquisher's Mark of Sanctification (id=52025), Shadowblade Breastplate (id=50087)";
  app.modalConstants.ExtendedCost[2760] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Shadowblade Breastplate (id=51189)";
  app.modalConstants.ExtendedCost[2761] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Shadowblade Gauntlets (id=51188)";
  app.modalConstants.ExtendedCost[2762] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Shadowblade Helmet (id=51187)";
  app.modalConstants.ExtendedCost[2763] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Shadowblade Legplates (id=51186)";
  app.modalConstants.ExtendedCost[2764] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Shadowblade Pauldrons (id=51185)";
  app.modalConstants.ExtendedCost[2765] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Pauldrons (id=50113)";
  app.modalConstants.ExtendedCost[2766] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Legplates (id=50109)";
  app.modalConstants.ExtendedCost[2767] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Helmet (id=50108)";
  app.modalConstants.ExtendedCost[2768] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Gauntlets (id=50107)";
  app.modalConstants.ExtendedCost[2769] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Robes (id=50106)";
  app.modalConstants.ExtendedCost[2770] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Robes (id=51139)";
  app.modalConstants.ExtendedCost[2771] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Gauntlets (id=51138)";
  app.modalConstants.ExtendedCost[2772] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Helmet (id=51137)";
  app.modalConstants.ExtendedCost[2773] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Legplates (id=51136)";
  app.modalConstants.ExtendedCost[2774] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Pauldrons (id=51135)";
  app.modalConstants.ExtendedCost[2775] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Vestment (id=50823)";
  app.modalConstants.ExtendedCost[2776] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Trousers (id=50820)";
  app.modalConstants.ExtendedCost[2777] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Mantle (id=50819)";
  app.modalConstants.ExtendedCost[2778] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Gloves (id=50822)";
  app.modalConstants.ExtendedCost[2779] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Cover (id=50821)";
  app.modalConstants.ExtendedCost[2780] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Cover (id=51149)";
  app.modalConstants.ExtendedCost[2781] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Gloves (id=51148)";
  app.modalConstants.ExtendedCost[2782] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Mantle (id=51147)";
  app.modalConstants.ExtendedCost[2783] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Trousers (id=51146)";
  app.modalConstants.ExtendedCost[2784] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Vestment (id=51145)";
  app.modalConstants.ExtendedCost[2785] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Shoulderpads (id=50824)";
  app.modalConstants.ExtendedCost[2786] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Raiment (id=50828)";
  app.modalConstants.ExtendedCost[2787] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Legguards (id=50825)";
  app.modalConstants.ExtendedCost[2788] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Headguard (id=50826)";
  app.modalConstants.ExtendedCost[2789] = "Vanquisher's Mark of Sanctification (id=52025), Lasherweave Handgrips (id=50827)";
  app.modalConstants.ExtendedCost[2790] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Handgrips (id=51144)";
  app.modalConstants.ExtendedCost[2791] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Headguard (id=51143)";
  app.modalConstants.ExtendedCost[2792] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Legguards (id=51142)";
  app.modalConstants.ExtendedCost[2793] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Raiment (id=51141)";
  app.modalConstants.ExtendedCost[2794] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Lasherweave Shoulderpads (id=51140)";
  app.modalConstants.ExtendedCost[2795] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Shoulderplates (id=50098)";
  app.modalConstants.ExtendedCost[2796] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Legplates (id=50097)";
  app.modalConstants.ExtendedCost[2797] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Helmet (id=50096)";
  app.modalConstants.ExtendedCost[2798] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Gauntlets (id=50095)";
  app.modalConstants.ExtendedCost[2799] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Battleplate (id=50094)";
  app.modalConstants.ExtendedCost[2800] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Battleplate (id=51129)";
  app.modalConstants.ExtendedCost[2801] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Gauntlets (id=51128)";
  app.modalConstants.ExtendedCost[2802] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Helmet (id=51127)";
  app.modalConstants.ExtendedCost[2803] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Legplates (id=51126)";
  app.modalConstants.ExtendedCost[2804] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Shoulderplates (id=51125)";
  app.modalConstants.ExtendedCost[2805] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Pauldrons (id=50853)";
  app.modalConstants.ExtendedCost[2806] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Legguards (id=50854)";
  app.modalConstants.ExtendedCost[2807] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Handguards (id=50856)";
  app.modalConstants.ExtendedCost[2808] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Faceguard (id=50855)";
  app.modalConstants.ExtendedCost[2809] = "Vanquisher's Mark of Sanctification (id=52025), Scourgelord Chestguard (id=50857)";
  app.modalConstants.ExtendedCost[2810] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Chestguard (id=51134)";
  app.modalConstants.ExtendedCost[2811] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Faceguard (id=51133)";
  app.modalConstants.ExtendedCost[2812] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Handguards (id=51132)";
  app.modalConstants.ExtendedCost[2813] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Legguards (id=51131)";
  app.modalConstants.ExtendedCost[2814] = "Vanquisher's Mark of Sanctification (id=52028), Sanctified Scourgelord Pauldrons (id=51130)";
  app.modalConstants.ExtendedCost[2815] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Tunic (id=50869)";
  app.modalConstants.ExtendedCost[2816] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Spaulders (id=50865)";
  app.modalConstants.ExtendedCost[2817] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Headpiece (id=50867)";
  app.modalConstants.ExtendedCost[2818] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Greaves (id=50866)";
  app.modalConstants.ExtendedCost[2819] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Gloves (id=50868)";
  app.modalConstants.ExtendedCost[2820] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Gloves (id=51169)";
  app.modalConstants.ExtendedCost[2821] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Greaves (id=51168)";
  app.modalConstants.ExtendedCost[2822] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Headpiece (id=51167)";
  app.modalConstants.ExtendedCost[2823] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Spaulders (id=51166)";
  app.modalConstants.ExtendedCost[2824] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Tunic (id=51165)";
  app.modalConstants.ExtendedCost[2825] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Shoulderplates (id=50324)";
  app.modalConstants.ExtendedCost[2826] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Legplates (id=50325)";
  app.modalConstants.ExtendedCost[2827] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Helmet (id=50326)";
  app.modalConstants.ExtendedCost[2828] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Gauntlets (id=50327)";
  app.modalConstants.ExtendedCost[2829] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Battleplate (id=50328)";
  app.modalConstants.ExtendedCost[2830] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Battleplate (id=51164)";
  app.modalConstants.ExtendedCost[2831] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Gauntlets (id=51163)";
  app.modalConstants.ExtendedCost[2832] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Helmet (id=51162)";
  app.modalConstants.ExtendedCost[2833] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Legplates (id=51161)";
  app.modalConstants.ExtendedCost[2834] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Shoulderplates (id=51160)";
  app.modalConstants.ExtendedCost[2835] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Shoulderguards (id=50860)";
  app.modalConstants.ExtendedCost[2836] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Legguards (id=50861)";
  app.modalConstants.ExtendedCost[2837] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Handguards (id=50863)";
  app.modalConstants.ExtendedCost[2838] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Faceguard (id=50862)";
  app.modalConstants.ExtendedCost[2839] = "Conqueror's Mark of Sanctification (id=52027), Lightsworn Chestguard (id=50864)";
  app.modalConstants.ExtendedCost[2840] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Chestguard (id=51174)";
  app.modalConstants.ExtendedCost[2841] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Faceguard (id=51173)";
  app.modalConstants.ExtendedCost[2842] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Handguards (id=51172)";
  app.modalConstants.ExtendedCost[2843] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Legguards (id=51171)";
  app.modalConstants.ExtendedCost[2844] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Lightsworn Shoulderguards (id=51170)";
  app.modalConstants.ExtendedCost[2845] = "Conqueror's Mark of Sanctification (id=52027), Dark Coven Shoulderpads (id=50244)";
  app.modalConstants.ExtendedCost[2846] = "Conqueror's Mark of Sanctification (id=52027), Dark Coven Robe (id=50243)";
  app.modalConstants.ExtendedCost[2847] = "Conqueror's Mark of Sanctification (id=52027), Dark Coven Leggings (id=50242)";
  app.modalConstants.ExtendedCost[2848] = "Conqueror's Mark of Sanctification (id=52027), Dark Coven Hood (id=50241)";
  app.modalConstants.ExtendedCost[2849] = "Conqueror's Mark of Sanctification (id=52027), Dark Coven Gloves (id=50240)";
  app.modalConstants.ExtendedCost[2850] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Dark Coven Gloves (id=51209)";
  app.modalConstants.ExtendedCost[2851] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Dark Coven Hood (id=51208)";
  app.modalConstants.ExtendedCost[2852] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Dark Coven Leggings (id=51207)";
  app.modalConstants.ExtendedCost[2853] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Dark Coven Robe (id=51206)";
  app.modalConstants.ExtendedCost[2854] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Dark Coven Shoulderpads (id=51205)";
  app.modalConstants.ExtendedCost[2855] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Shoulderpads (id=50767)";
  app.modalConstants.ExtendedCost[2856] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Robe (id=50768)";
  app.modalConstants.ExtendedCost[2857] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Leggings (id=50769)";
  app.modalConstants.ExtendedCost[2858] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Hood (id=50765)";
  app.modalConstants.ExtendedCost[2859] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Gloves (id=50766)";
  app.modalConstants.ExtendedCost[2860] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Gloves (id=51179)";
  app.modalConstants.ExtendedCost[2861] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Hood (id=51178)";
  app.modalConstants.ExtendedCost[2862] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Leggings (id=51177)";
  app.modalConstants.ExtendedCost[2863] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Robe (id=51176)";
  app.modalConstants.ExtendedCost[2864] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Shoulderpads (id=51175)";
  app.modalConstants.ExtendedCost[2865] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Raiments (id=50394)";
  app.modalConstants.ExtendedCost[2866] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Pants (id=50393)";
  app.modalConstants.ExtendedCost[2867] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Mantle (id=50396)";
  app.modalConstants.ExtendedCost[2868] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Handwraps (id=50391)";
  app.modalConstants.ExtendedCost[2869] = "Conqueror's Mark of Sanctification (id=52027), Crimson Acolyte Cowl (id=50392)";
  app.modalConstants.ExtendedCost[2870] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Cowl (id=51184)";
  app.modalConstants.ExtendedCost[2871] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Handwraps (id=51183)";
  app.modalConstants.ExtendedCost[2872] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Mantle (id=51182)";
  app.modalConstants.ExtendedCost[2873] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Pants (id=51181)";
  app.modalConstants.ExtendedCost[2874] = "Conqueror's Mark of Sanctification (id=52030), Sanctified Crimson Acolyte Raiments (id=51180)";
  app.modalConstants.ExtendedCost[2875] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Shoulderplates (id=50082)";
  app.modalConstants.ExtendedCost[2876] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Legplates (id=50081)";
  app.modalConstants.ExtendedCost[2877] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Helmet (id=50080)";
  app.modalConstants.ExtendedCost[2878] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Gauntlets (id=50079)";
  app.modalConstants.ExtendedCost[2879] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Battleplate (id=50078)";
  app.modalConstants.ExtendedCost[2880] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Battleplate (id=51214)";
  app.modalConstants.ExtendedCost[2881] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Gauntlets (id=51213)";
  app.modalConstants.ExtendedCost[2882] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Helmet (id=51212)";
  app.modalConstants.ExtendedCost[2883] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Legplates (id=51211)";
  app.modalConstants.ExtendedCost[2884] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Shoulderplates (id=51210)";
  app.modalConstants.ExtendedCost[2885] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Pauldrons (id=50846)";
  app.modalConstants.ExtendedCost[2886] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Legguards (id=50847)";
  app.modalConstants.ExtendedCost[2887] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Handguards (id=50849)";
  app.modalConstants.ExtendedCost[2888] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Greathelm (id=50848)";
  app.modalConstants.ExtendedCost[2889] = "Protector's Mark of Sanctification (id=52026), Ymirjar Lord's Breastplate (id=50850)";
  app.modalConstants.ExtendedCost[2890] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Breastplate (id=51219)";
  app.modalConstants.ExtendedCost[2891] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Greathelm (id=51218)";
  app.modalConstants.ExtendedCost[2892] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Handguards (id=51217)";
  app.modalConstants.ExtendedCost[2893] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Legguards (id=51216)";
  app.modalConstants.ExtendedCost[2894] = "Protector's Mark of Sanctification (id=52029), Sanctified Ymirjar Lord's Pauldrons (id=51215)";
  app.modalConstants.ExtendedCost[2895] = "Protector's Mark of Sanctification (id=52026), Ahn'Kahar Blood Hunter's Tunic (id=50118)";
  app.modalConstants.ExtendedCost[2896] = "Protector's Mark of Sanctification (id=52026), Ahn'Kahar Blood Hunter's Spaulders (id=50117)";
  app.modalConstants.ExtendedCost[2897] = "Protector's Mark of Sanctification (id=52026), Ahn'Kahar Blood Hunter's Legguards (id=50116)";
  app.modalConstants.ExtendedCost[2898] = "Protector's Mark of Sanctification (id=52026), Ahn'Kahar Blood Hunter's Headpiece (id=50115)";
  app.modalConstants.ExtendedCost[2899] = "Protector's Mark of Sanctification (id=52026), Ahn'Kahar Blood Hunter's Handguards (id=50114)";
  app.modalConstants.ExtendedCost[2900] = "Protector's Mark of Sanctification (id=52029), Sanctified Ahn'Kahar Blood Hunter's Handguards (id=51154)";
  app.modalConstants.ExtendedCost[2901] = "Protector's Mark of Sanctification (id=52029), Sanctified Ahn'Kahar Blood Hunter's Headpiece (id=51153)";
  app.modalConstants.ExtendedCost[2902] = "Protector's Mark of Sanctification (id=52029), Sanctified Ahn'Kahar Blood Hunter's Legguards (id=51152)";
  app.modalConstants.ExtendedCost[2903] = "Protector's Mark of Sanctification (id=52029), Sanctified Ahn'Kahar Blood Hunter's Spaulders (id=51151)";
  app.modalConstants.ExtendedCost[2904] = "Protector's Mark of Sanctification (id=52029), Sanctified Ahn'Kahar Blood Hunter's Tunic (id=51150)";
  app.modalConstants.ExtendedCost[2905] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Tunic (id=50835)";
  app.modalConstants.ExtendedCost[2906] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Handguards (id=50836)";
  app.modalConstants.ExtendedCost[2907] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Headpiece (id=50837)";
  app.modalConstants.ExtendedCost[2908] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Legguards (id=50838)";
  app.modalConstants.ExtendedCost[2909] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Spaulders (id=50839)";
  app.modalConstants.ExtendedCost[2910] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Tunic (id=51190)";
  app.modalConstants.ExtendedCost[2911] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Handguards (id=51191)";
  app.modalConstants.ExtendedCost[2912] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Headpiece (id=51192)";
  app.modalConstants.ExtendedCost[2913] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Legguards (id=51193)";
  app.modalConstants.ExtendedCost[2914] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Spaulders (id=51194)";
  app.modalConstants.ExtendedCost[2915] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Chestguard (id=50830)";
  app.modalConstants.ExtendedCost[2916] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Grips (id=50831)";
  app.modalConstants.ExtendedCost[2917] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Faceguard (id=50832)";
  app.modalConstants.ExtendedCost[2918] = "Protector's Mark of Sanctification (id=52026), Frost Witch's War-Kilt (id=50833)";
  app.modalConstants.ExtendedCost[2919] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Shoulderguards (id=50834)";
  app.modalConstants.ExtendedCost[2920] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Chestguard (id=51195)";
  app.modalConstants.ExtendedCost[2921] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Grips (id=51196)";
  app.modalConstants.ExtendedCost[2922] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Faceguard (id=51197)";
  app.modalConstants.ExtendedCost[2923] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's War-Kilt (id=51198)";
  app.modalConstants.ExtendedCost[2924] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's War-Kilt (id=51198)";
  app.modalConstants.ExtendedCost[2925] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Hauberk (id=50841)";
  app.modalConstants.ExtendedCost[2926] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Gloves (id=50842)";
  app.modalConstants.ExtendedCost[2927] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Helm (id=50843)";
  app.modalConstants.ExtendedCost[2928] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Kilt (id=50844)";
  app.modalConstants.ExtendedCost[2929] = "Protector's Mark of Sanctification (id=52026), Frost Witch's Shoulderpads (id=50845)";
  app.modalConstants.ExtendedCost[2930] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Hauberk (id=51200)";
  app.modalConstants.ExtendedCost[2931] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Gloves (id=51201)";
  app.modalConstants.ExtendedCost[2932] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Helm (id=51202)";
  app.modalConstants.ExtendedCost[2933] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Kilt (id=51203)";
  app.modalConstants.ExtendedCost[2934] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Shoulderpads (id=51204)";
  app.modalConstants.ExtendedCost[2935] = "Protector's Mark of Sanctification (id=52029), Sanctified Frost Witch's Shoulderguards (id=51199)";
  app.modalConstants.ExtendedCost[2936] = "55000 honor pts, 1540 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2937] = "550 arena pts, 2350 arenarank";
  app.modalConstants.ExtendedCost[2938] = "550 arena pts, 2300 arenarank";
  app.modalConstants.ExtendedCost[2939] = "4670 arena pts, 2200 arenarank";
  app.modalConstants.ExtendedCost[2940] = "3950 arena pts, 2200 arenarank";
  app.modalConstants.ExtendedCost[2941] = "55000 honor pts , 1540 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2942] = "4670 arena pts, 2200 arenarank";
  app.modalConstants.ExtendedCost[2943] = "38500 honor pts, 1300 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2944] = "3300 arena pts, 2200 arenarank";
  app.modalConstants.ExtendedCost[2945] = "38500 honor pts, 1090 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2946] = "16500 honor pts, 450 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2947] = "1370 arena pts, 2200 arenarank";
  app.modalConstants.ExtendedCost[2948] = "1930 arena pts, 2000 arenarank";
  app.modalConstants.ExtendedCost[2949] = "2370 arena pts, 1950 arenarank";
  app.modalConstants.ExtendedCost[2950] = "350 arena pts, 1800 arenarank";
  app.modalConstants.ExtendedCost[2951] = "1260 arena pts, 1700 arenarank";
  app.modalConstants.ExtendedCost[2952] = "2370 arena pts  1600 arenarank";
  app.modalConstants.ExtendedCost[2953] = "2370 arena pts, 1500 arenarank";
  app.modalConstants.ExtendedCost[2954] = "1430 arena pts, 1400 arenarank";
  app.modalConstants.ExtendedCost[2955] = "10550 honor pts, 600 arena pts";
  app.modalConstants.ExtendedCost[2956] = "13200 honor pts, 770 arena pts";
  app.modalConstants.ExtendedCost[2957] = "8000 honor pts, 440 arena pts";
  app.modalConstants.ExtendedCost[2958] = "34700 honor pts";
  app.modalConstants.ExtendedCost[2959] = "54500 honor pts";
  app.modalConstants.ExtendedCost[2960] = "43300 honor pts";
  app.modalConstants.ExtendedCost[2961] = "68200 honor pts, 1400 arenarank";
  app.modalConstants.ExtendedCost[2962] = "68200 honor pts, 1300 arenarank";
  app.modalConstants.ExtendedCost[2963] = "43400 honor pts";
  app.modalConstants.ExtendedCost[2964] = "52200 honor pts";
  app.modalConstants.ExtendedCost[2965] = "68200 honor pts";
  app.modalConstants.ExtendedCost[2966] = "34100 honor pts";
  app.modalConstants.ExtendedCost[2967] = "21700 honor pts";
  app.modalConstants.ExtendedCost[2968] = "26100 honor pts";
  app.modalConstants.ExtendedCost[2969] = "12000 honor pts";
  app.modalConstants.ExtendedCost[2974] = "Ashen Band of Endless Vengeance (id=50402)";
  app.modalConstants.ExtendedCost[2975] = "Ashen Band of Unmatched Vengeance (id=50401)";
  app.modalConstants.ExtendedCost[2976] = "Ashen Band of Greater Vengeance (id=50387)";
  app.modalConstants.ExtendedCost[2977] = "Ashen Band of Vengeance (id=50376)";
  app.modalConstants.ExtendedCost[2978] = "Ashen Band of Might (id=52569)";
  app.modalConstants.ExtendedCost[2979] = "Ashen Band of Greater Might (id=52570)";
  app.modalConstants.ExtendedCost[2980] = "Ashen Band of Unmatched Might (id=52571)";
  app.modalConstants.ExtendedCost[2981] = "Ashen Band of Endless Might (id=52571)";
  app.modalConstants.ExtendedCost[2982] = "Ashen Band of Endless Might (id=52572)";
  app.modalConstants.ExtendedCost[2985] = "1 Frozen Orb (id=43102)";
  app.modalConstants.ExtendedCost[2986] = "6 Frozen Orb (id=43102)";
  app.modalConstants.ExtendedCost[2987] = "4 Frozen Orb (id=43102)";
  app.modalConstants.ExtendedCost[2991] = "1 Isle of Conquest Mark of Honor (id=47395)";
  app.modalConstants.ExtendedCost[2992] = "1 Strand of the Ancients Mark of Honor (id=42425)";
  app.modalConstants.ExtendedCost[2993] = "1 Eye of the Storm Mark of Honor (id=29024)";
  app.modalConstants.ExtendedCost[2994] = "1 Arathi Basin Mark of Honor (id=20559)";
  app.modalConstants.ExtendedCost[2995] = "1 Alterac Valley Mark of Honor(id=20560)";
  app.modalConstants.ExtendedCost[2996] = "1 Warsong Gulch Mark of Honor (id=20558)";
  app.modalConstants.ExtendedCost[2997] = "22 Emblem of Triumph (id=47241)";

  /* bonding constants */
  app.modalConstants.bonding = [];
  app.modalConstants.bonding[0] = "No Bind";
  app.modalConstants.bonding[1] = "Binds when Picked Up";
  app.modalConstants.bonding[2] = "Binds when Equipped";
  app.modalConstants.bonding[3] = "Binds when Used";
  app.modalConstants.bonding[4] = "Quest Item";
  app.modalConstants.bonding[5] = "Quest Item1";

  /* item material constants */
  app.modalConstants.material = [];
  app.modalConstants.material[-1] = "Consumables (food, reagents, etc.)";
  app.modalConstants.material[0] = "Not Defined";
  app.modalConstants.material[1] = "Metal";
  app.modalConstants.material[2] = "Wood";
  app.modalConstants.material[3] = "Liquid";
  app.modalConstants.material[4] = "Jewelry";
  app.modalConstants.material[5] = "Chain";
  app.modalConstants.material[6] = "Plate";
  app.modalConstants.material[7] = "Cloth";
  app.modalConstants.material[8] = "Leather";

  /* item stat type */
  app.modalConstants.statType = [];
  app.modalConstants.statType[0] = "MANA/POWER";
  app.modalConstants.statType[1] = "HEALTH";
  app.modalConstants.statType[3] = "AGILITY";
  app.modalConstants.statType[4] = "STRENGTH";
  app.modalConstants.statType[5] = "INTELLECT";
  app.modalConstants.statType[6] = "SPIRIT";
  app.modalConstants.statType[7] = "STAMINA";
  app.modalConstants.statType[12] = "Defense_SKILL_RATING";
  app.modalConstants.statType[13] = "DODGE_RATING";
  app.modalConstants.statType[14] = "PARRY_RATING";
  app.modalConstants.statType[15] = "BLOCK_RATING";
  app.modalConstants.statType[16] = "MELEE_HIT_RATING";
  app.modalConstants.statType[17] = "RANGED_HIT_RATING";
  app.modalConstants.statType[18] = "SPELL_HIT_RATING";
  app.modalConstants.statType[19] = "CRIT_MELEE_RATING";
  app.modalConstants.statType[20] = "CRIT_RANGED_RATING";
  app.modalConstants.statType[21] = "CRIT_SPELL_RATING";
  app.modalConstants.statType[22] = "HIT_TAKEN_MELEE_RATING";
  app.modalConstants.statType[23] = "HIT_TAKEN_RANGED_RATING";
  app.modalConstants.statType[24] = "HIT_TAKEN_SPELL_RATING";
  app.modalConstants.statType[25] = "CRIT_TAKEN_MELEE_RATING";
  app.modalConstants.statType[26] = "CRIT_TAKEN_RANGED_RATING";
  app.modalConstants.statType[27] = "CRIT_TAKEN_SPELL_RATING";
  app.modalConstants.statType[28] = "HASTE_MELEE_RATING";
  app.modalConstants.statType[29] = "HASTE_RANGED_RATING";
  app.modalConstants.statType[30] = "HASTE_SPELL_RATING";
  app.modalConstants.statType[31] = "HIT_RATING";
  app.modalConstants.statType[32] = "CRIT_RATING";
  app.modalConstants.statType[33] = "HIT_TAKEN_RATING";
  app.modalConstants.statType[34] = "CRIT_TAKEN_RATING";
  app.modalConstants.statType[35] = "RESILIENCE_RATING";
  app.modalConstants.statType[36] = "HASTE_RATING";
  app.modalConstants.statType[37] = "EXPERTISE_RATING";
  app.modalConstants.statType[38] = "ATTACK_POWER";
  app.modalConstants.statType[39] = "RANGED_ATTACK_POWER";
  app.modalConstants.statType[40] = "FERAL_ATTACK_POWER (not used as of v3.3)";
  app.modalConstants.statType[41] = "SPELL_HEALING_DONE";
  app.modalConstants.statType[42] = "SPELL_DAMAGE_DONE";
  app.modalConstants.statType[43] = "MANA_REGENERATION";
  app.modalConstants.statType[44] = "ARMOR_PENETRATION_RATING";
  app.modalConstants.statType[45] = "SPELL_POWER";
  app.modalConstants.statType[46] = "HEALTH_REGEN";
  app.modalConstants.statType[47] = "SPELL_PENETRATION";
  app.modalConstants.statType[48] = "BLOCK_VALUE";

  /* PageTextMaterial constants */
  app.modalConstants.PageTextMaterial = [];
  app.modalConstants.PageTextMaterial[1] = "Parchemin";
  app.modalConstants.PageTextMaterial[2] = "Pierre";
  app.modalConstants.PageTextMaterial[3] = "Marble";
  app.modalConstants.PageTextMaterial[4] = "Argent";
  app.modalConstants.PageTextMaterial[5] = "Bronze";
  app.modalConstants.PageTextMaterial[6] = "Valentine";
  app.modalConstants.PageTextMaterial[7] = "Illidan";

  /* item BagFamily constants */
  app.modalConstants.bagFamily = [];
  app.modalConstants.bagFamily[0] = { id : 0,     name : "None" };
  app.modalConstants.bagFamily[1] = { id : 1,     name : "Arrows" };
  app.modalConstants.bagFamily[2] = { id : 2,     name : "Bullets" };
  app.modalConstants.bagFamily[3] = { id : 4,     name : "Soul Shards" };
  app.modalConstants.bagFamily[4] = { id : 8,     name : "Leatherworking Supplies" };
  app.modalConstants.bagFamily[5] = { id : 16,    name : "Inscription Supplies" };
  app.modalConstants.bagFamily[6] = { id : 32,    name : "Herbs" };
  app.modalConstants.bagFamily[7] = { id : 64,    name : "Enchanting Supplies" };
  app.modalConstants.bagFamily[8] = { id : 128,   name : "Engineering Supplies" };
  app.modalConstants.bagFamily[9] = { id : 256,   name : "Keys" };
  app.modalConstants.bagFamily[10] = { id : 512,   name : "Gems" };
  app.modalConstants.bagFamily[11] = { id : 1024,  name : "Mining Supplies" };
  app.modalConstants.bagFamily[12] = { id : 2048,  name : "Soulbound Equipment" };
  app.modalConstants.bagFamily[13] = { id : 4096,  name : "Vanity Pets" };
  app.modalConstants.bagFamily[14] = { id : 8192,  name : "Currency Tokens" };
  app.modalConstants.bagFamily[15] = { id : 16384, name : "QuestItems" };

  /* item sheath constants */
  app.modalConstants.sheath = [];
  app.modalConstants.sheath[0] = "NONE";
  app.modalConstants.sheath[1] = "2H Weapon - Accross the back pointing down";
  app.modalConstants.sheath[2] = "Staff - Diagonally accross the back poiting up";
  app.modalConstants.sheath[3] = "One Handed - To the side";
  app.modalConstants.sheath[4] = "Shield - On the back in the middle";
  app.modalConstants.sheath[5] = "Enchanter's Rod ";
  app.modalConstants.sheath[6] = "Off hand - On the other side of One Handed";

  /* PetFoodType constants */
  app.modalConstants.foodType = [];
  app.modalConstants.foodType[1] = "Meat";
  app.modalConstants.foodType[2] = "Fish";
  app.modalConstants.foodType[3] = "Cheese";
  app.modalConstants.foodType[4] = "Bread";
  app.modalConstants.foodType[5] = "Fungus";
  app.modalConstants.foodType[6] = "Fruit";
  app.modalConstants.foodType[7] = "Raw Meat";
  app.modalConstants.foodType[8] = "Raw Fish";

  /* item dmgType constants */
  app.modalConstants.dmgType = [];
  app.modalConstants.dmgType[0] = "Physique";
  app.modalConstants.dmgType[1] = "Sacre";
  app.modalConstants.dmgType[2] = "Feu";
  app.modalConstants.dmgType[3] = "Nature";
  app.modalConstants.dmgType[4] = "Givre";
  app.modalConstants.dmgType[5] = "Ombre";
  app.modalConstants.dmgType[6] = "Arcane";

  /* item Quality constants */
  app.modalConstants.Quality = [];
  app.modalConstants.Quality[0] = "Grey Poor";
  app.modalConstants.Quality[1] = "White Common";
  app.modalConstants.Quality[2] = "Green Uncommon";
  app.modalConstants.Quality[3] = "Blue Rare";
  app.modalConstants.Quality[4] = "Purple Epic";
  app.modalConstants.Quality[5] = "Orange Legendary";
  app.modalConstants.Quality[6] = "Red Artifact";
  app.modalConstants.Quality[7] = "Gold Heirloom (Bind to Account)";

  /* item RequiredReputationRank constants */
  app.modalConstants.reputationRank = [];
  app.modalConstants.reputationRank[0] = "HATED";
  app.modalConstants.reputationRank[1] = "HOSTILE";
  app.modalConstants.reputationRank[2] = "UNFRIENDLY";
  app.modalConstants.reputationRank[3] = "NEUTRAL";
  app.modalConstants.reputationRank[4] = "FRIENDLY";
  app.modalConstants.reputationRank[5] = "HONORED";
  app.modalConstants.reputationRank[6] = "REVERED";
  app.modalConstants.reputationRank[7] = "EXALTED";

  /* item flags Custom constants */
  app.modalConstants.flagsCustom = [];
  app.modalConstants.flagsCustom[1] = "DURATION_REAL_TIME (Item duration will tick even if player is offline)";
  app.modalConstants.flagsCustom[2] = "IGNORE_QUEST_STATUS (No quest status will be checked when this item drops)";
  app.modalConstants.flagsCustom[4] = "FOLLOW_LOOT_RULES (Item will always follow group/master/need before greed looting rules)";

  /* spell Trigger constants */
  app.modalConstants.spellTrigger = [];
  app.modalConstants.spellTrigger[0] = "On Use";
  app.modalConstants.spellTrigger[1] = "On Equip";
  app.modalConstants.spellTrigger[2] = "Chance on Hit";
  app.modalConstants.spellTrigger[4] = "Soulstone";
  app.modalConstants.spellTrigger[5] = "Use with no delay";
  app.modalConstants.spellTrigger[6] = "Learn Spell ID";

  /* Languages constants */
  app.modalConstants.languages = [];
  app.modalConstants.languages[0] = "Universal";
  app.modalConstants.languages[1] = "Orcish";
  app.modalConstants.languages[2] = "Darnassian";
  app.modalConstants.languages[3] = "Taurahe";
  app.modalConstants.languages[6] = "Dwarvish";
  app.modalConstants.languages[7] = "Common";
  app.modalConstants.languages[8] = "Demonic";
  app.modalConstants.languages[9] = "Titan";
  app.modalConstants.languages[10] = "Thalassian";
  app.modalConstants.languages[11] = "Draconic";
  app.modalConstants.languages[12] = "Kalimag";
  app.modalConstants.languages[13] = "Gnomish";
  app.modalConstants.languages[14] = "Troll";
  app.modalConstants.languages[33] = "Gutterspeak";
  app.modalConstants.languages[35] = "Draenei";
  app.modalConstants.languages[36] = "Zombie";
  app.modalConstants.languages[37] = "Gnomish Binary";
  app.modalConstants.languages[38] = "Goblin Binary";
  app.modalConstants.languages[39] = "Addon";

  /* rank constants */
  app.modalConstants.rank = [];
  app.modalConstants.rank[0] = "Normal";
  app.modalConstants.rank[1] = "Elite";
  app.modalConstants.rank[2] = "Rare Elite";
  app.modalConstants.rank[3] = "Boss";
  app.modalConstants.rank[4] = "Rare";

  /* trainer type constants */
  app.modalConstants.trainer_type = [];
  app.modalConstants.trainer_type[0] = "CLASS";
  app.modalConstants.trainer_type[1] = "MOUNTS";
  app.modalConstants.trainer_type[2] = "TRADESKILLS";
  app.modalConstants.trainer_type[3] = "PET";

  /* creature MovementType constants */
  app.modalConstants.MovementType = [];
  app.modalConstants.MovementType[0] = "Statique (stay in place)";
  app.modalConstants.MovementType[1] = "Aleatoire (in radius)";
  app.modalConstants.MovementType[2] = "Waypoint movement";

  /* GameObject Type constants */
  app.modalConstants.GameObjectType = [];
  app.modalConstants.GameObjectType[0] = "PORTE";
  app.modalConstants.GameObjectType[1] = "BOUTON";
  app.modalConstants.GameObjectType[2] = "DONNEUR DE QUETE";
  app.modalConstants.GameObjectType[3] = "COFFRE";
  app.modalConstants.GameObjectType[4] = "CLASSEUR (non utilise)";
  app.modalConstants.GameObjectType[5] = "Generique";
  app.modalConstants.GameObjectType[6] = "PIEGE";
  app.modalConstants.GameObjectType[7] = "CHAISE";
  app.modalConstants.GameObjectType[8] = "SPELL_FOCUS";
  app.modalConstants.GameObjectType[9] = "TEXT";
  app.modalConstants.GameObjectType[10] = "GOOBER";
  app.modalConstants.GameObjectType[11] = "TRANSPORT";
  app.modalConstants.GameObjectType[12] = "ZONE DE DOMMAGE";
  app.modalConstants.GameObjectType[13] = "CAMERA";
  app.modalConstants.GameObjectType[14] = "MAP_OBJECT (non utilise)";
  app.modalConstants.GameObjectType[15] = "MO_TRANSPORT (Navire)";
  app.modalConstants.GameObjectType[16] = "DUEL_ARBITER";
  app.modalConstants.GameObjectType[17] = "NODE DE PECHE";
  app.modalConstants.GameObjectType[18] = "RITUEL DINVOCATION";
  app.modalConstants.GameObjectType[19] = "BOITES AUX LETTRES";
  app.modalConstants.GameObjectType[20] = "VENTE AUX ENCHERES (DO_NOT_USE)";
  app.modalConstants.GameObjectType[21] = "POSTE DE GARDE";
  app.modalConstants.GameObjectType[22] = "SPELLCASTER (portail etc)";
  app.modalConstants.GameObjectType[23] = "PIERRE DE RENCONTRE";
  app.modalConstants.GameObjectType[24] = "FLAGSTAND";
  app.modalConstants.GameObjectType[25] = "SUPPORT DE DRAPEAU";
  app.modalConstants.GameObjectType[26] = "FLAGDROP (drapeau)";
  app.modalConstants.GameObjectType[27] = "MINI_GAME";
  app.modalConstants.GameObjectType[28] = "KIOSQUE DE LOTERIE (DO_NOT_USE)";
  app.modalConstants.GameObjectType[29] = "POINT DE CAPTURE";
  app.modalConstants.GameObjectType[30] = "GENERATEUR AURA";
  app.modalConstants.GameObjectType[31] = "DUNGEON_DIFFICULTY (portail dinstance)";
  app.modalConstants.GameObjectType[32] = "CHAISE DE BARBIER";
  app.modalConstants.GameObjectType[33] = "BATIMENT DESTRUCTIBLE";
  app.modalConstants.GameObjectType[34] = "BANQUE DE GUILDE";
  app.modalConstants.GameObjectType[35] = "TRAPPE (REMOVED?)";
  app.modalConstants.GameObjectType[36] = "NEW_FLAG";
	app.modalConstants.GameObjectType[37] = "NEW_FLAG_DROP";
	app.modalConstants.GameObjectType[38] = "GARRISON_BUILDING";
	app.modalConstants.GameObjectType[39] = "GARRISON_PLOT";
	app.modalConstants.GameObjectType[40] = "CLIENT_CREATURE";
	app.modalConstants.GameObjectType[41] = "CLIENT_ITEM";
	app.modalConstants.GameObjectType[42] = "POINT DE CAPTURE";
	app.modalConstants.GameObjectType[43] = "PHASEABLE_MO";
	app.modalConstants.GameObjectType[44] = "GARRISON_MONUMENT";
	app.modalConstants.GameObjectType[45] = "GARRISON_SHIPMENT";
	app.modalConstants.GameObjectType[46] = "GARRISON_MONUMENT_PLAQUE";
	app.modalConstants.GameObjectType[47] = "DO_NOT_USE_3";
	app.modalConstants.GameObjectType[48] = "UI_LINK";
  
  
  
  /* STITCH : creature dmgschool constants */
  app.modalConstants.dmgschool = [];
  app.modalConstants.dmgschool[0]  = "Physique-SPELL_SCHOOL_NORMAL";
  app.modalConstants.dmgschool[1]  = "Sacre-SPELL_SCHOOL_HOLY";
  app.modalConstants.dmgschool[2]  = "Feu-SPELL_SCHOOL_FIRE";
  app.modalConstants.dmgschool[3]  = "Nature-SPELL_SCHOOL_NATURE";
  app.modalConstants.dmgschool[4]  = "Givre-SPELL_SCHOOL_FROST";
  app.modalConstants.dmgschool[5]  = "Ombre-SPELL_SCHOOL_SHADOW";
  app.modalConstants.dmgschool[6]  = "Arcane-SPELL_SCHOOL_ARCANE";

  /* STITCH : creature HoverHeight constants */
  app.modalConstants.HoverHeight = [];
  app.modalConstants.HoverHeight[0]  = "Pas de Vol temporaire";
  app.modalConstants.HoverHeight[1]  = "Vol temporaire permis";

  /* STITCH : creature QuestType constants */
  app.modalConstants.QuestType = [];
  app.modalConstants.QuestType[0]  = "Enabled, auto-complete quand accepted";
  app.modalConstants.QuestType[1]  = "Disabled";
	app.modalConstants.QuestType[2]  = "Enabled";
	app.modalConstants.QuestType[3]  = "World Quest";


}());
