//====================INCLUDES=======================//
#include "SystemClass.h"

//====================DIAGRAMS=======================//
/*			----------------------------------------------------
I			Game_Main.cpp <== <SystemClass.h> <-- SystemFunc.cpp
			------------------------||--------------------------
									

*/

//====================GLOBALS========================//

//====================FUNCTIONS======================//

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prehinstance,
				   LPSTR szCmd, int nShow)
{
	SystemClass*	C_System;
	bool	result;

	UNREFERENCED_PARAMETER(prehinstance);
	UNREFERENCED_PARAMETER(nShow);

	C_System = new SystemClass;
	if (!C_System)
		return -1;

	result = C_System->Sys_Initialize();

	C_System->Pre_hinstance = prehinstance;

	if (result)
	{
		C_System->Run();
	}
		

	C_System->Sys_ShutDown();
	delete C_System;
	C_System = NULL;

	return 0;
}



