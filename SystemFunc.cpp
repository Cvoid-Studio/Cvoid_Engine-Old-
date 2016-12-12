#include "SystemClass.h"

#define GAME_TITLE "–|·½¹Ö¾_Õ„ ~ Mystic Square"

//================GLOBALS=======================//
static SystemClass* MS_GAME = NULL;
static float Sys_ThisTime = 0.f;
static float Sys_PrevTime = 0.f;

//================FUNCTIONS=====================//
SystemClass::SystemClass()
{
	S_WndSize = C_FullWnd;
	S_hwnd = NULL;
	//-------------------//
	
}

SystemClass::SystemClass(const SystemClass&)
{

}

SystemClass::~SystemClass()
{
	Sys_ShutDown();
}

bool SystemClass::Sys_Initialize()
{
	WNDCLASSEXW MS_Window;
	Game_Name = (BSTR)GAME_TITLE;
	MS_GAME = this;
	S_hinstance = GetModuleHandleW(NULL);

	//Initialize the Window//
	MS_Window.cbSize		= sizeof(WNDCLASSEX);
	MS_Window.cbClsExtra	= 0;
	MS_Window.cbWndExtra	= 0;
	MS_Window.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	MS_Window.hCursor		= LoadCursor(NULL, IDC_ARROW);
	MS_Window.style			= CS_HREDRAW | CS_VREDRAW;
	MS_Window.lpfnWndProc	= WndProc;
	MS_Window.hInstance		= S_hinstance;
	MS_Window.lpszMenuName	= NULL;
	MS_Window.lpszClassName	= Game_Name;
	MS_Window.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	MS_Window.hIconSm		= MS_Window.hIcon;



	
	//Register the Class//
	if (!RegisterClassExW(&MS_Window))	return false;

	//Window Size//
	switch (S_WndSize)
	{
	case C_FullWnd:
	{
		PosX = 0;
		PosY = 0;
		S_ScreenWidth = GetSystemMetrics(SM_CYSCREEN);
		S_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		S_hwnd = CreateWindowExW(WS_EX_APPWINDOW, Game_Name, Game_Name,
			WS_MAXIMIZE | WS_POPUP | WS_MINIMIZEBOX,
			PosX, PosY,
			S_ScreenWidth, S_ScreenHeight,
			NULL, NULL,
			S_hinstance, NULL);

		break;
	}
		
	case C_Size1:
	{
		S_ScreenWidth = 640;
		S_ScreenHeight = 480;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - S_ScreenWidth) / 3;
		PosY = 30.f;

		S_hwnd = CreateWindowExW(WS_EX_APPWINDOW, Game_Name, Game_Name,
			WS_OVERLAPPEDWINDOW & (~WS_SIZEBOX) & (~WS_MAXIMIZEBOX),
			PosX, PosY,
			S_ScreenWidth, S_ScreenHeight,
			NULL, NULL,
			S_hinstance, NULL);

		break;
	}
		
	case C_Size2:
	{
		S_ScreenWidth = 800;
		S_ScreenHeight = 600;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - S_ScreenWidth) / 3;
		PosY = 30.f;

		S_hwnd = CreateWindowExW(WS_EX_APPWINDOW, Game_Name, Game_Name,
			WS_OVERLAPPEDWINDOW & (~WS_SIZEBOX) & (~WS_MAXIMIZEBOX),
			PosX, PosY,
			S_ScreenWidth, S_ScreenHeight,
			NULL, NULL,
			S_hinstance, NULL);

		break;
	}
		
	case C_Size3:
	{
		S_ScreenWidth = 1280;
		S_ScreenHeight = 960;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - S_ScreenWidth) / 3;
		PosY = 30.f;

		S_hwnd = CreateWindowExW(WS_EX_APPWINDOW, Game_Name, Game_Name,
			WS_OVERLAPPEDWINDOW & (~WS_SIZEBOX) & (~WS_MAXIMIZEBOX),
			PosX, PosY,
			S_ScreenWidth, S_ScreenHeight,
			NULL, NULL,
			S_hinstance, NULL);

		break;
	}
	}


	//Present the Window//
	ShowWindow(S_hwnd, SW_SHOW);

	SetForegroundWindow(S_hwnd);
	SetFocus(S_hwnd);

	ShowCursor(!(S_WndSize == C_FullWnd));

	return true;
}

void SystemClass::Run()
{
	bool C_Quit = false;
	bool C_Result = true;

	//Initialize the objects//
	std::auto_ptr<SpriteClass2D> MS_GAME(new SpriteClass2D());

	//=========================Initializ=================================//
	bool result = MS_GAME->Initialize(S_hinstance, S_hwnd);
	//=========================Get Screen Size===========================//
	MS_GAME->getWindSize(S_ScreenWidth, S_ScreenHeight);
	//===================================================================//

	if (result == false)
	{
		MessageBox(NULL, "Failed initializing D3D!", "Error!", MB_OK);
		return;
	}

	//Initialize the message//
	ZeroMemory(&msg, sizeof(msg));

	//Message Loop//
	while (!C_Quit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			C_Quit = true;
		}
		//----------------Do all the rendering work here-----------------//
		MS_GAME->InputAcquire(Pre_hinstance);
		MS_GAME->Update(0.0f);
		MS_GAME->Render();
	}
	MS_GAME->Dispose();
	return;
}

/*void SystemClass::Run()
{
	HANDLE phWait = CreateWaitableTimer(NULL, FALSE, NULL);

	bool C_Quit = false;
	bool C_Result = true;
	
	//Initialize the objects//
	std::auto_ptr<SpriteClass2D> MS_GAME(new SpriteClass2D());
	
	//=========================Initializ=================================//
	bool result = MS_GAME->Initialize(S_hinstance, S_hwnd);
	//=========================Get Screen Size===========================//
	MS_GAME->getWindSize(S_ScreenWidth, S_ScreenHeight);
	//===================================================================//

	if (result == false)
	{
		MessageBox(NULL, "Failed initializing D3D!", "Error!", MB_OK);
		return;
	}
		

	//Initialize the message//
	ZeroMemory(&msg, sizeof(msg));

	//Message Loop//
	while (!C_Quit)
	{
		switch (MsgWaitForMultipleObjects(1, &phWait, FALSE, INFINITE, QS_ALLINPUT))
		{
		case WAIT_OBJECT_0:
		{
			//----------------Do all the rendering work here-----------------//
			MS_GAME->InputAcquire(Pre_hinstance);
			MS_GAME->Update(0.0f);
			MS_GAME->Render();
		}
		break;

		case WAIT_OBJECT_0 + 1:
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

			if (msg.message == WM_QUIT)
				C_Quit = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		default:
			break;
		}
	}
	MS_GAME->Dispose();
	return;
}*/

void SystemClass::Sys_ShutDown()
{
	ShowCursor(true);

	if (S_WndSize == C_FullWnd)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(S_hwnd);
	S_hwnd = NULL;

	UnregisterClassW(Game_Name, S_hinstance);
	S_hinstance = NULL;
	MS_GAME = NULL;

	return;
}

//----------------WINPROC-----------------------//
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC	hdc;

	switch (message)
	{
	case	WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case	WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case	WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wp, lp);
	}

	return 0;
}



//---------------FPS LIMIT----------------//
bool FPSLimit()
{
	if (Sys_ThisTime - Sys_PrevTime < 1000.f)
	{
		Sys_ThisTime += 10.f;
		return false;
	}
	else
	{
		Sys_PrevTime = Sys_ThisTime;
		return true;
	}
}