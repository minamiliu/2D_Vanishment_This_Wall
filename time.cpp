//******************************************************************************
//
// �^�C�g��:		���Ԃ̕\������
// �v���O������:	time.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "time.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number003.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(590)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(60)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(35)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(70)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(10)							// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define NUM_OBJECT					(3)								// �I�u�W�F�N�g�̐�
#define STAGE_TIME					(100)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);
void SetTextureTime( int nIdx, int number);
HRESULT ChangeVertexColor(LPDIRECT3DDEVICE9 pDevice, int R, int G, int B);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTime = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;

TIME			g_time; //Time�̃��[�N
int				nCounterSecond;

/*******************************************************************************
�֐���:	HRESULT InitPolygon(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
*******************************************************************************/
HRESULT InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	nCounterSecond = 0;

	//Time�̏�����
	g_time.pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_time.time = STAGE_TIME;

	//���_���̍쐬
	if(FAILED(MakeVertexTime(pDevice)))
	{
		return E_FAIL;
	}

	SetTime(STAGE_TIME);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,	// �t�@�C���̖��O
								&g_pTextureTime);	// �ǂݍ��ރ������[

	return S_OK;
}

/*******************************************************************************
�֐���:	void DrawPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawTime(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	//�|���S���̕`��
	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			(nCntPlace*NUM_VERTEX),						//���[�h����ŏ��̒��_�C���f�b�N�X
			NUM_POLYGON				//�|���S���̐�
		);
	}


}

/*******************************************************************************
�֐���:	void UninitTime(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̊J���֐�
*******************************************************************************/
void UninitTime(void)
{
	if(g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	if(g_pVtxBufferTime != NULL)
	{
		g_pVtxBufferTime->Release();
		g_pVtxBufferTime = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferTime,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTime->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_time.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_time.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_time.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_time.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferTime->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdatePolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdateTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	nCounterSecond++;

	if(nCounterSecond >= 60)
	{
		if(g_time.time <= 10)
		{
			//���_�J���[�̕ύX
			ChangeVertexColor(pDevice,255,0,0);

			//SE
			SetSE(SOUND_LABEL_SE_COUNTDOWN);
		}
		
		//time's up
		if(g_time.time <= 0)
		{
			SetGameStep(STEP_GAMEOVER);
			SetMessage(MSG_TIME_UP);

			//SE
			SetSE(SOUND_LABEL_SE_GAMEOVER);
		}
		g_time.time--;
		SetTime(g_time.time);
		nCounterSecond = 0;
	}
}



/*******************************************************************************
�֐���:	void SetTextureTime( int nTdx, int number) 
����:	 int nTdx�F�����ڂ�, int number�F�\�����鐔
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureTime( int nIdx, int number) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferTime->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;

		//�e�N�X�`�����W�̐ݒ�
		fPosXLeft = TEX_PATTERN_SIZE_X * number;
		fPosXRight = TEX_PATTERN_SIZE_X * number + TEX_PATTERN_SIZE_X;
		fPosYUp = 0;
		fPosYDown = TEX_PATTERN_SIZE_Y;

		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferTime->Unlock();
	}
}

/*******************************************************************************
�֐���:	void ChangeTime(int value)
����:	int value�F��������_��
�߂�l:	�Ȃ�
����:	���_����ύX����
*******************************************************************************/
void SetTime(int value)
{

	int number;

	for(int keta = 1; keta <= NUM_OBJECT; keta++)
	{
		number = value % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTextureTime(keta,number);
	}
}

HRESULT ChangeVertexColor(LPDIRECT3DDEVICE9 pDevice, int R, int G, int B)
{

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTime->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++, pVtx += NUM_VERTEX)
	{
		//color
		pVtx[0].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[1].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[2].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[3].col = D3DCOLOR_RGBA(R,G,B,255);
	}



	g_pVtxBufferTime->Unlock();

	return S_OK;
}
