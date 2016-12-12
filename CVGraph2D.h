#pragma once
#ifndef _CVGraph2D_H_
#define _CVGraph2D_H_


#include "DXBase.h"
#include "GraphicClass.h"
#include "Stages.h"

//---------------------MACRO------------------------//
#define MS_FONT		"FZZYJW.TTF"

#define TexVertex	6



#define MSSize3x	1280
#define MSSize3y	960
//--------------------------------------------------//

//..................................................//
#define TR_limit	1
#define M_limit		23

//...



#define SourceLimit 30
//..................................................//
#define TR_Head		0
#define TR_Tail		1

#define M_Head		2
#define M_Tail		22
//==================================================//
enum MS_STAGES
{
	TR = 0,
	M = 1,
	S1 = 2,
	S2 = 3,
	//...
};

//--------------------------------------------------//
class SpriteClass2D : public DXBase
{
public:
	SpriteClass2D();
	~SpriteClass2D();

	bool LoadContent();
	void Render();
	void UnloadContent();
	void Update(float);
	
	
private:
	//----------------------------------------------//


	ID3D11VertexShader*	SolidColorVS;
	ID3D11PixelShader*	SolidColorPS;
	ID3D11InputLayout*	InputLayout;
	ID3D11Buffer*		vertexBuffer;

	//Texture//
	ID3D11ShaderResourceView*	m_colorMap[SourceLimit];//
	ID3D11SamplerState*			m_colorMapSampler;

	//ALPHA//

	ID3D11BlendState*	alphaBlendState;
	ID3D11Buffer*	mvpCB;
	XMMATRIX		vpMatrix;

private:
	MS_Generate	M_Gen;
	MS_STAGES	NowStage;

	//AlphaBet MS_AlphaBet = Space;

	float	blendFactor[4] = { 1.f,1.f,1.f,1.f };

	//=============SOUND VOLUME===============//
	LONG	Music_Volume = 2000;
	LONG	Sound_Volume = 2000;

public:
	//==================================API Functions===========================================//
	bool DrawTex(int index, TexContext texInfo, float X, float Y, float Z, bool rotate);
	bool DrawTex(int index, TexContext texInfo, float X, float Y, float Z, float streX, float streY, float RotAng, bool rotate);
	bool DrawTex(MS_FileInfo fileInfo, float X, float Y, float Z);
	bool DrawTex(MS_FileInfo fileInfo, float X, float Y, float Z, float streX, float streY, float RotAng);
	bool DrawTexC(int index, TexContext texInfo, float X, float Y, float Z, bool rotate);
	bool DrawTexC(int index, TexContext texInfo, float X, float Y, float Z, float streX, float streY, float RotAng, bool rotate);
	bool DrawTex(int indexX, int indexY, TexContext texInfo, float X, float Y, float Z);

	bool DrawString(char* message, float startX, float startY, float weight, float Gap, int state);


	void PaintText(int posX, int posY, int height, int width, int weight, char* text, int R, int G, int B);

	void setSource(int index) {
		m_context_ptr->PSSetShaderResources(0, 1, &m_colorMap[index]);
		m_context_ptr->PSSetSamplers(0, 1, &m_colorMapSampler);
		};

	float CountTime() { return static_cast<float>(GetTickCount()); }

	//-----------------SOUND-------------------//
	void CV_PlaySound(char* FilePath);
	void CV_PlayMusic(char* FilePath, bool isLoop);

//==============================================//
private:
	float	T_currentTime;
	float	T_lastTime;
	float	Time_test=0.f;
	float	Time_Test2=0.f;

	float	FPS_TimeNow;
	float	FPS_TimePrev;
	float	fps = 0.0f;
	float	framecount = 0.f;
	char	T_fpsStr[8] = { 0 };
	char	T_PosStr[16] = { 0 };
	char	T_Shot[8] = { 0 };
	int		temp_sum = 0;

	bool	isPlaying = true;
	LPVOID	buf = NULL;
	DWORD	buf_len = 0;
	DWORD	res = WAIT_OBJECT_0;
	DWORD	offset = BUFFERNOTIFYSIZE;

	//--------------------------GLOBAL-------------------------------------//
	const TexContext Trans_Cart = { 320.0f,5120.0f,0.0f,320.0f,0.0f,128.0f };
	inline void Tran_Cart();		inline void GetFPS();
	int index = 0;
	//-----------------------------TR---------------------------------------//
	GAME_TR G_TR;	bool Load_TR();	void Render_TR();	void Update_TR();	bool Unload_TR();
	//------------------------------M---------------------------------------//
	GAME_M	G_M;	bool Load_M();	void Render_M();	void Update_M();	bool Unload_M();
	bool Title_Cart();	void Wind_Cart();

	//-----------------------------BS---------------------------------------//
	GAME_BS G_BS;	bool Load_BS();	void RENDER_BS();	void Update_BS();	bool Unload_BS();
	bool Fog_Cart();	void moveCart();
	void PlayerShoot(TexContext& Bullet1, float shootSPD);
	inline void Player_shoot_Reimu();		void Bomb_Reimu();
	inline void Player_shoot_Marisa();		void Bomb_Marisa();
	inline void Player_shoot_Mima();		void Bomb_Mima();
	inline void Player_shoot_Youka();		void Bomb_Youka();
	//-----------------------------BS---------------------------------------//
	GAME_S1 G_S1;	bool Load_S1();	void RENDER_S1();	void Update_S1();	bool Unload_S1();

};



#endif