#ifndef __BATTLE_COMMAND_H__
#define __BATTLE_COMMAND_H__

void BattleCommandDispach(
	int fd,
	char *command
);

void BattleEncountOut(
	int charaindex
);

BOOL BATTLE_CommandSend( int charaindex, char *pszCommand );

BOOL BATTLE_MakeCharaString(
	int battleindex,
	char *pszCommand,	// キャラクター情報出力先
	int	size 			// サイズ
);

void BATTLE_CharSendAll( int battleindex );

BOOL	BATTLE_PetDefaultCommand( int petindex );

void BATTLE_ActSettingSend( int battleindex );

#endif
