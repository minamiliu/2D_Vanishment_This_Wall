//******************************************************************************
//
// �^�C�g��:		�v���C���[����
// �v���O������:	player.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _PLAYER_H_
#define _PLAYER_H_

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"
#include "status.h"

/*******************************************************************************
* ���C�u�����̃����N
*******************************************************************************/


/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define MAX_PLAYER			(4)	
#define PLAYER_ATARI		(3)		 //�v����[�����蔻��̔��a
#define PLAYER_LIFE			(3)
#define PLAYER_BATTLE_LIFE	(1)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef enum
{
	CHARACTOER0	= 0,
	CHARACTOER1,
	CHARACTOER2,
	CHARACTOER3,
	CHARACTOER4,
	CHARACTOER5,
	CHARACTOER6,
	CHARACTOER7,
	MAX_CHARACTER
}PICTURE_CHARACTOER;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	D3DXVECTOR3 dotPos;
	D3DXVECTOR3 size;
	DIRECTION dir;
	bool bUse;

	//animation
	PICTURE_CHARACTOER charaType;
	int nCounterAnim;	// �|���S���̃A�j���[�V�����J�E���^�[
	int nPatternAnim;	// �|���S���̃A�j���[�V�����p�^�[��No.
	bool walking;

	//item
	int power;
	int bombNum;
	int bUseBomb;
	int life;

	//status
	bool status[STATUS_MAX];
	int statusIndex[STATUS_MAX];
	int nCounterStatus[STATUS_MAX];
}PLAYER;
/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitPlayer();
HRESULT SetPlayer(int nPlayerIdx, D3DXVECTOR3 dotPos);
void DrawPlayer(void);		
void UninitPlayer(void);
void UpdatePlayer(void);
PLAYER *GetPlayer(void);
void SetColorPlayer(int nCntPlayer, int R, int G, int B, int A);
void SetVertexPlayer(int nCntPlayer);
void SetTexturePlayer(int nCntPlayer, int nPatternAnim, DIRECTION dir);
HRESULT ResetAllPlayer(void);
HRESULT SetPlayerAnimation(int nPlayerIdx, PICTURE_CHARACTOER type);
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice);
#endif