//=============================================================================
//
// スライム処理 [slime.cpp]
// Author : 古市
//
//=============================================================================
#include "slime.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "main.h"
#include "map.h"
#include <time.h>
#include "sound.h"
#include "timer.h"
#include "patrol.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define SLIME_SPD			(1.1f)
#define SLIME_SIZEX			(70.0f)
#define SLIME_SIZEY			(50.0f)

#define BABBLE_SIZE			(40.0f)
#define BABBLE_COUNT		(300)

#define SET_SPD				(150)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetBabble4(int i, D3DXVECTOR2 pos);
void MoveSlime4(int i);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char	g_SlimeTexture4 = 0;

// メモリ確保用アドレス
SLIME* g_Slime4;			  // スライム構造体
BABBLE** g_Babble4;			  // バブル構造体
char* g_RouteSlimeX4;		  // 巡回ルート用
char* g_RouteSlimeY4;		  // 巡回ルート用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSlime4(void)
{
	g_SlimeTexture4 = LoadTexture("data/TEXTURE/slime.png");

	// メモリ確保
	g_Slime4 = new SLIME[SLIME_MAX4];
	g_Babble4 = new BABBLE * [SLIME_MAX4];

	for (int i = 0; i < SLIME_MAX4; i++)
	{
		g_Babble4[i] = new BABBLE[BABBLE_MAX];
	}

	g_RouteSlimeX4 = new char[ROOT_MAX4_1];
	g_RouteSlimeY4 = new char[ROOT_MAX4_1];

	// スライム構造体の初期化
	for (int i = 0; i < SLIME_MAX4; i++)
	{
		// １体目
		if (i == 0)
		{
			g_Slime4[i].use = true;
			g_Slime4[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime4[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime4[i].routenom = 5;
		}

		// 共通
		g_Slime4[i].h = SLIME_SIZEY;
		g_Slime4[i].w = SLIME_SIZEX;
		g_Slime4[i].vel = D3DXVECTOR2(SLIME_SPD, SLIME_SPD);

		// スライムUV
		g_Slime4[i].u = 0.0f;
		g_Slime4[i].v = 0.0f;
		g_Slime4[i].uh = 1.0f;
		g_Slime4[i].vh = 1.0f;
	}

	// 巡回ルート設定
	for (char r = 0; r < ROOT_MAX4_1; r++)
	{
		g_RouteSlimeX4[r] = SetRouteX4_1(r);
		g_RouteSlimeY4[r] = SetRouteY4_1(r);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSlime4(void)
{
	// メモリ解放
	delete[] g_Slime4;
	delete[] g_Babble4;
	delete[] g_RouteSlimeX4;
	delete[] g_RouteSlimeY4;
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateSlime4(void)
{
	for (int i = 0; i < SLIME_MAX4; i++)
	{
		if (g_Slime4[i].use)
		{
			MoveSlime4(i);

			if (g_Slime4[i].setcount > SET_SPD)
			{
				g_Slime4[i].setcount = 0;
				SetBabble4(i, g_Slime4[i].pos);
			}
			else
			{
				g_Slime4[i].setcount++;
			}
		}

		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble4[i][j].use)
			{
				g_Babble4[i][j].count--;

				if (g_Babble4[i][j].count < 0)
				{
					g_Babble4[i][j].use = false;
				}
			}
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSlime4(void)
{
	// スライム
	for (int i = 0; i < SLIME_MAX4; i++)
	{
		// バブル
		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble4[i][j].use)
			{
				PLAYER* player = GetPlayer();

				// 画面内に入っているか
				if (fabsf(g_Babble4[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
					fabsf(g_Babble4[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
				{
					//エネミーの位置やテクスチャー座標を反映
					float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Babble4[i][j].pos.x - player->pos.x);	// エネミーの表示位置X
					float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Babble4[i][j].pos.y - player->pos.y);	// エネミーの表示位置Y
					float pw = g_Babble4[i][j].w;		// エネミーの表示幅
					float ph = g_Babble4[i][j].h;		// エネミーの表示高さ
					D3DXCOLOR col;

					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					DrawSpriteColor(g_SlimeTexture4, px, py, pw, ph,
						g_Babble4[i][j].u, g_Babble4[i][j].v,
						g_Babble4[i][j].uh, g_Babble4[i][j].vh,
						col);
				}
			}
		}

		// スライム
		if (g_Slime4[i].use)
		{
			PLAYER* player = GetPlayer();

			// 画面内に入っているか
			if (fabsf(g_Slime4[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Slime4[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//エネミーの位置やテクスチャー座標を反映
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Slime4[i].pos.x - player->pos.x);	// エネミーの表示位置X
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Slime4[i].pos.y - player->pos.y);	// エネミーの表示位置Y
				float pw = g_Slime4[i].w;		// エネミーの表示幅
				float ph = g_Slime4[i].h;		// エネミーの表示高さ
				D3DXCOLOR col;

				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_SlimeTexture4, px, py, pw, ph,
					g_Slime4[i].u, g_Slime4[i].v,
					g_Slime4[i].uh, g_Slime4[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// スライム構造体の先頭アドレスを取得
//=============================================================================
SLIME* GetSlime4(void)
{
	return g_Slime4;
}

BABBLE** GetBabble4(void)
{
	return g_Babble4;
}

// バブルを発生させる
void SetBabble4(int i, D3DXVECTOR2 pos)
{
	for (int j = 0; j < BABBLE_MAX; j++)
	{
		// もし未使用のデータを探す
		if (g_Babble4[i][j].use == false)	// 未使用状態の敵データを見つける
		{
			g_Babble4[i][j].use = true;		// 使用状態へ変更する
			g_Babble4[i][j].pos = pos;
			g_Babble4[i][j].w = BABBLE_SIZE;
			g_Babble4[i][j].h = BABBLE_SIZE - 10.0f;

			g_Babble4[i][j].count = BABBLE_COUNT;

			g_Babble4[i][j].u = 0.5f;
			g_Babble4[i][j].v = 0.0f;
			g_Babble4[i][j].uh = 0.5f;
			g_Babble4[i][j].vh = 0.5f;

			// 出現したときのSE

			return;						// 敵をセットできたので終了する
		}
	}
}

// スライムの移動
void MoveSlime4(int i)
{
	//現在位置
	D3DXVECTOR2 position = g_Slime4[i].pos;

	char work = g_Slime4[i].routenom;

	if (g_Slime4[i].pos.x < g_RouteSlimeX4[work] * CHIP_SIZE + CHIP_SIZE &&
		g_Slime4[i].pos.x > g_RouteSlimeX4[work] * CHIP_SIZE)
	{
		if (g_Slime4[i].pos.y < g_RouteSlimeY4[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Slime4[i].pos.y > g_RouteSlimeY4[work] * CHIP_SIZE)
		{
			if (i % 2 == 0)
			{
				// 偶数 or 0
				work++;
				g_Slime4[i].routenom++;
				if (work >= ROOT_MAX4_1)
				{
					work = 0;
					g_Slime4[i].routenom = 0;
				}
			}
			else
			{
				// 奇数
				work--;
				g_Slime4[i].routenom--;
				if (work < 0)
				{
					work = ROOT_MAX4_1;
					g_Slime4[i].routenom = ROOT_MAX4_1;
				}
			}
		}
	}

	//目標地点
	D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX4[work] * CHIP_SIZE + (CHIP_SIZE / 2),
		g_RouteSlimeY4[work] * CHIP_SIZE + (CHIP_SIZE / 2));

	//成分
	D3DXVECTOR2 components;

	//大きさ
	float magnitude;

	//方向（単位ベクトル）
	D3DXVECTOR2 direction;

	//成分計算
	components.x = destination.x - position.x;
	components.y = destination.y - position.y;

	//大きさ計算
	magnitude = (float)sqrt(components.x * components.x + components.y * components.y);

	//方向計算（正規化）
	direction.x = components.x / magnitude;
	direction.y = components.y / magnitude;

	// 横の移動量の方が大きいとき
	if (fabsf(direction.x) > fabsf(direction.y))
	{
		// 左に行くとき
		if (direction.x < 0)
		{
			g_Slime4[i].u = 0.0f;
			g_Slime4[i].v = 0.5f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
		else // 右に行くとき
		{
			g_Slime4[i].u = 0.5f;
			g_Slime4[i].v = 0.5f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
	}
	else // 縦の移動量の方が大きい or 同じとき
	{
		// 上に行くとき
		if (direction.y < 0)
		{
			g_Slime4[i].u = 0.5f;
			g_Slime4[i].v = 0.0f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
		else // 下に行くとき
		{
			g_Slime4[i].u = 0.0f;
			g_Slime4[i].v = 0.0f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
	}

	//移動後の位置を計算
	g_Slime4[i].pos.x += direction.x * g_Slime4[i].vel.x;
	g_Slime4[i].pos.y += direction.y * g_Slime4[i].vel.y;
}