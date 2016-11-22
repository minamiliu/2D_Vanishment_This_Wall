//******************************************************************************
//
// �^�C�g��:		��������
// �v���O������:	explosion.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "explosion.h"
#include "enemy.h"
#include "map.h"
#include "score.h"
#include "sound.h"
#include "player.h"
#include "item.h"
#include "bullet.h"
#include "player_life.h"
#include "boss.h"

#include "num_death.h"
#include "num_kill.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/explosion003.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(-100)								// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(-100)								// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(50)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(4)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(5)								// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define MAX_EXPLOSION				(50)							// �I�u�W�F�N�g�̐�


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void SetVertexExplosion(int nCntExplo);
void SetTextureExplosion(int,int);
bool CollisionCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2);


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferExplosion = NULL;
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;

EXPLOSION g_explosion[MAX_EXPLOSION]; //�v���C���[�̃��[�N
/*******************************************************************************
�֐���:	HRESULT InitExplosion(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
*******************************************************************************/
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		//bullet�̏�����
		g_explosion[nCntExplo].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_explosion[nCntExplo].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_explosion[nCntExplo].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_explosion[nCntExplo].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_explosion[nCntExplo].bUse = false;
		g_explosion[nCntExplo].who_setted = -1;

		// �A�j���[�V�����̏�����
		g_explosion[nCntExplo].g_nCounterAnim = 0;
		g_explosion[nCntExplo].g_nPatternAnim = 0;		
	}



	//���_���̍쐬
	if(FAILED(MakeVertexExplosion(pDevice)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,		// �t�@�C���̖��O
								&g_pTextureExplosion);	// �ǂݍ��ރ������[

	return S_OK;
}

/*******************************************************************************
�֐���:	void DrawExplosion(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		if(g_explosion[nCntExplo].bUse)
		{

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntExplo*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);
		}	
	}



}

/*******************************************************************************
�֐���:	void UninitExplosion(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̊J���֐�
*******************************************************************************/
void UninitExplosion(void)
{
	if(g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	if(g_pVtxBufferExplosion != NULL)
	{
		g_pVtxBufferExplosion->Release();
		g_pVtxBufferExplosion = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EXPLOSION,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferExplosion,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}



	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferExplosion->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	}



	g_pVtxBufferExplosion->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdateExplosion(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdateExplosion(void)
{
	PLAYER *pPlayer = GetPlayer();
	ITEM *pItem = GetItem();
	MAP *pMap = GetMap();
	ENEMY *pEnemy = GetEnemy();
	BOSS *pBoss = GetBoss();

	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		if(g_explosion[nCntExplo].bUse)
		{
			//���_���W�̐ݒ�
			//SetVertexExplosion(nCntExplo);
			
			//�����̓����蔻��
			//�G�Ɠ���������	
			for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				if(pEnemy[nCntEnemy].bUse)
				{
					if(CollisionCheck(pEnemy[nCntEnemy].pos, pEnemy[nCntEnemy].size, g_explosion[nCntExplo].pos,g_explosion[nCntExplo].size))
					{
						ChangeScore(100);
						pEnemy[nCntEnemy].bUse = false;

						//SE
						SetSE(SOUND_LABEL_SE_ENEMYDEATH);
					}	
				}
				
			}
			//BOSS�Ɠ���������	
			for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
			{
				if(pBoss[nCntBoss].bUse && pBoss[nCntBoss].nCounterHurt == -1)
				{
					if(CollisionCheck(pBoss[nCntBoss].pos, pBoss[nCntBoss].size/2, g_explosion[nCntExplo].pos,g_explosion[nCntExplo].size/2))
					{
						pBoss[nCntBoss].life--;
						pBoss[nCntBoss].nCounterHurt = 60; //hurt anime 60s

						//SE
						SetSE(SOUND_LABEL_SE_BOSSHURT);
					}	
				}

			}

			//�v���[���[�Ɠ���������
			for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{
				if(pPlayer[nCntPlayer].bUse)
				{
					if(CollisionCheck(pPlayer[nCntPlayer].pos, pPlayer[nCntPlayer].size, g_explosion[nCntExplo].pos, g_explosion[nCntExplo].size) && !pPlayer[nCntPlayer].status[STATUS_UNDEAD])
					{	
						if(pPlayer[nCntPlayer].status[STATUS_GUARD])
						{
							pPlayer[nCntPlayer].status[STATUS_UNDEAD] = true;
							pPlayer[nCntPlayer].nCounterStatus[STATUS_GUARD] = FRAME_STATUS_GUARD; //60FRAME�o������ASTATUS_GUARD������
						}
						else
						{
							//if player died
							SetSE(SOUND_LABEL_SE_HURT);
							SetStatus(nCntPlayer, STATUS_HURT);
							pPlayer[nCntPlayer].nCounterStatus[STATUS_HURT] = FRAME_STATUS_HURT;
							pPlayer[nCntPlayer].status[STATUS_UNDEAD] = true;
							pPlayer[nCntPlayer].life--;

							GAME_MODE mode = GetGameMode();
							switch(mode)
							{
								case GAME_COOPERATE:

									ChangePlayerLife(nCntPlayer, -1);
									break;
								case GAME_BATTLE:
									
									ChangeNumDeath(nCntPlayer, 1);
									if(g_explosion[nCntExplo].who_setted == nCntPlayer) break; //���E��kill�����ς��Ȃ�
									ChangeNumKill(g_explosion[nCntExplo].who_setted, 1);
									break;
							}

							//SE
							SetSE(SOUND_LABEL_SE_DEATH);
						}

					}				
				}
			}


			g_explosion[nCntExplo].g_nCounterAnim++;
			if((g_explosion[nCntExplo].g_nCounterAnim >= TIME_CHANGE_PATTERN) )
			{

				// �p�^�[���̐؂�ւ�
				g_explosion[nCntExplo].g_nPatternAnim++;
				if(g_explosion[nCntExplo].g_nPatternAnim == NUM_ANIM_PATTERN){
					g_explosion[nCntExplo].bUse = false;
					g_explosion[nCntExplo].g_nPatternAnim = 0;
				}

				// �e�N�X�`�����W��ݒ�
				SetTextureExplosion(nCntExplo,g_explosion[nCntExplo].g_nPatternAnim);
			
				//g_nCounterAnim�̃��Z�b�g
				g_explosion[nCntExplo].g_nCounterAnim = 0;
			}

			//�A�C�e���Ɠ���������
			for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
			{
				if(pItem[nCntItem].visible)
				{
					if(pItem[nCntItem].type == ITEM_GRAVE) continue;
					if(CollisionCheck(pItem[nCntItem].pos, pItem[nCntItem].size, g_explosion[nCntExplo].pos,g_explosion[nCntExplo].size) &&  pItem[nCntItem].type != ITEM_NEXT)
					{
						pItem[nCntItem].visible = false;
						pMap->item[(int)Pos2DotPos(pItem[nCntItem].pos).y][(int)Pos2DotPos(pItem[nCntItem].pos).x].type = ITEM_NONE;

						//SE
						SetSE(SOUND_LABEL_SE_LOSTITEM);
					}		
				}
			}
		}
	}

}

/*******************************************************************************
�֐���:	void SetVertexExplosion(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���_���W�̐ݒ�
*******************************************************************************/
void SetVertexExplosion(int nCntExplo)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntExplo * NUM_VERTEX; 
		
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_explosion[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_explosion[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);	

		

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferExplosion->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetTextureExplosion(int nPatternAnim)
����:	int nPatternAnim : �A�j���[�V�����p�^�[��No.
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureExplosion(int nCntExplo,int nPatternAnim)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;
	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntExplo*NUM_VERTEX;

	// �e�N�X�`�����W�̐ݒ�
	fPosXLeft	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X);
	fPosXRight	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X + 1);
	fPosYUp		= TEX_PATTERN_SIZE_Y * (nPatternAnim / TEX_PATTERN_DIVIDE_X);
	fPosYDown	= TEX_PATTERN_SIZE_Y * (nPatternAnim / TEX_PATTERN_DIVIDE_X + 1);


	pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
	pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
	pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
	pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferExplosion->Unlock();

}

/*******************************************************************************
�֐���:	void SetExplosion(D3DXVECTOR3 pos)
����:	D3DXVECTOR3 pos�F������ݒu����ʒu
�߂�l:	�Ȃ�
����:	�����̐ݒu
*******************************************************************************/
void SetExplosion(D3DXVECTOR3 pos, int who_setted)
{
	for(int nCntExplo = 0; nCntExplo < MAX_EXPLOSION; nCntExplo++)
	{
		if(!g_explosion[nCntExplo].bUse)
		{
			//�ʒu��ݒ�
			g_explosion[nCntExplo].pos = pos;
			SetVertexExplosion(nCntExplo);
			g_explosion[nCntExplo].bUse = true; //�������ɕύX
			g_explosion[nCntExplo].who_setted = who_setted;
			break;
		}	
	}

}

void SetScaleOfExplosion(D3DXVECTOR3 pos, int power, int who_setted)
{
	D3DXVECTOR3 dotPos,tPos;
	MAP *pMap = GetMap();
	ITEM *pItem = GetItem();
	PLAYER *pPlayer = GetPlayer();
	BULLET *pBullet = GetBullet();
	int dirCheck[4][2] = {
		{-1,0},
		{1,0},
		{0,-1},
		{0,1}
	};

	

	//�^��
	SetExplosion(pos, who_setted);
	dotPos = Pos2DotPos(pos);
	pPlayer[pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].who_setted].bUseBomb--;
	pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].bUse = false; 
	pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].nCounterExpo = FRAME_BEFORE_EXPLOSION;
	pMap->item[(int)dotPos.y][(int)dotPos.x].type = ITEM_NONE;

	//�㉺���E
	for(int nCntDir = 0; nCntDir < 4; nCntDir++)
	{
		for(int nCnt = 1; nCnt <= power; nCnt++)
		{
			////�㉺���E�̍��W�ݒ�
			tPos.y = pos.y + 50*nCnt*dirCheck[nCntDir][0];
			tPos.x = pos.x + 50*nCnt*dirCheck[nCntDir][1];
			dotPos = Pos2DotPos(tPos);


			if(pMap->data[(int)dotPos.y][(int)dotPos.x] == BLOCK_PILLAR)
			{
				break;
			}
			else if(pMap->data[(int)dotPos.y][(int)dotPos.x] == BLOCK_WALL)
			{
				SetExplosion(tPos, who_setted);
				ChangeMap(dotPos, BLOCK_GRASS);
				if(pMap->item[(int)dotPos.y][(int)dotPos.x].type != ITEM_NONE)
				{
					pItem[ pMap->item[(int)dotPos.y][(int)dotPos.x].index ].nCounterVisible = FRAME_BEFORE_VISIBLE;
				}

				//��ׂ�G
				//if(CollisionCheckEnemy(tPos))
				//{
				//	ChangeScore(100);
				//}
				break;
			}
			else
			{
				//���e�̃`�F�[������
				if(pMap->item[(int)dotPos.y][(int)dotPos.x].type == ITEM_SETTED_BOMB)
				{
					SetScaleOfExplosion(tPos, pBullet[pMap->item[(int)dotPos.y][(int)dotPos.x].index].power, who_setted);
				}
				else
				{

					SetExplosion(tPos, who_setted);
				}
			}

		}	
	}
	SetSE(SOUND_LABEL_SE_EXPLOSION02);
}

/*******************************************************************************
�֐���:	bool CollisionCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
����:	D3DXVECTOR3 pos1:���P�̈ʒu�A D3DXVECTOR3 size1:���P�̃T�C�Y�A D3DXVECTOR3 pos2:���Q�̈ʒu�A D3DXVECTOR3 size2:���Q�̃T�C�Y
�߂�l:	bool�F������ true�A������Ȃ� false
����:	�ėp�ȓ����蔻��
*******************************************************************************/
bool CollisionCheck(D3DXVECTOR3 pos1, D3DXVECTOR3 size1, D3DXVECTOR3 pos2, D3DXVECTOR3 size2)
{

	if( pos2.x + size2.x/2 > pos1.x - size1.x/2 && //LEFT
		pos2.x - size2.x/2 < pos1.x + size1.x/2 && //RIGHT
		pos2.y + size2.y/2 > pos1.y - size1.y/2 && //UP
		pos2.y - size2.y/2 < pos1.y + size1.y/2 )  //DOWN
	{
		return true;
	}
	return false;
}
