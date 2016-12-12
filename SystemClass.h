#pragma once
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include <Windows.h>
#include <memory>
#include <d3d11.h>
#include <D3DX11.h>
#include "DXBase.h"

#include "CVGraph2D.h"

//=================CONFIG================//
enum C_WndSize
{
	C_FullWnd,
	C_Size1,
	C_Size2,
	C_Size3
};

static int Main_ScreenWidth = 800;
static int Main_ScreenHeight = 600;
//--------------------------------------------------//
#ifndef Abandon
#define Abandon(p) { if (p) { (p)->Dispose();} }
#endif

//=================CLASS=================//

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Sys_Initialize();
	void Run();
	void Sys_ShutDown();
	HINSTANCE	Pre_hinstance;

private:
	WNDCLASSEX	S_Window;
	HINSTANCE	S_hinstance;
	
	HWND		S_hwnd;
	LPCWSTR		Game_Name;
	MSG			msg;

	C_WndSize	S_WndSize;
	int			S_ScreenWidth,
				S_ScreenHeight;
	int			PosX,
				PosY;
};


//==============GLOBAL FUNCTIONS================//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);




//================FPS LIMIT================//
inline bool FPSLimit();

#endif