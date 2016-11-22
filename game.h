//******************************************************************************
//
// �^�C�g��:		���̓��[�h�ƃo�g�����[�h�̑S�̊Ǘ�����
// �v���O������:	game.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "message.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_STAGE	(3)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef enum
{
	STEP_PLAY = 0,
	STEP_PAUSE,
	STEP_CLEAR,
	STEP_GAMEOVER,
	STEP_GO_NEXT,
	//STEP_GAME_SET,
	//STEP_NP_WIN,
	STEP_MAX
}GAME_STEP;

typedef enum
{
	GAME_COOPERATE = 0,
	GAME_BATTLE,
	GAME_MAX
}GAME_MODE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UninitGame(void);
GAME_STEP GetGameStep(void);
void SetGameStep(GAME_STEP);
GAME_MODE GetGameMode(void);
void SetGameMode(GAME_MODE mode);
int GetStage(void);
void NextStage(void);
#endif