//******************************************************************************
//
// �����ȥ�:		���ϥ⡼�ɤΥܥ�����
// �ץ����̾:	boss.cpp
// ������:			HAL���������زʡ�έ�
//
//******************************************************************************

/*******************************************************************************
* ���󥯥롼�ɥե�����
*******************************************************************************/

#include "boss.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "map.h"
#include "sound.h"
#include "game.h"

//*****************************************************************************
// �ޥ������
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/boss0001.png"		// �ɤ߹���ƥ�������ե�����̾
#define	POLYGON_DOTPOS_X			(0)								// �ݥꥴ��δ�����(�غ�ɸ)
#define	POLYGON_DOTPOS_Y			(0)								// �ݥꥴ��δ�����(�ٺ�ɸ)
#define	POLYGON_SIZE_X				(400.0f)							// �ݥꥴ�����
#define	POLYGON_SIZE_Y				(400.0f)							// �ݥꥴ��ι⤵

#define	TEX_PATTERN_DIVIDE_X		(3)								// ���˥᡼�����ѥ�����Υƥ���������Ǥ�ʬ���(������)
#define	TEX_PATTERN_DIVIDE_Y		(4)								// ���˥᡼�����ѥ�����Υƥ���������Ǥ�ʬ���(������)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// ���ѥ�����Υƥ������㥵����(������)(1.0f/X����ʬ���)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// ���ѥ�����Υƥ������㥵����(������)(1.0f/Y����ʬ���)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// ���˥᡼�����Υѥ������(X����ʬ�����Y����ʬ���)
#define	TIME_CHANGE_PATTERN			(10)							// ���˥᡼�������ڤ��ؤ�륿���ߥ�(�ե졼���)

#define	VALUE_MOVE					(1.0f)							// �ݥꥴ��ΰ�ư��



/*******************************************************************************
* ��¤�����
*******************************************************************************/

/*******************************************************************************
* �ץ�ȥ��������
*******************************************************************************/
HRESULT MakeVertexBoss(LPDIRECT3DDEVICE9 pDevice);
void SetTextureBoss(int nCntBoss, int nPatternAnim, DIRECTION dir);
void SetVertexBoss(int nCntBoss);

/*******************************************************************************
* �����Х��ѿ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBoss = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBoss = NULL;

BOSS	g_boss[MAX_BOSS]; //�ץ쥤�䡼�Υ��
bool hurt_flag;

/*******************************************************************************
�ؿ�̾:	HRESULT InitEnemy(void)
����:	�ʤ�
�����:	HRESUL : �������� ���ｪλ:S_OK
����:	���������
*******************************************************************************/
HRESULT InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		//enemy�ν����
		int randX, randY;
		do
		{
			randX = rand()%5 + 10;
			randY = rand()%5 + 5;
		}while(pMap->data[randY][randX] != BLOCK_GRASS);

		g_boss[nCntBoss].dotPos = D3DXVECTOR3( randX, randY, 0.0f);

		g_boss[nCntBoss].pos = DotPos2Pos(g_boss[nCntBoss].dotPos);
		g_boss[nCntBoss].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_boss[nCntBoss].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_boss[nCntBoss].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_boss[nCntBoss].bUse = false;	
		g_boss[nCntBoss].dir = D_DOWN;
		g_boss[nCntBoss].walking = false;
		g_boss[nCntBoss].life = 15;
		g_boss[nCntBoss].nCounterHurt = -1;
		g_boss[nCntBoss].nCounterDeath = -1;
	}
	//BOSS stage
	int stage = GetStage();
	if(stage == 2) //stage 3
	{ 
		g_boss[0].bUse = true;
	}

	// ���˥᡼�����ν����
	hurt_flag = false;


	//ĺ������κ���
	if(FAILED(MakeVertexBoss(pDevice)))
	{
		return E_FAIL;
	}

	// �ƥ���������ɤ߹���
	D3DXCreateTextureFromFile(pDevice,					// �ǥХ����ؤΥݥ���
								TEXTURE_POLYGON,		// �ե������̾��
								&g_pTextureBoss);	// �ɤ߹�����꡼

	return S_OK;
}

/*******************************************************************************
�ؿ�̾:	void DrawEnemy(void)
����:	�ʤ�
�����:	�ʤ�
����:	�ݥꥴ�������ؿ�
*******************************************************************************/
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ĺ���Хåե���ǥХ����Υǡ������ȥ꡼��˥Х����
	pDevice->SetStreamSource(0, g_pVtxBufferBoss, 0, sizeof(VERTEX_2D));

	//ĺ���ե����ޥåȤ�����
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�ƥ������������
	pDevice->SetTexture(0, g_pTextureBoss);

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(g_boss[nCntBoss].bUse)
		{
			//�ݥꥴ�������
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�ץ�ߥƥ��֤μ���
				nCntBoss*NUM_VERTEX,	//���ɤ���ǽ��ĺ������ǥå���
				NUM_POLYGON				//�ݥꥴ��ο�
			);
		}	
	}



}

/*******************************************************************************
�ؿ�̾:	void UninitEnemy(void)
����:	�ʤ�
�����:	�ʤ�
����:	�ݥꥴ��γ����ؿ�
*******************************************************************************/
void UninitBoss(void)
{
	if(g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}
	if(g_pVtxBufferBoss != NULL)
	{
		g_pVtxBufferBoss->Release();
		g_pVtxBufferBoss = NULL;
	}

	//BOSS�����Ѥ��ʤ������ꤹ��
	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_boss[nCntBoss].bUse = false;	

	}
}

/*******************************************************************************
�ؿ�̾:	HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device���֥�������
�����:	HRESUL : �������� ���ｪλ:S_OK
����:	�ݥꥴ���ĺ������κ����ؿ�
*******************************************************************************/
HRESULT MakeVertexBoss(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BOSS,	//ĺ���ǡ����ΥХåե������� 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//ĺ���ե����ޥå�
		D3DPOOL_MANAGED, 
		&g_pVtxBufferBoss,			//ĺ���Хåե����󥿡��ե������Υݥ���
		NULL)))
	{
		return E_FAIL;
	}


	//ĺ���Хåե�����Ȥ�����
	VERTEX_2D *pVtx;

	//ĺ���ǡ������ϰϤ��å�����ĺ���Хåե��ؤΥݥ��󥿤����
	g_pVtxBufferBoss->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pVtx += NUM_VERTEX)
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

		// ĺ����ɸ������
		pVtx[0].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//�ƥ��������ɸ����
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	
	}



	g_pVtxBufferBoss->Unlock();

	return S_OK;
}


/*******************************************************************************
�ؿ�̾:	void UpdateEnemy(void)
����:	�ʤ�
�����:	�ʤ�
����:	��������
*******************************************************************************/
void UpdateBoss(void)
{	
	DIRECTION new_dir;
	MAP *pMap = GetMap();


	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(g_boss[nCntBoss].bUse)
		{

			//��ư����Ĵ��
			if(fabs(g_boss[nCntBoss].pos.x - DotPos2Pos(g_boss[nCntBoss].dotPos).x) < VALUE_MOVE && fabs(g_boss[nCntBoss].pos.y - DotPos2Pos(g_boss[nCntBoss].dotPos).y) < VALUE_MOVE )
			{
				g_boss[nCntBoss].pos = DotPos2Pos(g_boss[nCntBoss].dotPos);
				g_boss[nCntBoss].walking = false;
			}
			
			//���ΰ��ְ�ư������
			if(g_boss[nCntBoss].pos == DotPos2Pos(g_boss[nCntBoss].dotPos))
			{
				g_boss[nCntBoss].walking = false;
			}

			//��˴
			if(g_boss[nCntBoss].life == 0)
			{
				g_boss[nCntBoss].nCounterDeath = 120; //hurt anime 60s
				g_boss[nCntBoss].nCounterHurt = -2; //do not use
				g_boss[nCntBoss].life = -1;
				SetSE(SOUND_LABEL_SE_GAMESTART);
				break;
			}

			//if Boss Hurt
			if(g_boss[nCntBoss].nCounterHurt > 0)
			{
				g_boss[nCntBoss].nCounterHurt--;
				if(g_boss[nCntBoss].nCounterHurt % 10 == 0)
				{
					hurt_flag = !hurt_flag;				
				}
				if(hurt_flag)
				{
					SetColorBoss(nCntBoss, 255, 0, 0, 255); //��
				}
				else
				{
					SetColorBoss(nCntBoss, 255, 255, 255, 255);
				}
			}
			else if(g_boss[nCntBoss].nCounterHurt == 0)
			{
				SetColorBoss(nCntBoss, 255, 255, 255, 255);
				g_boss[nCntBoss].nCounterHurt = -1;
				hurt_flag = false;
			}


			//if Boss Died
			if(g_boss[nCntBoss].nCounterDeath > 0)
			{
				g_boss[nCntBoss].nCounterDeath--;
				if(g_boss[nCntBoss].nCounterDeath % 5 == 0)
				{
					hurt_flag = !hurt_flag;				
				}
				if(hurt_flag)
				{
					SetColorBoss(nCntBoss, 255, 0, 0, 255); //��
					SetSE(SOUND_LABEL_SE_BOSSHURT);
				}
				else
				{
					SetColorBoss(nCntBoss, 255, 255, 255, 255);
				}
			}
			else if(g_boss[nCntBoss].nCounterDeath == 0)
			{
				SetColorBoss(nCntBoss, 255, 255, 255, 0);
				g_boss[nCntBoss].nCounterDeath = -2;
				g_boss[nCntBoss].bUse = false;
				hurt_flag = false;
				SetItem(ITEM_NEXT,g_boss[nCntBoss].dotPos); //NEXT�򸽤�

			}

			//if BOSS died
			if(g_boss[nCntBoss].life == -1) break;

			if(g_boss[nCntBoss].walking)
			{
				switch(g_boss[nCntBoss].dir)
				{
					case D_UP:

						g_boss[nCntBoss].pos.y -= g_boss[nCntBoss].move.y;
						break;
					case D_DOWN:

						g_boss[nCntBoss].pos.y += g_boss[nCntBoss].move.y;
						break;
					case D_LEFT:

						g_boss[nCntBoss].pos.x -= g_boss[nCntBoss].move.x;
						break;
					case D_RIGHT:

						g_boss[nCntBoss].pos.x += g_boss[nCntBoss].move.x;
						break;
				}
			}
			else
			{
				//random��ư 
				new_dir = (DIRECTION)(rand()%4);
				
				// ��ư
				if(new_dir == D_UP)
				{
					g_boss[nCntBoss].dir = D_UP;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y - 1][(int)g_boss[nCntBoss].dotPos.x] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.y -= g_boss[nCntBoss].move.y;
						g_boss[nCntBoss].dotPos.y -= 1;		
						g_boss[nCntBoss].walking = true;
					}
				}
				if(new_dir == D_DOWN)
				{
					g_boss[nCntBoss].dir = D_DOWN;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y + 1][(int)g_boss[nCntBoss].dotPos.x] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.y += g_boss[nCntBoss].move.y;
						g_boss[nCntBoss].dotPos.y += 1;
						g_boss[nCntBoss].walking = true;
					}

				}
				if(new_dir == D_LEFT)
				{
					g_boss[nCntBoss].dir = D_LEFT;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y][(int)g_boss[nCntBoss].dotPos.x - 1] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.x -= g_boss[nCntBoss].move.x;
						g_boss[nCntBoss].dotPos.x -= 1;
						g_boss[nCntBoss].walking = true;
					}
				}
				if(new_dir == D_RIGHT)
				{
					g_boss[nCntBoss].dir = D_RIGHT;
					if(pMap->data[(int)g_boss[nCntBoss].dotPos.y][(int)g_boss[nCntBoss].dotPos.x + 1] == BLOCK_GRASS)
					{
						g_boss[nCntBoss].pos.x += g_boss[nCntBoss].move.x;
						g_boss[nCntBoss].dotPos.x += 1;
						g_boss[nCntBoss].walking = true;
					}
				}
			}


			SetVertexBoss(nCntBoss);


			//�����ѹ�
			SetTextureBoss(nCntBoss, g_boss[nCntBoss].nPatternAnim, g_boss[nCntBoss].dir);
	
			//���˥�ɽ��
			g_boss[nCntBoss].nCounterAnim++;
			if((g_boss[nCntBoss].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// �ѥ�������ڤ��ؤ�
				g_boss[nCntBoss].nPatternAnim = (g_boss[nCntBoss].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// �ƥ��������ɸ������
				SetTextureBoss(nCntBoss, g_boss[nCntBoss].nPatternAnim, g_boss[nCntBoss].dir);

				//nCounterAnim�Υꥻ�å�
				g_boss[nCntBoss].nCounterAnim = 0;
			}	
		}
	}
	
}

/*******************************************************************************
�ؿ�̾:	void SetVertexEnemy(void)
����:	�ʤ�
�����:	�ʤ�
����:	ĺ����ɸ������
*******************************************************************************/
void SetVertexBoss(int nCntBoss)
{
	{//ĺ���Хåե�����Ȥ�����
		VERTEX_2D *pVtx;

		// ĺ���ǡ������ϰϤ��å�����ĺ���Хåե��ؤΥݥ��󥿤����
		g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);
		
		pVtx += nCntBoss * NUM_VERTEX; 

		//ĺ����ɸ������
		pVtx[0].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x - (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_boss[nCntBoss].pos.x + (POLYGON_SIZE_X/2), g_boss[nCntBoss].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		// ĺ���ǡ����򥢥��å�����
		g_pVtxBufferBoss->Unlock();
	}
}

/*******************************************************************************
�ؿ�̾:	void SetTextureEnemy(int nPatternAnim)
����:	int nPatternAnim : ���˥᡼�����ѥ�����No.
�����:	�ʤ�
����:	�ƥ��������ɸ������
*******************************************************************************/
void SetTextureBoss(int nCntBoss, int nPatternAnim, DIRECTION dir)
{
	{//ĺ���Хåե�����Ȥ�����
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// ĺ���ǡ������ϰϤ��å�����ĺ���Хåե��ؤΥݥ��󥿤����
		g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntBoss * NUM_VERTEX; 

		// �ƥ��������ɸ������
		fPosXLeft	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X);
		fPosXRight	= TEX_PATTERN_SIZE_X * (nPatternAnim % TEX_PATTERN_DIVIDE_X + 1);
		fPosYUp		= TEX_PATTERN_SIZE_Y * dir;
		fPosYDown	= TEX_PATTERN_SIZE_Y * (dir + 1);


		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// ĺ���ǡ����򥢥��å�����
		g_pVtxBufferBoss->Unlock();
	}
}

/*******************************************************************************
�ؿ�̾:	void SetEnemy(D3DXVECTOR3 pos)
����:	D3DXVECTOR3 pos��Ũ�����֤������
�����:	�ʤ�
����:	Ũ������
*******************************************************************************/
void SetBoss(D3DXVECTOR3 pos)
{
	for(int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if(!g_boss[nCntBoss].bUse)
		{
			//���֤�����
			g_boss[nCntBoss].pos = pos;
			SetVertexBoss(nCntBoss);
			g_boss[nCntBoss].bUse = true; //ȯ������ѹ�
			break;
		}
	}

}

void SetColorBoss(int nCntBoss, int R, int G, int B, int A)
{
	//ĺ���Хåե�����Ȥ�����
	VERTEX_2D *pVtx;

	// ĺ���ǡ������ϰϤ��å�����ĺ���Хåե��ؤΥݥ��󥿤����
	g_pVtxBufferBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntBoss * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// ĺ���ǡ����򥢥��å�����
	g_pVtxBufferBoss->Unlock();
	
}



/*******************************************************************************
�ؿ�̾:	BULLET* GetBullet(void)
����:	�ʤ�
�����: BULLET*��BULLET�Υݥ���
����:	������Ƚ��ʤɤλ����Ƥξ�����ɤ߼��
*******************************************************************************/
BOSS *GetBoss(void)
{
	return g_boss;
}