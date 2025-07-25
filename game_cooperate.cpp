//******************************************************************************
//
// タイトル:		協力モードの流れ管理処理
// プログラム名:	game_cooperate.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include <time.h>
#include "game_cooperate.h"
#include "main.h"
#include "player.h"
#include "input.h"
#include "BG.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "map.h"
#include "item.h"
#include "sound.h"
#include "time.h"
#include "status.h"
#include "fade.h"
#include "player_life.h"
#include "bar.h"
#include "message.h"
#include "boss.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void ReSetGame(void);

/*******************************************************************************
* グローバル変数
*******************************************************************************/

/*******************************************************************************
関数名:	void InitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの初期化処理
*******************************************************************************/
void InitGameCooperate(void)
{

	//random seedの初期化
	srand((unsigned int)time(NULL));

	//BGの初期化
	InitBG();

	//ステージの初期化
	InitMap();

	//アイテムの初期化
	InitItem();

	//ポリゴンの初期化
	InitPlayerLife();
	InitPlayer();

	//Bulletの初期化
	InitBullet();

	//Explosionの初期化
	InitExplosion();

	//Enemyの初期化
	InitEnemy();

	//SCOREの初期化
	InitScore();

	//TIMEの初期化
	InitTime();

	InitStatus();

	InitBar();

	InitBoss();


}

/*******************************************************************************
関数名:	void UpdateGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの更新関数
*******************************************************************************/
void UpdateGameCooperate(void)
{
	GAME_STEP stepNow = GetGameStep();

	switch(stepNow)
	{
		case STEP_PLAY:
			UpdatePlayer();
			UpdateStatus();
			UpdateBullet();
			UpdateEnemy();
			UpdateExplosion();
			//UpdateScore();
			UpdateItem();
			UpdateTime();
			UpdatePlayerLife();
			//UpdateBar();
			UpdateBoss();

			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PAUSE);
				SetMessage(MSG_PAUSE);
			}

			//ステージスギップ
			if(GetKeyboardTrigger(DIK_F4) )
			{
				SetGameStep(STEP_CLEAR);
				SetMessage(MSG_STAGECLEAR);

				//SE
				SetSE(SOUND_LABEL_SE_GAMESTART);
			}
			break;
		case STEP_PAUSE:
			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PLAY);
				ClearMessage(MSG_PAUSE);
			}
			break;
		case STEP_GAMEOVER:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage(MSG_GAMEOVER);
				ClearMessage(MSG_TIME_UP);
				FadeOutToNext(MODE_RESULT);
			}
			break;
		case STEP_CLEAR:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage(MSG_STAGECLEAR);
				if(GetStage() == 2)
				{
					FadeOutToNext(MODE_RESULT);
				}
				else
				{
					FadeOutToNext(STEP_GO_NEXT);
				}
				
			}
			break;
		case STEP_GO_NEXT:
			NextStage();
			ReSetGame();
			SetGameStep(STEP_PLAY);
			break;
	}	


}

/*******************************************************************************
関数名:	void DrawGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの描画関数
*******************************************************************************/
void DrawGameCooperate(void)
{
	DrawBG();
	DrawMap();
	DrawItem();
	DrawEnemy();
	DrawBullet();
	DrawPlayer();
	DrawStatus();
	DrawExplosion();
	DrawPlayerLife();
	DrawBar();
	DrawScore();
	DrawTime();
	DrawBoss();
}
/*******************************************************************************
関数名:	void UninitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの開放関数
*******************************************************************************/
void UninitGameCooperate(void)
{
	//BGの開放
	UninitBG();

	//ポリゴンの開放
	UninitPlayer();

	//bulletの開放
	UninitBullet();

	//Explosionの開放
	UninitExplosion();

	//Enemyの開放
	UninitEnemy();

	//SCOREの開放
	UninitScore();

	UninitMap();
	UninitItem();
	UninitTime();
	UninitStatus();
	UninitPlayerLife();
	UninitBar();
	UninitBoss();
}



void ReSetGame(void)
{

	//ステージの初期化
	InitMap();

	//アイテムの初期化
	InitItem();

	//Bulletの初期化
	InitBullet();

	//Explosionの初期化
	InitExplosion();

	//Enemyの初期化
	InitEnemy();

	//TIMEの初期化
	InitTime();

	InitBoss();

	InitStatus();

	ResetAllPlayer();
}


