//=============================================================================
//
// �G�l�~�[(����)���� [transparent_enemy.h]
// Author : �Îs
//
//=============================================================================
#pragma once

#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define TRANSPARENT_MAX				(1)



// �\���̂�enemy�Ƌ���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �X�e�[�W�P
HRESULT InitTransparent(void);
void UninitTransparent(void);
void UpdateTransparent(void);
void DrawTransparent(void);
ENEMY* GetTransparent(void);

//// �X�e�[�W�S
//HRESULT InitTransparent(void);
//void UninitTransparent(void);
//void UpdateTransparent(void);
//void DrawTransparent(void);
//ENEMY* GetTransparent(void);
//
//// �X�e�[�W�T
//HRESULT InitTransparent(void);
//void UninitTransparent(void);
//void UpdateTransparent(void);
//void DrawTransparent(void);
//ENEMY* GetTransparent(void);
