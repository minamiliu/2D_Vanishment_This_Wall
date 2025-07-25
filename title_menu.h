﻿//******************************************************************************
//
// タイトル:		タイトルのメニュー処理
// プログラム名:	title_menu.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _TITLE_MENU_H_
#define _TITLE_MENU_H_

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
}TITLE_MENU;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitTitleMenu(void);
void DrawTitleMenu(void);		
void UninitTitleMenu(void);
void UpdateTitleMenu(void);
int GetTitleMenu(void);
										
#endif