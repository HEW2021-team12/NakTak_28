//=============================================================================
//
// �X���C������ [slime.cpp]
// Author : �Îs
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
// �}�N����`
//*****************************************************************************

#define SLIME_SPD			(1.1f)
#define SLIME_SIZEX			(70.0f)
#define SLIME_SIZEY			(50.0f)

#define BABBLE_SIZE			(40.0f)
#define BABBLE_COUNT		(300)

#define SET_SPD				(150)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetBabble4(int i, D3DXVECTOR2 pos);
void MoveSlime4(int i);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_SlimeTexture4 = 0;

// �������m�ۗp�A�h���X
SLIME* g_Slime4;			  // �X���C���\����
BABBLE** g_Babble4;			  // �o�u���\����
char* g_RouteSlimeX4;		  // ���񃋁[�g�p
char* g_RouteSlimeY4;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSlime4(void)
{
	g_SlimeTexture4 = LoadTexture("data/TEXTURE/slime.png");

	// �������m��
	g_Slime4 = new SLIME[SLIME_MAX4];
	g_Babble4 = new BABBLE * [SLIME_MAX4];

	for (int i = 0; i < SLIME_MAX4; i++)
	{
		g_Babble4[i] = new BABBLE[BABBLE_MAX];
	}

	g_RouteSlimeX4 = new char[ROOT_MAX4_1];
	g_RouteSlimeY4 = new char[ROOT_MAX4_1];

	// �X���C���\���̂̏�����
	for (int i = 0; i < SLIME_MAX4; i++)
	{
		// �P�̖�
		if (i == 0)
		{
			g_Slime4[i].use = true;
			g_Slime4[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime4[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime4[i].routenom = 5;
		}

		// ����
		g_Slime4[i].h = SLIME_SIZEY;
		g_Slime4[i].w = SLIME_SIZEX;
		g_Slime4[i].vel = D3DXVECTOR2(SLIME_SPD, SLIME_SPD);

		// �X���C��UV
		g_Slime4[i].u = 0.0f;
		g_Slime4[i].v = 0.0f;
		g_Slime4[i].uh = 1.0f;
		g_Slime4[i].vh = 1.0f;
	}

	// ���񃋁[�g�ݒ�
	for (char r = 0; r < ROOT_MAX4_1; r++)
	{
		g_RouteSlimeX4[r] = SetRouteX4_1(r);
		g_RouteSlimeY4[r] = SetRouteY4_1(r);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSlime4(void)
{
	// ���������
	delete[] g_Slime4;
	delete[] g_Babble4;
	delete[] g_RouteSlimeX4;
	delete[] g_RouteSlimeY4;
}
//=============================================================================
// �X�V����
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
// �`�揈��
//=============================================================================
void DrawSlime4(void)
{
	// �X���C��
	for (int i = 0; i < SLIME_MAX4; i++)
	{
		// �o�u��
		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble4[i][j].use)
			{
				PLAYER* player = GetPlayer();

				// ��ʓ��ɓ����Ă��邩
				if (fabsf(g_Babble4[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
					fabsf(g_Babble4[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
				{
					//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Babble4[i][j].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
					float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Babble4[i][j].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
					float pw = g_Babble4[i][j].w;		// �G�l�~�[�̕\����
					float ph = g_Babble4[i][j].h;		// �G�l�~�[�̕\������
					D3DXCOLOR col;

					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					DrawSpriteColor(g_SlimeTexture4, px, py, pw, ph,
						g_Babble4[i][j].u, g_Babble4[i][j].v,
						g_Babble4[i][j].uh, g_Babble4[i][j].vh,
						col);
				}
			}
		}

		// �X���C��
		if (g_Slime4[i].use)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Slime4[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Slime4[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Slime4[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Slime4[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Slime4[i].w;		// �G�l�~�[�̕\����
				float ph = g_Slime4[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_SlimeTexture4, px, py, pw, ph,
					g_Slime4[i].u, g_Slime4[i].v,
					g_Slime4[i].uh, g_Slime4[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// �X���C���\���̂̐擪�A�h���X���擾
//=============================================================================
SLIME* GetSlime4(void)
{
	return g_Slime4;
}

BABBLE** GetBabble4(void)
{
	return g_Babble4;
}

// �o�u���𔭐�������
void SetBabble4(int i, D3DXVECTOR2 pos)
{
	for (int j = 0; j < BABBLE_MAX; j++)
	{
		// �������g�p�̃f�[�^��T��
		if (g_Babble4[i][j].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
		{
			g_Babble4[i][j].use = true;		// �g�p��Ԃ֕ύX����
			g_Babble4[i][j].pos = pos;
			g_Babble4[i][j].w = BABBLE_SIZE;
			g_Babble4[i][j].h = BABBLE_SIZE - 10.0f;

			g_Babble4[i][j].count = BABBLE_COUNT;

			g_Babble4[i][j].u = 0.5f;
			g_Babble4[i][j].v = 0.0f;
			g_Babble4[i][j].uh = 0.5f;
			g_Babble4[i][j].vh = 0.5f;

			// �o�������Ƃ���SE

			return;						// �G���Z�b�g�ł����̂ŏI������
		}
	}
}

// �X���C���̈ړ�
void MoveSlime4(int i)
{
	//���݈ʒu
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
				// ���� or 0
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
				// �
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

	//�ڕW�n�_
	D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX4[work] * CHIP_SIZE + (CHIP_SIZE / 2),
		g_RouteSlimeY4[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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
			g_Slime4[i].u = 0.0f;
			g_Slime4[i].v = 0.5f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
		else // �E�ɍs���Ƃ�
		{
			g_Slime4[i].u = 0.5f;
			g_Slime4[i].v = 0.5f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
	}
	else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
	{
		// ��ɍs���Ƃ�
		if (direction.y < 0)
		{
			g_Slime4[i].u = 0.5f;
			g_Slime4[i].v = 0.0f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
		else // ���ɍs���Ƃ�
		{
			g_Slime4[i].u = 0.0f;
			g_Slime4[i].v = 0.0f;
			g_Slime4[i].uh = 0.5f;
			g_Slime4[i].vh = 0.5f;
		}
	}

	//�ړ���̈ʒu���v�Z
	g_Slime4[i].pos.x += direction.x * g_Slime4[i].vel.x;
	g_Slime4[i].pos.y += direction.y * g_Slime4[i].vel.y;
}