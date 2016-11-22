//******************************************************************************
//
// �^�C�g��:		���̓��[�h��UI�\������
// �v���O������:	bar.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "bar.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_BAR_ICON	(7)

#define POLYGON_TEXTURENAME_BAR_P1		"data/TEXTURE/BAR/1P.png"
#define POLYGON_TEXTURENAME_BAR_P2		"data/TEXTURE/BAR/2P.png"
#define POLYGON_TEXTURENAME_BAR_P3		"data/TEXTURE/BAR/3P.png"
#define POLYGON_TEXTURENAME_BAR_P4		"data/TEXTURE/BAR/4P.png"
#define POLYGON_TEXTURENAME_BAR_SCORE	"data/TEXTURE/BAR/score.png"
#define POLYGON_TEXTURENAME_BAR_TIME	"data/TEXTURE/BAR/time.png"
#define POLYGON_TEXTURENAME_BAR_LIFE	"data/TEXTURE/BAR/life.png"

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexBar(LPDIRECT3DDEVICE9 pDevice);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBar = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBar[MAX_BAR_ICON] = {NULL};

BAR_ICON	g_barIcon[MAX_BAR_ICON];


/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitBar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//p1
	g_barIcon[0].pos = D3DXVECTOR3( 160, 25, 0.0f);
	g_barIcon[0].size = D3DXVECTOR3( 50, 25, 0.0f);
	g_barIcon[0].col = D3DCOLOR_RGBA(255,0,0,255); 

	//p2
	g_barIcon[1].pos = D3DXVECTOR3( 270, 25, 0.0f);
	g_barIcon[1].size = D3DXVECTOR3( 50, 25, 0.0f);
	g_barIcon[1].col = D3DCOLOR_RGBA(255,255,0,255); 

	//p3
	g_barIcon[2].pos = D3DXVECTOR3( 160, 65, 0.0f);
	g_barIcon[2].size = D3DXVECTOR3( 50, 25, 0.0f);
	g_barIcon[2].col = D3DCOLOR_RGBA(0,255,0,255); 

	//p4
	g_barIcon[3].pos = D3DXVECTOR3( 270, 65, 0.0f);
	g_barIcon[3].size = D3DXVECTOR3( 50, 25, 0.0f);
	g_barIcon[3].col = D3DCOLOR_RGBA(0,0,255,255); 

	//score
	g_barIcon[4].pos = D3DXVECTOR3( 770, 20, 0.0f);
	g_barIcon[4].size = D3DXVECTOR3( 200, 70, 0.0f);
	g_barIcon[4].col = D3DCOLOR_RGBA(30,225,180,255); 

	//time
	g_barIcon[5].pos = D3DXVECTOR3( 420, 20, 0.0f);
	g_barIcon[5].size = D3DXVECTOR3( 150, 70, 0.0f);
	g_barIcon[5].col = D3DCOLOR_RGBA(30,225,180,255);

	//life
	g_barIcon[6].pos = D3DXVECTOR3( 10, 20, 0.0f);
	g_barIcon[6].size = D3DXVECTOR3( 150, 70, 0.0f);
	g_barIcon[6].col = D3DCOLOR_RGBA(30,225,180,255);
	

	//���_���̍쐬
	if(FAILED(MakeVertexBar(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P1, &g_pTextureBar[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P2, &g_pTextureBar[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P3, &g_pTextureBar[2]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_P4, &g_pTextureBar[3]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_SCORE, &g_pTextureBar[4]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_TIME, &g_pTextureBar[5]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_BAR_LIFE, &g_pTextureBar[6]);


	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawBar(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferBar, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntIcon = 0; nCntIcon < MAX_BAR_ICON; nCntIcon++)
	{

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBar[nCntIcon]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntIcon*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
			NUM_POLYGON				//�|���S���̐�
		);		
	}


}
/*******************************************************************************
�֐���:	void UninitBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̊J���֐�
*******************************************************************************/
void UninitBar(void)
{
	for(int nCntIcon = 0; nCntIcon < MAX_BAR_ICON; nCntIcon++)
	{
		if(g_pTextureBar[nCntIcon] != NULL)
		{
			g_pTextureBar[nCntIcon]->Release();
			g_pTextureBar[nCntIcon] = NULL;
		}
	}

	if(g_pVtxBufferBar != NULL)
	{
		g_pVtxBufferBar->Release();
		g_pVtxBufferBar = NULL;
	}	


}
/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexBar(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BAR_ICON,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBar,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferBar->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntIcon = 0; nCntIcon < MAX_BAR_ICON; nCntIcon++,pVtx+=4)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3( g_barIcon[nCntIcon].pos.x, g_barIcon[nCntIcon].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_barIcon[nCntIcon].pos.x + g_barIcon[nCntIcon].size.x, g_barIcon[nCntIcon].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_barIcon[nCntIcon].pos.x, g_barIcon[nCntIcon].pos.y + g_barIcon[nCntIcon].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_barIcon[nCntIcon].pos.x + g_barIcon[nCntIcon].size.x, g_barIcon[nCntIcon].pos.y + g_barIcon[nCntIcon].size.y, 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_barIcon[nCntIcon].col;
		pVtx[1].col = g_barIcon[nCntIcon].col;
		pVtx[2].col = g_barIcon[nCntIcon].col;
		pVtx[3].col = g_barIcon[nCntIcon].col;

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferBar->Unlock();

	return S_OK;
}

void UpdateBar(void)
{


}


