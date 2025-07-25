//******************************************************************************
//
// タイトル:		タイトルの爆弾テクスチャ処理
// プログラム名:	title_bomb.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "title_bomb.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/bomb4.png"	// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(250)							// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(550)							// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(300)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(300)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(4)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define COUNTDOWN					(120)							//爆発のカントダウン(フレーム数)
#define MAX_TITLE_BOMB				(1)			

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexTitleBomb(LPDIRECT3DDEVICE9 pDevice);
void SetVertexTitleBomb(int nCntBullet);
void SetTextureTitleBomb(int nCntBullet,int nPatternAnim);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitleBomb = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitleBomb = NULL;

TITLE_BOMB			g_title_bomb[MAX_TITLE_BOMB]; //プレイヤーのワーク

/*******************************************************************************
関数名:	HRESULT InitPolygon(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitTitleBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++)
	{
		//bulletの初期化
		g_title_bomb[nCntBullet].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_title_bomb[nCntBullet].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_title_bomb[nCntBullet].bUse = true;

		// アニメーションの初期化
		g_title_bomb[nCntBullet].nCounterAnim = 0;
		g_title_bomb[nCntBullet].nPatternAnim = 0;	
	}



	//頂点情報の作成
	if(FAILED(MakeVertexTitleBomb(pDevice)))
	{
		return E_FAIL;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_POLYGON,		// ファイルの名前
								&g_pTextureTitleBomb);	// 読み込むメモリー

	return S_OK;
}

/*******************************************************************************
関数名:	void DrawPolygon(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawTitleBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferTitleBomb, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleBomb);


	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++)
	{
		if(g_title_bomb[nCntBullet].bUse)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntBullet*NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);
		}
	}



}

/*******************************************************************************
関数名:	void UninitPolygon(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの開放関数
*******************************************************************************/
void UninitTitleBomb(void)
{
	if(g_pTextureTitleBomb != NULL)
	{
		g_pTextureTitleBomb->Release();
		g_pTextureTitleBomb = NULL;
	}
	if(g_pVtxBufferTitleBomb != NULL)
	{
		g_pVtxBufferTitleBomb->Release();
		g_pVtxBufferTitleBomb = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexTitleBomb(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_TITLE_BOMB,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferTitleBomb,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferTitleBomb->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}

	g_pVtxBufferTitleBomb->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdatePolygon(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdateTitleBomb(void)
{

	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++)
	{
		if(g_title_bomb[nCntBullet].bUse)
		{
			//頂点座標の設定
			SetVertexTitleBomb(nCntBullet);

			g_title_bomb[nCntBullet].nCounterAnim++;
			if((g_title_bomb[nCntBullet].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{

				// パターンの切り替え
				g_title_bomb[nCntBullet].nPatternAnim = (g_title_bomb[nCntBullet].nPatternAnim + 1) % NUM_ANIM_PATTERN;

				// テクスチャ座標を設定
				SetTextureTitleBomb(nCntBullet,g_title_bomb[nCntBullet].nPatternAnim);
			
				//g_nCounterAnimのリセット
				g_title_bomb[nCntBullet].nCounterAnim = 0;
			}

		}
	}
}

/*******************************************************************************
関数名:	void SetVertexPolygon(void)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexTitleBomb(int nCntBullet)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferTitleBomb->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBullet * NUM_VERTEX; 

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		
		

		// 頂点データをアンロックする
		g_pVtxBufferTitleBomb->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetTexturePolygon(int nPatternAnim)
引数:	int nPatternAnim : アニメーションパターンNo.
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureTitleBomb(int nCntBullet,int nPatternAnim)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;
	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferTitleBomb->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntBullet*NUM_VERTEX;

	// テクスチャ座標の設定
	fPosXLeft	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X);
	fPosXRight	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X + 1);
	fPosYUp		= TEX_PATTERN_SIZE_Y * (nPatternAnim / TEX_PATTERN_DIVIDE_X);
	fPosYDown	= TEX_PATTERN_SIZE_Y * (nPatternAnim / TEX_PATTERN_DIVIDE_X + 1);


	pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
	pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
	pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
	pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

	// 頂点データをアンロックする
	g_pVtxBufferTitleBomb->Unlock();

}





