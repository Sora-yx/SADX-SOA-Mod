#include "pch.h"

Trampoline* RunLevelDestructor_t = nullptr;

void Chao_CreatePlayerControlManager_r()
{
	if (CurrentChaoStage == 8)
		LoadPirateBase_Geometry();

	return Chao_CreatePlayerControlManager();
}

void __cdecl RunLevelDestructor_r(int heap)
{
	if (heap == 0)
	{
		PirateIsle_Unload();
	}

	TARGET_DYNAMIC(RunLevelDestructor)(heap);
}


void init_SOALevels()
{
	RunLevelDestructor_t = new Trampoline((int)RunLevelDestructor, (int)RunLevelDestructor + 0x6, RunLevelDestructor_r);
	init_PirateIsle();
	WriteCall((void*)0x715956, Chao_CreatePlayerControlManager_r);
}