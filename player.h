//******************************************************************************
//
// タイトル:		プレイヤー処理
// プログラム名:	player.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _PLAYER_H_
#define _PLAYER_H_

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "status.h"

/*******************************************************************************
* ライブラリのリンク
*******************************************************************************/


/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define MAX_PLAYER			(4)	
#define PLAYER_ATARI		(3)		 //プレやー当たり判定の半径
#define PLAYER_LIFE			(3)
#define PLAYER_BATTLE_LIFE	(1)

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef enum
{
	CHARACTOER0	= 0,
	CHARACTOER1,
	CHARACTOER2,
	CHARACTOER3,
	CHARACTOER4,
	CHARACTOER5,
	CHARACTOER6,
	CHARACTOER7,
	MAX_CHARACTER
}PICTURE_CHARACTOER;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	D3DXVECTOR3 dotPos;
	D3DXVECTOR3 size;
	DIRECTION dir;
	bool bUse;

	//animation
	PICTURE_CHARACTOER charaType;
	int nCounterAnim;	// ポリゴンのアニメーションカウンター
	int nPatternAnim;	// ポリゴンのアニメーションパターンNo.
	bool walking;

	//item
	int power;
	int bombNum;
	int bUseBomb;
	int life;

	//status
	bool status[STATUS_MAX];
	int statusIndex[STATUS_MAX];
	int nCounterStatus[STATUS_MAX];
}PLAYER;
/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitPlayer();
HRESULT SetPlayer(int nPlayerIdx, D3DXVECTOR3 dotPos);
void DrawPlayer(void);		
void UninitPlayer(void);
void UpdatePlayer(void);
PLAYER *GetPlayer(void);
void SetColorPlayer(int nCntPlayer, int R, int G, int B, int A);
void SetVertexPlayer(int nCntPlayer);
void SetTexturePlayer(int nCntPlayer, int nPatternAnim, DIRECTION dir);
HRESULT ResetAllPlayer(void);
HRESULT SetPlayerAnimation(int nPlayerIdx, PICTURE_CHARACTOER type);
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice);
#endif