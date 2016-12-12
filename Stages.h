#pragma once
#ifndef _STAGES_H_
#define _STAGES_H_

#include "GraphicClass.h"
#include "Chars.h"
//===================Enumeration====================//
enum main_stg {
	M_0, 		//Main Menu
	M_1,  		//Level Select -- Normal Game
	M_2,  		//Char Select
	M_3,  		//Level Select -- Extra
	M_4,  		//Replay
	M_5,  		//Player Data
	M_6,  		//Music Room
	M_7,  		//Option
	M_8 		//Manual
};

enum MS_LEVELS
{
	Easy, Normal, Hard, Lunatic, Extra
};

enum MS_CHAR
{
	Reimu, Marisa, Mima, Youka
};

struct StageBase
{
	bool L_here = false;
	bool L_next = false;

	float	timept_pre = 0.f;
	float	timept_now = 0.f;
};

//==================================================//
//===========            TR            =============//
//==================================================//

struct GAME_TR : StageBase
{
	//================OBJECTS=================//
	MS_FileInfo Trans_Back;
	TexContext	Trans_Cart;
	//================ C & DC ================//
	GAME_TR();
	~GAME_TR();
	//=================Param==================//
	float Back_Alpha;
};

//==================================================//
//===========             M            =============//
//==================================================//

struct GAME_M : StageBase
{
	//================OBJECTS=================//
	MS_FileInfo	Main_Back1;
	MS_FileInfo	Main_Back2;
	MS_FileInfo	Main_Title;

	TexContext	Main_Title_cart;

	TexContext	Main_Back_cart;

	TexContext	Main_Wind_cart;

	TexContext	Main_buttons_s1;
	TexContext	Main_buttons_s2;
	TexContext	Main_buttons_s3;
	TexContext	SL_LEVELS;

	MS_FileInfo	ST_LEVEL_T;
	MS_FileInfo	ST_EX;

	TexContext	Char_Select;
	TexContext	Char_Select_S;
	MS_FileInfo	ST_CHAR_T;
	TexContext	Char_Desc;

	//================ C & DC ================//
	GAME_M();
	~GAME_M();
	//=================Param==================//
	main_stg M_stg = M_0;
	MS_LEVELS M_Lev = Normal;
	MS_CHAR	M_CHAR = Reimu;

	int cart_index = 0;
	bool Ready = false;
	bool TransLoad = false;

	int m0 = 0;		// MAIN OPTIONS: 0 - 8
	int m1 = 1;		// LEVEL: 0, 1, 2, 3
	int m2 = 0;		// CHARS: 0, 1, 2, 3

	float Back_Alpha;
	float Obj_Alpha = 1.f;
	float temp_stride;
	float char_stride;
};

//==================================================//
//===========             BS           =============//
//==================================================//
enum GameStatus
{
	G_Pause = 0,
	G_Sure = 1,
	G_Running = 2,
	G_Sum = 3,
	G_Save = 4
};


struct GAME_BS : StageBase
{
	//================CHARS===================//
	G_MainChar	GC_Player;



	//================OBJECTS=================//
	TexContext Fog_cart;
	TexContext Character;

	MS_FileInfo Game_Frame;
	
	TexContext Pause_Title;
	TexContext Pause_Return;
	TexContext Pause_Quit;
	TexContext Pause_SandQ;
	TexContext Pause_Restart;
	TexContext Pause_Sure;
	TexContext Pause_YN;

	TexContext Item_B_P_Pow;
	TexContext Item_D_F_UP;
	TexContext Item_Add;
	TexContext Item_Lf_Bomb;

	TexContext Player_VX;

	TexContext Reimu_Amulet1;	TexContext Reimu_Hit1;
	TexContext Reimu_Amulet2;	TexContext Reimu_Hit2;
	TexContext Reimu_CriPT;

	TexContext Marisa_Star1;
	TexContext Marisa_Star2;
	TexContext Marisa_CriPT;

	TexContext Mima_Energy1;
	TexContext Mima_Energy2;
	TexContext Mima_CriPT;

	TexContext Youka_Flower1;
	TexContext Youka_Flower2;
	TexContext Youka_CriPT;

	TexContext Bullet_Rice;

	//================ C & DC ================//
	GAME_BS();
	~GAME_BS();
	//=================Param==================//
	int Trans_Index = 0;
	bool gate = false;

	GameStatus G_Status = G_Running;

	float CritPtang = 0.f;

	int	 Index_Pause = 0;
	bool Index_Sure = false;
	int  Index_Save = 0;

	//================GAME PARAM==============//
	int MS_life;
	int MS_bomb;
	int MS_Power;
	int MS_Dream;
	int MS_Point;
	int MS_MaxGain;
};

//==================================================//
//===========             S1           =============//
//==================================================//

struct GAME_S1 : StageBase
{
	//================OBJECTS=================//
	TexContext SceneBack1;
	TexContext SceneBack2;
	//================ C & DC ================//
	GAME_S1();
	~GAME_S1();
	//=================Param==================//
	

};
#endif













