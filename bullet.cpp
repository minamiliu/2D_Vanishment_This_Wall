//******************************************************************************
//
// タイトル:		爆弾処理
// プログラム名:	bullet.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "bullet.h"
#include "explosion.h"
#include "map.h"
#include "item.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/bomb4.png"	// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(-100)							// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(-100)							// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(50)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(50)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(4)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define COUNTDOWN					(120)							//爆発のカントダウン(フレーム数)

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(int nCntBullet);
void SetTextureBullet(int nCntBullet,int nPatternAnim);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;

BULLET			g_bullet[MAX_BULLET]; //プレイヤーのワーク

/*******************************************************************************
関数名:	HRESULT InitPolygon(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//bulletの初期化
		g_bullet[nCntBullet].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_bullet[nCntBullet].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_bullet[nCntBullet].bUse = false;
		g_bullet[nCntBullet].who_setted = -1;

		// アニメーションの初期化
		g_bullet[nCntBullet].nCounterAnim = 0;
		g_bullet[nCntBullet].nPatternAnim = 0;	
		g_bullet[nCntBullet].nCounterExpo = COUNTDOWN;
	}



	//頂点情報の作成
	if(FAILED(MakeVertexBullet(pDevice)))
	{
		return E_FAIL;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_POLYGON,		// ファイルの名前
								&g_pTextureBullet);	// 読み込むメモリー

	return S_OK;
}

/*******************************************************************************
関数名:	void DrawPolygon(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(g_bullet[nCntBullet].bUse)
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
void UninitBullet(void)
{
	if(g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	if(g_pVtxBufferBullet != NULL)
	{
		g_pVtxBufferBullet->Release();
		g_pVtxBufferBullet = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BULLET,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBullet,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferBullet->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}

	g_pVtxBufferBullet->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdatePolygon(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdateBullet(void)
{
	MAP *pMap = GetMap();

	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(g_bullet[nCntBullet].bUse)
		{
			//頂点座標の設定
			//SetVertexBullet(nCntBullet);

			//爆弾のカウントダウン
			g_bullet[nCntBullet].nCounterExpo--;
			if(g_bullet[nCntBullet].nCounterExpo == 0)
			{
				SetScaleOfExplosion(g_bullet[nCntBullet].pos, g_bullet[nCntBullet].power, g_bullet[nCntBullet].who_setted);
				pMap->item[(int)Pos2DotPos(g_bullet[nCntBullet].pos).y][(int)Pos2DotPos(g_bullet[nCntBullet].pos).x].type = ITEM_NONE;
				g_bullet[nCntBullet].bUse = false;
				g_bullet[nCntBullet].nCounterExpo = COUNTDOWN;
			}

			g_bullet[nCntBullet].nCounterAnim++;
			if((g_bullet[nCntBullet].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{

				// パターンの切り替え
				g_bullet[nCntBullet].nPatternAnim = (g_bullet[nCntBullet].nPatternAnim + 1) % NUM_ANIM_PATTERN;

				// テクスチャ座標を設定
				SetTextureBullet(nCntBullet,g_bullet[nCntBullet].nPatternAnim);
			
				//g_nCounterAnimのリセット
				g_bullet[nCntBullet].nCounterAnim = 0;
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
void SetVertexBullet(int nCntBullet)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBullet * NUM_VERTEX; 

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		
		

		// 頂点データをアンロックする
		g_pVtxBufferBullet->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetTexturePolygon(int nPatternAnim)
引数:	int nPatternAnim : アニメーションパターンNo.
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureBullet(int nCntBullet,int nPatternAnim)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;
	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBufferBullet->Unlock();

}

/*******************************************************************************
関数名:	void SetBullet(D3DXVECTOR3 pos)
引数:	D3DXVECTOR3 pos：弾を設置する位置
戻り値:	なし
説明:	弾の設置
*******************************************************************************/
void SetBullet(int nPlayerIdx, D3DXVECTOR3 pos, int power)
{
	MAP *pMap = GetMap();


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(!g_bullet[nCntBullet].bUse) 
		{
			//位置を設定			
			g_bullet[nCntBullet].pos = pos;
			g_bullet[nCntBullet].power = power;
			SetVertexBullet(nCntBullet);
			g_bullet[nCntBullet].bUse = true; //発射中に変更
			g_bullet[nCntBullet].who_setted = nPlayerIdx;

			//item mapに記録
			pMap->item[(int)Pos2DotPos(pos).y][(int)Pos2DotPos(pos).x].index = nCntBullet;
			pMap->item[(int)Pos2DotPos(pos).y][(int)Pos2DotPos(pos).x].type = ITEM_SETTED_BOMB;
			pMap->item[(int)Pos2DotPos(pos).y][(int)Pos2DotPos(pos).x].pos = pos;
			break;
		}
	}
	SetSE(SOUND_LABEL_SE_SETBOMB);
}

/*******************************************************************************
関数名:	BULLET* GetBullet(void)
引数:	なし
戻り値: BULLET*：BULLETのポインタ
説明:	当たり判定などの時、弾の情報を読み取り
*******************************************************************************/
BULLET *GetBullet(void)
{
	return g_bullet;
}

