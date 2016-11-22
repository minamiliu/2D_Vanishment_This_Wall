//******************************************************************************
//
// タイトル:		リザルトの協力モードのランキング表示処理
// プログラム名:	result_rank.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include <stdio.h>
#include "result_rank.h"
#include "result_rank_tex.h"
#include "result_rank_nth.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_SIZE_X	(100)//ポリゴンのSIZE X
#define POLYGON_SIZE_Y	(100)//ポリゴンのSIZE Y
#define POLYGON_POS_X	(50 + POLYGON_SIZE_X/2)//ポリゴンの左上X
#define POLYGON_POS_Y	(180 + POLYGON_SIZE_Y/2)//ポリゴンの左上Y

#define POLYGON_TEXTURENAME "data/TEXTURE/RESULT/CROWN2.png"

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
void ScoreFileReadWrite(void);
void BubbleSort(int data[], int size);
HRESULT MakeVertexResultPlayer(LPDIRECT3DDEVICE9 pDevice);
void SetVertexResultRank(int nRankIdx);
/*******************************************************************************
* グローバル変数
*******************************************************************************/

LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultPlayer = NULL;

RESULT_RANK_PLAYER g_resultPlayer[MAX_RANK];

int rank[MAX_RANK];
int g_playerRank;

const int key = 0x45bf6c7d; //任意の暗号化キー
float g_angle2;

/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitResultRank(void)
{
	//外部関数の初期化
	InitResultRankNth();
	InitResultRankTex();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{		
		g_resultPlayer[nCntRank].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_resultPlayer[nCntRank].bUse = false;
		g_resultPlayer[nCntRank].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

	}
	for(int nCntRank = 0; nCntRank < MAX_RANK/2; nCntRank++)
	{
		g_resultPlayer[nCntRank].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y + nCntRank*110, 0.0f);  //左5
		g_resultPlayer[nCntRank + MAX_RANK/2].pos = D3DXVECTOR3( POLYGON_POS_X + 550, POLYGON_POS_Y + nCntRank*110, 0.0f); //右5

		//switch(nCntRank)
		//{
		//	case 0:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,0,0,255); //red
		//		break;
		//	case 1:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,128,0,255); //orange
		//		break;
		//	case 2:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		//		break;
		//	case 3:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(0,255,0,255); //green
		//		break;
		//	case 4:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(0,0,255,255); //blue
		//		break;
		//}	
	}
	g_resultPlayer[MAX_RANK-1].pos = D3DXVECTOR3( -1000, -1000 , 0.0f);

	//初期化
	g_angle2 = 0;

	//頂点情報の作成
	if(FAILED(MakeVertexResultPlayer(pDevice)))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME, &g_pTextureResultPlayer);

	//SCORE FILE 処理
	ScoreFileReadWrite();

	//rankの表示
	for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if(nCnt == g_playerRank)
		{
			g_resultPlayer[nCnt].bUse = true;
		}
		ChangeResultRankTex(nCnt, rank[nCnt], D3DCOLOR_RGBA(255,255,255,255) );
		ChangeResultRankNth(nCnt, nCnt+1, D3DCOLOR_RGBA(255,255,255,255) );
	}

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawResultRank(void)
{
	//外部関数の呼び出す
	DrawResultRankTex();
	DrawResultRankNth();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferResultPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultPlayer);

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if(g_resultPlayer[nCntRank].bUse)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntRank*NUM_VERTEX,	//ロードする最初の頂点インデックス
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
void UninitResultRank(void)
{
	//外部関数の呼び出す
	UninitResultRankTex();
	UninitResultRankNth();

	if(g_pTextureResultPlayer != NULL)
	{
		g_pTextureResultPlayer->Release();
		g_pTextureResultPlayer = NULL;
	}
	if(g_pVtxBufferResultPlayer != NULL)
	{
		g_pVtxBufferResultPlayer->Release();
		g_pVtxBufferResultPlayer = NULL;
	}
}

void UpdateResultRank()
{
	//外部関数の呼び出す
	UpdateResultRankTex();
	UpdateResultRankNth();


	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if(g_resultPlayer[nCntRank].bUse)
		{
			//change size
			g_resultPlayer[nCntRank].size.x += sin(g_angle2)*0.5f;
			g_resultPlayer[nCntRank].size.y += sin(g_angle2)*0.5f;
			g_angle2 += 0.1f;
			if(g_angle2 >= 6.28) g_angle2 = 0;

			SetVertexResultRank(nCntRank);	
		}	
	}


}
HRESULT MakeVertexResultPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_RANK,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferResultPlayer,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferResultPlayer->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++, pVtx += NUM_VERTEX)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x - (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x + (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x - (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x + (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_resultPlayer[nCntRank].col;
		pVtx[1].col = g_resultPlayer[nCntRank].col;
		pVtx[2].col = g_resultPlayer[nCntRank].col;
		pVtx[3].col = g_resultPlayer[nCntRank].col;

		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);	
	}



	g_pVtxBufferResultPlayer->Unlock();

	return S_OK;
}



void SetVertexResultRank(int nRankIdx)
{

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferResultPlayer->Lock( 0, 0, (void**)&pVtx, 0);

	pVtx += nRankIdx * NUM_VERTEX; 

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x - (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y - (g_resultPlayer[nRankIdx].size.y/2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x + (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y - (g_resultPlayer[nRankIdx].size.y/2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x - (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y + (g_resultPlayer[nRankIdx].size.y/2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x + (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y + (g_resultPlayer[nRankIdx].size.y/2), 0.0f);

	// 頂点データをアンロックする
	g_pVtxBufferResultPlayer->Unlock();
}

void BubbleSort(int data[], int size)
{
	int temp;
	for(int i = 0; i <size; i++)
	{
		for(int j = size - 1; j > i ;j--)
		{
			if(data[j-1] < data[j])
			{
				temp = data[j-1];
				data[j-1] = data[j];
				data[j] = temp;
			}
		}
	}
}

void ScoreFileReadWrite(void)
{
	FILE *pRank;

	//ファイルの読み込み
	pRank = fopen( "data/FILE/ranking.bin","rb" );

	if(pRank == NULL) //エラー
	{
		pRank = fopen( "data/FILE/ranking.bin","wb" );
		for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			rank[nCnt] = 0;
		}	
	}
	else //正常
	{
		fread(rank,sizeof(int),MAX_RANK,pRank);
		//復号
		for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			rank[nCnt] ^= key;
		}
	}
	fclose(pRank);


	//今回の点数
	rank[MAX_RANK-1] = GetScore()->score;
	
	//ソートする
	BubbleSort(rank,MAX_RANK);
	
	//プレヤーの点数を指す
	for(int nCnt = MAX_RANK-1; nCnt >= 0; nCnt--)
	{
		if(GetScore()->score == rank[nCnt])
		{
			g_playerRank = nCnt;
			break;
		}
	}
	GetScore()->score = 0;

	//暗号化
	for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		rank[nCnt] ^= key;
	}
	//ファイルの書き込み
	pRank = fopen( "data/FILE/ranking.bin","wb" );
	fwrite(rank,sizeof(int),MAX_RANK,pRank);
	fclose(pRank);
	//暗号化
	for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		rank[nCnt] ^= key;
	}
}
