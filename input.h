//******************************************************************************
//
// タイトル:		入力処理
// プログラム名:	input.h
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


/* Game Pad  */
#define BUTTON_UP		0x00000001L	
#define BUTTON_DOWN		0x00000002L	
#define BUTTON_LEFT		0x00000004L	
#define BUTTON_RIGHT	0x00000008L	
#define BUTTON_A		0x00000010L	
#define BUTTON_B		0x00000020L	
#define BUTTON_C		0x00000040L	
#define BUTTON_X		0x00000080L	
#define BUTTON_Y		0x00000100L	
#define BUTTON_Z		0x00000200L	
#define BUTTON_L		0x00000400L	
#define BUTTON_R		0x00000800L	
#define BUTTON_START	0x00001000L	
#define BUTTON_M		0x00002000L	
#define GAMEPADMAX		4			

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
HRESULT InitGamePad( void );											

void UninitInput(void);
void UninitKeyboard(void);
void UninitGamePad( void );

void UpdateInput(void);
void UpdateKeyboard(void);
void UpdateGamePad( void );

bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);

BOOL GetGamePadPress(int padNo,DWORD button);
BOOL GetGamePadTrigger(int padNo,DWORD button);
BOOL GetGamePadRelease( int padNo,DWORD button );

#endif