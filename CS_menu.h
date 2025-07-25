﻿//******************************************************************************
//
// タイトル:		キャラ選択の流れ管理処理
// プログラム名:	CS_menu.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _CS_MENU_H_
#define _CS_MENU_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCSMenu(void);
void UpdateCSMenu(void);
void DrawCSMenu(void);
void UninitCSMenu(void);
void AllPlayerLock(void);

#endif