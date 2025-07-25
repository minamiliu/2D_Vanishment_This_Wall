//******************************************************************************
//
// タイトル:		アイテム処理
// プログラム名:	item.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _ITEM_H_
#define _ITEM_H_

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
#define ITEM_MAP_SIZE_X (25)
#define ITEM_MAP_SIZE_Y (13)
#define MAX_ITEM		(ITEM_MAP_SIZE_Y*ITEM_MAP_SIZE_X)		// オブジェクトの数
#define FRAME_BEFORE_VISIBLE	(25)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef enum
{
	ITEM_NONE = 0,
	ITEM_BOMB, 
	ITEM_FIRE,
	ITEM_SKATE,
	ITEM_DIAMOND,
	ITEM_GUARD,
	ITEM_BONE,
	ITEM_QUESTION,  //例外のITEM_QUESTION 
	ITEM_NEXT,
	ITEM_GRAVE,
	ITEM_SETTED_BOMB,
	ITEM_TYPE_MAX
}ITEM_TYPE;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	int index;
	bool visible;
	ITEM_TYPE type;

	int nCounterVisible;
}ITEM;





/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitItem(void);
void DrawItem(void);		
void UninitItem(void);
void UpdateItem(void);
ITEM *GetItem(void);
void SetItem(ITEM_TYPE type, D3DXVECTOR3 dotPos);

#endif