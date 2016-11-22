//******************************************************************************
//
// タイトル:		リザルトのバトル結果表示処理
// プログラム名:	result_kdw.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include <stdio.h>
#include "player.h"
#include "result_kdw.h"
#include "num_death.h"
#include "num_kill.h"
#include "num_win.h"
#include "result_kdw_kill.h"
#include "result_kdw_death.h"
#include "result_kdw_tex.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPD3DXFONT         g_ResultFont      = NULL;

/*******************************************************************************
関数名:	HRESULT InitBG(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	背景の初期化処理
*******************************************************************************/
HRESULT InitResultKDW(void)
{
	//外部関数の初期化
	InitResultKDWKill();
	InitResultKDWDeath();
	InitResultKDWTex();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXCreateFont( pDevice , 80 ,  30, 0 , 0 , 0 , 0 , 0 , 0 , 0 , "Times New Roman" , &g_ResultFont );
	NUM_KILL *pNumKill = GetNumKill();
	NUM_DEATH *pNumDeath = GetNumDeath();

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		ChangeResultKDWKill(nCntPlayer, pNumKill[nCntPlayer].score);
		ChangeResultKDWDeath(nCntPlayer, pNumDeath[nCntPlayer].score);
	}
	

	return S_OK;
}
/*******************************************************************************
関数名:	void DrawBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの描画関数
*******************************************************************************/
void DrawResultKDW(void)
{
	DrawResultKDWKill();
	DrawResultKDWDeath();
	DrawResultKDWTex();
	//RECT rect = {100, 200, SCREEN_WIDTH, SCREEN_HEIGHT};
	//char aStr[256];
	//NUM_KILL *pNumKill = GetNumKill();
	//NUM_DEATH *pNumDeath = GetNumDeath();
	//NUM_WIN *pNumWin = GetNumWin();

	//for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++) //最大９位
	//{
	//	// 表示するテキストの準備
	//	wsprintf(&aStr[0], "%2dP: KILL %02d, DEATH %02d, WIN %1d \n",nCntPlayer+1 , pNumKill[nCntPlayer].score, pNumDeath[nCntPlayer].score, pNumWin[nCntPlayer].score);


	//	switch(nCntPlayer)
	//	{
	//		case 0:
	//			
	//			// テキスト描画
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff)); //red				
	//			break;
	//		case 1:

	//			// テキスト描画
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0x00, 0xff)); //yellow					
	//			break;
	//		case 2:

	//			// テキスト描画
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0x00, 0xff, 0x00, 0xff)); //green				
	//			break;
	//		case 3:

	//			// テキスト描画
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0x00, 0x00, 0xff, 0xff)); //blue
	//			break;
	//	}	
	//
	//	

	//	rect.top += 100;
	//}

}
/*******************************************************************************
関数名:	void UninitBG(void)
引数:	なし
戻り値:	なし
説明:	背景のポリゴンの開放関数
*******************************************************************************/
void UninitResultKDW(void)
{
	//if ( g_ResultFont != NULL )
	//{
	//	g_ResultFont->Release();
	//	g_ResultFont = NULL;
	//}
	UninitResultKDWKill();
	UninitResultKDWDeath();
	UninitResultKDWTex();
}

void UpdateResultKDW(void)
{
	UpdateResultKDWKill();
	UpdateResultKDWDeath();
	UpdateResultKDWTex();
}

