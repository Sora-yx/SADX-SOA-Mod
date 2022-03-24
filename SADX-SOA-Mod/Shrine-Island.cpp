#include "pch.h"

NJS_TEXNAME ShrineTex_TexNames[87];
NJS_TEXLIST Shrine_TexList = { arrayptrandlength(ShrineTex_TexNames) };

LandTableInfo* ShrineIsleGeo = nullptr;

NJS_VECTOR TriggerLeaveShrine = { 81, 157, 283 };
NJS_VECTOR startPosShrine = { -65, 0, 851 };

ModelInfo* Shrine_SkyBox[4];
ModelInfo* Shrine_Water[4];

void Shrine_Delete(ObjectMaster* obj)
{
	for (Uint8 i = 0; i < LengthOfArray(Shrine_SkyBox); ++i) {
		FreeMDL(Shrine_SkyBox[i]);
		Shrine_Water[i];
	}
	return;
}

void Load_ShrineMDL()
{
	for (Uint8 i = 0; i < LengthOfArray(Shrine_SkyBox); ++i) {
		std::string str = "ShrineIsle-BG" + std::to_string(i);
		Shrine_SkyBox[i] = LoadBasicModel(str.c_str());
	}

	for (Uint8 i = 0; i < LengthOfArray(Shrine_SkyBox); ++i) {
		std::string str = "Shrine-Water" + std::to_string(i);
		Shrine_Water[i] = LoadBasicModel(str.c_str());
	}

}

void SHrine_PlayerANTIOob()
{
	char posx = 5;

	for (uint8_t i = 0; i < 8; i++) {

		if (!EntityData1Ptrs[i])
			continue;

		if (EntityData1Ptrs[i]->Position.y <= -210) {
			EntityData1Ptrs[i]->Position = startPosShrine;
			EntityData1Ptrs[i]->Rotation.y = 0x8000;

			if (i)
				EntityData1Ptrs[i]->Position.x += posx;

			posx += 5;
		}
	}
}

void ShrineIsle_DrawWater(int num)
{
	if (MissedFrames || !CurrentLandTable)
		return;

	njPushMatrix(_nj_current_matrix_ptr_);

	if (!num > 0 && num != 2)
	{
		Shrine_Water[num]->getmodel()->pos[1] =  2.0f;
	}
	AnimateUV_TexID(Shrine_Water[num]->getmodel()->child->child->basicdxmodel, 68, 0, 1);
	AnimateUV_TexID(Shrine_Water[num]->getmodel()->child->child->basicdxmodel, 69, 0, 1);
	njTranslate(_nj_current_matrix_ptr_, Shrine_Water[num]->getmodel()->pos[0], Shrine_Water[num]->getmodel()->pos[1], Shrine_Water[num]->getmodel()->pos[2]);
	dsDrawObject(Shrine_Water[num]->getmodel());
	

	njPopMatrix(1);
}

void ShrineIsle_Display(ObjectMaster* obj)
{
	if (MissedFrames || !CurrentLandTable)
		return;

	EntityData1* data = obj->Data1;

	njControl3D_Backup();
	njControl3D_Add(NJD_CONTROL_3D_NO_CLIP_CHECK);
	njControl3D_Remove(NJD_CONTROL_3D_DEPTH_QUEUE);
	Direct3D_SetNearFarPlanes(SkyboxDrawDistance.Minimum, SkyboxDrawDistance.Maximum);
	DisableFog();

	njSetTexture(CurrentLandTable->TexList);


	for (uint8_t i = 0; i < 4; i++)
	{
		ShrineIsle_DrawWater(i);
	}


	njPushMatrix(_nj_current_matrix_ptr_);
	njTranslate(_nj_current_matrix_ptr_, Camera_Data1->Position.x, 0, Camera_Data1->Position.z);


	for (uint8_t i = 0; i < LengthOfArray(Shrine_SkyBox); i++)
	{
		dsDrawObject(Shrine_SkyBox[i]->getmodel());
	}

	njPopMatrix(1);




	ToggleStageFog();
	Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);
	njControl3D_Restore();



}

void __cdecl ShrineIsle_Main_r(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	switch (data->Action)
	{
	case 0:
		FreeQueueSound();
		LoadSoundList(38);
		SetGlobalPoint2Col_Colors(0xFF000000, 0xFF000000, 0xFF000000);
		LevelFogData.Toggle = 0;

		StopMusic();

		LevelDrawDistance.Minimum = -1.0;
		SetDrawingDistances(20000.0f, 58000.0);
		SetLevelFog(-2.0f, 18000.0f, 0xFF000000);
		obj->DisplaySub = ShrineIsle_Display;
		data->Action++;
		break;
	case 1:
		if (++data->Index == 5)
		{
			for (uint8_t i = 0; i < 8; i++) {

				if (EntityData1Ptrs[i]) {
					EntityData1Ptrs[i]->Position = startPosShrine;

					if (i)
					{
						EntityData1Ptrs[i]->Position.x += 5;
					}
				}
			}

			data->Position = TriggerLeaveShrine;

			data->Action++;
		}
		break;
	case 2:
		SHrine_PlayerANTIOob();
		if (IsPlayerInsideSphere(&data->Position, 12))
		{
			ForcePlayerAction(0, 12);
			data->Action++;
			//GoTo_CustomChaoArea(PirateIsle);
		}

		break;
	}

	obj->DisplaySub(obj);
}

void LoadShrineIsle_Geometry()
{

	PrintDebug("SOA Mod: Shrine Isle...\n");

	LoadLandTableFile(&ShrineIsleGeo, "system\\ShrIsl.sa1lvl", &Shrine_TexList);
	LandTable* land = ShrineIsleGeo->getlandtable();
	//GeoLists[ShrineIsle * 8] = land;
	GeoLists[LevelIDs_MysticRuins * 8 + 6] = land;

	for (int i = 0; i < land->COLCount; ++i) {

		if (land->Col[i].Flags & ColFlags_Visible) {
			RegisterLandPalette(land->Col[i].Model);
		}
	}

	Load_ShrineMDL();
	PrintDebug("SOA Mod: Load over.\n");
	return;
}

