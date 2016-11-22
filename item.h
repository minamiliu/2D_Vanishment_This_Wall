//******************************************************************************
//
// �^�C�g��:		�A�C�e������
// �v���O������:	item.h
// �쐬��:			HAL�����Q�[���w�ȁ@����G
//
//******************************************************************************


#ifndef _ITEM_H_
#define _ITEM_H_

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
#define ITEM_MAP_SIZE_X (25)
#define ITEM_MAP_SIZE_Y (13)
#define MAX_ITEM		(ITEM_MAP_SIZE_Y*ITEM_MAP_SIZE_X)		// �I�u�W�F�N�g�̐�
#define FRAME_BEFORE_VISIBLE	(25)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef enum
{
	ITEM_NONE = 0,
	ITEM_BOMB, 
	ITEM_FIRE,
	ITEM_SKATE,
	ITEM_DIAMOND,
	ITEM_GUARD,
	ITEM_BONE,
	ITEM_QUESTION,  //��O��ITEM_QUESTION 
	ITEM_NEXT,
	ITEM_GRAVE,
	ITEM_SETTED_BOMB,
	ITEM_TYPE_MAX
}ITEM_TYPE;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	int index;
	bool visible;
	ITEM_TYPE type;

	int nCounterVisible;
}ITEM;





/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitItem(void);
void DrawItem(void);		
void UninitItem(void);
void UpdateItem(void);
ITEM *GetItem(void);
void SetItem(ITEM_TYPE type, D3DXVECTOR3 dotPos);

#endif