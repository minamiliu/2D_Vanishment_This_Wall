//******************************************************************************
//
// �^�C�g��:		���C������
// �v���O������:	main.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _MAIN_H_
#define _MAIN_H_

#define USE_SOUND		// �T�E���h�g�p�̗L��
//#undef USE_SOUND

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <windows.h>
#include "d3dx9.h"
#include "dinput.h"

#ifdef USE_SOUND
#include "xaudio2.h"		// XAudio2�֘A�̃w�b�_�[
#endif

/*******************************************************************************
* ���C�u�����̃����N
*******************************************************************************/
#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib,"d3d9.lib")			// �`�揈���ɕK�v
#pragma comment (lib,"d3dx9.lib")			// [d3d9.lib]�̊g�����C�u����
#pragma comment (lib,"dxguid.lib")			// DirectX�R���|�[�l���g�g�p�ɕK�v
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"winmm.lib")
#endif

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define SCREEN_WIDTH (1250)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT (750)				// �E�C���h�E�̍���
#define WINDOW_TITLE "Vanishment This Wall"	
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) //���_�t�H�[�}�b�g(�ݒ�)
#define DIRECTINPUT_VERTION (0x0800)


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex; //�e�N�X�`�����W
}VERTEX_2D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_CHARA,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
}MODE;

typedef enum  //���Ԃ�texture�ɍ��킹��
{
	D_DOWN = 0,
	D_LEFT,
	D_RIGHT,
	D_UP,
	D_BOMBSET,
	D_MAX
}DIRECTION;

typedef struct
{
	int DotX;
	int DotY;
}DOTPOS;


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
#endif