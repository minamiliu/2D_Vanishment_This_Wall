//******************************************************************************
//
// �^�C�g��:		�v���C���[����
// �v���O������:	player.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
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
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	POLYGON_DOTPOS_X			(1)								// �|���S���̊�ʒu(�w���W)
#define	POLYGON_DOTPOS_Y			(11)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(50)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(3)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(4)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(3.0f)							// �|���S���̈ړ���
#define BOMB_POWER					(2)
#define BOMB_NUM					(1)

#define	TEXTURE_PLAYER0		"data/TEXTURE/PLAYER/player00.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER1		"data/TEXTURE/PLAYER/player01.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER2		"data/TEXTURE/PLAYER/player02.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER3		"data/TEXTURE/PLAYER/player03.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER4		"data/TEXTURE/PLAYER/player04.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER5		"data/TEXTURE/PLAYER/player05.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER6		"data/TEXTURE/PLAYER/player06.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PLAYER7		"data/TEXTURE/PLAYER/player07.png"// �ǂݍ��ރe�N�X�`���t�@�C����

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
bool CollisionCheckPlayerBullet(int nCntPlayer);
bool CollisionCheckPlayerEnemy(int nCntPlayer);
bool CollisionCheckPlayerBoss(int nCntPlayer);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexturePlayer[MAX_PLAYER] = {NULL};

PLAYER			g_player[MAX_PLAYER]; //�v���C���[�̃��[�N

//Player controller setting
int keyOfPlayer[MAX_PLAYER][5] = { 
	{DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_RETURN}, //1p �� �� �E �� attack
	{DIK_S, DIK_A, DIK_D, DIK_W, DIK_TAB},  //2p
	{DIK_J, DIK_H, DIK_K, DIK_U, DIK_SPACE}, //3p
	{DIK_NUMPAD5, DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD0}}; //4p

LPCSTR textureName[8] = {TEXTURE_PLAYER0,TEXTURE_PLAYER1,TEXTURE_PLAYER2,TEXTURE_PLAYER3,TEXTURE_PLAYER4,TEXTURE_PLAYER5,TEXTURE_PLAYER6,TEXTURE_PLAYER7};

/*******************************************************************************
�֐���:	HRESULT InitPolygon(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
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

			// �e�N�X�`���̓ǂݍ���            �t�@�C���̖��O        �ǂݍ��ރ������[
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

			//life�̕\�����������A�������Ȃ��ƊF�[����\������
			ChangePlayerLife(nCntPlayer, 0);
		}


	}

	//���_���̍쐬
	if(FAILED(MakeVertexPlayer(pDevice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT SetPlayer(int nPlayerIdx, D3DXVECTOR3 dotPos)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//player�̏�����
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
�֐���:	void DrawPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_player[nCntPlayer].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePlayer[nCntPlayer]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntPlayer*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);			
		}

	}
}

/*******************************************************************************
�֐���:	void UninitPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̊J���֐�
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
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_PLAYER,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferPlayer,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
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

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (POLYGON_SIZE_X/2), g_player[nCntPlayer].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferPlayer->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdatePolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdatePlayer(void)
{
	MAP *pMap = GetMap();
	ENEMY *pEnemy = GetEnemy();
	
	int normalDir[4][2] = { //���Ԃ�texture�ɍ��킹��
		{0,1}, //��
		{-1,0}, //��
		{1,0},  //�E
		{0,-1}   //��
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
			if(alive == 1) //�����҂�����
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
			else if(alive == 0) //�S������ł��܂���
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

			//�ړ��ʒu����
			if(abs(g_player[nCntPlayer].pos.x - DotPos2Pos(g_player[nCntPlayer].dotPos).x) < g_player[nCntPlayer].move.x && abs(g_player[nCntPlayer].pos.y - DotPos2Pos(g_player[nCntPlayer].dotPos).y) < g_player[nCntPlayer].move.y )
			{
				g_player[nCntPlayer].pos = DotPos2Pos(g_player[nCntPlayer].dotPos);
				g_player[nCntPlayer].walking = false;
			}

			//�v���[���[�ƓG�̓����蔻��
			if( ( CollisionCheckPlayerBoss(nCntPlayer) || CollisionCheckPlayerEnemy(nCntPlayer) ) && !g_player[nCntPlayer].status[STATUS_UNDEAD])
			{
				if(g_player[nCntPlayer].status[STATUS_GUARD])
				{
					g_player[nCntPlayer].status[STATUS_UNDEAD] = true;
					g_player[nCntPlayer].nCounterStatus[STATUS_GUARD] = FRAME_STATUS_GUARD; //60FRAME�o������ASTATUS_GUARD������

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

		
			//���e�̐ݒu
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
				//�v���[���[�̎��̈ʒu�ɔ��e����������
				if(CollisionCheckPlayerBullet(nCntPlayer)  && pMap->item[(int)g_player[nCntPlayer].dotPos.y][(int)g_player[nCntPlayer].dotPos.x].type == ITEM_SETTED_BOMB && (g_player[nCntPlayer].dotPos.x != Pos2DotPos(g_player[nCntPlayer].pos).x || g_player[nCntPlayer].dotPos.y != Pos2DotPos(g_player[nCntPlayer].pos).y) ) 
				{
					g_player[nCntPlayer].dotPos.x -= normalDir[g_player[nCntPlayer].dir][0];
					g_player[nCntPlayer].dotPos.y -= normalDir[g_player[nCntPlayer].dir][1];
					g_player[nCntPlayer].pos = DotPos2Pos(g_player[nCntPlayer].dotPos);
					g_player[nCntPlayer].walking = false;	
				}
				else //���̈ʒu�Ɉړ�����
				{
					g_player[nCntPlayer].pos.x +=  normalDir[g_player[nCntPlayer].dir][0] * g_player[nCntPlayer].move.x;
					g_player[nCntPlayer].pos.y +=  normalDir[g_player[nCntPlayer].dir][1] * g_player[nCntPlayer].move.y;
				}
			}
			else
			{
				// �ړ�
				bool keyPress_flag = false;
				if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_UP]) || GetGamePadPress( nCntPlayer, BUTTON_UP) )
				{
					g_player[nCntPlayer].dir = D_UP;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_DOWN; //����
					keyPress_flag = true;
				}
				else if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_DOWN]) || GetGamePadPress( nCntPlayer, BUTTON_DOWN) )
				{
					g_player[nCntPlayer].dir = D_DOWN;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_UP; //����
					keyPress_flag = true;
				}
				else if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_LEFT]) || GetGamePadPress( nCntPlayer, BUTTON_LEFT) )
				{
					g_player[nCntPlayer].dir = D_LEFT;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_RIGHT; //����
					keyPress_flag = true;
				}
				else if(GetKeyboardRepeat(keyOfPlayer[nCntPlayer][D_RIGHT]) || GetGamePadPress( nCntPlayer, BUTTON_RIGHT) )
				{
					g_player[nCntPlayer].dir = D_RIGHT;
					if(g_player[nCntPlayer].status[STATUS_LEFTRIGHT]) g_player[nCntPlayer].dir = D_LEFT; //����
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
	
			// ���_���W�̐ݒ�
			SetVertexPlayer(nCntPlayer);

			//�����ύX
			SetTexturePlayer(nCntPlayer, g_player[nCntPlayer].nPatternAnim, g_player[nCntPlayer].dir);
	
			//�A�j���\��
			g_player[nCntPlayer].nCounterAnim++;
			if((g_player[nCntPlayer].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_player[nCntPlayer].nPatternAnim = (g_player[nCntPlayer].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// �e�N�X�`�����W��ݒ�
				SetTexturePlayer(nCntPlayer, g_player[nCntPlayer].nPatternAnim, g_player[nCntPlayer].dir);

				//nCounterAnim�̃��Z�b�g
				g_player[nCntPlayer].nCounterAnim = 0;
			}			
		}
	}



}

/*******************************************************************************
�֐���:	void SetVertexPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���_���W�̐ݒ�
*******************************************************************************/
void SetVertexPlayer(int nCntPlayer)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferPlayer->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntPlayer * NUM_VERTEX; 

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y - (g_player[nCntPlayer].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y - (g_player[nCntPlayer].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x - (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y + (g_player[nCntPlayer].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_player[nCntPlayer].pos.x + (g_player[nCntPlayer].size.x/2), g_player[nCntPlayer].pos.y + (g_player[nCntPlayer].size.y/2), 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferPlayer->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetTexturePolygon(int nPatternAnim)
����:	int nPatternAnim : �A�j���[�V�����p�^�[��No.
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTexturePlayer(int nCntPlayer, int nPatternAnim, DIRECTION dir)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferPlayer->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntPlayer * NUM_VERTEX; 

		// �e�N�X�`�����W�̐ݒ�
		fPosXLeft	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X);
		fPosXRight	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X + 1);
		fPosYUp		= TEX_PATTERN_SIZE_Y * dir;
		fPosYDown	= TEX_PATTERN_SIZE_Y * (dir + 1);


		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// ���_�f�[�^���A�����b�N����
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
			//�o�E���f�B���O�X�t�B�A
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
			//�o�E���f�B���O�X�t�B�A
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
			//�o�E���f�B���O�X�t�B�A
			if( (g_player[nCntPlayer].pos.x - pBoss[nCntEnemy].pos.x)*(g_player[nCntPlayer].pos.x - pBoss[nCntEnemy].pos.x) + (g_player[nCntPlayer].pos.y - pBoss[nCntEnemy].pos.y)*(g_player[nCntPlayer].pos.y - pBoss[nCntEnemy].pos.y) < (PLAYER_ATARI + pBoss[nCntEnemy].size.x/2.5)*(PLAYER_ATARI + pBoss[nCntEnemy].size.x/2.5) )
			{
				return true;
			}		
		}

	}	
	return false;
}


/*******************************************************************************
�֐���:	PLAYER GetPlayer(void)
����:	�Ȃ�
�߂�l: PLAYER�FPLAYER�̍\����
����:	�����蔻��Ȃǂ̎��A�v���[���[�̏���ǂݎ��
*******************************************************************************/
PLAYER *GetPlayer(void)
{
	return g_player;
}


void SetColorPlayer(int nCntPlayer, int R, int G, int B, int A)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntPlayer * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// ���_�f�[�^���A�����b�N����
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

			//player�̏�����
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

	// �e�N�X�`���̓ǂݍ���            �t�@�C���̖��O        �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice, textureName[ type ], &g_pTexturePlayer[nPlayerIdx]);	

	return S_OK;
}



