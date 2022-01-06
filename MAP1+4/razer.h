/*==============================================================================

   レーザー [razer.h]
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

#define RAZER_NUM		(5)					// レーザーのコマ数

// UV管理用
#define RAZER_U			(1.0f / RAZER_NUM)	// レーザーのu値
#define RAZER_V			(1.0f / 1)			// レーザーのv値

// ステージ１
#define RAZER_MAX		(2)

// ステージ２
#define RAZER2_MAX		(2)

// ステージ３
#define RAZER3_MAX		(2)

// ステージ４
#define RAZER4_MAX		(2)

// ステージ５
#define RAZER5_MAX		(2)

// レーザー構造体
struct RAZER
{
	bool					use;		 // true:ペナルティー付与  false:ペナルティー無し
	float					w, h;		 // 幅と高さ
	D3DXVECTOR2				pos;		 // ポリゴンの座標
	float					u;
	float					v;
	float					uh;
	float					vh;
	int						setcount;	 // 250カウントしたらセットにしようか？
	unsigned char			changecount;
};

// ステージ１
HRESULT InitRazer(void);
void UninitRazer(void);
void UpdateRazer(void);
void DrawRazer(void);
RAZER* GetRazer(void);

// ステージ４
//HRESULT InitRazer4(void);
//void UninitRazer4(void);
//void UpdateRazer4(void);
//void DrawRazer4(void);
//RAZER* GetRazer4(void);

