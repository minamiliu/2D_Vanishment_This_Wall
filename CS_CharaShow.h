﻿//******************************************************************************
//
// タイトル:		キャラ選択のキャラクター表示処理
// プログラム名:	CS_CharaShow.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _CS_CHARASHOW_H_
#define _CS_CHARASHOW_H_

#include "main.h"
#include "player.h"

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	PICTURE_CHARACTOER type;
	bool bUse;
}CS_INTRO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCS_CharaShow(void);
void Update_CharaShow(void);
void DrawCS_CharaShow(void);
void Uninit_CharaShow(void);

#endif