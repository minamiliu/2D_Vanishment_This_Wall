//******************************************************************************
//
// �^�C�g��:		���U���g�̃o�g�����ʕ\������
// �v���O������:	result_kdw.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include <stdio.h>
#include "player.h"
#include "result_kdw.h"
#include "num_death.h"
#include "num_kill.h"
#include "num_win.h"
#include "result_kdw_kill.h"
#include "result_kdw_death.h"
#include "result_kdw_tex.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPD3DXFONT         g_ResultFont      = NULL;

/*******************************************************************************
�֐���:	HRESULT InitBG(void)
����:	�Ȃ�
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̏���������
*******************************************************************************/
HRESULT InitResultKDW(void)
{
	//�O���֐��̏�����
	InitResultKDWKill();
	InitResultKDWDeath();
	InitResultKDWTex();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXCreateFont( pDevice , 80 ,  30, 0 , 0 , 0 , 0 , 0 , 0 , 0 , "Times New Roman" , &g_ResultFont );
	NUM_KILL *pNumKill = GetNumKill();
	NUM_DEATH *pNumDeath = GetNumDeath();

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		ChangeResultKDWKill(nCntPlayer, pNumKill[nCntPlayer].score);
		ChangeResultKDWDeath(nCntPlayer, pNumDeath[nCntPlayer].score);
	}
	

	return S_OK;
}
/*******************************************************************************
�֐���:	void DrawBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̕`��֐�
*******************************************************************************/
void DrawResultKDW(void)
{
	DrawResultKDWKill();
	DrawResultKDWDeath();
	DrawResultKDWTex();
	//RECT rect = {100, 200, SCREEN_WIDTH, SCREEN_HEIGHT};
	//char aStr[256];
	//NUM_KILL *pNumKill = GetNumKill();
	//NUM_DEATH *pNumDeath = GetNumDeath();
	//NUM_WIN *pNumWin = GetNumWin();

	//for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++) //�ő�X��
	//{
	//	// �\������e�L�X�g�̏���
	//	wsprintf(&aStr[0], "%2dP: KILL %02d, DEATH %02d, WIN %1d \n",nCntPlayer+1 , pNumKill[nCntPlayer].score, pNumDeath[nCntPlayer].score, pNumWin[nCntPlayer].score);


	//	switch(nCntPlayer)
	//	{
	//		case 0:
	//			
	//			// �e�L�X�g�`��
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0x00, 0x00, 0xff)); //red				
	//			break;
	//		case 1:

	//			// �e�L�X�g�`��
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0x00, 0xff)); //yellow					
	//			break;
	//		case 2:

	//			// �e�L�X�g�`��
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0x00, 0xff, 0x00, 0xff)); //green				
	//			break;
	//		case 3:

	//			// �e�L�X�g�`��
	//			g_ResultFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0x00, 0x00, 0xff, 0xff)); //blue
	//			break;
	//	}	
	//
	//	

	//	rect.top += 100;
	//}

}
/*******************************************************************************
�֐���:	void UninitBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̊J���֐�
*******************************************************************************/
void UninitResultKDW(void)
{
	//if ( g_ResultFont != NULL )
	//{
	//	g_ResultFont->Release();
	//	g_ResultFont = NULL;
	//}
	UninitResultKDWKill();
	UninitResultKDWDeath();
	UninitResultKDWTex();
}

void UpdateResultKDW(void)
{
	UpdateResultKDWKill();
	UpdateResultKDWDeath();
	UpdateResultKDWTex();
}

