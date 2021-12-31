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
#define ROOT_NUM			(2)
#define ROOT_NUM2			(0)
#define ROOT_NUM3			(8)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetEnemy5(void);
void WatchEnemy5(int i);
void NoneWatchEnemy5(int i);
bool SerchPlayer5(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_EnemyTexture5 = 0;
unsigned char	g_watch_SE5 = 0;
unsigned char	g_watch_lost_SE = 0;

// �������m�ۗp�A�h���X
ENEMY* g_Enemy5;				  // �G�l�~�[�\����

char* g_RouteX;		  // ���񃋁[�g�p
char* g_RouteY;		  // ���񃋁[�g�p

char* g_RouteX;		  // ���񃋁[�g�p
char* g_RouteY;		  // ���񃋁[�g�p

char* g_RouteX;		  // ���񃋁[�g�p
char* g_RouteY;		  // ���񃋁[�g�p

char* g_RouteX;		  // ���񃋁[�g�p
char* g_RouteY;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy5(void)
{
	g_EnemyTexture5 = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE5 = LoadSound("data/SE/watch1.wav");

	// �������m��
	g_Enemy5 = new ENEMY[ENEMY_MAX];

	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// ��̖�
		if (i == 0)
		{
			g_Enemy5[i].use = true;
			g_Enemy5[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 6;
			g_Enemy5[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 16;
		}
		// ��̖�
		if (i == 1)
		{
			g_Enemy5[i].use = false;
			g_Enemy5[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 16;
			g_Enemy5[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 14;
		}
		// �O�̖�
		if (i == 2)
		{
			g_Enemy5[i].use = false;
			g_Enemy5[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 23;
			g_Enemy5[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 30;
		}

		// ����
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

		// �G�l�~�[UV
		g_Enemy5[i].u = 0.0f;
		g_Enemy5[i].v = 0.0f;
		g_Enemy5[i].uh = 0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy5(void)
{
	// ���������
	delete[] g_Enemy5;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy5(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy5[i].use == true)
		{
			// �ǐՔ���
			if (SerchPlayer5(GetPlayerPosition(), g_Enemy5[i].pos))
			{// ������(�͈͓��ɓ�����)

				if (!g_Enemy5[i].watch)
				{
					PlaySound(g_watch_SE5, 0);
					g_Enemy5[i].watch = true;
				}

				WatchEnemy5(i);

			}
			// �����Ă��Ȃ�
			else
			{
				if (g_Enemy5[i].watch)
				{
					// �����������̏����iSE�j

				}

				g_Enemy5[i].watch = false;

				NoneWatchEnemy5(i);

			}
		}
	}
	// �G�l�~�[�ǉ�
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
// �`�揈��
//=============================================================================
void DrawEnemy5(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy5[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Enemy5[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Enemy5[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy5[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy5[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Enemy5[i].w;		// �G�l�~�[�̕\����
				float ph = g_Enemy5[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				if (SerchPlayer5(GetPlayerPosition(), g_Enemy5[i].pos))
				{
					col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_EnemyTexture5, px, py, pw, ph,
					g_Enemy5[i].u, g_Enemy5[i].v,
					g_Enemy5[i].uh, g_Enemy5[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY* GetEnemy5(void)
{
	return &g_Enemy5[0];
}

//=============================================================================
// �G�̔�������
//=============================================================================
void SetEnemy5(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// �������g�p�̃f�[�^��T��
		if (g_Enemy5[i].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
		{
			g_Enemy5[i].use = true;		// �g�p��Ԃ֕ύX����
			return;						// �G���Z�b�g�ł����̂ŏI������
		}
	}
}

// ��������
void WatchEnemy5(int i)
{
	// ���ŃT�[�`
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Enemy5[i].pos;

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
				g_Enemy5[i].u = 1.0f;
				g_Enemy5[i].v = 0.0f;
				g_Enemy5[i].uh = -0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Enemy5[i].u = 0.5f;
				g_Enemy5[i].v = 0.0f;
				g_Enemy5[i].uh = 0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Enemy5[i].u = 0.5f;
				g_Enemy5[i].v = 0.5f;
				g_Enemy5[i].uh = 0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Enemy5[i].u = 0.0f;
				g_Enemy5[i].v = 0.5f;
				g_Enemy5[i].uh = 0.5f;
				g_Enemy5[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Enemy5[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x + direction.x * FIND_SPD, g_Enemy5[i].pos.y))
			== 1)
		{
			g_Enemy5[i].pos.x -= direction.x * FIND_SPD;
			g_Enemy5[i].notmove.x += fabsf(direction.x);

			if (!g_Enemy5[i].movecntX)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
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
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Enemy5[i].notmove.y > CHIP_SIZE)
				{
					g_Enemy5[i].movecntY = true;
					g_Enemy5[i].notmove.y = 0.0f;
				}
			}

		}

		// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
		if (g_Enemy5[i].movecntX)
		{
			g_Enemy5[i].addmove.y += ENEMY_SPD;

			g_Enemy5[i].pos.y += ENEMY_SPD;
			if (GetMapEnter5(D3DXVECTOR2(g_Enemy5[i].pos.x, g_Enemy5[i].pos.y + ENEMY_SPD))
				== 1)
			{
				g_Enemy5[i].pos.y -= ENEMY_SPD;
			}

			// STOP_CNT���ړ�������
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

			// STOP_CNT���ړ�������
			if (g_Enemy5[i].addmove.x > CHIP_SIZE * 3 / 2)
			{
				g_Enemy5[i].movecntY = false;
				g_Enemy5[i].addmove.x = 0.0f;
			}
		}

	}

}

// �����Ă��Ȃ�������
void NoneWatchEnemy5(int i)
{
	g_Enemy5[i].timer++;

	if (g_Enemy5[i].timer > MOVE_TIMER)
	{
		g_Enemy5[i].move = rand() % 5;
		g_Enemy5[i].timer = 0;
		srand((unsigned)time(NULL));
	}

	// �G�l�~�[�ړ�
	// 1:��@2:���@3:�E�@4:���@0:�Ȃ�(�ҋ@)
	if (g_Enemy5[i].move == 1) // ��
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

	if (g_Enemy5[i].move == 2) // ��
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

	if (g_Enemy5[i].move == 3) // �E
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

	if (g_Enemy5[i].move == 4) // ��
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

	if (g_Enemy5[i].move == 0) // �ҋ@
	{
		g_Enemy5[i].u = 0.0f;
		g_Enemy5[i].v = 0.0f;
		g_Enemy5[i].uh = 0.5f;
		g_Enemy5[i].vh = 0.5f;
	}

}

// �͈͓��Ƀv���C���[������荞�񂾂�
bool SerchPlayer5(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
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