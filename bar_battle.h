//******************************************************************************
//
// タイトル:		バトルモードのUI表示処理
// プログラム名:	bar_battle.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

#ifndef _BAR_BATTLE_H_
#define _BAR_BATTLE_H_

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
}BAR_ICON_BATTLE;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitBarBattle(void);
void DrawBarBattle(void);		
void UninitBarBattle(void);
void UpdateBarBattle(void);
										
#endif