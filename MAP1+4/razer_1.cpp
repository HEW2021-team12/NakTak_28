/*==============================================================================

   レーザー [razer.cpp]
	Author :	古市
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "main.h"
#include "map.h"
#include <time.h>
#include "sound.h"
#include "timer.h"
#include "razer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define RAZER_SIZEX			(200.0f)
#define RAZER_SIZEY			(100.0f)

// サイズを設置場所で変更したいならここで宣言してちゃんと初期化すること


#define RAZER_COUNT			(250)		// useがfalseになり続けるカウント
#define RAZER_KEEP			(400)		// useがtrueになり続けるカウント

#define RAZER_POS_X1		(13)
#define RAZER_POS_Y1		(9)

#define RAZER_POS_X2		(3)
#define RAZER_POS_Y2		(4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

unsigned char	g_RazerTexture = 0;

// メモリ確保用アドレス
RAZER* g_Razer;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRazer(void)
{
	g_RazerTexture = LoadTexture("data/TEXTURE/razer.png");

	// メモリ確保
	g_Razer = new RAZER[RAZER_MAX];

	// レーザー構造体の初期化
	for (int i = 0; i < RAZER_MAX; i++)
	{
		g_Razer[i].use = false;
		g_Razer[i].w = RAZER_SIZEX;
		g_Razer[i].h = RAZER_SIZEY;

		g_Razer[i].u = 0.0f;
		g_Razer[i].v = 0.0f;
		g_Razer[i].uh = RAZER_U;
		g_Razer[i].vh = RAZER_V;

		g_Razer[i].setcount = 0;

		// 一つ目
		if (i == 0)
		{
			g_Razer[i].pos.x = CHIP_SIZE * RAZER_POS_X1;
			g_Razer[i].pos.y = CHIP_SIZE * RAZER_POS_Y1;
		}
		// 二つ目
		if (i == 1)
		{
			g_Razer[i].pos.x = CHIP_SIZE * RAZER_POS_X2;
			g_Razer[i].pos.y = CHIP_SIZE * RAZER_POS_Y2;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRazer(void)
{
	// メモリ解放
	delete[] g_Razer;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRazer(void)
{
	for (int i = 0; i < RAZER_MAX; i++)
	{
		if (g_Razer[i].use)
		{
			g_Razer[i].setcount++;

			if (g_Razer[i].setcount > RAZER_KEEP)
			{
				g_Razer[i].use = false;
				g_Razer[i].setcount = 0;
			}
		}
		else
		{
			g_Razer[i].setcount++;

			if (g_Razer[i].setcount > RAZER_COUNT)
			{
				g_Razer[i].changecount = g_Razer[i].setcount - RAZER_COUNT;

				g_Razer[i].u = g_Razer[i].changecount * RAZER_U;

				if (g_Razer[i].changecount >= RAZER_NUM)
				{
					g_Razer[i].changecount = 0;
					g_Razer[i].setcount = 0;
					g_Razer[i].use = true;
				}
			}
			else
			{
				g_Razer[i].u = 0.0f;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRazer(void)
{

	PLAYER* player = GetPlayer();

	for (int i = 0; i < RAZER_MAX; i++)
	{
		// 画面内に入っているか
		if (fabsf(g_Razer[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
			fabsf(g_Razer[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
		{

			//エネミーの位置やテクスチャー座標を反映
			float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Razer[i].pos.x - player->pos.x);	// エネミーの表示位置X
			float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Razer[i].pos.y - player->pos.y);	// エネミーの表示位置Y
			float pw = g_Razer[i].w;		// エネミーの表示幅
			float ph = g_Razer[i].h;		// エネミーの表示高さ
			D3DXCOLOR col;

			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (g_Razer[i].use)
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_RazerTexture, px, py, pw, ph,
					RAZER_U * (RAZER_NUM - 1), g_Razer[i].v,
					g_Razer[i].uh, g_Razer[i].vh,
					col);
			}
			else
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_RazerTexture, px, py, pw, ph,
					g_Razer[i].u, g_Razer[i].v,
					g_Razer[i].uh, g_Razer[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// レーザー構造体の先頭アドレスを取得
//=============================================================================
RAZER* GetRazer(void)
{
	return g_Razer;
}