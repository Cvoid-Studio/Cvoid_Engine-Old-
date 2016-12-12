#include "DXBase.h"


DXBase::DXBase()
{
	drivertype = D3D_DRIVER_TYPE_NULL;
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	m_device_ptr			= NULL;
	m_context_ptr			= NULL;
	m_SwapChain_ptr			= NULL;
	m_RenderTargetView_ptr	= NULL;

	MS_SoundDevice	= NULL;
	DSoundBuffer	= NULL;
	M_DSBuffer		= NULL;
	M_DSBuffer8		= NULL;
	M_DSNotify		= NULL;

	memset(M_DSPosNotify, 0, sizeof(DSBPOSITIONNOTIFY)*MAX_AUDIO_BUF);
	memset(M_Event, 0, sizeof(HANDLE)*MAX_AUDIO_BUF);

	S_Hdc = GetDC(m_hwnd);
}

DXBase::~DXBase()
{
	Dispose();
}

bool DXBase::LoadContent()  //Overrided
{
	return true;
}

void DXBase::UnloadContent()  //Overrided
{

}

bool DXBase::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	m_hinstance = hinstance;
	m_hwnd = hwnd;

	RECT dimensions;
	GetClientRect(hwnd, &dimensions);

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	D3D_DRIVER_TYPE	driverTypes[]
	{
		D3D_DRIVER_TYPE_HARDWARE,		D3D_DRIVER_TYPE_WARP,

		D3D_DRIVER_TYPE_REFERENCE,		D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL	featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);


	DXGI_SWAP_CHAIN_DESC	swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;			//---------FPS
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	unsigned int creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT	result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
			featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc,
			&m_SwapChain_ptr, &m_device_ptr, &featureLevel, &m_context_ptr);
		if (SUCCEEDED(result))
		{
			drivertype = driverTypes[driver];
			break;
		}
	}

	if (FAILED(result))
	{
		MessageBox(0, _T("Failed to create the Direct3D device!"), _T("Compile Error"), MB_OK);
		return false;
	}

	ID3D11Texture2D*	m_Tex2DBuffer_ptr;

	result = m_SwapChain_ptr->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_Tex2DBuffer_ptr);
	if (FAILED(result))
	{
		MessageBox(0, _T("Failed to get the swap chain back buffer!"), _T("Compile Error"), MB_OK);
		return false;
	}

	result = m_device_ptr->CreateRenderTargetView(m_Tex2DBuffer_ptr, 0, &m_RenderTargetView_ptr);
	if (m_Tex2DBuffer_ptr)
		m_Tex2DBuffer_ptr->Release();

	if (FAILED(result))
	{
		MessageBox(0, _T("Failed to create the render target view!"), _T("Compile Error"), MB_OK);
		return false;
	}

	m_context_ptr->OMSetRenderTargets(1, &m_RenderTargetView_ptr, 0);



	D3D11_VIEWPORT		viewPort;

	viewPort.Width = static_cast<float>(width);
	viewPort.Height = static_cast<float>(height);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	m_context_ptr->RSSetViewports(1, &viewPort);


	//===================Initialize DInput============================//

	ZeroMemory(keyboardKeys, sizeof(keyboardKeys));
	ZeroMemory(prevKeyboardKeys, sizeof(prevKeyboardKeys));

	result = DirectInput8Create
		(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXIpunt 0x01!"), _T("Error!"), MB_OK);
		return false;
	}

	result = directInput->CreateDevice(GUID_SysKeyboard, &KeyBoardDevice, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXIpunt 0x02!"), _T("Error!"), MB_OK);
		return false;
	}

	result = KeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXIpunt 0x03!"), _T("Error!"), MB_OK);
		return false;
	}

	result = KeyBoardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXIpunt 0x04!"), _T("Error!"), MB_OK);
		return false;
	}

	result = KeyBoardDevice->Acquire();
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXIpunt 0x05!"), _T("Error!"), MB_OK);
		return false;
	}
	//===================================================================//
	result = DirectSoundCreate8(NULL, &MS_SoundDevice, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXSound! Code:0xS00"), _T("Error!"), MB_OK);
		return false;
	}
	result = MS_SoundDevice->SetCooperativeLevel(m_hwnd, DSSCL_NORMAL);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXSound! Code:0xS01"), _T("Error!"), MB_OK);
		return false;
	}

	DSBUFFERDESC SoundDBuferDesc;
	ZeroMemory(&SoundDBuferDesc, sizeof(SoundDBuferDesc));

	SoundDBuferDesc.dwSize = sizeof(SoundDBuferDesc);
	SoundDBuferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;

	SoundDBuferDesc.lpwfxFormat = (WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	SoundDBuferDesc.lpwfxFormat->wFormatTag = WAVE_FORMAT_PCM;
	SoundDBuferDesc.lpwfxFormat->nChannels = channels;
	SoundDBuferDesc.lpwfxFormat->nSamplesPerSec = sample_rate;
	SoundDBuferDesc.lpwfxFormat->nAvgBytesPerSec = sample_rate*(bits_per_sample / 8)*channels;
	SoundDBuferDesc.lpwfxFormat->nBlockAlign = (bits_per_sample / 8)*channels;
	SoundDBuferDesc.lpwfxFormat->wBitsPerSample = bits_per_sample;
	SoundDBuferDesc.lpwfxFormat->cbSize = 0;

	SoundDBuferDesc.dwBufferBytes = MAX_AUDIO_BUF*BUFFERNOTIFYSIZE;

	result = MS_SoundDevice->CreateSoundBuffer(&SoundDBuferDesc, &M_DSBuffer, NULL);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXSound! Code:0xS02"), _T("Error!"), MB_OK);
		return false;
	}

	result = M_DSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&M_DSBuffer8);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXSound! Code:0xS03"), _T("Error!"), MB_OK);
		return false;
	}

	result = M_DSBuffer8->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&M_DSNotify);
	if (FAILED(result))
	{
		MessageBox(NULL, _T("Failed to initialize DXSound! Code:0xS04"), _T("Error!"), MB_OK);
		return false;
	}

	for (int i = 0; i < MAX_AUDIO_BUF; ++i)
	{
		M_DSPosNotify[i].dwOffset = i*BUFFERNOTIFYSIZE;
		M_Event[i] = ::CreateEvent(NULL, false, false, NULL);
		M_DSPosNotify[i].hEventNotify = M_Event[i];
	}
	M_DSNotify->SetNotificationPositions(MAX_AUDIO_BUF, M_DSPosNotify);
	M_DSNotify->Release();
	//===================================================================//
	return  LoadContent();
}

void DXBase::Dispose()
{
	UnloadContent();

	Kill(m_device_ptr);

	Kill(m_context_ptr);

	Kill(m_SwapChain_ptr);

	Kill(m_RenderTargetView_ptr);

	if (KeyBoardDevice)
	{
		KeyBoardDevice->Unacquire();
		KeyBoardDevice->Release();
	}	KeyBoardDevice = NULL;

	Kill(directInput);
}

bool DXBase::CompileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined (DEBUG)|| defined (_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob*	errorBuffer = 0;
	HRESULT		result;
	result = D3DX11CompileFromFile(filePath, 0, 0, entry, shaderModel, shaderFlags, 0, 0, buffer, &errorBuffer, 0);

	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return false;
	}

	if (errorBuffer != 0)
		errorBuffer->Release();

	return true;
}

void DXBase::getWindSize(int X, int Y)
{
	C_WindH = Y;
	C_WindW = X;
}