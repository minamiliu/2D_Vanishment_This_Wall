﻿//******************************************************************************
//
// タイトル:		メッセージ表示処理
// プログラム名:	message.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _MESSAGE_H_
#define _MESSAGE_H_

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
typedef enum
{
	MSG_GAMEOVER = 0,
	MSG_STAGECLEAR,
	MSG_RANKING,
	MSG_PAUSE,
	MSG_TIME_UP,
	MSG_GAME_SET,
	MSG_DRAW,
	MSG_WIN,
	MSG_1P,
	MSG_2P,
	MSG_3P,
	MSG_4P,
	MSG_RESULT,
	MSG_MAX
}MSG_WORD;
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	D3DCOLOR col;
	bool bUse;
}MESSAGE;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitMessage(void);
void DrawMessage(void);		
void UninitMessage(void);
void UpdateMessage(void);
void SetMessage(int nMsgIdx);
void ClearMessage(int nMsgIdx);
void ClearMessage(void);
#endif