#include "version.h"
#include <string.h>
#include "object.h"
#include "char_base.h"
#include "char.h"
#include "util.h"
#include "npcutil.h"
#include "npc_oldman.h"
#include "title.h"
#include "char_data.h"
#include "readmap.h"
#include "lssproto_serv.h"
#include "log.h"
#include "handletime.h"





enum {
	E_INT_GRAPHIC = CHAR_NPCWORKINT1,		/* 変身前の画像 */
	E_INT_GRAPHIC2 = CHAR_NPCWORKINT2,		/* 変身後の画像 */
	E_INT_BORN = CHAR_NPCWORKINT3,			/* 誕生時間 */
	E_INT_DEAD = CHAR_NPCWORKINT4,			/* 消える時間*/ 
	E_INT_MODE = CHAR_NPCWORKINT5,			/* 現在のモード */
	E_INT_NOWGRAPHIC = CHAR_NPCWORKINT6,	/* 現在のグラフィックナンバー */

};


typedef struct {
	char	arg[32];
	int		born;
	int 	dead;
}NPC_TimeMan;


#define YOAKE 700
#define NICHIBOTU 300
#define SHOUGO 125
#define YONAKA 500

static NPC_TimeMan	TimeTble[] = {

	{"ALLNIGHT",	NICHIBOTU+1,	YOAKE}, 	/* 太陽が出ていない時 */
	{"ALLNOON",		YOAKE+1,		NICHIBOTU},/* 太陽が出ている時 */
	{"AM",			YONAKA+1,		SHOUGO}, 	/* AM 時のみ */
	{"PM",			SHOUGO+1,		YONAKA}, 	/* PM 時のみ */
	{"FORE",		YOAKE+1,		SHOUGO},	/* 太陽がでてから正午まで */
	{"AFTER",		SHOUGO+1,		NICHIBOTU},/* 正午から太陽が沈むまで */
	{"EVNING",		NICHIBOTU+1,	YONAKA}, 	/* 太陽が沈んでから真夜中まで */
	{"MORNING",		YONAKA+1,		YOAKE}, 	/* 真夜中から太陽が昇るまで */
	{"FREE",		0,				1024},
};




BOOL NPC_TimeManInit( int meindex )
{

	int i=0;
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char buf[32];
	
	if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr))==NULL){
		print("GetArgStrErr");
		return FALSE;
	}

	CHAR_setInt( meindex , CHAR_WHICHTYPE , CHAR_TYPETOWNPEOPLE );
	if( NPC_Util_GetStrFromStrWithDelim( argstr, "change_no", buf, sizeof( buf)) != NULL ){
		if(strstr(buf,"CLS")!=NULL){
			CHAR_setWorkInt( meindex,E_INT_GRAPHIC2, 9999 );
		}else{
			CHAR_setWorkInt( meindex,E_INT_GRAPHIC2, atoi(buf) );
		}
	}else{
		CHAR_setWorkInt( meindex,E_INT_GRAPHIC2, 9999 );
	}

	CHAR_setWorkInt( meindex, E_INT_GRAPHIC, CHAR_getInt( meindex, CHAR_BASEIMAGENUMBER) );

	if( NPC_Util_GetStrFromStrWithDelim( argstr, "time", buf, sizeof( buf)) != NULL ){
		for(i=0 ; i<9 ; i++){
			if(strstr(buf,TimeTble[i].arg) != NULL){
				CHAR_setWorkInt( meindex, E_INT_BORN, TimeTble[i].born);
				CHAR_setWorkInt( meindex, E_INT_DEAD, TimeTble[i].dead);
				return TRUE;
			}
		}
	}
	
	return FALSE;

}


void NPC_TimeManTalked( int meindex , int talkerindex , char *msg ,int color )
{
	char	argstr[NPC_UTIL_GETARGSTR_BUFSIZE];
	char token[512];
	char buf[512];
	int tokennum=0;
	int i;


	/*--目の前にいるかどうか？--*/
	if(NPC_Util_isFaceToFace( meindex ,talkerindex ,2)==FALSE){
		/* １グリッド以内のみ */
		if( NPC_Util_CharDistance( talkerindex ,meindex ) > 1) return;
	}

	if(CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER)==9999){
		/*--消えているモード--*/
		return;
	}

	if(NPC_Util_GetArgStr( meindex, argstr, sizeof(argstr))==NULL){
		print("GetArgStrErr");	
		return;
	}
	

	if( CHAR_getInt(talkerindex,CHAR_WHICHTYPE) != CHAR_TYPEPLAYER )return;
	
	if( NPC_Util_charIsInFrontOfChar( talkerindex, meindex, 3 ) ==FALSE) return;




	if(CHAR_getWorkInt(meindex,E_INT_MODE)==0){
		/*--メッセージを返す */
		NPC_Util_GetStrFromStrWithDelim(argstr,"main_msg",buf,sizeof( buf) );
 	
 	}else{
 		/*グラフィックＮＯを変えただけなので、違うメッセージを返す */
		NPC_Util_GetStrFromStrWithDelim(argstr,"change_msg",buf,sizeof( buf) );
 	}

   	tokennum = 1;
   	/* コンマで区切られたトークンが何こあるか数える */
	 for( i=0;buf[i]!='\0';i++ ){
          if( buf[i] == ',' ) tokennum++;
     }
 
    /* ランダムでどれを喋るか決めて、そのトークンを取りだす */
    getStringFromIndexWithDelim( buf,",", rand()%tokennum+1,token, sizeof(token));
	/*--しゃべる--*/
	CHAR_talkToCli( talkerindex, meindex, token, CHAR_COLORWHITE );


}

/** まわりに何かが起こったときに呼ばれる*/
void NPC_TimeManWatch( int meobjindex, int objindex, CHAR_ACTION act,
                    int x,int y,int dir, int* opt,int optlen )
{
	int meindex=0;
	int index;
	LSTIME	nowlstime;
	int born;
	int dead;
	
	if( OBJECT_getType( objindex) != OBJTYPE_CHARA) return;
	index = OBJECT_getIndex( objindex);

	/* プレイヤーにのみ反応する */
	if( CHAR_getInt( index, CHAR_WHICHTYPE) != CHAR_TYPEPLAYER) return;

    meindex = OBJECT_getIndex( meobjindex);
	born = CHAR_getWorkInt( meindex , E_INT_BORN );
	dead = CHAR_getWorkInt( meindex , E_INT_DEAD );

//	print("now_hour=%d",nowlstime.hour);
	
	/* 時間を見て状態を決める */
	if(born < dead){
		RealTimeToLSTime( NowTime.tv_sec, &nowlstime);
		if( (born < nowlstime.hour) && (dead > nowlstime.hour) ){
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC)) return;

			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
							CHAR_getWorkInt( meindex, E_INT_GRAPHIC));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,0);		
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
								CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));
		}else{
			/*-消えている--*/
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC2)) return;

			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
								CHAR_getWorkInt( meindex, E_INT_GRAPHIC2));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,1);
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
								CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));		

		}
	}else{
		RealTimeToLSTime( NowTime.tv_sec, &nowlstime);
		if( (born < nowlstime.hour && 1024 > nowlstime.hour) 
			|| ( 0 < nowlstime.hour && dead > nowlstime.hour) 
		){
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC)) return;
	
			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
							CHAR_getWorkInt( meindex, E_INT_GRAPHIC));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,0);
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
							CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));		
		}else{
			if(CHAR_getWorkInt(meindex,E_INT_NOWGRAPHIC)
				==CHAR_getWorkInt( meindex, E_INT_GRAPHIC2)) return;
			
			CHAR_setInt(meindex,CHAR_BASEIMAGENUMBER,
							CHAR_getWorkInt( meindex, E_INT_GRAPHIC2));
			CHAR_sendCToArroundCharacter( CHAR_getWorkInt( meindex, CHAR_WORKOBJINDEX ) );
			CHAR_setWorkInt(meindex,E_INT_MODE,1);
			CHAR_setWorkInt(meindex,E_INT_NOWGRAPHIC,
							CHAR_getInt(meindex,CHAR_BASEIMAGENUMBER));		
		}
	}

}

