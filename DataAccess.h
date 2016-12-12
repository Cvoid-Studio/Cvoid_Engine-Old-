#pragma once

#ifndef DATAACCESS_H  
#define DATAACCESS_H 

#include "DLL_API_BASE.h"
#include "CVGraph2D.h"

class DataAccess
{
public:
	DataAccess() {};
	~DataAccess() {};

	static CDLL_API_GRAPH* CVcreateGraphDevice()
	{
		return (new SpriteClass2D);
	}
};

#endif  