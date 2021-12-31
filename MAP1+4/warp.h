/*==============================================================================

   ワープ [warp.cpp]
	Author :	古市
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	WARP_PAIR		(2)				// 1組でいくつなのか
#define WARP_COUNT		(500)			// クールタイム管理用
#define WARP_SIZE		(60.0f)

// ステージ１
#define WARP_MAX		(2)				// 1組で１とカウント、つまり2なら２組

//// ステージ４
//#define WARP_MAX		(2)				// 1組で１とカウント、つまり2なら２組
//
//// ステージ５
//#define WARP_MAX		(2)				// 1組で１とカウント、つまり2なら２組


// ワープ構造体
struct WARP
{
	bool					use;		 // true:ワープ可能  false:ワープ不可
	float					w, h;		 // 幅と高さ
	D3DXVECTOR2				pos;		 // ポリゴンの座標
	float					u;
	float					v;
	float					uh;
	float					vh;
	int						setcount;	 // クールタイム管理用
};

// ステージ１
HRESULT InitWarp(void);
void UninitWarp(void);
void UpdateWarp(void);
void DrawWarp(void);

//// ステージ４
//HRESULT InitWarp(void);
//void UninitWarp(void);
//void UpdateWarp(void);
//void DrawWarp(void);
//
//// ステージ５
//HRESULT InitWarp(void);
//void UninitWarp(void);
//void UpdateWarp(void);
//void DrawWarp(void);