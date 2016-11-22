//******************************************************************************
//
// �^�C�g��:		���̓��[�h�̃{�X����
// �v���O������:	boss.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "boss.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "map.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/boss0001.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_DOTPOS_X			(0)								// �|���S���̊�ʒu(�w���W)
#define	POLYGON_DOTPOS_Y			(0)								// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(400.0f)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(400.0f)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(3)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(4)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(1.0f)							// �|���S���̈ړ���



/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexBoss(LPDIRECT3DDEVICE9 pDevice);
void SetTextureBoss(int nCntBoss, int nPatternAnim, DIRECTION dir);
void SetVertexBoss(int nCntBoss);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBoss = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBoss = NULL;

BOSS	g_boss[MAX_BOSS]; //�v���C���[�̃��[�N
bool hurt_flag;

/*******************************************************************************
�֐���:	HRESULT InitEnemy(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
*******************************************************************************/
HRESULT InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		//enemy�̏�����
		int randX, randY;
		do
		{
			randX = rand()%5 + 10;
			randY = rand()%5 + 5;
		}while(pMap->data[randY][randX] != BLOCK_GRASS);

		g_boss[nCntBoss].dotPos = D3DXVECTOR3( randX, randY, 0.0f);

		g_boss[nCntBoss].pos = DotPos2Pos(g_boss[nCntBoss].dotPos);
		g_boss[nCntBoss].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_boss[nCntBoss].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_boss[nCntBoss].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_boss[nCntBoss].bUse = false;	
		g_boss[nCntBoss].dir = D_DOWN;
		g_boss[nCntBoss].walking = false;
		g_boss[nCntBoss].life = 15;
		g_boss[nCntBoss].nCounterHurt = -1;
		g_boss[nCntBoss].nCounterDeath = -1;
	}
	//BOSS stage
	int stage = GetStage();
	if(stage == 2) //stage 3
	{ 
		g_boss[0].bUse = true;
	}

	// �A�j���[�V�����̏�����
	hurt_flag = false;


	//���_���̍쐬
	if(FAILED(MakeVertexBoss(pDevice)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,		// �t�@�C���̖��O
								&g_pTextureBoss);	// �ǂݍ��ރ������[

	return S_OK;
}

/*******************************************************************************
�֐���:	void DrawEnemy(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferBoss, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBoss);

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(g_boss[nCntBoss].bUse)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBoss*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);
		}	
	}



}

/*******************************************************************************
�֐���:	void UninitEnemy(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̊J���֐�
*******************************************************************************/
void UninitBoss(void)
{
	if(g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}
	if(g_pVtxBufferBoss != NULL)
	{
		g_pVtxBufferBoss->Release();
		g_pVtxBufferBoss = NULL;
	}

	//BOSS�𗘗p���Ȃ��ɐݒ肷��
	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_boss[nCntBoss].bUse = false;	

	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexBoss(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BOSS,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBoss,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferBoss->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	
	}



	g_pVtxBufferBoss->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdateEnemy(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdateBoss(void)
{	
	DIRECTION new_dir;
	MAP *pMap = GetMap();


	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(g_boss[nCntBoss].bUse)
		{

			//�ړ��ʒu����
			if(abs(g_boss[nCntBoss].pos.x - DotPos2Pos(g_boss[nCntBoss].dotPos).x) < VALUE_MOVE && abs(g_boss[nCntBoss].pos.y - DotPos2Pos(g_boss[nCntBoss].dotPos).y) < VALUE_MOVE )
			{
				g_boss[nCntBoss].pos = DotPos2Pos(g_boss[nCntBoss].dotPos);
				g_boss[nCntBoss].walking = false;
			}
			
			//���̈ʒu�ړ�������
			if(g_boss[nCntBoss].pos == DotPos2Pos(g_boss[nCntBoss].dotPos))
			{
				g_boss[nCntBoss].walking = false;
			}

			//���S
			if(g_boss[nCntBoss].life == 0)
			{
				g_boss[nCntBoss].nCounterDeath = 120; //hurt anime 60s
				g_boss[nCntBoss].nCounterHurt = -2; //do not use
				g_boss[nCntBoss].life = -1;
				SetSE(SOUND_LABEL_SE_GAMESTART);
				break;
			}

			//if Boss Hurt
			if(g_boss[nCntBoss].nCounterHurt > 0)
			{
				g_boss[nCntBoss].nCounterHurt--;
				if(g_boss[nCntBoss].nCounterHurt % 10 == 0)
				{
					hurt_flag = !hurt_flag;				
				}
				if(hurt_flag)
				{
					SetColorBoss(nCntBoss, 255, 0, 0, 255); //��
				}
				else
				{
					SetColorBoss(nCntBoss, 255, 255, 255, 255);
				}
			}
			else if(g_boss[nCntBoss].nCounterHurt == 0)
			{
				SetColorBoss(nCntBoss, 255, 255, 255, 255);
				g_boss[nCntBoss].nCounterHurt = -1;
				hurt_flag = false;
			}


			//if Boss Died
			if(g_boss[nCntBoss].nCounterDeath > 0)
			{
				g_boss[nCntBoss].nCounterDeath--;
				if(g_boss[nCntBoss].nCounterDeath % 5 == 0)
				{
					hurt_flag = !hurt_flag;				
				}
				if(hurt_flag)
				{
					SetColorBoss(nCntBoss, 255, 0, 0, 255); //��
					SetSE(SOUND_LABEL_SE_BOSSHURT);
				}
				else
				{
					SetColorBoss(nCntBoss, 255, 255, 255, 255);
				}
			}
			else if(g_boss[nCntBoss].nCounterDeath == 0)
			{
				SetColorBoss(nCntBoss, 255, 255, 255, 0);
				g_boss[nCntBoss].nCounterDeath = -2;
				g_boss[nCntBoss].bUse = false;
				hurt_flag = false;
				SetItem(ITEM_NEXT,g_boss[nCntBoss].dotPos); //NEXT������

			}

			//if BOSS died
			if(g_boss[nCntBoss].life == -1) break;

			if(g_boss[nCntBoss].walking)
			{
				switch(g_boss[nCntBoss].dir)
				{
					case D_UP:

						g_boss[nCntBoss].pos.y -= g_boss[nCntBoss].move.y;
						break;
					case D_DOWN:

						g_boss[nCntBoss].pos.y += g_boss[nCntBoss].move.y;
						break;
					case D_LEFT:

						g_boss[nCntBoss].pos.x -= g_boss[nCntBoss].move.x;
						break;
					case D_RIGHT:

						g_boss[nCntBoss].pos.x += g_boss[nCntBoss].move.x;
						break;
				}
			}
			else
			{
				//random�ړ� 
				new_dir = (DIRECTION)(rand()%4);
				
				// �ړ�
				if(new_dir == D_UP)
				{
					g_boss[nCntBoss].dir = D_UP;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y - 1][(int)g_boss[nCntBoss].dotPos.x] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.y -= g_boss[nCntBoss].move.y;
						g_boss[nCntBoss].dotPos.y -= 1;		
						g_boss[nCntBoss].walking = true;
					}
				}
				if(new_dir == D_DOWN)
				{
					g_boss[nCntBoss].dir = D_DOWN;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y + 1][(int)g_boss[nCntBoss].dotPos.x] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.y += g_boss[nCntBoss].move.y;
						g_boss[nCntBoss].dotPos.y += 1;
						g_boss[nCntBoss].walking = true;
					}

				}
				if(new_dir == D_LEFT)
				{
					g_boss[nCntBoss].dir = D_LEFT;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y][(int)g_boss[nCntBoss].dotPos.x - 1] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.x -= g_boss[nCntBoss].move.x;
						g_boss[nCntBoss].dotPos.x -= 1;
						g_boss[nCntBoss].walking = true;
					}
				}
				if(new_dir == D_RIGHT)
				{
					g_boss[nCntBoss].dir = D_RIGHT;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y][(int)g_boss[nCntBoss].dotPos.x + 1] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.x += g_boss[nCntBoss].move.x;
						g_boss[nCntBoss].dotPos.x += 1;
						g_boss[nCntBoss].walking = true;
					}
				}
			}


			SetVertexBoss(nCntBoss);


			//�����ύX
			SetTextureBoss(nCntBoss, g_boss[nCntBoss].nPatternAnim, g_boss[nCntBoss].dir);
	
			//�A�j���\��
			g_boss[nCntBoss].nCounterAnim++;
			if((g_boss[nCntBoss].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_boss[nCntBoss].nPatternAnim = (g_boss[nCntBoss].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// �e�N�X�`�����W��ݒ�
				SetTextureBoss(nCntBoss, g_boss[nCntBoss].nPatternAnim, g_boss[nCntBoss].dir);

				//nCounterAnim�̃��Z�b�g
				g_boss[nCntBoss].nCounterAnim = 0;
			}	
		}
	}
	
}

/*******************************************************************************
�֐���:	void SetVertexEnemy(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���_���W�̐ݒ�
*******************************************************************************/
void SetVertexBoss(int nCntBoss)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);
		
		pVtx += nCntBoss * NUM_VERTEX; 

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferBoss->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetTextureEnemy(int nPatternAnim)
����:	int nPatternAnim : �A�j���[�V�����p�^�[��No.
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureBoss(int nCntBoss, int nPatternAnim, DIRECTION dir)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBoss * NUM_VERTEX; 

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
		g_pVtxBufferBoss->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetEnemy(D3DXVECTOR3 pos)
����:	D3DXVECTOR3 pos�F�G��ݒu����ʒu
�߂�l:	�Ȃ�
����:	�G�̐ݒu
*******************************************************************************/
void SetBoss(D3DXVECTOR3 pos)
{
	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(!g_boss[nCntBoss].bUse)
		{
			//�ʒu��ݒ�
			g_boss[nCntBoss].pos = pos;
			SetVertexBoss(nCntBoss);
			g_boss[nCntBoss].bUse = true; //���˒��ɕύX
			break;
		}
	}

}

void SetColorBoss(int nCntBoss, int R, int G, int B, int A)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntBoss * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferBoss->Unlock();
	
}



/*******************************************************************************
�֐���:	BULLET* GetBullet(void)
����:	�Ȃ�
�߂�l: BULLET*�FBULLET�̃|�C���^
����:	�����蔻��Ȃǂ̎��A�e�̏���ǂݎ��
*******************************************************************************/
BOSS *GetBoss(void)
{
	return g_boss;
}