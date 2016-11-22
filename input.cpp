//******************************************************************************
//
// �^�C�g��:		���͏���
// �v���O������:	input.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_KEY_MAX (256)
#define REPEAT_DELAY_COUNT (10)


#define DEADZONE		(2500)											// �e����25%�𖳌��]�[���Ƃ���B
#define RANGE_MAX		(1000)											// �L���͈͂̍ő�l�B
#define RANGE_MIN		(-1000)											// �L���͈͂̍ŏ��l�B

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

/* Game Pad  */

BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID );			// �p�b�h�����R�[���o�b�N

											

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

/* Keyboard */
LPDIRECTINPUT8 g_pDInput = NULL;							// DirectInput�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDIDevKeyboard = NULL;				// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
															// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];								// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];						// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE g_aKeyStateRelease[NUM_KEY_MAX];						// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE g_aKeyStateRepeat[NUM_KEY_MAX];						// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE g_aKeyStateRepeatCnt[NUM_KEY_MAX];						// �L�[�{�[�h�̃��s�[�g�J�E���^

/* Game Pad  */
static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = {NULL,NULL,NULL,NULL};	// �p�b�h�f�o�C�X�B
static DWORD	padState[GAMEPADMAX];										// �p�b�h���i�����Ή��j�B
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];
static int		padCount = 0;												// ���o�����p�b�h�̐��B

/*******************************************************************************
�֐���:	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
����:	
�߂�l:	����I��: int�^�� 0
����:	���͏����̏������֐�
*******************************************************************************/
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if(g_pDInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERTION, IID_IDirectInput8, (void **)&g_pDInput, NULL);	
		
		return hr;
	}


	return S_OK;
}

/*******************************************************************************
�֐���:	void UninitInput(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���͏����̏I������
*******************************************************************************/
void UninitInput(void)
{
	if(g_pDInput != NULL)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pDInput->Release();
		g_pDInput = NULL;
	}

}

/*******************************************************************************
�֐���:	void UpdateInput(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���͏����̍X�V����
*******************************************************************************/
void UpdateInput(void)
{

}


/*******************************************************************************
�֐���:	HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
����:	
�߂�l:	����I��: int�^�� 0
����:	�L�[�{�[�h�̏�����
*******************************************************************************/
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// ���͏����̏�����
	hr = InitInput(hInstance, hWnd);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "DirectInput�I�u�W�F�N�g�����˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();
	
	return S_OK;
}


/*******************************************************************************
�֐���:	void UninitKeyboard(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�L�[�{�[�h�̏I������
*******************************************************************************/
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		g_pDIDevKeyboard->Unacquire();// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}

	// ���͏����̏I������
	UninitInput();
}

/*******************************************************************************
�֐���:	void UpdateKeyboard(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�L�[�{�[�h�̍X�V����
*******************************************************************************/
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//�f�o�C�X����f�[�^���擾
	if(SUCCEEDED(g_pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//Trigger�̐���
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey] ) & aKeyState[nCntKey];
			
			//Release�̐���
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey] ) & ~aKeyState[nCntKey];
			
			//Repeat�̐���
			if(aKeyState[nCntKey])
			{
				if(g_aKeyStateRepeatCnt[nCntKey] < 20)
				{	
					g_aKeyStateRepeatCnt[nCntKey]++;
					if(g_aKeyStateRepeatCnt[nCntKey] == 1 || g_aKeyStateRepeatCnt[nCntKey] >= REPEAT_DELAY_COUNT)
					{	
						//�L�[�������n�߂��ŏ��̃t���[���A�܂��͈�莞�Ԍo�߂ŏ��ON
						g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
					}
					else
					{
						g_aKeyStateRepeat[nCntKey] = 0;
					}
				}
			}
			else{
				g_aKeyStateRepeatCnt[nCntKey] = 0; //���s�[�g�J�E���g�̃N���A
				g_aKeyStateRepeat[nCntKey] = 0; //���s�[�g�̃N���A
			}

			//�L�[�v���X����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		g_pDIDevKeyboard->Acquire();
	}

}

/*******************************************************************************
�֐���:	bool GetKeyboardPress(int nKey)
����:	int: �L�[�̃}�N����`
�߂�l:	bool:��������true�A�����ĂȂ���false 
����:	���͂̎��: Press���f�֐�
*******************************************************************************/
bool GetKeyboardPress(int nKey)
{
	return ( (g_aKeyState[nKey] & 0x80) ? true : false );
}

/*******************************************************************************
�֐���:	bool GetKeyboardTrigger(int nKey)
����:	int: �L�[�̃}�N����`
�߂�l:	bool:��������true�A�����ĂȂ���false 
����:	���͂̎��: Trigger���f�֐�
*******************************************************************************/
bool GetKeyboardTrigger(int nKey)
{
	return ( (g_aKeyStateTrigger[nKey] & 0x80) ? true : false );
}

/*******************************************************************************
�֐���:	bool GetKeyboardRelease(int nKey)
����:	int: �L�[�̃}�N����`
�߂�l:	bool:��������true�A�����ĂȂ���false 
����:	���͂̎��: Release���f�֐�
*******************************************************************************/
bool GetKeyboardRelease(int nKey)
{
	return ( (g_aKeyStateRelease[nKey] & 0x80) ? true : false );
}

/*******************************************************************************
�֐���:	bool GetKeyboardRepeat(int nKey)
����:	int: �L�[�̃}�N����`
�߂�l:	bool:��������true�A�����ĂȂ���false 
����:	���͂̎��: Repeat���f�֐�
*******************************************************************************/
bool GetKeyboardRepeat(int nKey)
{
	return ( (g_aKeyStateRepeat[nKey] & 0x80) ? true : false );
}




BOOL CALLBACK SearchGamePadCallback( LPDIDEVICEINSTANCE lpddi, LPVOID )
{
	static GUID pad_discrimination[GAMEPADMAX];
	

	// ���Ɏ擾�ς݂̃f�o�C�X�������ꍇ�͂��������s����
	for( int k = 0; k < padCount; k++ )
	{
		if(pad_discrimination[k] == lpddi->guidInstance)
		return DIENUM_CONTINUE;
	}


	if( FAILED(g_pDInput->CreateDevice( lpddi->guidInstance, &pGamePad[padCount], NULL )) )
	{
		return DIENUM_CONTINUE;	// ���̃f�o�C�X��񋓁B
	}


	// �f�o�C�X�̎��ʎq��ۑ�
	pad_discrimination[padCount] = lpddi->guidInstance;

	// ���̃f�o�C�X���g��(�񋓂��I��)
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
		// �W���C�p�b�h��T���B
		g_pDInput->EnumDevices( DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY );	
	}

	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B
	for( i = 0; i < padCount; i++ )
	{

		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�B
		result = pGamePad[i]->SetDataFormat( &c_dfDIJoystick );
		if( FAILED(result) )
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s�B

		//�����̃Q�[���p�b�h�̓R�����g���Ă��������B
		{
		 //���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j�B
		 //result = pGamePad[i]->SetCooperativeLevel( NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		 //if ( FAILED(result) ) return false; // ���[�h�̐ݒ�Ɏ��s�B
		}

		// ���̒l�͈̔͂�ݒ�B
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)

		DIPROPRANGE				diprg;
		ZeroMemory( &diprg, sizeof(diprg) );
		diprg.diph.dwSize		= sizeof( diprg );
		diprg.diph.dwHeaderSize	= sizeof( diprg.diph );
		diprg.diph.dwHow		= DIPH_BYOFFSET;
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;

		// X���͈̔͂�ݒ�B
		diprg.diph.dwObj		= DIJOFS_X; 
		pGamePad[i]->SetProperty( DIPROP_RANGE, &diprg.diph );

		// Y���͈̔͂�ݒ�B
		diprg.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty( DIPROP_RANGE, &diprg.diph );

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof( DIPROPDWORD );
		dipdw.diph.dwHeaderSize	= sizeof( dipdw.diph );
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;

		//X���̖����]�[����ݒ�B
		dipdw.diph.dwObj		= DIJOFS_X;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph );

		//Y���̖����]�[����ݒ�B
		dipdw.diph.dwObj		= DIJOFS_Y;
		pGamePad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph );
			
		//�W���C�X�e�B�b�N���͐���J�n�B
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
		padState[i] = 0x00000000l;		// �������B

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������B
		if( FAILED(result) )
		{
			result = pGamePad[i]->Acquire();
			while( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState( sizeof(DIJOYSTATE), &dijs );	// �f�o�C�X��Ԃ�ǂݎ��B
		if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
		{
			result = pGamePad[i]->Acquire();
			while ( result == DIERR_INPUTLOST )
				result = pGamePad[i]->Acquire();
		}


		// paddata.rgdwPOV[0]�ɉ����ꂽ�������p�x�~100�Ƃ��������Ŋi�[����Ă���  
		// �^�オ0�Ŏ��v����36000�܂Ŕ͈͂�����  
		// �f�W�^�����͕����̃Q�[���p�b�h�̏ꍇ�A45�x�P�ʂŎ擾���Ă��܂����Ȃ�  
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
	
		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���B
		//* y-axis (forward)�B
		//if( dijs.lY < 0 )					padState[i] |= BUTTON_UP;
		
		//* y-axis (backward)�B
		//if( dijs.lY > 0 )					padState[i] |= BUTTON_DOWN;
		
		//* x-axis (left)�B
		//if( dijs.lX < 0 )					padState[i] |= BUTTON_LEFT;
		
		//* x-axis (right)�B
		//if( dijs.lX > 0 )					padState[i] |= BUTTON_RIGHT;
		
		
		//* �`�{�^���B
		if( dijs.rgbButtons[0] & 0x80 )		padState[i] |= BUTTON_A;
		//* �a�{�^���B
		if( dijs.rgbButtons[1] & 0x80 )		padState[i] |= BUTTON_B;
		//* �b�{�^���B
		if( dijs.rgbButtons[2] & 0x80 )		padState[i] |= BUTTON_C;
		//* �w�{�^���B
		if( dijs.rgbButtons[3] & 0x80 )		padState[i] |= BUTTON_X;
		//* �x�{�^���B
		if( dijs.rgbButtons[4] & 0x80 )		padState[i] |= BUTTON_Y;
		//* �y�{�^���B
		if( dijs.rgbButtons[5] & 0x80 )		padState[i] |= BUTTON_Z;
		//* �k�{�^���B
		if( dijs.rgbButtons[6] & 0x80 )		padState[i] |= BUTTON_L;
		//* �q�{�^��
		if( dijs.rgbButtons[7] & 0x80 )		padState[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if( dijs.rgbButtons[8] & 0x80 )		padState[i] |= BUTTON_START;
		//* �l�{�^��
		if( dijs.rgbButtons[9] & 0x80 )		padState[i] |= BUTTON_M;


		// Trigger�ݒ�B
		padTrigger[i] = ( (lastPadState ^ padState[i])	// �O��ƈ���Ă��āB
						& padState[i] );				// ��������ON�̂�B

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

