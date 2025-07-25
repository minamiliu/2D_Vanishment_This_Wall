//******************************************************************************
//
// タイトル:		ステータス処理
// プログラム名:	status.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _STATUS_H_
#define _STATUS_H_

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
#define FRAME_STATUS_GUARD		(60) 
#define FRAME_STATUS_BOMBING	(60*10)
#define FRAME_STATUS_LEFTRIGHT	(60*10)
#define FRAME_STATUS_INVISIBLE	(60*10)
#define FRAME_STATUS_HURT		(60) 
#define FRAME_STATUS_GET_ITEM	(60) 

/*******************************************************************************
* 構造体定義
*******************************************************************************/


typedef enum
{
	STATUS_NONE = 0,
	STATUS_BOMBING,
	STATUS_LEFTRIGHT,
	STATUS_INVISIBLE,
	STATUS_GUARD,
	STATUS_ADDBOMB,
	STATUS_ADDSCORE,
	STATUS_ADDSKATE,
	STATUS_ADDFIRE,
	STATUS_HURT,
	STATUS_UNDEAD,
	STATUS_MAX
}STATUS;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	bool bUse;
	STATUS type;
	int who_get;
}STATUS_OBJECT;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitStatus(void);
void DrawStatus(void);		
void UninitStatus(void);
void UpdateStatus(void);
void SetStatus(int nPlayerIdx, STATUS type);
void ClearStatus(int nIdx);

#endif