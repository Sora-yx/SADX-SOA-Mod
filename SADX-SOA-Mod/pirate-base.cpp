#include "pch.h"


NJS_TEXNAME PirateBase_TexNames[60];
NJS_TEXLIST PirateBase_TexList = { arrayptrandlength(PirateBase_TexNames) };

LandTableInfo* PirateBaseGeo = nullptr;

void PirateBase_Delete(ObjectMaster* obj)
{
	UnsetPaletteBlendMode();
	MusicList[MusicIDs_chao].Name = "chao";
	Reset_LadderHack();
	ChaoStgGarden01EC_Delete(obj);
	return;
}


void Load_PirateBaseMDL()
{

}

NJS_VECTOR startPosPBase = { 76, 157, 192 };

void __cdecl PirateBase_Load_r(ObjectMaster* obj)
{

	EntityData1* data = obj->Data1;

	switch (data->Action)
	{
	case 0:
		ReleaseSetFile();
		SetGlobalPoint2Col_Colors(0xFF000000, 0xFF000000, 0xFF000000);
		LevelFogData.Toggle = 0;
		//LoadChaoCamCol();
		Chao_CreateNormalCameraTask();
		AlMsgFontInit();

		MusicList[MusicIDs_chao].Name = "Pirate_Secret_Base";
		PlayMusic(MusicIDs_chao);

		LevelDrawDistance.Minimum = -1.0;
		SetDrawingDistances(20000.0f, 58000.0);
		SetLevelFog(-2.0f, 18000.0f, 0xFF000000);
		data->Action++;
		break;
	case 1:
		if (++data->Index == 5)
		{
			EntityData1Ptrs[0]->Position = startPosPBase;
			data->Action++;
		}
	case 2:

		break;
	}

}

void LoadPirateBase_Geometry()
{

	PrintDebug("SOA Mod: Load Pirate Base...\n");

	LoadLandTableFile(&PirateBaseGeo, "system\\PirateBase.sa1lvl", &PirateBase_TexList);
	LandTable* land = PirateBaseGeo->getlandtable();
	LoadObject(LoadObj_Data1, 2, PirateBase_Load_r);

	for (int i = 0; i < land->COLCount; ++i) {

		if (land->Col[i].Flags & ColFlags_Visible) {
			RegisterLandPalette(land->Col[i].Model);
		}
	}


	SetChaoLandTable(land);
	PrintDebug("SOA Mod: Load over.\n");

	return;
}


void GoTo_PirateBase()
{
	NextChaoStage = PirateSecretBase;
	dword_3C85EE4 = 1; //force Chao Garden Manager to the next Chao Level
	return;
}

