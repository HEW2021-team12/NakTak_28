/*==============================================================================

   ���[�U�[ [razer.cpp]
	Author :	�Îs
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "main.h"
#include "map.h"
#include <time.h>
#include "sound.h"
#include "timer.h"
#include "razer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define RAZER_SIZEX			(200.0f)
#define RAZER_SIZEY			(100.0f)

// �T�C�Y��ݒu�ꏊ�ŕύX�������Ȃ炱���Ő錾���Ă����Ə��������邱��


#define RAZER_COUNT			(250)		// use��false�ɂȂ葱����J�E���g
#define RAZER_KEEP			(400)		// use��true�ɂȂ葱����J�E���g

#define RAZER_POS_X1		(13)
#define RAZER_POS_Y1		(9)

#define RAZER_POS_X2		(3)
#define RAZER_POS_Y2		(4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

unsigned char	g_RazerTexture = 0;

// �������m�ۗp�A�h���X
RAZER* g_Razer;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitRazer(void)
{
	g_RazerTexture = LoadTexture("data/TEXTURE/razer.png");

	// �������m��
	g_Razer = new RAZER[RAZER_MAX];

	// ���[�U�[�\���̂̏�����
	for (int i = 0; i < RAZER_MAX; i++)
	{
		g_Razer[i].use = false;
		g_Razer[i].w = RAZER_SIZEX;
		g_Razer[i].h = RAZER_SIZEY;

		g_Razer[i].u = 0.0f;
		g_Razer[i].v = 0.0f;
		g_Razer[i].uh = RAZER_U;
		g_Razer[i].vh = RAZER_V;

		g_Razer[i].setcount = 0;

		// ���
		if (i == 0)
		{
			g_Razer[i].pos.x = CHIP_SIZE * RAZER_POS_X1;
			g_Razer[i].pos.y = CHIP_SIZE * RAZER_POS_Y1;
		}
		// ���
		if (i == 1)
		{
			g_Razer[i].pos.x = CHIP_SIZE * RAZER_POS_X2;
			g_Razer[i].pos.y = CHIP_SIZE * RAZER_POS_Y2;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitRazer(void)
{
	// ���������
	delete[] g_Razer;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRazer(void)
{
	for (int i = 0; i < RAZER_MAX; i++)
	{
		if (g_Razer[i].use)
		{
			g_Razer[i].setcount++;

			if (g_Razer[i].setcount > RAZER_KEEP)
			{
				g_Razer[i].use = false;
				g_Razer[i].setcount = 0;
			}
		}
		else
		{
			g_Razer[i].setcount++;

			if (g_Razer[i].setcount > RAZER_COUNT)
			{
				g_Razer[i].changecount = g_Razer[i].setcount - RAZER_COUNT;

				g_Razer[i].u = g_Razer[i].changecount * RAZER_U;

				if (g_Razer[i].changecount >= RAZER_NUM)
				{
					g_Razer[i].changecount = 0;
					g_Razer[i].setcount = 0;
					g_Razer[i].use = true;
				}
			}
			else
			{
				g_Razer[i].u = 0.0f;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRazer(void)
{

	PLAYER* player = GetPlayer();

	for (int i = 0; i < RAZER_MAX; i++)
	{
		// ��ʓ��ɓ����Ă��邩
		if (fabsf(g_Razer[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
			fabsf(g_Razer[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
		{

			//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Razer[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
			float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Razer[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
			float pw = g_Razer[i].w;		// �G�l�~�[�̕\����
			float ph = g_Razer[i].h;		// �G�l�~�[�̕\������
			D3DXCOLOR col;

			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (g_Razer[i].use)
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_RazerTexture, px, py, pw, ph,
					RAZER_U * (RAZER_NUM - 1), g_Razer[i].v,
					g_Razer[i].uh, g_Razer[i].vh,
					col);
			}
			else
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_RazerTexture, px, py, pw, ph,
					g_Razer[i].u, g_Razer[i].v,
					g_Razer[i].uh, g_Razer[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// ���[�U�[�\���̂̐擪�A�h���X���擾
//=============================================================================
RAZER* GetRazer(void)
{
	return g_Razer;
}