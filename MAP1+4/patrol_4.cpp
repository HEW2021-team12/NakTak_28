/*==============================================================================

   ���񃋁[�g�Ǘ� [patrol.cpp]
	Author :	�Îs
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "patrol.h"

// ���[�g���W�l1
const char g_Root_1[2][ROOT_MAX4_1] =
{
	// �オX ����Y
//	  1  2  3  4  5  6  7  8  9 10 
	{17,13,14,15,20,22,25,25,21,21 },
	{24,24,14,11,10,11,11,21,23,24 }
};

// ���[�g���W�l2
const char g_Root_2[2][ROOT_MAX4_2] =
{
	// �オX ����Y
//	  1  2  3  4  5  6  7  8  9 10 
	{20,22,25,25,21,21,17,13,14,15 },	
	{10,11,11,21,23,24,24,24,14,11 }
};

// ���[�g���W�l3
const char g_Root_3[2][ROOT_MAX4_3] =
{
	// �オX ����Y
//	  1  2  3  4  5  6  7  8 
	{10, 8, 6, 6,10,10,13,15 },
	{11,13,15,18,18,16,16,11 }
};

// ���[�g���W�l4
const char g_Root_4[2][ROOT_MAX4_4] =
{
	// �オX ����Y
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
