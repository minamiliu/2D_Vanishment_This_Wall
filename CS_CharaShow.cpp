//******************************************************************************
//
// �^�C�g��:		�L�����I���̃L�����N�^�[�\������
// �v���O������:	CS_CharaShow.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "CS_CharaShow.h"
#include "player.h"
#include "CS_select.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define	TEX_PATTERN_DIVIDE_X		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define MAX_CHARA_INTRO	(8)

#define POLYGON_TEXTURENAME_CS_INTRO00		"data/TEXTURE/PLAYER/CS_introduce00.png"
#define POLYGON_TEXTURENAME_CS_INTRO01		"data/TEXTURE/PLAYER/CS_introduce01.png"
#define POLYGON_TEXTURENAME_CS_INTRO02		"data/TEXTURE/PLAYER/CS_introduce02.png"
#define POLYGON_TEXTURENAME_CS_INTRO03		"data/TEXTURE/PLAYER/CS_introduce03.png"
#define POLYGON_TEXTURENAME_CS_INTRO04		"data/TEXTURE/PLAYER/CS_introduce04.png"
#define POLYGON_TEXTURENAME_CS_INTRO05		"data/TEXTURE/PLAYER/CS_introduce05.png"
#define POLYGON_TEXTURENAME_CS_INTRO06		"data/TEXTURE/PLAYER/CS_introduce06.png"
#define POLYGON_TEXTURENAME_CS_INTRO07		"data/TEXTURE/PLAYER/CS_introduce07.png"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexCS_Intro(LPDIRECT3DDEVICE9 pDevice);
void SetVertexCharaIntro(int nCharaIdx);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCS_Intro = NULL;
LPDIRECT3DTEXTURE9 g_pTextureCS_Intro[MAX_CHARA_INTRO] = {NULL};

CS_INTRO g_charaIntro[MAX_PLAYER];


PICTURE_CHARACTOER *OldcharaList = NULL;

/*******************************************************************************
�֐���:	void InitTitle(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���^�C�g���̏���������
*******************************************************************************/
HRESULT InitCS_CharaShow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	PLAYER *pPlayer = GetPlayer();

	pPlayer[0].pos = D3DXVECTOR3(  200, 150, 0.0);
	pPlayer[1].pos = D3DXVECTOR3( 1050, 150, 0.0);
	pPlayer[2].pos = D3DXVECTOR3(  200, 500, 0.0);
	pPlayer[3].pos = D3DXVECTOR3( 1050, 500, 0.0);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		pPlayer[nCntPlayer].bUse = false;
		pPlayer[nCntPlayer].size = D3DXVECTOR3(50, 50, 0.0);
	}

	//���_���̍쐬
	if(FAILED(MakeVertexPlayer(pDevice)))
	{
		return E_FAIL;
	}

	//////chara introduce////////////

	for(int nCntChara = 0; nCntChara < MAX_PLAYER; nCntChara++)
	{
		g_charaIntro[nCntChara].bUse = false;
		g_charaIntro[nCntChara].size = D3DXVECTOR3( 300, 300, 0.0f);
	}
	g_charaIntro[0].pos = D3DXVECTOR3( 200, 200, 0.0f);
	g_charaIntro[1].pos = D3DXVECTOR3( 1050, 200, 0.0f);
	g_charaIntro[2].pos = D3DXVECTOR3( 200, 550, 0.0f);
	g_charaIntro[3].pos = D3DXVECTOR3( 1050, 550, 0.0f);

	//���_���̍쐬

	if(FAILED(MakeVertexCS_Intro(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO00	, &g_pTextureCS_Intro[CHARACTOER0]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO01	, &g_pTextureCS_Intro[CHARACTOER1]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO02	, &g_pTextureCS_Intro[CHARACTOER2]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO03	, &g_pTextureCS_Intro[CHARACTOER3]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO04	, &g_pTextureCS_Intro[CHARACTOER4]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO05	, &g_pTextureCS_Intro[CHARACTOER5]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO06	, &g_pTextureCS_Intro[CHARACTOER6]);
	D3DXCreateTextureFromFile( pDevice,POLYGON_TEXTURENAME_CS_INTRO07	, &g_pTextureCS_Intro[CHARACTOER7]);

	return S_OK;
}

/*******************************************************************************
�֐���:	void UpdateTitle(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���^�C�g���̍X�V�֐�
*******************************************************************************/
void Update_CharaShow(void)
{
	PLAYER *pPlayer = GetPlayer();
	PICTURE_CHARACTOER *charaList = GetCharacterList();

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(GetPlayerJoinData(nCntPlayer))
		{
			pPlayer[nCntPlayer].bUse = true;
			SetPlayerAnimation(nCntPlayer, charaList[nCntPlayer] );
			g_charaIntro[nCntPlayer].bUse = true;
			g_charaIntro[nCntPlayer].type = charaList[nCntPlayer];
		}
		if(pPlayer[nCntPlayer].bUse){

			if(OldcharaList[nCntPlayer] != charaList[nCntPlayer])
			{
				SetPlayerAnimation(nCntPlayer, charaList[nCntPlayer] );
				g_charaIntro[nCntPlayer].type = charaList[nCntPlayer];
			}


			// ���_���W�̐ݒ�
			SetVertexPlayer(nCntPlayer);

			//�����ύX
			SetTexturePlayer(nCntPlayer, pPlayer[nCntPlayer].nPatternAnim, D_DOWN);
	
			//�A�j���\��
			pPlayer[nCntPlayer].nCounterAnim++;
			if((pPlayer[nCntPlayer].nCounterAnim % 10 ) == 0) //10frame change pattern
			{
				// �p�^�[���̐؂�ւ�
				pPlayer[nCntPlayer].nPatternAnim = (pPlayer[nCntPlayer].nPatternAnim + 1) % 3; //3 pattern 

				// �e�N�X�`�����W��ݒ�
				SetTexturePlayer(nCntPlayer, pPlayer[nCntPlayer].nPatternAnim, D_DOWN);

				//nCounterAnim�̃��Z�b�g
				pPlayer[nCntPlayer].nCounterAnim = 0;
			}	
		}
	}
	
	OldcharaList = charaList;
}

/*******************************************************************************
�֐���:	void DrawTitle(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���^�C�g���̕`��֐�
*******************************************************************************/
void DrawCS_CharaShow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferCS_Intro, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntChara = 0; nCntChara < MAX_PLAYER; nCntChara++)
	{
		if(g_charaIntro[nCntChara].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCS_Intro[ g_charaIntro[nCntChara].type ]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntChara*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);			
		}
	}


	//character animation
	DrawPlayer();
}
/*******************************************************************************
�֐���:	void UninitTitle(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���^�C�g���̊J���֐�
*******************************************************************************/
void Uninit_CharaShow(void)
{
	UninitPlayer();

	for(int nCntChara = 0; nCntChara < MAX_CHARA_INTRO; nCntChara++)
	{
		if(g_pTextureCS_Intro[nCntChara] != NULL)
		{
			g_pTextureCS_Intro[nCntChara]->Release();
			g_pTextureCS_Intro[nCntChara] = NULL;
		}
	}

	if(g_pVtxBufferCS_Intro != NULL)
	{
		g_pVtxBufferCS_Intro->Release();
		g_pVtxBufferCS_Intro = NULL;
	}	
}

HRESULT MakeVertexCS_Intro(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_PLAYER,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferCS_Intro,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferCS_Intro->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntChara = 0; nCntChara < MAX_PLAYER; nCntChara++, pVtx+=4)
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

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x - (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y - (g_charaIntro[nCntChara].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x + (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y - (g_charaIntro[nCntChara].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x - (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y + (g_charaIntro[nCntChara].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_charaIntro[nCntChara].pos.x + (g_charaIntro[nCntChara].size.x/2), g_charaIntro[nCntChara].pos.y + (g_charaIntro[nCntChara].size.y/2), 0.0f);


		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferCS_Intro->Unlock();

	return S_OK;
}

/*******************************************************************************
�֐���:	void SetVertexPolygon(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���_���W�̐ݒ�
*******************************************************************************/
void SetVertexCharaIntro(int nCharaIdx)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferCS_Intro->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCharaIdx * NUM_VERTEX; 

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x - (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y - (g_charaIntro[nCharaIdx].size.y/2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x + (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y - (g_charaIntro[nCharaIdx].size.y/2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x - (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y + (g_charaIntro[nCharaIdx].size.y/2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_charaIntro[nCharaIdx].pos.x + (g_charaIntro[nCharaIdx].size.x/2), g_charaIntro[nCharaIdx].pos.y + (g_charaIntro[nCharaIdx].size.y/2), 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferCS_Intro->Unlock();
	}
}
