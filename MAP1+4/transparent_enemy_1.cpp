//=============================================================================
//
// �G�l�~�[(����)���� [transparent_enemy.cpp]
// Author : �Îs
//
//=============================================================================
#include "transparent_enemy.h"
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
#define ROOT_NUM			(5)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetTransparent(int i);
void WatchTransparent(int i);
void NoneWatchTransparent(int i);
bool SerchPlayerTransparent(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_TransparentTexture = 0;
unsigned char	g_Transparent_watch_SE = 0;
unsigned char	g_Transparent_watch_lost_SE = 0;

// �������m�ۗp�A�h���X
ENEMY* g_Transparent;			  // �G�l�~�[�\����
char* g_Transparent_RouteX;		  // ���񃋁[�g�p
char* g_Transparent_RouteY;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTransparent(void)
{
	g_TransparentTexture = LoadTexture("data/TEXTURE/enemy.png");
	g_Transparent_watch_SE = LoadSound("data/SE/watch1.wav");
	g_Transparent_watch_lost_SE = LoadSound("data/SE/enemy_worp.wav");

	// �������m��
	g_Transparent = new ENEMY[TRANSPARENT_MAX];

	g_Transparent_RouteX = new char[ROOT_MAX];
	g_Transparent_RouteY = new char[ROOT_MAX];

	// ���񃋁[�g�ݒ�
	for (char r = 0; r < ROOT_MAX; r++)
	{
		g_Transparent_RouteX[r] = SetRouteX(r);
		g_Transparent_RouteY[r] = SetRouteY(r);
	}

	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < TRANSPARENT_MAX; i++)
	{
		// ��̖�
		if (i == 0)
		{
			g_Transparent[i].use = true;
			g_Transparent[i].routenom = ROOT_NUM;
			g_Transparent[i].pos.x = g_Transparent_RouteX[ROOT_NUM] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Transparent[i].pos.y = g_Transparent_RouteY[ROOT_NUM] * CHIP_SIZE + (CHIP_SIZE / 2);

		}

		// ����
		g_Transparent[i].watch = false;
		g_Transparent[i].w = ENEMY_SIZE;
		g_Transparent[i].h = ENEMY_SIZE;
		g_Transparent[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
		g_Transparent[i].notmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Transparent[i].addmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Transparent[i].movecntX = false;
		g_Transparent[i].movecntY = false;

		// �G�l�~�[UV
		g_Transparent[i].u = 0.0f;
		g_Transparent[i].v = 0.0f;
		g_Transparent[i].uh = 0.5f;
		g_Transparent[i].vh = 0.5f;
	}



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTransparent(void)
{
	// ���������
	delete[] g_Transparent;
	delete[] g_Transparent_RouteX;
	delete[] g_Transparent_RouteY;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTransparent(void)
{
	for (int i = 0; i < TRANSPARENT_MAX; i++)
	{
		if (g_Transparent[i].use == true)
		{
			// �ǐՔ���
			if (SerchPlayerTransparent(GetPlayerPosition(), g_Transparent[i].pos))
			{// ������(�͈͓��ɓ�����)

				if (!g_Transparent[i].watch)
				{
					PlaySound(g_Transparent_watch_SE, 0);
					g_Transparent[i].watch = true;
				}

				WatchTransparent(i);

			}
			// �����Ă��Ȃ�
			else
			{
				if (g_Transparent[i].watch)
				{
					// �����������̏����iSE�j
					PlaySound(g_Transparent_watch_lost_SE, 0);

					char work = rand() % ROOT_MAX;

					g_Transparent[i].routenom = work;
					g_Transparent[i].pos.x = SetRouteX(work) * CHIP_SIZE + (CHIP_SIZE / 2);
					g_Transparent[i].pos.y = SetRouteY(work) * CHIP_SIZE + (CHIP_SIZE / 2);
				}

				g_Transparent[i].watch = false;

				NoneWatchTransparent(i);
			}
		}
	}
	//// �G�l�~�[�ǉ�
	//if (GetTimer() == 20)
	//{
	//	SetTransparent(1);
	//}
	//if (GetTimer() == 10)
	//{
	//	SetTransparent(2);
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTransparent(void)
{
	for (int i = 0; i < TRANSPARENT_MAX; i++)
	{
		if (g_Transparent[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Transparent[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Transparent[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Transparent[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Transparent[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Transparent[i].w;		// �G�l�~�[�̕\����
				float ph = g_Transparent[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				if (SerchPlayerTransparent(GetPlayerPosition(), g_Transparent[i].pos))
				{
					col = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				}
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_TransparentTexture, px, py, pw, ph,
					g_Transparent[i].u, g_Transparent[i].v,
					g_Transparent[i].uh, g_Transparent[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// �G�̔�������
//=============================================================================
void SetTransparent(int i)
{
	// �������g�p�̃f�[�^��T��
	if (g_Transparent[i].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
	{
		g_Transparent[i].use = true;		// �g�p��Ԃ֕ύX����

		// �o�������Ƃ���SE

		return;						// �G���Z�b�g�ł����̂ŏI������
	}
}

// ��������
void WatchTransparent(int i)
{
	// ���ŃT�[�`
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Transparent[i].pos;

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
				g_Transparent[i].u = 1.0f;
				g_Transparent[i].v = 0.0f;
				g_Transparent[i].uh = -0.5f;
				g_Transparent[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Transparent[i].u = 0.5f;
				g_Transparent[i].v = 0.0f;
				g_Transparent[i].uh = 0.5f;
				g_Transparent[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Transparent[i].u = 0.5f;
				g_Transparent[i].v = 0.5f;
				g_Transparent[i].uh = 0.5f;
				g_Transparent[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Transparent[i].u = 0.0f;
				g_Transparent[i].v = 0.5f;
				g_Transparent[i].uh = 0.5f;
				g_Transparent[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Transparent[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Transparent[i].pos.x + direction.x * FIND_SPD, g_Transparent[i].pos.y))
			== 1)
		{
			g_Transparent[i].pos.x -= direction.x * FIND_SPD;
			g_Transparent[i].notmove.x += fabsf(direction.x);

			if (!g_Transparent[i].movecntX)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Transparent[i].notmove.x > NONE_MOVE)
				{
					g_Transparent[i].movecntX = true;
					g_Transparent[i].notmove.x = 0.0f;
				}
			}
		}

		g_Transparent[i].pos.y += direction.y * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Transparent[i].pos.x, g_Transparent[i].pos.y + direction.y * FIND_SPD))
			== 1)
		{
			g_Transparent[i].pos.y -= direction.y * FIND_SPD;
			g_Transparent[i].notmove.y += fabsf(direction.y);

			if (!g_Transparent[i].movecntY)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Transparent[i].notmove.y > NONE_MOVE)
				{
					g_Transparent[i].movecntY = true;
					g_Transparent[i].notmove.y = 0.0f;
				}
			}

		}

		// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
		if (g_Transparent[i].movecntX)
		{
			g_Transparent[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_Transparent_watch_lost_SE, 0);

			char work = rand() % ROOT_MAX;

			g_Transparent[i].routenom = work;
			g_Transparent[i].pos.x = g_Transparent_RouteX[work] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Transparent[i].pos.y = g_Transparent_RouteY[work] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		if (g_Transparent[i].movecntY)
		{
			g_Transparent[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_Transparent_watch_lost_SE, 0);

			char work = rand() % ROOT_MAX;

			g_Transparent[i].routenom = work;
			g_Transparent[i].pos.x = g_Transparent_RouteX[work] * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Transparent[i].pos.y = g_Transparent_RouteY[work] * CHIP_SIZE + (CHIP_SIZE / 2);
		}
	}
}

// �����Ă��Ȃ�������
void NoneWatchTransparent(int i)
{
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Transparent[i].pos;

		char work = g_Transparent[i].routenom;

		if (g_Transparent[i].pos.x < g_Transparent_RouteX[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Transparent[i].pos.x > g_Transparent_RouteX[work] * CHIP_SIZE)
		{
			if (g_Transparent[i].pos.y < g_Transparent_RouteY[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Transparent[i].pos.y > g_Transparent_RouteY[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// ���� or 0
					work++;
					g_Transparent[i].routenom++;
					if (work >= ROOT_MAX)
					{
						work = 0;
						g_Transparent[i].routenom = 0;
					}
				}
				else
				{
					// �
					work--;
					g_Transparent[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX;
						g_Transparent[i].routenom = ROOT_MAX;
					}
				}
			}
		}

		//�ڕW�n�_
		D3DXVECTOR2 destination = D3DXVECTOR2(g_Transparent_RouteX[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_Transparent_RouteY[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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
				g_Transparent[i].u = 1.0f;
				g_Transparent[i].v = 0.0f;
				g_Transparent[i].uh = -0.5f;
				g_Transparent[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Transparent[i].u = 0.5f;
				g_Transparent[i].v = 0.0f;
				g_Transparent[i].uh = 0.5f;
				g_Transparent[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Transparent[i].u = 0.5f;
				g_Transparent[i].v = 0.5f;
				g_Transparent[i].uh = 0.5f;
				g_Transparent[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Transparent[i].u = 0.0f;
				g_Transparent[i].v = 0.5f;
				g_Transparent[i].uh = 0.5f;
				g_Transparent[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Transparent[i].pos.x += direction.x * ENEMY_SPD;
		g_Transparent[i].pos.y += direction.y * ENEMY_SPD;

	}
}

// �͈͓��Ƀv���C���[�����荞�񂾂�
bool SerchPlayerTransparent(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
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
ENEMY* GetTransparent(void)
{
	return g_Transparent;
}
