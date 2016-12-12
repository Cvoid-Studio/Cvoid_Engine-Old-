#pragma once
#ifndef GRAPHICCLASS_H_
#define GRAPHICCLASS_H_


#define FileNameLength	64


struct TexContext
{
	float	sizeX;
	float	sizeY;

	float	L_map;
	float	R_map;
	float	U_map;
	float	D_map;
};

struct MS_FileInfo
{
	float	Width;
	float	Height;
};


struct MS_Generate
{
	float	timer;
	int		count1;
	bool	PN1;
};


#endif