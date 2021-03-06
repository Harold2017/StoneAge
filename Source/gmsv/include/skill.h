#ifndef __SKILL_H__
#define __SKILL_H__

#include "common.h"
#include "util.h"

typedef struct tagSKILL_intDataSetting
{
    char    *dumpskill;
}SKILL_intDataSetting;

typedef struct tagSKILL_charDataSetting
{
    char    *dumpskill;
}SKILL_charDataSetting;


typedef enum
{
#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
    SKILL_1,
    SKILL_2,
    SKILL_3,
    SKILL_4,
    SKILL_5,
    SKILL_6,
    SKILL_7,
    SKILL_8,
    SKILL_9,
    SKILL_10,
    SKILL_11,
    SKILL_12,
    SKILL_13,
    SKILL_14,
    SKILL_15,
    SKILL_16,	
	SKILL_NUM
#else
    SKILL_FIRE,
    SKILL_MAGICIAN,
    SKILL_PRIEST,
    SKILL_ALOTOFTHINGS,
    SKILL_AVOIDRATEUP,
    SKILL_DETERMINEITEM,
    SKILL_DETERMINEOTHERS,
    SKILL_FIRECUT,
    SKILL_THUNDERCUT,
    SKILL_ICECUT,
    SKILL_MERCHANT,
    SKILL_HEALER,
    SKILL_FIST,
    SKILL_SWORD,
    SKILL_AXE,
    SKILL_SPEAR,
    SKILL_BOW,
    SKILL_CANE,
    SKILL_LARGEVOICE,
    SKILL_NUM
#endif
}SKILL_ID;


typedef struct tagSkillTable
{
    const int       maxlevel;
    void*           effectfunc;                                          
}SKILL_table;


typedef enum
{
    SKILL_LEVEL,		// 技能等級
    SKILL_IDENTITY,		// 技能編號
    SKILL_DATAINTNUM,
}SKILL_DATAINT;

typedef enum
{
    SKILL_DATACHARNUM,
}SKILL_DATACHAR;


typedef struct tagSkill
{
    int         data[SKILL_DATAINTNUM];
#if PLATFORM_WINDOWS
#else
    STRING32    string[SKILL_DATACHARNUM];
#endif
}Skill;

char* SKILL_makeStringFromSkillData( Skill* sk );
BOOL  SKILL_makeSkillFromStringToArg( char* src, Skill* sk );

int SKILL_getInt( Skill* skill, int element);
int SKILL_setInt( Skill* skill, int element, int newInt);

#ifdef _CHAR_PROFESSION			// WON ADD 人物職業
char* SKILL_makeSkillStatusString( Skill* skill, int charaindex, int skill_num );
#else
char* SKILL_makeSkillStatusString( Skill* skill );
#endif

char* SKILL_makeSkillFalseString( void );

BOOL SKILL_makeSkillData( Skill* sk ,int skid, int lev );

int SKILL_levelup( Skill* sk );
int SKILL_getLevelFromSkillID( int charaindex, SKILL_ID id );
BOOL SKILL_getUpableSkillID( int charaindex,char* buf, int buflen );
void SKILL_skillEffect( int charaindex );

#ifdef _PROFESSION_SKILL			// WON ADD 人物職業技能
int PROFESSION_MAGIC_COST_MP( int charaindex, int skill_num );
INLINE int PROFESSION_CHANGE_SKILL_LEVEL_M( int skill_level );
INLINE int PROFESSION_CHANGE_SKILL_LEVEL_A( int skill_level );
INLINE int SKILL_getRealInt( Skill* skill, int element);
#endif


#endif 
/*__SKILL_H__*/


