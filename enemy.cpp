//******************************************************************************
//
// �����ȥ�:		���ϥ⡼�ɤ�Ũ����
// �ץ����̾:	enemy.cpp
// ������:			HAL���������زʡ�έ�
//
//******************************************************************************


/*******************************************************************************
* ���󥯥롼�ɥե�����
*******************************************************************************/

#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "map.h"

#include "game.h"

//*****************************************************************************
// �ޥ������
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/teki0001.png"		// �ɤ߹���ƥ�������ե�����̾
#define	POLYGON_DOTPOS_X			(0)							// �ݥꥴ��δ�����(�غ�ɸ)
#define	POLYGON_DOTPOS_Y			(0)								// �ݥꥴ��δ�����(�ٺ�ɸ)
#define	POLYGON_SIZE_X				(50.0f)							// �ݥꥴ�����
#define	POLYGON_SIZE_Y				(50.0f)							// �ݥꥴ��ι⤵

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
HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice);
void SetTextureEnemy(int nCntEnemy, int nPatternAnim, DIRECTION dir);
void SetVertexEnemy(int nCntEnemy);
bool CollisionCheckEB(int enemyIdx);
bool CollisionCheckEP(int enemyIdx);

/*******************************************************************************
* �����Х��ѿ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;

ENEMY	g_enemy[MAX_ENEMY]; //�ץ쥤�䡼�Υ��

/*******************************************************************************
�ؿ�̾:	HRESULT InitEnemy(void)
����:	�ʤ�
�����:	HRESUL : �������� ���ｪλ:S_OK
����:	���������
*******************************************************************************/
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MAP *pMap = GetMap();

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//enemy�ν����
		int randX, randY;
		do
		{
			randX = rand()%19 + 3;
			randY = rand()%10;
		}while(pMap->data[randY][randX] != BLOCK_GRASS);

		g_enemy[nCntEnemy].dotPos = D3DXVECTOR3( randX, randY, 0.0f);

		g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
		g_enemy[nCntEnemy].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_enemy[nCntEnemy].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_enemy[nCntEnemy].dir = (DIRECTION)(rand()%4);
		g_enemy[nCntEnemy].walking = false;

		GAME_MODE mode = GetGameMode();
		switch(mode)
		{
			case GAME_COOPERATE:
				g_enemy[nCntEnemy].bUse = true;	
				break;
			case GAME_BATTLE:
				g_enemy[nCntEnemy].bUse = false;	
				break;
		}

		
	}

	// ���˥᡼�����ν����



	//ĺ������κ���
	if(FAILED(MakeVertexEnemy(pDevice)))
	{
		return E_FAIL;
	}

	// �ƥ���������ɤ߹���
	D3DXCreateTextureFromFile(pDevice,					// �ǥХ����ؤΥݥ���
								TEXTURE_POLYGON,		// �ե������̾��
								&g_pTextureEnemy);	// �ɤ߹�����꡼

	return S_OK;
}

/*******************************************************************************
�ؿ�̾:	void DrawEnemy(void)
����:	�ʤ�
�����:	�ʤ�
����:	�ݥꥴ�������ؿ�
*******************************************************************************/
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ĺ���Хåե���ǥХ����Υǡ������ȥ꡼��˥Х����
	pDevice->SetStreamSource(0, g_pVtxBufferEnemy, 0, sizeof(VERTEX_2D));

	//ĺ���ե����ޥåȤ�����
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�ƥ������������
	pDevice->SetTexture(0, g_pTextureEnemy);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{
			//�ݥꥴ�������
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�ץ�ߥƥ��֤μ���
				nCntEnemy*NUM_VERTEX,	//���ɤ���ǽ��ĺ������ǥå���
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
void UninitEnemy(void)
{
	if(g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}
	if(g_pVtxBufferEnemy != NULL)
	{
		g_pVtxBufferEnemy->Release();
		g_pVtxBufferEnemy = NULL;
	}

	//enemy�����Ѥ��ʤ������ꤹ��
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_enemy[nCntEnemy].bUse = false;	
	}
}

/*******************************************************************************
�ؿ�̾:	HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device���֥�������
�����:	HRESUL : �������� ���ｪλ:S_OK
����:	�ݥꥴ���ĺ������κ����ؿ�
*******************************************************************************/
HRESULT MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ENEMY,	//ĺ���ǡ����ΥХåե������� 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//ĺ���ե����ޥå�
		D3DPOOL_MANAGED, 
		&g_pVtxBufferEnemy,			//ĺ���Хåե����󥿡��ե������Υݥ���
		NULL)))
	{
		return E_FAIL;
	}


	//ĺ���Хåե�����Ȥ�����
	VERTEX_2D *pVtx;

	//ĺ���ǡ������ϰϤ��å�����ĺ���Хåե��ؤΥݥ��󥿤����
	g_pVtxBufferEnemy->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += NUM_VERTEX)
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
		pVtx[0].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//�ƥ��������ɸ����
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	
	}



	g_pVtxBufferEnemy->Unlock();

	return S_OK;
}


/*******************************************************************************
�ؿ�̾:	void UpdateEnemy(void)
����:	�ʤ�
�����:	�ʤ�
����:	��������
*******************************************************************************/
void UpdateEnemy(void)
{	
	DIRECTION new_dir;
	MAP *pMap = GetMap();

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{

			//��ư����Ĵ��
			if(fabs(g_enemy[nCntEnemy].pos.x - DotPos2Pos(g_enemy[nCntEnemy].dotPos).x) < VALUE_MOVE && fabs(g_enemy[nCntEnemy].pos.y - DotPos2Pos(g_enemy[nCntEnemy].dotPos).y) < VALUE_MOVE )
			{
				g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
				g_enemy[nCntEnemy].walking = false;
			}
			
			//���ΰ��ְ�ư������
			if(g_enemy[nCntEnemy].pos == DotPos2Pos(g_enemy[nCntEnemy].dotPos))
			{
				g_enemy[nCntEnemy].walking = false;
			}

			if(g_enemy[nCntEnemy].walking)
			{
				switch(g_enemy[nCntEnemy].dir)
				{
					case D_UP:
						//Ũ���Ƥ�������Ƚ��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.y += 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.y -= g_enemy[nCntEnemy].move.y;
						break;
					case D_DOWN:
						//Ũ���Ƥ�������Ƚ��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.y -= 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;
						break;
					case D_LEFT:
						//Ũ���Ƥ�������Ƚ��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.x += 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.x -= g_enemy[nCntEnemy].move.x;
						break;
					case D_RIGHT:
						//Ũ���Ƥ�������Ƚ��
						if(CollisionCheckEB( nCntEnemy))
						{
							g_enemy[nCntEnemy].dotPos.x -= 1;
							g_enemy[nCntEnemy].pos = DotPos2Pos(g_enemy[nCntEnemy].dotPos);
							g_enemy[nCntEnemy].walking = false;
							break;
						}
						g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].move.x;
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
					g_enemy[nCntEnemy].dir = D_UP;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y - 1][(int)g_enemy[nCntEnemy].dotPos.x] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.y -= g_enemy[nCntEnemy].move.y;
						g_enemy[nCntEnemy].dotPos.y -= 1;		
						g_enemy[nCntEnemy].walking = true;
					}
				}
				if(new_dir == D_DOWN)
				{
					g_enemy[nCntEnemy].dir = D_DOWN;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y + 1][(int)g_enemy[nCntEnemy].dotPos.x] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;
						g_enemy[nCntEnemy].dotPos.y += 1;
						g_enemy[nCntEnemy].walking = true;
					}

				}
				if(new_dir == D_LEFT)
				{
					g_enemy[nCntEnemy].dir = D_LEFT;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y][(int)g_enemy[nCntEnemy].dotPos.x - 1] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.x -= g_enemy[nCntEnemy].move.x;
						g_enemy[nCntEnemy].dotPos.x -= 1;
						g_enemy[nCntEnemy].walking = true;
					}
				}
				if(new_dir == D_RIGHT)
				{
					g_enemy[nCntEnemy].dir = D_RIGHT;
					if(pMap->data[(int)g_enemy[nCntEnemy].dotPos.y][(int)g_enemy[nCntEnemy].dotPos.x + 1] == BLOCK_GRASS)
					{
						g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].move.x;
						g_enemy[nCntEnemy].dotPos.x += 1;
						g_enemy[nCntEnemy].walking = true;
					}
				}
			}


			SetVertexEnemy(nCntEnemy);


			//�����ѹ�
			SetTextureEnemy(nCntEnemy, g_enemy[nCntEnemy].nPatternAnim, g_enemy[nCntEnemy].dir);
	
			//���˥�ɽ��
			g_enemy[nCntEnemy].nCounterAnim++;
			if((g_enemy[nCntEnemy].nCounterAnim % TIME_CHANGE_PATTERN) == 0)
			{
				// �ѥ�������ڤ��ؤ�
				g_enemy[nCntEnemy].nPatternAnim = (g_enemy[nCntEnemy].nPatternAnim + 1) % TEX_PATTERN_DIVIDE_X;

				// �ƥ��������ɸ������
				SetTextureEnemy(nCntEnemy, g_enemy[nCntEnemy].nPatternAnim, g_enemy[nCntEnemy].dir);

				//nCounterAnim�Υꥻ�å�
				g_enemy[nCntEnemy].nCounterAnim = 0;
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
void SetVertexEnemy(int nCntEnemy)
{
	{//ĺ���Хåե�����Ȥ�����
		VERTEX_2D *pVtx;

		// ĺ���ǡ������ϰϤ��å�����ĺ���Хåե��ؤΥݥ��󥿤����
		g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);
		
		pVtx += nCntEnemy * NUM_VERTEX; 

		//ĺ����ɸ������
		pVtx[0].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x - (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_enemy[nCntEnemy].pos.x + (POLYGON_SIZE_X/2), g_enemy[nCntEnemy].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		// ĺ���ǡ����򥢥��å�����
		g_pVtxBufferEnemy->Unlock();
	}
}

/*******************************************************************************
�ؿ�̾:	void SetTextureEnemy(int nPatternAnim)
����:	int nPatternAnim : ���˥᡼�����ѥ�����No.
�����:	�ʤ�
����:	�ƥ��������ɸ������
*******************************************************************************/
void SetTextureEnemy(int nCntEnemy, int nPatternAnim, DIRECTION dir)
{
	{//ĺ���Хåե�����Ȥ�����
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// ĺ���ǡ������ϰϤ��å�����ĺ���Хåե��ؤΥݥ��󥿤����
		g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntEnemy * NUM_VERTEX; 

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
		g_pVtxBufferEnemy->Unlock();
	}
}

/*******************************************************************************
�ؿ�̾:	void SetEnemy(D3DXVECTOR3 pos)
����:	D3DXVECTOR3 pos��Ũ�����֤������
�����:	�ʤ�
����:	Ũ������
*******************************************************************************/
void SetEnemy(D3DXVECTOR3 pos)
{
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(!g_enemy[nCntEnemy].bUse)
		{
			//���֤�����
			g_enemy[nCntEnemy].pos = pos;
			SetVertexEnemy(nCntEnemy);
			g_enemy[nCntEnemy].bUse = true; //ȯ������ѹ�
			break;
		}
	}

}


/*******************************************************************************
�ؿ�̾:	bool CollisionCheckEB(int enemyIdx)
����:	int enemyIdx�������ܤ�Ũ
�����:	bool�������� true��������ʤ� false
����:	Ũ(E)����(B)��������Ƚ��
*******************************************************************************/
bool CollisionCheckEB(int enemyIdx)
{
	BULLET* pBellet = GetBullet();


	for(int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if(pBellet[nCntBullet].bUse)
		{
			//�Х���ǥ��󥰥��ե���
			if( (g_enemy[enemyIdx].pos.x - pBellet[nCntBullet].pos.x)*(g_enemy[enemyIdx].pos.x - pBellet[nCntBullet].pos.x) + (g_enemy[enemyIdx].pos.y - pBellet[nCntBullet].pos.y)*(g_enemy[enemyIdx].pos.y - pBellet[nCntBullet].pos.y) < (POLYGON_SIZE_X/2 + pBellet[nCntBullet].size.x/2)*(POLYGON_SIZE_X/2+pBellet[nCntBullet].size.x/2) )
			{
				return true;
			}	
		}

	}	
	return false;
}


/*******************************************************************************
�ؿ�̾:	BULLET* GetBullet(void)
����:	�ʤ�
�����: BULLET*��BULLET�Υݥ���
����:	������Ƚ��ʤɤλ����Ƥξ�����ɤ߼��
*******************************************************************************/
ENEMY *GetEnemy(void)
{
	return g_enemy;
}