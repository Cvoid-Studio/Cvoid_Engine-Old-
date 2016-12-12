#include "TextClass.h"
#include "GameSpriteClass.h"

TextClass::TextClass()
{
	SolidColorVS = NULL;
	SolidColorPS = NULL;
	InputLayout = NULL;
	vertexBuffer = NULL;

	m_colorMap = NULL;
	m_colorMapSampler = NULL;

}

TextClass::~TextClass()
{

}

bool TextClass::LoadContent()
{
	//------------------V SHADER-----------------------//
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader("TextShader_1_0.fx", "VS_Main", "vs_4_0", &vsBuffer);
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
	//--------------------------Texture resource-----------------------------//
	d3dResult = D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, "Font/MS_Font.png", 0, 0, &m_colorMap, 0);

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
	//=======================================================================//
	//=======================================================================//
	//------------------------Vertex Desc-----------------------------------//

	D3D11_BUFFER_DESC	vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));

	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	const int sizeOfSprite = sizeof(VertexPos) * 6;
	const int maxLetters = 24;

	vertexDesc.ByteWidth = sizeOfSprite * maxLetters;

	//----------------------------SubResource Data--------------------------//
	//----------------------------Vertex Buffer-----------------------------//

	d3dResult = m_device_ptr->CreateBuffer(&vertexDesc, 0, &vertexBuffer);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, "Failed to create vertex buffer!", "Error!", MB_OK);
		return false;
	}

	return true;
}

void TextClass::Render()
{
	if (m_context_ptr == NULL)
		return;


	float clearColor[4] = { 0.3f, 0.0f, 0.25f, 1.0f };
	m_context_ptr->ClearRenderTargetView(m_RenderTargetView_ptr, clearColor);


	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	m_context_ptr->IASetInputLayout(InputLayout);
	m_context_ptr->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	m_context_ptr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_context_ptr->VSSetShader(SolidColorVS, 0, 0);
	m_context_ptr->PSSetShader(SolidColorPS, 0, 0);

	//Various Textures Sources//
	m_context_ptr->PSSetShaderResources(0, 1, &m_colorMap);
	m_context_ptr->PSSetSamplers(0, 1, &m_colorMapSampler);

	//Set sprites and drawing//
	DrawString("HELLO WORLD!", -0.2f, 0.0f);
	DrawString("hasjdf", -0.2f, 0.4f);

	m_SwapChain_ptr->Present(0, 0);

	return;
}

void TextClass::UnloadContent()
{
	Kill(SolidColorVS);
	Kill(SolidColorPS);
	Kill(InputLayout);
	Kill(vertexBuffer);

	Kill(m_colorMap);
	Kill(m_colorMapSampler);

}

void TextClass::Update(float dt)
{
}


//-------------------------------------------------------------------//



bool TextClass::DrawString(char* message, float startX, float startY)
{
	//------------------------------------------------------------//
	//---------------------String Size----------------------------//
	//------------------------------------------------------------//
	int length = strlen(message);
	const int sizeOfSprite = sizeof(VertexPos) * 6;
	const int maxLetters = 24;

	if (length > maxLetters)
		length = maxLetters;

	float charWidth = 32.0f / 800.0f;
	float charHeight = 32.0f / 640.0f;

	float texelWidth = 32.0f / 864.0f;

	const int verticesPerLetter = 6;

	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->
		Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, "Failed to map resource to text!", "Error!", MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	const int indexA = static_cast<char>('A');
	const int indexZ = static_cast<char>('Z');

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//
	for (int i = 0; i < length; ++i)
	{
		float	thisStartX	= startX + (charWidth*static_cast<float>(i));
		float	thisEndX	= thisStartX + charWidth;
		float	thisEndY	= startY + charHeight;

		spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
		spritePtr[1].pos = XMFLOAT3(thisEndX, startY, 1.0f);
		spritePtr[2].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[3].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
		spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

		int texLookup = 0;
		int letter = static_cast<char>(message[i]);

		if (letter<indexA || letter>indexZ)
		{
			texLookup = (indexZ - indexA) + 1; //Leave Blank
		}
		else
		{
			texLookup = (letter - indexA);
		}

		float	tuStart = 0.0f + (texelWidth*static_cast<float>(texLookup));
		float	tuEnd	= tuStart + texelWidth;

		spritePtr[0].tex0 = XMFLOAT2(tuEnd, 0.0f);
		spritePtr[1].tex0 = XMFLOAT2(tuEnd, 1.0f);
		spritePtr[2].tex0 = XMFLOAT2(tuStart, 1.0f);
		spritePtr[3].tex0 = XMFLOAT2(tuStart, 1.0f);
		spritePtr[4].tex0 = XMFLOAT2(tuStart, 0.0f);
		spritePtr[5].tex0 = XMFLOAT2(tuEnd, 0.0f);

		spritePtr += 6;
	}

	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(6 * length, 0);

	return true;
}
