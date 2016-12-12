#pragma once
#ifndef DIRECTINPUTCLASS_H_
#define DIRECTINPUTCLASS_H_

#include "DXBase.h"

//====================CLASS==========================//
class KeyBoardInput : public DXBase
{
public:
	KeyBoardInput();
	~KeyBoardInput();

	bool LoadContent();
	void Render();
	void UnloadContent();
	void Update(float);


private:
	ID3D11VertexShader*	SolidColorVS;
	ID3D11PixelShader*	SolidColorPS;
	ID3D11InputLayout*	InputLayout;
	ID3D11Buffer*		vertexBuffer;
	ID3D11Buffer*		PosCB;

	ID3D11ShaderResourceView*	m_colorMap;
	ID3D11SamplerState*			m_colorMapSampler;

	float selectedPosX;
	float selectedPosY;

};



#endif