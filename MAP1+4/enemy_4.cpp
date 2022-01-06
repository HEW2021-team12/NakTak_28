//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "player.h"
#include "map.h"
#include <time.h>
#include "sound.h"
#include "timer.h"
#include "patrol.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_SPD			(1.4f)
#define ENEMY_SIZE			(55.0f)
#define SERCH_RANGE			(220.0f)
#define FIND_SPD			(2.0f)
#define NONE_MOVE			(150.0f)

// 初期位置管理用
#define ROOT_NUM4_1			(2)
#define ROOT_NUM4_2			(0)
#define ROOT_NUM4_3			(8)
#define ROOT_NUM4_4			(1)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetEnemy4(int i);
void WatchEnemy4(int i);
void NoneWatchEnemy4(int i);
bool SerchPlayer4(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos); 

//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char	g_EnemyTexture4 = 0;
unsigned char	g_watch_SE4 = 0;
unsigned char	g_watch_lost_SE4 = 0;

// メモリ確保用アドレス
ENEMY* g_Enemy4;				  // エネミー構造体

char* g_RouteX4_1;		  // 巡回ルート用
char* g_RouteY4_1;		  // 巡回ルート用

char* g_RouteX4_2;		  // 巡回ルート用
char* g_RouteY4_2;		  // 巡回ルート用

char* g_RouteX4_3;		  // 巡回ルート用
char* g_RouteY4_3;		  // 巡回ルート用

char* g_RouteX4_4;		  // 巡回ルート用
char* g_RouteY4_4;		  // 巡回ルート用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy4(void)
{
	g_EnemyTexture4 = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE4 = LoadSound("data/SE/watch1.wav");
	g_watch_lost_SE4 = LoadSound("data/SE/enemy_worp.wav");

	// メモリ確保
	g_Enemy4 = new ENEMY[ENEMY_MAX4];

	g_RouteX4_1 = new char[ROOT_MAX4_1];
	g_RouteY4_1 = new char[ROOT_MAX4_1];

	g_RouteX4_2 = new char[ROOT_MAX4_2];
	g_RouteY4_2 = new char[ROOT_MAX4_2];

	g_RouteX4_3 = new char[ROOT_MAX4_3];
	g_RouteY4_3 = new char[ROOT_MAX4_3];

	g_RouteX4_4 = new char[ROOT_MAX4_4];
	g_RouteY4_4 = new char[ROOT_MAX4_4];

	// 巡回ルート設定
	for (char r = 0; r < ROOT_MAX4_1; r++)
	{
		g_RouteX4_1[r] = SetRouteX4_1(r);
		g_RouteY4_1[r] = SetRouteY4_1(r);
	}
	for (char r = 0; r < ROOT_MAX4_2; r++)
	{
		g_RouteX4_2[r] = SetRouteX4_2(r);
		g_RouteY4_2[r] = SetRouteY4_2(r);
	}
	for (char r = 0; r < ROOT_MAX4_3; r++)
	{
		g_RouteX4_3[r] = SetRouteX4_3(r);
		g_RouteY4_3[r] = SetRouteY4_3(r);
	}
	for (char r = 0; r < ROOT_MAX4_4; r++)
	{
		g_RouteX4_4[r] = SetRouteX4_4(r);
		g_RouteY4_4[r] = SetRouteY4_4(r);
	}

	// エネミー構造体の初期化
	for (int i = 0; i < ENEMY_MAX4; i++)
	{
		// 一体目
		if (i == 0)
		{
			g_Enemy4[i].use = true;
			g_Enemy4[i].routenom = ROOT_NUM4_1;

			g_Enemy4[i].pos.x = g_RouteX4_1[ROOT_NUM4_1] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_1[ROOT_NUM4_1] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		// 二体目
		if (i == 1)
		{
			g_Enemy4[i].use = false;
			g_Enemy4[i].routenom = ROOT_NUM4_2;

			g_Enemy4[i].pos.x = g_RouteX4_2[ROOT_NUM4_2] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_2[ROOT_NUM4_2] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		// 三体目
		if (i == 2)
		{
			g_Enemy4[i].use = false;
			g_Enemy4[i].routenom = ROOT_NUM4_3;

			g_Enemy4[i].pos.x = g_RouteX4_3[ROOT_NUM4_3] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_3[ROOT_NUM4_3] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		// 四体目
		if (i == 3)
		{
			g_Enemy4[i].use = false;
			g_Enemy4[i].routenom = ROOT_NUM4_4;

			g_Enemy4[i].pos.x = g_RouteX4_4[ROOT_NUM4_4] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_4[ROOT_NUM4_4] * CHIP_SIZE + (CHIP_SIZE / 2);
		}


		// 共通
		g_Enemy4[i].watch = false;
		g_Enemy4[i].w = ENEMY_SIZE;
		g_Enemy4[i].h = ENEMY_SIZE;
		g_Enemy4[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
		g_Enemy4[i].notmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy4[i].addmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy4[i].movecntX = false;
		g_Enemy4[i].movecntY = false;

		// エネミーUV
		g_Enemy4[i].u = 0.0f;
		g_Enemy4[i].v = 0.0f;
		g_Enemy4[i].uh = 0.5f;
		g_Enemy4[i].vh = 0.5f;
	}




	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy4(void)
{
	// メモリ解放
	delete[] g_Enemy4;

	delete[] g_RouteX4_1;
	delete[] g_RouteY4_1;
	delete[] g_RouteX4_2;
	delete[] g_RouteY4_2;
	delete[] g_RouteX4_3;
	delete[] g_RouteY4_3;
	delete[] g_RouteX4_4;
	delete[] g_RouteY4_4;

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy4(void)
{
	for (int i = 0; i < ENEMY_MAX4; i++)
	{
		if (g_Enemy4[i].use == true)
		{
			// 追跡判定
			if (SerchPlayer4(GetPlayerPosition(), g_Enemy4[i].pos))
			{// 見つけた(範囲内に入った)

				if (!g_Enemy4[i].watch)
				{
					PlaySound(g_watch_SE4, 0);
					g_Enemy4[i].watch = true;
				}

				WatchEnemy4(i);

			}
			// 見つけていない
			else
			{
				if (i == 0)
				{
					if (g_Enemy4[i].watch)
					{
						// 見失った時の処理（SE）
						PlaySound(g_watch_lost_SE4, 0);

						char work = rand() % ROOT_MAX4_1;

						g_Enemy4[i].routenom = work;
						g_Enemy4[i].pos.x = SetRouteX4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
						g_Enemy4[i].pos.y = SetRouteY4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);

					}
				}

				if (i == 1)
				{
					if (g_Enemy4[i].watch)
					{
						// 見失った時の処理（SE）
						PlaySound(g_watch_lost_SE4, 0);

						char work = rand() % ROOT_MAX4_2;

						g_Enemy4[i].routenom = work;
						g_Enemy4[i].pos.x = SetRouteX4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
						g_Enemy4[i].pos.y = SetRouteY4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);

					}
				}

				if (i == 2)
				{
					if (g_Enemy4[i].watch)
					{
						// 見失った時の処理（SE）
						PlaySound(g_watch_lost_SE4, 0);

						char work = rand() % ROOT_MAX4_3;

						g_Enemy4[i].routenom = work;
						g_Enemy4[i].pos.x = SetRouteX4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
						g_Enemy4[i].pos.y = SetRouteY4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);

					}
				}

				if (i == 3)
				{
					if (g_Enemy4[i].watch)
					{
						// 見失った時の処理（SE）
						PlaySound(g_watch_lost_SE4, 0);

						char work = rand() % ROOT_MAX4_4;

						g_Enemy4[i].routenom = work;
						g_Enemy4[i].pos.x = SetRouteX4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
						g_Enemy4[i].pos.y = SetRouteY4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);

					}
				}
				g_Enemy4[i].watch = false;

				NoneWatchEnemy4(i);

			}
		}
	}
	// エネミー追加
	if (GetTimer() == 20)
	{
		SetEnemy4(1);
	}
	if (GetTimer() == 10)
	{
		SetEnemy4(2);
	}
	if (GetTimer() == 10)
	{
		SetEnemy4(3);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy4(void)
{
	for (int i = 0; i < ENEMY_MAX4; i++)
	{
		if (g_Enemy4[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// 画面内に入っているか
			if (fabsf(g_Enemy4[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Enemy4[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//エネミーの位置やテクスチャー座標を反映
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy4[i].pos.x - player->pos.x);	// エネミーの表示位置X
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy4[i].pos.y - player->pos.y);	// エネミーの表示位置Y
				float pw = g_Enemy4[i].w;		// エネミーの表示幅
				float ph = g_Enemy4[i].h;		// エネミーの表示高さ
				D3DXCOLOR col;

				if (SerchPlayer4(GetPlayerPosition(), g_Enemy4[i].pos))
				{
					col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_EnemyTexture4, px, py, pw, ph,
					g_Enemy4[i].u, g_Enemy4[i].v,
					g_Enemy4[i].uh, g_Enemy4[i].vh,
					col);
			}
		}
	}
}


//=============================================================================
// 敵の発生処理
//=============================================================================
void SetEnemy4(int i)
{
	// もし未使用のデータを探す
	if (g_Enemy4[i].use == false)	// 未使用状態の敵データを見つける
	{
		g_Enemy4[i].use = true;		// 使用状態へ変更する

		// 出現したときのSE

		return;						// 敵をセットできたので終了する
	}
}

// 見つけたら
void WatchEnemy4(int i)
{
	// 旧版サーチ
	{
		//現在位置
		D3DXVECTOR2 position = g_Enemy4[i].pos;

		//目標地点
		D3DXVECTOR2 destination = GetPlayerPosition();

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
				g_Enemy4[i].u = 1.0f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = -0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // 右に行くとき
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}
		else // 縦の移動量の方が大きい or 同じとき
		{
			// 上に行くとき
			if (direction.y < 0)
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // 下に行くとき
			{
				g_Enemy4[i].u = 0.0f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}

		//移動後の位置を計算
		g_Enemy4[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy4[i].pos.x + direction.x * FIND_SPD, g_Enemy4[i].pos.y))
			== 1)
		{
			g_Enemy4[i].pos.x -= direction.x * FIND_SPD;
			g_Enemy4[i].notmove.x += fabsf(direction.x);

			if (!g_Enemy4[i].movecntX)
			{
				// 一定時間動けない（障害物に引っかかる）
				if (g_Enemy4[i].notmove.x > NONE_MOVE)
				{
					g_Enemy4[i].movecntX = true;
					g_Enemy4[i].notmove.x = 0.0f;
				}
			}
		}

		g_Enemy4[i].pos.y += direction.y * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy4[i].pos.x, g_Enemy4[i].pos.y + direction.y * FIND_SPD))
			== 1)
		{
			g_Enemy4[i].pos.y -= direction.y * FIND_SPD;
			g_Enemy4[i].notmove.y += fabsf(direction.y);

			if (!g_Enemy4[i].movecntY)
			{
				// 一定時間動けない（障害物に引っかかる）
				if (g_Enemy4[i].notmove.y > NONE_MOVE)
				{
					g_Enemy4[i].movecntY = true;
					g_Enemy4[i].notmove.y = 0.0f;
				}
			}

		}

		// 一定時間動けない（障害物に引っかかる）
		if (g_Enemy4[i].movecntX)
		{
			g_Enemy4[i].movecntX = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_1;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);

		}
		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_1;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);


		}

		if (g_Enemy4[i].movecntX)
		{

			g_Enemy4[i].movecntX = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_2;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_1;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntX)
		{

			g_Enemy4[i].movecntX = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_3;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_3;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntX)
		{

			g_Enemy4[i].movecntX = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_4;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_4;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

	}

}

// 見つけていなかったら
void NoneWatchEnemy4(int i)
{
	{
		//現在位置
		D3DXVECTOR2 position = g_Enemy4[i].pos;

		char work = g_Enemy4[i].routenom;

		if (g_Enemy4[i].pos.x < g_RouteX4_1[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Enemy4[i].pos.x > g_RouteX4_1[work] * CHIP_SIZE)
		{
			if (g_Enemy4[i].pos.y < g_RouteY4_1[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Enemy4[i].pos.y > g_RouteY4_1[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// 偶数 or 0
					work++;
					g_Enemy4[i].routenom++;
					if (work >= ROOT_MAX4_1)
					{
						work = 0;
						g_Enemy4[i].routenom = 0;
					}
				}
				else
				{
					// 奇数
					work--;
					g_Enemy4[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX4_1;
						g_Enemy4[i].routenom = ROOT_MAX4_1;
					}
				}


			}
		}

		if (g_Enemy4[i].pos.x < g_RouteX4_2[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Enemy4[i].pos.x > g_RouteX4_2[work] * CHIP_SIZE)
		{
			if (g_Enemy4[i].pos.y < g_RouteY4_2[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Enemy4[i].pos.y > g_RouteY4_2[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// 偶数 or 0
					work++;
					g_Enemy4[i].routenom++;
					if (work >= ROOT_MAX4_2)
					{
						work = 0;
						g_Enemy4[i].routenom = 0;
					}
				}
				else
				{
					// 奇数
					work--;
					g_Enemy4[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX4_2;
						g_Enemy4[i].routenom = ROOT_MAX4_2;
					}
				}


			}
		}

		if (g_Enemy4[i].pos.x < g_RouteX4_3[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Enemy4[i].pos.x > g_RouteX4_3[work] * CHIP_SIZE)
		{
			if (g_Enemy4[i].pos.y < g_RouteY4_3[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Enemy4[i].pos.y > g_RouteY4_3[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// 偶数 or 0
					work++;
					g_Enemy4[i].routenom++;
					if (work >= ROOT_MAX4_3)
					{
						work = 0;
						g_Enemy4[i].routenom = 0;
					}
				}
				else
				{
					// 奇数
					work--;
					g_Enemy4[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX4_3;
						g_Enemy4[i].routenom = ROOT_MAX4_3;
					}
				}


			}
		}


		if (g_Enemy4[i].pos.x < g_RouteX4_4[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Enemy4[i].pos.x > g_RouteX4_4[work] * CHIP_SIZE)
		{
			if (g_Enemy4[i].pos.y < g_RouteY4_4[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Enemy4[i].pos.y > g_RouteY4_4[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// 偶数 or 0
					work++;
					g_Enemy4[i].routenom++;
					if (work >= ROOT_MAX4_4)
					{
						work = 0;
						g_Enemy4[i].routenom = 0;
					}
				}
				else
				{
					// 奇数
					work--;
					g_Enemy4[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX4_4;
						g_Enemy4[i].routenom = ROOT_MAX4_4;
					}
				}


			}
		}


		//目標地点
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_1[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_1[work] * CHIP_SIZE + (CHIP_SIZE / 2));
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_2[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_2[work] * CHIP_SIZE + (CHIP_SIZE / 2));
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_3[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_3[work] * CHIP_SIZE + (CHIP_SIZE / 2));
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_4[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_4[work] * CHIP_SIZE + (CHIP_SIZE / 2));



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
				g_Enemy4[i].u = 1.0f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = -0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // 右に行くとき
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}
		else // 縦の移動量の方が大きい or 同じとき
		{
			// 上に行くとき
			if (direction.y < 0)
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // 下に行くとき
			{
				g_Enemy4[i].u = 0.0f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}

		//移動後の位置を計算
		g_Enemy4[i].pos.x += direction.x * ENEMY_SPD;
		g_Enemy4[i].pos.y += direction.y * ENEMY_SPD;

	}
}

// 範囲内にプレイヤーが入り込んだら
bool SerchPlayer4(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
{
	D3DXVECTOR2 maxRange, minRange;

	minRange.x = Enemypos.x - SERCH_RANGE;
	minRange.y = Enemypos.y - SERCH_RANGE;
	maxRange.x = Enemypos.x + SERCH_RANGE;
	maxRange.y = Enemypos.y + SERCH_RANGE;

	//X軸の判定
	if (minRange.x < Playerpos.x &&
		maxRange.x > Playerpos.x)
	{
		//Y軸の判定
		if (minRange.y < Playerpos.y &&
			maxRange.y > Playerpos.y)
		{
			//全ての条件がTRUEならヒット判定
			return true;
		}
	}

	return false;
}


//=============================================================================
// エネミー構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetEnemy4(void)
{
	return &g_Enemy4[0];
}