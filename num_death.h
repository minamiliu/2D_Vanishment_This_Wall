//******************************************************************************
//
// タイトル:		バトルモードの死亡回数表示処理
// プログラム名:	num_death.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

#ifndef _NUM_DEATH_H_
#define _NUM_DEATH_H_

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* ライブラリのリンク
*******************************************************************************/


/*******************************************************************************
* マクロ定義
*******************************************************************************/


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	D3DCOLOR col;
	int score;

}NUM_DEATH;

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitNumDeath();
void DrawNumDeath(void);		
void UninitNumDeath(void);
void UpdateNumDeath(void);
void ChangeNumDeath(int nPlayerIdx, int value);	
NUM_DEATH *GetNumDeath(void);
#endif