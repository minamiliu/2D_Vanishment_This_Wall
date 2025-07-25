﻿//******************************************************************************
//
// タイトル:		フェード処理
// プログラム名:	fade.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _FADE_H_
#define _FADE_H_

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "game.h"

/*******************************************************************************
* ライブラリのリンク
*******************************************************************************/


/*******************************************************************************
* マクロ定義
*******************************************************************************/


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_CHANGE,
	FADE_OUT,
	FADE_MAX
}FADE;

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitFade(void);
void DrawFade(void);		
void UninitFade(void);
void UpdateFade(void);
void FadeOutToNext(MODE mode);
void FadeOutToNext(GAME_STEP mode);
										
#endif