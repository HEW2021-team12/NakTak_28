/*==============================================================================

   ワープ [warp.cpp]
	Author :	古市
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "map.h"
#include <time.h>
#include "sound.h"
#include "timer.h"
#include "warp.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// １つ目
#define WARP_POS_X1		(23)
#define WARP_POS_Y1		(9)

#define WARP_POS_X2		(8)
#define WARP_POS_Y2		(4)

// ２つ目
#define WARP_POS2_X1	(5)
#define WARP_POS2_Y1	(9)

#define WARP_POS2_X2	(18)
#define WARP_POS2_Y2	(14)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

bool WarpCollision(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

unsigned char	g_WarpTexture = 0;
unsigned char	g_WarpSE = 0;
unsigned char	g_WarpSetSE = 0;

// メモリ確保用アドレス
WARP** g_Warp;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWarp(void)
{
	g_WarpTexture = LoadTexture("data/TEXTURE/warp.png");
	g_WarpSE = LoadSound("data/SE/warp.wav");
	g_WarpSetSE = LoadSound("data/SE/warp_set.wav");

	// メモリ確保
	g_Warp = new WARP * [WARP_MAX];
	for (int i = 0; i < WARP_MAX; i++)
	{
		g_Warp[i] = new WARP[WARP_PAIR];
	}

	// ワープ構造体の初期化
	for (int i = 0; i < WARP_MAX; i++)
	{
		for (int j = 0; j < WARP_PAIR; j++)
		{
			// 共通
			g_Warp[i][j].use = true;		// 初期化時にはtrue
			g_Warp[i][j].w = WARP_SIZE;
			g_Warp[i][j].h = WARP_SIZE;
			g_Warp[i][j].u = 0.0f;
			g_Warp[i][j].v = 0.0f;
			g_Warp[i][j].uh = 1.0f;
			g_Warp[i][j].vh = 1.0f;
			g_Warp[i][j].setcount = 0;
		}

		// １つ目
		if (i == 0)
		{
			// 対の片方
			g_Warp[i][0].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_X1;
			g_Warp[i][0].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_Y1;

			// もう片方
			g_Warp[i][1].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_X2;
			g_Warp[i][1].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_Y2;
		}

		// ２つ目
		if (i == 1)
		{
			// 対の片方
			g_Warp[i][0].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_X1;
			g_Warp[i][0].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_Y1;

			// もう片方
			g_Warp[i][1].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_X2;
			g_Warp[i][1].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_Y2;
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitWarp(void)
{
	// メモリ解放
	delete[] g_Warp;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWarp(void)
{
	for (int i = 0; i < WARP_MAX; i++)
	{
		// 対のどちらかがtrueであるとき(基本どちらもuseは同じようにしてるが)
		if (g_Warp[i][0].use || g_Warp[i][1].use)
		{
			PLAYER* player = GetPlayer();

			// 対の片方
			if (WarpCollision(player->pos, g_Warp[i][0].pos,
				player->size, D3DXVECTOR2(g_Warp[i][0].w, g_Warp[i][0].h)))
			{
				player->pos = g_Warp[i][1].pos;
				player->vel.x = 0.0f;
				player->vel.y = 0.0f;
				g_Warp[i][0].use = false;
				g_Warp[i][1].use = false;

				// ワープしたというサウンド
				PlaySound(g_WarpSE, 0);
			}
			// もう片方
			else if (WarpCollision(player->pos, g_Warp[i][1].pos,
				player->size, D3DXVECTOR2(g_Warp[i][1].w, g_Warp[i][1].h)))
			{
				player->pos = g_Warp[i][0].pos;
				player->vel.x = 0.0f;
				player->vel.y = 0.0f;
				g_Warp[i][0].use = false;
				g_Warp[i][1].use = false;

				// ワープしたというサウンド
				PlaySound(g_WarpSE, 0);
			}
		}
		else
		{
			g_Warp[i][0].setcount++;

			if (g_Warp[i][0].setcount > WARP_COUNT)
			{
				g_Warp[i][0].use = true;
				g_Warp[i][1].use = true;
				g_Warp[i][0].setcount = 0;

				// セットしたというサウンド
				PlaySound(g_WarpSetSE, 0);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWarp(void)
{
	for (int i = 0; i < WARP_MAX; i++)
	{
		for (int j = 0; j < WARP_PAIR; j++)
		{
			PLAYER* player = GetPlayer();

			// 画面内に入っているか
			if (fabsf(g_Warp[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Warp[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//エネミーの位置やテクスチャー座標を反映
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Warp[i][j].pos.x - player->pos.x);	// エネミーの表示位置X
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Warp[i][j].pos.y - player->pos.y);	// エネミーの表示位置Y
				float pw = g_Warp[i][j].w;		// エネミーの表示幅
				float ph = g_Warp[i][j].h;		// エネミーの表示高さ
				D3DXCOLOR col;

				if (g_Warp[i][j].use)
				{
					if (i == 0) col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);

					if (i == 1) col = D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f);

					if (i == 2) col = D3DXCOLOR(0.0f, 0.0f, 0.8f, 1.0f);

					DrawSpriteColor(g_WarpTexture, px, py, pw, ph,
						g_Warp[i][j].u, g_Warp[i][j].v,
						g_Warp[i][j].uh, g_Warp[i][j].vh,
						col);
				}
				else
				{
					if (i == 0) col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 0.4f);

					if (i == 1) col = D3DXCOLOR(0.0f, 0.8f, 0.0f, 0.4f);

					if (i == 2) col = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.4f);

					DrawSpriteColor(g_WarpTexture, px, py, pw, ph,
						g_Warp[i][j].u, g_Warp[i][j].v,
						g_Warp[i][j].uh, g_Warp[i][j].vh,
						col);
				}
			}
		}
	}
}

// プレイヤーとワープの当たり判定
bool WarpCollision(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	D3DXVECTOR2 min1, max1;
	D3DXVECTOR2 min2, max2;

	min1.x = pos1.x - size1.x / 2;
	min1.y = pos1.y - size1.y / 2;
	max1.x = pos1.x + size1.x / 2;
	max1.y = pos1.y + size1.y / 2;

	min2.x = pos2.x - size2.x / 2;
	min2.y = pos2.y - size2.y / 2;
	max2.x = pos2.x + size2.x / 2;
	max2.y = pos2.y + size2.y / 2;

	//X軸の判定
	if (min1.x < max2.x &&
		max1.x > min2.x)
	{
		//Y軸の判定
		if (min1.y < max2.y &&
			max1.y > min2.y)
		{
			//全ての条件がTRUEならヒット判定
			return true;
		}
	}

	return false;
}