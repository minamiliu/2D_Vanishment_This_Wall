//******************************************************************************
//
// タイトル:		バトルモードのゲーム流れ処理
// プログラム名:	game_battle.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include <time.h>
#include "game_battle.h"
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
//#include "time.h"
#include "status.h"
#include "fade.h"
#include "player_life.h"
#include "message.h"

#include "bar_battle.h"
#include "num_kill.h"
#include "num_death.h"
#include "num_win.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void ReSetGameBattle(void);

/*******************************************************************************
* グローバル変数
*******************************************************************************/

/*******************************************************************************
関数名:	void InitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの初期化処理
*******************************************************************************/
void InitGameBattle(void)
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

	//Statusの初期化
	InitStatus();


	//battle mode に表示しないもの
	InitScore();
	//InitTime();


	//battle mode に表示するもの
	InitBarBattle();
	InitNumKill();
	InitNumDeath();
	InitNumWin();

	UninitEnemy();
}

/*******************************************************************************
関数名:	void UpdateGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの更新関数
*******************************************************************************/
void UpdateGameBattle(void)
{
	GAME_STEP stepNow = GetGameStep();

	switch(stepNow)
	{
		case STEP_PLAY:
			UpdatePlayer();
			UpdateStatus();
			UpdateBullet();
			//UpdateEnemy();
			UpdateExplosion();
			UpdateScore();
			UpdateItem();
			//UpdateTime();
			UpdatePlayerLife();
			UpdateNumKill();
			UpdateNumDeath();
			UpdateNumWin();
			UpdateBarBattle();

			if(GetKeyboardTrigger(DIK_P))
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
			if(GetKeyboardTrigger(DIK_P))
			{
				SetGameStep(STEP_PLAY);
				ClearMessage(MSG_PAUSE);
			}
			break;
		case STEP_GAMEOVER:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage(MSG_GAMEOVER);
				FadeOutToNext(MODE_RESULT);
			}
			break;
		case STEP_CLEAR:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage();

				int winner = GetWinner();
				if(winner == -1)
				{
					FadeOutToNext(STEP_GO_NEXT);
				}
				else
				{
					FadeOutToNext(MODE_RESULT);
				}
			}
			break;
		case STEP_GO_NEXT:
			NextStage();
			ReSetGameBattle();
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
void DrawGameBattle(void)
{
	DrawBG();
	DrawMap();
	DrawItem();
	//DrawEnemy();
	DrawBullet();
	DrawPlayer();
	DrawStatus();
	DrawExplosion();
	DrawBarBattle();
	DrawNumKill();
	DrawNumDeath();
	DrawNumWin();

	//DrawPlayerLife();
	//DrawScore();
	//DrawTime();

}
/*******************************************************************************
関数名:	void UninitGame(void)
引数:	なし
戻り値:	なし
説明:	ゲームの開放関数
*******************************************************************************/
void UninitGameBattle(void)
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
	//UninitEnemy();

	//SCOREの開放
	UninitScore();

	UninitMap();
	UninitItem();
	//UninitTime();
	UninitStatus();
	UninitPlayerLife();

	UninitBarBattle();
	UninitNumKill();
	UninitNumDeath();
	UninitNumWin();
}



void ReSetGameBattle(void)
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
	//InitEnemy();

	//TIMEの初期化
	//InitTime();

	InitStatus();

	InitPlayer();
}
