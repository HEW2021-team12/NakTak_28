//=============================================================================
//
// エネミー(透明)処理 [transparent_enemy.h]
// Author : 古市
//
//=============================================================================
#pragma once

#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define TRANSPARENT_MAX				(1)



// 構造体はenemyと共通

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// ステージ１
HRESULT InitTransparent(void);
void UninitTransparent(void);
void UpdateTransparent(void);
void DrawTransparent(void);
ENEMY* GetTransparent(void);

//// ステージ４
//HRESULT InitTransparent(void);
//void UninitTransparent(void);
//void UpdateTransparent(void);
//void DrawTransparent(void);
//ENEMY* GetTransparent(void);
//
//// ステージ５
//HRESULT InitTransparent(void);
//void UninitTransparent(void);
//void UpdateTransparent(void);
//void DrawTransparent(void);
//ENEMY* GetTransparent(void);
