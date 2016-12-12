#pragma once
#include "CVGraph2D.h"

//===================SHOOTING MODE===================//
void SpriteClass2D::Player_shoot_Reimu()
{
	for (int i = 0; i < LIMITShot1; ++i)
	{
		if (G_BS.GC_Player.Bullet1LBuffer[i].Y > 2000.f)
			G_BS.GC_Player.Bullet1LBuffer[i].exist = false;

		if (G_BS.GC_Player.Bullet1LBuffer[i].exist)
		{
			DrawTexC(0, G_BS.Reimu_Amulet1, G_BS.GC_Player.Bullet1RBuffer[i].X + 25.f, G_BS.GC_Player.Bullet1LBuffer[i].Y, 1.f, false);
			DrawTexC(0, G_BS.Reimu_Amulet1, G_BS.GC_Player.Bullet1LBuffer[i].X - 25.f, G_BS.GC_Player.Bullet1LBuffer[i].Y, 1.f, false);
			G_BS.GC_Player.Bullet1RBuffer[i].Y += 8.f;
			G_BS.GC_Player.Bullet1LBuffer[i].Y += 8.f;
		}
		else
		{
			G_BS.GC_Player.Bullet1LBuffer[i].X = G_BS.GC_Player.posX;
			G_BS.GC_Player.Bullet1LBuffer[i].Y = G_BS.GC_Player.posY;
			G_BS.GC_Player.Bullet1RBuffer[i].X = G_BS.GC_Player.posX;
			G_BS.GC_Player.Bullet1RBuffer[i].Y = G_BS.GC_Player.posY;
		}
	}
}

void SpriteClass2D::Player_shoot_Marisa()
{

}

void SpriteClass2D::Player_shoot_Mima()
{

}

void SpriteClass2D::Player_shoot_Youka()
{

}

//===================Bomb MODE=======================//
void SpriteClass2D::Bomb_Reimu()
{

}

void SpriteClass2D::Bomb_Marisa()
{

}

void SpriteClass2D::Bomb_Mima()
{

}

void SpriteClass2D::Bomb_Youka()
{

}

//===================Player MODE=====================//
void SpriteClass2D::moveCart()
{
	setSource(9);

	switch (G_M.M_CHAR)
	{
	case Reimu:
		if (G_BS.GC_Player.Shooting)
		{
			G_BS.GC_Player.Energy++;
			PlayerShoot(G_BS.Reimu_Amulet1, Reimu_ShootSPD);
		}
		Player_shoot_Reimu();
		break;
	case Marisa:
		if (G_BS.GC_Player.Shooting)
		{
			G_BS.GC_Player.Energy++;
			PlayerShoot(G_BS.Marisa_Star1, Marisa_ShootSPD);
		}
		Player_shoot_Marisa();
		break;
	case Mima:
		if (G_BS.GC_Player.Shooting)
		{
			G_BS.GC_Player.Energy++;
			PlayerShoot(G_BS.Mima_Energy1, Marisa_ShootSPD);
		}
		Player_shoot_Mima();
		break;
	case Youka:
		if (G_BS.GC_Player.Shooting)
		{
			G_BS.GC_Player.Energy++;
			PlayerShoot(G_BS.Youka_Flower1, Marisa_ShootSPD);
		}
		Player_shoot_Youka();
		break;
	}
	
	if(G_BS.GC_Player.movStatus == 3)
		DrawTexC(3, G_BS.Player_VX, G_BS.GC_Player.posX, G_BS.GC_Player.posY, 1.f, true);
	else if (G_BS.GC_Player.movStatus == 4)
		DrawTexC(4, G_BS.Player_VX, G_BS.GC_Player.posX, G_BS.GC_Player.posY, 1.f, true);
	else
	{
		Time_test++;
		if (Time_test - Time_Test2 > 75.0f) {
			G_BS.GC_Player.movStatus++;
			G_BS.GC_Player.movStatus %= 3;
			DrawTexC(G_BS.GC_Player.movStatus, G_BS.Player_VX, G_BS.GC_Player.posX, G_BS.GC_Player.posY, 1.f, true);
			Time_Test2 = Time_test = 0.f;
		}
		else
			DrawTexC(G_BS.GC_Player.movStatus, G_BS.Player_VX, G_BS.GC_Player.posX, G_BS.GC_Player.posY, 1.f, true);
	}

	if (G_BS.GC_Player.LowSpd)
	{
		G_BS.CritPtang += 0.0008f;
		if (G_BS.CritPtang >= 10.f)
			G_BS.CritPtang = 0.f;
		DrawTexC(0, G_BS.Reimu_CriPT, G_BS.GC_Player.posX, G_BS.GC_Player.posY, 1.f, 1.f, 1.f, G_BS.CritPtang, false);
		DrawTexC(0, G_BS.Reimu_CriPT, G_BS.GC_Player.posX, G_BS.GC_Player.posY, 1.f, 1.f, 1.f, -G_BS.CritPtang, false);
	}

	
}

bool SpriteClass2D::Fog_Cart()
{
	setSource(28);
	T_currentTime = CountTime();
	if (G_BS.Trans_Index < 5)
	{
		if (T_currentTime - T_lastTime > 80.0f) {
			G_BS.Trans_Index++;
			DrawTex(G_BS.Trans_Index, G_BS.Fog_cart, 0.f, 0.f, 1.f, true);
			T_lastTime = CountTime();
		}
		else
			DrawTex(G_BS.Trans_Index, G_BS.Fog_cart, 0.f, 0.f, 1.f, true);
		G_BS.timept_pre = CountTime();
		return true;
	}
	else
	{
		Tran_Cart();
		G_BS.timept_now = CountTime();
		return true;
	}

	if (G_BS.timept_now - G_BS.timept_pre > 600.f)
		return false;
		
}
//=====================STAGE DESIGN======================//

GAME_BS::GAME_BS()
{
	Fog_cart = { 1280.f,4800.f,0.f,1280.f,0.f,960.f };

	Character = { 2560.f,960.f,0.f,512.f,0.f,960.f };

	Game_Frame = { 1280.f,960.f };

	Pause_Title		= { 640.f,384.f,0.f,320.f,0.f,64.f };
	Pause_Return	= { 640.f,384.f,0.f,320.f,64.f,128.f };
	Pause_Quit		= { 640.f,384.f,0.f,320.f,128.f,192.f };
	Pause_SandQ		= { 640.f,384.f,0.f,320.f,192.f,256.f };
	Pause_Restart	= { 640.f,384.f,0.f,320.f,256.f,320.f };
	Pause_Sure		= { 640.f,384.f,0.f,320.f,320.f,384.f };
	Pause_YN		= { 640.f,384.f,320.f,400.f,320.f,384.f };

	Item_B_P_Pow	= { 96.f,96.f,0.f,32.f,0.f,32.f };
	Item_D_F_UP		= { 96.f,96.f,0.f,32.f,32.f,64.f };
	Item_Add		= { 72.f,72.f,0.f,12.f,48.f,60.f };
	Item_Lf_Bomb	= { 96.f,96.f,32.f,64.f,64.f,96.f };


	Bullet_Rice		= { 36.f,36.f,6.f,12.f,24.f,30.f };

	Player_VX		= { 96.f,576.f,0.f,64.f,0.f,96.f };

	Reimu_Amulet1	= { 60.f,360.f,40.f,60.f,0.f,60.f };	Reimu_Hit1 = { 60.f,360.f,40.f,60.f,60.f,120.f };
	Reimu_Amulet2	= { 60.f,360.f,40.f,60.f,120.f,160.f };	Reimu_Hit2 = { 60.f,360.f,40.f,60.f,160.f,180.f };
	Reimu_CriPT		= { 96.f,576.f,0.f,96.f,480.f,576.f };

	Marisa_Star1	= { 48.f,288.f,32.f,48.f,0.f,32.f };
	Marisa_Star2	= { 48.f,288.f,32.f,48.f,32.f,64.f };
	Marisa_CriPT	= { 96.f,576.f,0.f,96.f,480.f,576.f };

	Mima_Energy1	= { 48.f,288.f,32.f,48.f,0.f,32.f };
	Mima_Energy2	= { 48.f,288.f,32.f,48.f,32.f,64.f };
	Mima_CriPT		= { 96.f,576.f,0.f,96.f,480.f,576.f };

	Youka_Flower1	= { 48.f,288.f,32.f,48.f,0.f,32.f };
	Youka_Flower2	= { 48.f,288.f,32.f,48.f,32.f,64.f };
	Youka_CriPT		= { 96.f,576.f,0.f,96.f,480.f,576.f };

	MS_life = 2;
	MS_bomb = 2;
	MS_Power = 0;
	MS_Dream = 0;
	MS_Point = 0;
	MS_MaxGain = 5000.f;

	G_Status = G_Running;
}

GAME_BS::~GAME_BS()
{

}

bool SpriteClass2D::Load_BS()
{
	switch (G_M.M_CHAR)
	{
	case Reimu:
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/L_Reimu.png"), 0, 0, &m_colorMap[8], 0);
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/G_Reimu.png"), 0, 0, &m_colorMap[9], 0);
		break;
	case Marisa:
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/L_Marisa.png"), 0, 0, &m_colorMap[8], 0);
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/G_Marisa.png"), 0, 0, &m_colorMap[9], 0);
		break;
	case Mima:
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/L_Mima.png"), 0, 0, &m_colorMap[8], 0);
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/G_Reimu.png"), 0, 0, &m_colorMap[9], 0);
		break;
	case Youka:
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/L_Youka.png"), 0, 0, &m_colorMap[8], 0);
		D3DX11CreateShaderResourceViewFromFile
			(m_device_ptr, _T("Data/Game/G_Reimu.png"), 0, 0, &m_colorMap[9], 0);
		break;
	}

	

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Game/Frame_game.png"), 0, 0, &m_colorMap[3], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Game/ATTACT.png"), 0, 0, &m_colorMap[4], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Game/Items.png"), 0, 0, &m_colorMap[5], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Game/Dialog.png"), 0, 0, &m_colorMap[6], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Game/Pause.png"), 0, 0, &m_colorMap[7], 0);

	return true;
}

void SpriteClass2D::RENDER_BS()
{
	if (G_BS.gate && !G_BS.L_here)
	{
		G_BS.gate = Fog_Cart();
		G_BS.L_here = Load_BS();
	}
	else
	{
		float temp_stride = 0.f;
		//=============================PLAYER STATUS================================//
		moveCart();


		setSource(3);
		DrawTex(G_BS.Game_Frame, 0.f, 0.f, 1.f);
		setSource(5);
		for (int life = 0; life < G_BS.MS_life; ++life)
		{
			DrawTex(1, G_BS.Item_Lf_Bomb, 1955.f + life*72.f, 1460.f, 1.f, false);
		}
		for (int Bomb = 0; Bomb < G_BS.MS_bomb; ++Bomb)
		{
			DrawTex(0, G_BS.Item_Lf_Bomb, 1955.f + Bomb*72.f, 1300.f, 1.f, false);
		}

		switch (G_BS.G_Status)
		{
		case G_Pause:
		{
			

		}
			break;
		case G_Sure:
			break;
		case G_Sum:
			break;
		case G_Save:
			break;
		default:
			break;
		}

	}
}

void SpriteClass2D::Update_BS()
{
	G_BS.GC_Player.InitChar();
	if (G_BS.GC_Player.movStatus > 2)
		G_BS.GC_Player.movStatus = 0;

	if (KeyOn(DIK_Z))		//=================== Shoot & Ensure
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			break;
		case G_Sure:
			break;
		case G_Running:
			G_BS.GC_Player.Shooting = true;
			break;
		case G_Sum:
			break;
		case G_Save:
			break;
		}		
	}
	else if (KeyTap(DIK_Z))
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			switch (G_BS.Index_Pause)
			{
			case 0:
				G_BS.G_Status = G_Running;
				break;
			case 1:
				G_BS.G_Status = G_Sure;
				break;
			case 2:
				G_BS.G_Status = G_Sure;
				break;
			case 3:
				G_BS.G_Status = G_Sure;
				break;
			}
			break;
		case G_Sure:
			break;
		case G_Save:
			break;
		}
	}

	if (KeyOn(DIK_X))		//=================== Bomb & Cancel
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			break;
		case G_Sure:
			break;
		case G_Running:
			break;
		case G_Sum:
			break;
		case G_Save:
			break;
		}
	}
	else if (KeyTap(DIK_X))
	{
		switch (G_BS.G_Status)
		{
		case G_Sure:
			G_BS.G_Status = G_Pause;
			break;
		case G_Running:
			break;
		case G_Save:
			G_BS.G_Status = G_Pause;
			break;
		}
	}

	//=======================UDLR Control========================//
	if (KeyOn(DIK_UP))
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			break;
		case G_Running:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.Weight[0] = 1;
			break;
		case G_Sum:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.Weight[0] = 1;
			break;
		case G_Save:
			break;
		}
	}
	else if (KeyTap(DIK_UP))
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			G_BS.Index_Pause -= 1;
			G_BS.Index_Pause %= 4;
			if (G_BS.Index_Pause < 0)
				G_BS.Index_Pause = 3;
			break;
		case G_Save:
			break;
		}
	}

	if (KeyOn(DIK_DOWN))
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			break;
		case G_Running:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.Weight[2] = 1;
			break;
		case G_Sum:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.Weight[2] = 1;
			break;
		case G_Save:
			break;
		}
	}
	else if (KeyTap(DIK_DOWN))
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			G_BS.Index_Pause += 1;
			G_BS.Index_Pause %= 4;
			if (G_BS.Index_Pause < 0)
				G_BS.Index_Pause = 3;
			break;
		case G_Save:
			break;
		}
	}

	if (KeyOn(DIK_LEFT))
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			break;
		case G_Running:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.movStatus = 3;
			G_BS.GC_Player.Weight[3] = 1;
			break;
		case G_Sum:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.movStatus = 3;
			G_BS.GC_Player.Weight[3] = 1;
			break;
		case G_Save:
			break;
		}
	}
	else if (KeyTap(DIK_LEFT))
	{
		switch (G_BS.G_Status)
		{
		case G_Sure:
			G_BS.Index_Sure != G_BS.Index_Sure;
			break;
		case G_Save:
			break;
		}
	}

	if (KeyOn(DIK_RIGHT))
	{
		switch (G_BS.G_Status)
		{
		case G_Sure:
			break;
		case G_Running:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.movStatus = 4;
			G_BS.GC_Player.Weight[1] = 1;			
			break;
		case G_Sum:
			G_BS.GC_Player.Moving = true;
			G_BS.GC_Player.movStatus = 4;
			G_BS.GC_Player.Weight[1] = 1;
			break;
		case G_Save:
			break;
		}
	}
	else if (KeyTap(DIK_RIGHT))
	{
		switch (G_BS.G_Status)
		{
		case G_Sure:
			G_BS.Index_Sure != G_BS.Index_Sure;
			break;
		case G_Save:
			break;
		}
	}

	if (KeyTap(DIK_ESCAPE))
	{
		switch (G_BS.G_Status)
		{
		case G_Pause:
			G_BS.G_Status = G_Running;
			break;
		case G_Save:
			G_BS.G_Status = G_Sure;
			break;
		default:
			G_BS.G_Status = G_Pause;
			break;
		}
	}


	if ((KeyOn(DIK_LSHIFT) || KeyOn(DIK_RSHIFT)) && (G_BS.G_Status == G_Running))
		G_BS.GC_Player.LowSpd = true;

	if (G_BS.GC_Player.Moving)
	{
		G_BS.GC_Player.rotateX =
			(G_BS.GC_Player.Weight[1] - G_BS.GC_Player.Weight[3]);
		G_BS.GC_Player.rotateY =
			(G_BS.GC_Player.Weight[0] - G_BS.GC_Player.Weight[2]);
		float temp_param = 1.f;
		if ((G_BS.GC_Player.rotateX*G_BS.GC_Player.rotateY) != 0)
			temp_param = 0.707f;
		else if ((abs(G_BS.GC_Player.rotateX) + abs(G_BS.GC_Player.rotateY))== 0)
			temp_param = 0.f;
		float temp_C = static_cast<float>(G_BS.GC_Player.rotateX) * temp_param;
		float temp_S = static_cast<float>(G_BS.GC_Player.rotateY) * temp_param;
		
		switch (G_M.M_CHAR)
		{
		case Reimu:
			if (G_BS.GC_Player.LowSpd)
			{
				G_BS.GC_Player.posX += (Reimu_LowSPD*temp_C);
				G_BS.GC_Player.posY += (Reimu_LowSPD*temp_S);
			}
			else
			{
				G_BS.GC_Player.posX += (Reimu_SPD*temp_C);
				G_BS.GC_Player.posY += (Reimu_SPD*temp_S);
			}
			break;
		case Marisa:
			if (G_BS.GC_Player.LowSpd)
			{
				G_BS.GC_Player.posX += (Marisa_LowSPD*temp_C);
				G_BS.GC_Player.posY += (Marisa_LowSPD*temp_S);
			}
			else
			{
				G_BS.GC_Player.posX += (Marisa_SPD*temp_C);
				G_BS.GC_Player.posY += (Marisa_SPD*temp_S);
			}
			break;
		case Mima:
			if (G_BS.GC_Player.LowSpd)
			{
				G_BS.GC_Player.posX += (Mima_LowSPD*temp_C);
				G_BS.GC_Player.posY += (Mima_LowSPD*temp_S);
			}
			else
			{
				G_BS.GC_Player.posX += (Mima_SPD*temp_C);
				G_BS.GC_Player.posY += (Mima_SPD*temp_S);
			}
			break;
		case Youka:
			if (G_BS.GC_Player.LowSpd)
			{
				G_BS.GC_Player.posX += (Youka_LowSPD*temp_C);
				G_BS.GC_Player.posY += (Youka_LowSPD*temp_S);
			}
			else
			{
				G_BS.GC_Player.posX += (Youka_SPD*temp_C);
				G_BS.GC_Player.posY += (Youka_SPD*temp_S);
			}
			break;
		}

		if (G_BS.GC_Player.posX < 124.f)
			G_BS.GC_Player.posX = 124.f;

		if (G_BS.GC_Player.posX > 1658.f)
			G_BS.GC_Player.posX = 1658.f;

		if (G_BS.GC_Player.posY < 78.f)
			G_BS.GC_Player.posY = 78.f;

		if (G_BS.GC_Player.posY > 1830.f)
			G_BS.GC_Player.posY = 1830.f;

	}
	

	memcpy(prevKeyboardKeys, keyboardKeys, sizeof(keyboardKeys));
}


void SpriteClass2D::PlayerShoot(TexContext& Bullet1, float shootSPD)
{
	if (G_BS.GC_Player.Energy > shootSPD)
	{
		if (!G_BS.GC_Player.Bullet1LBuffer[G_BS.GC_Player.BulletNum].exist)
			G_BS.GC_Player.Bullet1LBuffer[G_BS.GC_Player.BulletNum].exist = true;

		G_BS.GC_Player.BulletNum++;
		G_BS.GC_Player.BulletNum %= LIMITShot1;
		G_BS.GC_Player.Energy = 0;
	}
	return;
}