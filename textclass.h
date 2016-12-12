#pragma once

#ifndef TEXTCLASS_H_
#define TEXTCLASS_H_

#include "DXBase.h"
//--------------------------------------------------//
class TextClass : public DXBase
{
public:
	TextClass();
	~TextClass();
	
	bool LoadContent();
	void Render();
	void UnloadContent();
	void Update(float);

private:
	bool DrawString(char* message, float startX, float startY);



private:
	ID3D11VertexShader*	SolidColorVS;
	ID3D11PixelShader*	SolidColorPS;
	ID3D11InputLayout*	InputLayout;
	ID3D11Buffer*		vertexBuffer;

	//Texture//
	ID3D11ShaderResourceView*	m_colorMap;
	ID3D11SamplerState*			m_colorMapSampler;
};

#endif
