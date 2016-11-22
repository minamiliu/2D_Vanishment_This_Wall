//******************************************************************************
//
// �^�C�g��:		���e����
// �v���O������:	bullet.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "bullet.h"
#include "explosion.h"
#include "map.h"
#include "item.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/bomb4.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(-100)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(-100)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(50)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(4)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define COUNTDOWN					(120)							//�����̃J���g�_�E��(�t���[����)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBullet(int nCntBullet);
void SetTextureBullet(int nCntBullet,int nPatternAnim);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;

BULLET			g_bullet[MAX_BULLET]; //�v���C���[�̃��[�N

/*******************************************************************************
�֐���:	HRESULT InitPolygon(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
*******************************************************************************/
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//bullet�̏�����
		g_bullet[nCntBullet].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_bullet[nCntBullet].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_bullet[nCntBullet].bUse = false;
		g_bullet[nCntBullet].who_setted = -1;

		// �A�j���[�V�����̏�����
		g_bullet[nCntBullet].nCounterAnim = 0;
		g_bullet[nCntBullet].nPatternAnim = 0;	
		g_bullet[nCntBullet].nCounterExpo = COUNTDOWN;
	}



	//���_���̍쐬
	if(FAILED(MakeVertexBullet(pDevice)))
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,		// �t�@�C���̖��O
								&g_pTextureBullet);	// �ǂݍ��ރ������[

	return S_OK;
}

/*******************************************************************************
�֐���:	void DrawPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(g_bullet[nCntBullet].bUse)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBullet*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
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
void UninitBullet(void)
{
	if(g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	if(g_pVtxBufferBullet != NULL)
	{
		g_pVtxBufferBullet->Release();
		g_pVtxBufferBullet = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BULLET,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBullet,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferBullet->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}

	g_pVtxBufferBullet->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdatePolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdateBullet(void)
{
	MAP *pMap = GetMap();

	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(g_bullet[nCntBullet].bUse)
		{
			//���_���W�̐ݒ�
			//SetVertexBullet(nCntBullet);

			//���e�̃J�E���g�_�E��
			g_bullet[nCntBullet].nCounterExpo--;
			if(g_bullet[nCntBullet].nCounterExpo == 0)
			{
				SetScaleOfExplosion(g_bullet[nCntBullet].pos, g_bullet[nCntBullet].power, g_bullet[nCntBullet].who_setted);
				pMap->item[(int)Pos2DotPos(g_bullet[nCntBullet].pos).y][(int)Pos2DotPos(g_bullet[nCntBullet].pos).x].type = ITEM_NONE;
				g_bullet[nCntBullet].bUse = false;
				g_bullet[nCntBullet].nCounterExpo = COUNTDOWN;
			}

			g_bullet[nCntBullet].nCounterAnim++;
			if((g_bullet[nCntBullet].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{

				// �p�^�[���̐؂�ւ�
				g_bullet[nCntBullet].nPatternAnim = (g_bullet[nCntBullet].nPatternAnim + 1) % NUM_ANIM_PATTERN;

				// �e�N�X�`�����W��ݒ�
				SetTextureBullet(nCntBullet,g_bullet[nCntBullet].nPatternAnim);
			
				//g_nCounterAnim�̃��Z�b�g
				g_bullet[nCntBullet].nCounterAnim = 0;
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
void SetVertexBullet(int nCntBullet)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBullet * NUM_VERTEX; 

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_bullet[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_bullet[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		
		

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferBullet->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetTexturePolygon(int nPatternAnim)
����:	int nPatternAnim : �A�j���[�V�����p�^�[��No.
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureBullet(int nCntBullet,int nPatternAnim)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;
	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntBullet*NUM_VERTEX;

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
	g_pVtxBufferBullet->Unlock();

}

/*******************************************************************************
�֐���:	void SetBullet(D3DXVECTOR3 pos)
����:	D3DXVECTOR3 pos�F�e��ݒu����ʒu
�߂�l:	�Ȃ�
����:	�e�̐ݒu
*******************************************************************************/
void SetBullet(int nPlayerIdx, D3DXVECTOR3 pos, int power)
{
	MAP *pMap = GetMap();


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(!g_bullet[nCntBullet].bUse) 
		{
			//�ʒu��ݒ�			
			g_bullet[nCntBullet].pos = pos;
			g_bullet[nCntBullet].power = power;
			SetVertexBullet(nCntBullet);
			g_bullet[nCntBullet].bUse = true; //���˒��ɕύX
			g_bullet[nCntBullet].who_setted = nPlayerIdx;

			//item map�ɋL�^
			pMap->item[(int)Pos2DotPos(pos).y][(int)Pos2DotPos(pos).x].index = nCntBullet;
			pMap->item[(int)Pos2DotPos(pos).y][(int)Pos2DotPos(pos).x].type = ITEM_SETTED_BOMB;
			pMap->item[(int)Pos2DotPos(pos).y][(int)Pos2DotPos(pos).x].pos = pos;
			break;
		}
	}
	SetSE(SOUND_LABEL_SE_SETBOMB);
}

/*******************************************************************************
�֐���:	BULLET* GetBullet(void)
����:	�Ȃ�
�߂�l: BULLET*�FBULLET�̃|�C���^
����:	�����蔻��Ȃǂ̎��A�e�̏���ǂݎ��
*******************************************************************************/
BULLET *GetBullet(void)
{
	return g_bullet;
}

