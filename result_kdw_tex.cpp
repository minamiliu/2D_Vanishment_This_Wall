//******************************************************************************
//
// �^�C�g��:		���U���g�̃o�g������UI�\������
// �v���O������:	result_kdw_tex.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "result_kdw_tex.h"
#include "player.h"
#include "num_win.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_KDW_ICON	(MAX_PLAYER*4)

#define POLYGON_TEXTURENAME_TEX_P1		"data/TEXTURE/RESULT/1P.png"
#define POLYGON_TEXTURENAME_TEX_P2		"data/TEXTURE/RESULT/2P.png"
#define POLYGON_TEXTURENAME_TEX_P3		"data/TEXTURE/RESULT/3P.png"
#define POLYGON_TEXTURENAME_TEX_P4		"data/TEXTURE/RESULT/4P.png"
#define POLYGON_TEXTURENAME_TEX_DEATH	"data/TEXTURE/RESULT/DEATH.png"
#define POLYGON_TEXTURENAME_TEX_KILL	"data/TEXTURE/RESULT/KILL.png"
#define POLYGON_TEXTURENAME_TEX_CROWN	"data/TEXTURE/RESULT/CROWN2.png"

#define POLYGON_P1_POS_X	(200)
#define POLYGON_P1_POS_Y	(200)

#define POLYGON_PN_SIZE_X	(150)
#define POLYGON_PN_SIZE_Y	(100)

#define POLYGON_KILL_SIZE_X	(200)
#define POLYGON_KILL_SIZE_Y	(100)

#define POLYGON_DEATH_SIZE_X	(200)
#define POLYGON_DEATH_SIZE_Y	(100)

#define POLYGON_WIN_SIZE_X	(100)
#define POLYGON_WIN_SIZE_Y	(100)


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexResultKDWTex(LPDIRECT3DDEVICE9 pDevice);
void SetVertexResultKDWTex(int nIconIdx);
/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultKDWTex = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultKDWTex[MAX_KDW_ICON] = {NULL};

RESULT_KDW_TEX	g_KDWIcon[MAX_KDW_ICON];
float g_angle3;

/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitResultKDWTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++)
	{
		g_KDWIcon[nCntIcon].bUse = true;
	}

	//1P
	g_KDWIcon[0].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y, 0.0f);
	g_KDWIcon[0].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[0].col = D3DCOLOR_RGBA(255,0,0,255); //red
	{
		//kill
		g_KDWIcon[1].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y, 0.0f);
		g_KDWIcon[1].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[1].col = D3DCOLOR_RGBA(255,0,0,255); //red

		//death
		g_KDWIcon[2].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y, 0.0f);
		g_KDWIcon[2].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[2].col = D3DCOLOR_RGBA(255,0,0,255); //red

		//win
		g_KDWIcon[3].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[3].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[3].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[3].bUse = (GetWinner() == 0) ? true : false;
	}

	//2P
	g_KDWIcon[4].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y + 100, 0.0f);
	g_KDWIcon[4].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[4].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
	{
		//kill
		g_KDWIcon[5].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y + 100, 0.0f);
		g_KDWIcon[5].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[5].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

		//death
		g_KDWIcon[6].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y + 100, 0.0f);
		g_KDWIcon[6].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[6].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

		//win
		g_KDWIcon[7].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + 100 + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[7].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[7].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[7].bUse = (GetWinner() == 1) ? true : false;
	}

	//3P
	g_KDWIcon[8].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y + 200, 0.0f);
	g_KDWIcon[8].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[8].col = D3DCOLOR_RGBA(0,255,0,255); //green
	{
		//kill
		g_KDWIcon[9].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y + 200, 0.0f);
		g_KDWIcon[9].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[9].col = D3DCOLOR_RGBA(0,255,0,255); //green

		//death
		g_KDWIcon[10].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y + 200, 0.0f);
		g_KDWIcon[10].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[10].col = D3DCOLOR_RGBA(0,255,0,255); //green

		//win
		g_KDWIcon[11].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + 200 + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[11].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[11].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[11].bUse = (GetWinner() == 2) ? true : false;
	}

	//4P
	g_KDWIcon[12].pos = D3DXVECTOR3( POLYGON_P1_POS_X, POLYGON_P1_POS_Y + 300, 0.0f);
	g_KDWIcon[12].size = D3DXVECTOR3( POLYGON_PN_SIZE_X, POLYGON_PN_SIZE_Y, 0.0f);
	g_KDWIcon[12].col = D3DCOLOR_RGBA(0,0,255,255); //blue
	{
		//kill
		g_KDWIcon[13].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 200, POLYGON_P1_POS_Y + 300, 0.0f);
		g_KDWIcon[13].size = D3DXVECTOR3( POLYGON_KILL_SIZE_X, POLYGON_KILL_SIZE_Y, 0.0f);
		g_KDWIcon[13].col = D3DCOLOR_RGBA(0,0,255,255); //blue

		//death
		g_KDWIcon[14].pos = D3DXVECTOR3( POLYGON_P1_POS_X + 550, POLYGON_P1_POS_Y + 300, 0.0f);
		g_KDWIcon[14].size = D3DXVECTOR3( POLYGON_DEATH_SIZE_X, POLYGON_DEATH_SIZE_Y, 0.0f);
		g_KDWIcon[14].col = D3DCOLOR_RGBA(0,0,255,255); //blue

		//win
		g_KDWIcon[15].pos = D3DXVECTOR3( POLYGON_P1_POS_X - 100 + POLYGON_WIN_SIZE_X/2, POLYGON_P1_POS_Y + 300 + POLYGON_WIN_SIZE_Y/2, 0.0f);
		g_KDWIcon[15].size = D3DXVECTOR3( POLYGON_WIN_SIZE_X, POLYGON_WIN_SIZE_Y, 0.0f);
		g_KDWIcon[15].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		g_KDWIcon[15].bUse = (GetWinner() == 3) ? true : false;
	}
	


	//���_���̍쐬
	if(FAILED(MakeVertexResultKDWTex(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P1, &g_pTextureResultKDWTex[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[2]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[3]);
															
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P2, &g_pTextureResultKDWTex[4]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[5]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[6]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[7]);
															
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P3, &g_pTextureResultKDWTex[8]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[9]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[10]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[11]);
															
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_P4, &g_pTextureResultKDWTex[12]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_KILL, &g_pTextureResultKDWTex[13]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_DEATH, &g_pTextureResultKDWTex[14]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TEX_CROWN, &g_pTextureResultKDWTex[15]);



	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawResultKDWTex(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferResultKDWTex, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++)
	{
		if(g_KDWIcon[nCntIcon].bUse)
		{

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResultKDWTex[nCntIcon]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntIcon*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);				
		}
	
	}


}
/*******************************************************************************
�֐���:	void UninitBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̊J���֐�
*******************************************************************************/
void UninitResultKDWTex(void)
{
	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++)
	{
		if(g_pTextureResultKDWTex[nCntIcon] != NULL)
		{
			g_pTextureResultKDWTex[nCntIcon]->Release();
			g_pTextureResultKDWTex[nCntIcon] = NULL;
		}
	}

	if(g_pVtxBufferResultKDWTex != NULL)
	{
		g_pVtxBufferResultKDWTex->Release();
		g_pVtxBufferResultKDWTex = NULL;
	}	


}
/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexResultKDWTex(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_KDW_ICON,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferResultKDWTex,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferResultKDWTex->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntIcon = 0; nCntIcon < MAX_KDW_ICON; nCntIcon++,pVtx+=4)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3( g_KDWIcon[nCntIcon].pos.x, g_KDWIcon[nCntIcon].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_KDWIcon[nCntIcon].pos.x + g_KDWIcon[nCntIcon].size.x, g_KDWIcon[nCntIcon].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_KDWIcon[nCntIcon].pos.x, g_KDWIcon[nCntIcon].pos.y + g_KDWIcon[nCntIcon].size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_KDWIcon[nCntIcon].pos.x + g_KDWIcon[nCntIcon].size.x, g_KDWIcon[nCntIcon].pos.y + g_KDWIcon[nCntIcon].size.y, 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_KDWIcon[nCntIcon].col;
		pVtx[1].col = g_KDWIcon[nCntIcon].col;
		pVtx[2].col = g_KDWIcon[nCntIcon].col;
		pVtx[3].col = g_KDWIcon[nCntIcon].col;

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferResultKDWTex->Unlock();

	return S_OK;
}

void UpdateResultKDWTex(void)
{
	for(int nCntCrown = 3; nCntCrown < MAX_KDW_ICON; nCntCrown+=4) //3 7 11 15�̎l��
	{
		if(g_KDWIcon[nCntCrown].bUse)
		{
			//change size
			g_KDWIcon[nCntCrown].size.x += sin(g_angle3)*0.5f;
			g_KDWIcon[nCntCrown].size.y += sin(g_angle3)*0.5f;
			g_angle3 += 0.1f;
			if(g_angle3 >= 6.28) g_angle3 = 0;

			SetVertexResultKDWTex(nCntCrown);	
		}	
	}
}

void SetVertexResultKDWTex(int nIconIdx)
{

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferResultKDWTex->Lock( 0, 0, (void**)&pVtx, 0);

	pVtx += nIconIdx * NUM_VERTEX; 

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x - (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y - (g_KDWIcon[nIconIdx].size.y/2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x + (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y - (g_KDWIcon[nIconIdx].size.y/2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x - (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y + (g_KDWIcon[nIconIdx].size.y/2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_KDWIcon[nIconIdx].pos.x + (g_KDWIcon[nIconIdx].size.x/2), g_KDWIcon[nIconIdx].pos.y + (g_KDWIcon[nIconIdx].size.y/2), 0.0f);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferResultKDWTex->Unlock();
}


