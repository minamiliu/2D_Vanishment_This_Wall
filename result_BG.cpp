//******************************************************************************
//
// タイトル:		リザルトの背景処理
// プログラム名:	result_BG.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "result_BG.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//ポリゴンの左上X
#define POLYGON_POS_Y	(0)//ポリゴンの左上Y
#define POLYGON_SIZE_X	(1250)//ポリゴンのSIZE X
#define POLYGON_SIZE_Y	(750)//ポリゴンのSIZE Y
#define POLYGON_TEXTURENAME "data/TEXTURE/BG_Ranking.jpg"


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexResultBG(LPDIRECT3DDEVICE9 pDevice);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultBG = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultBG = NULL;

D3DXVECTOR3 g_scroll;
/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitResultBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報の作成
	if(FAILED(MakeVertexResultBG(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME, &g_pTextureResultBG);

	g_scroll = D3DXVECTOR3(0,0,0);

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawResultBG(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferResultBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultBG);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,						//ロードする最初の頂点インデックス
		NUM_POLYGON				//ポリゴンの数
	);

}
/*******************************************************************************
関数名:	void UninitBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの開放関数
*******************************************************************************/
void UninitResultBG(void)
{
	if(g_pTextureResultBG != NULL)
	{
		g_pTextureResultBG->Release();
		g_pTextureResultBG = NULL;
	}
	if(g_pVtxBufferResultBG != NULL)
	{
		g_pVtxBufferResultBG->Release();
		g_pVtxBufferResultBG = NULL;
	}
}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexResultBG(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferResultBG,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferResultBG->Lock( 0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);


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

	////テクスチャ座標指定
	//pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
	//pVtx[1].tex = D3DXVECTOR2(5.0F, 0.0F);
	//pVtx[2].tex = D3DXVECTOR2(0.0F, 3.0F);
	//pVtx[3].tex = D3DXVECTOR2(5.0F, 3.0F);

	//テクスチャ座標指定
	pVtx[0].tex = D3DXVECTOR2(g_scroll.x, g_scroll.y);
	pVtx[1].tex = D3DXVECTOR2(g_scroll.x + 5.0F, g_scroll.y + 0.0F);
	pVtx[2].tex = D3DXVECTOR2(g_scroll.x + 0.0F, g_scroll.y + 3.0F);
	pVtx[3].tex = D3DXVECTOR2(g_scroll.x + 5.0F, g_scroll.y + 3.0F);

	g_pVtxBufferResultBG->Unlock();

	return S_OK;
}

void UpdateResultBG(void)
{
	g_scroll.y += 0.001;
	SetVertexBG();
}

void SetVertexBG(void)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferResultBG->Lock( 0, 0, (void**)&pVtx, 0);

	//テクスチャ座標指定
	pVtx[0].tex = D3DXVECTOR2(g_scroll.x, g_scroll.y);
	pVtx[1].tex = D3DXVECTOR2(g_scroll.x + 5.0F, g_scroll.y + 0.0F);
	pVtx[2].tex = D3DXVECTOR2(g_scroll.x + 0.0F, g_scroll.y + 3.0F);
	pVtx[3].tex = D3DXVECTOR2(g_scroll.x + 5.0F, g_scroll.y + 3.0F);

	g_pVtxBufferResultBG->Unlock();
}