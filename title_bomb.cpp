//******************************************************************************
//
// �^�C�g��:		�^�C�g���̔��e�e�N�X�`������
// �v���O������:	title_bomb.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "title_bomb.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/bomb4.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(250)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(550)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(300)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(300)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(4)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define COUNTDOWN					(120)							//�����̃J���g�_�E��(�t���[����)
#define MAX_TITLE_BOMB				(1)			

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexTitleBomb(LPDIRECT3DDEVICE9 pDevice);
void SetVertexTitleBomb(int nCntBullet);
void SetTextureTitleBomb(int nCntBullet,int nPatternAnim);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitleBomb = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitleBomb = NULL;

TITLE_BOMB			g_title_bomb[MAX_TITLE_BOMB]; //�v���C���[�̃��[�N

/*******************************************************************************
�֐���:	HRESULT InitPolygon(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
*******************************************************************************/
HRESULT InitTitleBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++)
	{
		//bullet�̏�����
		g_title_bomb[nCntBullet].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_title_bomb[nCntBullet].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_title_bomb[nCntBullet].bUse = true;

		// �A�j���[�V�����̏�����
		g_title_bomb[nCntBullet].nCounterAnim = 0;
		g_title_bomb[nCntBullet].nPatternAnim = 0;	
	}



	//���_���̍쐬
	if(FAILED(MakeVertexTitleBomb(pDevice)))
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,		// �t�@�C���̖��O
								&g_pTextureTitleBomb);	// �ǂݍ��ރ������[

	return S_OK;
}

/*******************************************************************************
�֐���:	void DrawPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawTitleBomb(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferTitleBomb, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleBomb);


	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++)
	{
		if(g_title_bomb[nCntBullet].bUse)
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
void UninitTitleBomb(void)
{
	if(g_pTextureTitleBomb != NULL)
	{
		g_pTextureTitleBomb->Release();
		g_pTextureTitleBomb = NULL;
	}
	if(g_pVtxBufferTitleBomb != NULL)
	{
		g_pVtxBufferTitleBomb->Release();
		g_pVtxBufferTitleBomb = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexTitleBomb(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_TITLE_BOMB,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferTitleBomb,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTitleBomb->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}

	g_pVtxBufferTitleBomb->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdatePolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdateTitleBomb(void)
{

	for(int nCntBullet = 0; nCntBullet < MAX_TITLE_BOMB; nCntBullet++)
	{
		if(g_title_bomb[nCntBullet].bUse)
		{
			//���_���W�̐ݒ�
			SetVertexTitleBomb(nCntBullet);

			g_title_bomb[nCntBullet].nCounterAnim++;
			if((g_title_bomb[nCntBullet].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{

				// �p�^�[���̐؂�ւ�
				g_title_bomb[nCntBullet].nPatternAnim = (g_title_bomb[nCntBullet].nPatternAnim + 1) % NUM_ANIM_PATTERN;

				// �e�N�X�`�����W��ݒ�
				SetTextureTitleBomb(nCntBullet,g_title_bomb[nCntBullet].nPatternAnim);
			
				//g_nCounterAnim�̃��Z�b�g
				g_title_bomb[nCntBullet].nCounterAnim = 0;
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
void SetVertexTitleBomb(int nCntBullet)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferTitleBomb->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBullet * NUM_VERTEX; 

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x - (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_title_bomb[nCntBullet].pos.x + (POLYGON_SIZE_X/2), g_title_bomb[nCntBullet].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		
		

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferTitleBomb->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetTexturePolygon(int nPatternAnim)
����:	int nPatternAnim : �A�j���[�V�����p�^�[��No.
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureTitleBomb(int nCntBullet,int nPatternAnim)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;
	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTitleBomb->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBufferTitleBomb->Unlock();

}





