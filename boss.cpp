//******************************************************************************
//
// タイトル:		協力モードのボス処理
// プログラム名:	boss.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "boss.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "map.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/boss0001.png"		// 読み込むテクスチャファイル名
#define	POLYGON_DOTPOS_X			(0)								// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_DOTPOS_Y			(0)								// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(400.0f)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(400.0f)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(3)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(4)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(1.0f)							// ポリゴンの移動量



/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexBoss(LPDIRECT3DDEVICE9 pDevice);
void SetTextureBoss(int nCntBoss, int nPatternAnim, DIRECTION dir);
void SetVertexBoss(int nCntBoss);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBoss = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBoss = NULL;

BOSS	g_boss[MAX_BOSS]; //プレイヤーのワーク
bool hurt_flag;

/*******************************************************************************
関数名:	HRESULT InitEnemy(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		//enemyの初期化
		int randX, randY;
		do
		{
			randX = rand()%5 + 10;
			randY = rand()%5 + 5;
		}while(pMap->data[randY][randX] != BLOCK_GRASS);

		g_boss[nCntBoss].dotPos = D3DXVECTOR3( randX, randY, 0.0f);

		g_boss[nCntBoss].pos = DotPos2Pos(g_boss[nCntBoss].dotPos);
		g_boss[nCntBoss].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_boss[nCntBoss].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_boss[nCntBoss].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_boss[nCntBoss].bUse = false;	
		g_boss[nCntBoss].dir = D_DOWN;
		g_boss[nCntBoss].walking = false;
		g_boss[nCntBoss].life = 15;
		g_boss[nCntBoss].nCounterHurt = -1;
		g_boss[nCntBoss].nCounterDeath = -1;
	}
	//BOSS stage
	int stage = GetStage();
	if(stage == 2) //stage 3
	{ 
		g_boss[0].bUse = true;
	}

	// アニメーションの初期化
	hurt_flag = false;


	//頂点情報の作成
	if(FAILED(MakeVertexBoss(pDevice)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_POLYGON,		// ファイルの名前
								&g_pTextureBoss);	// 読み込むメモリー

	return S_OK;
}

/*******************************************************************************
関数名:	void DrawEnemy(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBoss);

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(g_boss[nCntBoss].bUse)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntBoss*NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);
		}	
	}



}

/*******************************************************************************
関数名:	void UninitEnemy(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの開放関数
*******************************************************************************/
void UninitBoss(void)
{
	if(g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}
	if(g_pVtxBufferBoss != NULL)
	{
		g_pVtxBufferBoss->Release();
		g_pVtxBufferBoss = NULL;
	}

	//BOSSを利用しないに設定する
	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_boss[nCntBoss].bUse = false;	

	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexBoss(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BOSS,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBoss,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferBoss->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	
	}



	g_pVtxBufferBoss->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdateEnemy(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdateBoss(void)
{	
	DIRECTION new_dir;
	MAP *pMap = GetMap();


	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(g_boss[nCntBoss].bUse)
		{

			//移動位置調整
			if(fabs(g_boss[nCntBoss].pos.x - DotPos2Pos(g_boss[nCntBoss].dotPos).x) < VALUE_MOVE && fabs(g_boss[nCntBoss].pos.y - DotPos2Pos(g_boss[nCntBoss].dotPos).y) < VALUE_MOVE )
			{
				g_boss[nCntBoss].pos = DotPos2Pos(g_boss[nCntBoss].dotPos);
				g_boss[nCntBoss].walking = false;
			}
			
			//次の位置移動したら
			if(g_boss[nCntBoss].pos == DotPos2Pos(g_boss[nCntBoss].dotPos))
			{
				g_boss[nCntBoss].walking = false;
			}

			//死亡
			if(g_boss[nCntBoss].life == 0)
			{
				g_boss[nCntBoss].nCounterDeath = 120; //hurt anime 60s
				g_boss[nCntBoss].nCounterHurt = -2; //do not use
				g_boss[nCntBoss].life = -1;
				SetSE(SOUND_LABEL_SE_GAMESTART);
				break;
			}

			//if Boss Hurt
			if(g_boss[nCntBoss].nCounterHurt > 0)
			{
				g_boss[nCntBoss].nCounterHurt--;
				if(g_boss[nCntBoss].nCounterHurt % 10 == 0)
				{
					hurt_flag = !hurt_flag;				
				}
				if(hurt_flag)
				{
					SetColorBoss(nCntBoss, 255, 0, 0, 255); //赤
				}
				else
				{
					SetColorBoss(nCntBoss, 255, 255, 255, 255);
				}
			}
			else if(g_boss[nCntBoss].nCounterHurt == 0)
			{
				SetColorBoss(nCntBoss, 255, 255, 255, 255);
				g_boss[nCntBoss].nCounterHurt = -1;
				hurt_flag = false;
			}


			//if Boss Died
			if(g_boss[nCntBoss].nCounterDeath > 0)
			{
				g_boss[nCntBoss].nCounterDeath--;
				if(g_boss[nCntBoss].nCounterDeath % 5 == 0)
				{
					hurt_flag = !hurt_flag;				
				}
				if(hurt_flag)
				{
					SetColorBoss(nCntBoss, 255, 0, 0, 255); //赤
					SetSE(SOUND_LABEL_SE_BOSSHURT);
				}
				else
				{
					SetColorBoss(nCntBoss, 255, 255, 255, 255);
				}
			}
			else if(g_boss[nCntBoss].nCounterDeath == 0)
			{
				SetColorBoss(nCntBoss, 255, 255, 255, 0);
				g_boss[nCntBoss].nCounterDeath = -2;
				g_boss[nCntBoss].bUse = false;
				hurt_flag = false;
				SetItem(ITEM_NEXT,g_boss[nCntBoss].dotPos); //NEXTを現す

			}

			//if BOSS died
			if(g_boss[nCntBoss].life == -1) break;

			if(g_boss[nCntBoss].walking)
			{
				switch(g_boss[nCntBoss].dir)
				{
					case D_UP:

						g_boss[nCntBoss].pos.y -= g_boss[nCntBoss].move.y;
						break;
					case D_DOWN:

						g_boss[nCntBoss].pos.y += g_boss[nCntBoss].move.y;
						break;
					case D_LEFT:

						g_boss[nCntBoss].pos.x -= g_boss[nCntBoss].move.x;
						break;
					case D_RIGHT:

						g_boss[nCntBoss].pos.x += g_boss[nCntBoss].move.x;
						break;
				}
			}
			else
			{
				//random移動 
				new_dir = (DIRECTION)(rand()%4);
				
				// 移動
				if(new_dir == D_UP)
				{
					g_boss[nCntBoss].dir = D_UP;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y - 1][(int)g_boss[nCntBoss].dotPos.x] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.y -= g_boss[nCntBoss].move.y;
						g_boss[nCntBoss].dotPos.y -= 1;		
						g_boss[nCntBoss].walking = true;
					}
				}
				if(new_dir == D_DOWN)
				{
					g_boss[nCntBoss].dir = D_DOWN;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y + 1][(int)g_boss[nCntBoss].dotPos.x] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.y += g_boss[nCntBoss].move.y;
						g_boss[nCntBoss].dotPos.y += 1;
						g_boss[nCntBoss].walking = true;
					}

				}
				if(new_dir == D_LEFT)
				{
					g_boss[nCntBoss].dir = D_LEFT;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y][(int)g_boss[nCntBoss].dotPos.x - 1] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.x -= g_boss[nCntBoss].move.x;
						g_boss[nCntBoss].dotPos.x -= 1;
						g_boss[nCntBoss].walking = true;
					}
				}
				if(new_dir == D_RIGHT)
				{
					g_boss[nCntBoss].dir = D_RIGHT;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y][(int)g_boss[nCntBoss].dotPos.x + 1] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.x += g_boss[nCntBoss].move.x;
						g_boss[nCntBoss].dotPos.x += 1;
						g_boss[nCntBoss].walking = true;
					}
				}
			}


			SetVertexBoss(nCntBoss);


			//向き変更
			SetTextureBoss(nCntBoss, g_boss[nCntBoss].nPatternAnim, g_boss[nCntBoss].dir);
	
			//アニメ表現
			g_boss[nCntBoss].nCounterAnim++;
			if((g_boss[nCntBoss].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// パターンの切り替え
				g_boss[nCntBoss].nPatternAnim = (g_boss[nCntBoss].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// テクスチャ座標を設定
				SetTextureBoss(nCntBoss, g_boss[nCntBoss].nPatternAnim, g_boss[nCntBoss].dir);

				//nCounterAnimのリセット
				g_boss[nCntBoss].nCounterAnim = 0;
			}	
		}
	}
	
}

/*******************************************************************************
関数名:	void SetVertexEnemy(void)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexBoss(int nCntBoss)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);
		
		pVtx += nCntBoss * NUM_VERTEX; 

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		// 頂点データをアンロックする
		g_pVtxBufferBoss->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetTextureEnemy(int nPatternAnim)
引数:	int nPatternAnim : アニメーションパターンNo.
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureBoss(int nCntBoss, int nPatternAnim, DIRECTION dir)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBoss * NUM_VERTEX; 

		// テクスチャ座標の設定
		fPosXLeft	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X);
		fPosXRight	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X + 1);
		fPosYUp		= TEX_PATTERN_SIZE_Y * dir;
		fPosYDown	= TEX_PATTERN_SIZE_Y * (dir + 1);


		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// 頂点データをアンロックする
		g_pVtxBufferBoss->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetEnemy(D3DXVECTOR3 pos)
引数:	D3DXVECTOR3 pos：敵を設置する位置
戻り値:	なし
説明:	敵の設置
*******************************************************************************/
void SetBoss(D3DXVECTOR3 pos)
{
	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(!g_boss[nCntBoss].bUse)
		{
			//位置を設定
			g_boss[nCntBoss].pos = pos;
			SetVertexBoss(nCntBoss);
			g_boss[nCntBoss].bUse = true; //発射中に変更
			break;
		}
	}

}

void SetColorBoss(int nCntBoss, int R, int G, int B, int A)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntBoss * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// 頂点データをアンロックする
	g_pVtxBufferBoss->Unlock();
	
}



/*******************************************************************************
関数名:	BULLET* GetBullet(void)
引数:	なし
戻り値: BULLET*：BULLETのポインタ
説明:	当たり判定などの時、弾の情報を読み取り
*******************************************************************************/
BOSS *GetBoss(void)
{
	return g_boss;
}