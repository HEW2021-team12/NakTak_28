/*==============================================================================

   巡回ルート管理 [patrol.cpp]
	Author :	古市
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "patrol.h"

// ルート座標値1
const char g_Root_1[2][ROOT_MAX4_1] =
{
	// 上がX 下がY
//	  1  2  3  4  5  6  7  8  9 10 
	{17,13,14,15,20,22,25,25,21,21 },
	{24,24,14,11,10,11,11,21,23,24 }
};

// ルート座標値2
const char g_Root_2[2][ROOT_MAX4_2] =
{
	// 上がX 下がY
//	  1  2  3  4  5  6  7  8  9 10 
	{20,22,25,25,21,21,17,13,14,15 },	
	{10,11,11,21,23,24,24,24,14,11 }
};

// ルート座標値3
const char g_Root_3[2][ROOT_MAX4_3] =
{
	// 上がX 下がY
//	  1  2  3  4  5  6  7  8 
	{10, 8, 6, 6,10,10,13,15 },
	{11,13,15,18,18,16,16,11 }
};

// ルート座標値4
const char g_Root_4[2][ROOT_MAX4_4] =
{
	// 上がX 下がY
//	  1  2  3  4  
	{11,13,11, 8 },
	{22,24,27,25 }
};


char SetRoute4X_1(char number)
{
	return g_Root_1[0][number];
}

char SetRoute4Y_1(char number)
{
	return g_Root_1[1][number];

}

char SetRoute4X_2(char number)
{
	return g_Root_2[0][number];
}

char SetRoute4Y_2(char number)
{
	return g_Root_2[1][number];

}

char SetRoute4X_3(char number)
{
	return g_Root_3[0][number];
}

char SetRoute4Y_3(char number)
{
	return g_Root_3[1][number];

}

char SetRoute4X_4(char number)
{
	return g_Root_4[0][number];
}

char SetRoute4Y_4(char number)
{
	return g_Root_4[1][number];

}
