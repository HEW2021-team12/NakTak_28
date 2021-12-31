/*==============================================================================

   ゲーム処理 [game.cpp]
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
   定数定義
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   構造体宣言
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
   グローバル変数の定義
------------------------------------------------------------------------------*/
unsigned char g_BGMNo4 = 0;


/*------------------------------------------------------------------------------
   初期化関数
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
   終了処理をする関数
------------------------------------------------------------------------------*/
void UninitGame4()
{
	StopSoundAll();

	//初期化とは逆順に終了処理を行う
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
   更新処理をする関数
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

	//スペースキーが押されていて、フェード処理中ではないとき
	if (Keyboard_IsKeyDown(KK_ENTER) ||
		IsButtonTriggered(0, XINPUT_GAMEPAD_B)
		&& GetFadeState() == FADE_NONE)
	{
		//RESULTへ移行する
		SceneTransition(SCENE_RESULT);
	}
}

/*------------------------------------------------------------------------------
   描画処理をする関数
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