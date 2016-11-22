//******************************************************************************
//
// タイトル:		キャラ選択の選ぶ処理
// プログラム名:	CS_select.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

#ifndef _CS_SELECT_H_
#define _CS_SELECT_H_

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "player.h"

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
	PICTURE_1P	= 0,
	PICTURE_2P		,
	PICTURE_3P		,
	PICTURE_4P		,
	PICTURE_PRESSFIRE,	
	PICTURE_READY	,
	PICTURE_ALLREADY,
	PICTURE_MAX
}PICTURE_CS;

typedef enum
{
	SELECT	= 0,
	PRESSFIRE,	
	READY,
	JOINCOLOR,
	MAX_ICON_TYPE
}ICON_TYPE;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	PICTURE_CS type;
	bool bUse;
}CS_ICON;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitCS_Select(void);
void DrawCS_Select(void);		
void UninitCS_Select(void);
void UpdateCS_Select(void);

PICTURE_CHARACTOER *GetCharacterList(void);
bool *GetPlayerList(void);
bool GetPlayerJoinData(int nPlayerIdx);
#endif