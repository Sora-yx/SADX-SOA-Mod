#include "pch.h"

HelperFunctions HelperFunctionsGlobal;
std::string modpath;
void __cdecl PirateBase_Main_r(ObjectMaster* obj);

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		if (helperFunctions.Version < 11)
		{
			MessageBox(WindowHandle,
				L"Error, your version of the mod loader does not support API version 11. Some functionality will not be available.\nPlease exit the game and update the mod loader for the best experience.",
				L"Mod Name Error: Mod Loader out of date", MB_OK | MB_ICONERROR);
		}

		HelperFunctionsGlobal = helperFunctions; // Save the helper pointer for external use

		modpath = path;
		ReadConfig(path, helperFunctions);
		init_SOALevels();
		init_ChaoFixes();
		init_ObjectsHack();
		init_ObjsCommon();
	}

	__declspec(dllexport) void __cdecl OnInitEnd() {
		
		LightingAPI_InitEnd();

	}


	__declspec(dllexport) void __cdecl OnFrame()
	{
		if (GameMode == 9)
		{
			GameMode = (GameModes)5;
		}

	}

	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
}