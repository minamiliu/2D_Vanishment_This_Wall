//******************************************************************************
//
// タイトル:		キャラ選択のキャラクター表示処理
// プログラム名:	CS_CharaShow.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "CS_CharaShow.h"
#include "player.h"
#include "CS_select.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define	TEX_PATTERN_DIVIDE_X		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define MAX_CHARA_INTRO	(8)

#define POLYGON_TEXTURENAME_CS_INTRO00		"data/TEXTURE/PLAYER/CS_introduce00.png"
#define POLYGON_TEXTURENAME_CS_INTRO01		"data/TEXTURE/PLAYER/CS_introduce01.png"
#define POLYGON_TEXTURENAME_CS_INTRO02		"data/TEXTURE/PLAYER/CS_introduce02.png"
#define POLYGON_TEXTURENAME_CS_INTRO03		"data/TEXTURE/PLAYER/CS_introduce03.png"
#define POLYGON_TEXTURENAME_CS_INTRO04		"data/TEXTURE/PLAYER/CS_introduce04.png"
#define POLYGON_TEXTURENAME_CS_INTRO05		"data/TEXTURE/PLAYER/CS_introduce05.png"
#define POLYGON_TEXTURENAME_CS_INTRO06		"data/TEXTURE/PLAYER/CS_introduce06.png"
#define POLYGON_TEXTURENAME_CS_INTRO07		"data/TEXTURE/PLAYER/CS_introduce07.png"


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT MakeVertexCS_Intro(LPDIRECT3DDEVICE9 pDevice);
void SetVertexCharaIntro(int nCharaIdx);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCS_Intro = NULL;
LPDIRECT3DTEXTURE9 g_pTextureCS_Intro[MAX_CHARA_INTRO] = {NULL};

CS_INTRO g_charaIntro[MAX_PLAYER];


PICTURE_CHARACTOER *OldcharaList = NULL;

/*******************************************************************************
関数名:	void InitTitle(void)
引数:	なし
戻り値:	なし
説明:	ゲームタイトルの初期化処理
*******************************************************************************/
HRESULT InitCS_CharaShow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	PLAYER *pPlayer = GetPlayer();

	pPlayer[0].pos = D3DXVECTOR3(  200, 150, 0.0);
	pPlayer[1].pos = D3DXVECTOR3( 1050, 150, 0.0);
	pPlayer[2].pos = D3DXVECTOR3(  200, 500, 0.0);
	pPlayer[3].pos = D3DXVECTOR3( 1050, 500, 0.0);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		pPlayer[nCntPlayer].bUse = false;
		pPlayer[nCntPlayer].size = D3DXVECTOR3(50, 50, 0.0);
	}

	//頂点情報の作成
	if(FAILED(MakeVertexPlayer(pDevice)))
	{
		return E_FAIL;
	}

	//////chara introduce////////////

	for(int nCntChara = 0; nCntChara < MAX_PLAYER; nCntChara++)
	{
		g_charaIntro[nCntChara].bUse = false;
		g_charaIntro[nCntChara].size = D3DXVECTOR3( 300, 300, 0.0f);
	}
	g_charaIntro[0].pos = D3DXVECTOR3( 200, 200, 0.0f);
	g_charaIntro[1].pos = D3DXVECTOR3( 1050, 200, 0.0f);
	g_charaIntro[2].pos = D3DXVECTOR3( 200, 550, 0.0f);
	g_charaIntro[3].pos = D3DXVECTOR3( 1050, 550, 0.0f);

	//頂点情報の作成

	if(FAILED(MakeVertexCS_Intro(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO00	, &g_pTextureCS_Intro[CHARACTOER0]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO01	, &g_pTextureCS_Intro[CHARACTOER1]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO02	, &g_pTextureCS_Intro[CHARACTOER2]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO03	, &g_pTextureCS_Intro[CHARACTOER3]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO04	, &g_pTextureCS_Intro[CHARACTOER4]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO05	, &g_pTextureCS_Intro[CHARACTOER5]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO06	, &g_pTextureCS_Intro[CHARACTOER6]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO07	, &g_pTextureCS_Intro[CHARACTOER7]);

	return S_OK;
}

/*******************************************************************************
関数名:	void UpdateTitle(void)
引数:	なし
戻り値:	なし
説明:	ゲームタイトルの更新関数
*******************************************************************************/
void Update_CharaShow(void)
{
	PLAYER *pPlayer = GetPlayer();
	PICTURE_CHARACTOER *charaList = GetCharacterList();

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(GetPlayerJoinData(nCntPlayer))
		{
			pPlayer[nCntPlayer].bUse = true;
			SetPlayerAnimation(nCntPlayer, charaList[nCntPlayer] );
			g_charaIntro[nCntPlayer].bUse = true;
			g_charaIntro[nCntPlayer].type = charaList[nCntPlayer];
		}
		if(pPlayer[nCntPlayer].bUse){

			if(OldcharaList[nCntPlayer] != charaList[nCntPlayer])
			{
				SetPlayerAnimation(nCntPlayer, charaList[nCntPlayer] );
				g_charaIntro[nCntPlayer].type = charaList[nCntPlayer];
			}


			// 頂点座標の設定
			SetVertexPlayer(nCntPlayer);

			//向き変更
			SetTexturePlayer(nCntPlayer, pPlayer[nCntPlayer].nPatternAnim, D_DOWN);
	
			//アニメ表現
			pPlayer[nCntPlayer].nCounterAnim++;
			if((pPlayer[nCntPlayer].nCounterAnim % 10 ) == 0) //10frame change pattern
			{
				// パターンの切り替え
				pPlayer[nCntPlayer].nPatternAnim = (pPlayer[nCntPlayer].nPatternAnim + 1) % 3; //3 pattern 

				// テクスチャ座標を設定
				SetTexturePlayer(nCntPlayer, pPlayer[nCntPlayer].nPatternAnim, D_DOWN);

				//nCounterAnimのリセット
				pPlayer[nCntPlayer].nCounterAnim = 0;
			}	
		}
	}
	
	OldcharaList = charaList;
}

/*******************************************************************************
関数名:	void DrawTitle(void)
引数:	なし
戻り値:	なし
説明:	ゲームタイトルの描画関数
*******************************************************************************/
void DrawCS_CharaShow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferCS_Intro, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntChara = 0; nCntChara < MAX_PLAYER; nCntChara++)
	{
		if(g_charaIntro[nCntChara].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCS_Intro[ g_charaIntro[nCntChara].type ]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntChara*4,			//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);			
		}
	}


	//character animation
	DrawPlayer();
}
/*******************************************************************************
関数名:	void UninitTitle(void)
引数:	なし
戻り値:	なし
説明:	ゲームタイトルの開放関数
*******************************************************************************/
void Uninit_CharaShow(void)
{
	UninitPlayer();

	for(int nCntChara = 0; nCntChara < MAX_CHARA_INTRO; nCntChara++)
	{
		if(g_pTextureCS_Intro[nCntChara] != NULL)
		{
			g_pTextureCS_Intro[nCntChara]->Release();
			g_pTextureCS_Intro[nCntChara] = NULL;
		}
	}

	if(g_pVtxBufferCS_Intro != NULL)
	{
		g_pVtxBufferCS_Intro->Release();
		g_pVtxBufferCS_Intro = NULL;
	}	
}

HRESULT MakeVertexCS_Intro(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_PLAYER,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferCS_Intro,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferCS_Intro->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntChara = 0; nCntChara < MAX_PLAYER; nCntChara++, pVtx+=4)
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
		pVtx[0].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x - (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y - (g_charaIntro[nCntChara].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x + (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y - (g_charaIntro[nCntChara].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x - (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y + (g_charaIntro[nCntChara].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x + (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y + (g_charaIntro[nCntChara].size.y/2), 0.0f);


		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferCS_Intro->Unlock();

	return S_OK;
}

/*******************************************************************************
関数名:	void SetVertexPolygon(void)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexCharaIntro(int nCharaIdx)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferCS_Intro->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCharaIdx * NUM_VERTEX; 

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x - (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y - (g_charaIntro[nCharaIdx].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x + (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y - (g_charaIntro[nCharaIdx].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x - (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y + (g_charaIntro[nCharaIdx].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x + (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y + (g_charaIntro[nCharaIdx].size.y/2), 0.0f);

		// 頂点データをアンロックする
		g_pVtxBufferCS_Intro->Unlock();
	}
}
