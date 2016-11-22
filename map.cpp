//******************************************************************************
//
// �^�C�g��:		�X�e�[�W�}�b�v�̓ǂݍ��ݏ���
// �v���O������:	map.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "map.h"
#include "block.h"
#include <stdio.h>
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define NUM_VERTEX (4)
//#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//�|���S���̍���X
#define POLYGON_POS_Y	(100)//�|���S���̍���Y
#define POLYGON_SIZE_X	(50)//�|���S����SIZE X
#define POLYGON_SIZE_Y	(50)//�|���S����SIZE Y


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
//*******************************************************************************/
void LoadMapFile(void);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMap = NULL;
//LPDIRECT3DTEXTURE9 g_pTextureMap[BLOCK_TYPE_MAX] = {NULL};

MAP g_map[MAX_STAGE];

/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitMap(void)
{
	//load map file
	LoadMapFile();

	InitBlock();


	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawMap(void)
{
	DrawBlock();
}
/*******************************************************************************
�֐���:	void UninitBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̊J���֐�
*******************************************************************************/
void UninitMap(void)
{
	UninitBlock();
}


MAP *GetMap(void)
{
	int stage = GetStage();
	return &g_map[stage];
}

D3DXVECTOR3 Pos2DotPos(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 dotPos;
	dotPos.x = (int)((pos.x / POLYGON_SIZE_X));
	dotPos.y = (int)((pos.y -  POLYGON_POS_Y)/POLYGON_SIZE_Y);
	return dotPos;
}

D3DXVECTOR3 DotPos2Pos(D3DXVECTOR3 dotPos)
{
	D3DXVECTOR3 pos = D3DXVECTOR3( dotPos.x * POLYGON_SIZE_X + POLYGON_SIZE_X/2, dotPos.y * POLYGON_SIZE_Y + POLYGON_SIZE_Y/2 + POLYGON_POS_Y, 0.0f);
	return pos;
}

void ChangeMap(D3DXVECTOR3 dotPos, BLOCK_TYPE b_type)
{
	int stage = GetStage();
	BLOCK *pBlock = GetBlock();  
	g_map[stage].data[(int)dotPos.y][(int)dotPos.x] = b_type;
	pBlock[(int)(dotPos.y * MAP_SIZE_X + dotPos.x)].type = b_type;
}

void LoadMapFile(void)
{
	FILE *fp;
	fp = fopen( "data/FILE/stage.txt","r" );

	for(int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		for(int nCntR = 0; nCntR < MAP_SIZE_Y; nCntR++)
		{
			for(int nCntC = 0; nCntC < MAP_SIZE_X; nCntC++)
			{
				//MAP�\���̂̏�����
				fscanf(fp,"%d ",&g_map[nCntStage].data[nCntR][nCntC]);		
			}
		}	
	}

	fclose(fp);
}






