//******************************************************************************
//
// タイトル:		リザルトの協力モードの何位を表示処理
// プログラム名:	result_rank_nth.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "result_rank_nth.h"
#include "result_rank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number002.png"// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(150)							// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(180)							// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(50)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(100)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(12)							// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define NUM_DIGIT					(2)
#define MAX_NUM_BOUND				(10)
#define NUM_OBJECT					(NUM_DIGIT*MAX_RANK)			// オブジェクトの数

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexResultRankNth(LPDIRECT3DDEVICE9 pDevice);
void SetTextureResultRankNth( int nRank, int nIdx, int number);
void SetColorResultRankNth( int nRank, int nIdx, D3DCOLOR col);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRankNth = NULL;
LPDIRECT3DTEXTURE9 g_pTextureRankNth = NULL;

RESULT_RANK_NTH	g_numRankNth[MAX_RANK]; //scoreのワーク


/*******************************************************************************
関数名:	HRESULT InitPolygon(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitResultRankNth()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//人数分のみ、初期化
	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{		
		g_numRankNth[nCntRank].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_numRankNth[nCntRank].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_numRankNth[nCntRank].score = 0;
	}
	for(int nCntRank = 0; nCntRank < MAX_RANK/2; nCntRank++)
	{
		g_numRankNth[nCntRank].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y + nCntRank*110, 0.0f);
		g_numRankNth[nCntRank + MAX_RANK/2].pos = D3DXVECTOR3( POLYGON_POS_X + 550, POLYGON_POS_Y + nCntRank*110, 0.0f);
	}

	g_numRankNth[MAX_RANK-1].pos = D3DXVECTOR3( -1000, -1000 , 0.0f);


	//頂点情報の作成
	if(FAILED(MakeVertexResultRankNth(pDevice)))
	{
		return E_FAIL;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_POLYGON,	// ファイルの名前
								&g_pTextureRankNth);	// 読み込むメモリー

	return S_OK;
}

/*******************************************************************************
関数名:	void DrawPolygon(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawResultRankNth(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferRankNth, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankNth);

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
void UninitResultRankNth(void)
{
	if(g_pTextureRankNth != NULL)
	{
		g_pTextureRankNth->Release();
		g_pTextureRankNth = NULL;
	}
	if(g_pVtxBufferRankNth!= NULL)
	{
		g_pVtxBufferRankNth->Release();
		g_pVtxBufferRankNth = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexResultRankNth(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferRankNth,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferRankNth->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for(int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++, pVtx += NUM_VERTEX)
		{

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

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y , 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + g_numRankNth[nCntRank].size.x +  nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y , 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y + g_numRankNth[nCntRank].size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + g_numRankNth[nCntRank].size.x + nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y + g_numRankNth[nCntRank].size.y, 0.0f);

			//テクスチャ座標指定
			pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
			pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
			pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
			pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		}	
	}

	g_pVtxBufferRankNth->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdatePolygon(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdateResultRankNth(void)
{

}



/*******************************************************************************
関数名:	void SetTextureScore( int nTdx, int number) 
引数:	 int nTdx：何桁目か, int number：表示する数
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureResultRankNth( int nRank, int nIdx, int number) 
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferRankNth->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nRank * NUM_DIGIT * NUM_VERTEX);

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
		g_pVtxBufferRankNth->Unlock();
	}
}

void SetColorResultRankNth( int nRank, int nIdx, D3DCOLOR col) 
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferRankNth->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nRank * NUM_DIGIT * NUM_VERTEX);

		//color
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// 頂点データをアンロックする
		g_pVtxBufferRankNth->Unlock();
	}
}

/*******************************************************************************
関数名:	void ChangeScore(int value)
引数:	int value：増加する点数
戻り値:	なし
説明:	総点数を変更する
*******************************************************************************/
void ChangeResultRankNth(int nRank, int value, D3DCOLOR col)
{
	g_numRankNth[nRank].score += value;

	if(g_numRankNth[nRank].score >= MAX_NUM_BOUND) g_numRankNth[nRank].score = MAX_NUM_BOUND; //最大値


	for(int keta = 1; keta <= NUM_DIGIT; keta++)
	{
		if(keta == 1)
		{
			SetTextureResultRankNth(nRank ,keta, 11); //11は点々のテクスチャ
			SetColorResultRankNth(nRank, keta, col );		
		}
		else
		{
			SetTextureResultRankNth(nRank ,keta, value);
			SetColorResultRankNth(nRank, keta, col );			
		}

	}

}


