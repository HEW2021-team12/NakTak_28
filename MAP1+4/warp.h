/*==============================================================================

   ���[�v [warp.cpp]
	Author :	�Îs
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	WARP_PAIR		(2)				// 1�g�ł����Ȃ̂�
#define WARP_COUNT		(500)			// �N�[���^�C���Ǘ��p
#define WARP_SIZE		(60.0f)

// �X�e�[�W�P
#define WARP_MAX		(2)				// 1�g�łP�ƃJ�E���g�A�܂�2�Ȃ�Q�g

//// �X�e�[�W�S
//#define WARP_MAX		(2)				// 1�g�łP�ƃJ�E���g�A�܂�2�Ȃ�Q�g
//
//// �X�e�[�W�T
//#define WARP_MAX		(2)				// 1�g�łP�ƃJ�E���g�A�܂�2�Ȃ�Q�g


// ���[�v�\����
struct WARP
{
	bool					use;		 // true:���[�v�\  false:���[�v�s��
	float					w, h;		 // ���ƍ���
	D3DXVECTOR2				pos;		 // �|���S���̍��W
	float					u;
	float					v;
	float					uh;
	float					vh;
	int						setcount;	 // �N�[���^�C���Ǘ��p
};

// �X�e�[�W�P
HRESULT InitWarp(void);
void UninitWarp(void);
void UpdateWarp(void);
void DrawWarp(void);

//// �X�e�[�W�S
//HRESULT InitWarp(void);
//void UninitWarp(void);
//void UpdateWarp(void);
//void DrawWarp(void);
//
//// �X�e�[�W�T
//HRESULT InitWarp(void);
//void UninitWarp(void);
//void UpdateWarp(void);
//void DrawWarp(void);