#pragma once
#ifndef _D3DBASE_H_
#define _D3DBASE_H_


#pragma comment(lib,"x86/dxgi.lib")
#pragma comment(lib,"x86/d3d11.lib")
#pragma comment(lib,"x86/d3dx11.lib")
#pragma comment(lib,"x86/d3dx10.lib")
#pragma comment(lib,"x86/DxErr.lib")
#pragma comment(lib,"x86/XInput.lib")
#pragma comment(lib,"x86/dwrite.lib")
#pragma comment(lib,"x86/dinput8.lib")
#pragma comment(lib,"x86/XInput.lib")
#pragma comment(lib,"x86/dxguid.lib")
#pragma comment(lib,"x86/WinMM.Lib")
#pragma comment(lib,"x86/dsound.lib")

#pragma comment(lib,"x86/d3d9.Lib")
#pragma comment(lib,"x86/d3dx9.Lib")


#include <tchar.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <cstdio>
#include <d3d11.h>
#include <dxgi.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#include <dinput.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dsound.h>

//====================MACROS=========================//
#ifndef Kill
#define Kill(p) { if (p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef KEYDOWN
#define KEYDOWN(name,key)(name[key] & 0x80)
#endif

#ifndef KeyTap
#define KeyTap(K) (KEYDOWN(prevKeyboardKeys, K) && !KEYDOWN(keyboardKeys, K))
#endif

#ifndef KeyOn
#define KeyOn(K) (KEYDOWN(prevKeyboardKeys, K))
#endif

#define MAX_AUDIO_BUF		4
#define BUFFERNOTIFYSIZE	192000

//==========================================//
//=================CLASS====================//
//==========================================//
class DXBase
{
public:
	DXBase();
	virtual ~DXBase();

	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	void Dispose();

	//-----------------------------------------------//
	virtual	bool LoadContent();
	virtual void Render() = 0;
	virtual	void UnloadContent();
	virtual void Update(float) = 0;
	//-----------------------------------------------//
	bool CompileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer);

	void getWindSize(int X, int Y);

	void InputAcquire(HINSTANCE prehins) { if (prehins != m_hinstance) KeyBoardDevice->Acquire(); };



private:
	HINSTANCE	m_hinstance;
	HWND		m_hwnd;

	D3D_DRIVER_TYPE		drivertype;
	D3D_FEATURE_LEVEL	featureLevel;

protected:
	ID3D11Device*			m_device_ptr;
	ID3D11DeviceContext*	m_context_ptr;
	IDXGISwapChain*			m_SwapChain_ptr;
	ID3D11RenderTargetView*	m_RenderTargetView_ptr;

	//---------------Direct Input--------------------//
protected:
	LPDIRECTINPUT8			directInput;
	LPDIRECTINPUTDEVICE8	KeyBoardDevice;

	char	keyboardKeys[256];
	char	prevKeyboardKeys[256];

	//---------------Direct Sound-------------------//
	IDirectSound8*			MS_SoundDevice;
	LPDIRECTSOUNDBUFFER*	DSoundBuffer;
	IDirectSoundBuffer*		M_DSBuffer;
	IDirectSoundBuffer8*	M_DSBuffer8;
	IDirectSoundNotify8*	M_DSNotify;
	DSBPOSITIONNOTIFY		M_DSPosNotify[MAX_AUDIO_BUF];
	HANDLE					M_Event[MAX_AUDIO_BUF];


	int	C_WindW;
	int	C_WindH;

	int sample_rate = 44100;
	int channels = 2;
	int bits_per_sample = 16;

	HDC			S_Hdc;

	
	const float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
};

//====================================================//
//====================================================//
//==================================================//
struct VertexPos
{
	XMFLOAT3	pos;
	XMFLOAT2	tex0;
};


//==================================================//

#endif