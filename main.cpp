//******************************************************************************
//
// �^�C�g��:		���C������
// �v���O������:	main.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "message.h"
#include "CS_menu.h"

#include "resource.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);



/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3D9			g_pD3D = NULL;       //Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL; //Device �I�u�W�F�N�g�i�`��ɕK�v�j
LPD3DXFONT g_pD3DFont = NULL;
int g_nCountFPS = 0;
MODE g_mode;

/*******************************************************************************
�֐���:	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
����:	
�߂�l:	����I��: int�^�� 0
����:	���C���֐�
*******************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	DWORD dwExecLastTime; //�ŏI���s����
	DWORD dwFPSLastTime;  //�ŏIFPS�v������
	DWORD dwCurrentTime;  //���ݎ���
	DWORD dwFrameCount;   //�t���[�����s�J�E���g

	MSG msg;

	// �E�B���h�E�N���X
	WNDCLASSEX wcex = 
	{
		sizeof(WNDCLASSEX),			// WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,					// �\������E�C���h�E�̃X�^�C����ݒ�
		WndProc,					// �E�B���h�E�v���V�[�W���̃A�h���X���w��
		0,							// �ʏ�͎g�p���Ȃ��̂�"0"���w��
		0,							// �ʏ�͎g�p���Ȃ��̂�"0"���w��
		hInstance,					// WinMain�̃p�����[�^�̃C���X�^���X�n���h����ݒ�
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),// �g�p����A�C�R�����w��
		LoadCursor(NULL,IDC_ARROW),	// �}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),	// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,						// Window�ɂ��郁�j���[��ݒ�
		"WindowClass",				// �E�C���h�E�N���X�̖��O
		NULL						// �������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	};					
									
								
	// �E�B���h�E�N���X�o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�쐬
	HWND hWnd = CreateWindow(
		"WindowClass",
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME)*2,
		SCREEN_HEIGHT + GetSystemMetrics(SM_CYDLGFRAME),
		NULL,
		NULL,
		hInstance,
		NULL
	);
	//�����������i�E�B���h�E���쐬���Ă���s���j
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	timeBeginPeriod(1);             //����\���P�~���ɐݒ�
	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();  //�V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = timeGetTime();
	dwFrameCount = 0;

	// �E�B���h�E�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime(); //�V�X�e�������擾
			if((dwCurrentTime - dwFPSLastTime) >= 500) //0.5�b���ƂɎ��s
			{
				g_nCountFPS = (dwFrameCount*1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if((dwCurrentTime - dwExecLastTime) >= (1000/60))
			{
				dwExecLastTime = dwCurrentTime; //��������������ۑ�
				
				// DirectX�̏���
				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++; //�t���[���J�E���g�����Z
			}
			

		}
	}
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass("WINdowClass",wcex.hInstance);

	//�I������
	Uninit();
	timeEndPeriod(1); //����\��߂�

	return (int)msg.wParam;
}

/*******************************************************************************
�֐���:	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
����:	
�߂�l:	����I��: int�^�� 0
����:	���C���֐�
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	int id ;
	switch( message )
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			
			id = MessageBox(hWnd,"Are you sure?","Do you want to quit the game?",MB_YESNO);
			if(id == IDYES){
				DestroyWindow(hWnd);
			}

		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*******************************************************************************
�֐���:	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
����:	hInstance : �C���X�^���X�̃n���h��
		hWnd      : �E�C���h�E�̃n���h��
		bWindow   : �E�C���h�E���[�h�ɂ��邩�ǂ���
�߂�l:	HRESULT�^	S_OK:����I��
					E_FAIL:���������s
����:	DirectX �������֐�
*******************************************************************************/
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g(�w��ł��Ȃ��̂�0�Œ�)
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��(VSync��҂����ɕ`��)
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��(VSync��҂��ĕ`��)
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,						// �f�B�X�v���C�A�_�v�^
									D3DDEVTYPE_HAL,							// �f�o�C�X�^�C�v
									hWnd,									// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// �f�o�C�X�쐬����̑g�ݍ��킹
									&d3dpp,									// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
									&g_pD3DDevice)))						// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp,
										&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp,
											&g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}
	//�����_�[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);          //Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); //�e�N�X�`��U�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP); //�e�N�X�`��V�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  //�e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  //�e�N�X�`���k�����̕�Ԑݒ�

	// �e�N�X�`���X�e�[�W���Z��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	//���\���p�t�H���g�𐶐�
	D3DXCreateFont( g_pD3DDevice, 18, 0,0,0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DFont);

	//���͂̏�����
	//InitInput(hInstance, hWnd);
	InitKeyboard(hInstance, hWnd);
	InitGamePad();

	//sound�̏�����
	InitSound(hWnd);

	//Fade�̏�����
	InitFade();

	InitMessage();

	//Title�̏�����
	InitTitle();

	//GAME�̏�����
	InitGame();

	//Result�̏�����
	InitResult();

	InitCSMenu();

	SetMode(MODE_TITLE);

	return S_OK;
}

/*******************************************************************************
�֐���:	void Uninit(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�J���̊֐�
*******************************************************************************/
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	if(g_pD3DFont != NULL)
	{// ���\���p�t�H���g�̊J��
		g_pD3DFont->Release();
		g_pD3DFont = NULL;
	}

	//���͂̊J��
	UninitKeyboard();
	UninitGamePad();
	//UninitInput();


	//Game�̊J��
	UninitGame();

	//Title�̊J��
	UninitTitle();

	//Result�̊J��
	UninitResult();

	//Sound�̊J��
	UninitSound();

	//Fade�̊J��
	UninitFade();

	UninitMessage();

	UninitCSMenu();

}

/*******************************************************************************
�֐���:	void Update(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���f�[�^�̍X�V�֐�
*******************************************************************************/
void Update(void)
{
	//UpdateInput();
	UpdateKeyboard();
	UpdateGamePad();

	UpdateFade();
	switch(g_mode)
	{
		case MODE_TITLE:
			UpdateTitle();
			break;
		case MODE_CHARA:
			UpdateCSMenu();
			break;
		case MODE_GAME:
			UpdateGame();
			break;
		case MODE_RESULT:
			UpdateResult();
			break;
	}

}

/*******************************************************************************
�֐���:	void Draw(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�`�揈��
*******************************************************************************/
void Draw()
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	// Count   : pRects�z��ɂ����`�̐�
	// pRects  : �N���A�����`�̔z��(NULL�w��Ńr���[�|�[�g��`�S�̂��N���A)
	// Flags   : �N���A����T�[�t�F�X�������t���O(���Ȃ��Ƃ��P���g�p���Ȃ���΂Ȃ�Ȃ�)
	//         : [D3DCLEAR_TARGET - �����_�����O�^�[�Q�b�g���N���A����Color�p�����[�^�̒l�ɂ���]
	//         : [D3DCLEAR_ZBUFFER - �[�x(�y)�o�b�t�@���N���A����Z�p�����[�^�̒l�ɂ���]
	//         : [D3DCLEAR_STENCIL - �X�e���V���o�b�t�@���N���A����Stencil�p�����[�^�̒l�ɂ���]
	// Color   : �T�[�t�F�X���N���A����F
	// Z       : �f�v�X�o�b�t�@�ɕۑ�����l
	// Stencil : �X�e���V���o�b�t�@�ɕۑ�����l(����)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);


	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch(g_mode)
		{
			case MODE_TITLE:
				DrawTitle();
				break;
			case MODE_CHARA:
				DrawCSMenu();
				break;
			case MODE_GAME:
				DrawGame();
				break;
			case MODE_RESULT:
				DrawResult();
				break;
		}
		DrawMessage();
		DrawFade();


#ifdef _DEBUG
		// FPS�\��
		DrawFPS();
#endif
		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}


	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	// pSourceRect         : �]������`
	// pDestRect           : �]�����`
	// hDestWindowOverride : �]����E�C���h�E�ւ̃|�C���^
	// pDirtyRegion        : �ʏ��NULL��
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	
}

/*******************************************************************************
�֐���:	void DrawFPS(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	FPS�\������
*******************************************************************************/
void DrawFPS(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aStr[256];

	// �\������e�L�X�g�̏���
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// �e�L�X�g�`��
	g_pD3DFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}

/*******************************************************************************
�֐���:	LPDIRECT3DDEVICE9 GetDevice(void)
����:	�Ȃ�
�߂�l:	Device �I�u�W�F�N�g
����:	�`��p�I�u�W�F�N�g�@�|���S���֐�����Ăяo��
*******************************************************************************/
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

/*******************************************************************************
�֐���:	void SetMode(MODE mode)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���J�ڂ̐ݒ�֐�
*******************************************************************************/
void SetMode(MODE mode)
{
	g_mode = mode;

	switch(g_mode)
	{
		case MODE_TITLE:
			StopSound();
			PlaySound(SOUND_LABEL_BGM_OP);
			UninitResult();
			InitTitle();
			break;
		case MODE_CHARA:
			StopSound(SOUND_LABEL_BGM_OP);
			PlaySound(SOUND_LABEL_BGM_CS);
			UninitTitle();
			InitCSMenu();
			break;
		case MODE_GAME:
			StopSound(SOUND_LABEL_BGM_CS);
			PlaySound(SOUND_LABEL_BGM_PLAY);
			UninitCSMenu();
			InitGame();
			break;
		case MODE_RESULT:
			StopSound();
			PlaySound(SOUND_LABEL_BGM_RANK);
			UninitGame();
			UninitTitle();
			InitResult();
			break;
	}
}



