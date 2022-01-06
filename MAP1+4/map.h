/*==============================================================================

   �}�b�v�Ǘ�[map.h]
														 Author :	�^ ���� �Îs
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MAP_H_
#define MAP_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//���͐�΂ɂ�����Ȃ����ƁI
#define VIEW_SCREEN_WIDTH	(16.0f)		// �\���͈�(��)
#define VIEW_SCREEN_HEIGHT	(9.0f)		// �\���͈�(�c)

#define CHIP_SIZE			(60.0f)		// �}�X�̑傫��

// ���Ń}�b�v
#define MAP_X				(27)		// �}�b�v�̉��̃}�X��
#define MAP_Y				(18)		// �}�b�v�̏c�̃}�X��

// ����ȍ~�̃}�b�v
#define MAP2_X				(25)		// 2�}�b�v��
#define MAP2_Y				(22)		// 2�}�b�v��

#define MAP3_X				()			// 3�}�b�v��
#define MAP3_Y				()			// 3�}�b�v��

#define MAP4_X				(34)			// 4�}�b�v��
#define MAP4_Y				(35)			// 4�}�b�v��

#define MAP5_X				(27)			// 5�}�b�v��
#define MAP5_Y				(22)			// 5�}�b�v��

//�}�b�v�\����
typedef struct
{
	float		u = 0.0f;
	float		v = 0.0f;
	float		uh = 0.0f;
	float		vh = 0.0f;
}MAP;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMap	(void);
void UninitMap	(void);
void UpdateMap	(void);
void DrawMap	(void);
void ChangeUv   (void);

int GetMapEnter(D3DXVECTOR2 pos);



void InitMap4(void);
void UninitMap4(void);
void UpdateMap4(void);
void DrawMap4(void);
void ChangeUv4(void);

int GetMapEnter4(D3DXVECTOR2 pos);



//int GetMap(int x, int y);

#endif //MAP_H_
