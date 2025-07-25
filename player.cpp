//******************************************************************************
//
// タイトル:		プレイヤー処理
// プログラム名:	player.cpp
// 作成者:			HAL東京ゲーム学科　劉南宏
//
//******************************************************************************


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "player.h"
#include "input.h"
#include "bullet.h"
#include "sound.h"
#include "map.h"
#include "item.h"
#include "enemy.h"
#include "player_life.h"
#include "game.h"
#include "CS_select.h"
#include "boss.h"

#include "num_death.h"
#include "num_win.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	POLYGON_DOTPOS_X			(1)								// ポリゴンの基準位置(Ｘ座標)
#define	POLYGON_DOTPOS_Y			(11)							// ポリゴンの基準位置(Ｙ座標)
#define	POLYGON_SIZE_X				(50)							// ポリゴンの幅
#define	POLYGON_SIZE_Y				(50)							// ポリゴンの高さ

#define	TEX_PATTERN_DIVIDE_X		(3)								// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y		(4)								// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// １パターンのテクスチャサイズ(Ｘ方向)(1.0f/X方向分割数)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// １パターンのテクスチャサイズ(Ｙ方向)(1.0f/Y方向分割数)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// アニメーションのパターン数(X方向分割数×Y方向分割数)
#define	TIME_CHANGE_PATTERN			(10)							// アニメーションの切り替わるタイミング(フレーム数)

#define	VALUE_MOVE					(3.0f)							// ポリゴンの移動量
#define BOMB_POWER					(2)
#define BOMB_NUM					(1)

#define	TEXTURE_PLAYER0		"data/TEXTURE/PLAYER/player00.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER1		"data/TEXTURE/PLAYER/player01.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER2		"data/TEXTURE/PLAYER/player02.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER3		"data/TEXTURE/PLAYER/player03.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER4		"data/TEXTURE/PLAYER/player04.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER5		"data/TEXTURE/PLAYER/player05.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER6		"data/TEXTURE/PLAYER/player06.png"// 読み込むテクスチャファイル名
#define	TEXTURE_PLAYER7		"data/TEXTURE/PLAYER/player07.png"// 読み込むテクスチャファイル名

/*******************************************************************************
* 構造体定義
*******************************************************************************/

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
bool CollisionCheckPlayerBullet(int nCntPlayer);
bool CollisionCheckPlayerEnemy(int nCntPlayer);
bool CollisionCheckPlayerBoss(int nCntPlayer);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexturePlayer[MAX_PLAYER] = {NULL};

PLAYER			g_player[MAX_PLAYER]; //プレイヤーのワーク

//Player controller setting
int keyOfPlayer[MAX_PLAYER][5] = { 
	{DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_RETURN}, //1p 下 左 右 上 attack
	{DIK_S, DIK_A, DIK_D, DIK_W, DIK_TAB},  //2p
	{DIK_J, DIK_H, DIK_K, DIK_U, DIK_SPACE}, //3p
	{DIK_NUMPAD5, DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD0}}; //4p

LPCSTR textureName[8] = {TEXTURE_PLAYER0,TEXTURE_PLAYER1,TEXTURE_PLAYER2,TEXTURE_PLAYER3,TEXTURE_PLAYER4,TEXTURE_PLAYER5,TEXTURE_PLAYER6,TEXTURE_PLAYER7};

/*******************************************************************************
関数名:	HRESULT InitPolygon(void)
引数:	なし
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	初期化処理
*******************************************************************************/
HRESULT InitPlayer()
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	bool *pPlayerList = GetPlayerList();
	PICTURE_CHARACTOER *pCharaList = GetCharacterList();

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(pPlayerList[nCntPlayer])
		{
			switch(nCntPlayer)
			{
				case 0:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 1, 1, 0.0f);
					break;
				case 1:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 23, 1, 0.0f);
					break;
				case 2:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 1, 11, 0.0f);
					break;
				case 3:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 23, 11, 0.0f);
					break;
			}


			SetPlayer(nCntPlayer, g_player[nCntPlayer].dotPos);

			// テクスチャの読み込み            ファイルの名前        読み込むメモリー
			D3DXCreateTextureFromFile(pDevice, textureName[ pCharaList[nCntPlayer] ], &g_pTexturePlayer[nCntPlayer]);

			switch(pCharaList[nCntPlayer])
			{
				case CHARACTOER0:
				case CHARACTOER1:
					g_player[nCntPlayer].bombNum++;
					break;
				case CHARACTOER2:
				case CHARACTOER3:
					g_player[nCntPlayer].move.x++;
					g_player[nCntPlayer].move.y++;
					break;
				case CHARACTOER4:
				case CHARACTOER5:
					g_player[nCntPlayer].move.x-=0.5;
					g_player[nCntPlayer].move.y-=0.5;
					g_player[nCntPlayer].life++;
					ChangePlayerLife(nCntPlayer, 1);
					break;
				case CHARACTOER6:
				case CHARACTOER7:	
					g_player[nCntPlayer].power++;
					break;
			}

			//lifeの表示を初期化、そうしないと皆ゼロを表示する
			ChangePlayerLife(nCntPlayer, 0);
		}


	}

	//頂点情報の作成
	if(FAILED(MakeVertexPlayer(pDevice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT SetPlayer(int nPlayerIdx, D3DXVECTOR3 dotPos)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//playerの初期化
	//g_player[nPlayerIdx].dotPos = dotPos;
	g_player[nPlayerIdx].pos = DotPos2Pos(dotPos);
	g_player[nPlayerIdx].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_player[nPlayerIdx].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
	g_player[nPlayerIdx].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	g_player[nPlayerIdx].dir = D_DOWN;
	g_player[nPlayerIdx].bUseBomb = 0;
	g_player[nPlayerIdx].nCounterAnim = 0;
	g_player[nPlayerIdx].nPatternAnim = 0;
	g_player[nPlayerIdx].walking = false;
	g_player[nPlayerIdx].bUse = true;
	
	//item
	g_player[nPlayerIdx].power = BOMB_POWER;
	g_player[nPlayerIdx].bombNum = BOMB_NUM;
	

	GAME_MODE mode = GetGameMode();
	switch(mode)
	{
		case GAME_COOPERATE:
			g_player[nPlayerIdx].life = PLAYER_LIFE;
			break;
		case GAME_BATTLE:
			g_player[nPlayerIdx].life = PLAYER_BATTLE_LIFE;						
			break;
	}
	
	//status
	for(int nCnt = 0; nCnt < STATUS_MAX; nCnt++)
	{
		g_player[nPlayerIdx].statusIndex[nCnt] = -1;
		g_player[nPlayerIdx].status[nCnt] = false;
		g_player[nPlayerIdx].nCounterStatus[nCnt] = -1;
	}	


	return S_OK;
}

/*******************************************************************************
関数名:	void DrawPolygon(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの描画関数
*******************************************************************************/
void DrawPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBufferPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_player[nCntPlayer].bUse)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePlayer[nCntPlayer]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntPlayer*NUM_VERTEX,	//ロードする最初の頂点インデックス
				NUM_POLYGON				//ポリゴンの数
			);			
		}

	}
}

/*******************************************************************************
関数名:	void UninitPolygon(void)
引数:	なし
戻り値:	なし
説明:	ポリゴンの開放関数
*******************************************************************************/
void UninitPlayer(void)
{
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_pTexturePlayer[nCntPlayer] != NULL)
		{
			g_pTexturePlayer[nCntPlayer]->Release();
			g_pTexturePlayer[nCntPlayer] = NULL;
		}	
	}

	if(g_pVtxBufferPlayer != NULL)
	{
		g_pVtxBufferPlayer->Release();
		g_pVtxBufferPlayer = NULL;
	}
}

/*******************************************************************************
関数名:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
引数:	LPDIRECT3DDEVICE9 pDevice : Deviceオブジェクト
戻り値:	HRESUL : 初期化結果 正常終了:S_OK
説明:	ポリゴンの頂点情報の作成関数
*******************************************************************************/
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_PLAYER,	//頂点データのバッファサイズ 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED, 
		&g_pVtxBufferPlayer,			//頂点バッファインターフェースのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferPlayer->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx+=4)
	{
		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//テクスチャ座標指定
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferPlayer->Unlock();

	return S_OK;
}


/*******************************************************************************
関数名:	void UpdatePolygon(void)
引数:	なし
戻り値:	なし
説明:	更新処理
*******************************************************************************/
void UpdatePlayer(void)
{
	MAP *pMap = GetMap();
	ENEMY *pEnemy = GetEnemy();
	
	int normalDir[4][2] = { //順番はtextureに合わせる
		{0,1}, //下
		{-1,0}, //左
		{1,0},  //右
		{0,-1}   //上
	};

	//GAME OVER
	int alive = 0;
	int who_alive = -1;
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_player[nCntPlayer].bUse)
		{
			alive ++;
			who_alive = nCntPlayer;
		}
	}

	GAME_MODE mode = GetGameMode();
	switch(mode)
	{
		case GAME_COOPERATE:
			if(alive <= 0)
			{
				SetGameStep(STEP_GAMEOVER);
				SetMessage(MSG_GAMEOVER);

				//SE
				SetSE(SOUND_LABEL_SE_GAMEOVER);
			}
			break;
		case GAME_BATTLE:
			if(alive == 1) //勝利者がある
			{
				ChangeNumWin(who_alive, 1);
				SetGameStep(STEP_CLEAR);

				switch(who_alive)
				{
				case 0:
					SetMessage(MSG_1P);
					break;
				case 1:
					SetMessage(MSG_2P);
					break;
				case 2:
					SetMessage(MSG_3P);
					break;
				case 3:
					SetMessage(MSG_4P);
					break;
				}
				SetMessage(MSG_WIN);

				//GAME SET
				int winner = GetWinner();
				if(winner != -1)
				{
					SetMessage(MSG_GAME_SET);
				}

				//SE
				SetSE(SOUND_LABEL_SE_GAMESTART);	
			}
			else if(alive == 0) //全員死んでしまった
			{
				SetGameStep(STEP_CLEAR);
				SetMessage(MSG_DRAW); 

				//SE
				SetSE(SOUND_LABEL_SE_GAMEOVER);
			}
			break;
	}



	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_player[nCntPlayer].bUse)
		{
			if(g_player[nCntPlayer].life <= 0)
			{
				g_player[nCntPlayer].bUse = false;
				SetItem( ITEM_GRAVE, g_player[nCntPlayer].dotPos);
			}

			//移動位置調整
			if(fabs(g_player[nCntPlayer].pos.x - DotPos2Pos(g_player[nCntPlayer].dotPos).x) < g_player[nCntPlayer].move.x && fabs(g_player[nCntPlayer].pos.y - DotPos2Pos(g_player[nCntPlayer].dotPos).y) < g_player[nCntPlayer].move.y )
			{
				g_player[nCntPlayer].pos = DotPos2Pos(g_player[nCntPlayer].dotPos);
				g_player[nCntPlayer].walking = false;
			}

			//プレーヤーと敵の当たり判定
			if( ( CollisionCheckPlayerBoss(nCntPlayer) || CollisionCheckPlayerEnemy(nCntPlayer) ) && !g_player[nCntPlayer].status[STATUS_UNDEAD])
			{
				if(g_player[nCntPlayer].status[STATUS_GUARD])
				{
					g_player[nCntPlayer].status[STATUS_UNDEAD] = true;
					g_player[nCntPlayer].nCounterStatus[STATUS_GUARD] = FRAME_STATUS_GUARD; //60FRAME経ったら、STATUS_GUARD消える

					//SE
					SetSE(SOUND_LABEL_SE_LOSTITEM);
				}
				else
				{
					//if player died
					SetSE(SOUND_LABEL_SE_HURT);
					SetStatus(nCntPlayer, STATUS_HURT);
					g_player[nCntPlayer].nCounterStatus[STATUS_HURT] = FRAME_STATUS_HURT;
					g_player[nCntPlayer].status[STATUS_UNDEAD] = true;
					g_player[nCntPlayer].life--;
					
					GAME_MODE mode = GetGameMode();
					switch(mode)
					{
						case GAME_COOPERATE:

							ChangePlayerLife(nCntPlayer, -1);
							break;
						case GAME_BATTLE:

							ChangeNumDeath(nCntPlayer, 1);
							break;
					}

					//SE
					SetSE(SOUND_LABEL_SE_DEATH);
				}
			}

		
			//爆弾の設置
			if( (GetKeyboardTrigger(keyOfPlayer[nCntPlayer][D_BOMBSET]) || GetGamePadTrigger( nCntPlayer, BUTTON_B) )  && !CollisionCheckPlayerEnemy(nCntPlayer))
			{
				if(g_player[nCntPlayer].bUseBomb < g_player[nCntPlayer].bombNum && pMap->item[(int)Pos2DotPos(g_player[nCntPlayer].pos).y][(int)Pos2DotPos(g_player[nCntPlayer].pos).x].type == ITEM_NONE)
				{
					D3DXVECTOR3 tPos = DotPos2Pos(Pos2DotPos(g_player[nCntPlayer].pos));
					SetBullet(nCntPlayer, tPos, g_player[nCntPlayer].power);
					g_player[nCntPlayer].bUseBomb++;
				}
			}

			if(g_player[nCntPlayer].walking)
			{
				//プレーヤーの次の位置に爆弾があったら
				if(CollisionCheckPlayerBullet(nCntPlayer)  && pMap->item[(int)g_player[nCntPlayer].dotPos.y][(int)g_player[nCntPlayer].dotPos.x].type == ITEM_SETTED_BOMB && (g_player[nCntPlayer].dotPos.x != Pos2DotPos(g_player[nCntPlayer].pos).x || g_player[nCntPlayer].dotPos.y != Pos2DotPos(g_player[nCntPlayer].pos).y) ) 
				{
					g_player[nCntPlayer].dotPos.x -= normalDir[g_player[nCntPlayer].dir][0];
					g_player[nCntPlayer].dotPos.y -= normalDir[g_player[nCntPlayer].dir][1];
					g_player[nCntPlayer].pos = DotPos2Pos(g_player[nCntPlayer].dotPos);
					g_player[nCntPlayer].walking = false;	
				}
				else //次の位置に移動する
				{
					g_player[nCntPlayer].pos.x +=  normalDir[g_player[nCntPlayer].dir][0] * g_player[nCntPlayer].move.x;
					g_player[nCntPlayer].pos.y +=  normalDir[g_player[nCntPlayer].dir][1] * g_player[nCntPlayer].move.y;
				}
			}
			else
			{
				// 移動
				bool keyPress_flag = false;
				if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_UP]) || GetGamePadPress( nCntPlayer, BUTTON_UP) )
				{
					g_player[nCntPlayer].dir = D_UP;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_DOWN; //中毒
					keyPress_flag = true;
				}
				else if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_DOWN]) || GetGamePadPress( nCntPlayer, BUTTON_DOWN) )
				{
					g_player[nCntPlayer].dir = D_DOWN;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_UP; //中毒
					keyPress_flag = true;
				}
				else if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_LEFT]) || GetGamePadPress( nCntPlayer, BUTTON_LEFT) )
				{
					g_player[nCntPlayer].dir = D_LEFT;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_RIGHT; //中毒
					keyPress_flag = true;
				}
				else if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_RIGHT]) || GetGamePadPress( nCntPlayer, BUTTON_RIGHT) )
				{
					g_player[nCntPlayer].dir = D_RIGHT;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_LEFT; //中毒
					keyPress_flag = true;
				}

				if(keyPress_flag){
					if(pMap->data[(int)g_player[nCntPlayer].dotPos.y + normalDir[g_player[nCntPlayer].dir][1] ][(int)g_player[nCntPlayer].dotPos.x + normalDir[g_player[nCntPlayer].dir][0] ] == BLOCK_GRASS)
					{
						g_player[nCntPlayer].pos.x += normalDir[g_player[nCntPlayer].dir][0] * g_player[nCntPlayer].move.x;
						g_player[nCntPlayer].pos.y += normalDir[g_player[nCntPlayer].dir][1] * g_player[nCntPlayer].move.y;
						g_player[nCntPlayer].dotPos.x += normalDir[g_player[nCntPlayer].dir][0];
						g_player[nCntPlayer].dotPos.y += normalDir[g_player[nCntPlayer].dir][1];
						g_player[nCntPlayer].walking = true;
					}
					keyPress_flag = false;
				}
			
			}
	
			// 頂点座標の設定
			SetVertexPlayer(nCntPlayer);

			//向き変更
			SetTexturePlayer(nCntPlayer, g_player[nCntPlayer].nPatternAnim, g_player[nCntPlayer].dir);
	
			//アニメ表現
			g_player[nCntPlayer].nCounterAnim++;
			if((g_player[nCntPlayer].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// パターンの切り替え
				g_player[nCntPlayer].nPatternAnim = (g_player[nCntPlayer].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// テクスチャ座標を設定
				SetTexturePlayer(nCntPlayer, g_player[nCntPlayer].nPatternAnim, g_player[nCntPlayer].dir);

				//nCounterAnimのリセット
				g_player[nCntPlayer].nCounterAnim = 0;
			}			
		}
	}



}

/*******************************************************************************
関数名:	void SetVertexPolygon(void)
引数:	なし
戻り値:	なし
説明:	頂点座標の設定
*******************************************************************************/
void SetVertexPlayer(int nCntPlayer)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferPlayer->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntPlayer * NUM_VERTEX; 

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y - (g_player[nCntPlayer].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y - (g_player[nCntPlayer].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y + (g_player[nCntPlayer].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y + (g_player[nCntPlayer].size.y/2), 0.0f);

		// 頂点データをアンロックする
		g_pVtxBufferPlayer->Unlock();
	}
}

/*******************************************************************************
関数名:	void SetTexturePolygon(int nPatternAnim)
引数:	int nPatternAnim : アニメーションパターンNo.
戻り値:	なし
説明:	テクスチャ座標の設定
*******************************************************************************/
void SetTexturePlayer(int nCntPlayer, int nPatternAnim, DIRECTION dir)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBufferPlayer->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntPlayer * NUM_VERTEX; 

		// テクスチャ座標の設定
		fPosXLeft	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X);
		fPosXRight	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X + 1);
		fPosYUp		= TEX_PATTERN_SIZE_Y * dir;
		fPosYDown	= TEX_PATTERN_SIZE_Y * (dir + 1);


		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// 頂点データをアンロックする
		g_pVtxBufferPlayer->Unlock();
	}
}

bool CollisionCheckPlayerBullet(int nCntPlayer)
{
	BULLET* pBullet = GetBullet();


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(pBullet[nCntBullet].bUse)
		{
			//バウンディングスフィア
			if( (g_player[nCntPlayer].pos.x - pBullet[nCntBullet].pos.x)*(g_player[nCntPlayer].pos.x - pBullet[nCntBullet].pos.x) + (g_player[nCntPlayer].pos.y - pBullet[nCntBullet].pos.y)*(g_player[nCntPlayer].pos.y - pBullet[nCntBullet].pos.y) < (POLYGON_SIZE_X/2 + pBullet[nCntBullet].size.x/2)*(POLYGON_SIZE_X/2+pBullet[nCntBullet].size.x/2) )
			{
				return true;
			}		
		}

	}	
	return false;
}

bool CollisionCheckPlayerEnemy(int nCntPlayer)
{

	ENEMY *pEnemy = GetEnemy();


	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(pEnemy[nCntEnemy].bUse)
		{
			//バウンディングスフィア
			if( (g_player[nCntPlayer].pos.x - pEnemy[nCntEnemy].pos.x)*(g_player[nCntPlayer].pos.x - pEnemy[nCntEnemy].pos.x) + (g_player[nCntPlayer].pos.y - pEnemy[nCntEnemy].pos.y)*(g_player[nCntPlayer].pos.y - pEnemy[nCntEnemy].pos.y) < (PLAYER_ATARI + pEnemy[nCntEnemy].size.x/2)*(PLAYER_ATARI + pEnemy[nCntEnemy].size.x/2) )
			{
				return true;
			}		
		}

	}	
	return false;
}

bool CollisionCheckPlayerBoss(int nCntPlayer)
{

	BOSS *pBoss = GetBoss();

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(pBoss[nCntEnemy].bUse)
		{
			//バウンディングスフィア
			if( (g_player[nCntPlayer].pos.x - pBoss[nCntEnemy].pos.x)*(g_player[nCntPlayer].pos.x - pBoss[nCntEnemy].pos.x) + (g_player[nCntPlayer].pos.y - pBoss[nCntEnemy].pos.y)*(g_player[nCntPlayer].pos.y - pBoss[nCntEnemy].pos.y) < (PLAYER_ATARI + pBoss[nCntEnemy].size.x/2.5)*(PLAYER_ATARI + pBoss[nCntEnemy].size.x/2.5) )
			{
				return true;
			}		
		}

	}	
	return false;
}


/*******************************************************************************
関数名:	PLAYER GetPlayer(void)
引数:	なし
戻り値: PLAYER：PLAYERの構造体
説明:	当たり判定などの時、プレーヤーの情報を読み取り
*******************************************************************************/
PLAYER *GetPlayer(void)
{
	return g_player;
}


void SetColorPlayer(int nCntPlayer, int R, int G, int B, int A)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBufferPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntPlayer * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// 頂点データをアンロックする
	g_pVtxBufferPlayer->Unlock();
	
}

HRESULT ResetAllPlayer(void)
{

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_player[nCntPlayer].bUse)
		{
			switch(nCntPlayer)
			{
				case 0:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 1, 1, 0.0f);
					break;
				case 1:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 23, 1, 0.0f);
					break;
				case 2:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 1, 11, 0.0f);
					break;
				case 3:
					g_player[nCntPlayer].dotPos = D3DXVECTOR3( 23, 11, 0.0f);
					break;
			}

			//playerの初期化
			g_player[nCntPlayer].pos = DotPos2Pos(g_player[nCntPlayer].dotPos);
			g_player[nCntPlayer].dir = D_DOWN;
			g_player[nCntPlayer].nCounterAnim = 0;
			g_player[nCntPlayer].nPatternAnim = 0;
			g_player[nCntPlayer].walking = false;
			g_player[nCntPlayer].bUseBomb = 0;

			//status
			for(int nCnt = 0; nCnt < STATUS_MAX; nCnt++)
			{
				g_player[nCntPlayer].statusIndex[nCnt] = -1;
				g_player[nCntPlayer].status[nCnt] = false;
				g_player[nCntPlayer].nCounterStatus[nCnt] = -1;
			}

			SetColorPlayer( nCntPlayer, 255, 255, 255, 255);
		}
	}


	return S_OK;
}


HRESULT SetPlayerAnimation(int nPlayerIdx, PICTURE_CHARACTOER type)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み            ファイルの名前        読み込むメモリー
	D3DXCreateTextureFromFile(pDevice, textureName[ type ], &g_pTexturePlayer[nPlayerIdx]);	

	return S_OK;
}



