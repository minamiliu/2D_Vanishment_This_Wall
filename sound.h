//******************************************************************************
//
// �^�C�g��:		�T�E���h����
// �v���O������:	sound.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_OP = 0,		// BGM op
	SOUND_LABEL_BGM_CS,			// BGM Character Select
	SOUND_LABEL_BGM_PLAY,		// BGM game playing
	SOUND_LABEL_BGM_RANK,		// BGM rank
	SOUND_LABEL_SE_BOSSHURT,	// BOSS����
	SOUND_LABEL_SE_COUNTDOWN,	// countdown��
	SOUND_LABEL_SE_DEATH,		// ���S��
	SOUND_LABEL_SE_ENEMYDEATH,	// �G�����S
	SOUND_LABEL_SE_EXPLOSION01,	// ������
	SOUND_LABEL_SE_EXPLOSION02,	// ������
	SOUND_LABEL_SE_GAMEOVER,	// GameOver
	SOUND_LABEL_SE_GETITEM,		// �A�C�e���Q�b�g
	SOUND_LABEL_SE_HURT,		// ���䉹
	SOUND_LABEL_SE_LOSTITEM,	// �A�C�e�����ʂ���������
	SOUND_LABEL_SE_OK,			// OK
	SOUND_LABEL_SE_POISON,		// ����
	SOUND_LABEL_SE_SELECT,		// �I��
	SOUND_LABEL_SE_SETBOMB,		// ���e�ݒu
	SOUND_LABEL_SE_GAMESTART,	// �Q�[���X�^�[�g
	SOUND_LABEL_SE_SWITCH,		// ���Z�̃{�^���@�@�@�@�@�@�@

	SOUND_LABEL_MAX,
} SOUND_LABEL;


typedef enum
{
	BGM_ON = 0,
	BGM_OFF,
	BGM_MODE_MAX
}BGM_MODE;

typedef enum
{
	SE_ON = 0,
	SE_OFF,
	SE_MODE_MAX
}SE_MODE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
BGM_MODE *GetBGM(void);
SE_MODE *GetSE(void);
void SetSE(SOUND_LABEL label);
void SwitchBGM(SOUND_LABEL label);

#endif
