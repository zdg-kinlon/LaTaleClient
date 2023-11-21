// Copyright (C) metalgeni
//*****************************************************************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-19 오전 10:26:19
//
// modified history :
//
//					metalgeni 2004/10/12 ---------------------------------------------------------------------
//					removed SPGOBPartPool
//					removed SPGOBPart
//
//					appended
//					SPGOBInfoSequencePart	- 애니메숑 파트 시퀀스 정보
//					SPGOBPartModel			- 파츠고유번호, 색깔번호, 텍스쳐등 아이템에 따른 파트정보
//
//
//					metalgeni 2004/11/5 ---------------------------------------------------------------------
//					SPGOBClusterDEF에 있는 애니메이션과 모델을 두개의 모듈로분리.
//					SPGOBClusterDEF는 그대로 존재한다.
//					SPGOBAnimation	- 애니메이션에 관련된 모듈
//					SPGOBModel		- Part정보와 모델정보에 관련된 모듈
//										모듈만 분리되었을뿐 구조가 변경된건 아님.
//										단 SPGOBModel은 struct에서 class로 변경.
//
//					
//*****************************************************************************************************************************

#pragma once


//#define IS_VALID_ANI_INDEX(x)	( (x)>=ANI_BEGIN && (x)<ANI_NUM )? true:false

enum GOB_SEQ_INDEX
{
	GOB_SEQ_NULL = 0,

	GOB_SEQ_STAND,
	GOB_SEQ_WALK,
	GOB_SEQ_JUMP,
	GOB_SEQ_FALL,
	GOB_SEQ_LAND,
	GOB_SEQ_RUN,
	GOB_SEQ_RUN_STOP,
	GOB_SEQ_VERROPE_STOP,
	GOB_SEQ_VERROPE_MOVE_UP,
	GOB_SEQ_VERROPE_MOVE_DOWN,
	GOB_SEQ_LADDER_STOP,
	GOB_SEQ_LADDER_MOVE_UP,
	GOB_SEQ_LADDER_MOVE_DOWN,
	GOB_SEQ_HANGING_STOP,
	GOB_SEQ_HANGING_MOVE,
	GOB_SEQ_ATTACK_NORMAL01,
	GOB_SEQ_ATTACK_NORMAL02,
	GOB_SEQ_ATTACK_NORMAL03,
	GOB_SEQ_ATTACK_HEAVY01,
	GOB_SEQ_ATTACK_HEAVY02,
	GOB_SEQ_JUMP_ATTACK_NORMAL01,
	GOB_SEQ_JUMP_ATTACK_HEAVY01,
	GOB_SEQ_GUARD,
	GOB_SEQ_DAMAGE_NORMAL01,
	GOB_SEQ_DAMAGE_CRITICAL,
	GOB_SEQ_DOWN,
	GOB_SEQ_DOWN_STAND,
	GOB_SEQ_STUN,
	GOB_SEQ_DEAD,
	GOB_SEQ_LOOTING,
	GOB_SEQ_LOOTING_STAND,
	GOB_SEQ_SIT_DOWN,
	GOB_SEQ_SIT_WAIT,
	GOB_SEQ_SIT_STAND,
	GOB_SEQ_CHAIR_SIT_DOWN,
	GOB_SEQ_CHAIR_SIT_DOWN_WAIT,
	GOB_SEQ_CHAIR_SIT_STAND,
	GOB_SEQ_GUARD_DAMAGE,
	GOB_SEQ_DEATH_STAND,
	GOB_SEQ_ATTACK_NORMAL04,
	GOB_SEQ_ATTACK_NORMAL05,
	GOB_SEQ_ATTACK_NORMAL06,
	GOB_SEQ_ATTACK_NORMAL07,
	GOB_SEQ_ATTACK_NORMAL08,
	GOB_SEQ_ATTACK_NORMAL09,
	GOB_SEQ_ATTACK_NORMAL10,
	GOB_SEQ_ATTACK_HEAVY03,
	GOB_SEQ_ATTACK_HEAVY04,
	GOB_SEQ_ATTACK_HEAVY05,
	GOB_SEQ_ATTACK_HEAVY06,
	GOB_SEQ_ATTACK_HEAVY07,
	GOB_SEQ_ATTACK_HEAVY08,
	GOB_SEQ_ATTACK_HEAVY09,
	GOB_SEQ_ATTACK_HEAVY10,
	GOB_SEQ_ENCHANTTING,
	GOB_SEQ_ENCHANT_SUCCESSED,
	GOB_SEQ_ENCHANT_FAILED,
	GOB_SEQ_USERSHOP01,
	GOB_SEQ_USERSHOP02,
	GOB_SEQ_USERSHOP03,
	GOB_SEQ_USERSHOP04,
	GOB_SEQ_USERSHOP05,
	GOB_SEQ_DOUBLE_JUMP,
	GOB_SEQ_DOUBLE_JUMP_FALL,
	GOB_SEQ_DOUBLE_JUMP_LAND,
	GOB_SEQ_DAMAGE_NORMAL02,
	GOB_SEQ_DAMAGE_NORMAL03,
	GOB_SEQ_DAMAGE_NORMAL04,
	GOB_SEQ_DAMAGE_NORMAL05,
};

enum ANIMATION_INDEX {
	ANI_NULL			= -1,

	ANI_MONSTER_BEGIN	= 0,
	ANI_MONSTER_STAND	 = ANI_MONSTER_BEGIN,
	ANI_MONSTER_WALK	,
	ANI_MONSTER_ATTACK	,
	ANI_MONSTER_BEATTACKED,
	ANI_MONSTER_JUMP,
	ANI_MONSTER_FALL,
	ANI_MONSTER_LAND,
	ANI_MONSTER_DEAD,
	ANI_MONSTER_DASH,
	ANI_MONSTER_SKILL01,
	ANI_MONSTER_SKILL02,
	ANI_MONSTER_SKILL03,
	ANI_MONSTER_SKILL04,
	ANI_MONSTER_SKILL05,
	ANI_MONSTER_SIT_DOWN,
	ANI_MONSTER_SIT_WAIT,
	ANI_MONSTER_SIT_UP,
	ANI_MONSTER_GUARD,
	ANI_MONSTER_GUARD_DAMAGE,
	ANI_MONSTER_BEATTACKED02,
	ANI_MONSTER_BEATTACKED03,
	ANI_MONSTER_BEATTACKED04,
	ANI_MONSTER_BEATTACKED05,
	ANI_MONSTER_RESERVE01,
	ANI_MONSTER_RESERVE02,
	ANI_MONSTER_RESERVE03,
	ANI_MONSTER_RESERVE04,
	ANI_MONSTER_RESERVE05,
	ANI_MONSTER_RESERVE06,
	ANI_MONSTER_RESERVE07,
	ANI_MONSTER_RESERVE08,
	ANI_MONSTER_RESERVE09,
	ANI_MONSTER_RESERVE10,
	ANI_MONSTER_END		= ANI_MONSTER_RESERVE10,

	ANI_NPC_BEGIN			,
	ANI_NPC_STAND01		= ANI_NPC_BEGIN	,
	ANI_NPC_STAND02		,
	ANI_NPC_STAND03		,
	ANI_NPC_END			= ANI_NPC_STAND03,

	ANI_NUM				,		// Index 가 아닌 갯수!!!
};

enum PART_LAYER {
	PL_NULL				= 0x0000,

	PL_FIRST			= 0x0001,

	PL_WEAPON_OUT		= 0x0001,
	PL_BLOUSE_ARM_OUT	= 0x0002,	
	PL_GLOVE_OUT		= 0x0003,
	PL_HAND_OUT			= 0x0004,
	PL_HAND_ARM_OUT		= 0x0005,
	PL_ARM_OUT			= 0x0006,
	PL_PANTS_LEG_OUT	= 0x0007,	
	PL_SHOE_OUT			= 0x0008,
	PL_FOOT_OUT			= 0x0009,
	PL_FOOT_LEG_OUT		= 0x000A,
	PL_LEG_OUT			= 0x000B,	
	PL_BLOUSE			= 0x000C,
	PL_PANTS			= 0x000D,
	PL_BODY				= 0x000E,
	PL_PANTS_LEG_IN		= 0x000F,
	PL_SHOE_IN			= 0x0010,
	PL_FOOT_IN			= 0x0011,
	PL_FOOT_LEG_IN		= 0x0012,
	PL_LEG_IN			= 0x0013,
	PL_CAP_FRONT		= 0x0014,
	PL_GOGGLE			= 0x0015,
	PL_EAR				= 0x0016,
	PL_HAIR_FRONT		= 0x0017,
	PL_HAIR_REAR		= 0x0018,
	PL_FACE				= 0x0019,
	PL_MAKEUP			= 0x001A,
	PL_HEAD				= 0x001B,
	PL_CAP_REAR			= 0x001C,
	PL_BLOUSE_ARM_IN	= 0x001D,	
	PL_GLOVE_IN			= 0x001E,
	PL_HAND_IN			= 0x001F,
	PL_HAND_ARM_IN		= 0x0020,
	PL_ARM_IN			= 0x0021,
	PL_WEAPON_IN		= 0x0022, // 방패
	PL_COAT				= 0x0023,	

	PL_LAST				= 0x0025,
};


static const char *g_strAnimationIndex[ANI_NUM] = { 
		"ANI_MONSTER_STAND",
		"ANI_MONSTER_WALK",
		"ANI_MONSTER_ATTACK",
		"ANI_MONSTER_BEATTACKED",
		"ANI_MONSTER_JUMP",
		"ANI_MONSTER_FALL",
		"ANI_MONSTER_LAND",
		"ANI_MONSTER_DEAD",
		"ANI_MONSTER_DASH",
		"ANI_MONSTER_SKILL01",
		"ANI_MONSTER_SKILL02",
		"ANI_MONSTER_SKILL03",
		"ANI_MONSTER_SKILL04",
		"ANI_MONSTER_SKILL05",
		"ANI_MONSTER_SIT_DOWN",
		"ANI_MONSTER_SIT_WAIT",
		"ANI_MONSTER_SIT_UP",
		"ANI_MONSTER_GUARD",
		"ANI_MONSTER_GUARD_DAMAGE",
		"ANI_MONSTER_BEATTACKED02",
		"ANI_MONSTER_BEATTACKED03",
		"ANI_MONSTER_BEATTACKED04",
		"ANI_MONSTER_BEATTACKED05",
		"ANI_MONSTER_RESERVE01",
		"ANI_MONSTER_RESERVE02",
		"ANI_MONSTER_RESERVE03",
		"ANI_MONSTER_RESERVE04",
		"ANI_MONSTER_RESERVE05",
		"ANI_MONSTER_RESERVE06",
		"ANI_MONSTER_RESERVE07",
		"ANI_MONSTER_RESERVE08",
		"ANI_MONSTER_RESERVE09",
		"ANI_MONSTER_RESERVE10",
		"ANI_NPC_STAND01",
		"ANI_NPC_STAND02",
		"ANI_NPC_STAND03",
};

// Part resource file bound header name.
#define RI_SX	0
#define RI_SY	1
#define RI_NOX	2
#define RI_NOY	3

extern const char* g_cstrPartLayer[];
extern const int g_ciCharResIndex[][4];
extern const char* g_strAnimationIndex[ANI_NUM];

const char* GetHeadString(ANIMATION_INDEX index);

#ifndef _LATALE_SERVER
bool GetResIndexRect(PART_LAYER PartLayer, int iIndex, RECT& rect );
#endif

#if defined(_USED_TEXTURE_TABLE)
struct TEXTURE_TABLE_INFO;
struct FRAME_INFO;

FRAME_INFO* GetResIndexRectbyTable(TEXTURE_TABLE_INFO* pTableInfo, int iIndex, RECT& rect);
#endif





