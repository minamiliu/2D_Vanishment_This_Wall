//******************************************************************************
//
// �^�C�g��:		�X�R�A����
// �v���O������:	score.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _SCORE_H_
#define _SCORE_H_

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


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int score;

}SCORE;
/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitScore(void);
void DrawScore(void);		
void UninitScore(void);
void UpdateScore(void);
void ChangeScore(int value);
SCORE *GetScore(void);
#endif