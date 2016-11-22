//******************************************************************************
//
// �^�C�g��:		���̓��[�h�̓G����
// �v���O������:	enemy.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "map.h"

#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/teki0001.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_DOTPOS_X			(0)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_DOTPOS_Y			(0)								// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50.0f)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(50.0f)							// �|���S���̍���

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
HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice);
void SetTextureEnemy(int nCntEnemy, int nPatternAnim, DIRECTION dir);
void SetVertexEnemy(int nCntEnemy);
bool CollisionCheckEB(int enemyIdx);
bool CollisionCheckEP(int enemyIdx);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;

ENEMY	g_enemy[MAX_ENEMY]; //�v���C���[�̃��[�N

/*******************************************************************************
�֐���:	HRESULT InitEnemy(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
*******************************************************************************/
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//enemy�̏�����
		int randX, randY;
		do
		{
			randX = rand()%19 + 3;
			randY = rand()%10;
		}while(pMap->data[randY][randX] != BLOCK_GRASS);

		g_enemy[nCntEnemy].dotPos = D3DXVECTOR3( randX, randY, 0.0f);

		g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
		g_enemy[nCntEnemy].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_enemy[nCntEnemy].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_enemy[nCntEnemy].dir = (DIRECTION)(rand()%4);
		g_enemy[nCntEnemy].walking = false;

		GAME_MODE mode = GetGameMode();
		switch(mode)
		{
			case GAME_COOPERATE:
				g_enemy[nCntEnemy].bUse = true;	
				break;
			case GAME_BATTLE:
				g_enemy[nCntEnemy].bUse = false;	
				break;
		}

		
	}

	// �A�j���[�V�����̏�����



	//���_���̍쐬
	if(FAILED(MakeVertexEnemy(pDevice)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,		// �t�@�C���̖��O
								&g_pTextureEnemy);	// �ǂݍ��ރ������[

	return S_OK;
}

/*******************************************************************************
�֐���:	void DrawEnemy(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEnemy);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntEnemy*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
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
void UninitEnemy(void)
{
	if(g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}
	if(g_pVtxBufferEnemy != NULL)
	{
		g_pVtxBufferEnemy->Release();
		g_pVtxBufferEnemy = NULL;
	}

	//enemy�𗘗p���Ȃ��ɐݒ肷��
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_enemy[nCntEnemy].bUse = false;	
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ENEMY,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferEnemy,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferEnemy->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	
	}



	g_pVtxBufferEnemy->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdateEnemy(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdateEnemy(void)
{	
	DIRECTION new_dir;
	MAP *pMap = GetMap();

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{

			//�ړ��ʒu����
			if(abs(g_enemy[nCntEnemy].pos.x - DotPos2Pos(g_enemy[nCntEnemy].dotPos).x) < VALUE_MOVE && abs(g_enemy[nCntEnemy].pos.y - DotPos2Pos(g_enemy[nCntEnemy].dotPos).y) < VALUE_MOVE )
			{
				g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
				g_enemy[nCntEnemy].walking = false;
			}
			
			//���̈ʒu�ړ�������
			if(g_enemy[nCntEnemy].pos == DotPos2Pos(g_enemy[nCntEnemy].dotPos))
			{
				g_enemy[nCntEnemy].walking = false;
			}

			if(g_enemy[nCntEnemy].walking)
			{
				switch(g_enemy[nCntEnemy].dir)
				{
					case D_UP:
						//�G�ƒe�̓����蔻��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.y += 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.y -= g_enemy[nCntEnemy].move.y;
						break;
					case D_DOWN:
						//�G�ƒe�̓����蔻��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.y -= 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;
						break;
					case D_LEFT:
						//�G�ƒe�̓����蔻��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.x += 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.x -= g_enemy[nCntEnemy].move.x;
						break;
					case D_RIGHT:
						//�G�ƒe�̓����蔻��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.x -= 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].move.x;
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
					g_enemy[nCntEnemy].dir = D_UP;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y - 1][(int)g_enemy[nCntEnemy].dotPos.x] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.y -= g_enemy[nCntEnemy].move.y;
						g_enemy[nCntEnemy].dotPos.y -= 1;		
						g_enemy[nCntEnemy].walking = true;
					}
				}
				if(new_dir == D_DOWN)
				{
					g_enemy[nCntEnemy].dir = D_DOWN;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y + 1][(int)g_enemy[nCntEnemy].dotPos.x] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;
						g_enemy[nCntEnemy].dotPos.y += 1;
						g_enemy[nCntEnemy].walking = true;
					}

				}
				if(new_dir == D_LEFT)
				{
					g_enemy[nCntEnemy].dir = D_LEFT;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y][(int)g_enemy[nCntEnemy].dotPos.x - 1] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.x -= g_enemy[nCntEnemy].move.x;
						g_enemy[nCntEnemy].dotPos.x -= 1;
						g_enemy[nCntEnemy].walking = true;
					}
				}
				if(new_dir == D_RIGHT)
				{
					g_enemy[nCntEnemy].dir = D_RIGHT;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y][(int)g_enemy[nCntEnemy].dotPos.x + 1] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].move.x;
						g_enemy[nCntEnemy].dotPos.x += 1;
						g_enemy[nCntEnemy].walking = true;
					}
				}
			}


			SetVertexEnemy(nCntEnemy);


			//�����ύX
			SetTextureEnemy(nCntEnemy, g_enemy[nCntEnemy].nPatternAnim, g_enemy[nCntEnemy].dir);
	
			//�A�j���\��
			g_enemy[nCntEnemy].nCounterAnim++;
			if((g_enemy[nCntEnemy].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_enemy[nCntEnemy].nPatternAnim = (g_enemy[nCntEnemy].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// �e�N�X�`�����W��ݒ�
				SetTextureEnemy(nCntEnemy, g_enemy[nCntEnemy].nPatternAnim, g_enemy[nCntEnemy].dir);

				//nCounterAnim�̃��Z�b�g
				g_enemy[nCntEnemy].nCounterAnim = 0;
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
void SetVertexEnemy(int nCntEnemy)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);
		
		pVtx += nCntEnemy * NUM_VERTEX; 

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferEnemy->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetTextureEnemy(int nPatternAnim)
����:	int nPatternAnim : �A�j���[�V�����p�^�[��No.
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureEnemy(int nCntEnemy, int nPatternAnim, DIRECTION dir)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * NUM_VERTEX; 

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
		g_pVtxBufferEnemy->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetEnemy(D3DXVECTOR3 pos)
����:	D3DXVECTOR3 pos�F�G��ݒu����ʒu
�߂�l:	�Ȃ�
����:	�G�̐ݒu
*******************************************************************************/
void SetEnemy(D3DXVECTOR3 pos)
{
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(!g_enemy[nCntEnemy].bUse)
		{
			//�ʒu��ݒ�
			g_enemy[nCntEnemy].pos = pos;
			SetVertexEnemy(nCntEnemy);
			g_enemy[nCntEnemy].bUse = true; //���˒��ɕύX
			break;
		}
	}

}


/*******************************************************************************
�֐���:	bool CollisionCheckEB(int enemyIdx)
����:	int enemyIdx�F���Ԗڂ̓G
�߂�l:	bool�F������ true�A������Ȃ� false
����:	�G(E)�ƒe(B)�̓����蔻��
*******************************************************************************/
bool CollisionCheckEB(int enemyIdx)
{
	BULLET* pBellet = GetBullet();


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(pBellet[nCntBullet].bUse)
		{
			//�o�E���f�B���O�X�t�B�A
			if( (g_enemy[enemyIdx].pos.x - pBellet[nCntBullet].pos.x)*(g_enemy[enemyIdx].pos.x - pBellet[nCntBullet].pos.x) + (g_enemy[enemyIdx].pos.y - pBellet[nCntBullet].pos.y)*(g_enemy[enemyIdx].pos.y - pBellet[nCntBullet].pos.y) < (POLYGON_SIZE_X/2 + pBellet[nCntBullet].size.x/2)*(POLYGON_SIZE_X/2+pBellet[nCntBullet].size.x/2) )
			{
				return true;
			}	
		}

	}	
	return false;
}


/*******************************************************************************
�֐���:	BULLET* GetBullet(void)
����:	�Ȃ�
�߂�l: BULLET*�FBULLET�̃|�C���^
����:	�����蔻��Ȃǂ̎��A�e�̏���ǂݎ��
*******************************************************************************/
ENEMY *GetEnemy(void)
{
	return g_enemy;
}