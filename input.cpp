//******************************************************************************
//
// タイトル:		入力処理
// プログラム名:	input.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX (256)
#define REPEAT_DELAY_COUNT (10)


#define DEADZONE		(2500)											// 各軸の25%を無効ゾーンとする。
#define RANGE_MAX		(1000)											// 有効範囲の最大値。
#define RANGE_MIN		(-1000)											// 有効範囲の最小値。

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

/* Game Pad  */

BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID );			// パッド検査コールバック

											

/*******************************************************************************
* グローバル変数
*******************************************************************************/

/* Keyboard */
LPDIRECTINPUT8 g_pDInput = NULL;							// DirectInputインターフェースへのポインタ
LPDIRECTINPUTDEVICE8 g_pDIDevKeyboard = NULL;				// 入力デバイス(キーボード)へのポインタ
															// 入力デバイス(マウス)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];								// キーボードの状態を受け取るワーク
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];						// キーボードの状態を受け取るワーク
BYTE g_aKeyStateRelease[NUM_KEY_MAX];						// キーボードの状態を受け取るワーク
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];						// キーボードの状態を受け取るワーク
BYTE g_aKeyStateRepeatCnt[NUM_KEY_MAX];						// キーボードのリピートカウンタ

/* Game Pad  */
static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = {NULL,NULL,NULL,NULL};	// パッドデバイス。
static DWORD	padState[GAMEPADMAX];										// パッド情報（複数対応）。
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];
static int		padCount = 0;												// 検出したパッドの数。

/*******************************************************************************
関数名:	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
引数:	
戻り値:	正常終了: int型の 0
説明:	入力処理の初期化関数
*******************************************************************************/
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if(g_pDInput == NULL)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERTION, IID_IDirectInput8, (void **)&g_pDInput, NULL);	
		
		return hr;
	}


	return S_OK;
}

/*******************************************************************************
関数名:	void UninitInput(void)
引数:	なし
戻り値:	なし
説明:	入力処理の終了処理
*******************************************************************************/
void UninitInput(void)
{
	if(g_pDInput != NULL)
	{// DirectInputオブジェクトの開放
		g_pDInput->Release();
		g_pDInput = NULL;
	}

}

/*******************************************************************************
関数名:	void UpdateInput(void)
引数:	なし
戻り値:	なし
説明:	入力処理の更新処理
*******************************************************************************/
void UpdateInput(void)
{

}


/*******************************************************************************
関数名:	HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
引数:	
戻り値:	正常終了: int型の 0
説明:	キーボードの初期化
*******************************************************************************/
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// 入力処理の初期化
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();
	
	return S_OK;
}


/*******************************************************************************
関数名:	void UninitKeyboard(void)
引数:	なし
戻り値:	なし
説明:	キーボードの終了処理
*******************************************************************************/
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		g_pDIDevKeyboard->Unacquire();// キーボードへのアクセス権を開放(入力制御終了)
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	UninitInput();
}

/*******************************************************************************
関数名:	void UpdateKeyboard(void)
引数:	なし
戻り値:	なし
説明:	キーボードの更新処理
*******************************************************************************/
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//デバイスからデータを取得
	if(SUCCEEDED(g_pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//Triggerの生成
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey] ) & aKeyState[nCntKey];
			
			//Releaseの生成
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey] ) & ~aKeyState[nCntKey];
			
			//Repeatの生成
			if(aKeyState[nCntKey])
			{
				if(g_aKeyStateRepeatCnt[nCntKey] < 20)
				{	
					g_aKeyStateRepeatCnt[nCntKey]++;
					if(g_aKeyStateRepeatCnt[nCntKey] == 1 || g_aKeyStateRepeatCnt[nCntKey] >= REPEAT_DELAY_COUNT)
					{	
						//キーを押し始めた最初のフレーム、または一定時間経過で情報ON
						g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
					}
					else
					{
						g_aKeyStateRepeat[nCntKey] = 0;
					}
				}
			}
			else{
				g_aKeyStateRepeatCnt[nCntKey] = 0; //リピートカウントのクリア
				g_aKeyStateRepeat[nCntKey] = 0; //リピートのクリア
			}

			//キープレス情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		g_pDIDevKeyboard->Acquire();
	}

}

/*******************************************************************************
関数名:	bool GetKeyboardPress(int nKey)
引数:	int: キーのマクロ定義
戻り値:	bool:押したとtrue、押してないとfalse 
説明:	入力の種類: Press判断関数
*******************************************************************************/
bool GetKeyboardPress(int nKey)
{
	return ( (g_aKeyState[nKey] & 0x80) ? true : false );
}

/*******************************************************************************
関数名:	bool GetKeyboardTrigger(int nKey)
引数:	int: キーのマクロ定義
戻り値:	bool:押したとtrue、押してないとfalse 
説明:	入力の種類: Trigger判断関数
*******************************************************************************/
bool GetKeyboardTrigger(int nKey)
{
	return ( (g_aKeyStateTrigger[nKey] & 0x80) ? true : false );
}

/*******************************************************************************
関数名:	bool GetKeyboardRelease(int nKey)
引数:	int: キーのマクロ定義
戻り値:	bool:押したとtrue、押してないとfalse 
説明:	入力の種類: Release判断関数
*******************************************************************************/
bool GetKeyboardRelease(int nKey)
{
	return ( (g_aKeyStateRelease[nKey] & 0x80) ? true : false );
}

/*******************************************************************************
関数名:	bool GetKeyboardRepeat(int nKey)
引数:	int: キーのマクロ定義
戻り値:	bool:押したとtrue、押してないとfalse 
説明:	入力の種類: Repeat判断関数
*******************************************************************************/
bool GetKeyboardRepeat(int nKey)
{
	return ( (g_aKeyStateRepeat[nKey] & 0x80) ? true : false );
}




BOOL CALLBACK SearchGamePadCallback( LPDIDEVICEINSTANCE lpddi, LPVOID )
{
	static GUID pad_discrimination[GAMEPADMAX];
	

	// 既に取得済みのデバイスだった場合はもう一回実行する
	for( int k = 0; k < padCount; k++ )
	{
		if(pad_discrimination[k] == lpddi->guidInstance)
		return DIENUM_CONTINUE;
	}


	if( FAILED(g_pDInput->CreateDevice( lpddi->guidInstance, &pGamePad[padCount], NULL )) )
	{
		return DIENUM_CONTINUE;	// 次のデバイスを列挙。
	}


	// デバイスの識別子を保存
	pad_discrimination[padCount] = lpddi->guidInstance;

	// このデバイスを使う(列挙を終了)
	padCount++;
	return DIENUM_STOP;
}


HRESULT InitGamePad( void )
{
	HRESULT		result;
	int			i;

	padCount = 0;

	for( i = 0; i < GAMEPADMAX; i++ )
	{
		// ジョイパッドを探す。
		g_pDInput->EnumDevices( DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY );	
	}

	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。
	for( i = 0; i < padCount; i++ )
	{

		// ジョイスティック用のデータ・フォーマットを設定。
		result = pGamePad[i]->SetDataFormat( &c_dfDIJoystick );
		if( FAILED(result) )
			return false; // データフォーマットの設定に失敗。

		//複数のゲームパッドはコメントしてください。
		{
		 //モードを設定（フォアグラウンド＆非排他モード）。
		 //result = pGamePad[i]->SetCooperativeLevel( NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		 //if ( FAILED(result) ) return false; // モードの設定に失敗。
		}

		// 軸の値の範囲を設定。
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)

		DIPROPRANGE				diprg;
		ZeroMemory( &diprg, sizeof(diprg) );
		diprg.diph.dwSize		= sizeof( diprg );
		diprg.diph.dwHeaderSize	= sizeof( diprg.diph );
		diprg.diph.dwHow		= DIPH_BYOFFSET;
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;

		// X軸の範囲を設定。
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty( DIPROP_RANGE, &diprg.diph );

		// Y軸の範囲を設定。
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty( DIPROP_RANGE, &diprg.diph );

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof( DIPROPDWORD );
		dipdw.diph.dwHeaderSize	= sizeof( dipdw.diph );
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;

		//X軸の無効ゾーンを設定。
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph );

		//Y軸の無効ゾーンを設定。
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph );
			
		//ジョイスティック入力制御開始。
		pGamePad[i]->Acquire();
	}
	return true;
}

void UninitGamePad( void )
{
	for( int i = 0; i < GAMEPADMAX; i++ )
	{
		if( pGamePad[i] )
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}
}

void UpdateGamePad( void )
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;


	for( i=0 ; i<padCount ; i++ )
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;		// 初期化。

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける。
		if( FAILED(result) )
		{
			result = pGamePad[i]->Acquire();
			while( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState( sizeof(DIJOYSTATE), &dijs );	// デバイス状態を読み取る。
		if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
		{
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}


		// paddata.rgdwPOV[0]に押された方向が角度×100という整数で格納されている  
		// 真上が0で時計回りに36000まで範囲がある  
		// デジタル入力方式のゲームパッドの場合、45度単位で取得してもまず問題ない  
		switch(dijs.rgdwPOV[0]){  
			case 0:  
				padState[i] |= BUTTON_UP;
				break;
			case 4500:  
				break;
			case 9000:  
				padState[i] |= BUTTON_RIGHT;
			case 13500:  
				break;
			case 18000:  
				padState[i] |= BUTTON_DOWN; 
			case 22500:  
				break;  
			case 27000:  
				padState[i] |= BUTTON_LEFT;
			case 31500:  
				break; 
		}
	
		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする。
		//* y-axis (forward)。
		//if( dijs.lY < 0 )					padState[i] |= BUTTON_UP;
		
		//* y-axis (backward)。
		//if( dijs.lY > 0 )					padState[i] |= BUTTON_DOWN;
		
		//* x-axis (left)。
		//if( dijs.lX < 0 )					padState[i] |= BUTTON_LEFT;
		
		//* x-axis (right)。
		//if( dijs.lX > 0 )					padState[i] |= BUTTON_RIGHT;
		
		
		//* Ａボタン。
		if( dijs.rgbButtons[0] & 0x80 )		padState[i] |= BUTTON_A;
		//* Ｂボタン。
		if( dijs.rgbButtons[1] & 0x80 )		padState[i] |= BUTTON_B;
		//* Ｃボタン。
		if( dijs.rgbButtons[2] & 0x80 )		padState[i] |= BUTTON_C;
		//* Ｘボタン。
		if( dijs.rgbButtons[3] & 0x80 )		padState[i] |= BUTTON_X;
		//* Ｙボタン。
		if( dijs.rgbButtons[4] & 0x80 )		padState[i] |= BUTTON_Y;
		//* Ｚボタン。
		if( dijs.rgbButtons[5] & 0x80 )		padState[i] |= BUTTON_Z;
		//* Ｌボタン。
		if( dijs.rgbButtons[6] & 0x80 )		padState[i] |= BUTTON_L;
		//* Ｒボタン
		if( dijs.rgbButtons[7] & 0x80 )		padState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if( dijs.rgbButtons[8] & 0x80 )		padState[i] |= BUTTON_START;
		//* Ｍボタン
		if( dijs.rgbButtons[9] & 0x80 )		padState[i] |= BUTTON_M;


		// Trigger設定。
		padTrigger[i] = ( (lastPadState ^ padState[i])	// 前回と違っていて。
						& padState[i] );				// しかも今ONのやつ。

		padRelease[i] = ( (lastPadState ^ padState[i]) & ~padState[i] );
	}
}

BOOL GetGamePadPress( int padNo, DWORD button )
{
	return ( button & padState[padNo] );
}

BOOL GetGamePadTrigger( int padNo, DWORD button )
{
	return( button & padTrigger[padNo] );
}

BOOL GetGamePadRelease( int padNo, DWORD button )
{
	return( button & padRelease[padNo] );
}

