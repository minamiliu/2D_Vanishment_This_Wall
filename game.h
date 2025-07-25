//******************************************************************************
//
// タイトル:		協力モードとバトルモードの全体管理処理
// プログラム名:	game.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "message.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STAGE	(3)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef enum
{
	STEP_PLAY = 0,
	STEP_PAUSE,
	STEP_CLEAR,
	STEP_GAMEOVER,
	STEP_GO_NEXT,
	//STEP_GAME_SET,
	//STEP_NP_WIN,
	STEP_MAX
}GAME_STEP;

typedef enum
{
	GAME_COOPERATE = 0,
	GAME_BATTLE,
	GAME_MAX
}GAME_MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UninitGame(void);
GAME_STEP GetGameStep(void);
void SetGameStep(GAME_STEP);
GAME_MODE GetGameMode(void);
void SetGameMode(GAME_MODE mode);
int GetStage(void);
void NextStage(void);
#endif