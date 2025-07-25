//******************************************************************************
//
// タイトル:		メイン処理
// プログラム名:	main.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
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
// マクロ定義
//*****************************************************************************


/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);



/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3D9			g_pD3D = NULL;       //Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL; //Device オブジェクト（描画に必要）
LPD3DXFONT g_pD3DFont = NULL;
int g_nCountFPS = 0;
MODE g_mode;

/*******************************************************************************
関数名:	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
引数:	
戻り値:	正常終了: int型の 0
説明:	メイン関数
*******************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）
	DWORD dwExecLastTime; //最終実行時間
	DWORD dwFPSLastTime;  //最終FPS計測時間
	DWORD dwCurrentTime;  //現在時間
	DWORD dwFrameCount;   //フレーム実行カウント

	MSG msg;

	// ウィンドウクラス
	WNDCLASSEX wcex = 
	{
		sizeof(WNDCLASSEX),			// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,					// 表示するウインドウのスタイルを設定
		WndProc,					// ウィンドウプロシージャのアドレスを指定
		0,							// 通常は使用しないので"0"を指定
		0,							// 通常は使用しないので"0"を指定
		hInstance,					// WinMainのパラメータのインスタンスハンドルを設定
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),// 使用するアイコンを指定
		LoadCursor(NULL,IDC_ARROW),	// マウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),	// ウインドウのクライアント領域の背景色を設定
		NULL,						// Windowにつけるメニューを設定
		"WindowClass",				// ウインドウクラスの名前
		NULL						// 小さいアイコンが設定された場合の情報を記述
	};					
									
								
	// ウィンドウクラス登録
	RegisterClassEx(&wcex);

	// ウィンドウ作成
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
	//初期化処理（ウィンドウを作成してから行う）
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	timeBeginPeriod(1);             //分解能を１ミリに設定
	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();  //システム時刻をミリ秒単位で取得
	dwCurrentTime = timeGetTime();
	dwFrameCount = 0;

	// ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
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
			dwCurrentTime = timeGetTime(); //システム時刻取得
			if((dwCurrentTime - dwFPSLastTime) >= 500) //0.5秒ごとに実行
			{
				g_nCountFPS = (dwFrameCount*1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if((dwCurrentTime - dwExecLastTime) >= (1000/60))
			{
				dwExecLastTime = dwCurrentTime; //処理した時刻を保存
				
				// DirectXの処理
				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++; //フレームカウントを加算
			}
			

		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass("WINdowClass",wcex.hInstance);

	//終了処理
	Uninit();
	timeEndPeriod(1); //分解能を戻す

	return (int)msg.wParam;
}

/*******************************************************************************
関数名:	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
引数:	
戻り値:	正常終了: int型の 0
説明:	メイン関数
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
関数名:	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
引数:	hInstance : インスタンスのハンドル
		hWnd      : ウインドウのハンドル
		bWindow   : ウインドウモードにするかどうか
戻り値:	HRESULT型	S_OK:正常終了
					E_FAIL:初期化失敗
説明:	DirectX 初期化関数
*******************************************************************************/
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート(指定できないので0固定)
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル(VSyncを待たずに描画)
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート(現在の速度に合わせる)
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル(VSyncを待って描画)
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,						// ディスプレイアダプタ
									D3DDEVTYPE_HAL,							// デバイスタイプ
									hWnd,									// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// デバイス作成制御の組み合わせ
									&d3dpp,									// デバイスのプレゼンテーションパラメータ
									&g_pD3DDevice)))						// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp,
										&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp,
											&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
	//レンダーステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);          //Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); //テクスチャU値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP); //テクスチャV値の繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);  //テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);  //テクスチャ縮小時の補間設定

	// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	//情報表示用フォントを生成
	D3DXCreateFont( g_pD3DDevice, 18, 0,0,0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DFont);

	//入力の初期化
	//InitInput(hInstance, hWnd);
	InitKeyboard(hInstance, hWnd);
	InitGamePad();

	//soundの初期化
	InitSound(hWnd);

	//Fadeの初期化
	InitFade();

	InitMessage();

	//Titleの初期化
	InitTitle();

	//GAMEの初期化
	InitGame();

	//Resultの初期化
	InitResult();

	InitCSMenu();

	SetMode(MODE_TITLE);

	return S_OK;
}

/*******************************************************************************
関数名:	void Uninit(void)
引数:	なし
戻り値:	なし
説明:	開放の関数
*******************************************************************************/
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	if(g_pD3DFont != NULL)
	{// 情報表示用フォントの開放
		g_pD3DFont->Release();
		g_pD3DFont = NULL;
	}

	//入力の開放
	UninitKeyboard();
	UninitGamePad();
	//UninitInput();


	//Gameの開放
	UninitGame();

	//Titleの開放
	UninitTitle();

	//Resultの開放
	UninitResult();

	//Soundの開放
	UninitSound();

	//Fadeの開放
	UninitFade();

	UninitMessage();

	UninitCSMenu();

}

/*******************************************************************************
関数名:	void Update(void)
引数:	なし
戻り値:	なし
説明:	ゲームデータの更新関数
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
関数名:	void Draw(void)
引数:	なし
戻り値:	なし
説明:	描画処理
*******************************************************************************/
void Draw()
{
	// バックバッファ＆Ｚバッファのクリア
	// Count   : pRects配列にある矩形の数
	// pRects  : クリアする矩形の配列(NULL指定でビューポート矩形全体をクリア)
	// Flags   : クリアするサーフェスを示すフラグ(少なくとも１つを使用しなければならない)
	//         : [D3DCLEAR_TARGET - レンダリングターゲットをクリアしてColorパラメータの値にする]
	//         : [D3DCLEAR_ZBUFFER - 深度(Ｚ)バッファをクリアしてZパラメータの値にする]
	//         : [D3DCLEAR_STENCIL - ステンシルバッファをクリアしてStencilパラメータの値にする]
	// Color   : サーフェスをクリアする色
	// Z       : デプスバッファに保存する値
	// Stencil : ステンシルバッファに保存する値(整数)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);


	// Direct3Dによる描画の開始
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
		// FPS表示
		DrawFPS();
#endif
		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}


	// バックバッファとフロントバッファの入れ替え
	// pSourceRect         : 転送元矩形
	// pDestRect           : 転送先矩形
	// hDestWindowOverride : 転送先ウインドウへのポインタ
	// pDirtyRegion        : 通常はNULLで
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	
}

/*******************************************************************************
関数名:	void DrawFPS(void)
引数:	なし
戻り値:	なし
説明:	FPS表示処理
*******************************************************************************/
void DrawFPS(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aStr[256];

	// 表示するテキストの準備
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// テキスト描画
	g_pD3DFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}

/*******************************************************************************
関数名:	LPDIRECT3DDEVICE9 GetDevice(void)
引数:	なし
戻り値:	Device オブジェクト
説明:	描画用オブジェクト　ポリゴン関数から呼び出す
*******************************************************************************/
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

/*******************************************************************************
関数名:	void SetMode(MODE mode)
引数:	なし
戻り値:	なし
説明:	ゲーム遷移の設定関数
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



