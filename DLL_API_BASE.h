#pragma once
#include "GraphicClass.h"


//--------------------MACRO----------------------//
#ifndef DLL_API_BASE_H
#define DLL_API_BASE_H

#include "Interface.h"

DeclareInterface(CDLL_API_GRAPH)

virtual bool LoadContent();
virtual void Render();
virtual void UnloadContent();
virtual void Update(float);
//++++++++++++++++++++++++++++++++++++++++++++++++++++//
virtual bool  DrawTex(int index, TexContext texInfo, int subordinate, float X, float Y, float Z, bool rotate) = 0;
virtual bool  DrawTex(int index, TexContext texInfo, int subordinate, float X, float Y, float Z, float streX, float streY, float Alpha, float RotAng, bool rotate) = 0;
virtual bool  DrawTex(MS_FileInfo fileInfo, float X, float Y, float Z) = 0;
virtual bool  DrawTex(MS_FileInfo fileInfo, float X, float Y, float Z, float streX, float streY, float Alpha, float RotAng) = 0;

virtual bool  DrawString(char* message, float startX, float startY) = 0;

virtual void  PaintText(int posX, int posY, int height, int width, int weight, char* text, int R, int G, int B) = 0;

virtual void  setSource(int index) = 0;

virtual	float CountTime() = 0;

EndInterface

#endif