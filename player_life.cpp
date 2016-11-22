//******************************************************************************
//
// タイトル:		プレイヤーのライフ表示処理
// プログラム名:	player_life.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "player_life.h"
#include "player.h"
#include "CS_select.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number003.png"// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(0)							// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(60)							// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(20)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(40)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(10)							// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define NUM_DIGIT					(2)
#define NUM_OBJECT					(NUM_DIGIT*MAX_PLAYER)			// オブジェクトの数

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexPlayerLife(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexScore(void);
void SetTexturePlayerLife( int nPlayerIdx, int nIdx, int number);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayerLife = NULL;
LPDIRECT3DTEXTURE9 g_pTexturePlayerLife = NULL;

LIFE			g_life[MAX_PLAYER]; //scoreのワーク

int g_lifePosX[MAX_PLAYER] = {100,400,800,1100};

/*******************************************************************************
関数名:	HRESULT InitPolygon(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitPlayerLife()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//bool *pPlayerList = GetPlayerList();

	//P1 life
	g_life[0].pos = D3DXVECTOR3( 220, 20, 0.0f);
	g_life[0].col = D3DCOLOR_RGBA(255,0,0,255); //red 

	//P2 life
	g_life[1].pos = D3DXVECTOR3( 330, 20, 0.0f);
	g_life[1].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

	//P3 life
	g_life[2].pos = D3DXVECTOR3( 220, 60, 0.0f);
	g_life[2].col = D3DCOLOR_RGBA(0,255,0,255); //green

	//P4 life
	g_life[3].pos = D3DXVECTOR3( 330, 60, 0.0f);
	g_life[3].col = D3DCOLOR_RGBA(0,0,255,255); //blue


	//人数分のみ、初期化
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_life[nCntPlayer].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_life[nCntPlayer].score = PLAYER_LIFE;
	}


	//頂点情報の作成
	if(FAILED(MakeVertexPlayerLife(pDevice)))
	{
		return E_FAIL;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_POLYGON,	// ファイルの名前
								&g_pTexturePlayerLife);	// 読み込むメモリー

	return S_OK;
}

/*******************************************************************************
関数名:	void DrawPolygon(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawPlayerLife(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferPlayerLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayerLife);

	//ポリゴンの描画
	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			(nCntPlace*NUM_VERTEX),						//ロードする最初の頂点インデックス
			NUM_POLYGON				//ポリゴンの数
		);
	}


}

/*******************************************************************************
関数名:	void UninitScore(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの開放関数
*******************************************************************************/
void UninitPlayerLife(void)
{
	if(g_pTexturePlayerLife != NULL)
	{
		g_pTexturePlayerLife->Release();
		g_pTexturePlayerLife = NULL;
	}
	if(g_pVtxBufferPlayerLife!= NULL)
	{
		g_pVtxBufferPlayerLife->Release();
		g_pVtxBufferPlayerLife = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexPlayerLife(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferPlayerLife,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferPlayerLife->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for(int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++, pVtx += NUM_VERTEX)
		{

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			//color
			pVtx[0].col = g_life[nCntPlayer].col;
			pVtx[1].col = g_life[nCntPlayer].col;
			pVtx[2].col = g_life[nCntPlayer].col;
			pVtx[3].col = g_life[nCntPlayer].col;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_life[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_life[nCntPlayer].pos.y , 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_life[nCntPlayer].pos.x  + g_life[nCntPlayer].size.x +  nCntDigit*POLYGON_SIZE_X, g_life[nCntPlayer].pos.y , 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_life[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_life[nCntPlayer].pos.y + g_life[nCntPlayer].size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_life[nCntPlayer].pos.x  + g_life[nCntPlayer].size.x + nCntDigit*POLYGON_SIZE_X, g_life[nCntPlayer].pos.y + g_life[nCntPlayer].size.y, 0.0f);

			//テクスチャ座標指定
			pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
			pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
			pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
			pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		}	
	}

	g_pVtxBufferPlayerLife->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdatePolygon(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdatePlayerLife(void)
{

}



/*******************************************************************************
関数名:	void SetTextureScore( int nTdx, int number) 
引数:	 int nTdx：何桁目か, int number：表示する数
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTexturePlayerLife( int nPlayerIdx, int nIdx, int number) 
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferPlayerLife->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nPlayerIdx * NUM_DIGIT * NUM_VERTEX);

		//テクスチャ座標の設定
		fPosXLeft = TEX_PATTERN_SIZE_X * number;
		fPosXRight = TEX_PATTERN_SIZE_X * number + TEX_PATTERN_SIZE_X;
		fPosYUp = 0;
		fPosYDown = TEX_PATTERN_SIZE_Y;

		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// 頂点データをアンロックする
		g_pVtxBufferPlayerLife->Unlock();
	}
}

/*******************************************************************************
関数名:	void ChangeScore(int value)
引数:	int value：増加する点数
戻り値:	なし
説明:	総点数を変更する
*******************************************************************************/
void ChangePlayerLife(int nPlayerIdx, int value)
{
	g_life[nPlayerIdx].score += value;

	if(g_life[nPlayerIdx].score >= 999999) g_life[nPlayerIdx].score = 999999; //6桁の最大値

	int number = 0;

	for(int keta = 1; keta <= NUM_DIGIT; keta++)
	{
		number = g_life[nPlayerIdx].score % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTexturePlayerLife(nPlayerIdx ,keta,number);
	}
}
