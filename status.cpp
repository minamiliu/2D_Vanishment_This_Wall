//******************************************************************************
//
// タイトル:		ステータス処理
// プログラム名:	status.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "status.h"
#include "player.h"
#include "bullet.h"
#include "map.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(-100)	//ポリゴンの左上X
#define POLYGON_POS_Y	(-400)	//ポリゴンの左上Y
#define POLYGON_SIZE_X	(50)	//ポリゴンのSIZE X
#define POLYGON_SIZE_Y	(50)	//ポリゴンのSIZE Y
#define MAX_OBJECT		(40)		// オブジェクトの数

#define POLYGON_TEXTURENAME_STATUS_GUARD		"data/TEXTURE/STATUS/status_guard.png"
#define POLYGON_TEXTURENAME_STATUS_BOMBING		"data/TEXTURE/STATUS/status_bone.png" 
#define POLYGON_TEXTURENAME_STATUS_LEFTRIGHT	"data/TEXTURE/STATUS/status_leftright.png"
#define POLYGON_TEXTURENAME_STATUS_INVISIBLE	"data/TEXTURE/STATUS/status_invisible.png"
#define POLYGON_TEXTURENAME_STATUS_BOMB			"data/TEXTURE/STATUS/status_bomb.png"
#define POLYGON_TEXTURENAME_STATUS_FIRE			"data/TEXTURE/STATUS/status_fire.png"
#define POLYGON_TEXTURENAME_STATUS_SKATE		"data/TEXTURE/STATUS/status_skate.png"
#define POLYGON_TEXTURENAME_STATUS_SCORE		"data/TEXTURE/STATUS/status_score.png"


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexStatus(LPDIRECT3DDEVICE9 pDevice);
void SetVertexStatus(int nCntStatus);
void SetColorStatus(int nCntStatus,int R, int G, int B, int A);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferStatus = NULL;
LPDIRECT3DTEXTURE9 g_pTextureStatus[STATUS_MAX] = {NULL};

STATUS_OBJECT	g_status[MAX_OBJECT];

bool change_flag;


/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitStatus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		g_status[nCntStatus].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_status[nCntStatus].size = D3DXVECTOR3(POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_status[nCntStatus].type = STATUS_NONE;
		g_status[nCntStatus].bUse = false;
		g_status[nCntStatus].who_get = -1;
	}

	change_flag = false;
	

	//頂点情報の作成
	if(FAILED(MakeVertexStatus(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_GUARD, &g_pTextureStatus[STATUS_GUARD]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_BOMBING, &g_pTextureStatus[STATUS_BOMBING]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_LEFTRIGHT, &g_pTextureStatus[STATUS_LEFTRIGHT]); 
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_INVISIBLE, &g_pTextureStatus[STATUS_INVISIBLE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_INVISIBLE, &g_pTextureStatus[STATUS_HURT]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_BOMB, &g_pTextureStatus[STATUS_ADDBOMB]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_FIRE, &g_pTextureStatus[STATUS_ADDFIRE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_SKATE, &g_pTextureStatus[STATUS_ADDSKATE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_SCORE, &g_pTextureStatus[STATUS_ADDSCORE]);

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawStatus(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferStatus, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		if(g_status[nCntStatus].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStatus[g_status[nCntStatus].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntStatus*4,			//ロードする最初の頂点インデックス
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
void UninitStatus(void)
{
	for(int nCnt = 0; nCnt < STATUS_MAX; nCnt++)
	{
		if(g_pTextureStatus[nCnt] != NULL)
		{
			g_pTextureStatus[nCnt]->Release();
			g_pTextureStatus[nCnt] = NULL;
		}
	}

	if(g_pVtxBufferStatus != NULL)
	{
		g_pVtxBufferStatus->Release();
		g_pVtxBufferStatus = NULL;
	}	


}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexStatus(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_OBJECT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferStatus,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferStatus->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++,pVtx+=4)
	{

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


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

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferStatus->Unlock();

	return S_OK;
}

/*******************************************************************************
関数名:	void SetVertexPolygon(void)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexStatus(int nCntStatus)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferStatus->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntStatus * NUM_VERTEX; 

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		
		

		// 頂点データをアンロックする
		g_pVtxBufferStatus->Unlock();
	}
}

void UpdateStatus(void)
{
	PLAYER *pPlayer = GetPlayer();
	MAP *pMap = GetMap();	

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		if(g_status[nCntStatus].bUse  )
		{
			//if player died
			if(!pPlayer[g_status[nCntStatus].who_get].bUse) g_status[nCntStatus].bUse = false;

			//Counter カウントダウン
			pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type]--;

			switch(g_status[nCntStatus].type)
			{
				case STATUS_ADDBOMB:
				case STATUS_ADDSCORE:
				case STATUS_ADDSKATE:
				case STATUS_ADDFIRE:
					
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
					g_status[nCntStatus].pos.y -= pPlayer[g_status[nCntStatus].who_get].size.y;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 0);
						}
						else
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[g_status[nCntStatus].type] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[g_status[nCntStatus].type]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
					}
					break;
				case STATUS_GUARD:

					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 5 == 0)
						{
							change_flag = !change_flag;				
						}
						if(change_flag)
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 0);
						}
						else
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
						}
						
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_GUARD] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_GUARD]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_UNDEAD] = false;
						SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
					}
					break;

				case STATUS_BOMBING:

					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
					g_status[nCntStatus].pos.y -= pPlayer[g_status[nCntStatus].who_get].size.y;

					//爆弾が勝手に設置される
					if(pPlayer[g_status[nCntStatus].who_get].bUseBomb < pPlayer[g_status[nCntStatus].who_get].bombNum && pMap->item[(int)Pos2DotPos(pPlayer[g_status[nCntStatus].who_get].pos).y][(int)Pos2DotPos(pPlayer[g_status[nCntStatus].who_get].pos).x].type == ITEM_NONE)
					{
						D3DXVECTOR3 tPos = DotPos2Pos(Pos2DotPos(pPlayer[g_status[nCntStatus].who_get].pos));
						SetBullet( g_status[nCntStatus].who_get, tPos, pPlayer[g_status[nCntStatus].who_get].power);
						pPlayer[g_status[nCntStatus].who_get].bUseBomb++;
					}

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 0, 255, 0, 255);

							//SE
							SetSE(SOUND_LABEL_SE_POISON);
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_BOMBING] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_BOMBING]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
					}
					
					break;

				case STATUS_LEFTRIGHT:
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
					g_status[nCntStatus].pos.y -= pPlayer[g_status[nCntStatus].who_get].size.y;


					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 0, 255, 0, 255);

							//SE
							SetSE(SOUND_LABEL_SE_POISON);
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_LEFTRIGHT] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_LEFTRIGHT]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
					}
					break;
				case STATUS_INVISIBLE:
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 120 == 0) //120frame ちょっと姿を現す
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 200);

							//SE
							SetSE(SOUND_LABEL_SE_POISON);
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 0);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_INVISIBLE] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_INVISIBLE]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
					}
					break;
				case STATUS_HURT:
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 0, 0, 255); //赤
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_HURT] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_HURT]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_UNDEAD] = false;
					}
					break;
			}
			//頂点座標の設定
			SetVertexStatus(nCntStatus);
		}
	}
}



void SetStatus(int nPlayerIdx, STATUS type)
{
	PLAYER *pPlayer = GetPlayer();


	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		if(!g_status[nCntStatus].bUse) 
		{
			//位置を設定			
			g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
			g_status[nCntStatus].type = type;
			g_status[nCntStatus].bUse = true;
			g_status[nCntStatus].who_get = nPlayerIdx;
			
			//player status 変更
			pPlayer[nPlayerIdx].status[type] = true;
			pPlayer[nPlayerIdx].statusIndex[type] = nCntStatus;
			break;
		}
	}
}

void ClearStatus(int nIdx)
{
	
	g_status[nIdx].bUse = false;
	g_status[nIdx].type = STATUS_NONE;
}

void SetColorStatus(int nCntStatus,int R, int G, int B, int A)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferStatus->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntStatus * NUM_VERTEX; 

		//color
		pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
		pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
		pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
		pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);
		

		// 頂点データをアンロックする
		g_pVtxBufferStatus->Unlock();
	}
}






