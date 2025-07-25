//******************************************************************************
//
// タイトル:		協力モードとバトルモードの全体管理処理
// プログラム名:	game.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include <time.h>
#include "game.h"
#include "game_cooperate.h"
#include "game_battle.h"
#include "sound.h"
#include "input.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/


/*******************************************************************************
* グローバル変数
*******************************************************************************/
GAME_STEP g_gameStep;
GAME_MODE g_gameMode;
int stageNow;

/*******************************************************************************
関数名:	void InitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの初期化処理
*******************************************************************************/
void InitGame(void)
{
	stageNow = 0;

	//g_gameMode was initialized in title.cpp
	g_gameStep = STEP_PLAY;

	switch(g_gameMode)
	{
		case GAME_COOPERATE:
			InitGameCooperate();
			break;
		case GAME_BATTLE:
			InitGameBattle();
			break;
	}
	
}

/*******************************************************************************
関数名:	void UpdateGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの更新関数
*******************************************************************************/
void UpdateGame(void)
{
	switch(g_gameMode)
	{
		case GAME_COOPERATE:
			UpdateGameCooperate();
			break;
		case GAME_BATTLE:
			UpdateGameBattle();
			break;
	}

	//SE setting
	if(GetKeyboardTrigger(DIK_F1))
	{
		PlaySound(SOUND_LABEL_SE_SWITCH);
		SE_MODE *pSE = GetSE();
		switch(*pSE)
		{
			case SE_ON:
				*pSE = SE_OFF;
				break;
			case SE_OFF:
				*pSE = SE_ON;
				break;
		}
	}

	//BGM setting
	if(GetKeyboardTrigger(DIK_F2))
	{
		PlaySound(SOUND_LABEL_SE_SWITCH);
		BGM_MODE *pBGM = GetBGM();
		switch(*pBGM)
		{
			case SE_ON:
				*pBGM = BGM_OFF;
				break;
			case SE_OFF:
				*pBGM = BGM_ON;
				break;
		}
		//BGM ON/OFF
		SwitchBGM(SOUND_LABEL_BGM_PLAY);
	}

}

/*******************************************************************************
関数名:	void DrawGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの描画関数
*******************************************************************************/
void DrawGame(void)
{
	switch(g_gameMode)
	{
		case GAME_COOPERATE:
			DrawGameCooperate();
			break;
		case GAME_BATTLE:
			DrawGameBattle();
			break;
	}
}
/*******************************************************************************
関数名:	void UninitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの開放関数
*******************************************************************************/
void UninitGame(void)
{
	switch(g_gameMode)
	{
		case GAME_COOPERATE:
			UninitGameCooperate();
			break;
		case GAME_BATTLE:
			UninitGameBattle();
			break;
	}
}

void SetGameStep(GAME_STEP step)
{
	g_gameStep = step;
}

GAME_STEP GetGameStep(void)
{
	return g_gameStep;
}

void SetGameMode(GAME_MODE mode)
{
	g_gameMode = mode;
}

GAME_MODE GetGameMode(void)
{
	return g_gameMode;
}

int GetStage(void)
{
	return stageNow;
}

void NextStage(void)
{
	stageNow++;
	if(stageNow == MAX_STAGE) stageNow = 0;
}



