/*==============================================================================

   �Q�[������ [game.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/

#include "game.h"
#include "main.h"
#include "input.h"
#include "keyboard.h"
#include "fade.h"
#include "player.h"
#include "collision.h"
#include "enemy.h"
#include "map.h"
#include "sound.h"
#include "timer.h"
#include "slime.h"
#include "razer.h"
#include "warp.h"
#include "transparent_enemy.h"

/*------------------------------------------------------------------------------
   �萔��`
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   �\���̐錾
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   �v���g�^�C�v�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
   �O���[�o���ϐ��̒�`
------------------------------------------------------------------------------*/
unsigned char g_BGMNo4 = 0;


/*------------------------------------------------------------------------------
   �������֐�
------------------------------------------------------------------------------*/
void InitGame4(void)
{
	InitPlayer();
	InitEnemy4();
	//InitTransparent4();
	//InitSlime4();
	//InitRazer4();
	//InitWarp4();
	InitMap4();
	InitTimer();
	InitCollision();

	g_BGMNo4 = LoadSound("data/BGM/BGM.wav");
	PlaySound(g_BGMNo4, 255);
}

/*------------------------------------------------------------------------------
   �I������������֐�
------------------------------------------------------------------------------*/
void UninitGame4()
{
	StopSoundAll();

	//�������Ƃ͋t���ɏI���������s��
	UninitMap4();
	UninitPlayer();
	//UninitTransparent4();
	//UninitSlime4();
	//UninitRazer4();
	//UninitWarp4();
	UninitEnemy4();
	UninitTimer();
}

/*------------------------------------------------------------------------------
   �X�V����������֐�
------------------------------------------------------------------------------*/
void UpdateGame4(void)
{
	UpdateMap4();
	UpdateEnemy4();
	UpdatePlayer();
	//UpdateTransparent4();
	//UpdateSlime4();
	//UpdateRazer4();
	//UpdateWarp4();

	UpdateCollision();
	UpdateTimer();

	//�X�y�[�X�L�[��������Ă��āA�t�F�[�h�������ł͂Ȃ��Ƃ�
	if (Keyboard_IsKeyDown(KK_ENTER) ||
		IsButtonTriggered(0, XINPUT_GAMEPAD_B)
		&& GetFadeState() == FADE_NONE)
	{
		//RESULT�ֈڍs����
		SceneTransition(SCENE_RESULT);
	}
}

/*------------------------------------------------------------------------------
   �`�揈��������֐�
------------------------------------------------------------------------------*/
void DrawGame4(void)
{
	DrawMap4();
	//DrawTransparent4();
	//DrawSlime4();
	//DrawRazer4();
	//DrawWarp4();
	DrawPlayer();
	DrawEnemy4();
	DrawTimer();
}