#include "version.h"
#include <string.h>
#include "common.h"
#include "util.h"
#include "char_event.h"
#include "char.h"
#include "item_event.h"
#include "magic.h"
#include "pet_event.h"
#include "npc_townpeople.h"
#include "npc_Dengon.h"
#include "npc_door.h"
#include "npc_healer.h"
#include "npc_oldman.h"
#include "npc_warp.h"
#include "npc_storyteller.h"
#include "npc_msg.h"
#include "npc_npcenemy.h"
#include "npc_action.h"
#include "npc_windowman.h"
#include "npc_savepoint.h"
#include "npc_windowhealer.h"
#include "npc_itemshop.h"
#include "npc_sysinfo.h"
#include "npc_duelranking.h"
#include "npc_petskillshop.h"
#include "npc_petshop.h"
#include "npc_signboard.h"
#include "npc_warpman.h"
#include "npc_exchangeman.h"
#include "petmail.h"
#include "npc_timeman.h"
#include "npc_bodylan.h"
#include "npc_mic.h"
#include "npc_luckyman.h"
#include "npc_bus.h"
#include "npc_charm.h"
#include "npc_poolitemshop.h"
#include "npc_quiz.h"
#include "npc_checkman.h"
#include "npc_janken.h"
#include "npc_transmigration.h"
#include "battle_event.h"
#include "enemy.h"
// Robin 0517
#include "npc_familyman.h"
#include "npc_bankman.h"
// add code by shan
#include "npc_fmdengon.h"
#include "npc_fmhealer.h"
#include "npc_petmaker.h"

// CoolFish: Family 2001/6/4
#include "npc_fmwarpman.h"
#include "npc_fmpkman.h"
#include "npc_fmpkcallman.h"

// Arminius 7.7 Airplane
#include "npc_airplane.h"

// Arminius 7.13 Scheduleman
#include "npc_scheduleman.h"

// Arminius 7.24 manor scheduleman
#include "npc_manorsman.h"

// Robin 0725
#include "npc_riderman.h"
#include "npc_fmletter.h"

#ifdef _SERVICE
// Terry 2001/09/01
#include "npc_stoneserviceman.h"
#endif

#ifdef _NPC_SELLSTH
#include "npc_sellsthman.h"
#endif

//andy
#ifdef _GAMBLE_BANK
#include "npc_gamblebank.h"
#endif

#ifdef _NEW_WARPMAN
#include "npc_newnpcman.h"
#endif

#ifdef _MARKET_TRADE
#include "npc_mtradenpcman.h"
#endif

#ifdef _GAMBLE_ROULETTE 
#include "npc_gambleroulette.h"
#include "npc_gamblemaster.h"
#endif

#ifdef _TRANSER_MAN
#include "npc_transerman.h"
#endif

#ifdef _PAUCTION_MAN
#include "npc_pauctionman.h"
#endif

#ifdef _CFREE_petskill
#include "npc_freepetskillshop.h"
#endif

#ifdef _PETRACE
#include "npc_petracemaster.h"
#include "npc_petracepet.h"
#endif

#ifdef _AUCTIONEER
#include "npc_auctioneer.h"
#endif

#ifdef _BLACK_MARKET
#include "npc_blackmarket.h"
#endif

#ifdef _ITEM_NPCCHANGE
#include "npc_itemchange.h"
#endif

#ifdef _NPC_MAKEPAIR
#include "npc_makepair.h"
#endif

#ifdef _NPC_FUSION
#include "npc_petfusion.h"
#endif

#ifdef _ALLDOMAN // (不可開) Syu ADD 排行榜NPC
#include "npc_alldoman.h"
#endif

#ifdef _NPC_WELFARE
#include "npc_welfare.h"
#endif

#ifdef _NPC_WELFARE_2				// WON ADD 職業NPC-2
#include "npc_welfare2.h"
#endif

#ifdef _NPC_VERYWELFARE
#include "npc_verywelfare.h"
#endif

#ifdef _RACEMAN
#include "npc_raceman.h"
#endif

#define DEBUG

typedef struct tagCorrespondStringAndFunctionTable
{
    STRING32    functionName;
    void*       functionPointer;
    int         hashcode;
}CorrespondStringAndFunctionTable;

static CorrespondStringAndFunctionTable
correspondStringAndFunctionTable[]=
{
    /* 以下はサーバー内部コードで定義されている関数だ。NPCもこれ
     をつかうときがある*/
    { {"core_PreWalk"},  			CHAR_allprewalk,   		0 },
    { {"core_PostWalk"},  			CHAR_allpostwalk,  		0 },
    { {"core_Loop"},				CHAR_loopFunc, 			0 },
    { {"core_Dying"},				CHAR_die, 				0 },
    { {"core_PlayerWatch"},   		CHAR_playerWatchfunc, 	0 },
    { {"core_PlayerTalked"},		CHAR_playerTalkedfunc, 	0 },


    /* 以下はアイテムを使うとき用の関数だ。 */
    { {"MedicineInit"},   			ITEM_MedicineInit, 		0 },
    { {"MedicineUsed"},   			ITEM_MedicineUsed, 		0 },
    { {"SandClockDetach"},			ITEM_SandClockDetach, 	0 },
    { {"addTitleAttach"}, 			ITEM_addTitleAttach, 	0 },
    { {"delTitleDetach"}, 			ITEM_delTitleDetach, 	0 },
    { {"ITEM_DeleteByWatched"}, 	ITEM_DeleteByWatched, 	0 },
    { {"ITEM_DeleteTimeWatched"}, 	ITEM_DeleteTimeWatched, 0 },
    { {"ITEM_useEffectTohelos"}, 	ITEM_useEffectTohelos, 	0 },

//    { {"ITEM_useHpRecovery"}, 	ITEM_useHpRecovery, 	0 },
    { {"ITEM_useRecovery"}, 		ITEM_useRecovery, 		0 },
#ifdef _ITEM_MAGICRECOVERY
	{ {"ITEM_useMRecovery"},	ITEM_useMRecovery,	0 },
#endif
#ifdef _ITEM_USEMAGIC
	{ {"ITEM_useMagic"},	ITEM_useMagic,	0 },
#endif
    { {"ITEM_useStatusChange"}, 	ITEM_useStatusChange, 	0 },
    { {"ITEM_useStatusRecovery"}, 	ITEM_useStatusRecovery, 0 },
    { {"ITEM_useMagicDef"}, 		ITEM_useMagicDef, 		0 },
    { {"ITEM_useParamChange"}, 		ITEM_useParamChange, 	0 },
    { {"ITEM_useFieldChange"}, 		ITEM_useFieldChange, 	0 },
    { {"ITEM_useAttReverse"}, 		ITEM_useAttReverse, 	0 },
    { {"ITEM_useRessurect"}, 		ITEM_useRessurect, 		0 },
    { {"ITEM_useMic"}, 				ITEM_useMic, 			0 },
    { {"ITEM_dropMic"}, 			ITEM_dropMic, 			0 },
    { {"ITEM_useCaptureUp"}, 		ITEM_useCaptureUp, 		0 },
    { {"ITEM_useRenameItem"}, 		ITEM_useRenameItem,		0 },
    { {"ITEM_pickupDice"}, 			ITEM_pickupDice,		0 },
    { {"ITEM_dropDice"}, 			ITEM_dropDice,			0 },
    { {"ITEM_initLottery"}, 		ITEM_initLottery,			0 },
    { {"ITEM_useLottery"}, 			ITEM_useLottery,			0 },
    { {"ITEM_useWarp"},	    ITEM_useWarp, 0 },
    { {"ITEM_petFollow"},   ITEM_petFollow, 0 },
    { {"ITEM_useSkup"},     ITEM_useSkup, 0 }, // Nuke 0624: Hero's bless
    { {"ITEM_useNoenemy"},  ITEM_useNoenemy, 0 }, // Nuke 0626: Dragon's help
    { {"ITEM_equipNoenemy"},ITEM_equipNoenemy, 0 }, // Arminius 7.2 Ra's amulet
    { {"ITEM_remNoenemy"},  ITEM_remNoenemy, 0 },   // Arminius 7.2 Ra's amulet
    { {"ITEM_useEncounter"}, ITEM_useEncounter, 0},     // Arminius 7.31 cursed stone

	{ {"ITEM_AddPRSkillPoint"}, ITEM_AddPRSkillPoint, 0},
	{ {"ITEM_AddPRSkillPercent"}, ITEM_AddPRSkillPercent, 0},

#ifdef _ITEM_METAMO
	{ {"ITEM_metamo"},   ITEM_metamo, 0 },
#endif

#ifdef _USEWARP_FORNUM
	{ {"ITEM_useWarpForNum"},	    ITEM_useWarpForNum, 0 },
#endif

#ifdef _IMPRECATE_ITEM
	{ {"ITEM_useImprecate"},	    ITEM_useImprecate, 0 },
#endif
#ifdef _ITEM_FIRECRACKER	//Terry add 2001/12/21
		{ {"ITEM_firecracker"}, ITEM_firecracker, 0 },
#endif
#ifdef _ITEM_CRACKER	//vincent 拉炮
		{ {"ITEM_Cracker"}, ITEM_Cracker, 0 },
#endif
#ifdef _ITEM_ADDEXP	//vincent 經驗提昇
		{ {"ITEM_Addexp"}, ITEM_Addexp, 0 },
#endif
#ifdef _ITEM_REFRESH //vincent 解除異常狀態道具
		{ {"ITEM_Refresh"}, ITEM_Refresh, 0 },
#endif

	{ {"ITEM_WearEquip"}, ITEM_WearEquip, 0 },
	{ {"ITEM_ReWearEquip"}, ITEM_ReWearEquip, 0 },

#ifdef _ITEM_CONSTITUTION
	{ {"ITEM_Constitution"}, ITEM_Constitution, 0 },
#endif

#ifdef _Item_ReLifeAct
	{ {"ITEM_DIErelife"}, ITEM_DIErelife, 0 },
#endif

#ifdef _ITEM_ORNAMENTS
	{ {"ITEM_PutOrnaments"}, ITEM_PutOrnaments, 0},
#endif
#ifdef _CHIKULA_STONE
	{ {"ITEM_ChikulaStone"}, ITEM_ChikulaStone, 0},
#endif

#ifdef _THROWITEM_ITEMS
	{ {"ITEM_ThrowItemBox"}, ITEM_ThrowItemBox, 0},
#endif

#ifdef _ITEM_WATERWORDSTATUS
	{ {"ITEM_WaterWordStatus"}, ITEM_WaterWordStatus, 0},
#endif

#ifdef _ITEM_LOVERPARTY
	{ {"ITEM_LoverSelectUser"}, ITEM_LoverSelectUser, 0},
#endif

#ifdef _Item_MoonAct
	{ {"ITEM_randEnemyEquipOne"}, ITEM_randEnemyEquipOne, 0 },

	{ {"ITEM_randEnemyEquip"}, ITEM_randEnemyEquip, 0 },
	{ {"ITEM_RerandEnemyEquip"}, ITEM_RerandEnemyEquip, 0},
#endif

#ifdef _SUIT_ITEM
	{ {"ITEM_suitEquip"},	ITEM_suitEquip,		0 },
	{ {"ITEM_ResuitEquip"},	ITEM_ResuitEquip,	0 },
#endif

#ifdef _Item_DeathAct
	{ {"ITEM_useDeathcounter"}, ITEM_UseDeathCounter, 0 },
#endif
#ifdef _DEATH_CONTENDWATCH
	{ {"ITEM_useWatchBattle"}, ITEM_useWatchBattle, 0 },
#endif
#ifdef _FEV_ADD_NEW_ITEM			// FEV ADD 增加復活守精
	{ {"ITEM_ResAndDef"} ,   ITEM_ResAndDef,        0 },
#endif

#ifdef _CHRISTMAS_REDSOCKS
	{ {"ITEM_useMaxRedSocks"}, ITEM_useMaxRedSocks, 0 },
#endif

#ifdef _CHRISTMAS_REDSOCKS_NEW
	{ {"ITEM_useMaxRedSocksNew"}, ITEM_useMaxRedSocksNew, 0 },
#endif

#ifdef _PETSKILL_CANNEDFOOD
	{ {"ITEM_useSkillCanned"}, ITEM_useSkillCanned, 0},
#endif

#ifdef _NEW_RIDEPETS
	{ {"ITEM_useLearnRideCode"}, ITEM_useLearnRideCode, 0 },
#endif

#ifdef _EQUIT_DEFMAGIC
	{ {"ITEM_MagicEquitWear"}, ITEM_MagicEquitWear, 0 },
	{ {"ITEM_MagicEquitReWear"}, ITEM_MagicEquitReWear, 0 },
#endif
#ifdef _EQUIT_RESIST
	{ {"ITEM_MagicResist"}, ITEM_MagicResist, 0 },
	{ {"ITEM_MagicReResist"}, ITEM_MagicReResist, 0 },
#endif

#ifdef _MAGIC_RESIST_EQUIT			// WON ADD 職業抗性裝備    
	{ {"ITEM_P_MagicEquitWear"},		ITEM_P_MagicEquitWear, 0 },
	{ {"ITEM_P_MagicEquitReWear"},	ITEM_P_MagicEquitReWear, 0 },
#endif

#ifdef _ANGEL_SUMMON	
	{ {"ITEM_AngelToken"}, ITEM_AngelToken, 0 },
	{ {"ITEM_HeroToken"}, ITEM_HeroToken, 0 },
#endif
#ifdef _HALLOWEEN_EFFECT
	{ {"ITEM_MapEffect"}, ITEM_MapEffect, 0 },
#endif

	{ {"ITEM_changePetOwner"}, 		ITEM_changePetOwner, 		0 },

	{ {"core_PetWatch"},   			PET_Watchfunc, 			0 },
    { {"PETMAIL_Loop"},   			PETMAIL_Loopfunc, 		0 },
#ifdef _USER_CHARLOOPS
	{ {"CHAR_BattleStayLoop"},		CHAR_BattleStayLoop,	0 },
	{ {"PET_CheckIncubateLoop"},	PET_CheckIncubate,		0 },
#endif

#ifdef _PETSKILL_PROPERTY
	{ {"PET_PetskillPropertyEvent"},	PET_PetskillPropertyEvent,		0 },
#endif

    { {"core_PetTalk"}, PET_Talkfunc, 0},	// Arminius 8.14 pet talk


	/* warp */
    { {"WarpInit"},					NPC_WarpInit, 			0 },
    { {"WarpPostOver"},				NPC_WarpPostOver, 		0 },
    { {"WarpWatch"},				NPC_WarpWatch,			0 },

     /* Dengon */
    { {"DengonInit"}, 				NPC_DengonInit,			0 },
    { {"DengonWindowTalked"}, 		NPC_DengonWindowTalked, 0 },
    { {"DengonLooked"}, 			NPC_DengonLooked, 		0 },
    
     /* FmDengon add code by shan */
    { {"FmDengonInit"}, 			NPC_FmDengonInit,         0 },
    { {"FmDengonWindowTalked"}, 		NPC_FmDengonWindowTalked, 0 },
    { {"FmDengonLooked"}, 			NPC_FmDengonLooked, 	  0 },

    /* Healer */
    { {"HealerInit"} ,				NPC_HealerInit,			0 },
    { {"HealerTalked"} ,			NPC_HealerTalked , 		0 },
    
    /* FMHealer add code by shan */
    { {"FmHealerInit"} ,			NPC_FmHealerInit,	0 },
    { {"FmHealerTalked"} ,			NPC_FmHealerTalked , 	0 },
    
    /* petmaker add code by shan */
    { {"PetMakerInit"} ,			NPC_PetMakerInit,	0 },
    { {"PetMakerTalked"} ,			NPC_PetMakerTalked , 	0 },    
    
    /* TownPeople */
    { {"TownPeopleTalked"},			NPC_TownPeopleTalked, 	0 },
    { {"TownPeopleInit"},			NPC_TownPeopleInit, 	0 },

    /* TownPeople */
    { {"MsgLooked"},				NPC_MsgLooked , 		0 },
    { {"MsgInit"},					NPC_MsgInit, 			0 },

    /* Oldman */
    { {"OldmanInit"} , 				NPC_OldmanInit , 		0 },
    { {"OldmanTalked"} ,			NPC_OldmanTalked ,		0 },

    /* SavePOint */
    { {"SavePointInit"} ,			NPC_SavePointInit , 	0 },
    { {"SavePointTalked"} ,			NPC_SavePointTalked , 	0 },
    { {"SavePointWindowTalked"},	NPC_SavePointWindowTalked, 0 },

    /* StoryTeller */
    { {"StoryTellerInit"} , 		NPC_StoryTellerInit , 	0 },
    { {"StoryTellerTalked"} , 		NPC_StoryTellerTalked , 0 },

    /* NPCEnemy */
    { {"NPCEnemyInit"} ,			NPC_NPCEnemyInit ,		0 },
    { {"NPCEnemyTalked"} ,			NPC_NPCEnemyTalked ,	0 },
    { {"NPCEnemyWatch"} ,			NPC_NPCEnemyWatch ,		0 },
	{ {"NPCEnemyLoop"},				NPC_NPCEnemyLoop,		0 },
    { {"NPCEnemyWindowTalked"}, 	NPC_NPCEnemyWindowTalked, 0 },

    /* アクション君 */
    { {"ActionInit"} ,				NPC_ActionInit ,		0 },
    { {"ActionTalked"} ,			NPC_ActionTalked ,		0 },
    { {"ActionWatch"} ,				NPC_ActionWatch ,		0 },

    /* ウィンドウ君 */
    { {"WindowmanInit"} , 			NPC_WindowmanInit ,		0 },
    { {"WindowmanTalked"} , 		NPC_WindowmanTalked ,	0 },
    { {"WindowmanLooked"}, 			NPC_WindowmanLooked,	0 },
    { {"WindowmanWindowTalked"}, 	NPC_WindowmanWindowTalked, 0 },

    /* ウィンドウヒーラー */
    { {"WindowHealerInit"} , 		NPC_WindowHealerInit ,	0 },
    { {"WindowHealerTalked"} , 		NPC_WindowHealerTalked,	0 },
    { {"WindowHealerLooked"} , 		NPC_WindowHealerLooked,	0 },
    { {"WindowHealerWindowTalked"}, NPC_WindowHealerWindowTalked, 0 },

	/* アイテム屋 */
    { {"ItemShopInit"} ,			NPC_ItemShopInit ,		0 },
    { {"ItemShopTalked"} ,			NPC_ItemShopTalked ,	0 },
    { {"ItemShopWindowTalked"},		NPC_ItemShopWindowTalked, 0 },

    /* Sysinfo */
    { {"SysinfoInit"},				NPC_SysinfoInit, 		0 },
    { {"SysinfoLoop"},				NPC_SysinfoLoop,		0 },
    { {"SysinfoTalked"} ,			NPC_SysinfoTalked ,		0 },

    /* Duelランキング表示NPC */
    { {"DuelrankingInit"} ,			NPC_DuelrankingInit ,	0 },
    { {"DuelrankingLooked"},		NPC_DuelrankingLooked,	0 },
    { {"DuelrankingWindowTalked"}, 	NPC_DuelrankingWindowTalked, 0 },
#ifdef _DEATH_CONTEND
	{ {"Duelrankingloop"}, 	NPC_Duelrankingloop, 0 },
#endif
    /* ウィンドウペットの技屋 */
    { {"PetSkillShopInit"} ,		NPC_PetSkillShopInit ,	0 },
    { {"PetSkillShopTalked"} ,		NPC_PetSkillShopTalked,	0 },
    { {"PetSkillShopLooked"} ,		NPC_PetSkillShopLooked,	0 },
    { {"PetSkillShopWindowTalked"},	NPC_PetSkillShopWindowTalked, 0 },

    /* ウィンドウペット買い取り屋 */
    { {"PetShopInit"} ,				NPC_PetShopInit, 		0 },
    { {"PetShopTalked"} ,			NPC_PetShopTalked,		0 },
    { {"PetShopLooked"} ,			NPC_PetShopLooked,		0 },
    { {"PetShopWindowTalked"},		NPC_PetShopWindowTalked,0 },

    /* 看板 */
    { {"SignBoardInit"} ,			NPC_SignBoardInit,		0 },
    { {"SignBoardLooked"} ,			NPC_SignBoardLooked,	0 },
    { {"SignBoardWindowTalked"},	NPC_SignBoardWindowTalked, 0 },

    /*ワープマン */
    { {"WarpManInit"},				NPC_WarpManInit,		0 },
    { {"WarpManTalked"},			NPC_WarpManTalked,		0 },
	{ {"WarpManWatch"},				NPC_WarpManWatch,		0 },
	{ {"WarpManLoop"} ,				NPC_WarpManLoop,		0 },
    { {"WarpManWindowTalked"},		NPC_WarpManWindowTalked,0 },


    /*イベント君（exchangeman) */
    { {"ExChangeManInit"},				NPC_ExChangeManInit,		0 },
    { {"ExChangeManTalked"},			NPC_ExChangeManTalked,		0 },
    { {"ExChangeManWindowTalked"},		NPC_ExChangeManWindowTalked,0 },

    /*タイムマン */
    { {"TimeManInit"},				NPC_TimeManInit,		0 },
    { {"TimeManTalked"},			NPC_TimeManTalked,		0 },
    { {"TimeManWatch"} ,			NPC_TimeManWatch ,		0 },

    /* ボディランゲージ */
    { {"BodyLanInit"},				NPC_BodyLanInit,		0 },
    { {"BodyLanTalked"},			NPC_BodyLanTalked,		0 },
    { {"BodyLanWatch"} ,			NPC_BodyLanWatch ,		0 },
    { {"BodyLanWindowTalked"} ,		NPC_BodyLanWindowTalked,0 },

    /* マイク */
    { {"MicInit"},				NPC_MicInit,		0 },
    { {"MicTalked"},			NPC_MicTalked,		0 },

    /* ラッキーマン */
    { {"LuckyManInit"} , 		NPC_LuckyManInit ,	0 },
    { {"LuckyManTalked"} , 		NPC_LuckyManTalked,	0 },
    { {"LuckyManWindowTalked"}, NPC_LuckyManWindowTalked, 0 },

    /* 君 */
    { {"BusInit"} ,				NPC_BusInit ,		0 },
    { {"BusTalked"} ,			NPC_BusTalked ,		0 },
    { {"BusLoop"} ,				NPC_BusLoop ,		0 },

    /* 加美航空 */      // Arminius 7.7 Airplane
    { {"AirInit"} ,     NPC_AirInit ,           0 },
    { {"AirTalked"} ,   NPC_AirTalked ,         0 },
    { {"AirLoop"} ,     NPC_AirLoop ,           0 },

    /* 魅力ＵＰ君 */
    { {"CharmInit"} , 		NPC_CharmInit ,	0 },
    { {"CharmTalked"} , 		NPC_CharmTalked,	0 },
    { {"CharmWindowTalked"}, NPC_CharmWindowTalked, 0 },
	
    { {"PoolItemShopInit"} ,			NPC_PoolItemShopInit ,		0 },
	{ {"PoolItemShopLoop"} ,			NPC_PoolItemShopLoop ,		0 },
    { {"PoolItemShopTalked"} ,			NPC_PoolItemShopTalked ,	0 },
    { {"PoolItemShopWindowTalked"},		NPC_PoolItemShopWindowTalked, 0 },

    { {"QuizInit"} , 		NPC_QuizInit ,	0 },
    { {"QuizTalked"} , 		NPC_QuizTalked,	0 },
    { {"QuizWindowTalked"}, NPC_QuizWindowTalked, 0 },
	

    /* ちぇっくまん */
    { {"CheckManInit"} , 		NPC_CheckManInit ,	0 },
    { {"CheckManTalked"} , 		NPC_CheckManTalked,	0 },
    { {"CheckManWindowTalked"}, NPC_CheckManWindowTalked, 0 },

    /* じゃんけん君 */
    { {"JankenInit"} , 		NPC_JankenInit ,	0 },
    { {"JankenTalked"} , 		NPC_JankenTalked,	0 },
    { {"JankenWindowTalked"}, NPC_JankenWindowTalked, 0 },

    /* 転生人 */
    { {"TransmigrationInit"} , 		NPC_TransmigrationInit ,	0 },
    { {"TransmigrationTalked"} , 		NPC_TransmigrationTalked,	0 },
    { {"TransmigrationWindowTalked"}, NPC_TransmigrationWindowTalked, 0 },

    /* Family man */
    { {"FamilymanInit"} ,          NPC_FamilymanInit ,        0 },
    { {"FamilymanTalked"} ,                NPC_FamilymanTalked,       0 },
    { {"FamilymanLooked"},	NPC_FamilymanLooked,0 },    
    { {"FamilymanWindowTalked"}, NPC_FamilymanWindowTalked, 0 },
                
    /* CoolFish: Family WarpMan 2001/6/6 */
    { {"FMWarpManInit"},		NPC_FMWarpManInit,		0 },
    { {"FMWarpManTalked"},		NPC_FMWarpManTalked,		0 },
    { {"FMWarpManLoop"} ,		NPC_FMWarpManLoop,		0 },
    { {"FMWarpManWindowTalked"},	NPC_FMWarpManWindowTalked,	0 },

    /* CoolFish: Family PKMan 2001/7/4 */
    { {"FMPKManInit"},		NPC_FMPKManInit,		0 },
    { {"FMPKManTalked"},		NPC_FMPKManTalked,		0 },
    { {"FMPKManWindowTalked"},	NPC_FMPKManWindowTalked,	0 },

    /* CoolFish: Family PKCallMan 2001/7/13 */
    { {"FMPKCallManInit"},		NPC_FMPKCallManInit,		0 },
    { {"FMPKCallManTalked"},		NPC_FMPKCallManTalked,		0 },
    { {"FMPKCallManWindowTalked"},	NPC_FMPKCallManWindowTalked,	0 },
    
    /* Bank man */
    { {"BankmanInit"} ,          NPC_BankmanInit ,        0 },
    { {"BankmanTalked"} ,                NPC_BankmanTalked,       0 },
    { {"BankmanLooked"},	NPC_BankmanLooked,0 },
    { {"BankmanWindowTalked"}, NPC_BankmanWindowTalked, 0 },

    /* Arminius 7.13 scheduleman */
    { {"SchedulemanInit"},	NPC_SchedulemanInit,	0},
    { {"SchedulemanTalked"},	NPC_SchedulemanTalked,	0},
    { {"SchedulemanWindowTalked"},	NPC_SchedulemanWindowTalked,	0},
    { {"SchedulemanLoop"},	NPC_SchedulemanLoop,	0},

    /* Arminius 7.24 manor scheduleman */
    { {"ManorSmanInit"},	NPC_ManorSmanInit,	0},
    { {"ManorSmanTalked"},	NPC_ManorSmanTalked,	0},
    { {"ManorSmanWindowTalked"},	NPC_ManorSmanWindowTalked,	0},
    { {"ManorSmanLoop"},	NPC_ManorSmanLoop,	0},

    /* Rider man */
    { {"RidermanInit"} ,          NPC_RidermanInit ,        0 },
    { {"RidermanTalked"} ,                NPC_RidermanTalked,       0 },
    { {"RidermanLooked"},	NPC_RidermanLooked,0 },    
    { {"RidermanWindowTalked"}, NPC_RidermanWindowTalked, 0 },

    /* FmLetter man */
    { {"FmLetterInit"} ,          NPC_FmLetterInit ,        0 },
    { {"FmLetterTalked"} ,                NPC_FmLetterTalked,       0 },
    { {"FmLetterLooked"},	NPC_FmLetterLooked,0 },    
    { {"FmLetterWindowTalked"}, NPC_FmLetterWindowTalked, 0 },
#ifdef _SERVICE    
    // Terry 2001/08/31
    // 石器服務員 StoneServiceMan
    { {"StoneServiceManInit"},         NPC_StoneServiceManInit,0},
    { {"StoneServiceManLoop"},         NPC_StoneServiceManLoop,0},
    { {"StoneServiceManTalked"},       NPC_StoneServiceManTalked,0},
    { {"StoneServiceManWindowTalked"}, NPC_StoneServiceManWindowTalked,0},
#endif 

#ifdef _GAMBLE_BANK	//銀行
	{ {"GambleBankInit"},			NPC_GambleBankInit, 0},
	{ {"GambleBankLoop"},			NPC_GambleBankLoop,0},
    { {"GambleBankTalked"},			NPC_GambleBankTalked,0},
    { {"GambleBankWindowTalked"},	NPC_GambleBankWindowTalked,0},
#endif

#ifdef _PET_LIMITLEVEL
	{ {"ITEM_useOtherEditBase"}, ITEM_useOtherEditBase, 0},
#endif
#ifdef _ITEM_EDITBASES
	{ {"ITEM_useFusionEditBase"}, ITEM_useFusionEditBase, 0},
#endif
#ifdef _GAMBLE_ROULETTE //賭場輪盤
	{ {"GambleRouletteInit"},			NPC_Gamble_RouletteInit, 0},
	{ {"GambleRouletteLoop"},			NPC_Gamble_RouletteLoop, 0},
    { {"GambleRouletteTalked"},		NPC_Gamble_RouletteTalked, 0},
	{ {"GambleRouletteWindowTalked"}, NPC_Gamble_RouletteWindowTalked, 0},

	{ {"GambleMasterInit"},			NPC_Gamble_MasterInit, 0},
	{ {"GambleMasterLoop"},			NPC_Gamble_MasterLoop, 0},
    { {"GambleMasterTalked"},		NPC_Gamble_MasterTalked, 0},
	{ {"GambleMasterWindowTalked"}, NPC_Gamble_MasterWindowTalked, 0},
#endif

#ifdef _TRANSER_MAN
    { {"TranserManInit"},			NPC_TranserManInit,			0 },
    { {"TranserManTalked"},			NPC_TranserManTalked,		0 },
	{ {"TranserManLoop"} ,			NPC_TranserManLoop,			0 },
    { {"TranserManWindowTalked"},	NPC_TranserManWindowTalked,	0 },
#endif

#ifdef _NPC_SELLSTH
	{ {"SellsthManInit"},			NPC_SellsthManInit,			0 },
	{ {"SellsthManTalked"},			NPC_SellsthManTalked,		0 },
	{ {"SellsthManLoop"} ,			NPC_SellsthManLoop,			0 },
	{ {"SellsthManWindowTalked"},	NPC_SellsthManWindowTalked,	0 },
#endif

#ifdef _NPC_MAKEPAIR
	{ {"MakePairManInit"},			NPC_MakePairManInit,		0 },
    { {"MakePairManTalked"},		NPC_MakePairManTalked,		0 },
	{ {"MakePairManLoop"} ,			NPC_MakePairManLoop,		0 },
    { {"MakePairManWindowTalked"},	NPC_MakePairManWindowTalked,0 },
#endif
#ifdef _NPC_FUSION
	{ {"PetFusionManInit"},			NPC_PetFusionManInit,		0 },
    { {"PetFusionManTalked"},		NPC_PetFusionManTalked,		0 },
	{ {"PetFusionManLoop"} ,		NPC_PetFusionManLoop,		0 },
    { {"PetFusionManWindowTalked"},	NPC_PetFusionManWindowTalked,0 },
#endif
#ifdef _PAUCTION_MAN
    { {"PauctionManInit"},			NPC_PauctionManInit,			0 },
    { {"PauctionManTalked"},		NPC_PauctionManTalked,		0 },
	{ {"PauctionManLoop"} ,			NPC_PauctionManLoop,			0 },
    { {"PauctionManWindowTalked"},	NPC_PauctionManWindowTalked,	0 },
#endif
#ifdef _ITEM_NPCCHANGE
    { {"ItemchangeManInit"},		NPC_ItemchangeManInit,			0 },
    { {"ItemchangeManTalked"},		NPC_ItemchangeManTalked,		0 },
	{ {"ItemchangeManLoop"} ,		NPC_ItemchangeManLoop,			0 },
    { {"ItemchangeManWindowTalked"},		NPC_ItemchangeManWindowTalked,	0 },
#endif

#ifdef _CFREE_petskill
    { {"FreePetSkillInit"} ,		NPC_FreePetSkillShopInit,	0 },
    { {"FreePetSkillTalked"} ,		NPC_FreePetSkillShopTalked,	0 },
    { {"FreePetSkillWindowTalked"},	NPC_FreePetSkillShopWindowTalked, 0 },
#endif

#ifdef _PETRACE // 寵物競速
	{ {"PetRaceMasterInit"},			NPC_PetRaceMasterInit, 0},
	{ {"PetRaceMasterLoop"},			NPC_PetRaceMasterLoop, 0},
    { {"PetRaceMasterTalked"},		NPC_PetRaceMasterTalked, 0},
	{ {"PetRaceMasterWindowTalked"}, NPC_PetRaceMasterWindowTalked, 0},

	{ {"PetRacePetInit"},	NPC_PetRacePetInit, 0},
	{ {"PetRacePetLoop"},	NPC_PetRacePetLoop, 0},
	{ {"PetRacePetTalked"},	NPC_PetRacePetTalked, 0},
#endif

#ifdef _NEW_WARPMAN
	{ {"NewNpcManInit"},			NPC_NewNpcManInit, 0},
	{ {"NewNpcManLoop"},			NPC_NewNpcManLoop, 0},
    { {"NewNpcManTalked"},			NPC_NewNpcManTalked, 0},
    { {"NewNpcManWindowTalked"},	NPC_NewNpcManWindowTalked, 0},
#endif

#ifdef _MARKET_TRADE
	{ {"MapTradeManInit"},			MapTradeManInit, 0},
	{ {"MapTradeManLoop"},			MapTradeManLoop, 0},
    { {"MapTradeManTalked"},		MapTradeManTalked, 0},
    { {"MapTradeManWindowTalked"},	MapTradeManWindowTalked, 0},
#endif

#ifdef _AUCTIONEER
    { {"AuctioneerInit"},			NPC_AuctioneerInit, 0},
    { {"AuctioneerTalked"},			NPC_AuctioneerTalked, 0},
    { {"AuctioneerWindowTalked"},	NPC_AuctioneerWindowTalked, 0},
    { {"AuctioneerLoop"},			NPC_AuctioneerLoop, 0},
#endif

#ifdef _BLACK_MARKET
    { {"BlackMarketInit"},			   NPC_BlackMarketInit, 0},
	{ {"BlackMarketTalked"},		   NPC_BlackMarketTalked, 0},
	{ {"BlackMarketWindowTalked"},	   NPC_BlackMarketWindowTalked, 0},
#endif

#ifdef _ALLDOMAN   // (不可開) Syu ADD 排行榜NPC
    { {"AlldomanInit"} ,                        NPC_AlldomanInit,        0 },
    { {"AlldomanTalked"} ,                      NPC_AlldomanTalked ,     0 },
    { {"AlldomanWindowTalked"},                 NPC_AlldomanWindowTalked , 0},
#endif

#ifdef _NPC_WELFARE
    { {"WelfareInit"} ,                        NPC_WelfareInit,        0 },
    { {"WelfareTalked"} ,                      NPC_WelfareTalked ,     0 },
    { {"WelfareWindowTalked"},                 NPC_WelfareWindowTalked , 0},
#endif

#ifdef _NPC_WELFARE_2				// WON ADD 職業NPC-2
    { {"WelfareInit2"} ,                        NPC_WelfareInit2,        0 },
    { {"WelfareTalked2"} ,                      NPC_WelfareTalked2,     0 },
    { {"WelfareWindowTalked2"},                 NPC_WelfareWindowTalked2, 0},
#endif
	
#ifdef _NPC_VERYWELFARE
    { {"VeryWelfareInit"} ,                        NPC_VeryWelfareInit,        0 },
    { {"VeryWelfareTalked"} ,                      NPC_VeryWelfareTalked ,     0 },
    { {"VeryWelfareWindowTalked"},                 NPC_VeryWelfareWindowTalked , 0},
#endif

#ifdef _CONTRACT
	{ {"ITEM_contract"}, ITEM_contract, 0},
#endif

#ifdef _TIME_TICKET
	{ {"ITEM_timeticket"}, ITEM_timeticket, 0},
#endif

#ifdef _RACEMAN
    { {"RacemanInit"} ,                        NPC_RacemanInit,			0 },
	{ {"RacemanTalked"} ,					   NPC_RacemanTalked,		0 },
    { {"RacemanWindowTalked"},                 NPC_RacemanWindowTalked ,0 },
#endif

};

BOOL initFunctionTable( void )
{

    int     i;
    {
        char*   strings[arraysizeof(correspondStringAndFunctionTable)];
        int     stringnum=0;
        for( i=0 ; i<arraysizeof(correspondStringAndFunctionTable) ; i++ )
            strings[stringnum++] =
                correspondStringAndFunctionTable[i].functionName.string;
        if( ! checkStringsUnique( strings, stringnum , 1 ) ){
            fprint("Function Name Table is overlapped.\n" );
            fprint("It is not allowed\n");
            return FALSE;
        }
    }

    for(i = 0; i < arraysizeof(correspondStringAndFunctionTable) ; i ++ ){
        correspondStringAndFunctionTable[i].hashcode =
            hashpjw( correspondStringAndFunctionTable[i].
                     functionName.string);
    }
    return TRUE;
}

void* getFunctionPointerFromName( char* funcname )
{
    int     i;
    int     hashcode;
    if( funcname == NULL || funcname[0] == '\0' ){
		return NULL;
	}
    hashcode = hashpjw( funcname );
    for( i=0 ; i<arraysizeof(correspondStringAndFunctionTable) ; i++ )
        if( correspondStringAndFunctionTable[i].hashcode == hashcode )
            if( strcmp( correspondStringAndFunctionTable[i].functionName.string,funcname ) == 0 ){
				DebugFunctionName = correspondStringAndFunctionTable[i].functionName.string;
                return correspondStringAndFunctionTable[i].functionPointer;
            }

#ifdef DEBUG
    print("No such Function: %s\n" ,funcname );
#endif
    return NULL;
}
