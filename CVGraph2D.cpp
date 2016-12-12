#include "CVGraph2D.h"
#include "Alpha.h"

//===============================================================//
//===============================================================//
//===============================================================//

SpriteClass2D::SpriteClass2D()
{
	T_currentTime = 0.0f;
	T_lastTime = 0.0f;
	FPS_TimeNow = 0.0f;
	FPS_TimePrev = 0.0f;

	SolidColorVS	= NULL;
	SolidColorPS	= NULL;
	InputLayout		= NULL;
	vertexBuffer	= NULL;

	for (int i = 0; i < SourceLimit; ++i)
	{
		m_colorMap[i] = NULL; 
	}
	
	m_colorMapSampler = NULL;

	alphaBlendState = NULL;
	mvpCB = NULL;

	M_Gen = { 0.0f,0,true };
	NowStage = TR;

	//--------------------------------------//

}

SpriteClass2D::~SpriteClass2D()
{
	
}

bool SpriteClass2D::LoadContent()
{
	//------------------V SHADER-----------------------//
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader("MS_Shader_1_0.fx", "VS_Main", "vs_4_0", &vsBuffer);
	if (compileResult == false)
	{
		MessageBox(0, _T("Error loading vertex shader!"), _T("Compile Error"), MB_OK);
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
		MessageBox(0, _T("Error Layout!"), _T("Compile Error"), MB_OK);
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
		MessageBox(0, _T("Error Layout!"), _T("Compile Error"), MB_OK);
		return false;
	}

	//------------------P SHADER-----------------------//
	ID3DBlob* psBuffer = 0;
	compileResult = CompileD3DShader("MS_Shader_1_0.fx", "PS_Main", "ps_4_0", &psBuffer);
	if (compileResult == false)
	{
		MessageBox(0, _T("Error loading pixel shader!"), _T("Compile Error"), MB_OK);
		return false;
	}

	d3dResult = m_device_ptr->CreatePixelShader(psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(), 0, &SolidColorPS);

	psBuffer->Release();

	if (FAILED(d3dResult))
	{
		MessageBox(0, _T("ErrorFlag 0x00000001!"), _T("Compile Error"), MB_OK);
		return false;
	}


	//=======================================================================//
	//=======================================================================//
	//D3DX11_IMAGE_LOAD_INFO testinfo;
	//testinfo.Depth = 0.1f;

	//===================================================//

	//-----------------load the static textures here----------------------//       =============================>> Level S0
	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/TR/Trans_Cart.png"), 0, 0, &m_colorMap[0], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Font/Alphabet.png"), 0, 0, &m_colorMap[SourceLimit - 1], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Menu/EnterGame_Cart.png"), 0, 0, &m_colorMap[SourceLimit - 2], 0);

	D3DX11CreateShaderResourceViewFromFile
		(m_device_ptr, _T("Data/Game/Mystic Square_Load.png"), 0, 0, &m_colorMap[SourceLimit - 3], 0);
	//-----------------------Texture Desc----------------------------//
	D3D11_SAMPLER_DESC	colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));

	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MinLOD = 0;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;


	d3dResult = m_device_ptr->CreateSamplerState(&colorMapDesc, &m_colorMapSampler);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to create color map sampler state!"), _T("Error!"), MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC	vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));

	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	vertexDesc.ByteWidth = sizeof(VertexPos) * 6 * 30;                  ///////


	d3dResult = m_device_ptr->CreateBuffer(&vertexDesc, 0, &vertexBuffer);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to create vertex buffer!"), _T("Error!"), MB_OK);
		return false;
	}
	

	//------------------------Const Buffers----------------------------------//

	D3D11_BUFFER_DESC	constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));

	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = m_device_ptr->CreateBuffer(&constDesc, 0, &mvpCB);

	if (FAILED(d3dResult))
	{
		MessageBox(0, _T("Failed to create the const Buffer!"), _T("Error!"), MB_OK);
		return false;
	}

	//==============================Sprites==================================//
	//-----------------------Positions------------------------//
	const XMMATRIX view = XMMatrixIdentity();
	const XMMATRIX projection = XMMatrixOrthographicOffCenterLH
		(0.0f, static_cast<float>(C_WindW), 0.0f, static_cast<float>(C_WindH), 0.1f, 100.0f);
	vpMatrix = XMMatrixMultiply(view, projection);

	//--------------------Blend State------------------------//
	D3D11_BLEND_DESC	blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	

	m_device_ptr->CreateBlendState(&blendDesc, &alphaBlendState);

	m_context_ptr->VSSetShader(SolidColorVS, 0, 0);
	m_context_ptr->PSSetShader(SolidColorPS, 0, 0);

	M_DSBuffer8->SetCurrentPosition(0);
	M_DSBuffer8->Play(0, 0, DSBPLAY_LOOPING);

	return true;
}

void SpriteClass2D::Render()
{
	m_context_ptr->OMSetBlendState(alphaBlendState, blendFactor, 0xFFFFFFF);
	if (m_context_ptr == NULL)
			return;

	m_context_ptr->ClearRenderTargetView(m_RenderTargetView_ptr, clearColor);


	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	m_context_ptr->IASetInputLayout(InputLayout);
	m_context_ptr->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	m_context_ptr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = static_cast<int>(stride);
	offset = static_cast<int>(offset);

	//=========================================================//
	//=================    Various   Stages    ================//
	//=========================================================//
	GetFPS();

	switch (NowStage)
	{
	case TR:
		Render_TR();
		break;
	case M:
		CV_PlayMusic("Data/Music/M1.wav", true);
		Render_M();
		break;
	case S1:
		RENDER_BS();
		break;
	}
	
	
	sprintf(T_fpsStr, "%0.1f",fps);
	sprintf(T_PosStr, "%0.1f,%0.1f", G_BS.GC_Player.posX, G_BS.GC_Player.posY);
	for (int i = 0; i < LIMITShot1; ++i)
	{
		temp_sum += static_cast<int>(G_BS.GC_Player.Bullet1LBuffer[i].exist);
	}
	sprintf(T_Shot, "%i", temp_sum);
	DrawString(T_Shot, 10.f, 10.f, 0.8f, 30.f, 1);
	temp_sum = 0;
	DrawString("FPS:", 10.f, 1860.f, 0.8f, 32.f, 1); DrawString(T_fpsStr, 160.f, 1860.f, 0.8f, 32.f, 2);
	DrawString("Pos:", 10.f, 1800.f, 0.8f, 32.f, 1); DrawString(T_PosStr, 160.f, 1800.f, 0.8f, 32.f, 2);

	//=========================================================//
	//=================         Ending         ================//
	//=========================================================//
	m_context_ptr->OMSetBlendState(0, 0, 0xFFFFFFF);

	m_SwapChain_ptr->Present(0, 0);

	return;
}


void SpriteClass2D::UnloadContent()
{
	Kill(SolidColorVS);
	Kill(SolidColorPS);
	Kill(InputLayout);
	Kill(vertexBuffer);

	for (int i = 0; i < SourceLimit; ++i)
	{
		Kill(m_colorMap[i]);
	}
	
	Kill(m_colorMapSampler);

	Kill(alphaBlendState);
	Kill(mvpCB);

}

void SpriteClass2D::Update(float dt)
{
	KeyBoardDevice->GetDeviceState(sizeof(keyboardKeys), (LPVOID)&keyboardKeys);
	switch (NowStage)
	{
	case TR:
		Update_TR();
		break;
	case M:
		Update_M();
		break;
	case S1:
		Update_BS();
		//Update_S1();
		break;
	//...
	}
}



//======================MS_GameSprite==========================//
/*
MS_GameSprite::MS_GameSprite()
{
m_rotation = 0.0f;
m_scale.x = m_scale.y = 1.0f;
}

MS_GameSprite::MS_GameSprite(const MS_GameSprite&)
{

}

MS_GameSprite::~MS_GameSprite()
{
}

XMMATRIX MS_GameSprite::GetWorldMatrix()
{
XMMATRIX	translation = XMMatrixTranslation(m_position.x, m_position.y, 0.0f);
XMMATRIX	rotationZ = XMMatrixRotationZ(m_rotation);
XMMATRIX	scale = XMMatrixScaling(m_scale.x, m_scale.y, 1.0f);

return translation * rotationZ * scale;
}


void MS_GameSprite::SetPosition(XMFLOAT2& position)
{
m_position = position;
}

void MS_GameSprite::SetRotation(float rotation)
{
m_rotation = rotation;
}

void MS_GameSprite::SetScale(XMFLOAT2& scale)
{
m_scale = scale;
}
*/


//=====================================================================================================//
//*****************************************************************************************************//
//=====================================================================================================//

bool SpriteClass2D::DrawTex(int index, TexContext texInfo, float X, float Y, float Z, bool rotate)
{

	X -= C_WindW;
	Y -= C_WindH;

	const float gapX = texInfo.R_map - texInfo.L_map;
	const float gapY = texInfo.D_map - texInfo.U_map;

	const float charWidth = 2 * gapX / C_WindW;
	const float charHeight = 2 * gapY / C_WindH;
	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{

		MessageBox(NULL, _T("Failed to map resource to texture!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//

	float EndX = X / C_WindW + charWidth;
	float EndY = Y / C_WindH + charHeight;
	float LayerZ = Z / 100.0f;

	spritePtr[0].pos = XMFLOAT3(EndX, EndY, LayerZ);
	spritePtr[1].pos = XMFLOAT3(EndX, Y / C_WindH, LayerZ);
	spritePtr[2].pos = XMFLOAT3(X / C_WindW, Y / C_WindH, LayerZ);
	spritePtr[3].pos = XMFLOAT3(X / C_WindW, Y / C_WindH, LayerZ);
	spritePtr[4].pos = XMFLOAT3(X / C_WindW, EndY, LayerZ);
	spritePtr[5].pos = XMFLOAT3(EndX, EndY, LayerZ);

	if (!rotate)
	{
		const float texEdgeL = (texInfo.L_map + gapX * index) / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = texInfo.U_map / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}
	else
	{
		const float texEdgeL = texInfo.L_map / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = (texInfo.U_map + gapY * index) / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}


	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(TexVertex, 0);

	return true;
}

//=====================================================================================================//

bool SpriteClass2D::DrawTex(int index, TexContext texInfo, float X, float Y, float Z, float streX, float streY, float RotAng, bool rotate)
{

	X -= C_WindW;
	Y -= C_WindH;
	X /= C_WindW;
	Y /= C_WindH;
	RotAng *= (6.12831855f);

	float gapX = (texInfo.R_map - texInfo.L_map);
	float gapY = (texInfo.D_map - texInfo.U_map);
	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to map resource to texture!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//
	float EX = gapX * streX;
	float EY = 2 * gapY * streY;
	float NX = -gapX * streX;
	float NY = 0.f;

	float c = cosf(RotAng);
	float s = sinf(RotAng);

	float X1 = EX*c + EY*s;
	float Y1 = EY*c - EX*s;

	float X2 = EX*c;
	float Y2 = -EX*s;

	float X3 = -X1;		float Y3 = -Y1;
	float X4 = -X2;		float Y4 = -Y2;

	X1 /= C_WindW;	X2 /= C_WindW;	X3 /= C_WindW;	X4 /= C_WindW;
	Y1 /= C_WindH;	Y2 /= C_WindH;	Y3 /= C_WindH;	Y4 /= C_WindH;

	X1 += X;	X2 += X;	X3 += X;	X4 += X;
	Y1 += Y;	Y2 += Y;	Y3 += Y;	Y4 += Y;

	float LayerZ = Z / 100.0f;

	spritePtr[0].pos = XMFLOAT3(X1, Y1, LayerZ);
	spritePtr[1].pos = XMFLOAT3(X2, Y2, LayerZ);
	spritePtr[2].pos = XMFLOAT3(X3, Y3, LayerZ);
	spritePtr[3].pos = XMFLOAT3(X3, Y3, LayerZ);
	spritePtr[4].pos = XMFLOAT3(X4, Y4, LayerZ);
	spritePtr[5].pos = XMFLOAT3(X1, Y1, LayerZ);
	//----------------------------------------------------------//
	//----------------------ALPHA BLEND-------------------------//

	//----------------------------------------------------------//
	gapX *= C_WindW;
	gapY *= C_WindH;
	if (!rotate)
	{
		const float texEdgeL = (texInfo.L_map + gapX * index) / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = texInfo.U_map / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}
	else
	{
		const float texEdgeL = texInfo.L_map / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = (texInfo.U_map + gapY * index) / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}
	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(TexVertex, 0);
	return true;
}

//=====================================================================================================//

bool SpriteClass2D::DrawTex(MS_FileInfo fileInfo, float X, float Y, float Z)
{
	X -= C_WindW;
	Y -= C_WindH;

	const float gapX = 2 * fileInfo.Width / C_WindW;
	const float gapY = 2 * fileInfo.Height / C_WindH;
	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to map resource to texture!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//

	float EndX = X / C_WindW + gapX;
	float EndY = Y / C_WindH + gapY;
	float LayerZ = Z / 100.0f;

	spritePtr[0].pos = XMFLOAT3(EndX, EndY, LayerZ);
	spritePtr[1].pos = XMFLOAT3(EndX, Y / C_WindH, LayerZ);
	spritePtr[2].pos = XMFLOAT3(X / C_WindW, Y / C_WindH, LayerZ);
	spritePtr[3].pos = XMFLOAT3(X / C_WindW, Y / C_WindH, LayerZ);
	spritePtr[4].pos = XMFLOAT3(X / C_WindW, EndY, LayerZ);
	spritePtr[5].pos = XMFLOAT3(EndX, EndY, LayerZ);

	spritePtr[0].tex0 = XMFLOAT2(1.0f, 0.0f);
	spritePtr[1].tex0 = XMFLOAT2(1.0f, 1.0f);
	spritePtr[2].tex0 = XMFLOAT2(0.0f, 1.0f);
	spritePtr[3].tex0 = XMFLOAT2(0.0f, 1.0f);
	spritePtr[4].tex0 = XMFLOAT2(0.0f, 0.0f);
	spritePtr[5].tex0 = XMFLOAT2(1.0f, 0.0f);
	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(TexVertex, 0);
	return true;
}

//=====================================================================================================//

bool SpriteClass2D::DrawTex(MS_FileInfo fileInfo, float X, float Y, float Z, float streX, float streY, float RotAng)
{
	X -= C_WindW;
	Y -= C_WindH;

	RotAng *= (2 * 3.14f);

	const float gapX = 2 * streX * fileInfo.Width / C_WindW;
	const float gapY = 2 * streY * fileInfo.Height / C_WindH;
	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to map resource to texture!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//

	float EX = X / C_WindW + gapX;
	float EY = Y / C_WindH + gapY;
	float LayerZ = Z / 100.0f;

	float NX = X / C_WindW;
	float NY = Y / C_WindH;

	spritePtr[0].pos = XMFLOAT3(EX*cosf(RotAng) + EY*sinf(RotAng), EY*cosf(RotAng) - EX*sinf(RotAng), LayerZ);
	spritePtr[1].pos = XMFLOAT3(EX*cosf(RotAng) + NY*sinf(RotAng), NY*cosf(RotAng) - EX*sinf(RotAng), LayerZ);
	spritePtr[2].pos = XMFLOAT3(NX*cosf(RotAng) + NY*sinf(RotAng), NY*cosf(RotAng) - NX*sinf(RotAng), LayerZ);
	spritePtr[3].pos = XMFLOAT3(NX*cosf(RotAng) + NY*sinf(RotAng), NY*cosf(RotAng) - NX*sinf(RotAng), LayerZ);
	spritePtr[4].pos = XMFLOAT3(NX*cosf(RotAng) + EY*sinf(RotAng), EY*cosf(RotAng) - NX*sinf(RotAng), LayerZ);
	spritePtr[5].pos = XMFLOAT3(EX*cosf(RotAng) + EY*sinf(RotAng), EY*cosf(RotAng) - EX*sinf(RotAng), LayerZ);


	spritePtr[0].tex0 = XMFLOAT2(1.0f, 0.0f);
	spritePtr[1].tex0 = XMFLOAT2(1.0f, 1.0f);
	spritePtr[2].tex0 = XMFLOAT2(0.0f, 1.0f);
	spritePtr[3].tex0 = XMFLOAT2(0.0f, 1.0f);
	spritePtr[4].tex0 = XMFLOAT2(0.0f, 0.0f);
	spritePtr[5].tex0 = XMFLOAT2(1.0f, 0.0f);
	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(TexVertex, 0);
	return true;
}

//=====================================================================================================//
//=====================================================================================================//

bool SpriteClass2D::DrawString(char* message, float startX, float startY, float weight, float Gap, int state)
{
	setSource(29);


	startX -= C_WindW;
	startY -= C_WindH;

	//------------------------------------------------------------//
	//---------------------String Size----------------------------//
	//------------------------------------------------------------//
	int length = strlen(message);

	float charWidth = 64.f;
	float charHeight = 64.f;

	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->
		Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to map resource to text!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;
	int row, column = 0;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//
	for (int i = 0; i < length; ++i)
	{
		charWidth = 64.f * weight;
		charHeight = 64.f * weight;
		float	thisStartX = startX / C_WindW + (Gap * static_cast<float>(i)) / C_WindW;
		float	thisEndX = thisStartX + charWidth / C_WindW;
		float	thisStartY = startY / C_WindH;
		float	thisEndY = thisStartY + charHeight / C_WindH;

		spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
		spritePtr[1].pos = XMFLOAT3(thisEndX, thisStartY, 1.0f);
		spritePtr[2].pos = XMFLOAT3(thisStartX, thisStartY, 1.0f);
		spritePtr[3].pos = XMFLOAT3(thisStartX, thisStartY, 1.0f);
		spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
		spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

		charWidth = 32.f;
		charHeight = 32.f;

		row = Alpha::AlphaCheckBoard(message[i]).CheckY;
		column = Alpha::AlphaCheckBoard(message[i]).CheckX;

		float texEdgeL = row * charWidth / 416.f;
		float texEdgeR = texEdgeL + charWidth / 416.f;
		float texEdgeU = column * charHeight / 672.f;
		float texEdgeD = texEdgeU + charHeight / 672.f;
		switch (state)
		{
			case 0:
				texEdgeL += (1 / 3);	texEdgeR += (1 / 3);
				break;
			case 2:
				texEdgeL += (2 / 3);	texEdgeR += (2 / 3);
				break;
			default:break;
		}
		

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);

		spritePtr += 6;
	}

	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(6 * length, 0);

	return true;
}


void SpriteClass2D::PaintText(int posX, int posY, int height, int width, int weight, char* text, int R,int G, int B)
{
	HFONT hFont = CreateFont(height, width, 0, 0, weight, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT(MS_FONT));
	SelectObject(S_Hdc, hFont);

	SetBkMode(S_Hdc, TRANSPARENT);

	SetTextColor(S_Hdc, RGB(R, G, B));
	TextOut(S_Hdc, posX, posY, text, strlen(text));

	DeleteObject(hFont);
}

//=====================================================================================================//
//=====================================================================================================//
bool SpriteClass2D::DrawTexC(int index, TexContext texInfo, float X, float Y, float Z, bool rotate)
{

	X -= C_WindW;
	Y -= C_WindH;

	const float gapX = texInfo.R_map - texInfo.L_map;
	const float gapY = texInfo.D_map - texInfo.U_map;

	const float charWidth = 2 * gapX / C_WindW;
	const float charHeight = 2 * gapY / C_WindH;
	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to map resource to texture!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//
	float StaX = X / C_WindW - charWidth / 2;
	float StaY = Y / C_WindH - charHeight / 2;
	float EndX = X / C_WindW + charWidth / 2;
	float EndY = Y / C_WindH + charHeight / 2;
	float LayerZ = Z / 100.0f;

	spritePtr[0].pos = XMFLOAT3(EndX, EndY, LayerZ);
	spritePtr[1].pos = XMFLOAT3(EndX, StaY, LayerZ);
	spritePtr[2].pos = XMFLOAT3(StaX, StaY, LayerZ);
	spritePtr[3].pos = XMFLOAT3(StaX, StaY, LayerZ);
	spritePtr[4].pos = XMFLOAT3(StaX, EndY, LayerZ);
	spritePtr[5].pos = XMFLOAT3(EndX, EndY, LayerZ);

	if (!rotate)
	{
		const float texEdgeL = (texInfo.L_map + gapX * index) / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = texInfo.U_map / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}
	else
	{
		const float texEdgeL = texInfo.L_map / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = (texInfo.U_map + gapY * index) / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}


	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(TexVertex, 0);

	return true;
}

bool SpriteClass2D::DrawTexC(int index, TexContext texInfo, float X, float Y, float Z, float streX, float streY, float RotAng, bool rotate)
{

	X -= C_WindW;
	Y -= C_WindH;
	X /= C_WindW;
	Y /= C_WindH;
	RotAng *= (6.12831855f);

	float gapX = (texInfo.R_map - texInfo.L_map);
	float gapY = (texInfo.D_map - texInfo.U_map);
	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to map resource to texture!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//
	float EX = gapX * streX;
	float EY = gapY * streY;
	float NX = -gapX * streX;
	float NY = -gapY * streY;

	float c = cosf(RotAng);
	float s = sinf(RotAng);

	float X1 = EX*c + EY*s;
	float Y1 = EY*c - EX*s;

	float X2 = EX*c + NY*s;
	float Y2 = NY*c - EX*s;

	float X3 = -X1;		float Y3 = -Y1;
	float X4 = -X2;		float Y4 = -Y2;

	X1 /= C_WindW;	X2 /= C_WindW;	X3 /= C_WindW;	X4 /= C_WindW;
	Y1 /= C_WindH;	Y2 /= C_WindH;	Y3 /= C_WindH;	Y4 /= C_WindH;

	X1 += X;	X2 += X;	X3 += X;	X4 += X;
	Y1 += Y;	Y2 += Y;	Y3 += Y;	Y4 += Y;

	float LayerZ = Z / 100.0f;

	spritePtr[0].pos = XMFLOAT3(X1, Y1, LayerZ);
	spritePtr[1].pos = XMFLOAT3(X2, Y2, LayerZ);
	spritePtr[2].pos = XMFLOAT3(X3, Y3, LayerZ);
	spritePtr[3].pos = XMFLOAT3(X3, Y3, LayerZ);
	spritePtr[4].pos = XMFLOAT3(X4, Y4, LayerZ);
	spritePtr[5].pos = XMFLOAT3(X1, Y1, LayerZ);
	//----------------------------------------------------------//
	//----------------------ALPHA BLEND-------------------------//
	if (!rotate)
	{
		const float texEdgeL = (texInfo.L_map + gapX * index) / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = texInfo.U_map / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}
	else
	{
		const float texEdgeL = texInfo.L_map / texInfo.sizeX;
		const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
		const float texEdgeU = (texInfo.U_map + gapY * index) / texInfo.sizeY;
		const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

		spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
		spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
		spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
		spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
		spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	}
	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(TexVertex, 0);
	return true;
}

bool SpriteClass2D::DrawTex(int indexX, int indexY, TexContext texInfo, float X, float Y, float Z)
{

	X -= C_WindW;
	Y -= C_WindH;

	const float gapX = texInfo.R_map - texInfo.L_map;
	const float gapY = texInfo.D_map - texInfo.U_map;

	const float charWidth = 2 * gapX / C_WindW;
	const float charHeight = 2 * gapY / C_WindH;
	//------------------------------------------------------------//
	//---------------------Map Resource---------------------------//
	//------------------------------------------------------------//
	D3D11_MAPPED_SUBRESOURCE	mapResource;
	HRESULT d3dResult = m_context_ptr->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);

	if (FAILED(d3dResult))
	{
		MessageBox(NULL, _T("Failed to map resource to texture!"), _T("Error!"), MB_OK);
		return false;
	}

	VertexPos*	spritePtr = (VertexPos*)mapResource.pData;

	//------------------------------------------------------------//
	//----------------------Numeration----------------------------//
	//----------------------Vertex Buffer-------------------------//

	float EndX = X / C_WindW + charWidth;
	float EndY = Y / C_WindH + charHeight;
	float LayerZ = Z / 100.0f;

	spritePtr[0].pos = XMFLOAT3(EndX, EndY, LayerZ);
	spritePtr[1].pos = XMFLOAT3(EndX, Y / C_WindH, LayerZ);
	spritePtr[2].pos = XMFLOAT3(X / C_WindW, Y / C_WindH, LayerZ);
	spritePtr[3].pos = XMFLOAT3(X / C_WindW, Y / C_WindH, LayerZ);
	spritePtr[4].pos = XMFLOAT3(X / C_WindW, EndY, LayerZ);
	spritePtr[5].pos = XMFLOAT3(EndX, EndY, LayerZ);

	const float texEdgeL = (texInfo.L_map + gapX * indexX) / texInfo.sizeX;
	const float texEdgeR = texEdgeL + (gapX / texInfo.sizeX);
	const float texEdgeU = (texInfo.U_map + gapY * indexY) / texInfo.sizeY;
	const float texEdgeD = texEdgeU + (gapY / texInfo.sizeY);

	spritePtr[0].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	spritePtr[1].tex0 = XMFLOAT2(texEdgeR, texEdgeD);
	spritePtr[2].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
	spritePtr[3].tex0 = XMFLOAT2(texEdgeL, texEdgeD);
	spritePtr[4].tex0 = XMFLOAT2(texEdgeL, texEdgeU);
	spritePtr[5].tex0 = XMFLOAT2(texEdgeR, texEdgeU);
	//------------------------------------------------------------//
	//-------------------------Drawing----------------------------//
	//------------------------------------------------------------//

	m_context_ptr->Unmap(vertexBuffer, 0);
	m_context_ptr->Draw(TexVertex, 0);

	return true;
}

inline void SpriteClass2D::GetFPS()
{
	++framecount;
	FPS_TimeNow = CountTime();
	if (FPS_TimeNow - FPS_TimePrev > 100.0f)
	{
		fps = framecount;
		FPS_TimePrev = FPS_TimeNow;
		framecount = 0;
	}
}