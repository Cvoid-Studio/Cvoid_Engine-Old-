#pragma once
#include "CVGraph2D.h"

bool SpriteClass2D::Title_Cart()
{
	G_M.timept_now = CountTime();
	if (G_M.cart_index < 5)
	{
		//============================================================================//
		setSource(6);

		if (G_M.timept_now - G_M.timept_pre > 80.0f)
		{
			G_M.cart_index++;
			DrawTex(0, G_M.cart_index, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(0, G_M.cart_index, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
	}
	else if (G_M.cart_index < 10 && G_M.cart_index > 4)
	{
		setSource(6);

		if (G_M.timept_now - G_M.timept_pre > 80.0f)
		{
			G_M.cart_index++;
			DrawTex(1, G_M.cart_index - 5, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(1, G_M.cart_index - 5, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
	}
	else if (G_M.cart_index < 15 && G_M.cart_index > 9)
	{
		setSource(6);

		if (G_M.timept_now - G_M.timept_pre > 80.0f)
		{
			G_M.cart_index++;
			DrawTex(2, G_M.cart_index - 10, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(2, G_M.cart_index - 10, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
	}
	else if (G_M.cart_index < 20 && G_M.cart_index > 14)
	{
		setSource(6);

		if (G_M.timept_now - G_M.timept_pre > 80.0f)
		{
			G_M.cart_index++;
			DrawTex(3, G_M.cart_index - 15, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(3, G_M.cart_index - 15, G_M.Main_Back_cart, 0.f, 0.f, 1.f);
	}
	else if (G_M.cart_index < 30 && G_M.cart_index > 19)
	{
		//============================================================================//
		setSource(2);

		if (G_M.timept_now - G_M.timept_pre > 90.0f)
		{
			G_M.cart_index++;
			DrawTex(G_M.cart_index - 20, G_M.Main_Title_cart, 420.0f, 840.0f, 1.0f, true);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(G_M.cart_index - 20, G_M.Main_Title_cart, 420.0f, 840.0f, 1.0f, true);
	}
	else
	{
		setSource(4);
		DrawTex(G_M.Main_Back1, 0.f, 0.f, 1.f);

		setSource(6);
		DrawTex(3, 4, G_M.Main_Back_cart, 0.f, 0.f, G_M.Back_Alpha);
		G_M.Back_Alpha -= 0.01f;

		setSource(2);
		DrawTex(10, G_M.Main_Title_cart, 420.0f, 840.0f, 1.0f, true);

	}
	
	if (G_M.Back_Alpha < 0.01f)
	{
		G_M.cart_index = 0;
		G_M.Back_Alpha = 0.015f;
		return true;
	}	
	//============================================================================//
	return false;
}

void SpriteClass2D::Wind_Cart()
{
	G_M.timept_now = CountTime();
	setSource(7);

	if (G_M.cart_index < 5)
	{
		//============================================================================//
		

		if (G_M.timept_now - G_M.timept_pre > 100.0f)
		{
			G_M.cart_index++;
			DrawTex(0, G_M.cart_index, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(0, G_M.cart_index, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);
	}
	else if (G_M.cart_index < 10 && G_M.cart_index > 4)
	{
		//============================================================================//

		if (G_M.timept_now - G_M.timept_pre > 100.0f)
		{
			G_M.cart_index++;
			DrawTex(1, G_M.cart_index - 5, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(1, G_M.cart_index - 5, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);
	}
	else if (G_M.cart_index < 15 && G_M.cart_index > 9)
	{
		//============================================================================//

		if (G_M.timept_now - G_M.timept_pre > 100.0f)
		{
			G_M.cart_index++;
			DrawTex(2, G_M.cart_index - 10, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(2, G_M.cart_index - 10, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);
	}
	else if (G_M.cart_index > 14)
	{
		//============================================================================//

		if (G_M.timept_now - G_M.timept_pre > 100.0f)
		{
			G_M.cart_index++;
			DrawTex(3, G_M.cart_index - 15, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);
			G_M.timept_pre = CountTime();
		}
		else
			DrawTex(3, G_M.cart_index - 15, G_M.Main_Wind_cart, 0.f, 160.f, 1.f);

		G_M.cart_index %= 20;
	}
}

//===================M Stage=====================//
GAME_M::GAME_M()
{
	Back_Alpha = 1.f;
	temp_stride = 0.f;
	char_stride = 0.f;

	Main_Back1 = { 1280.f,960.f };
	Main_Back2 = { 1280.f,960.f };
	Main_Title = { 960.f,512.f };

	Main_Title_cart = { 960.f,5632.f,0.f,960.f,0.f,512.f };
	Main_Back_cart = { 5120.f,4800.f,0.f,1280.f,0.f,960.f };

	Main_Wind_cart = { 5120.f,4000.f,0.f,1280.f,0.f,800.f };

	Main_buttons_s1 = { 1152.f,432.f,0.f,384.f,0.f,48.f };
	Main_buttons_s2 = { 1152.f,432.f,384.f,768.f,0.f,48.f };
	Main_buttons_s3 = { 1152.f,432.f,768.f,1152.f,0.f,48.f };
	SL_LEVELS = { 800.f,1600.f,0.f,800.f,0.f,320.f };

	ST_LEVEL_T = { 1280.f,2560.f };
	ST_EX = { 1024.f,256.f };

	Char_Select = { 2560.f,1920.f,0.f,640.f,0.f,960.f };
	Char_Select_S = { 2560.f,1920.f,0.f,640.f,960.f,1920.f };
	Char_Desc = { 768.f,2048.f,0.f,768.f,0.f,512.f };
	ST_CHAR_T = { 1280.f,256.f };

}


GAME_M::~GAME_M()
{

}
//================================================================//
//================================================================//
//================================================================//



bool SpriteClass2D::Load_M()
{	
	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/Title_Cart.png"), 0, 0, &m_colorMap[2], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/TITLE_Text.png"), 0, 0, &m_colorMap[3], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/TITLE1.jpg"), 0, 0, &m_colorMap[4], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/Main1.png"), 0, 0, &m_colorMap[5], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/Start.jpg"), 0, 0, &m_colorMap[6], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/Wind_Cart.png"), 0, 0, &m_colorMap[7], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/Char_Select.png"), 0, 0, &m_colorMap[8], 0);

	return true;
}

void SpriteClass2D::Render_M()
{
	if (G_M.L_here)
	{
	if (!G_M.Ready)
	{
		G_M.Ready = Title_Cart();		
	}
	else
	{		
		switch (G_M.M_stg)
		{
		case M_0:
		{
			setSource(4);
			DrawTex(G_M.Main_Back1, 0.f, 0.f, 1.f);
			setSource(3);
			DrawTex(G_M.Main_Title, 420.0f, 840.0f, 1.0f);
			setSource(5);
			for (int i = 0; i < 9; i++)
			{
				if (G_M.m0 == i)
					DrawTex(i, G_M.Main_buttons_s2, 18.f*i, 810.f - 96.f*i, 1.f, true);
				else
					DrawTex(i, G_M.Main_buttons_s1, 10.f*i, 810.f - 96.f*i, 1.f, true);
			}
			if (!G_M.TransLoad) {
				D3DX11CreateShaderResourceViewFromFile
					(m_device_ptr, _T("Data/Menu/TITLE2.jpg"), 0, 0, &m_colorMap[1], 0);
				D3DX11CreateShaderResourceViewFromFile
					(m_device_ptr, _T("Data/Menu/SL_LEVEL.png"), 0, 0, &m_colorMap[2], 0);
				D3DX11CreateShaderResourceViewFromFile
					(m_device_ptr, _T("Data/Menu/SL_CHAR.png"), 0, 0, &m_colorMap[6], 0);
				G_M.TransLoad = true;
			}
		}
			break;
		//========================================================//
		case M_1:
		{
			setSource(1);
			if (G_M.Back_Alpha < 1.f)
				G_M.Back_Alpha += 0.02f;
			else
				G_M.Back_Alpha = 1.f;
			DrawTex(G_M.Main_Back2, 0.f, 0.f, G_M.Back_Alpha);

			Wind_Cart();

			setSource(2);

			DrawTex(0, G_M.SL_LEVELS, 440.f, 1300.f + G_M.temp_stride, 1.f - G_M.m1 * 0.75f, true);
			DrawTex(1, G_M.SL_LEVELS, 440.f, 540.f + G_M.temp_stride, 1.f - fabs(G_M.m1 - 1) * 0.75f, true);
			DrawTex(2, G_M.SL_LEVELS, 440.f, -220.f + G_M.temp_stride, 1.f - fabs(G_M.m1 - 2) * 0.75f, true);
			DrawTex(3, G_M.SL_LEVELS, 440.f, -980.f + G_M.temp_stride, 1.f - (3 - G_M.m1) * 0.75f, true);

			switch (G_M.m1)
			{
			case 0:
				if (G_M.temp_stride > -759.f)
					G_M.temp_stride -= 10.f;
				break;
			case 1:
				if (G_M.temp_stride > 1.f)
					G_M.temp_stride -= 10.f;
				else if (G_M.temp_stride < -1.f)
					G_M.temp_stride += 10.f;
				break;
			case 2:
				if (G_M.temp_stride > 761.f)
					G_M.temp_stride -= 10.f;
				else if (G_M.temp_stride < 759.f)
					G_M.temp_stride += 10.f;
				break;
			case 3:
				if (G_M.temp_stride < 1519.f)
					G_M.temp_stride += 10.f;
				break;
			}

		}			
			break;
		//========================================================//
		case M_2:
		{
			setSource(1);
			if (G_M.Back_Alpha < 1.f)
				G_M.Back_Alpha += 0.02f;
			else
				G_M.Back_Alpha = 1.f;
			DrawTex(G_M.Main_Back2, 0.f, 0.f, G_M.Back_Alpha);

			Wind_Cart();

			
			if (G_M.char_stride > 0.f)
				G_M.char_stride -= 0.02f;
			setSource(8); //Char_select
			DrawTex(G_M.m2, G_M.Char_Select_S, 80.f, -40.f - 80.f*G_M.char_stride, 1.f - G_M.char_stride, false);
			DrawTex(G_M.m2, G_M.Char_Select, 0.f, - 150.f*G_M.char_stride, 1.f - G_M.char_stride, false);
			setSource(6); //Char_Description
			DrawTex(G_M.m2, G_M.Char_Desc, 1080.f, 700.f - 900.f*G_M.char_stride, 1.f - G_M.char_stride, true);

			if (G_BS.gate)
			{
				Fog_Cart();
			}
				
		}
			break;
		//========================================================//
		case M_3:
			setSource(1);
			if (G_M.Back_Alpha < 1.f)
				G_M.Back_Alpha += 0.02f;
			else
				G_M.Back_Alpha = 1.f;
			DrawTex(G_M.Main_Back2, 0.f, 0.f, G_M.Back_Alpha);

			Wind_Cart();

			
			break;
		//========================================================//
		case M_4:
			setSource(1);
			if (G_M.Back_Alpha < 1.f)
				G_M.Back_Alpha += 0.02f;
			else
				G_M.Back_Alpha = 1.f;
			DrawTex(G_M.Main_Back2, 0.f, 0.f, G_M.Back_Alpha);
			break;
		//========================================================//
		case M_5:
			setSource(1);
			if (G_M.Back_Alpha < 1.f)
				G_M.Back_Alpha += 0.02f;
			else
				G_M.Back_Alpha = 1.f;
			DrawTex(G_M.Main_Back2, 0.f, 0.f, G_M.Back_Alpha);
			break;
		//========================================================//
		case M_6:
			setSource(1);
			if (G_M.Back_Alpha < 1.f)
				G_M.Back_Alpha += 0.02f;
			else
				G_M.Back_Alpha = 1.f;
			DrawTex(G_M.Main_Back2, 0.f, 0.f, G_M.Back_Alpha);
			break;
		//========================================================//
		case M_7:
			setSource(1);
			if (G_M.Back_Alpha < 1.f)
				G_M.Back_Alpha += 0.02f;
			else
				G_M.Back_Alpha = 1.f;
			DrawTex(G_M.Main_Back2, 0.f, 0.f, G_M.Back_Alpha);
			break;
		}
	}
	}
	else
		G_M.L_here = Load_M();
}

void SpriteClass2D::Update_M()
{
	switch (G_M.M_stg)
	{
	case M_0:
	{
		if (KeyTap(DIK_UP) || KeyTap(DIK_LEFT))						//----------- 'UP'	
		{
			G_M.m0--;
		}

		if (KeyTap(DIK_DOWN) || KeyTap(DIK_RIGHT))					//----------- 'DOWN'	
		{
			G_M.m0++;
		}

		if (KeyTap(DIK_ESCAPE) || KeyTap(DIK_X))										//----------- 'ESC'	
		{
			G_M.m0 = 8;
		}

		if (KeyTap(DIK_Z))						//----------- 'Z'
		{
			switch (G_M.m0)
			{
			case 0:
				G_M.M_stg = M_1;
				break;
			case 1:
				G_M.M_stg = M_3;
				break;
			case 2:
				G_M.M_stg = M_1;
				break;
			case 3:
				G_M.M_stg = M_4;
				break;
			case 4:
				G_M.M_stg = M_5;
				break;
			case 5:
				G_M.M_stg = M_6;
				break;
			case 6:
				G_M.M_stg = M_7;
				break;
			case 7:
				G_M.M_stg = M_8;
				break;
			case 8:
				PostQuitMessage(0);
				break;
			}
		}

		if (G_M.m0 < 0)
			G_M.m0 = 8;
		G_M.m0 %= 9;
	}
		break;
	//=======================================//
	//=======================================//
	case M_1:
	{
		if (KeyTap(DIK_UP) || KeyTap(DIK_LEFT))						//----------- 'UP'	
		{
			G_M.m1--;
			if (G_M.m1 < 0)
				G_M.m1 = 3;
			G_M.m1 %= 4;
		}

		if (KeyTap(DIK_DOWN) || KeyTap(DIK_RIGHT))					//----------- 'DOWN'	
		{
			G_M.m1++;
			if (G_M.m1 < 0)
				G_M.m1 = 3;
			G_M.m1 %= 4;
		}

		if (KeyTap(DIK_ESCAPE) || KeyTap(DIK_X))					//----------- 'ESC'	
		{
			G_M.M_stg = M_0;
			G_M.Back_Alpha = 0.f;
		}

		if (KeyTap(DIK_Z))						//----------- 'Z'
		{
			G_M.M_stg = M_2;
			G_M.Back_Alpha = 0.f;
			switch (G_M.m1)
			{
			case 0:
				G_M.M_Lev = Easy;
				break;
			case 1:
				G_M.M_Lev = Normal;
				break;
			case 2:
				G_M.M_Lev = Hard;
				break;
			case 3:
				G_M.M_Lev = Lunatic;
				break;
			}
		}	
	}
		break;
	//=======================================//
	//=======================================//
	case M_2:
	{
		if (KeyTap(DIK_UP) || KeyTap(DIK_LEFT))						//----------- 'UP'	
		{
			G_M.m2--;
			if (G_M.m2 < 0)
				G_M.m2 = 3;
			G_M.m2 %= 4;

			G_M.char_stride = 0.9f;
		}

		if (KeyTap(DIK_DOWN) || KeyTap(DIK_RIGHT))					//----------- 'DOWN'	
		{
			G_M.m2++;
			if (G_M.m2 < 0)
				G_M.m2 = 3;
			G_M.m2 %= 4;

			G_M.char_stride = 0.9f;
		}

		if (KeyTap(DIK_ESCAPE) || KeyTap(DIK_X))					//----------- 'ESC'	
		{
			G_M.M_stg = M_1;
			G_M.Back_Alpha = 0.f;
		}

		if (KeyTap(DIK_Z))						//----------- 'Z'
		{
			switch (G_M.m2)
			{
			case 0:
				G_M.M_CHAR = Reimu;
				break;
			case 1:
				G_M.M_CHAR = Marisa;
				break;
			case 2:
				G_M.M_CHAR = Mima;
				break;
			case 3:
				G_M.M_CHAR = Youka;
				break;
			}

			G_BS.gate = true;
			NowStage = S1;
		}
	}
	break;
	//=======================================//
	//=======================================//
	case M_3:
		break;
	//=======================================//
	//=======================================//
	case M_4:
		break;
	//=======================================//
	//=======================================//
	case M_5:
		break;
	//=======================================//
	//=======================================//
	case M_6:
		break;
	//=======================================//
	//=======================================//
	case M_7:
		break;
	}
	

	if (KEYDOWN(prevKeyboardKeys, DIK_UP))						//----------- 'UP'
	{
		//selectedPosY += 0.02f;
	}
	if (KEYDOWN(prevKeyboardKeys, DIK_LEFT))					//----------- 'LEFT'
	{
		//selectedPosX -= 0.02f;
	}
	if (KEYDOWN(prevKeyboardKeys, DIK_RIGHT))					//----------- 'RIGHT'
	{
		//selectedPosX += 0.02f;
	}

	if (KEYDOWN(prevKeyboardKeys, DIK_LSHIFT || DIK_RSHIFT))	//----------- 'SHIFT'
	{

	}

	if (KEYDOWN(prevKeyboardKeys, DIK_RETURN))					//----------- 'ENTER'
	{

	}

	

	if (KEYDOWN(prevKeyboardKeys, DIK_LALT || DIK_RALT))		//----------- 'ALT'
	{

	}

	memcpy(prevKeyboardKeys, keyboardKeys, sizeof(keyboardKeys));

	
}

bool SpriteClass2D::Unload_M()
{
	return true;
}