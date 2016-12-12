#include "DirectInputClass.h"

KeyBoardInput::KeyBoardInput()
{
	SolidColorVS = NULL;
	SolidColorPS = NULL;
	InputLayout = NULL;
	vertexBuffer = NULL;
	PosCB = NULL;

	m_colorMap = NULL;
	m_colorMapSampler = NULL;

	selectedPosX = 0.0f;
	selectedPosY = 0.0f;
}

KeyBoardInput::~KeyBoardInput()
{
}


bool KeyBoardInput::LoadContent()
{
	//------------------V SHADER-----------------------//
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader("MSShader.fx", "VS_Main", "vs_4_0", &vsBuffer);
	if (compileResult == false)
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return false;
	}


	HRESULT d3dResult;
	d3dResult = m_device_ptr->CreateVertexShader(
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		NULL,
		&SolidColorVS);

	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();
		MessageBox(0, "Error Layout!", "Compile Error", MB_OK);
		return false;
	}

	//-----------------------------SOLID LAYOUT---------------------------------------//
	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = m_device_ptr->CreateInputLayout(
		solidColorLayout,
		totalLayoutElements,
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		&InputLayout);

	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		MessageBox(0, "Error Layout!", "Compile Error", MB_OK);
		return false;
	}

	//------------------P SHADER-----------------------//
	ID3DBlob* psBuffer = 0;
	compileResult = CompileD3DShader("HSLSShader_1_0.fx", "PS_Main", "ps_4_0", &psBuffer);
	if (compileResult == false)
	{
		MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
		return false;
	}

	d3dResult = m_device_ptr->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &SolidColorPS);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		MessageBox(0, "ErrorFlag 0x00000001!", "Compile Error", MB_OK);
		return false;
	}


	//=======================================================================//
	//=======================================================================//
	//=======================================================================//

	//------------------------Vertex Desc-----------------------------------//

	VertexPos vertices[] =
	{
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) }
	};

	D3D11_BUFFER_DESC	vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));

	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 6;

	//----------------------------SubResource Data--------------------------//
	//----------------------------Vertex Buffer-----------------------------//
	D3D11_SUBRESOURCE_DATA	resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));

	resourceData.pSysMem = vertices;

	d3dResult = m_device_ptr->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, "Failed to create vertex buffer!", "Error!", MB_OK);
		return false;
	}

	//--------------------------Texture resource-----------------------------//
	d3dResult = D3DX11CreateShaderResourceViewFromFile
		//(m_device_ptr, "GamePic/Player/Reimu-G0-1.png", 0, 0, &m_colorMap, 0);
		(m_device_ptr, "1.jpg", 0, 0, &m_colorMap, 0);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, "Failed to load the texture Image!", "Error!", MB_OK);
		return false;
	}

	//-----------------------Texture Desc----------------------------//
	D3D11_SAMPLER_DESC	colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));

	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dResult = m_device_ptr->CreateSamplerState(&colorMapDesc, &m_colorMapSampler);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, "Failed to create color map sampler state!", "Error!", MB_OK);
		return false;
	}

	return true;
}

void KeyBoardInput::Render()
{
	if (m_context_ptr == NULL)
		return;

	
	float clearColor[4] = { 0.3f, 0.0f, 0.25f, 1.0f };
	m_context_ptr->ClearRenderTargetView(m_RenderTargetView_ptr, clearColor);


	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;


	//==========================================================//
	VertexPos vertices[] =
	{
		{ XMFLOAT3(1.0f + selectedPosX, 1.0f + selectedPosY, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f + selectedPosX, -1.0f + selectedPosY, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f + selectedPosX, -1.0f + selectedPosY, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f + selectedPosX, -1.0f + selectedPosY, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f + selectedPosX, 1.0f + selectedPosY, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f + selectedPosX, 1.0f + selectedPosY, 1.0f), XMFLOAT2(1.0f, 0.0f) }
	};

	m_context_ptr->UpdateSubresource(vertexBuffer,0,0,&vertices,0,0);
	///////////////////////////////////////////////////////////////


	m_context_ptr->IASetInputLayout(InputLayout);
	m_context_ptr->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	m_context_ptr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_context_ptr->VSSetShader(SolidColorVS, 0, 0);
	m_context_ptr->PSSetShader(SolidColorPS, 0, 0);

	m_context_ptr->PSSetShaderResources(0, 1, &m_colorMap);
	m_context_ptr->PSSetSamplers(0, 1, &m_colorMapSampler);

	m_context_ptr->Draw(6, 0);

	m_SwapChain_ptr->Present(0, 0);

	return;
}

void KeyBoardInput::UnloadContent()
{
	Kill(SolidColorVS);
	Kill(SolidColorPS);
	Kill(InputLayout);
	Kill(vertexBuffer);
	Kill(PosCB);

	Kill(m_colorMap);
	Kill(m_colorMapSampler);
}

void KeyBoardInput::Update(float)
{
	

	if (GetAsyncKeyState(VK_ESCAPE))							//----------- 'ESC'
		PostQuitMessage(0);

	if (KEYDOWN(prevKeyboardKeys, DIK_DOWN))					//----------- 'DOWN'
	{
		selectedPosY -= 0.02f;
	}
	if (KEYDOWN(prevKeyboardKeys, DIK_UP))						//----------- 'UP'
	{
		selectedPosY += 0.02f;
	}
	if (KEYDOWN(prevKeyboardKeys, DIK_LEFT))					//----------- 'LEFT'
	{
		selectedPosX -= 0.02f;
	}
	if (KEYDOWN(prevKeyboardKeys, DIK_RIGHT))					//----------- 'RIGHT'
	{
		selectedPosX += 0.02f;
	}

	if (KEYDOWN(prevKeyboardKeys, DIK_LSHIFT || DIK_RSHIFT))	//----------- 'SHIFT'
	{

	}

	if (KEYDOWN(prevKeyboardKeys, DIK_RETURN))					//----------- 'ENTER'
	{

	}

	if (KEYDOWN(prevKeyboardKeys, DIK_Z))						//----------- 'Z'
	{

	}

	if (KEYDOWN(prevKeyboardKeys, DIK_X))						//----------- 'X'
	{

	}

	if (KEYDOWN(prevKeyboardKeys, DIK_LALT || DIK_RALT))		//----------- 'ALT'
	{

	}

	memcpy(prevKeyboardKeys, keyboardKeys, sizeof(keyboardKeys));
}