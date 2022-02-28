#include "pch.h"
#include "sadx-util.h"

NJS_TEXNAME PirateIsle_TexNames[161];
NJS_TEXLIST PirateIsle_TexList = { arrayptrandlength(PirateIsle_TexNames) };

LandTableInfo* PirateIsleGeo = nullptr;
Trampoline* LoadECGarden_t = nullptr;

static ModelInfo* PirateSkyBox[3];

NJS_TEXNAME PirateBG_TexNames[7];
NJS_TEXLIST PirateBG_TexList = { arrayptrandlength(PirateBG_TexNames) };

NJS_TEXNAME PirateBGEvening_TexNames[8];
NJS_TEXLIST PirateBG2_TexList = { arrayptrandlength(PirateBGEvening_TexNames) };

NJS_TEXNAME PirateBGNight_TexNames[8];
NJS_TEXLIST PirateBG3_TexList = { arrayptrandlength(PirateBGNight_TexNames) };

void Draw_SkyBoxDay(EntityData1* data)
{
	if (getTimeOfDay() != day)
		return;

	if (!data->Object)
		return;

	NJS_OBJECT* cloud[3] = { data->Object->child->sibling->sibling->sibling->sibling->sibling };
	cloud[1] = cloud[0]->sibling;
	cloud[2] = cloud[1]->sibling;

	njSetTexture(&PirateBG_TexList);

	AnimateUV_TexID(cloud[0]->basicdxmodel, 4, 0, 1);
	AnimateUV_TexID(cloud[1]->basicdxmodel, 6, 0, 1);
	AnimateUV_TexID(cloud[2]->basicdxmodel, 6, 0, 1);

	DrawQueueDepthBias = -99999;
	njSetTexture(&PirateBG_TexList);
	njPushMatrix(_nj_current_matrix_ptr_);
	njTranslate(_nj_current_matrix_ptr_, 0, 0, 0);
	late_DrawObject(data->Object, QueuedModelFlagsB_EnableZWrite);
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
	njTranslate(_nj_current_matrix_ptr_, 0, 0, 0);
	late_DrawObject(skybox, QueuedModelFlagsB_EnableZWrite);
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
	njTranslate(_nj_current_matrix_ptr_, 0, 0, 0);
	late_DrawObject(skybox, QueuedModelFlagsB_EnableZWrite);
	njPopMatrix(1);

	AnimateUV_TexID(skybox->child->sibling->sibling->sibling->sibling->sibling->basicdxmodel, 6, 1, 0);
	AnimateUV_TexID(skybox->child->sibling->sibling->basicdxmodel, 3, 1, 0);
}

void PirateIsle_Display(ObjectMaster* obj)
{
	if (MissedFrames)
		return;

	EntityData1* data = obj->Data1;

	Direct3D_SetNearFarPlanes(0, SkyboxDrawDistance.Maximum);

	Draw_SkyBoxDay(data);
	Draw_SkyBoxEvening();
	Draw_SkyBoxNight();

	Direct3D_SetNearFarPlanes(0, LevelDrawDistance.Maximum);
}

NJS_VECTOR startpos = { 84, 100, 97.64 };

void PlayerStartPos()
{
	char posx = 5;

	for (uint8_t i = 0; i < 8; i++) {

		if (!EntityData1Ptrs[i])
			continue;

		EntityData1Ptrs[i]->Position = startpos;
		EntityData1Ptrs[i]->Rotation.y = 0x8000;

		if (i)
			EntityData1Ptrs[i]->Position.x += posx;

		posx += 5;
	}
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

void PirateIsle_Garden(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	switch (data->Action)
	{
	case 0:
		LoadChildObject(LoadObj_Data1, Garden_TimeOfDay, obj);
		data->Object = PirateSkyBox[day]->getmodel();
		obj->DisplaySub = PirateIsle_Display;
		data->Action++;
		break;
	case 1:
		if (++data->Index == 10) {
			PlayerStartPos();
			data->Action++;
		}
		break;
	case 2:
		PlayerANTIOob();
		break;
	}

	obj->DisplaySub(obj);
	RunObjectChildren(obj);
}

DataPointer(int, LandTable_CollisionMeshCount, 0x03B36D3C);

void Load_PirateMDL()
{

	PirateSkyBox[day] = LoadBasicModel("PirateIsle-SkyBox");
	PirateSkyBox[evening] = LoadBasicModel("PirateIsle-SkyBox2");
	PirateSkyBox[night] = LoadBasicModel("PirateIsle-SkyBox3");

	LoadPVM("PirateIsle-BGTex", &PirateBG_TexList);
	LoadPVM("PirateIsle-BGTex2", &PirateBG2_TexList);
	LoadPVM("PirateIsle-BGTex3", &PirateBG3_TexList);

}


void __cdecl ChaoStgGarden01EC_Load_r(ObjectMaster* parent)
{
	NJS_VECTOR position; // [esp+4h] [ebp-Ch] BYREF

	SetGlobalPoint2Col_Colors(0xFF000000, 0xFF000000, 0xFF000000);
	LevelFogData.Toggle = 0;
	LoadChaoCamCol();
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
	LoadChaoTransporter(parent, SADXChaoStage_MysticRuins, -20.0, 13.44, -214.0, 0x100);
	LoadChaoTransporter(parent, SADXChaoStage_StationSquare, 5.0, 13.44, -220.51, 0x100);
	LoadChaoTransporter(parent, 8, 487.0, 161.5300002, -109.0, 0x4000);
	LoadHeldChao();
	LoadHeldItem();
	ChaoDX_Message_PlayerAction_Load();
	SetChaoFlag();
	InitializeSoundManager();
	MusicList[MusicIDs_chao].Name = "Sky_Pirate_Isle";
	PlayMusic(MusicIDs_chao);
	parent->MainSub = PirateIsle_Garden;
	parent->DeleteSub = ChaoStgGarden01EC_Delete;
	LevelDrawDistance.Minimum = -1.0;
	LevelDrawDistance.Maximum = -10000.0;
}

void Load_PirateIsle()
{

	auto original = reinterpret_cast<decltype(Load_PirateIsle)*>(LoadECGarden_t->Target());
	original();

	auto pointer = (EntityData1*)0x3B36D30;

	LoadLandTableFile(&PirateIsleGeo, "system\\PirateIsle.sa1lvl", &PirateIsle_TexList);
	GeoLists[LevelIDs_ECGarden * 8] = PirateIsleGeo->getlandtable();
	pointer->Action = 2;
	DynamicCOLCount_B_Again = 0;
	LandTable_CollisionMeshCount = 0;

	Load_PirateMDL();
	//LoadObject(LoadObj_Data1, 2, PirateIsle_Garden);

	return;
}

void SetNumberMaxOfTree(uint8_t number)
{
	WriteData<1>((int*)0x717bc6, number);
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
	LoadECGarden_t = new Trampoline((int)LoadECGarden, (int)LoadECGarden + 0x5, Load_PirateIsle);
	WriteData<5>((void*)0x423795, 0x90u); //Prevent DC Mod to load Chao Garden stuff.
	WriteJump(ChaoStgGarden01EC_Load, ChaoStgGarden01EC_Load_r);
	SetNewTreePos();

	Chao_ECChaoSpawnPoints[0] = { -92, -0, 124};
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
	Chao_ECChaoSpawnPoints[12] = { -88, 26, -84 };
	Chao_ECChaoSpawnPoints[13] = { 109, 26, 8.92 };
	Chao_ECChaoSpawnPoints[14] = { -71, 0, -76 };
	Chao_ECChaoSpawnPoints[15] = { -28, -0, -67 };
}