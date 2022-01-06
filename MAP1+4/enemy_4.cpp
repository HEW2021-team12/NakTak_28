//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
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
// �}�N����`
//*****************************************************************************
#define ENEMY_SPD			(1.4f)
#define ENEMY_SIZE			(55.0f)
#define SERCH_RANGE			(220.0f)
#define FIND_SPD			(2.0f)
#define NONE_MOVE			(150.0f)

// �����ʒu�Ǘ��p
#define ROOT_NUM4_1			(2)
#define ROOT_NUM4_2			(0)
#define ROOT_NUM4_3			(8)
#define ROOT_NUM4_4			(1)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetEnemy4(int i);
void WatchEnemy4(int i);
void NoneWatchEnemy4(int i);
bool SerchPlayer4(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos); 

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_EnemyTexture4 = 0;
unsigned char	g_watch_SE4 = 0;
unsigned char	g_watch_lost_SE4 = 0;

// �������m�ۗp�A�h���X
ENEMY* g_Enemy4;				  // �G�l�~�[�\����

char* g_RouteX4_1;		  // ���񃋁[�g�p
char* g_RouteY4_1;		  // ���񃋁[�g�p

char* g_RouteX4_2;		  // ���񃋁[�g�p
char* g_RouteY4_2;		  // ���񃋁[�g�p

char* g_RouteX4_3;		  // ���񃋁[�g�p
char* g_RouteY4_3;		  // ���񃋁[�g�p

char* g_RouteX4_4;		  // ���񃋁[�g�p
char* g_RouteY4_4;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy4(void)
{
	g_EnemyTexture4 = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE4 = LoadSound("data/SE/watch1.wav");
	g_watch_lost_SE4 = LoadSound("data/SE/enemy_worp.wav");

	// �������m��
	g_Enemy4 = new ENEMY[ENEMY_MAX4];

	g_RouteX4_1 = new char[ROOT_MAX4_1];
	g_RouteY4_1 = new char[ROOT_MAX4_1];

	g_RouteX4_2 = new char[ROOT_MAX4_2];
	g_RouteY4_2 = new char[ROOT_MAX4_2];

	g_RouteX4_3 = new char[ROOT_MAX4_3];
	g_RouteY4_3 = new char[ROOT_MAX4_3];

	g_RouteX4_4 = new char[ROOT_MAX4_4];
	g_RouteY4_4 = new char[ROOT_MAX4_4];

	// ���񃋁[�g�ݒ�
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

	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < ENEMY_MAX4; i++)
	{
		// ��̖�
		if (i == 0)
		{
			g_Enemy4[i].use = true;
			g_Enemy4[i].routenom = ROOT_NUM4_1;

			g_Enemy4[i].pos.x = g_RouteX4_1[ROOT_NUM4_1] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_1[ROOT_NUM4_1] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		// ��̖�
		if (i == 1)
		{
			g_Enemy4[i].use = false;
			g_Enemy4[i].routenom = ROOT_NUM4_2;

			g_Enemy4[i].pos.x = g_RouteX4_2[ROOT_NUM4_2] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_2[ROOT_NUM4_2] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		// �O�̖�
		if (i == 2)
		{
			g_Enemy4[i].use = false;
			g_Enemy4[i].routenom = ROOT_NUM4_3;

			g_Enemy4[i].pos.x = g_RouteX4_3[ROOT_NUM4_3] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_3[ROOT_NUM4_3] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		// �l�̖�
		if (i == 3)
		{
			g_Enemy4[i].use = false;
			g_Enemy4[i].routenom = ROOT_NUM4_4;

			g_Enemy4[i].pos.x = g_RouteX4_4[ROOT_NUM4_4] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = g_RouteY4_4[ROOT_NUM4_4] * CHIP_SIZE + (CHIP_SIZE / 2);
		}


		// ����
		g_Enemy4[i].watch = false;
		g_Enemy4[i].w = ENEMY_SIZE;
		g_Enemy4[i].h = ENEMY_SIZE;
		g_Enemy4[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
		g_Enemy4[i].notmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy4[i].addmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy4[i].movecntX = false;
		g_Enemy4[i].movecntY = false;

		// �G�l�~�[UV
		g_Enemy4[i].u = 0.0f;
		g_Enemy4[i].v = 0.0f;
		g_Enemy4[i].uh = 0.5f;
		g_Enemy4[i].vh = 0.5f;
	}




	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy4(void)
{
	// ���������
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
// �X�V����
//=============================================================================
void UpdateEnemy4(void)
{
	for (int i = 0; i < ENEMY_MAX4; i++)
	{
		if (g_Enemy4[i].use == true)
		{
			// �ǐՔ���
			if (SerchPlayer4(GetPlayerPosition(), g_Enemy4[i].pos))
			{// ������(�͈͓��ɓ�����)

				if (!g_Enemy4[i].watch)
				{
					PlaySound(g_watch_SE4, 0);
					g_Enemy4[i].watch = true;
				}

				WatchEnemy4(i);

			}
			// �����Ă��Ȃ�
			else
			{
				if (i == 0)
				{
					if (g_Enemy4[i].watch)
					{
						// �����������̏����iSE�j
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
						// �����������̏����iSE�j
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
						// �����������̏����iSE�j
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
						// �����������̏����iSE�j
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
	// �G�l�~�[�ǉ�
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
// �`�揈��
//=============================================================================
void DrawEnemy4(void)
{
	for (int i = 0; i < ENEMY_MAX4; i++)
	{
		if (g_Enemy4[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Enemy4[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Enemy4[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy4[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy4[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Enemy4[i].w;		// �G�l�~�[�̕\����
				float ph = g_Enemy4[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				if (SerchPlayer4(GetPlayerPosition(), g_Enemy4[i].pos))
				{
					col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_EnemyTexture4, px, py, pw, ph,
					g_Enemy4[i].u, g_Enemy4[i].v,
					g_Enemy4[i].uh, g_Enemy4[i].vh,
					col);
			}
		}
	}
}


//=============================================================================
// �G�̔�������
//=============================================================================
void SetEnemy4(int i)
{
	// �������g�p�̃f�[�^��T��
	if (g_Enemy4[i].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
	{
		g_Enemy4[i].use = true;		// �g�p��Ԃ֕ύX����

		// �o�������Ƃ���SE

		return;						// �G���Z�b�g�ł����̂ŏI������
	}
}

// ��������
void WatchEnemy4(int i)
{
	// ���ŃT�[�`
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Enemy4[i].pos;

		//�ڕW�n�_
		D3DXVECTOR2 destination = GetPlayerPosition();

		//����
		D3DXVECTOR2 components;

		//�傫��
		float magnitude;

		//�����i�P�ʃx�N�g���j
		D3DXVECTOR2 direction;

		//�����v�Z
		components.x = destination.x - position.x;
		components.y = destination.y - position.y;

		//�傫���v�Z
		magnitude = (float)sqrt(components.x * components.x + components.y * components.y);

		//�����v�Z�i���K���j
		direction.x = components.x / magnitude;
		direction.y = components.y / magnitude;

		// ���̈ړ��ʂ̕����傫���Ƃ�
		if (fabsf(direction.x) > fabsf(direction.y))
		{
			// ���ɍs���Ƃ�
			if (direction.x < 0)
			{
				g_Enemy4[i].u = 1.0f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = -0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Enemy4[i].u = 0.0f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Enemy4[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy4[i].pos.x + direction.x * FIND_SPD, g_Enemy4[i].pos.y))
			== 1)
		{
			g_Enemy4[i].pos.x -= direction.x * FIND_SPD;
			g_Enemy4[i].notmove.x += fabsf(direction.x);

			if (!g_Enemy4[i].movecntX)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
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
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Enemy4[i].notmove.y > NONE_MOVE)
				{
					g_Enemy4[i].movecntY = true;
					g_Enemy4[i].notmove.y = 0.0f;
				}
			}

		}

		// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
		if (g_Enemy4[i].movecntX)
		{
			g_Enemy4[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_1;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);

		}
		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_1;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);


		}

		if (g_Enemy4[i].movecntX)
		{

			g_Enemy4[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_2;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_1;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntX)
		{

			g_Enemy4[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_3;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_3;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_3(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntX)
		{

			g_Enemy4[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_4;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy4[i].movecntY)
		{
			g_Enemy4[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE4, 0);

			char work = rand() % ROOT_MAX4_4;

			g_Enemy4[i].routenom = work;
			g_Enemy4[i].pos.x = SetRouteX4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy4[i].pos.y = SetRouteY4_4(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

	}

}

// �����Ă��Ȃ�������
void NoneWatchEnemy4(int i)
{
	{
		//���݈ʒu
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
					// ���� or 0
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
					// �
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
					// ���� or 0
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
					// �
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
					// ���� or 0
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
					// �
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
					// ���� or 0
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
					// �
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


		//�ڕW�n�_
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_1[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_1[work] * CHIP_SIZE + (CHIP_SIZE / 2));
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_2[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_2[work] * CHIP_SIZE + (CHIP_SIZE / 2));
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_3[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_3[work] * CHIP_SIZE + (CHIP_SIZE / 2));
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX4_4[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY4_4[work] * CHIP_SIZE + (CHIP_SIZE / 2));



		//����
		D3DXVECTOR2 components;

		//�傫��
		float magnitude;

		//�����i�P�ʃx�N�g���j
		D3DXVECTOR2 direction;

		//�����v�Z
		components.x = destination.x - position.x;
		components.y = destination.y - position.y;

		//�傫���v�Z
		magnitude = (float)sqrt(components.x * components.x + components.y * components.y);

		//�����v�Z�i���K���j
		direction.x = components.x / magnitude;
		direction.y = components.y / magnitude;

		// ���̈ړ��ʂ̕����傫���Ƃ�
		if (fabsf(direction.x) > fabsf(direction.y))
		{
			// ���ɍs���Ƃ�
			if (direction.x < 0)
			{
				g_Enemy4[i].u = 1.0f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = -0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.0f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Enemy4[i].u = 0.5f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Enemy4[i].u = 0.0f;
				g_Enemy4[i].v = 0.5f;
				g_Enemy4[i].uh = 0.5f;
				g_Enemy4[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Enemy4[i].pos.x += direction.x * ENEMY_SPD;
		g_Enemy4[i].pos.y += direction.y * ENEMY_SPD;

	}
}

// �͈͓��Ƀv���C���[�����荞�񂾂�
bool SerchPlayer4(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
{
	D3DXVECTOR2 maxRange, minRange;

	minRange.x = Enemypos.x - SERCH_RANGE;
	minRange.y = Enemypos.y - SERCH_RANGE;
	maxRange.x = Enemypos.x + SERCH_RANGE;
	maxRange.y = Enemypos.y + SERCH_RANGE;

	//X���̔���
	if (minRange.x < Playerpos.x &&
		maxRange.x > Playerpos.x)
	{
		//Y���̔���
		if (minRange.y < Playerpos.y &&
			maxRange.y > Playerpos.y)
		{
			//�S�Ă̏�����TRUE�Ȃ�q�b�g����
			return true;
		}
	}

	return false;
}


//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY* GetEnemy4(void)
{
	return &g_Enemy4[0];
}