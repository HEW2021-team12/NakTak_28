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
unsigned char g_BGMNo = 0;


/*------------------------------------------------------------------------------
   �������֐�
------------------------------------------------------------------------------*/
void InitGame(void)
{
	InitPlayer();
	InitEnemy();
	InitTransparent();
	InitSlime();
	InitRazer();
	InitWarp();
	InitMap();
	InitTimer();
	InitCollision();		// �T�E���h�ǉ��p

	g_BGMNo = LoadSound("data/BGM/BGM.wav");
	PlaySound(g_BGMNo, 255);
}

/*------------------------------------------------------------------------------
   �I������������֐�
------------------------------------------------------------------------------*/
void UninitGame()
{
	StopSoundAll();

	//�������Ƃ͋t���ɏI���������s��
	UninitMap();
	UninitPlayer();
	UninitTransparent();
	//UninitEnemy();
	UninitSlime();
	UninitWarp();
	UninitRazer();
	UninitTimer();
}

/*------------------------------------------------------------------------------
   �X�V����������֐�
------------------------------------------------------------------------------*/
void UpdateGame(void)
{
	UpdateMap();
	UpdateEnemy();
	UpdateTransparent();
	UpdateSlime();
	UpdateRazer();
	UpdateWarp();
	UpdatePlayer();

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
void DrawGame(void)
{
	DrawMap();
	DrawWarp();
	DrawPlayer();
	DrawEnemy();
	DrawTransparent();
	DrawSlime();
	DrawRazer();
	DrawTimer();
}