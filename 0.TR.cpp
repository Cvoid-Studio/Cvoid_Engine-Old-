#pragma once
#include "CVGraph2D.h"
void SpriteClass2D::Tran_Cart()
{
	T_currentTime ++;
	if (T_currentTime - T_lastTime > 45.f) {
		index++;
		index %= 40;
		DrawTex(index, Trans_Cart, 1880.0f, 50.0f, G_TR.Back_Alpha, true);
		T_lastTime = T_currentTime = 0.f;
	}
	else
		DrawTex(index, Trans_Cart, 1880.0f, 50.0f, G_TR.Back_Alpha, true);
}

//===================TR Stage=====================//
GAME_TR::GAME_TR()
{
	Trans_Back = { 1280.0f, 960.0f } ;
	Back_Alpha = 0.01f;
}

GAME_TR::~GAME_TR()
{

}
//================================================================//
//================================================================//
//================================================================//



bool SpriteClass2D::Load_TR()
{
	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/TR/Trans_Back.jpg"), 0, 0, &m_colorMap[1], 0);



	G_TR.timept_pre = CountTime();
	return true;
}

void SpriteClass2D::Render_TR()
{
	//MS_FileInfo Trans_Back = { "Data/TR/Trans_Back.jpg", 1280.0f, 960.0f };

	if (G_TR.L_here & (!G_TR.L_next))
	{
		setSource(1);
		if (G_TR.L_next == false)
			DrawTex(G_TR.Trans_Back, 0.0f, 0.0f, G_TR.Back_Alpha);
		setSource(0);
		Tran_Cart();
	}
	else
		G_TR.L_here = Load_TR();

	G_TR.timept_now = CountTime();

	if (G_TR.timept_now - G_TR.timept_pre > 4000.f && !G_TR.L_next)
		G_TR.L_next = Load_M();
}

void SpriteClass2D::Update_TR()
{
	if (G_TR.L_next)
	{
		G_TR.L_here = false;
		G_M.L_here = true;
		NowStage = M;
	}

	if (G_TR.Back_Alpha < 1.f)
		G_TR.Back_Alpha += 0.001f;
}

bool SpriteClass2D::Unload_TR()
{
	return true;
}

