//******************************************************************************
//
// タイトル:		メッセージ表示処理
// プログラム名:	message.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "message.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_MSG	(13)

#define POLYGON_POS_X	(625)
#define POLYGON_POS_Y	(375)

#define POLYGON_TEXTURENAME_MSG_0		"data/TEXTURE/MSG/GAME_OVER.png"
#define POLYGON_TEXTURENAME_MSG_1		"data/TEXTURE/MSG/STAGE_CLEAR.png"
#define POLYGON_TEXTURENAME_MSG_2		"data/TEXTURE/MSG/RANK.png"
#define POLYGON_TEXTURENAME_MSG_3		"data/TEXTURE/MSG/PAUSE.png"
#define POLYGON_TEXTURENAME_MSG_4		"data/TEXTURE/MSG/TIME_UP.png"
#define POLYGON_TEXTURENAME_MSG_5		"data/TEXTURE/MSG/GAME_SET.png"
#define POLYGON_TEXTURENAME_MSG_6		"data/TEXTURE/MSG/DRAW.png"
#define POLYGON_TEXTURENAME_MSG_7		"data/TEXTURE/MSG/WIN.png"
#define POLYGON_TEXTURENAME_MSG_8		"data/TEXTURE/MSG/1P.png"
#define POLYGON_TEXTURENAME_MSG_9		"data/TEXTURE/MSG/2P.png"
#define POLYGON_TEXTURENAME_MSG_10		"data/TEXTURE/MSG/3P.png"
#define POLYGON_TEXTURENAME_MSG_11		"data/TEXTURE/MSG/4P.png"
#define POLYGON_TEXTURENAME_MSG_12		"data/TEXTURE/MSG/RESULT.png"

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexMessage(LPDIRECT3DDEVICE9 pDevice);
void SetColorMessage(int nMenuIdx, int R, int G, int B, int A);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMessage = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMessage[MAX_MSG] = {NULL};

MESSAGE	g_msg[MAX_MSG];


/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitMessage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntMsg = 0; nCntMsg < MAX_MSG; nCntMsg++)
	{
		g_msg[nCntMsg].bUse = false;
	}

	//GAME_OVER
	g_msg[0].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[0].size = D3DXVECTOR3( 900, 100, 0.0f);
	g_msg[0].col = D3DCOLOR_RGBA(255,255,0,255); 

	//STAGE_CLEAR
	g_msg[1].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[1].size = D3DXVECTOR3( 800, 100, 0.0f);
	g_msg[1].col = D3DCOLOR_RGBA(255,255,0,255); 

	//RANK
	g_msg[2].pos = D3DXVECTOR3( POLYGON_POS_X, 100, 0.0f);
	g_msg[2].size = D3DXVECTOR3( 400, 100, 0.0f);
	//g_msg[2].col = D3DCOLOR_RGBA(255,0,0,255); 
	g_msg[2].col = D3DCOLOR_RGBA(30,225,180,255);

	//PAUSE
	g_msg[3].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[3].size = D3DXVECTOR3( 500, 100, 0.0f);
	g_msg[3].col = D3DCOLOR_RGBA(255,255,0,255); 

	//TIME_UP
	g_msg[4].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[4].size = D3DXVECTOR3( 700, 100, 0.0f);
	g_msg[4].col = D3DCOLOR_RGBA(255,255,0,255); 

	//GAME_SET
	g_msg[5].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y + 100, 0.0f);
	g_msg[5].size = D3DXVECTOR3( 800, 100, 0.0f);
	g_msg[5].col = D3DCOLOR_RGBA(255,255,0,255);

	//DRAW
	g_msg[6].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[6].size = D3DXVECTOR3( 400, 100, 0.0f);
	g_msg[6].col = D3DCOLOR_RGBA(255,255,0,255);

	//WIN
	g_msg[7].pos = D3DXVECTOR3( POLYGON_POS_X + 150, POLYGON_POS_Y, 0.0f);
	g_msg[7].size = D3DXVECTOR3( 300, 100, 0.0f);
	g_msg[7].col = D3DCOLOR_RGBA(255,255,0,255);

	//1P
	g_msg[8].pos = D3DXVECTOR3( POLYGON_POS_X - 150, POLYGON_POS_Y, 0.0f);
	g_msg[8].size = D3DXVECTOR3( 300, 100, 0.0f);
	g_msg[8].col = D3DCOLOR_RGBA(255,0,0,255); //red 

	//2P
	g_msg[9].pos = D3DXVECTOR3( POLYGON_POS_X - 150, POLYGON_POS_Y, 0.0f);
	g_msg[9].size = D3DXVECTOR3( 300, 100, 0.0f);
	g_msg[9].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

	//3P
	g_msg[10].pos = D3DXVECTOR3( POLYGON_POS_X - 150, POLYGON_POS_Y, 0.0f);
	g_msg[10].size = D3DXVECTOR3( 300, 100, 0.0f);
	g_msg[10].col = D3DCOLOR_RGBA(0,255,0,255); //green

	//4P
	g_msg[11].pos = D3DXVECTOR3( POLYGON_POS_X - 150, POLYGON_POS_Y, 0.0f);
	g_msg[11].size = D3DXVECTOR3( 300, 100, 0.0f);
	g_msg[11].col = D3DCOLOR_RGBA(0,0,255,255); //blue

	//RESULT
	g_msg[12].pos = D3DXVECTOR3( POLYGON_POS_X, 100, 0.0f);
	g_msg[12].size = D3DXVECTOR3( 500, 100, 0.0f);
	//g_msg[12].col = D3DCOLOR_RGBA(255,0,0,255);
	g_msg[12].col = D3DCOLOR_RGBA(30,225,180,255);

	//頂点情報の作成
	if(FAILED(MakeVertexMessage(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_0, &g_pTextureMessage[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_1, &g_pTextureMessage[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_2, &g_pTextureMessage[2]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_3, &g_pTextureMessage[3]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_4, &g_pTextureMessage[4]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_5, &g_pTextureMessage[5]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_6, &g_pTextureMessage[6]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_7, &g_pTextureMessage[7]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_8, &g_pTextureMessage[8]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_9, &g_pTextureMessage[9]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_10, &g_pTextureMessage[10]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_11, &g_pTextureMessage[11]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_12, &g_pTextureMessage[12]);

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawMessage(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferMessage, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntMsg = 0; nCntMsg < MAX_MSG; nCntMsg++)
	{
		if(g_msg[nCntMsg].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMessage[nCntMsg]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntMsg*4,			//ロードする最初の頂点インデックス
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
void UninitMessage(void)
{
	for(int nCntMsg = 0; nCntMsg < MAX_MSG; nCntMsg++)
	{
		if(g_pTextureMessage[nCntMsg] != NULL)
		{
			g_pTextureMessage[nCntMsg]->Release();
			g_pTextureMessage[nCntMsg] = NULL;
		}
	}

	if(g_pVtxBufferMessage != NULL)
	{
		g_pVtxBufferMessage->Release();
		g_pVtxBufferMessage = NULL;
	}	


}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexMessage(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_MSG,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferMessage,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferMessage->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntMsg = 0; nCntMsg < MAX_MSG; nCntMsg++,pVtx+=4)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_msg[nCntMsg].pos.x - (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y - (g_msg[nCntMsg].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_msg[nCntMsg].pos.x + (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y - (g_msg[nCntMsg].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_msg[nCntMsg].pos.x - (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y + (g_msg[nCntMsg].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_msg[nCntMsg].pos.x + (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y + (g_msg[nCntMsg].size.y/2), 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_msg[nCntMsg].col;
		pVtx[1].col = g_msg[nCntMsg].col;
		pVtx[2].col = g_msg[nCntMsg].col;
		pVtx[3].col = g_msg[nCntMsg].col;

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferMessage->Unlock();

	return S_OK;
}

void UpdateMessage(void)
{

}

void SetColorMessage(int nMenuIdx, int R, int G, int B, int A)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferMessage->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nMenuIdx * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// 頂点データをアンロックする
	g_pVtxBufferMessage->Unlock();
	
}

void SetMessage(int nMsgIdx)
{
	g_msg[nMsgIdx].bUse = true;
}

void ClearMessage(int nMsgIdx)
{
	g_msg[nMsgIdx].bUse = false;
}

void ClearMessage(void)
{
	for(int nCntMsg = 0; nCntMsg < MAX_MSG; nCntMsg++)
	{
		g_msg[nCntMsg].bUse = false;
	}
}



