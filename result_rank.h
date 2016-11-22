//******************************************************************************
//
// �^�C�g��:		���U���g�̋��̓��[�h�̃����L���O�\������
// �v���O������:	result_rank.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _RESULT_RANK_H_
#define _RESULT_RANK_H_

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* ���C�u�����̃����N
*******************************************************************************/


/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define MAX_RANK	(11)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	D3DCOLOR col;
	bool bUse;
}RESULT_RANK_PLAYER;

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitResultRank(void);
void DrawResultRank(void);		
void UninitResultRank(void);
void UpdateResultRank(void);
										
#endif