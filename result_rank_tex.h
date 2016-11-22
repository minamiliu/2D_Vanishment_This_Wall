//******************************************************************************
//
// タイトル:		リザルトの協力モードのUI表示処理
// プログラム名:	result_rank_tex.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _RESULT_RANK_TEX_H_
#define _RESULT_RANK_TEX_H_

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* ライブラリのリンク
*******************************************************************************/


/*******************************************************************************
* マクロ定義
*******************************************************************************/


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	D3DCOLOR col;
	int score;

}RESULT_RANK_TEX;

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitResultRankTex();
void DrawResultRankTex(void);		
void UninitResultRankTex(void);
void UpdateResultRankTex(void);
void ChangeResultRankTex(int nRank, int value, D3DCOLOR col);
#endif