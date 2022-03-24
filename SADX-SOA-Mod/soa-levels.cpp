#include "pch.h"

Trampoline* RunLevelDestructor_t = nullptr;
Trampoline* LoadSkyboxObject_t = nullptr;
Trampoline* MovePlayerToStartPoint_t = nullptr;
Trampoline* DeathZoneHandler_t = nullptr;
Trampoline* Obj_MysticRuins_t = nullptr;

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

bool isSoaLevel()
{
	return CurrentLevel == LevelIDs_MysticRuins;
}

void LoadLevelFiles_r()
{

	LoadLevelFiles();

	if (isSoaLevel())
	{
		ReleaseSetFile();
		ReleaseCamFile();
		LoadShrineIsle_Geometry();
		LoadFileWithMalloc("CAM0000S.bin", (LPVOID*)&CamData[CurrentAct]);
		CurrentCamData[CurrentAct] = CamData[CurrentAct];
		return;
	}
}

void __cdecl DeathZoneHandler_r(ObjectMaster* obj)
{
	if (isSoaLevel() == false)
	{
		obj->MainSub = TARGET_DYNAMIC(DeathZoneHandler);
	}
}

void __cdecl ShrineIsle_Main_r(ObjectMaster* obj);
void __cdecl MovePlayerToStartPoint_r(EntityData1* data)
{
	if (isSoaLevel() == false)
	{
		TARGET_DYNAMIC(MovePlayerToStartPoint)(data);
	}
}

void __cdecl LoadSkyboxObject_r()
{
	if (isSoaLevel() == false)
	{
		TARGET_DYNAMIC(LoadSkyboxObject)();
	}
}

void __cdecl Obj_MysticRuins_r(ObjectMaster* obj)
{
	if (isSoaLevel() == false)
	{
		TARGET_DYNAMIC(Obj_MysticRuins)(obj);
	}

	obj->MainSub = ShrineIsle_Main_r;
}

void init_SOALevels()
{
	RunLevelDestructor_t = new Trampoline((int)RunLevelDestructor, (int)RunLevelDestructor + 0x6, RunLevelDestructor_r);
	LoadSkyboxObject_t = new Trampoline((int)LoadSkyboxObject, (int)LoadSkyboxObject + 0x7, LoadSkyboxObject_r);
	MovePlayerToStartPoint_t = new Trampoline((int)MovePlayerToStartPoint, (int)MovePlayerToStartPoint + 0x6, MovePlayerToStartPoint_r);
	DeathZoneHandler_t = new Trampoline((int)DeathZoneHandler, (int)DeathZoneHandler + 0x8, DeathZoneHandler_r);
	Obj_MysticRuins_t = new Trampoline((int)Obj_MysticRuins, (int)Obj_MysticRuins + 0x6, Obj_MysticRuins_r);


	init_PirateIsle();
	WriteCall((void*)0x715956, Chao_CreatePlayerControlManager_r);

	WriteCall((void*)0x415308, LoadLevelFiles_r);
	WriteCall((void*)0x4152C9, LoadLevelFiles_r);

}