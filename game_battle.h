﻿//******************************************************************************
//
// タイトル:		バトルモードのゲーム流れ処理
// プログラム名:	game_battle.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

#ifndef _GAME_BATTLE_H_
#define _GAME_BATTLE_H_

#include "main.h"
#include "message.h"

/*******************************************************************************
* 構造体定義
*******************************************************************************/


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGameBattle(void);
void UpdateGameBattle(void);
void DrawGameBattle(void);
void UninitGameBattle(void);


#endif