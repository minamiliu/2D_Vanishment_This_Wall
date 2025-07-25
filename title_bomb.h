﻿//******************************************************************************
//
// タイトル:		タイトルの爆弾テクスチャ処理
// プログラム名:	title_bomb.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _TITLE_BOMB_H_
#define _TITLE_BOMB_H_

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
	bool bUse;			//発射中かどうか
	int nCounterAnim;	// ポリゴンのアニメーションカウンター
	int nPatternAnim;	// ポリゴンのアニメーションパターンNo.
}TITLE_BOMB;
/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitTitleBomb(void);
void DrawTitleBomb(void);		
void UninitTitleBomb(void);
void UpdateTitleBomb(void);

#endif