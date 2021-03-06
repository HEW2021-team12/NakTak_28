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
#define ROOT_NUM			(2)
#define ROOT_NUM2			(0)
#define ROOT_NUM3			(8)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetEnemy5(void);
void WatchEnemy5(int i);
void NoneWatchEnemy5(int i);
bool SerchPlayer5(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char	g_EnemyTexture5 = 0;
unsigned char	g_watch_SE5 = 0;
unsigned char	g_watch_lost_SE = 0;

// メモリ確保用アドレス
ENEMY* g_Enemy5;				  // エネミー構造体

char* g_RouteX;		  // 巡回ルート用
char* g_RouteY;		  // 巡回ルート用

char* g_RouteX;		  // 巡回ルート用
char* g_RouteY;		  // 巡回ルート用

char* g_RouteX;		  // 巡回ルート用
char* g_RouteY;		  // 巡回ルート用

char* g_RouteX;		  // 巡回ルート用
char* g_RouteY;		  // 巡回ルート用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy5(void)
{
	g_EnemyTexture5 = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE5 = LoadSound("data/SE/watch1.wav");

	// メモリ確保
	g_Enemy5 = new ENEMY[ENEMY_MAX];

	// エネミー構造体の初期化
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// 一体目
		if (i == 0)
		{
			g_Enemy5[i].use = true;
			g_Enemy5[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 6;
			g_Enemy5[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 16;
		}
		// 二体目
		if (i == 1)
		{
			g_Enemy5[i].use = false;
			g_Enemy5[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 16;
			g_Enemy5[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 14;
		}
		// 三体目
		if (i == 2)
		{
			g_Enemy5[i].use = false;
			g_Enemy5[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 23;
			g_Enemy5[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 30;
		}

		// 共通
		g_Enemy5[i].watch = false;
		g_Enemy5[i].w = ENEMY_SIZE;
		g_Enemy5[i].h = ENEMY_SIZE;
		g_Enemy5[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
		g_Enemy5[i].timer = 0;
		g_Enemy5[i].move = 0;
		g_Enemy5[i].notmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy5[i].addmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy5[i].movecntX = false;
		g_Enemy5[i].movecntY = false;

		// エネミーUV
		g_Enemy5[i].u = 0.0f;
		g_Enemy5[i].v = 0.0f;
		g_Enemy5[i].uh = 0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy5(void)
{
	// メモリ解放
	delete[] g_Enemy5;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy5(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy5[i].use == true)
		{
			// 追跡判定
			if (SerchPlayer5(GetPlayerPosition(), g_Enemy5[i].pos))
			{// 見つけた(範囲内に入った)

				if (!g_Enemy5[i].watch)
				{
					PlaySound(g_watch_SE5, 0);
					g_Enemy5[i].watch = true;
				}

				WatchEnemy5(i);

			}
			// 見つけていない
			else
			{
				if (g_Enemy5[i].watch)
				{
					// 見失った時の処理（SE）

				}

				g_Enemy5[i].watch = false;

				NoneWatchEnemy5(i);

			}
		}
	}
	// エネミー追加
	if (GetTimer() == 20)
	{
		g_Enemy5[1].use = true;
	}
	if (GetTimer() == 10)
	{
		g_Enemy5[2].use = true;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy5(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy5[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// 画面内に入っているか
			if (fabsf(g_Enemy5[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Enemy5[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//エネミーの位置やテクスチャー座標を反映
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy5[i].pos.x - player->pos.x);	// エネミーの表示位置X
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy5[i].pos.y - player->pos.y);	// エネミーの表示位置Y
				float pw = g_Enemy5[i].w;		// エネミーの表示幅
				float ph = g_Enemy5[i].h;		// エネミーの表示高さ
				D3DXCOLOR col;

				if (SerchPlayer5(GetPlayerPosition(), g_Enemy5[i].pos))
				{
					col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_EnemyTexture5, px, py, pw, ph,
					g_Enemy5[i].u, g_Enemy5[i].v,
					g_Enemy5[i].uh, g_Enemy5[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// エネミー構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetEnemy5(void)
{
	return &g_Enemy5[0];
}

//=============================================================================
// 敵の発生処理
//=============================================================================
void SetEnemy5(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// もし未使用のデータを探す
		if (g_Enemy5[i].use == false)	// 未使用状態の敵データを見つける
		{
			g_Enemy5[i].use = true;		// 使用状態へ変更する
			return;						// 敵をセットできたので終了する
		}
	}
}

// 見つけたら
void WatchEnemy5(int i)
{
	// 旧版サーチ
	{
		//現在位置
		D3DXVECTOR2 position = g_Enemy5[i].pos;

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
				g_Enemy5[i].u = 1.0f;
				g_Enemy5[i].v = 0.0f;
				g_Enemy5[i].uh = -0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
			else // 右に行くとき
			{
				g_Enemy5[i].u = 0.5f;
				g_Enemy5[i].v = 0.0f;
				g_Enemy5[i].uh = 0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
		}
		else // 縦の移動量の方が大きい or 同じとき
		{
			// 上に行くとき
			if (direction.y < 0)
			{
				g_Enemy5[i].u = 0.5f;
				g_Enemy5[i].v = 0.5f;
				g_Enemy5[i].uh = 0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
			else // 下に行くとき
			{
				g_Enemy5[i].u = 0.0f;
				g_Enemy5[i].v = 0.5f;
				g_Enemy5[i].uh = 0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
		}

		//移動後の位置を計算
		g_Enemy5[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x + direction.x * FIND_SPD, g_Enemy5[i].pos.y))
			== 1)
		{
			g_Enemy5[i].pos.x -= direction.x * FIND_SPD;
			g_Enemy5[i].notmove.x += fabsf(direction.x);

			if (!g_Enemy5[i].movecntX)
			{
				// 一定時間動けない（障害物に引っかかる）
				if (g_Enemy5[i].notmove.x > CHIP_SIZE)
				{
					g_Enemy5[i].movecntX = true;
					g_Enemy5[i].notmove.x = 0.0f;
				}
			}
		}

		g_Enemy5[i].pos.y += direction.y * FIND_SPD;
		if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x, g_Enemy5[i].pos.y + direction.y * FIND_SPD))
			== 1)
		{
			g_Enemy5[i].pos.y -= direction.y * FIND_SPD;
			g_Enemy5[i].notmove.y += fabsf(direction.y);

			if (!g_Enemy5[i].movecntY)
			{
				// 一定時間動けない（障害物に引っかかる）
				if (g_Enemy5[i].notmove.y > CHIP_SIZE)
				{
					g_Enemy5[i].movecntY = true;
					g_Enemy5[i].notmove.y = 0.0f;
				}
			}

		}

		// 一定時間動けない（障害物に引っかかる）
		if (g_Enemy5[i].movecntX)
		{
			g_Enemy5[i].addmove.y += ENEMY_SPD;

			g_Enemy5[i].pos.y += ENEMY_SPD;
			if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x, g_Enemy5[i].pos.y + ENEMY_SPD))
				== 1)
			{
				g_Enemy5[i].pos.y -= ENEMY_SPD;
			}

			// STOP_CNT分移動したら
			if (g_Enemy5[i].addmove.y > CHIP_SIZE * 3 / 2)
			{
				g_Enemy5[i].movecntX = false;
				g_Enemy5[i].addmove.y = 0.0f;
			}
		}
		if (g_Enemy5[i].movecntY)
		{
			g_Enemy5[i].addmove.x += ENEMY_SPD;

			g_Enemy5[i].pos.x += ENEMY_SPD;
			if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x + ENEMY_SPD, g_Enemy5[i].pos.y))
				== 1)
			{
				g_Enemy5[i].pos.x -= ENEMY_SPD;
			}

			// STOP_CNT分移動したら
			if (g_Enemy5[i].addmove.x > CHIP_SIZE * 3 / 2)
			{
				g_Enemy5[i].movecntY = false;
				g_Enemy5[i].addmove.x = 0.0f;
			}
		}

	}

}

// 見つけていなかったら
void NoneWatchEnemy5(int i)
{
	g_Enemy5[i].timer++;

	if (g_Enemy5[i].timer > MOVE_TIMER)
	{
		g_Enemy5[i].move = rand() % 5;
		g_Enemy5[i].timer = 0;
		srand((unsigned)time(NULL));
	}

	// エネミー移動
	// 1:上　2:下　3:右　4:左　0:なし(待機)
	if (g_Enemy5[i].move == 1) // 上
	{
		g_Enemy5[i].pos.y -= g_Enemy5[i].vel.y;
		if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x, g_Enemy5[i].pos.y - g_Enemy5[i].vel.y))
			== 1)
		{
			g_Enemy5[i].pos.y += g_Enemy5[i].vel.y;
		}
		g_Enemy5[i].u = 0.5f;
		g_Enemy5[i].v = 0.5f;
		g_Enemy5[i].uh = 0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

	if (g_Enemy5[i].move == 2) // 下
	{
		g_Enemy5[i].pos.y += g_Enemy5[i].vel.y;
		if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x, g_Enemy5[i].pos.y + g_Enemy5[i].vel.y))
			== 1)
		{
			g_Enemy5[i].pos.y -= g_Enemy5[i].vel.y;
		}
		g_Enemy5[i].u = 0.0f;
		g_Enemy5[i].v = 0.5f;
		g_Enemy5[i].uh = 0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

	if (g_Enemy5[i].move == 3) // 右
	{
		g_Enemy5[i].pos.x += g_Enemy5[i].vel.x;
		if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x + g_Enemy5[i].vel.x, g_Enemy5[i].pos.y))
			== 1)
		{
			g_Enemy5[i].pos.x -= g_Enemy5[i].vel.x;
		}
		g_Enemy5[i].u = 0.5f;
		g_Enemy5[i].v = 0.0f;
		g_Enemy5[i].uh = 0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

	if (g_Enemy5[i].move == 4) // 左
	{
		g_Enemy5[i].pos.x -= g_Enemy5[i].vel.x;
		if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x - g_Enemy5[i].vel.x, g_Enemy5[i].pos.y))
			== 1)
		{
			g_Enemy5[i].pos.x += g_Enemy5[i].vel.x;
		}
		g_Enemy5[i].u = 1.0f;
		g_Enemy5[i].v = 0.0f;
		g_Enemy5[i].uh = -0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

	if (g_Enemy5[i].move == 0) // 待機
	{
		g_Enemy5[i].u = 0.0f;
		g_Enemy5[i].v = 0.0f;
		g_Enemy5[i].uh = 0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

}

// 範囲内にプレイヤーがら入り込んだら
bool SerchPlayer5(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
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