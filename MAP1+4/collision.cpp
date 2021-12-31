//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
#include "scene.h"
#include "slime.h"
#include "razer.h"
#include "transparent_enemy.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool CollisionBB(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);
bool CollisionBC(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, float size1, float size2);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY* enemy;
SLIME* slime;
BABBLE** babble;
RAZER* razer;
ENEMY* transparent;

unsigned char	g_Slime_SE = 0;
unsigned char	g_Razer_SE = 0;

bool			g_PenaltySlime;
bool			g_PenaltyBabble;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCollision(void)
{
	g_Slime_SE = LoadSound("data/SE/slime.wav");
	g_Razer_SE = LoadSound("data/SE/razer.wav");

	g_PenaltySlime = false;
	g_PenaltyBabble = false;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void UpdateCollision(void)
{
	PLAYER* player = GetPlayer();	// プレイヤーのポインターを初期化
	int enemy_max = 0;
	int slime_max = 0;
	int babble_max = 0;
	int razer_max = 0;
	int transparent_max = 0;

	// ステージ１
	if (GetScene() == SCENE_GAME1)
	{
		enemy = GetEnemy();		// エネミーのポインターを初期化
		enemy_max = ENEMY_MAX;

		slime = GetSlime();
		slime_max = SLIME_MAX;

		babble = GetBabble();
		babble_max = BABBLE_MAX;

		razer = GetRazer();
		razer_max = RAZER_MAX;

		transparent = GetTransparent();
		transparent_max = TRANSPARENT_MAX;

		// ステージ１判定まとめ
		{
			// 敵と操作キャラ(BB)
			for (int i = 0; i < enemy_max; i++)
			{
				if (enemy[i].use == false)
					continue;

				if (CollisionBC(player->pos, enemy[i].pos, player->size.y / 2, enemy[i].h / 2))
				{
					// 操作キャラクターは敗北
					SceneTransition(SCENE_LOSE);
				}
			}

			// プレイヤーとスライム
			for (int i = 0; i < slime_max; i++)
			{
				if (slime[i].use)
				{
					if (CollisionBB(player->pos, slime[i].pos, player->size, D3DXVECTOR2(slime[i].w, slime[i].h)))
					{
						player->penalty = true;

						// サウンド再生
						if (!g_PenaltySlime)
						{
							PlaySound(g_Slime_SE, 0);
							g_PenaltySlime = true;
						}
					}
					else
					{
						g_PenaltySlime = false;
					}
				}

				// プレイヤーとバブル
				for (int j = 0; j < babble_max; j++)
				{
					if (babble[i][j].use)
					{
						if (CollisionBB(player->pos, babble[i][j].pos, player->size, D3DXVECTOR2(babble[i][j].w, babble[i][j].h)))
						{
							player->penalty = true;

							// サウンド再生
							if (!g_PenaltyBabble)
							{
								PlaySound(g_Slime_SE, 0);
								g_PenaltyBabble = true;
							}
						}
						else
						{
							g_PenaltyBabble = false;
						}
					}
				}
			}

			// プレイヤーとレーザー
			for (int i = 0; i < razer_max; i++)
			{
				if (razer[i].use)
				{
					if (CollisionBB(player->pos, razer[i].pos, player->size, D3DXVECTOR2(razer[i].w, razer[i].h)))
					{
						player->penalty = true;

						// サウンド再生
						PlaySound(g_Razer_SE, 0);
					}
				}
			}

			// プレイヤーとエネミー(透明)
			for (int i = 0; i < transparent_max; i++)
			{
				if (transparent[i].use)
				{
					if (CollisionBB(player->pos, transparent[i].pos, player->size, D3DXVECTOR2(transparent[i].w, transparent[i].h)))
					{
						// 操作キャラクターは敗北
						SceneTransition(SCENE_LOSE);
					}
				}
			}
		}
	}

	// ステージ２
	if (GetScene() == SCENE_GAME2)
	{
		enemy = GetEnemy2();	// エネミーのポインターを初期化
		enemy_max = ENEMY_MAX2;

		// ステージ２判定まとめ
		{
			// ここに１のようにまとめておくように

		}
	}
}

//=============================================================================
// BBによる当たり判定処理
// 回転は考慮しない
// 戻り値：当たってたらtrue
//=============================================================================
bool CollisionBB(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
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

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CollisionBC(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, float size1, float size2)
{
	//pos1とpos2をつなぐベクトルを作る
	D3DXVECTOR2 vDistance = pos1 - pos2;

	//vDistanceのベクトル長を得る
	float length;
	//	length = D3DXVec2Length(&vDistance);
	length = D3DXVec2LengthSq(&vDistance);

	//	float size = size1 + size2;
	float size = (size1 + size2) * (size1 + size2);

	//1と2の半径を足して判定する
	//差分のベクトル長の方が小さければ
	//ヒットしている
	if (length < size)
	{
		return true;
	}

	return false;
}