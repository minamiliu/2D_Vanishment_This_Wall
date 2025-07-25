//******************************************************************************
//
// タイトル:		ステージマップの読み込み処理
// プログラム名:	map.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "map.h"
#include "block.h"
#include <stdio.h>
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define NUM_VERTEX (4)
//#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//ポリゴンの左上X
#define POLYGON_POS_Y	(100)//ポリゴンの左上Y
#define POLYGON_SIZE_X	(50)//ポリゴンのSIZE X
#define POLYGON_SIZE_Y	(50)//ポリゴンのSIZE Y


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
//*******************************************************************************/
void LoadMapFile(void);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMap = NULL;
//LPDIRECT3DTEXTURE9 g_pTextureMap[BLOCK_TYPE_MAX] = {NULL};

MAP g_map[MAX_STAGE];

/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitMap(void)
{
	//load map file
	LoadMapFile();

	InitBlock();


	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawMap(void)
{
	DrawBlock();
}
/*******************************************************************************
関数名:	void UninitBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの開放関数
*******************************************************************************/
void UninitMap(void)
{
	UninitBlock();
}


MAP *GetMap(void)
{
	int stage = GetStage();
	return &g_map[stage];
}

D3DXVECTOR3 Pos2DotPos(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 dotPos;
	dotPos.x = (int)((pos.x / POLYGON_SIZE_X));
	dotPos.y = (int)((pos.y -  POLYGON_POS_Y)/POLYGON_SIZE_Y);
	return dotPos;
}

D3DXVECTOR3 DotPos2Pos(D3DXVECTOR3 dotPos)
{
	D3DXVECTOR3 pos = D3DXVECTOR3( dotPos.x * POLYGON_SIZE_X + POLYGON_SIZE_X/2, dotPos.y * POLYGON_SIZE_Y + POLYGON_SIZE_Y/2 + POLYGON_POS_Y, 0.0f);
	return pos;
}

void ChangeMap(D3DXVECTOR3 dotPos, BLOCK_TYPE b_type)
{
	int stage = GetStage();
	BLOCK *pBlock = GetBlock();  
	g_map[stage].data[(int)dotPos.y][(int)dotPos.x] = b_type;
	pBlock[(int)(dotPos.y * MAP_SIZE_X + dotPos.x)].type = b_type;
}

void LoadMapFile(void)
{
	FILE *fp;
	fp = fopen( "data/FILE/stage.txt","r" );

	for(int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		for(int nCntR = 0; nCntR < MAP_SIZE_Y; nCntR++)
		{
			for(int nCntC = 0; nCntC < MAP_SIZE_X; nCntC++)
			{
				//MAP構造体の初期化
				fscanf(fp,"%d ",&g_map[nCntStage].data[nCntR][nCntC]);		
			}
		}	
	}

	fclose(fp);
}






