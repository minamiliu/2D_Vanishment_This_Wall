//******************************************************************************
//
// タイトル:		リザルトのバトル結果UI表示処理
// プログラム名:	result_kdw_tex.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "result_kdw_tex.h"
#include "player.h"
#include "num_win.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_KDW_ICON	(MAX_PLAYER*4)

#define POLYGON_TEXTURENAME_TEX_P1		"data/TEXTURE/RESULT/1P.png"
#define POLYGON_TEXTURENAME_TEX_P2		"data/TEXTURE/RESULT/2P.png"
#define POLYGON_TEXTURENAME_TEX_P3		"data/TEXTURE/RESULT/3P.png"
#define POLYGON_TEXTURENAME_TEX_P4		"data/TEXTURE/RESULT/4P.png"
#define POLYGON_TEXTURENAME_TEX_DEATH	"data/TEXTURE/RESULT/DEATH.png"
#define POLYGON_TEXTURENAME_TEX_KILL	"data/TEXTURE/RESULT/KILL.png"
#define POLYGON_TEXTURENAME_TEX_CROWN	"data/TEXTURE/RESULT/CROWN2.png"

#define POLYGON_P1_POS_X	(200)
#define POLYGON_P1_POS_Y	(200)

#define POLYGON_PN_SIZE_X	(150)
#define POLYGON_PN_SIZE_Y	(100)

#define POLYGON_KILL_SIZE_X	(200)
#define POLYGON_KILL_SIZE_Y	(100)

#define POLYGON_DEATH_SIZE_X	(200)
#define POLYGON_DEATH_SIZE_Y	(100)

#define POLYGON_WIN_SIZE_X	(100)
#define POLYGON_WIN_SIZE_Y	(100)


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexResultKDWTex(LPDIRECT3DDEVICE9 pDevice);
void SetVertexResultKDWTex(int nIconIdx);
/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultKDWTex = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultKDWTex[MAX_KDW_ICON] = {NULL};

RESULT_KDW_TEX	g_KDWIcon[MAX_KDW_ICON];
float g_angle3;

/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitResultKDWTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++)
	{
		g_KDWIcon[nCntIcon].bUse = true;
	}

	//1P
	g_KDWIcon[0].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y, 0.0f);
	g_KDWIcon[0].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[0].col = D3DCOLOR_RGBA(255,0,0,255); //red
	{
		//kill
		g_KDWIcon[1].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y, 0.0f);
		g_KDWIcon[1].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[1].col = D3DCOLOR_RGBA(255,0,0,255); //red

		//death
		g_KDWIcon[2].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y, 0.0f);
		g_KDWIcon[2].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[2].col = D3DCOLOR_RGBA(255,0,0,255); //red

		//win
		g_KDWIcon[3].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[3].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[3].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[3].bUse = (GetWinner() == 0) ? true : false;
	}

	//2P
	g_KDWIcon[4].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y + 100, 0.0f);
	g_KDWIcon[4].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[4].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
	{
		//kill
		g_KDWIcon[5].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y + 100, 0.0f);
		g_KDWIcon[5].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[5].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

		//death
		g_KDWIcon[6].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y + 100, 0.0f);
		g_KDWIcon[6].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[6].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

		//win
		g_KDWIcon[7].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + 100 + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[7].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[7].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[7].bUse = (GetWinner() == 1) ? true : false;
	}

	//3P
	g_KDWIcon[8].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y + 200, 0.0f);
	g_KDWIcon[8].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[8].col = D3DCOLOR_RGBA(0,255,0,255); //green
	{
		//kill
		g_KDWIcon[9].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y + 200, 0.0f);
		g_KDWIcon[9].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[9].col = D3DCOLOR_RGBA(0,255,0,255); //green

		//death
		g_KDWIcon[10].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y + 200, 0.0f);
		g_KDWIcon[10].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[10].col = D3DCOLOR_RGBA(0,255,0,255); //green

		//win
		g_KDWIcon[11].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + 200 + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[11].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[11].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[11].bUse = (GetWinner() == 2) ? true : false;
	}

	//4P
	g_KDWIcon[12].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y + 300, 0.0f);
	g_KDWIcon[12].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[12].col = D3DCOLOR_RGBA(0,0,255,255); //blue
	{
		//kill
		g_KDWIcon[13].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y + 300, 0.0f);
		g_KDWIcon[13].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[13].col = D3DCOLOR_RGBA(0,0,255,255); //blue

		//death
		g_KDWIcon[14].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y + 300, 0.0f);
		g_KDWIcon[14].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[14].col = D3DCOLOR_RGBA(0,0,255,255); //blue

		//win
		g_KDWIcon[15].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + 300 + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[15].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[15].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[15].bUse = (GetWinner() == 3) ? true : false;
	}
	


	//頂点情報の作成
	if(FAILED(MakeVertexResultKDWTex(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P1, &g_pTextureResultKDWTex[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[2]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[3]);
															
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P2, &g_pTextureResultKDWTex[4]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[5]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[6]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[7]);
															
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P3, &g_pTextureResultKDWTex[8]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[9]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[10]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[11]);
															
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P4, &g_pTextureResultKDWTex[12]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[13]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[14]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[15]);



	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawResultKDWTex(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferResultKDWTex, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++)
	{
		if(g_KDWIcon[nCntIcon].bUse)
		{

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResultKDWTex[nCntIcon]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntIcon*4,			//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);				
		}
	
	}


}
/*******************************************************************************
関数名:	void UninitBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの開放関数
*******************************************************************************/
void UninitResultKDWTex(void)
{
	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++)
	{
		if(g_pTextureResultKDWTex[nCntIcon] != NULL)
		{
			g_pTextureResultKDWTex[nCntIcon]->Release();
			g_pTextureResultKDWTex[nCntIcon] = NULL;
		}
	}

	if(g_pVtxBufferResultKDWTex != NULL)
	{
		g_pVtxBufferResultKDWTex->Release();
		g_pVtxBufferResultKDWTex = NULL;
	}	


}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexResultKDWTex(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_KDW_ICON,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferResultKDWTex,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferResultKDWTex->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++,pVtx+=4)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3( g_KDWIcon[nCntIcon].pos.x, g_KDWIcon[nCntIcon].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_KDWIcon[nCntIcon].pos.x + g_KDWIcon[nCntIcon].size.x, g_KDWIcon[nCntIcon].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_KDWIcon[nCntIcon].pos.x, g_KDWIcon[nCntIcon].pos.y + g_KDWIcon[nCntIcon].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_KDWIcon[nCntIcon].pos.x + g_KDWIcon[nCntIcon].size.x, g_KDWIcon[nCntIcon].pos.y + g_KDWIcon[nCntIcon].size.y, 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_KDWIcon[nCntIcon].col;
		pVtx[1].col = g_KDWIcon[nCntIcon].col;
		pVtx[2].col = g_KDWIcon[nCntIcon].col;
		pVtx[3].col = g_KDWIcon[nCntIcon].col;

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferResultKDWTex->Unlock();

	return S_OK;
}

void UpdateResultKDWTex(void)
{
	for(int nCntCrown = 3; nCntCrown < MAX_KDW_ICON; nCntCrown+=4) //3 7 11 15の四つ
	{
		if(g_KDWIcon[nCntCrown].bUse)
		{
			//change size
			g_KDWIcon[nCntCrown].size.x += sin(g_angle3)*0.5f;
			g_KDWIcon[nCntCrown].size.y += sin(g_angle3)*0.5f;
			g_angle3 += 0.1f;
			if(g_angle3 >= 6.28) g_angle3 = 0;

			SetVertexResultKDWTex(nCntCrown);	
		}	
	}
}

void SetVertexResultKDWTex(int nIconIdx)
{

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferResultKDWTex->Lock( 0, 0, (void**)&pVtx, 0);

	pVtx += nIconIdx * NUM_VERTEX; 

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x - (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y - (g_KDWIcon[nIconIdx].size.y/2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x + (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y - (g_KDWIcon[nIconIdx].size.y/2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x - (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y + (g_KDWIcon[nIconIdx].size.y/2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x + (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y + (g_KDWIcon[nIconIdx].size.y/2), 0.0f);

	// 頂点データをアンロックする
	g_pVtxBufferResultKDWTex->Unlock();
}


