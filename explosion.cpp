//******************************************************************************
//
// タイトル:		爆風処理
// プログラム名:	explosion.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "explosion.h"
#include "enemy.h"
#include "map.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include "item.h"
#include "bullet.h"
#include "player_life.h"
#include "boss.h"

#include "num_death.h"
#include "num_kill.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/explosion003.png"// 読み込むテクスチャファイル名
#define	POLYGON_POS_X				(-100)								// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_POS_Y				(-100)								// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(50)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(50)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(4)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(5)								// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(0.0f)							// ポリゴンの移動量
#define MAX_EXPLOSION				(50)							// オブジェクトの数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void SetVertexExplosion(int nCntExplo);
void SetTextureExplosion(int,int);
bool CollisionCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);


/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferExplosion = NULL;
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;

EXPLOSION g_explosion[MAX_EXPLOSION]; //プレイヤーのワーク
/*******************************************************************************
関数名:	HRESULT InitExplosion(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		//bulletの初期化
		g_explosion[nCntExplo].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_explosion[nCntExplo].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_explosion[nCntExplo].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_explosion[nCntExplo].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_explosion[nCntExplo].bUse = false;
		g_explosion[nCntExplo].who_setted = -1;

		// アニメーションの初期化
		g_explosion[nCntExplo].g_nCounterAnim = 0;
		g_explosion[nCntExplo].g_nPatternAnim = 0;		
	}



	//頂点情報の作成
	if(FAILED(MakeVertexExplosion(pDevice)))
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_POLYGON,		// ファイルの名前
								&g_pTextureExplosion);	// 読み込むメモリー

	return S_OK;
}

/*******************************************************************************
関数名:	void DrawExplosion(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		if(g_explosion[nCntExplo].bUse)
		{

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntExplo*NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);
		}	
	}



}

/*******************************************************************************
関数名:	void UninitExplosion(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの開放関数
*******************************************************************************/
void UninitExplosion(void)
{
	if(g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	if(g_pVtxBufferExplosion != NULL)
	{
		g_pVtxBufferExplosion->Release();
		g_pVtxBufferExplosion = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EXPLOSION,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferExplosion,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}



	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferExplosion->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	}



	g_pVtxBufferExplosion->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdateExplosion(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdateExplosion(void)
{
	PLAYER *pPlayer = GetPlayer();
	ITEM *pItem = GetItem();
	MAP *pMap = GetMap();
	ENEMY *pEnemy = GetEnemy();
	BOSS *pBoss = GetBoss();

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		if(g_explosion[nCntExplo].bUse)
		{
			//頂点座標の設定
			//SetVertexExplosion(nCntExplo);
			
			//爆発の当たり判定
			//敵と当たったら	
			for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				if(pEnemy[nCntEnemy].bUse)
				{
					if(CollisionCheck(pEnemy[nCntEnemy].pos, pEnemy[nCntEnemy].size, g_explosion[nCntExplo].pos,g_explosion[nCntExplo].size))
					{
						ChangeScore(100);
						pEnemy[nCntEnemy].bUse = false;

						//SE
						SetSE(SOUND_LABEL_SE_ENEMYDEATH);
					}	
				}
				
			}
			//BOSSと当たったら	
			for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
			{
				if(pBoss[nCntBoss].bUse && pBoss[nCntBoss].nCounterHurt == -1)
				{
					if(CollisionCheck(pBoss[nCntBoss].pos, pBoss[nCntBoss].size/2, g_explosion[nCntExplo].pos,g_explosion[nCntExplo].size/2))
					{
						pBoss[nCntBoss].life--;
						pBoss[nCntBoss].nCounterHurt = 60; //hurt anime 60s

						//SE
						SetSE(SOUND_LABEL_SE_BOSSHURT);
					}	
				}

			}

			//プレーヤーと当たったら
			for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{
				if(pPlayer[nCntPlayer].bUse)
				{
					if(CollisionCheck(pPlayer[nCntPlayer].pos, pPlayer[nCntPlayer].size, g_explosion[nCntExplo].pos, g_explosion[nCntExplo].size) && !pPlayer[nCntPlayer].status[STATUS_UNDEAD])
					{	
						if(pPlayer[nCntPlayer].status[STATUS_GUARD])
						{
							pPlayer[nCntPlayer].status[STATUS_UNDEAD] = true;
							pPlayer[nCntPlayer].nCounterStatus[STATUS_GUARD] = FRAME_STATUS_GUARD; //60FRAME経ったら、STATUS_GUARD消える
						}
						else
						{
							//if player died
							SetSE(SOUND_LABEL_SE_HURT);
							SetStatus(nCntPlayer, STATUS_HURT);
							pPlayer[nCntPlayer].nCounterStatus[STATUS_HURT] = FRAME_STATUS_HURT;
							pPlayer[nCntPlayer].status[STATUS_UNDEAD] = true;
							pPlayer[nCntPlayer].life--;

							GAME_MODE mode = GetGameMode();
							switch(mode)
							{
								case GAME_COOPERATE:

									ChangePlayerLife(nCntPlayer, -1);
									break;
								case GAME_BATTLE:
									
									ChangeNumDeath(nCntPlayer, 1);
									if(g_explosion[nCntExplo].who_setted == nCntPlayer) break; //自殺はkill数が変わらない
									ChangeNumKill(g_explosion[nCntExplo].who_setted, 1);
									break;
							}

							//SE
							SetSE(SOUND_LABEL_SE_DEATH);
						}

					}				
				}
			}


			g_explosion[nCntExplo].g_nCounterAnim++;
			if((g_explosion[nCntExplo].g_nCounterAnim >= TIME_CHANGE_PATTERN) )
			{

				// パターンの切り替え
				g_explosion[nCntExplo].g_nPatternAnim++;
				if(g_explosion[nCntExplo].g_nPatternAnim == NUM_ANIM_PATTERN){
					g_explosion[nCntExplo].bUse = false;
					g_explosion[nCntExplo].g_nPatternAnim = 0;
				}

				// テクスチャ座標を設定
				SetTextureExplosion(nCntExplo,g_explosion[nCntExplo].g_nPatternAnim);
			
				//g_nCounterAnimのリセット
				g_explosion[nCntExplo].g_nCounterAnim = 0;
			}

			//アイテムと当たったら
			for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
			{
				if(pItem[nCntItem].visible)
				{
					if(pItem[nCntItem].type == ITEM_GRAVE) continue;
					if(CollisionCheck(pItem[nCntItem].pos, pItem[nCntItem].size, g_explosion[nCntExplo].pos,g_explosion[nCntExplo].size) &&  pItem[nCntItem].type != ITEM_NEXT)
					{
						pItem[nCntItem].visible = false;
						pMap->item[(int)Pos2DotPos(pItem[nCntItem].pos).y][(int)Pos2DotPos(pItem[nCntItem].pos).x].type = ITEM_NONE;

						//SE
						SetSE(SOUND_LABEL_SE_LOSTITEM);
					}		
				}
			}
		}
	}

}

/*******************************************************************************
関数名:	void SetVertexExplosion(void)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexExplosion(int nCntExplo)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntExplo * NUM_VERTEX; 
		
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);	

		

		// 頂点データをアンロックする
		g_pVtxBufferExplosion->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetTextureExplosion(int nPatternAnim)
引数:	int nPatternAnim : アニメーションパターンNo.
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTextureExplosion(int nCntExplo,int nPatternAnim)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;
	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntExplo*NUM_VERTEX;

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
	g_pVtxBufferExplosion->Unlock();

}

/*******************************************************************************
関数名:	void SetExplosion(D3DXVECTOR3 pos)
引数:	D3DXVECTOR3 pos：爆発を設置する位置
戻り値:	なし
説明:	爆発の設置
*******************************************************************************/
void SetExplosion(D3DXVECTOR3 pos, int who_setted)
{
	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		if(!g_explosion[nCntExplo].bUse)
		{
			//位置を設定
			g_explosion[nCntExplo].pos = pos;
			SetVertexExplosion(nCntExplo);
			g_explosion[nCntExplo].bUse = true; //爆発中に変更
			g_explosion[nCntExplo].who_setted = who_setted;
			break;
		}	
	}

}

void SetScaleOfExplosion(D3DXVECTOR3 pos, int power, int who_setted)
{
	D3DXVECTOR3 dotPos,tPos;
	MAP *pMap = GetMap();
	ITEM *pItem = GetItem();
	PLAYER *pPlayer = GetPlayer();
	BULLET *pBullet = GetBullet();
	int dirCheck[4][2] = {
		{-1,0},
		{1,0},
		{0,-1},
		{0,1}
	};

	

	//真ん中
	SetExplosion(pos, who_setted);
	dotPos = Pos2DotPos(pos);
	pPlayer[pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].who_setted].bUseBomb--;
	pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].bUse = false; 
	pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].nCounterExpo = FRAME_BEFORE_EXPLOSION;
	pMap->item[(int)dotPos.y][(int)dotPos.x].type = ITEM_NONE;

	//上下左右
	for(int nCntDir = 0; nCntDir < 4; nCntDir++)
	{
		for(int nCnt = 1; nCnt <= power; nCnt++)
		{
			////上下左右の座標設定
			tPos.y = pos.y + 50*nCnt*dirCheck[nCntDir][0];
			tPos.x = pos.x + 50*nCnt*dirCheck[nCntDir][1];
			dotPos = Pos2DotPos(tPos);


			if(pMap->data[(int)dotPos.y][(int)dotPos.x] == BLOCK_PILLAR)
			{
				break;
			}
			else if(pMap->data[(int)dotPos.y][(int)dotPos.x] == BLOCK_WALL)
			{
				SetExplosion(tPos, who_setted);
				ChangeMap(dotPos, BLOCK_GRASS);
				if(pMap->item[(int)dotPos.y][(int)dotPos.x].type != ITEM_NONE)
				{
					pItem[ pMap->item[(int)dotPos.y][(int)dotPos.x].index ].nCounterVisible = FRAME_BEFORE_VISIBLE;
				}

				//飛べる敵
				//if(CollisionCheckEnemy(tPos))
				//{
				//	ChangeScore(100);
				//}
				break;
			}
			else
			{
				//爆弾のチェーン爆発
				if(pMap->item[(int)dotPos.y][(int)dotPos.x].type == ITEM_SETTED_BOMB)
				{
					SetScaleOfExplosion(tPos, pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].power, who_setted);
				}
				else
				{

					SetExplosion(tPos, who_setted);
				}
			}

		}	
	}
	SetSE(SOUND_LABEL_SE_EXPLOSION02);
}

/*******************************************************************************
関数名:	bool CollisionCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
引数:	D3DXVECTOR3 pos1:物１の位置、 D3DXVECTOR3 size1:物１のサイズ、 D3DXVECTOR3 pos2:物２の位置、 D3DXVECTOR3 size2:物２のサイズ
戻り値:	bool：当たり true、当たらない false
説明:	汎用な当たり判定
*******************************************************************************/
bool CollisionCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{

	if( pos2.x + size2.x/2 > pos1.x - size1.x/2 && //LEFT
		pos2.x - size2.x/2 < pos1.x + size1.x/2 && //RIGHT
		pos2.y + size2.y/2 > pos1.y - size1.y/2 && //UP
		pos2.y - size2.y/2 < pos1.y + size1.y/2 )  //DOWN
	{
		return true;
	}
	return false;
}
