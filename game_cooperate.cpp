//******************************************************************************
//
// �^�C�g��:		���̓��[�h�̗���Ǘ�����
// �v���O������:	game_cooperate.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include <time.h>
#include "game_cooperate.h"
#include "main.h"
#include "player.h"
#include "input.h"
#include "BG.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "map.h"
#include "item.h"
#include "sound.h"
#include "time.h"
#include "status.h"
#include "fade.h"
#include "player_life.h"
#include "bar.h"
#include "message.h"
#include "boss.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
void ReSetGame(void);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

/*******************************************************************************
�֐���:	void InitGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̏���������
*******************************************************************************/
void InitGameCooperate(void)
{

	//random seed�̏�����
	srand((unsigned int)time(NULL));

	//BG�̏�����
	InitBG();

	//�X�e�[�W�̏�����
	InitMap();

	//�A�C�e���̏�����
	InitItem();

	//�|���S���̏�����
	InitPlayerLife();
	InitPlayer();

	//Bullet�̏�����
	InitBullet();

	//Explosion�̏�����
	InitExplosion();

	//Enemy�̏�����
	InitEnemy();

	//SCORE�̏�����
	InitScore();

	//TIME�̏�����
	InitTime();

	InitStatus();

	InitBar();

	InitBoss();


}

/*******************************************************************************
�֐���:	void UpdateGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̍X�V�֐�
*******************************************************************************/
void UpdateGameCooperate(void)
{
	GAME_STEP stepNow = GetGameStep();

	switch(stepNow)
	{
		case STEP_PLAY:
			UpdatePlayer();
			UpdateStatus();
			UpdateBullet();
			UpdateEnemy();
			UpdateExplosion();
			//UpdateScore();
			UpdateItem();
			UpdateTime();
			UpdatePlayerLife();
			//UpdateBar();
			UpdateBoss();

			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PAUSE);
				SetMessage(MSG_PAUSE);
			}

			//�X�e�[�W�X�M�b�v
			if(GetKeyboardTrigger(DIK_F4) )
			{
				SetGameStep(STEP_CLEAR);
				SetMessage(MSG_STAGECLEAR);

				//SE
				SetSE(SOUND_LABEL_SE_GAMESTART);
			}
			break;
		case STEP_PAUSE:
			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PLAY);
				ClearMessage(MSG_PAUSE);
			}
			break;
		case STEP_GAMEOVER:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage(MSG_GAMEOVER);
				ClearMessage(MSG_TIME_UP);
				FadeOutToNext(MODE_RESULT);
			}
			break;
		case STEP_CLEAR:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage(MSG_STAGECLEAR);
				if(GetStage() == 2)
				{
					FadeOutToNext(MODE_RESULT);
				}
				else
				{
					FadeOutToNext(STEP_GO_NEXT);
				}
				
			}
			break;
		case STEP_GO_NEXT:
			NextStage();
			ReSetGame();
			SetGameStep(STEP_PLAY);
			break;
	}	


}

/*******************************************************************************
�֐���:	void DrawGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̕`��֐�
*******************************************************************************/
void DrawGameCooperate(void)
{
	DrawBG();
	DrawMap();
	DrawItem();
	DrawEnemy();
	DrawBullet();
	DrawPlayer();
	DrawStatus();
	DrawExplosion();
	DrawPlayerLife();
	DrawBar();
	DrawScore();
	DrawTime();
	DrawBoss();
}
/*******************************************************************************
�֐���:	void UninitGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̊J���֐�
*******************************************************************************/
void UninitGameCooperate(void)
{
	//BG�̊J��
	UninitBG();

	//�|���S���̊J��
	UninitPlayer();

	//bullet�̊J��
	UninitBullet();

	//Explosion�̊J��
	UninitExplosion();

	//Enemy�̊J��
	UninitEnemy();

	//SCORE�̊J��
	UninitScore();

	UninitMap();
	UninitItem();
	UninitTime();
	UninitStatus();
	UninitPlayerLife();
	UninitBar();
	UninitBoss();
}



void ReSetGame(void)
{

	//�X�e�[�W�̏�����
	InitMap();

	//�A�C�e���̏�����
	InitItem();

	//Bullet�̏�����
	InitBullet();

	//Explosion�̏�����
	InitExplosion();

	//Enemy�̏�����
	InitEnemy();

	//TIME�̏�����
	InitTime();

	InitBoss();

	InitStatus();

	ResetAllPlayer();
}


