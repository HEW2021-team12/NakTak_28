/*==============================================================================

   ���[�U�[ [razer.h]
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

#define RAZER_NUM		(5)					// ���[�U�[�̃R�}��

// UV�Ǘ��p
#define RAZER_U			(1.0f / RAZER_NUM)	// ���[�U�[��u�l
#define RAZER_V			(1.0f / 1)			// ���[�U�[��v�l

// �X�e�[�W�P
#define RAZER_MAX		(2)

// �X�e�[�W�Q
#define RAZER2_MAX		(2)

// �X�e�[�W�R
#define RAZER3_MAX		(2)

// �X�e�[�W�S
#define RAZER4_MAX		(2)

// �X�e�[�W�T
#define RAZER5_MAX		(2)

// ���[�U�[�\����
struct RAZER
{
	bool					use;		 // true:�y�i���e�B�[�t�^  false:�y�i���e�B�[����
	float					w, h;		 // ���ƍ���
	D3DXVECTOR2				pos;		 // �|���S���̍��W
	float					u;
	float					v;
	float					uh;
	float					vh;
	int						setcount;	 // 250�J�E���g������Z�b�g�ɂ��悤���H
	unsigned char			changecount;
};

// �X�e�[�W�P
HRESULT InitRazer(void);
void UninitRazer(void);
void UpdateRazer(void);
void DrawRazer(void);
RAZER* GetRazer(void);

// �X�e�[�W�S
//HRESULT InitRazer4(void);
//void UninitRazer4(void);
//void UpdateRazer4(void);
//void DrawRazer4(void);
//RAZER* GetRazer4(void);

