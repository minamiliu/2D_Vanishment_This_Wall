//******************************************************************************
//
// �^�C�g��:		�A�C�e������
// �v���O������:	item.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "item.h"
#include "map.h"
#include "player.h"
#include "score.h"
#include "status.h"
#include "input.h"
#include "sound.h"
#include "enemy.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(-100)	//�|���S���̍���X
#define POLYGON_POS_Y	(-300)	//�|���S���̍���Y
#define POLYGON_SIZE_X	(50)	//�|���S����SIZE X
#define POLYGON_SIZE_Y	(50)	//�|���S����SIZE Y
#define POLYGON_TEXTURENAME_ITEM_BOMB		"data/TEXTURE/ITEM/item_bomb.png"
#define POLYGON_TEXTURENAME_ITEM_FIRE		"data/TEXTURE/ITEM/item_fire.png"
#define POLYGON_TEXTURENAME_ITEM_SKATE		"data/TEXTURE/ITEM/item_skate.png"
#define POLYGON_TEXTURENAME_ITEM_DIAMOND	"data/TEXTURE/ITEM/item_diamond.png"
#define POLYGON_TEXTURENAME_ITEM_NEXT		"data/TEXTURE/ITEM/item_next.png"
#define POLYGON_TEXTURENAME_ITEM_GUARD		"data/TEXTURE/ITEM/item_guard.png"
#define POLYGON_TEXTURENAME_ITEM_QUESTION	"data/TEXTURE/ITEM/item_question.png"
#define POLYGON_TEXTURENAME_ITEM_BONE		"data/TEXTURE/ITEM/item_bone.png"
#define POLYGON_TEXTURENAME_ITEM_GRAVE		"data/TEXTURE/ITEM/item_grave.png"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
bool CollisionCheckItemPlayer(int nItemIdx, int nPlayerIdx);
bool CollisionCheckItemEnemy(int nItemIdx, int nEnemyIdx);
void SetVertexItem(int nItemIdx);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferItem = NULL;
LPDIRECT3DTEXTURE9 g_pTextureItem[ITEM_TYPE_MAX] = {NULL};

ITEM	g_item[MAX_ITEM];
int g_nNumItem;

int g_nItemType[ITEM_TYPE_MAX] = 
	{	0,	//ITEM_NONE = 0,
		5,	//ITEM_BOMB,
		3,	//ITEM_FIRE,
		3,	//ITEM_SKATE,
		1,	//ITEM_DIAMOND,
		2,	//ITEM_GUARD,
		5,	//ITEM_BONE,
		5,	//ITEM_QUESTION,
		1}; //ITEM_NEXT,
	

/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *g_map = GetMap(); 

	//Item Map �̏�����
	for(int nCntR = 0; nCntR < MAP_SIZE_Y; nCntR++)
	{
		for(int nCntC = 0; nCntC < MAP_SIZE_X; nCntC++)
		{
			//MAP�\���̂̏�����
			g_map->item[nCntR][nCntC].type = ITEM_NONE;
		}
	}


	//if battle mode
	GAME_MODE mode = GetGameMode();
	switch(mode)
	{
		case GAME_COOPERATE:

			//if BOSS stage
			if(GetStage() == 2) //stage 3
			{ 
				g_nItemType[ITEM_NEXT] = 0; //No NEXT
			}
			else
			{
				g_nItemType[ITEM_NEXT] = 1; //YES NEXT
			}
			break;
		case GAME_BATTLE:
			
			g_nItemType[ITEM_NEXT] = 0; //No NEXT
			break;
	}

	//Item�̏�����
	g_nNumItem = 0;
	for(int nCntType = 0; nCntType < ITEM_TYPE_MAX; nCntType++)
	{

		for(int nCntNum = 0; nCntNum < g_nItemType[nCntType]; nCntNum++)
		{
			int randNum;
			do
			{
				randNum = rand() % g_map->wallListIdx;
			}while(g_map->bUseWallList[randNum]);
			
			g_item[g_nNumItem].type = (ITEM_TYPE)nCntType;
			g_map->item[(int)Pos2DotPos(g_map->wallPosList[randNum]).y][(int)Pos2DotPos(g_map->wallPosList[randNum]).x].type = g_item[g_nNumItem].type;
			g_map->item[(int)Pos2DotPos(g_map->wallPosList[randNum]).y][(int)Pos2DotPos(g_map->wallPosList[randNum]).x].index = g_nNumItem;
			g_map->bUseWallList[randNum] = true;
			g_item[g_nNumItem].pos = g_map->wallPosList[randNum];
			g_item[g_nNumItem].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
			g_item[g_nNumItem].visible = false;
			g_item[g_nNumItem].nCounterVisible = -1;
			g_nNumItem++;
		}
	}


	

	//���_���̍쐬
	if(FAILED(MakeVertexItem(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_BOMB, &g_pTextureItem[ITEM_BOMB]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_FIRE, &g_pTextureItem[ITEM_FIRE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_SKATE, &g_pTextureItem[ITEM_SKATE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_DIAMOND, &g_pTextureItem[ITEM_DIAMOND]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_NEXT, &g_pTextureItem[ITEM_NEXT]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_GUARD, &g_pTextureItem[ITEM_GUARD]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_QUESTION, &g_pTextureItem[ITEM_QUESTION]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_BONE, &g_pTextureItem[ITEM_BONE]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_ITEM_GRAVE, &g_pTextureItem[ITEM_GRAVE]);

	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawItem(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferItem, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntItem = 0; nCntItem < g_nNumItem; nCntItem++)
	{
		if(g_item[nCntItem].visible)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItem[g_item[nCntItem].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntItem*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
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
void UninitItem(void)
{
	for(int nCnt = 0; nCnt < ITEM_TYPE_MAX; nCnt++)
	{
		if(g_pTextureItem[nCnt] != NULL)
		{
			g_pTextureItem[nCnt]->Release();
			g_pTextureItem[nCnt] = NULL;
		}
	}

	if(g_pVtxBufferItem != NULL)
	{
		g_pVtxBufferItem->Release();
		g_pVtxBufferItem = NULL;
	}	


}
/*******************************************************************************
�֐���:	HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ITEM,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferItem,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferItem->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pVtx+=4)
	{

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_item[nCntItem].pos.x - (POLYGON_SIZE_X/2), g_item[nCntItem].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_item[nCntItem].pos.x + (POLYGON_SIZE_X/2), g_item[nCntItem].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_item[nCntItem].pos.x - (POLYGON_SIZE_X/2), g_item[nCntItem].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_item[nCntItem].pos.x + (POLYGON_SIZE_X/2), g_item[nCntItem].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


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



	g_pVtxBufferItem->Unlock();

	return S_OK;
}

void UpdateItem(void)
{
	PLAYER *pPlayer = GetPlayer();
	MAP *pMap = GetMap();

	for(int nCntItem = 0; nCntItem < g_nNumItem; nCntItem++)
	{
		for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if(pPlayer[nCntPlayer].bUse)
			{
				//�A�C�e�����v���[���[�Ɠ���������
				if(CollisionCheckItemPlayer(nCntItem, nCntPlayer))
				{
					//��O�̔�A�C�e��
					if(g_item[nCntItem].type == ITEM_GRAVE) continue;
					if(g_item[nCntItem].type == ITEM_NEXT)
					{
						SetGameStep(STEP_CLEAR);
						SetMessage(MSG_STAGECLEAR);

						//SE
						SetSE(SOUND_LABEL_SE_GAMESTART);
						break;
					}

					g_item[nCntItem].visible = false;
					pMap->item[(int)Pos2DotPos(g_item[nCntItem].pos).y][(int)Pos2DotPos(g_item[nCntItem].pos).x].type = ITEM_NONE;

					//��O��ITEM_QUESTION
					if(g_item[nCntItem].type == ITEM_QUESTION)
					{
						g_item[nCntItem].type = ITEM_TYPE(rand() % (ITEM_QUESTION - 1) + 1);
					}

					//SE
					SetSE(SOUND_LABEL_SE_GETITEM);

					switch(g_item[nCntItem].type)
					{
						case ITEM_BOMB:
							pPlayer[nCntPlayer].bombNum++;
							SetStatus(nCntPlayer, STATUS_ADDBOMB);
							pPlayer[nCntPlayer].nCounterStatus[STATUS_ADDBOMB] = FRAME_STATUS_GET_ITEM;
							break;
						case ITEM_FIRE:
							pPlayer[nCntPlayer].power++;
							SetStatus(nCntPlayer, STATUS_ADDFIRE);
							pPlayer[nCntPlayer].nCounterStatus[STATUS_ADDFIRE] = FRAME_STATUS_GET_ITEM;
							break;
						case ITEM_SKATE:
							pPlayer[nCntPlayer].move.x++;
							pPlayer[nCntPlayer].move.y++;
							SetStatus(nCntPlayer, STATUS_ADDSKATE);
							pPlayer[nCntPlayer].nCounterStatus[STATUS_ADDSKATE] = FRAME_STATUS_GET_ITEM;
							break;
						case ITEM_DIAMOND:
							ChangeScore(3000);
							SetStatus(nCntPlayer, STATUS_ADDSCORE);
							pPlayer[nCntPlayer].nCounterStatus[STATUS_ADDSCORE] = FRAME_STATUS_GET_ITEM;
							break;
						case ITEM_GUARD:
							if(!pPlayer[nCntPlayer].status[STATUS_GUARD])
							{
								SetStatus(nCntPlayer, STATUS_GUARD);	
							}
							break;
						case ITEM_BONE:
							STATUS tStatus = (STATUS)(rand() % 3 + 1); //ITEM_BONE�͂R��ނ�����
							if(!pPlayer[nCntPlayer].status[tStatus])
							{
								SetStatus(nCntPlayer, tStatus);	

								//���ʎ���Frame�ݒ�
								switch(tStatus)
								{
									case STATUS_BOMBING:
										pPlayer[nCntPlayer].bombNum += 1;
										pPlayer[nCntPlayer].nCounterStatus[STATUS_BOMBING] = FRAME_STATUS_BOMBING;
										break;
									case STATUS_LEFTRIGHT:
										pPlayer[nCntPlayer].move.x += 1;
										pPlayer[nCntPlayer].move.y += 1;
										pPlayer[nCntPlayer].nCounterStatus[STATUS_LEFTRIGHT] = FRAME_STATUS_LEFTRIGHT;
										break;
									case STATUS_INVISIBLE:
										pPlayer[nCntPlayer].nCounterStatus[STATUS_INVISIBLE] = FRAME_STATUS_INVISIBLE;
										break;
								}
							}
							break;
					}
					g_item[nCntItem].type = ITEM_NONE;
				}	
			}
	
		}
		//�A�C�e�����G�ƓG����������
		for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if(g_item[nCntItem].type == ITEM_GRAVE) continue;
			if(CollisionCheckItemEnemy(nCntItem, nCntEnemy) && g_item[nCntItem].type != ITEM_NEXT )
			{//�A�C�e����������
				g_item[nCntItem].visible = false;
				pMap->item[(int)Pos2DotPos(g_item[nCntItem].pos).y][(int)Pos2DotPos(g_item[nCntItem].pos).x].type = ITEM_NONE;
			}
		}
		//�A�C�e���͔����I���܂ŁA����Ȃ�
		if(g_item[nCntItem].nCounterVisible != -1)
		{
			g_item[nCntItem].nCounterVisible--;
		}
		if(g_item[nCntItem].nCounterVisible == 0)
		{
			g_item[nCntItem].nCounterVisible = -1;
			g_item[nCntItem].visible = true;
		}

	}

	//���Z�@�A�C�e���̕\���X�E�B�b�`
	if(GetKeyboardTrigger(DIK_F3))
	{
		PlaySound(SOUND_LABEL_SE_SWITCH);
		for(int nCntItem = 0; nCntItem < g_nNumItem; nCntItem++)
		{
			if(g_item[nCntItem].type == ITEM_NONE) continue;
			if(pMap->data[(int)Pos2DotPos(g_item[nCntItem].pos).y][(int)Pos2DotPos(g_item[nCntItem].pos).x] == BLOCK_WALL)
			{
				g_item[nCntItem].visible = !g_item[nCntItem].visible;
			}
		}
	}

}

bool CollisionCheckItemPlayer(int nItemIdx, int nPlayerIdx)
{
	PLAYER *pPlayer = GetPlayer();

	if(g_item[nItemIdx].visible)
	{
		//�o�E���f�B���O�X�t�B�A
		if( (pPlayer[nPlayerIdx].pos.x - g_item[nItemIdx].pos.x)*(pPlayer[nPlayerIdx].pos.x - g_item[nItemIdx].pos.x) + (pPlayer[nPlayerIdx].pos.y - g_item[nItemIdx].pos.y)*(pPlayer[nPlayerIdx].pos.y - g_item[nItemIdx].pos.y) < (POLYGON_SIZE_X/2 + PLAYER_ATARI)*(POLYGON_SIZE_X/2 + PLAYER_ATARI) )
		{
			return true;
		}	
	}
		
	return false;
}

bool CollisionCheckItemEnemy(int nItemIdx, int nEnemyIdx)
{
	ENEMY *pEnemy = GetEnemy();

	if(g_item[nItemIdx].visible)
	{
		//�o�E���f�B���O�X�t�B�A
		if( (pEnemy[nEnemyIdx].pos.x - g_item[nItemIdx].pos.x)*(pEnemy[nEnemyIdx].pos.x - g_item[nItemIdx].pos.x) + (pEnemy[nEnemyIdx].pos.y - g_item[nItemIdx].pos.y)*(pEnemy[nEnemyIdx].pos.y - g_item[nItemIdx].pos.y) < (POLYGON_SIZE_X/2 + pEnemy[nEnemyIdx].size.x / 2)*(POLYGON_SIZE_X/2 + pEnemy[nEnemyIdx].size.y / 2) )
		{
			return true;
		}	
	}
		
	return false;
}

/*******************************************************************************
�֐���:	ITEM *GetItem(void)
����:	�Ȃ�
�߂�l: ITEM�FITEM�̍\����
����:	�����蔻��Ȃǂ̎��A�A�C�e���̏���ǂݎ��
*******************************************************************************/
ITEM *GetItem(void)
{
	return g_item;
}

void SetItem(ITEM_TYPE type, D3DXVECTOR3 dotPos)
{
	//MAP *g_map = GetMap();

	g_item[g_nNumItem].type = type;
	//g_map->item[(int)dotPos.y][(int)dotPos.x].type = g_item[g_nNumItem].type;
	//g_map->item[(int)dotPos.y][(int)dotPos.x].index = g_nNumItem;
	//g_map->bUseWallList[dotPos] = true;
	g_item[g_nNumItem].pos = DotPos2Pos(dotPos);
	g_item[g_nNumItem].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	g_item[g_nNumItem].visible = true;
	g_item[g_nNumItem].index = g_nNumItem;

	SetVertexItem(g_item[g_nNumItem].index);

	g_nNumItem++;
}

void SetVertexItem(int nItemIdx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferItem->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nItemIdx * NUM_VERTEX; 

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_item[nItemIdx].pos.x - (POLYGON_SIZE_X/2), g_item[nItemIdx].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_item[nItemIdx].pos.x + (POLYGON_SIZE_X/2), g_item[nItemIdx].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_item[nItemIdx].pos.x - (POLYGON_SIZE_X/2), g_item[nItemIdx].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_item[nItemIdx].pos.x + (POLYGON_SIZE_X/2), g_item[nItemIdx].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferItem->Unlock();
	}
}


