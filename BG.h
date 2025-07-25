﻿//******************************************************************************
//
// タイトル:		ゲームの背景処理
// プログラム名:	BG.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

#ifndef _BG_H_
#define _BG_H_

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
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitBG(void);
void DrawBG(void);		
void UninitBG(void);
//void UpdateBG(void);
										
#endif