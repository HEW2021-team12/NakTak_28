/*==============================================================================

   ���[�v [warp.cpp]
	Author :	�Îs
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
// �}�N����`
//*****************************************************************************

// �P��
#define WARP_POS_X1		(23)
#define WARP_POS_Y1		(9)

#define WARP_POS_X2		(8)
#define WARP_POS_Y2		(4)

// �Q��
#define WARP_POS2_X1	(5)
#define WARP_POS2_Y1	(9)

#define WARP_POS2_X2	(18)
#define WARP_POS2_Y2	(14)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

bool WarpCollision(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

unsigned char	g_WarpTexture = 0;
unsigned char	g_WarpSE = 0;
unsigned char	g_WarpSetSE = 0;

// �������m�ۗp�A�h���X
WARP** g_Warp;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitWarp(void)
{
	g_WarpTexture = LoadTexture("data/TEXTURE/warp.png");
	g_WarpSE = LoadSound("data/SE/warp.wav");
	g_WarpSetSE = LoadSound("data/SE/warp_set.wav");

	// �������m��
	g_Warp = new WARP * [WARP_MAX];
	for (int i = 0; i < WARP_MAX; i++)
	{
		g_Warp[i] = new WARP[WARP_PAIR];
	}

	// ���[�v�\���̂̏�����
	for (int i = 0; i < WARP_MAX; i++)
	{
		for (int j = 0; j < WARP_PAIR; j++)
		{
			// ����
			g_Warp[i][j].use = true;		// ���������ɂ�true
			g_Warp[i][j].w = WARP_SIZE;
			g_Warp[i][j].h = WARP_SIZE;
			g_Warp[i][j].u = 0.0f;
			g_Warp[i][j].v = 0.0f;
			g_Warp[i][j].uh = 1.0f;
			g_Warp[i][j].vh = 1.0f;
			g_Warp[i][j].setcount = 0;
		}

		// �P��
		if (i == 0)
		{
			// �΂̕Е�
			g_Warp[i][0].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_X1;
			g_Warp[i][0].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_Y1;

			// �����Е�
			g_Warp[i][1].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_X2;
			g_Warp[i][1].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS_Y2;
		}

		// �Q��
		if (i == 1)
		{
			// �΂̕Е�
			g_Warp[i][0].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_X1;
			g_Warp[i][0].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_Y1;

			// �����Е�
			g_Warp[i][1].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_X2;
			g_Warp[i][1].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * WARP_POS2_Y2;
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitWarp(void)
{
	// ���������
	delete[] g_Warp;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWarp(void)
{
	for (int i = 0; i < WARP_MAX; i++)
	{
		// �΂̂ǂ��炩��true�ł���Ƃ�(��{�ǂ����use�͓����悤�ɂ��Ă邪)
		if (g_Warp[i][0].use || g_Warp[i][1].use)
		{
			PLAYER* player = GetPlayer();

			// �΂̕Е�
			if (WarpCollision(player->pos, g_Warp[i][0].pos,
				player->size, D3DXVECTOR2(g_Warp[i][0].w, g_Warp[i][0].h)))
			{
				player->pos = g_Warp[i][1].pos;
				player->vel.x = 0.0f;
				player->vel.y = 0.0f;
				g_Warp[i][0].use = false;
				g_Warp[i][1].use = false;

				// ���[�v�����Ƃ����T�E���h
				PlaySound(g_WarpSE, 0);
			}
			// �����Е�
			else if (WarpCollision(player->pos, g_Warp[i][1].pos,
				player->size, D3DXVECTOR2(g_Warp[i][1].w, g_Warp[i][1].h)))
			{
				player->pos = g_Warp[i][0].pos;
				player->vel.x = 0.0f;
				player->vel.y = 0.0f;
				g_Warp[i][0].use = false;
				g_Warp[i][1].use = false;

				// ���[�v�����Ƃ����T�E���h
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

				// �Z�b�g�����Ƃ����T�E���h
				PlaySound(g_WarpSetSE, 0);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWarp(void)
{
	for (int i = 0; i < WARP_MAX; i++)
	{
		for (int j = 0; j < WARP_PAIR; j++)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Warp[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Warp[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Warp[i][j].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Warp[i][j].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Warp[i][j].w;		// �G�l�~�[�̕\����
				float ph = g_Warp[i][j].h;		// �G�l�~�[�̕\������
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

// �v���C���[�ƃ��[�v�̓����蔻��
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

	//X���̔���
	if (min1.x < max2.x &&
		max1.x > min2.x)
	{
		//Y���̔���
		if (min1.y < max2.y &&
			max1.y > min2.y)
		{
			//�S�Ă̏�����TRUE�Ȃ�q�b�g����
			return true;
		}
	}

	return false;
}