//******************************************************************************
//
// タイトル:		メイン処理
// プログラム名:	main.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _MAIN_H_
#define _MAIN_H_

#define USE_SOUND		// サウンド使用の有無
//#undef USE_SOUND

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <windows.h>
#include "d3dx9.h"
#include "dinput.h"

#ifdef USE_SOUND
#include "xaudio2.h"		// XAudio2関連のヘッダー
#endif

/*******************************************************************************
* ライブラリのリンク
*******************************************************************************/
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib,"d3d9.lib")			// 描画処理に必要
#pragma comment (lib,"d3dx9.lib")			// [d3d9.lib]の拡張ライブラリ
#pragma comment (lib,"dxguid.lib")			// DirectXコンポーネント使用に必要
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"winmm.lib")
#endif

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define SCREEN_WIDTH (1250)				// ウインドウの幅
#define SCREEN_HEIGHT (750)				// ウインドウの高さ
#define WINDOW_TITLE "Vanishment This Wall"	
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) //頂点フォーマット(設定)
#define DIRECTINPUT_VERTION (0x0800)


/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex; //テクスチャ座標
}VERTEX_2D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_CHARA,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
}MODE;

typedef enum  //順番はtextureに合わせる
{
	D_DOWN = 0,
	D_LEFT,
	D_RIGHT,
	D_UP,
	D_BOMBSET,
	D_MAX
}DIRECTION;

typedef struct
{
	int DotX;
	int DotY;
}DOTPOS;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
#endif