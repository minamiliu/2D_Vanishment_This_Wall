//******************************************************************************
//
// �^�C�g��:		���Ԃ̕\������
// �v���O������:	time.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _TIME_H_
#define _TIME_H_

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
	int time;
}TIME;

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitTime(void);
void DrawTime(void);		
void UninitTime(void);
void UpdateTime(void);
void SetTime(int value);											
#endif