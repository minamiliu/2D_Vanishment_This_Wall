//******************************************************************************
//
// �^�C�g��:		���U���g�̃o�g�����ʁF�����|���񐔕\������
// �v���O������:	result_kdw_kill.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "result_kdw_kill.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number003.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(0)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(60)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(100)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(10)							// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define NUM_DIGIT					(2)
#define MAX_NUM_BOUND				(99)
#define NUM_OBJECT					(NUM_DIGIT*MAX_PLAYER)			// �I�u�W�F�N�g�̐�

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexResultKDWKill(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexScore(void);
void SetTextureResultKDWKill( int nPlayerIdx, int nIdx, int number);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultKDWKill = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultKDWKill = NULL;

RESULT_KDW_KILL	g_resultKill[MAX_PLAYER]; //score�̃��[�N


/*******************************************************************************
�֐���:	HRESULT InitPolygon(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	����������
*******************************************************************************/
HRESULT InitResultKDWKill()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//bool *pPlayerList = GetPlayerList();

	//P1 life
	g_resultKill[0].pos = D3DXVECTOR3( 600, 200, 0.0f);
	//g_resultKill[0].col = D3DCOLOR_RGBA(255,0,0,255); //red
	g_resultKill[0].col = D3DCOLOR_RGBA(255,255,255,255);

	//P2 life
	g_resultKill[1].pos = D3DXVECTOR3( 600, 300, 0.0f);
	//g_resultKill[1].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
	g_resultKill[1].col = D3DCOLOR_RGBA(255,255,255,255);

	//P3 life
	g_resultKill[2].pos = D3DXVECTOR3( 600, 400, 0.0f);
	//g_resultKill[2].col = D3DCOLOR_RGBA(0,255,0,255); //green
	g_resultKill[2].col = D3DCOLOR_RGBA(255,255,255,255);

	//P4 life
	g_resultKill[3].pos = D3DXVECTOR3( 600, 500, 0.0f);
	//g_resultKill[3].col = D3DCOLOR_RGBA(0,0,255,255); //blue
	g_resultKill[3].col = D3DCOLOR_RGBA(255,255,255,255);


	//�l�����̂݁A������
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_resultKill[nCntPlayer].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_resultKill[nCntPlayer].score = 0;
	}


	//���_���̍쐬
	if(FAILED(MakeVertexResultKDWKill(pDevice)))
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,	// �t�@�C���̖��O
								&g_pTextureResultKDWKill);	// �ǂݍ��ރ������[

	return S_OK;
}

/*******************************************************************************
�֐���:	void DrawPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̕`��֐�
*******************************************************************************/
void DrawResultKDWKill(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferResultKDWKill, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultKDWKill);

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
�֐���:	void UninitScore(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�|���S���̊J���֐�
*******************************************************************************/
void UninitResultKDWKill(void)
{
	if(g_pTextureResultKDWKill != NULL)
	{
		g_pTextureResultKDWKill->Release();
		g_pTextureResultKDWKill = NULL;
	}
	if(g_pVtxBufferResultKDWKill!= NULL)
	{
		g_pVtxBufferResultKDWKill->Release();
		g_pVtxBufferResultKDWKill = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexResultKDWKill(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferResultKDWKill,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferResultKDWKill->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for(int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++, pVtx += NUM_VERTEX)
		{

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			//color
			pVtx[0].col = g_resultKill[nCntPlayer].col;
			pVtx[1].col = g_resultKill[nCntPlayer].col;
			pVtx[2].col = g_resultKill[nCntPlayer].col;
			pVtx[3].col = g_resultKill[nCntPlayer].col;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_resultKill[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_resultKill[nCntPlayer].pos.y , 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_resultKill[nCntPlayer].pos.x  + g_resultKill[nCntPlayer].size.x +  nCntDigit*POLYGON_SIZE_X, g_resultKill[nCntPlayer].pos.y , 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_resultKill[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_resultKill[nCntPlayer].pos.y + g_resultKill[nCntPlayer].size.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_resultKill[nCntPlayer].pos.x  + g_resultKill[nCntPlayer].size.x + nCntDigit*POLYGON_SIZE_X, g_resultKill[nCntPlayer].pos.y + g_resultKill[nCntPlayer].size.y, 0.0f);

			//�e�N�X�`�����W�w��
			pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
			pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
			pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
			pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		}	
	}

	g_pVtxBufferResultKDWKill->Unlock();

	return S_OK;
}


/*******************************************************************************
�֐���:	void UpdatePolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�X�V����
*******************************************************************************/
void UpdateResultKDWKill(void)
{

}



/*******************************************************************************
�֐���:	void SetTextureScore( int nTdx, int number) 
����:	 int nTdx�F�����ڂ�, int number�F�\�����鐔
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureResultKDWKill( int nPlayerIdx, int nIdx, int number) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferResultKDWKill->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nPlayerIdx * NUM_DIGIT * NUM_VERTEX);

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
		g_pVtxBufferResultKDWKill->Unlock();
	}
}

/*******************************************************************************
�֐���:	void ChangeScore(int value)
����:	int value�F��������_��
�߂�l:	�Ȃ�
����:	���_����ύX����
*******************************************************************************/
void ChangeResultKDWKill(int nPlayerIdx, int value)
{
	g_resultKill[nPlayerIdx].score += value;

	if(g_resultKill[nPlayerIdx].score >= MAX_NUM_BOUND) g_resultKill[nPlayerIdx].score = MAX_NUM_BOUND; //2���̍ő�l

	int number = 0;

	for(int keta = 1; keta <= NUM_DIGIT; keta++)
	{
		number = g_resultKill[nPlayerIdx].score % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTextureResultKDWKill(nPlayerIdx ,keta,number);
	}
}