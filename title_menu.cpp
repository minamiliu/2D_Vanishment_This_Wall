//******************************************************************************
//
// タイトル:		タイトルのメニュー処理
// プログラム名:	title_menu.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "title_menu.h"
#include "input.h"
#include "sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_TITLE_MENU	(3)
#define POLYGON_SIZE_X	(300)//ポリゴンのSIZE X
#define POLYGON_SIZE_Y	(80)//ポリゴンのSIZE Y

#define POLYGON_TEXTURENAME_TITLE_MENU_0		"data/TEXTURE/TITLE/menu_COOPERATE.png"
#define POLYGON_TEXTURENAME_TITLE_MENU_1		"data/TEXTURE/TITLE/menu_BATTLE.png"
#define POLYGON_TEXTURENAME_TITLE_MENU_2		"data/TEXTURE/TITLE/menu_RANK.png"
//#define POLYGON_TEXTURENAME_TITLE_MENU_3		"data/TEXTURE/TITLE/menu_stageEditor.png"
//#define POLYGON_TEXTURENAME_TITLE_MENU_4		"data/TEXTURE/TITLE/menu_ranking.png"

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexTitleMenu(LPDIRECT3DDEVICE9 pDevice);
void SetColorTitleMenu(int nMenuIdx, int R, int G, int B, int A);
void SetVertexTitleMenu(int nMenuIdx);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitleMenu = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitleMenu[MAX_TITLE_MENU] = {NULL};

TITLE_MENU	g_titleMenu[MAX_TITLE_MENU];

int g_menu;
float g_angle;
/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitTitleMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_menu = 0;
	g_angle = 0;

	//menu_cooperate
	g_titleMenu[0].pos = D3DXVECTOR3( 625, 450, 0.0f);
	g_titleMenu[0].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	g_titleMenu[0].col = D3DCOLOR_RGBA(255,255,0,255); 

	//menu_battle
	g_titleMenu[1].pos = D3DXVECTOR3( 625, 550, 0.0f);
	g_titleMenu[1].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	g_titleMenu[1].col = D3DCOLOR_RGBA(255,255,255,255); 

	//menu_rank
	g_titleMenu[2].pos = D3DXVECTOR3( 625, 650, 0.0f);
	g_titleMenu[2].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	g_titleMenu[2].col = D3DCOLOR_RGBA(255,255,255,255); 

	////stage editor
	//g_titleMenu[3].pos = D3DXVECTOR3( 625, 600, 0.0f);
	//g_titleMenu[3].size = D3DXVECTOR3( 200, 45, 0.0f);
	//g_titleMenu[3].col = D3DCOLOR_RGBA(255,255,255,255); 

	////exit
	//g_titleMenu[4].pos = D3DXVECTOR3( 625, 650, 0.0f);
	//g_titleMenu[4].size = D3DXVECTOR3( 200, 45, 0.0f);
	//g_titleMenu[4].col = D3DCOLOR_RGBA(255,255,255,255); 


	//頂点情報の作成
	if(FAILED(MakeVertexTitleMenu(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_0, &g_pTextureTitleMenu[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_1, &g_pTextureTitleMenu[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_2, &g_pTextureTitleMenu[2]);
	//D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_3, &g_pTextureTitleMenu[3]);
	//D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_4, &g_pTextureTitleMenu[4]);

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawTitleMenu(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferTitleMenu, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntIcon = 0; nCntIcon < MAX_TITLE_MENU; nCntIcon++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitleMenu[nCntIcon]);

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
void UninitTitleMenu(void)
{
	for(int nCntIcon = 0; nCntIcon < MAX_TITLE_MENU; nCntIcon++)
	{
		if(g_pTextureTitleMenu[nCntIcon] != NULL)
		{
			g_pTextureTitleMenu[nCntIcon]->Release();
			g_pTextureTitleMenu[nCntIcon] = NULL;
		}
	}

	if(g_pVtxBufferTitleMenu != NULL)
	{
		g_pVtxBufferTitleMenu->Release();
		g_pVtxBufferTitleMenu = NULL;
	}	


}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexTitleMenu(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_TITLE_MENU,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferTitleMenu,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferTitleMenu->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntIcon = 0; nCntIcon < MAX_TITLE_MENU; nCntIcon++,pVtx+=4)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x - (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y - (g_titleMenu[nCntIcon].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x + (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y - (g_titleMenu[nCntIcon].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x - (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y + (g_titleMenu[nCntIcon].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x + (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y + (g_titleMenu[nCntIcon].size.y/2), 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_titleMenu[nCntIcon].col;
		pVtx[1].col = g_titleMenu[nCntIcon].col;
		pVtx[2].col = g_titleMenu[nCntIcon].col;
		pVtx[3].col = g_titleMenu[nCntIcon].col;

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferTitleMenu->Unlock();

	return S_OK;
}

void UpdateTitleMenu(void)
{

	if(GetKeyboardTrigger(DIK_DOWN) || GetGamePadTrigger( 0, BUTTON_DOWN) )
	{
		SetColorTitleMenu( g_menu, 255, 255, 255, 255);
		g_titleMenu[g_menu].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		SetVertexTitleMenu(g_menu);
		g_angle = 0;

		g_menu++;
		if(g_menu == MAX_TITLE_MENU) g_menu = 0;

		//SE
		SetSE(SOUND_LABEL_SE_SELECT);
	}
	if(GetKeyboardTrigger(DIK_UP) || GetGamePadTrigger( 0, BUTTON_UP) )
	{
		SetColorTitleMenu( g_menu, 255, 255, 255, 255);
		g_titleMenu[g_menu].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		SetVertexTitleMenu(g_menu);
		g_angle = 0;

		g_menu--;
		if(g_menu == -1) g_menu = MAX_TITLE_MENU - 1;

		//SE
		SetSE(SOUND_LABEL_SE_SELECT);
	}
	
	//change color
	SetColorTitleMenu( g_menu, 255, 255, 0, 255);

	//change size
	g_titleMenu[g_menu].size.x += sin(g_angle);
	g_titleMenu[g_menu].size.y += sin(g_angle);
	g_angle += 0.1f;
	if(g_angle >= 6.28) g_angle = 0;

	SetVertexTitleMenu(g_menu);
}

void SetColorTitleMenu(int nMenuIdx, int R, int G, int B, int A)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nMenuIdx * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// 頂点データをアンロックする
	g_pVtxBufferTitleMenu->Unlock();
	
}

void SetVertexTitleMenu(int nMenuIdx)
{

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferTitleMenu->Lock( 0, 0, (void**)&pVtx, 0);

	pVtx += nMenuIdx * NUM_VERTEX; 

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x - (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y - (g_titleMenu[nMenuIdx].size.y/2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x + (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y - (g_titleMenu[nMenuIdx].size.y/2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x - (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y + (g_titleMenu[nMenuIdx].size.y/2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x + (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y + (g_titleMenu[nMenuIdx].size.y/2), 0.0f);

	// 頂点データをアンロックする
	g_pVtxBufferTitleMenu->Unlock();
}

int GetTitleMenu(void)
{
	return g_menu;
}


