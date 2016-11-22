//******************************************************************************
//
// �^�C�g��:		�o�g�����[�h�̃Q�[�����ꏈ��
// �v���O������:	game_battle.cpp
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include <time.h>
#include "game_battle.h"
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
//#include "time.h"
#include "status.h"
#include "fade.h"
#include "player_life.h"
#include "message.h"

#include "bar_battle.h"
#include "num_kill.h"
#include "num_death.h"
#include "num_win.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
void ReSetGameBattle(void);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

/*******************************************************************************
�֐���:	void InitGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̏���������
*******************************************************************************/
void InitGameBattle(void)
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

	//Status�̏�����
	InitStatus();


	//battle mode �ɕ\�����Ȃ�����
	InitScore();
	//InitTime();


	//battle mode �ɕ\���������
	InitBarBattle();
	InitNumKill();
	InitNumDeath();
	InitNumWin();

	UninitEnemy();
}

/*******************************************************************************
�֐���:	void UpdateGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̍X�V�֐�
*******************************************************************************/
void UpdateGameBattle(void)
{
	GAME_STEP stepNow = GetGameStep();

	switch(stepNow)
	{
		case STEP_PLAY:
			UpdatePlayer();
			UpdateStatus();
			UpdateBullet();
			//UpdateEnemy();
			UpdateExplosion();
			UpdateScore();
			UpdateItem();
			//UpdateTime();
			UpdatePlayerLife();
			UpdateNumKill();
			UpdateNumDeath();
			UpdateNumWin();
			UpdateBarBattle();

			if(GetKeyboardTrigger(DIK_P))
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
			if(GetKeyboardTrigger(DIK_P))
			{
				SetGameStep(STEP_PLAY);
				ClearMessage(MSG_PAUSE);
			}
			break;
		case STEP_GAMEOVER:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage(MSG_GAMEOVER);
				FadeOutToNext(MODE_RESULT);
			}
			break;
		case STEP_CLEAR:
			if(GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger( 0, BUTTON_B))
			{
				ClearMessage();

				int winner = GetWinner();
				if(winner == -1)
				{
					FadeOutToNext(STEP_GO_NEXT);
				}
				else
				{
					FadeOutToNext(MODE_RESULT);
				}
			}
			break;
		case STEP_GO_NEXT:
			NextStage();
			ReSetGameBattle();
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
void DrawGameBattle(void)
{
	DrawBG();
	DrawMap();
	DrawItem();
	//DrawEnemy();
	DrawBullet();
	DrawPlayer();
	DrawStatus();
	DrawExplosion();
	DrawBarBattle();
	DrawNumKill();
	DrawNumDeath();
	DrawNumWin();

	//DrawPlayerLife();
	//DrawScore();
	//DrawTime();

}
/*******************************************************************************
�֐���:	void UninitGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̊J���֐�
*******************************************************************************/
void UninitGameBattle(void)
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
	//UninitEnemy();

	//SCORE�̊J��
	UninitScore();

	UninitMap();
	UninitItem();
	//UninitTime();
	UninitStatus();
	UninitPlayerLife();

	UninitBarBattle();
	UninitNumKill();
	UninitNumDeath();
	UninitNumWin();
}



void ReSetGameBattle(void)
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
	//InitEnemy();

	//TIME�̏�����
	//InitTime();

	InitStatus();

	InitPlayer();
}
