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


// ステージ４
char SetRouteX4_1(char number);
char SetRouteY4_1(char number);

char SetRouteX4_2(char number);
char SetRouteY4_2(char number);

char SetRouteX4_3(char number);
char SetRouteY4_3(char number);

char SetRouteX4_4(char number);
char SetRouteY4_4(char number);

