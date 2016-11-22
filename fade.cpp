//******************************************************************************
//
// タイトル:		フェード処理
// プログラム名:	fade.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//ポリゴンの左上X
#define POLYGON_POS_Y	(0)//ポリゴンの左上Y
#define POLYGON_SIZE_X	(1250)//ポリゴンのSIZE X
#define POLYGON_SIZE_Y	(750)//ポリゴンのSIZE Y
//#define POLYGON_TEXTURENAME "data/TEXTURE/BG/xxx.png"


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
void SetColorFade(int R, int G, int B, int A);
void SetFade(FADE fade);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFade = NULL;
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;

FADE g_fade;
int g_alpha;
MODE g_tempMode;
GAME_STEP g_tempGameMode;
bool g_changeModeFlag;
bool g_changeGameFlag;

/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報の作成
	if(FAILED(MakeVertexFade(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	//D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME, &g_pTextureFade);

	g_fade = FADE_NONE;
	g_alpha = 0; //透明
	g_changeModeFlag = false;
	g_changeGameFlag = true;

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawFade(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

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
void UninitFade(void)
{
	if(g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
	if(g_pVtxBufferFade != NULL)
	{
		g_pVtxBufferFade->Release();
		g_pVtxBufferFade = NULL;
	}
}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferFade,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferFade->Lock( 0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(0,0,0,0);
	pVtx[1].col = D3DCOLOR_RGBA(0,0,0,0);
	pVtx[2].col = D3DCOLOR_RGBA(0,0,0,0);
	pVtx[3].col = D3DCOLOR_RGBA(0,0,0,0);

	//テクスチャ座標指定
	pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
	pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
	pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
	pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);

	g_pVtxBufferFade->Unlock();

	return S_OK;
}

void UpdateFade(void)
{
	switch(g_fade)
	{
		case FADE_OUT:
			g_alpha += 5;
			if(g_alpha >= 255)
			{
				g_alpha = 255;
				SetFade(FADE_CHANGE);
			}
			SetColorFade(0, 0, 0, g_alpha);
			break;
		case FADE_CHANGE:
			if(g_changeModeFlag)
			{
				SetMode(g_tempMode); 
				g_changeModeFlag = false;
			}
			if(g_changeGameFlag)
			{
				SetGameStep(g_tempGameMode);
				g_changeGameFlag = false;
			}
			SetFade(FADE_IN);
			break;
		case FADE_IN:
			g_alpha -= 5;
			if(g_alpha <= 0)
			{
				g_alpha = 0;
				SetFade(FADE_NONE);
			}
			SetColorFade(0, 0, 0, g_alpha);
			break;
		case FADE_NONE:

			break;
	}
}

void SetColorFade(int R, int G, int B, int A)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferFade->Lock(0, 0, (void**)&pVtx, 0);

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// 頂点データをアンロックする
	g_pVtxBufferFade->Unlock();
	
}

void FadeOutToNext(MODE mode)
{
	g_fade = FADE_OUT;
	g_tempMode = mode;
	g_changeModeFlag = true;
}

void FadeOutToNext(GAME_STEP mode)
{
	g_fade = FADE_OUT;
	g_tempGameMode = mode;
	g_changeGameFlag = true;
}

void SetFade(FADE fade)
{
	g_fade = fade;
}
