//******************************************************************************
//
// �^�C�g��:		�u���b�N����
// �v���O������:	block.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "block.h"
#include "map.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//�|���S���̍���X
#define POLYGON_POS_Y	(100)//�|���S���̍���Y
#define POLYGON_SIZE_X	(50)//�|���S����SIZE X
#define POLYGON_SIZE_Y	(50)//�|���S����SIZE Y
#define POLYGON_TEXTURENAME_GRASS	"data/TEXTURE/BG/grass.png"
#define POLYGON_TEXTURENAME_PILLAR	"data/TEXTURE/BG/pillar.png"
#define POLYGON_TEXTURENAME_WALL	"data/TEXTURE/BG/wall.png"

#define MAX_BLOCK		(MAP_SIZE_Y*MAP_SIZE_X)		// �I�u�W�F�N�g�̐�


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBlock = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCK_TYPE_MAX] = {NULL};

BLOCK	g_block[MAX_BLOCK];



/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitBlock()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	pMap->wallListIdx = 0;
	
	int nCntBlock = 0; 
	for(int nCntR = 0; nCntR < MAP_SIZE_Y; nCntR++)
	{
		for(int nCntC = 0; nCntC < MAP_SIZE_X; nCntC++)
		{

			//BLOCK�̏����� ���W�͍���
			g_block[nCntBlock].pos = D3DXVECTOR3( nCntC*POLYGON_SIZE_X + POLYGON_POS_X, nCntR*POLYGON_SIZE_Y + POLYGON_POS_Y, 0.0f);
			g_block[nCntBlock].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);

			switch(pMap->data[nCntR][nCntC])
			{
				case BLOCK_GRASS:
					g_block[nCntBlock].type = BLOCK_GRASS;
					break;
				case BLOCK_PILLAR:
					g_block[nCntBlock].type = BLOCK_PILLAR;	
					break;
				case BLOCK_WALL:
					g_block[nCntBlock].type = BLOCK_WALL;
					pMap->wallPosList[pMap->wallListIdx].x = g_block[nCntBlock].pos.x + POLYGON_SIZE_X/2;
					pMap->wallPosList[pMap->wallListIdx].y = g_block[nCntBlock].pos.y + POLYGON_SIZE_Y/2;
					pMap->bUseWallList[pMap->wallListIdx] = false;
					pMap->wallListIdx++;
					break;
			} 				
			nCntBlock++;			
		}
	}

	//���_���̍쐬
	if(FAILED(MakeVertexBlock(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_GRASS, &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_PILLAR, &g_pTextureBlock[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_WALL, &g_pTextureBlock[2]);


	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawBlock(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBlock[g_block[nCntBlock].type]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntBlock*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
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
void UninitBlock(void)
{
	for(int nCnt = 0; nCnt < BLOCK_TYPE_MAX; nCnt++)
	{
		if(g_pTextureBlock[nCnt] != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}

	if(g_pVtxBufferBlock != NULL)
	{
		g_pVtxBufferBlock->Release();
		g_pVtxBufferBlock = NULL;
	}	


}
/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexBlock(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BLOCK,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBlock,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferBlock->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pVtx+=4)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x, g_block[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x + POLYGON_SIZE_X, g_block[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x, g_block[nCntBlock].pos.y + POLYGON_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_block[nCntBlock].pos.x + POLYGON_SIZE_X, g_block[nCntBlock].pos.y + POLYGON_SIZE_Y, 0.0f);


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
	}



	g_pVtxBufferBlock->Unlock();

	return S_OK;
}

BLOCK *GetBlock(void)
{
	return g_block;
}

