//******************************************************************************
//
// タイトル:		ブロック処理
// プログラム名:	block.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "block.h"
#include "map.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//ポリゴンの左上X
#define POLYGON_POS_Y	(100)//ポリゴンの左上Y
#define POLYGON_SIZE_X	(50)//ポリゴンのSIZE X
#define POLYGON_SIZE_Y	(50)//ポリゴンのSIZE Y
#define POLYGON_TEXTURENAME_GRASS	"data/TEXTURE/BG/grass.png"
#define POLYGON_TEXTURENAME_PILLAR	"data/TEXTURE/BG/pillar.png"
#define POLYGON_TEXTURENAME_WALL	"data/TEXTURE/BG/wall.png"

#define MAX_BLOCK		(MAP_SIZE_Y*MAP_SIZE_X)		// オブジェクトの数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBlock = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCK_TYPE_MAX] = {NULL};

BLOCK	g_block[MAX_BLOCK];



/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitBlock()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	pMap->wallListIdx = 0;
	
	int nCntBlock = 0; 
	for(int nCntR = 0; nCntR < MAP_SIZE_Y; nCntR++)
	{
		for(int nCntC = 0; nCntC < MAP_SIZE_X; nCntC++)
		{

			//BLOCKの初期化 座標は左上
			g_block[nCntBlock].pos = D3DXVECTOR3( nCntC*POLYGON_SIZE_X + POLYGON_POS_X, nCntR*POLYGON_SIZE_Y + POLYGON_POS_Y, 0.0f);
			g_block[nCntBlock].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);

			switch(pMap->data[nCntR][nCntC])
			{
				case BLOCK_GRASS:
					g_block[nCntBlock].type = BLOCK_GRASS;
					break;
				case BLOCK_PILLAR:
					g_block[nCntBlock].type = BLOCK_PILLAR;	
					break;
				case BLOCK_WALL:
					g_block[nCntBlock].type = BLOCK_WALL;
					pMap->wallPosList[pMap->wallListIdx].x = g_block[nCntBlock].pos.x + POLYGON_SIZE_X/2;
					pMap->wallPosList[pMap->wallListIdx].y = g_block[nCntBlock].pos.y + POLYGON_SIZE_Y/2;
					pMap->bUseWallList[pMap->wallListIdx] = false;
					pMap->wallListIdx++;
					break;
			} 				
			nCntBlock++;			
		}
	}

	//頂点情報の作成
	if(FAILED(MakeVertexBlock(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_GRASS, &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_PILLAR, &g_pTextureBlock[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_WALL, &g_pTextureBlock[2]);


	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawBlock(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBlock[g_block[nCntBlock].type]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntBlock*4,			//ロードする最初の頂点インデックス
			NUM_POLYGON				//ポリゴンの数
		);			
	}


}
/*******************************************************************************
関数名:	void UninitBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの開放関数
*******************************************************************************/
void UninitBlock(void)
{
	for(int nCnt = 0; nCnt < BLOCK_TYPE_MAX; nCnt++)
	{
		if(g_pTextureBlock[nCnt] != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}

	if(g_pVtxBufferBlock != NULL)
	{
		g_pVtxBufferBlock->Release();
		g_pVtxBufferBlock = NULL;
	}	


}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BLOCK,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBlock,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferBlock->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pVtx+=4)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x, g_block[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x + POLYGON_SIZE_X, g_block[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x, g_block[nCntBlock].pos.y + POLYGON_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x + POLYGON_SIZE_X, g_block[nCntBlock].pos.y + POLYGON_SIZE_Y, 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferBlock->Unlock();

	return S_OK;
}

BLOCK *GetBlock(void)
{
	return g_block;
}

