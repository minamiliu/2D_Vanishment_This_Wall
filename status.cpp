//******************************************************************************
//
// �^�C�g��:		�X�e�[�^�X����
// �v���O������:	status.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "status.h"
#include "player.h"
#include "bullet.h"
#include "map.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(-100)	//�|���S���̍���X
#define POLYGON_POS_Y	(-400)	//�|���S���̍���Y
#define POLYGON_SIZE_X	(50)	//�|���S����SIZE X
#define POLYGON_SIZE_Y	(50)	//�|���S����SIZE Y
#define MAX_OBJECT		(40)		// �I�u�W�F�N�g�̐�

#define POLYGON_TEXTURENAME_STATUS_GUARD		"data/TEXTURE/STATUS/status_guard.png"
#define POLYGON_TEXTURENAME_STATUS_BOMBING		"data/TEXTURE/STATUS/status_bone.png" 
#define POLYGON_TEXTURENAME_STATUS_LEFTRIGHT	"data/TEXTURE/STATUS/status_leftright.png"
#define POLYGON_TEXTURENAME_STATUS_INVISIBLE	"data/TEXTURE/STATUS/status_invisible.png"
#define POLYGON_TEXTURENAME_STATUS_BOMB			"data/TEXTURE/STATUS/status_bomb.png"
#define POLYGON_TEXTURENAME_STATUS_FIRE			"data/TEXTURE/STATUS/status_fire.png"
#define POLYGON_TEXTURENAME_STATUS_SKATE		"data/TEXTURE/STATUS/status_skate.png"
#define POLYGON_TEXTURENAME_STATUS_SCORE		"data/TEXTURE/STATUS/status_score.png"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexStatus(LPDIRECT3DDEVICE9 pDevice);
void SetVertexStatus(int nCntStatus);
void SetColorStatus(int nCntStatus,int R, int G, int B, int A);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferStatus = NULL;
LPDIRECT3DTEXTURE9 g_pTextureStatus[STATUS_MAX] = {NULL};

STATUS_OBJECT	g_status[MAX_OBJECT];

bool change_flag;


/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitStatus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		g_status[nCntStatus].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_status[nCntStatus].size = D3DXVECTOR3(POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_status[nCntStatus].type = STATUS_NONE;
		g_status[nCntStatus].bUse = false;
		g_status[nCntStatus].who_get = -1;
	}

	change_flag = false;
	

	//���_���̍쐬
	if(FAILED(MakeVertexStatus(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_GUARD, &g_pTextureStatus[STATUS_GUARD]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_BOMBING, &g_pTextureStatus[STATUS_BOMBING]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_LEFTRIGHT, &g_pTextureStatus[STATUS_LEFTRIGHT]); 
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_INVISIBLE, &g_pTextureStatus[STATUS_INVISIBLE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_INVISIBLE, &g_pTextureStatus[STATUS_HURT]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_BOMB, &g_pTextureStatus[STATUS_ADDBOMB]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_FIRE, &g_pTextureStatus[STATUS_ADDFIRE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_SKATE, &g_pTextureStatus[STATUS_ADDSKATE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_STATUS_SCORE, &g_pTextureStatus[STATUS_ADDSCORE]);

	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawStatus(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferStatus, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		if(g_status[nCntStatus].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureStatus[g_status[nCntStatus].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntStatus*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
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
void UninitStatus(void)
{
	for(int nCnt = 0; nCnt < STATUS_MAX; nCnt++)
	{
		if(g_pTextureStatus[nCnt] != NULL)
		{
			g_pTextureStatus[nCnt]->Release();
			g_pTextureStatus[nCnt] = NULL;
		}
	}

	if(g_pVtxBufferStatus != NULL)
	{
		g_pVtxBufferStatus->Release();
		g_pVtxBufferStatus = NULL;
	}	


}
/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexStatus(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferStatus,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferStatus->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++,pVtx+=4)
	{

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


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



	g_pVtxBufferStatus->Unlock();

	return S_OK;
}

/*******************************************************************************
�֐���:	void SetVertexPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���_���W�̐ݒ�
*******************************************************************************/
void SetVertexStatus(int nCntStatus)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferStatus->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntStatus * NUM_VERTEX; 

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x - (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_status[nCntStatus].pos.x + (POLYGON_SIZE_X/2), g_status[nCntStatus].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		
		

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferStatus->Unlock();
	}
}

void UpdateStatus(void)
{
	PLAYER *pPlayer = GetPlayer();
	MAP *pMap = GetMap();	

	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		if(g_status[nCntStatus].bUse  )
		{
			//if player died
			if(!pPlayer[g_status[nCntStatus].who_get].bUse) g_status[nCntStatus].bUse = false;

			//Counter �J�E���g�_�E��
			pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type]--;

			switch(g_status[nCntStatus].type)
			{
				case STATUS_ADDBOMB:
				case STATUS_ADDSCORE:
				case STATUS_ADDSKATE:
				case STATUS_ADDFIRE:
					
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
					g_status[nCntStatus].pos.y -= pPlayer[g_status[nCntStatus].who_get].size.y;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 0);
						}
						else
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[g_status[nCntStatus].type] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[g_status[nCntStatus].type]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
					}
					break;
				case STATUS_GUARD:

					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 5 == 0)
						{
							change_flag = !change_flag;				
						}
						if(change_flag)
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 0);
						}
						else
						{
							SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
						}
						
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_GUARD] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_GUARD]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_UNDEAD] = false;
						SetColorStatus(nCntStatus, 255 ,255 ,255, 255);
					}
					break;

				case STATUS_BOMBING:

					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
					g_status[nCntStatus].pos.y -= pPlayer[g_status[nCntStatus].who_get].size.y;

					//���e������ɐݒu�����
					if(pPlayer[g_status[nCntStatus].who_get].bUseBomb < pPlayer[g_status[nCntStatus].who_get].bombNum && pMap->item[(int)Pos2DotPos(pPlayer[g_status[nCntStatus].who_get].pos).y][(int)Pos2DotPos(pPlayer[g_status[nCntStatus].who_get].pos).x].type == ITEM_NONE)
					{
						D3DXVECTOR3 tPos = DotPos2Pos(Pos2DotPos(pPlayer[g_status[nCntStatus].who_get].pos));
						SetBullet( g_status[nCntStatus].who_get, tPos, pPlayer[g_status[nCntStatus].who_get].power);
						pPlayer[g_status[nCntStatus].who_get].bUseBomb++;
					}

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 0, 255, 0, 255);

							//SE
							SetSE(SOUND_LABEL_SE_POISON);
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_BOMBING] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_BOMBING]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
					}
					
					break;

				case STATUS_LEFTRIGHT:
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
					g_status[nCntStatus].pos.y -= pPlayer[g_status[nCntStatus].who_get].size.y;


					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 0, 255, 0, 255);

							//SE
							SetSE(SOUND_LABEL_SE_POISON);
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_LEFTRIGHT] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_LEFTRIGHT]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
					}
					break;
				case STATUS_INVISIBLE:
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 120 == 0) //120frame ������Ǝp������
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 200);

							//SE
							SetSE(SOUND_LABEL_SE_POISON);
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 0);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_INVISIBLE] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_INVISIBLE]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
					}
					break;
				case STATUS_HURT:
					//status icon
					g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;

					if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] > 0)
					{
						if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] % 10 == 0)
						{
							change_flag = !change_flag;				
						}

						if(change_flag)
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 0, 0, 255); //��
						}
						else
						{
							SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						}
					}
					else if(pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] == 0)
					{
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_HURT] = false;
						ClearStatus(pPlayer[g_status[nCntStatus].who_get].statusIndex[STATUS_HURT]);
						pPlayer[g_status[nCntStatus].who_get].nCounterStatus[g_status[nCntStatus].type] = -1;
						SetColorPlayer(g_status[nCntStatus].who_get, 255, 255, 255, 255);
						pPlayer[g_status[nCntStatus].who_get].status[STATUS_UNDEAD] = false;
					}
					break;
			}
			//���_���W�̐ݒ�
			SetVertexStatus(nCntStatus);
		}
	}
}



void SetStatus(int nPlayerIdx, STATUS type)
{
	PLAYER *pPlayer = GetPlayer();


	for(int nCntStatus = 0; nCntStatus < MAX_OBJECT; nCntStatus++)
	{
		if(!g_status[nCntStatus].bUse) 
		{
			//�ʒu��ݒ�			
			g_status[nCntStatus].pos = pPlayer[g_status[nCntStatus].who_get].pos;
			g_status[nCntStatus].type = type;
			g_status[nCntStatus].bUse = true;
			g_status[nCntStatus].who_get = nPlayerIdx;
			
			//player status �ύX
			pPlayer[nPlayerIdx].status[type] = true;
			pPlayer[nPlayerIdx].statusIndex[type] = nCntStatus;
			break;
		}
	}
}

void ClearStatus(int nIdx)
{
	
	g_status[nIdx].bUse = false;
	g_status[nIdx].type = STATUS_NONE;
}

void SetColorStatus(int nCntStatus,int R, int G, int B, int A)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferStatus->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntStatus * NUM_VERTEX; 

		//color
		pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
		pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
		pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
		pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);
		

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferStatus->Unlock();
	}
}






