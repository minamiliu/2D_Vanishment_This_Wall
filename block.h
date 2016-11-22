//******************************************************************************
//
// タイトル:		ブロック処理
// プログラム名:	block.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _BLOCK_H_
#define _BLOCK_H_

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
	BLOCK_GRASS = 0,
	BLOCK_PILLAR,
	BLOCK_WALL,
	BLOCK_TYPE_MAX
}BLOCK_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	BLOCK_TYPE type; 
}BLOCK;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitBlock();
void DrawBlock(void);		
void UninitBlock(void);
//void UpdateBlock(void);
BLOCK *GetBlock(void);

#endif