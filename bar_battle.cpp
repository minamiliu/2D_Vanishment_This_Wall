//******************************************************************************
//
// タイトル:		バトルモードのUI表示処理
// プログラム名:	bar_battle.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "bar_battle.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_BAR_ICON	(15)

#define POLYGON_TEXTURENAME_BAR_P1		"data/TEXTURE/BAR/1P.png"
#define POLYGON_TEXTURENAME_BAR_P2		"data/TEXTURE/BAR/2P.png"
#define POLYGON_TEXTURENAME_BAR_P3		"data/TEXTURE/BAR/3P.png"
#define POLYGON_TEXTURENAME_BAR_P4		"data/TEXTURE/BAR/4P.png"
#define POLYGON_TEXTURENAME_BAR_WIN		"data/TEXTURE/BAR/WIN.png"
#define POLYGON_TEXTURENAME_BAR_DEATH	"data/TEXTURE/BAR/DEATH.png"
#define POLYGON_TEXTURENAME_BAR_KILL	"data/TEXTURE/BAR/KILL.png"

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexBarBattle(LPDIRECT3DDEVICE9 pDevice);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBarBattle = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBarBattle[MAX_BAR_ICON] = {NULL};

BAR_ICON_BATTLE	g_barIcon[MAX_BAR_ICON];


/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitBarBattle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//kill
	g_barIcon[6].pos = D3DXVECTOR3( 10, 20, 0.0f);
	g_barIcon[6].size = D3DXVECTOR3( 150, 70, 0.0f);
	g_barIcon[6].col = D3DCOLOR_RGBA(30,225,180,255);
	{
		//p1
		g_barIcon[0].pos = D3DXVECTOR3( 160, 25, 0.0f);
		g_barIcon[0].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[0].col = D3DCOLOR_RGBA(255,0,0,255); //red

		//p2
		g_barIcon[1].pos = D3DXVECTOR3( 270, 25, 0.0f);
		g_barIcon[1].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[1].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

		//p3
		g_barIcon[2].pos = D3DXVECTOR3( 160, 65, 0.0f);
		g_barIcon[2].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[2].col = D3DCOLOR_RGBA(0,255,0,255); //green

		//p4
		g_barIcon[3].pos = D3DXVECTOR3( 270, 65, 0.0f);
		g_barIcon[3].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[3].col = D3DCOLOR_RGBA(0,0,255,255); //blue
	}

	//death
	g_barIcon[5].pos = D3DXVECTOR3( 380, 20, 0.0f);
	g_barIcon[5].size = D3DXVECTOR3( 150, 70, 0.0f);
	g_barIcon[5].col = D3DCOLOR_RGBA(30,225,180,255);
	{
		//p1
		g_barIcon[7].pos = D3DXVECTOR3( 530, 25, 0.0f);
		g_barIcon[7].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[7].col = D3DCOLOR_RGBA(255,0,0,255); 

		//p2
		g_barIcon[8].pos = D3DXVECTOR3( 640, 25, 0.0f);
		g_barIcon[8].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[8].col = D3DCOLOR_RGBA(255,255,0,255); 

		//p3
		g_barIcon[9].pos = D3DXVECTOR3( 530, 65, 0.0f);
		g_barIcon[9].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[9].col = D3DCOLOR_RGBA(0,255,0,255); 

		//p4
		g_barIcon[10].pos = D3DXVECTOR3( 640, 65, 0.0f);
		g_barIcon[10].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[10].col = D3DCOLOR_RGBA(0,0,255,255); 
	}

	//win
	g_barIcon[4].pos = D3DXVECTOR3( 800, 20, 0.0f);
	g_barIcon[4].size = D3DXVECTOR3( 150, 70, 0.0f);
	g_barIcon[4].col = D3DCOLOR_RGBA(30,225,180,255); 
	{
		//p1
		g_barIcon[11].pos = D3DXVECTOR3( 960, 25, 0.0f);
		g_barIcon[11].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[11].col = D3DCOLOR_RGBA(255,0,0,255); 

		//p2
		g_barIcon[12].pos = D3DXVECTOR3( 1100, 25, 0.0f);
		g_barIcon[12].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[12].col = D3DCOLOR_RGBA(255,255,0,255); 

		//p3
		g_barIcon[13].pos = D3DXVECTOR3( 960, 65, 0.0f);
		g_barIcon[13].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[13].col = D3DCOLOR_RGBA(0,255,0,255); 

		//p4
		g_barIcon[14].pos = D3DXVECTOR3( 1100, 65, 0.0f);
		g_barIcon[14].size = D3DXVECTOR3( 50, 25, 0.0f);
		g_barIcon[14].col = D3DCOLOR_RGBA(0,0,255,255); 
	}

	//頂点情報の作成
	if(FAILED(MakeVertexBarBattle(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P1, &g_pTextureBarBattle[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P2, &g_pTextureBarBattle[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P3, &g_pTextureBarBattle[2]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P4, &g_pTextureBarBattle[3]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_WIN, &g_pTextureBarBattle[4]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_DEATH, &g_pTextureBarBattle[5]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_KILL, &g_pTextureBarBattle[6]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P1, &g_pTextureBarBattle[7]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P2, &g_pTextureBarBattle[8]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P3, &g_pTextureBarBattle[9]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P4, &g_pTextureBarBattle[10]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P1, &g_pTextureBarBattle[11]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P2, &g_pTextureBarBattle[12]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P3, &g_pTextureBarBattle[13]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P4, &g_pTextureBarBattle[14]);


	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawBarBattle(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferBarBattle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntIcon = 0; nCntIcon < MAX_BAR_ICON; nCntIcon++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBarBattle[nCntIcon]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntIcon*4,			//ロードする最初の頂点インデックス
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
void UninitBarBattle(void)
{
	for(int nCntIcon = 0; nCntIcon < MAX_BAR_ICON; nCntIcon++)
	{
		if(g_pTextureBarBattle[nCntIcon] != NULL)
		{
			g_pTextureBarBattle[nCntIcon]->Release();
			g_pTextureBarBattle[nCntIcon] = NULL;
		}
	}

	if(g_pVtxBufferBarBattle != NULL)
	{
		g_pVtxBufferBarBattle->Release();
		g_pVtxBufferBarBattle = NULL;
	}	


}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexBarBattle(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BAR_ICON,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBarBattle,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferBarBattle->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntIcon = 0; nCntIcon < MAX_BAR_ICON; nCntIcon++,pVtx+=4)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3( g_barIcon[nCntIcon].pos.x, g_barIcon[nCntIcon].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_barIcon[nCntIcon].pos.x + g_barIcon[nCntIcon].size.x, g_barIcon[nCntIcon].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_barIcon[nCntIcon].pos.x, g_barIcon[nCntIcon].pos.y + g_barIcon[nCntIcon].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_barIcon[nCntIcon].pos.x + g_barIcon[nCntIcon].size.x, g_barIcon[nCntIcon].pos.y + g_barIcon[nCntIcon].size.y, 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_barIcon[nCntIcon].col;
		pVtx[1].col = g_barIcon[nCntIcon].col;
		pVtx[2].col = g_barIcon[nCntIcon].col;
		pVtx[3].col = g_barIcon[nCntIcon].col;

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferBarBattle->Unlock();

	return S_OK;
}

void UpdateBarBattle(void)
{


}


