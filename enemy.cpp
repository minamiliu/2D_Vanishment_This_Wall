//******************************************************************************
//
// タイトル:		協力モードの敵処理
// プログラム名:	enemy.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "map.h"

#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/teki0001.png"		// 読み込むテクスチャファイル名
#define	POLYGON_DOTPOS_X			(0)							// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_DOTPOS_Y			(0)								// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(50.0f)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(50.0f)							// ポリゴンの高さ

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
HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice);
void SetTextureEnemy(int nCntEnemy, int nPatternAnim, DIRECTION dir);
void SetVertexEnemy(int nCntEnemy);
bool CollisionCheckEB(int enemyIdx);
bool CollisionCheckEP(int enemyIdx);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;

ENEMY	g_enemy[MAX_ENEMY]; //プレイヤーのワーク

/*******************************************************************************
関数名:	HRESULT InitEnemy(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//enemyの初期化
		int randX, randY;
		do
		{
			randX = rand()%19 + 3;
			randY = rand()%10;
		}while(pMap->data[randY][randX] != BLOCK_GRASS);

		g_enemy[nCntEnemy].dotPos = D3DXVECTOR3( randX, randY, 0.0f);

		g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
		g_enemy[nCntEnemy].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_enemy[nCntEnemy].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_enemy[nCntEnemy].dir = (DIRECTION)(rand()%4);
		g_enemy[nCntEnemy].walking = false;

		GAME_MODE mode = GetGameMode();
		switch(mode)
		{
			case GAME_COOPERATE:
				g_enemy[nCntEnemy].bUse = true;	
				break;
			case GAME_BATTLE:
				g_enemy[nCntEnemy].bUse = false;	
				break;
		}

		
	}

	// アニメーションの初期化



	//頂点情報の作成
	if(FAILED(MakeVertexEnemy(pDevice)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_POLYGON,		// ファイルの名前
								&g_pTextureEnemy);	// 読み込むメモリー

	return S_OK;
}

/*******************************************************************************
関数名:	void DrawEnemy(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEnemy);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntEnemy*NUM_VERTEX,	//ロードする最初の頂点インデックス
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
void UninitEnemy(void)
{
	if(g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}
	if(g_pVtxBufferEnemy != NULL)
	{
		g_pVtxBufferEnemy->Release();
		g_pVtxBufferEnemy = NULL;
	}

	//enemyを利用しないに設定する
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_enemy[nCntEnemy].bUse = false;	
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ENEMY,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferEnemy,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferEnemy->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	
	}



	g_pVtxBufferEnemy->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdateEnemy(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdateEnemy(void)
{	
	DIRECTION new_dir;
	MAP *pMap = GetMap();

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{

			//移動位置調整
			if(abs(g_enemy[nCntEnemy].pos.x - DotPos2Pos(g_enemy[nCntEnemy].dotPos).x) < VALUE_MOVE && abs(g_enemy[nCntEnemy].pos.y - DotPos2Pos(g_enemy[nCntEnemy].dotPos).y) < VALUE_MOVE )
			{
				g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
				g_enemy[nCntEnemy].walking = false;
			}
			
			//次の位置移動したら
			if(g_enemy[nCntEnemy].pos == DotPos2Pos(g_enemy[nCntEnemy].dotPos))
			{
				g_enemy[nCntEnemy].walking = false;
			}

			if(g_enemy[nCntEnemy].walking)
			{
				switch(g_enemy[nCntEnemy].dir)
				{
					case D_UP:
						//敵と弾の当たり判定
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.y += 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.y -= g_enemy[nCntEnemy].move.y;
						break;
					case D_DOWN:
						//敵と弾の当たり判定
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.y -= 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;
						break;
					case D_LEFT:
						//敵と弾の当たり判定
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.x += 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.x -= g_enemy[nCntEnemy].move.x;
						break;
					case D_RIGHT:
						//敵と弾の当たり判定
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.x -= 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].move.x;
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
					g_enemy[nCntEnemy].dir = D_UP;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y - 1][(int)g_enemy[nCntEnemy].dotPos.x] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.y -= g_enemy[nCntEnemy].move.y;
						g_enemy[nCntEnemy].dotPos.y -= 1;		
						g_enemy[nCntEnemy].walking = true;
					}
				}
				if(new_dir == D_DOWN)
				{
					g_enemy[nCntEnemy].dir = D_DOWN;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y + 1][(int)g_enemy[nCntEnemy].dotPos.x] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;
						g_enemy[nCntEnemy].dotPos.y += 1;
						g_enemy[nCntEnemy].walking = true;
					}

				}
				if(new_dir == D_LEFT)
				{
					g_enemy[nCntEnemy].dir = D_LEFT;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y][(int)g_enemy[nCntEnemy].dotPos.x - 1] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.x -= g_enemy[nCntEnemy].move.x;
						g_enemy[nCntEnemy].dotPos.x -= 1;
						g_enemy[nCntEnemy].walking = true;
					}
				}
				if(new_dir == D_RIGHT)
				{
					g_enemy[nCntEnemy].dir = D_RIGHT;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y][(int)g_enemy[nCntEnemy].dotPos.x + 1] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].move.x;
						g_enemy[nCntEnemy].dotPos.x += 1;
						g_enemy[nCntEnemy].walking = true;
					}
				}
			}


			SetVertexEnemy(nCntEnemy);


			//向き変更
			SetTextureEnemy(nCntEnemy, g_enemy[nCntEnemy].nPatternAnim, g_enemy[nCntEnemy].dir);
	
			//アニメ表現
			g_enemy[nCntEnemy].nCounterAnim++;
			if((g_enemy[nCntEnemy].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// パターンの切り替え
				g_enemy[nCntEnemy].nPatternAnim = (g_enemy[nCntEnemy].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// テクスチャ座標を設定
				SetTextureEnemy(nCntEnemy, g_enemy[nCntEnemy].nPatternAnim, g_enemy[nCntEnemy].dir);

				//nCounterAnimのリセット
				g_enemy[nCntEnemy].nCounterAnim = 0;
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
void SetVertexEnemy(int nCntEnemy)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);
		
		pVtx += nCntEnemy * NUM_VERTEX; 

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		// 頂点データをアンロックする
		g_pVtxBufferEnemy->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetTextureEnemy(int nPatternAnim)
引数:	int nPatternAnim : アニメーションパターンNo.
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureEnemy(int nCntEnemy, int nPatternAnim, DIRECTION dir)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * NUM_VERTEX; 

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
		g_pVtxBufferEnemy->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetEnemy(D3DXVECTOR3 pos)
引数:	D3DXVECTOR3 pos：敵を設置する位置
戻り値:	なし
説明:	敵の設置
*******************************************************************************/
void SetEnemy(D3DXVECTOR3 pos)
{
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(!g_enemy[nCntEnemy].bUse)
		{
			//位置を設定
			g_enemy[nCntEnemy].pos = pos;
			SetVertexEnemy(nCntEnemy);
			g_enemy[nCntEnemy].bUse = true; //発射中に変更
			break;
		}
	}

}


/*******************************************************************************
関数名:	bool CollisionCheckEB(int enemyIdx)
引数:	int enemyIdx：何番目の敵
戻り値:	bool：当たり true、当たらない false
説明:	敵(E)と弾(B)の当たり判定
*******************************************************************************/
bool CollisionCheckEB(int enemyIdx)
{
	BULLET* pBellet = GetBullet();


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(pBellet[nCntBullet].bUse)
		{
			//バウンディングスフィア
			if( (g_enemy[enemyIdx].pos.x - pBellet[nCntBullet].pos.x)*(g_enemy[enemyIdx].pos.x - pBellet[nCntBullet].pos.x) + (g_enemy[enemyIdx].pos.y - pBellet[nCntBullet].pos.y)*(g_enemy[enemyIdx].pos.y - pBellet[nCntBullet].pos.y) < (POLYGON_SIZE_X/2 + pBellet[nCntBullet].size.x/2)*(POLYGON_SIZE_X/2+pBellet[nCntBullet].size.x/2) )
			{
				return true;
			}	
		}

	}	
	return false;
}


/*******************************************************************************
関数名:	BULLET* GetBullet(void)
引数:	なし
戻り値: BULLET*：BULLETのポインタ
説明:	当たり判定などの時、弾の情報を読み取り
*******************************************************************************/
ENEMY *GetEnemy(void)
{
	return g_enemy;
}