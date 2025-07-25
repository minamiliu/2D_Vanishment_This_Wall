//******************************************************************************
//
// タイトル:		キャラ選択の選ぶ処理
// プログラム名:	CS_select.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "input.h"
#include "CS_select.h"
#include "fade.h"
#include "player.h"
#include "CS_menu.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_PLAYER	(4)
#define MAX_OBJECT	(MAX_PLAYER * MAX_ICON_TYPE + 1)

#define POLYGON_TEXTURENAME_CS_1P			"data/TEXTURE/CHARACTER SELECT/CS_1P.png"
#define POLYGON_TEXTURENAME_CS_2P			"data/TEXTURE/CHARACTER SELECT/CS_2P.png"
#define POLYGON_TEXTURENAME_CS_3P			"data/TEXTURE/CHARACTER SELECT/CS_3P.png"
#define POLYGON_TEXTURENAME_CS_4P			"data/TEXTURE/CHARACTER SELECT/CS_4P.png"
#define POLYGON_TEXTURENAME_CS_PRESSFIRE	"data/TEXTURE/CHARACTER SELECT/CS_PressFire.png"
#define POLYGON_TEXTURENAME_CS_READY		"data/TEXTURE/CHARACTER SELECT/CS_Ready.png"
#define POLYGON_TEXTURENAME_CS_ALLREADY		"data/TEXTURE/CHARACTER SELECT/CS_AreYouReady.png"

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexCS_Select(LPDIRECT3DDEVICE9 pDevice);
void SetVertexPlayerIcon(int nPlayerIdx, ICON_TYPE type);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCS_Select = NULL;
LPDIRECT3DTEXTURE9 g_pTextureCS_Select[MAX_OBJECT] = {NULL};

CS_ICON	g_playerIcon[MAX_PLAYER][MAX_ICON_TYPE];
D3DXVECTOR3 g_iconPos[4][2];
DOTPOS g_selectSquare[MAX_PLAYER];

CS_ICON g_allReadyIcon;

bool g_playerList[MAX_PLAYER];
PICTURE_CHARACTOER g_charaList[MAX_PLAYER];
bool g_charaSetted;

/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitCS_Select(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//八つのCharacter Icon位置
	g_iconPos[0][0] = D3DXVECTOR3( 490, 200, 0.0f);
	g_iconPos[1][0] = D3DXVECTOR3( 490, 330, 0.0f);
	g_iconPos[2][0] = D3DXVECTOR3( 490, 460, 0.0f);
	g_iconPos[3][0] = D3DXVECTOR3( 490, 590, 0.0f);
	g_iconPos[0][1] = D3DXVECTOR3( 625, 200, 0.0f);
	g_iconPos[1][1] = D3DXVECTOR3( 625, 330, 0.0f);
	g_iconPos[2][1] = D3DXVECTOR3( 625, 460, 0.0f);
	g_iconPos[3][1] = D3DXVECTOR3( 625, 590, 0.0f);


	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for(int nCntIcon = 0; nCntIcon < MAX_ICON_TYPE; nCntIcon++)
		{
			if(nCntIcon == PRESSFIRE)
			{
				g_playerIcon[nCntPlayer][nCntIcon].bUse = true;
			}
			else
			{
				g_playerIcon[nCntPlayer][nCntIcon].bUse = false;
			}
		}
		g_playerIcon[nCntPlayer][SELECT].size = D3DXVECTOR3( 110, 110, 0.0f);
		g_playerIcon[nCntPlayer][PRESSFIRE].size = D3DXVECTOR3( 300, 300, 0.0f);
		g_playerIcon[nCntPlayer][READY].size = D3DXVECTOR3( 300, 300, 0.0f);
		g_playerIcon[nCntPlayer][JOINCOLOR].size = D3DXVECTOR3( 300, 300, 0.0f);

		g_selectSquare[nCntPlayer].DotX = 0;
		g_selectSquare[nCntPlayer].DotY = 0;

		g_playerList[nCntPlayer] = false;
		g_charaList[nCntPlayer] = CHARACTOER0;
	}

	{//1P
		//array0:select
		g_playerIcon[0][SELECT].pos = D3DXVECTOR3( 490, 200, 0.0f);
		g_playerIcon[0][SELECT].type = PICTURE_1P;

		//array1:press fire
		g_playerIcon[0][PRESSFIRE].pos = D3DXVECTOR3( 50, 50, 0.0f);
		g_playerIcon[0][PRESSFIRE].type = PICTURE_PRESSFIRE;

		//array2:ready
		g_playerIcon[0][READY].pos = D3DXVECTOR3( 50, 50, 0.0f);
		g_playerIcon[0][READY].type = PICTURE_READY;

		//array3:joincolor
		g_playerIcon[0][JOINCOLOR].pos = D3DXVECTOR3( 50, 50, 0.0f);
		g_playerIcon[0][JOINCOLOR].type = PICTURE_1P;
	}

	{//2P
		//array0:select
		g_playerIcon[1][SELECT].pos = D3DXVECTOR3( 490, 200, 0.0f);
		g_playerIcon[1][SELECT].type = PICTURE_2P;

		//array1:press fire
		g_playerIcon[1][PRESSFIRE].pos = D3DXVECTOR3( 900, 50, 0.0f);
		g_playerIcon[1][PRESSFIRE].type = PICTURE_PRESSFIRE;

		//array2:ready
		g_playerIcon[1][READY].pos = D3DXVECTOR3( 900, 50, 0.0f);
		g_playerIcon[1][READY].type = PICTURE_READY;

		//array3:joincolor
		g_playerIcon[1][JOINCOLOR].pos = D3DXVECTOR3( 900, 50, 0.0f);
		g_playerIcon[1][JOINCOLOR].type = PICTURE_2P;
	}

	{//3P
		//array0:select
		g_playerIcon[2][SELECT].pos = D3DXVECTOR3( 490, 200, 0.0f);
		g_playerIcon[2][SELECT].type = PICTURE_3P;

		//array1:press fire
		g_playerIcon[2][PRESSFIRE].pos = D3DXVECTOR3( 50, 400, 0.0f);
		g_playerIcon[2][PRESSFIRE].type = PICTURE_PRESSFIRE;

		//array2:ready
		g_playerIcon[2][READY].pos = D3DXVECTOR3( 50, 400, 0.0f);
		g_playerIcon[2][READY].type = PICTURE_READY;

		//array3:joincolor
		g_playerIcon[2][JOINCOLOR].pos = D3DXVECTOR3( 50, 400, 0.0f);
		g_playerIcon[2][JOINCOLOR].type = PICTURE_3P;
	}

	{//4P
		//array0:select
		g_playerIcon[3][SELECT].pos = D3DXVECTOR3( 490, 200, 0.0f);
		g_playerIcon[3][SELECT].type = PICTURE_4P;

		//array1:press fire
		g_playerIcon[3][PRESSFIRE].pos = D3DXVECTOR3( 900, 400, 0.0f);
		g_playerIcon[3][PRESSFIRE].type = PICTURE_PRESSFIRE;

		//array2:ready
		g_playerIcon[3][READY].pos = D3DXVECTOR3( 900, 400, 0.0f);
		g_playerIcon[3][READY].type = PICTURE_READY;

		//array3:joincolor
		g_playerIcon[3][JOINCOLOR].pos = D3DXVECTOR3( 900, 400, 0.0f);
		g_playerIcon[3][JOINCOLOR].type = PICTURE_4P;
	}

	//all ready
	g_allReadyIcon.bUse = false;
	g_allReadyIcon.pos = D3DXVECTOR3( 625, 375, 0.0f);
	g_allReadyIcon.size = D3DXVECTOR3( 400, 200, 0.0f);
	g_allReadyIcon.type = PICTURE_ALLREADY;

	//頂点情報の作成
	if(FAILED(MakeVertexCS_Select(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_1P		, &g_pTextureCS_Select[PICTURE_1P]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_2P		, &g_pTextureCS_Select[PICTURE_2P]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_3P		, &g_pTextureCS_Select[PICTURE_3P]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_4P		, &g_pTextureCS_Select[PICTURE_4P]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_PRESSFIRE	, &g_pTextureCS_Select[PICTURE_PRESSFIRE]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_READY		, &g_pTextureCS_Select[PICTURE_READY]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_ALLREADY	, &g_pTextureCS_Select[PICTURE_ALLREADY]);

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawCS_Select(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferCS_Select, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for(int nCntIcon = 0; nCntIcon < MAX_ICON_TYPE; nCntIcon++)
		{
			if(g_playerIcon[nCntPlayer][nCntIcon].bUse)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureCS_Select[g_playerIcon[nCntPlayer][nCntIcon].type]);

				//ポリゴンの描画
				pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP,	//プリミティブの種類
					(nCntPlayer*MAX_ICON_TYPE + nCntIcon)*4,			//ロードする最初の頂点インデックス
					NUM_POLYGON				//ポリゴンの数
				);			
			}
		}
	}
	
	// all ready
	if(g_allReadyIcon.bUse)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCS_Select[PICTURE_ALLREADY]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			(MAX_OBJECT- 1)*4,			//ロードする最初の頂点インデックス
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
void UninitCS_Select(void)
{
	for(int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if(g_pTextureCS_Select[nCntObj] != NULL)
		{
			g_pTextureCS_Select[nCntObj]->Release();
			g_pTextureCS_Select[nCntObj] = NULL;
		}
	}

	if(g_pVtxBufferCS_Select != NULL)
	{
		g_pVtxBufferCS_Select->Release();
		g_pVtxBufferCS_Select = NULL;
	}	


}
/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景のポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexCS_Select(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_OBJECT,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferCS_Select,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferCS_Select->Lock( 0, 0, (void**)&pVtx, 0);


	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for(int nCntIcon = 0; nCntIcon < MAX_ICON_TYPE; nCntIcon++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_playerIcon[nCntPlayer][nCntIcon].pos.x, g_playerIcon[nCntPlayer][nCntIcon].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_playerIcon[nCntPlayer][nCntIcon].pos.x + g_playerIcon[nCntPlayer][nCntIcon].size.x, g_playerIcon[nCntPlayer][nCntIcon].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_playerIcon[nCntPlayer][nCntIcon].pos.x, g_playerIcon[nCntPlayer][nCntIcon].pos.y + g_playerIcon[nCntPlayer][nCntIcon].size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_playerIcon[nCntPlayer][nCntIcon].pos.x + g_playerIcon[nCntPlayer][nCntIcon].size.x, g_playerIcon[nCntPlayer][nCntIcon].pos.y + g_playerIcon[nCntPlayer][nCntIcon].size.y, 0.0f);


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

			pVtx += 4;
		}
	}

	//all ready icon
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_allReadyIcon.pos.x - (g_allReadyIcon.size.x/2), g_allReadyIcon.pos.y - (g_allReadyIcon.size.y/2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_allReadyIcon.pos.x + (g_allReadyIcon.size.x/2), g_allReadyIcon.pos.y - (g_allReadyIcon.size.y/2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_allReadyIcon.pos.x - (g_allReadyIcon.size.x/2), g_allReadyIcon.pos.y + (g_allReadyIcon.size.y/2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_allReadyIcon.pos.x + (g_allReadyIcon.size.x/2), g_allReadyIcon.pos.y + (g_allReadyIcon.size.y/2), 0.0f);


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



	g_pVtxBufferCS_Select->Unlock();

	return S_OK;
}

void UpdateCS_Select(void)
{

	//Player controller setting
	int keyOfPlayer[MAX_PLAYER][5] = { 
		{DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_RETURN}, // 下 左 右 上 attack
		{DIK_S, DIK_A, DIK_D, DIK_W, DIK_TAB},
		{DIK_J, DIK_H, DIK_K, DIK_U, DIK_SPACE},
		{DIK_NUMPAD5, DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD0}};


	//go to game
	if(g_allReadyIcon.bUse)
	{
		for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if( (GetKeyboardTrigger( keyOfPlayer[nCntPlayer][D_BOMBSET] ) || GetGamePadTrigger( nCntPlayer, BUTTON_B) ) && g_playerIcon[nCntPlayer][READY].bUse )
			{
				AllPlayerLock();
				FadeOutToNext(MODE_GAME);

				//SE
				SetSE(SOUND_LABEL_SE_GAMESTART);
				return;
			}
		}	
	}

	
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		//wait for join
		if(!g_playerIcon[nCntPlayer][SELECT].bUse)
		{
			if(GetKeyboardTrigger( keyOfPlayer[nCntPlayer][D_BOMBSET] ) || GetGamePadTrigger( nCntPlayer, BUTTON_B))
			{
				g_playerIcon[nCntPlayer][SELECT].bUse = true;
				g_playerIcon[nCntPlayer][PRESSFIRE].bUse = false;
				g_playerIcon[nCntPlayer][JOINCOLOR].bUse = true;

				//SE
				SetSE(SOUND_LABEL_SE_SELECT);
			}		
		}
		else //selecting
		{

			//ready
			if(GetKeyboardTrigger( keyOfPlayer[nCntPlayer][D_BOMBSET] ) || GetGamePadTrigger( nCntPlayer, BUTTON_B))
			{
				g_playerIcon[nCntPlayer][READY].bUse = true;
				//SE
				SetSE(SOUND_LABEL_SE_OK);
			}
			
			//if not ready
			if(!g_playerIcon[nCntPlayer][READY].bUse)
			{
				//selecting
				if(GetKeyboardTrigger( keyOfPlayer[nCntPlayer][D_UP] ) || GetGamePadTrigger( nCntPlayer, BUTTON_UP))
				{
					g_selectSquare[nCntPlayer].DotY = (g_selectSquare[nCntPlayer].DotY + 3) % 4;

					//SE
					SetSE(SOUND_LABEL_SE_SELECT);
				}		
				if(GetKeyboardTrigger( keyOfPlayer[nCntPlayer][D_DOWN] ) || GetGamePadTrigger( nCntPlayer, BUTTON_DOWN))
				{
					g_selectSquare[nCntPlayer].DotY = (g_selectSquare[nCntPlayer].DotY + 1) % 4;
					
					//SE
					SetSE(SOUND_LABEL_SE_SELECT);
				}	
				if(GetKeyboardTrigger( keyOfPlayer[nCntPlayer][D_LEFT] ) || GetGamePadTrigger( nCntPlayer, BUTTON_LEFT))
				{
					g_selectSquare[nCntPlayer].DotX = (g_selectSquare[nCntPlayer].DotX + 1) % 2;

					//SE
					SetSE(SOUND_LABEL_SE_SELECT);
				}
				if(GetKeyboardTrigger( keyOfPlayer[nCntPlayer][D_RIGHT] ) || GetGamePadTrigger( nCntPlayer, BUTTON_RIGHT))
				{
					g_selectSquare[nCntPlayer].DotX = (g_selectSquare[nCntPlayer].DotX + 1) % 2;

					//SE
					SetSE(SOUND_LABEL_SE_SELECT);
				}
				g_playerIcon[nCntPlayer][SELECT].pos = g_iconPos[g_selectSquare[nCntPlayer].DotY][g_selectSquare[nCntPlayer].DotX];
				SetVertexPlayerIcon(nCntPlayer, SELECT);	

			}

		}

	}
	// all player OK
	bool allReadyFlag = true;
	int nPlayerJoin = 0;
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_playerIcon[nCntPlayer][SELECT].bUse != g_playerIcon[nCntPlayer][READY].bUse )
		{
			allReadyFlag = false;
		}
		if(!g_playerIcon[nCntPlayer][PRESSFIRE].bUse)
		{
			nPlayerJoin++;
		}
	}
	GAME_MODE mode = GetGameMode();
	switch(mode)
	{
		case GAME_COOPERATE:

			if(allReadyFlag && nPlayerJoin >= 1)
			{
				g_allReadyIcon.bUse = true;
			}
			else
			{
				g_allReadyIcon.bUse = false;
			}
			break;
		case GAME_BATTLE:

			if(allReadyFlag && nPlayerJoin >= 2)
			{
				g_allReadyIcon.bUse = true;
			}
			else
			{
				g_allReadyIcon.bUse = false;
			}
			break;
	}


}

/*******************************************************************************
関数名:	void SetVertexPolygon(void)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexPlayerIcon(int nPlayerIdx, ICON_TYPE type)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferCS_Select->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nPlayerIdx*MAX_ICON_TYPE + type) * NUM_VERTEX; 

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_playerIcon[nPlayerIdx][type].pos.x, g_playerIcon[nPlayerIdx][type].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_playerIcon[nPlayerIdx][type].pos.x + g_playerIcon[nPlayerIdx][type].size.x, g_playerIcon[nPlayerIdx][type].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_playerIcon[nPlayerIdx][type].pos.x, g_playerIcon[nPlayerIdx][type].pos.y + g_playerIcon[nPlayerIdx][type].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_playerIcon[nPlayerIdx][type].pos.x + g_playerIcon[nPlayerIdx][type].size.x, g_playerIcon[nPlayerIdx][type].pos.y + g_playerIcon[nPlayerIdx][type].size.y, 0.0f);

		// 頂点データをアンロックする
		g_pVtxBufferCS_Select->Unlock();
	}
}

PICTURE_CHARACTOER *GetCharacterList(void)
{
	for(int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if(g_playerIcon[nCnt][SELECT].bUse)
		{
			g_charaList[nCnt] = (PICTURE_CHARACTOER)(g_selectSquare[nCnt].DotX + g_selectSquare[nCnt].DotY * 2 );
		}
	}
	return g_charaList;
}

bool *GetPlayerList(void)
{	
	for(int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if(g_playerIcon[nCnt][READY].bUse)
		{
			g_playerList[nCnt] = true;
		}
	}
	return g_playerList;
}


bool GetPlayerJoinData(int nPlayerIdx)
{
	return g_playerIcon[nPlayerIdx][SELECT].bUse;	
}

