//******************************************************************************
//
// タイトル:		サウンド処理
// プログラム名:	sound.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_OP = 0,		// BGM op
	SOUND_LABEL_BGM_CS,			// BGM Character Select
	SOUND_LABEL_BGM_PLAY,		// BGM game playing
	SOUND_LABEL_BGM_RANK,		// BGM rank
	SOUND_LABEL_SE_BOSSHURT,	// BOSS怪我
	SOUND_LABEL_SE_COUNTDOWN,	// countdown音
	SOUND_LABEL_SE_DEATH,		// 死亡音
	SOUND_LABEL_SE_ENEMYDEATH,	// 雑魚死亡
	SOUND_LABEL_SE_EXPLOSION01,	// 爆発音
	SOUND_LABEL_SE_EXPLOSION02,	// 爆発音
	SOUND_LABEL_SE_GAMEOVER,	// GameOver
	SOUND_LABEL_SE_GETITEM,		// アイテムゲット
	SOUND_LABEL_SE_HURT,		// 怪我音
	SOUND_LABEL_SE_LOSTITEM,	// アイテム効果が消えった
	SOUND_LABEL_SE_OK,			// OK
	SOUND_LABEL_SE_POISON,		// 中毒
	SOUND_LABEL_SE_SELECT,		// 選択
	SOUND_LABEL_SE_SETBOMB,		// 爆弾設置
	SOUND_LABEL_SE_GAMESTART,	// ゲームスタート
	SOUND_LABEL_SE_SWITCH,		// 裏技のボタン　　　　　　　

	SOUND_LABEL_MAX,
} SOUND_LABEL;


typedef enum
{
	BGM_ON = 0,
	BGM_OFF,
	BGM_MODE_MAX
}BGM_MODE;

typedef enum
{
	SE_ON = 0,
	SE_OFF,
	SE_MODE_MAX
}SE_MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
BGM_MODE *GetBGM(void);
SE_MODE *GetSE(void);
void SetSE(SOUND_LABEL label);
void SwitchBGM(SOUND_LABEL label);

#endif
