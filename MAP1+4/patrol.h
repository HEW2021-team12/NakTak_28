/*==============================================================================

   巡回ルート管理 [patrol.h]
	Author :	古市
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once
//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ステージ１
#define ROOT_MAX			(12)

// ステージ２
#define ROOT_MAX2			()

// ステージ３
#define ROOT_MAX3			()

// ステージ４
#define ROOT_MAX4_1			(11)
#define ROOT_MAX4_2			(11)
#define ROOT_MAX4_3			(9)
#define ROOT_MAX4_4			(4)


// ステージ５
#define ROOT_MAX5			(12)


// ステージ１
char SetRouteX(char number);
char SetRouteY(char number);

//// ステージ２
//char SetRoute2X(char number);
//char SetRoute2Y(char number);

//
//// ステージ３
//char SetRoute3X(char number);
//char SetRoute3Y(char number);
//
// ステージ４
char SetRoute4X_1(char number);
char SetRoute4Y_1(char number);

char SetRoute4X_2(char number);
char SetRoute4Y_2(char number);

char SetRoute4X_3(char number);
char SetRoute4Y_3(char number);

char SetRoute4X_4(char number);
char SetRoute4Y_4(char number);

// ステージ５
char SetRoute5X(char number);
char SetRoute5Y(char number);