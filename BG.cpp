//******************************************************************************
//
// �^�C�g��:		�Q�[���̔w�i����
// �v���O������:	BG.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "BG.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//�|���S���̍���X
#define POLYGON_POS_Y	(100)//�|���S���̍���Y
#define POLYGON_SIZE_X	(1250)//�|���S����SIZE X
#define POLYGON_SIZE_Y	(100)//�|���S����SIZE Y
#define POLYGON_TEXTURENAME "data/TEXTURE/BG/bg_aozora.png"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBG = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;

/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̍쐬
	if(FAILED(MakeVertexBG(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME, &g_pTextureBG);


	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawBG(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBG);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,						//���[�h����ŏ��̒��_�C���f�b�N�X
		NUM_POLYGON				//�|���S���̐�
	);

}
/*******************************************************************************
�֐���:	void UninitBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̊J���֐�
*******************************************************************************/
void UninitBG(void)
{
	if(g_pTextureBG != NULL)
	{
		g_pTextureBG->Release();
		g_pTextureBG = NULL;
	}
	if(g_pVtxBufferBG != NULL)
	{
		g_pVtxBufferBG->Release();
		g_pVtxBufferBG = NULL;
	}
}
/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBG,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferBG->Lock( 0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);


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

	//�e�N�X�`�����W�w��
	pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
	pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
	pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
	pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);

	g_pVtxBufferBG->Unlock();

	return S_OK;
}
