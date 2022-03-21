#include "pch.h"

NJS_TEXNAME PirateIsle_TexNames[161];
NJS_TEXLIST PirateIsle_TexList = { arrayptrandlength(PirateIsle_TexNames) };

NJS_TEXNAME Houses_TexNames[176];
NJS_TEXLIST Houses_TexList = { arrayptrandlength(Houses_TexNames) };

LandTableInfo* PirateIsleGeo = nullptr;
LandTableInfo* HousesGeo = nullptr;

static ModelInfo* PirateSkyBox[3];

NJS_TEXNAME PirateBG_TexNames[7];
NJS_TEXLIST PirateBG_TexList = { arrayptrandlength(PirateBG_TexNames) };

NJS_TEXNAME PirateBGEvening_TexNames[8];
NJS_TEXLIST PirateBG2_TexList = { arrayptrandlength(PirateBGEvening_TexNames) };

NJS_TEXNAME PirateBGNight_TexNames[7];
NJS_TEXLIST PirateBG3_TexList = { arrayptrandlength(PirateBGNight_TexNames) };

NJS_VECTOR startpos = { -64, 0, 73.00 };
NJS_VECTOR startPosWarp = { -14, 13, -192 };

SecondaryEntrance Pirate_StartPos = { startpos, -0x4000 };
SecondaryEntrance Pirate_StartPosWarp = { startPosWarp, 0x0 };

void Draw_SkyBoxDay(EntityData1* data)
{
	if (getTimeOfDay() != day || !data->Object)
		return;

	NJS_OBJECT* cloud[3] = { data->Object->child->sibling->sibling->sibling->sibling->sibling };
	cloud[1] = cloud[0]->sibling;
	cloud[2] = cloud[1]->sibling;

	AnimateUV_TexID(cloud[0]->basicdxmodel, 4, 0, 1);
	AnimateUV_TexID(cloud[1]->basicdxmodel, 6, 0, 1);
	AnimateUV_TexID(cloud[2]->basicdxmodel, 6, 0, 1);

	DrawQueueDepthBias = -99999;

	njSetTexture(&PirateBG_TexList);
	njPushMatrix(_nj_current_matrix_ptr_);
	njTranslate(_nj_current_matrix_ptr_, Camera_Data1->Position.x, 0, Camera_Data1->Position.z);
	dsDrawObject(data->Object);
	njPopMatrix(1);
	DrawQueueDepthBias = 0;
}

void Draw_SkyBoxEvening()
{
	if (getTimeOfDay() != evening)
		return;

	NJS_OBJECT* skybox = PirateSkyBox[evening]->getmodel();

	if (!skybox)
		return;

	NJS_OBJECT* cloud[2] = { skybox->child->sibling->sibling->sibling->sibling->sibling };
	cloud[1] = cloud[0]->sibling->sibling;

	njSetTexture(&PirateBG2_TexList);
	AnimateUV_TexID(cloud[0]->basicdxmodel, 6, 1, 0);
	AnimateUV_TexID(cloud[1]->basicdxmodel, 8, 0, -1);

	njSetTexture(&PirateBG2_TexList);
	njPushMatrix(_nj_current_matrix_ptr_);
	njTranslate(_nj_current_matrix_ptr_, Camera_Data1->Position.x, 0, Camera_Data1->Position.z);
	dsDrawObject(skybox);
	njPopMatrix(1);
}

void Draw_SkyBoxNight()
{
	if (getTimeOfDay() != night)
		return;

	NJS_OBJECT* skybox = PirateSkyBox[night]->getmodel();

	if (!skybox)
		return;

	njSetTexture(&PirateBG3_TexList);
	njPushMatrix(_nj_current_matrix_ptr_);
	njTranslate(_nj_current_matrix_ptr_, Camera_Data1->Position.x, 0, Camera_Data1->Position.z);
	dsDrawObject(skybox); //fix transparency issue
	njPopMatrix(1);

	AnimateUV_TexID(skybox->child->sibling->sibling->sibling->sibling->sibling->basicdxmodel, 6, 1, 0);
	AnimateUV_TexID(skybox->child->sibling->sibling->basicdxmodel, 3, 1, 0);
}


void PirateIsle_Display(ObjectMaster* obj)
{
	if (MissedFrames)
		return;

	EntityData1* data = obj->Data1;

	njControl3D_Backup();
	njControl3D_Add(NJD_CONTROL_3D_NO_CLIP_CHECK);
	njControl3D_Remove(NJD_CONTROL_3D_DEPTH_QUEUE);
	Direct3D_SetNearFarPlanes(SkyboxDrawDistance.Minimum, SkyboxDrawDistance.Maximum);
	DisableFog();

	Draw_SkyBoxDay(data);
	Draw_SkyBoxEvening();
	Draw_SkyBoxNight();

	ToggleStageFog();
	Direct3D_SetNearFarPlanes(LevelDrawDistance.Minimum, LevelDrawDistance.Maximum);
	njControl3D_Restore();

	LandTable* land = HousesGeo->getlandtable();

	float color = (1.0f - powf(njSin(data->field_A), 2.0f)) * 0.6f + 0.2f;

	for (int i = 0; i < land->COLCount; ++i) {
		if (IsPlayerInsideSphere(&land->Col[i].Center, powf(land->Col[i].Radius, 2))) {
			NJS_MATERIAL* mat = land->Col[i].Model->basicdxmodel->mats;

			if (mat->attr_texId == 0 || mat->attr_texId == 6) {
				for (Uint8 j = 0; j < land->Col[i].Model->basicdxmodel->nbMat; ++j) {
					mat[j].diffuse.argb.b = static_cast<Uint8>(color * 255);
					mat[j].diffuse.argb.r = static_cast<Uint8>(color * 255);
					mat[j].diffuse.argb.g = static_cast<Uint8>(color * 255);
				}
			}

			njSetTexture(&Houses_TexList);
			njPushMatrix(0);
			DrawObject(land->Col[i].Model);
			njPopMatrix(1u);
		}
	}
}

void PirateIsle_Skybox(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	if (!data->Action) {
		data->Object = PirateSkyBox[day]->getmodel();
		obj->DisplaySub = PirateIsle_Display;
		data->Action++;
	}

	obj->DisplaySub(obj);
}

void PlayerANTIOob()
{
	char posx = 5;

	for (uint8_t i = 0; i < 8; i++) {

		if (!EntityData1Ptrs[i])
			continue;

		if (EntityData1Ptrs[i]->Position.y <= -210) {
			EntityData1Ptrs[i]->Position = startpos;
			EntityData1Ptrs[i]->Rotation.y = 0x8000;

			if (i)
				EntityData1Ptrs[i]->Position.x += posx;

			posx += 5;
		}
	}
}

void SetNumberMaxOfTree(uint8_t number)
{
	WriteData<1>((int*)0x717bc6, number);
	return;
}

void PirateIsle_Unload()
{
	if (LastLevel == LevelIDs_ECGarden || LastLevel == LevelIDs_MRGarden || LastLevel == LevelIDs_SSGarden) {
		for (uint8_t i = 0; i < LengthOfArray(PirateSkyBox); i++) {
			FreeMDL(PirateSkyBox[i]);
		}

		FreeLandTableFile(&PirateIsleGeo);
		njReleaseTexture(&PirateIsle_TexList);
		njReleaseTexture(&PirateBG_TexList);
		njReleaseTexture(&PirateBG2_TexList);
		njReleaseTexture(&PirateBG3_TexList);

	}
}

void PirateIsle_Delete(ObjectMaster* obj)
{
	UnsetPaletteBlendMode();
	SetNumberMaxOfTree(5);
	MusicList[MusicIDs_chao].Name = "chao";
	Reset_LadderHack();
	ChaoStgGarden01EC_Delete(obj);
	return;
}

void Move_WayPoints_ToNewPose()
{
	for (uint8_t j = 0; j < 16; j++) {

		for (int i = 0; i < ChaoWayPoints[j].nbIndex; i++)
		{
			ChaoWayPoints[j].pos[i].x -= 5.0f;
		}
	}
}

void PirateIsle_Garden(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	switch (data->Action)
	{
	case 0:
		Move_WayPoints_ToNewPose();
		LoadPirateIsle_Objects();
		LoadChildObject(LoadObj_Data1, Garden_TimeOfDay, obj);
		data->Position = { 17, -40, 221 };
		data->Action++;
		break;
	case 1:
		if (!isLeavingGarden)
			PlayerANTIOob();

		if (IsPlayerInsideSphere(&data->Position, 20))
		{
			isLeavingGarden = true;
			ForcePlayerAction(0, 12);
			DisableController(0);
			data->Action++;
			GoTo_PirateBase();
		}

		break;
	}

	RunObjectChildren(obj);
}

void Load_PirateMDL()
{
	LoadPVM("housesTex", &Houses_TexList);

	PirateSkyBox[day] = LoadBasicModel("PirateIsle-SkyBox");
	PirateSkyBox[evening] = LoadBasicModel("PirateIsle-SkyBox2");
	PirateSkyBox[night] = LoadBasicModel("PirateIsle-SkyBox3");

	LoadPVM("PirateIsle-BGTex", &PirateBG_TexList);
	LoadPVM("PirateIsle-BGTex2", &PirateBG2_TexList);
	LoadPVM("PirateIsle-BGTex3", &PirateBG3_TexList);

	LoadLittleJack_ModelAnim();
	LoadOBJModels();
}


void __cdecl ChaoStgGarden01EC_Load_r(ObjectMaster* parent)
{

	Set_LadderHack();
	NJS_VECTOR position;

	LoadObject(LoadObj_Data1, 8, init_SetObj); //in SADX the gardens don't use any set object, we manually load those

	SetGlobalPoint2Col_Colors(0xFF000000, 0xFF000000, 0xFF000000);
	LevelFogData.Toggle = 0;
	//LoadChaoCamCol();
	Chao_CreateNormalCameraTask();
	AlMsgFontInit();
	LoadChaoTexlist("AL_TEX_COMMON", &ChaoTexLists[1], 1u);
	LoadChaoTexlist("AL_DX_ETC", &AL_DX_ETC_TEXLIST, 1u);
	LoadChaoTexlist("AL_OBJECT", &AL_OBJECT_TEXLIST, 1u);
	LoadChaoTexlist("AL_TOY", &AL_TOY_TEXLIST, 1u);
	LoadChaoTexlist("AL_DX_OBJ_CMN", &AL_DX_OBJ_CMN_TEXLIST, 1u);
	LoadChaoTexlist("al_dx_tex_xybutton", &al_dx_tex_xybutton_TEXLIST, 1u);
	LoadAnimalPVMs();
	sub_722500();
	ChaoManager_Load();
	SetChaoOnTheGarden();
	Chao_LoadTrees();
	position.x = -135.0;
	position.y = 0.0;
	position.z = 62.0;
	LoadChaoNameMachine(&position, 0x0);
	LoadChaoTransporter(parent, SADXChaoStage_MysticRuins, -20.0, 13.44, -240.0, 0x0);
	LoadChaoTransporter(parent, SADXChaoStage_StationSquare, 5.0, 13.44, -240.0, 0x0);
	LoadChaoTransporter(parent, 8, 487.0, 161.5300002, -109.0, 0x4000);
	LoadHeldChao();
	LoadHeldItem();
	ChaoDX_Message_PlayerAction_Load();
	SetChaoFlag();
	InitializeSoundManager();


	if (timeDayOption != alwaysNight)
		MusicList[MusicIDs_chao].Name = "Sky_Pirate_Isle";
	else
		MusicList[MusicIDs_chao].Name = "Reflection";

	PlayMusic(MusicIDs_chao);

	ReleaseSetFile();
	std::string path = "SETPIRATE.BIN";
	LoadFileWithMalloc(path.c_str(), (LPVOID*)&SetFiles[CurrentAct]);
	SetFileB[CurrentAct] = SetFiles[CurrentAct];
	SetFileB[CurrentAct]->XRotation = SetFileB[CurrentAct]->XRotation;
	parent->MainSub = PirateIsle_Garden;
	parent->DeleteSub = PirateIsle_Delete;
	LevelDrawDistance.Minimum = -1.0;
	SetDrawingDistances(20000.0f, 58000.0);
	SetLevelFog(-2.0f, 18000.0f, 0xFF000000);
	CurrentSetFile = CurrentSetFile;
}


void SetNextChaoStage_r(int stage)
{

	if (CurrentChaoStage != stage) {

		LastChaoStage = CurrentChaoStage;
		NextChaoStage = stage;
	}

	if (stage == SADXChaoStage_EggCarrier)
	{
		SetNextLevelAndAct_CutsceneMode(LevelIDs_ECGarden, 0);
	}

}

void LoadPirateIsle_Garden()
{

	PrintDebug("SOA Mod: Load Pirate Isle Garden...\n");

	LoadLandTableFile(&PirateIsleGeo, "system\\PirateIsle.sa1lvl", &PirateIsle_TexList);
	LandTable* land = PirateIsleGeo->getlandtable();
	GeoLists[LevelIDs_ECGarden * 8] = land;

	for (int i = 0; i < land->COLCount; ++i) {

		if (land->Col[i].Flags & ColFlags_Visible) {
			RegisterLandPalette(land->Col[i].Model);
		}
	}

	SetChaoLandTable(land);

	LoadLandTableFile(&HousesGeo, "system\\Houses.sa1lvl", &Houses_TexList);
	auto extraLand = HousesGeo->getlandtable();

	for (int i = 0; i < extraLand->COLCount; ++i) {

		if (extraLand->Col[i].Flags & ColFlags_Visible) {
			RegisterLandPalette(extraLand->Col[i].Model);
		}
	}


	Load_PirateMDL();
	LoadObject(LoadObj_Data1, 3, ChaoStgGarden01EC_Load_r);
	LoadObject(LoadObj_Data1, 2, PirateIsle_Skybox);
	PrintDebug("SOA Mod: Load over.\n");

	return;
}

void SetNewTreePos()
{
	ChaoTreeSpawns[1].a = { 46, 13, -254 };
	ChaoTreeSpawns[1].b = { 94, 26, 91 };
	ChaoTreeSpawns[1].c = { -100, 0, -112 };
	SetNumberMaxOfTree(3);
};


void init_PirateIsle()
{
	WriteData<5>((void*)0x423795, 0x90u); //Prevent DC Mod to load Chao Garden stuff.
	WriteCall((void*)0x71570E, SetNextChaoStage_r); //Fix Objects not spawning 
	WriteJump(LoadECGarden, LoadPirateIsle_Garden);
	SetNewTreePos();

	WriteData((SecondaryEntrance**)0x7152de, &Pirate_StartPos);
	WriteData((SecondaryEntrance**)0x7152e5, &Pirate_StartPosWarp);


	Chao_ECChaoSpawnPoints[0] = { -92, -0, 124 };
	Chao_ECChaoSpawnPoints[1] = { -40, -0, 127 };
	Chao_ECChaoSpawnPoints[2] = { 10, -0, 121 };
	Chao_ECChaoSpawnPoints[3] = { 122, 26, 134 };
	Chao_ECChaoSpawnPoints[4] = { 81, 26, 172 };
	Chao_ECChaoSpawnPoints[5] = { -147, -0, 88 };
	Chao_ECChaoSpawnPoints[6] = { -105, 0, -50 };
	Chao_ECChaoSpawnPoints[7] = { -18, 0, -86 };
	Chao_ECChaoSpawnPoints[8] = { -27, 13, 166 };
	Chao_ECChaoSpawnPoints[9] = { 38, 13, -192 };
	Chao_ECChaoSpawnPoints[10] = { 62, 13, -251 };
	Chao_ECChaoSpawnPoints[11] = { 82, 13, -153 };
	Chao_ECChaoSpawnPoints[12] = { -88, 0.0f, -84 };
	Chao_ECChaoSpawnPoints[13] = { 109, 26, 8.92 };
	Chao_ECChaoSpawnPoints[14] = { -71, 0, -76 };
	Chao_ECChaoSpawnPoints[15] = { -28, -0, -67 };
	return;
}
