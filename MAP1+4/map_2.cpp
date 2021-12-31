/*==============================================================================

   マップ管理 [map.cpp]
	Author :	歐 改造 古市
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/

#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "map.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
// 背景用テクスチャ情報
unsigned char g_Back2 = 0;

// マップ用テクスチャ情報
unsigned char g_MapTexture2 = 0;

MAP*	g_map2;

//マップの当たり判定用データ
unsigned char g_map_hitchk2[MAP2_Y][MAP2_X] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


//=============================================================================
// 初期化処理
//=============================================================================
void InitMap2(void)
{
	g_Back2 = LoadTexture("data/TEXTURE/back.jpg");
	g_MapTexture2 = LoadTexture("data/MAP/MAP2.png");

	// メモリ確保
	g_map2 = new MAP;

	// マップUV情報
	g_map2[0].u = 0.0f;
	g_map2[0].v = 0.0f;
	g_map2[0].uh = VIEW_SCREEN_WIDTH / MAP2_X;
	g_map2[0].vh = VIEW_SCREEN_HEIGHT / MAP2_Y;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMap2(void)
{
	// メモリ解放
	delete g_map2;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMap2(void)
{
	ChangeUv2();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMap2(void)
{
	// 背景
	DrawSpriteLeftTop(g_Back2, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	// マップ描画
	DrawSpriteLeftTop(g_MapTexture2,
		0.0f, 0.0f,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		g_map2[0].u, g_map2[0].v,
		g_map2[0].uh, g_map2[0].vh);

	// オブジェクト配置を書くならこの辺?

}

void ChangeUv2(void)
{
	float u, v;
	PLAYER* player = GetPlayer();

	u = player->pos.x / (MAP2_X * CHIP_SIZE);
	v = player->pos.y / (MAP2_Y * CHIP_SIZE);

	// U描画
	if (u < (g_map2[0].uh / 2))
	{
		// 0.33fより小さいとき
		g_map2[0].u = 0.0f;
		player->difference.x = -(((g_map2[0].uh / 2) - u) * (MAP2_X * CHIP_SIZE));
	}
	else if (u > (1.0f - (g_map2[0].uh / 2)))
	{
		// 0.66fより大きいとき
		g_map2[0].u = 1.0f - g_map2[0].uh;
		player->difference.x = ((u - (1.0f - g_map2[0].uh / 2)) * (MAP2_X * CHIP_SIZE));
	}
	else
	{
		// それ以外
		g_map2[0].u = u - (g_map2[0].uh / 2);
		player->difference.x = 0.0f;
	}

	// V描画
	if (v < (g_map2[0].vh / 2))
	{
		// 0.25fより小さいとき
		g_map2[0].v = 0.0f;
		player->difference.y = -(((g_map2[0].vh / 2) - v) * (MAP2_Y * CHIP_SIZE));
	}
	else if (v > (1.0f - (g_map2[0].vh / 2)))
	{
		// 0.75fより大きいとき
		g_map2[0].v = 1.0f - g_map2[0].vh;
		player->difference.y = ((v - (1.0f - g_map2[0].vh / 2)) * (MAP2_Y * CHIP_SIZE));
	}
	else
	{
		// それ以外
		g_map2[0].v = v - (g_map2[0].vh / 2);
		player->difference.y = 0.0f;
	}
}

int GetMapEnter2(D3DXVECTOR2 pos)
{
	int gx = 0;
	int gy = 0;

	float sx, sy;

	// 横軸
	for (int x = 0; x < MAP2_X; x++)
	{
		sx = CHIP_SIZE * x;

		if (pos.x < sx + CHIP_SIZE && // プレイヤー小、マップ大
			pos.x > sx)				  // プレイヤー大、マップ小
		{
			gx = x;
		}
	}

	// 縦軸
	for (int y = 0; y < MAP2_Y; y++)
	{
		sy = CHIP_SIZE * y;

		if (pos.y < sy + CHIP_SIZE && // プレイヤー小、マップ大
			pos.y > sy)				  // プレイヤー大、マップ小
		{
			gy = y;
		}
	}

	return g_map_hitchk2[gy][gx];
}

//int GetMap(int x, int y)
//{
//	return g_map_hitchk[y][x];
//}