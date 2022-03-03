#include "pch.h"

static ModelInfo* DojoDoor[2];
static ModelInfo* Door = nullptr;



void LongLadder_main(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	if (!data->Action)
	{
		data->Position = { -9, 57, -63 };
		data->Rotation.y = 0x8000;
		obj->MainSub = LongLadder_Main;
	}
}

void __cdecl longladder_Display_r(ObjectMaster* a1)
{

	EntityData1* data; // esi
	Angle v2; // eax
	int v3; // esi
	float YDist; // [esp+0h] [ebp-Ch]

	if (IsLevelChaoGarden() || MissedFrames)
		return;

	data = a1->Data1;
	SetTextureToLevelObj();
	njPushMatrix(0);
	YDist = data->Position.y - 60.0;
	njTranslate(0, data->Position.x, YDist, data->Position.z);
	v2 = data->Rotation.y;
	if (v2)
	{
		njRotateY(0, (unsigned __int16)v2);
	}
	v3 = 3;
	do
	{
		DrawModel((NJS_MODEL_SADX*)ADV02_OBJECTS[83]->model);
		njTranslate(0, 0.0, 99.0, 0.0);
		--v3;
	} while (v3);
	njPopMatrix(1u);
}

void Reset_LadderHack()
{
	WriteData<1>((int*)0x536e01, 0x00);
	WriteData<1>((int*)0x536e02, 0x6A);

	WriteData<1>((int*)0x536d7f, 79);
	WriteData<1>((int*)0x536dfb, 79);

	WriteData<1>((int*)0x536d6a, 0x2c);
	WriteData<1>((int*)0x536d6b, 0x6);
}

const int loopMaxPlayerDist = 115;

//series of hack to make the player able to climb higher ladder than vanilla
void Set_LadderHack()
{
	//increase ladder dist max from 234 to 345.
	WriteData<1>((int*)0x536e01, 0x80);
	WriteData<1>((int*)0x536e02, 0xAC);

	//increase the number of loop to calc player pos shit from 79 to 115
	WriteData<1>((int*)0x536d7f, loopMaxPlayerDist);
	WriteData<1>((int*)0x536dfb, loopMaxPlayerDist);

	//increase allocated memory from 1500 to 2500, definitely no need that much, but I wanted to be safe.
	WriteData<1>((int*)0x536d6a, 0x4c);
	WriteData<1>((int*)0x536d6b, 0x9);
}

void LoadPirateIsle_Objects()
{
	Set_LadderHack();
	LoadObject(LoadObj_Data1, 2, LongLadder_main);
	LoadObject(LoadObj_Data1, 2, LittleJack_Cameo);
	return;
}

void LoadDojoDoor(ObjectMaster* obj)
{

}

void obj_DisplaySubRegular(ObjectMaster* obj)
{
	if (MissedFrames)
		return;

	EntityData1* data = obj->Data1;

	njSetTexture(CurrentLandTable->TexList);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	dsDrawObject(data->Object);
	njPopMatrix(1u);

}

void obj_MainGlobal(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	if (!data->Action)
	{
		obj->DisplaySub = obj_DisplaySubRegular;
		data->Action++;
	}

	obj->DisplaySub(obj);
}

void LoadDoor(ObjectMaster* obj)
{
	obj->Data1->Object = Door->getmodel();
	obj->MainSub = obj_MainGlobal;
}

ObjectListEntry EggCarrierChaoGardenObjectList_list[] = {
	{2, 2, 1, 10000, 0, LoadDoor, "Door"},
	{2, 2, 1, 10000, 0, LoadDojoDoor, "DojoDoor"},
	{2, 3, 1, 10000, 0, nullptr, "Laundry"},
	{2, 3, 1, 10000, 0, nullptr, "Laundry2"},
	{2, 3, 1, 10000, 0, nullptr, "chest"},
	{2, 3, 1, 10000, 0, nullptr, "tree"},
	{2, 3, 1, 10000, 0, nullptr, "weed"},
	{2, 2, 1, 10000, 0, nullptr, "mill"},
	{2, 2, 1, 10000, 0, nullptr, "hanky"},
	{2, 2, 0, 0, 0, nullptr, "save"},

};

ObjectList EggCarrierChaoGardenObjectList = { arraylengthandptrT(EggCarrierChaoGardenObjectList_list, int) };

void LoadOBJModels()
{
	DojoDoor[0] = LoadBasicModel("DoorDojo");
	DojoDoor[1] = LoadBasicModel("DoorDojo2");
	Door = LoadBasicModel("Door");
	return;
}

void init_Objects()
{ 
	WriteJump((void*)0x536C20, longladder_Display_r);

	WriteCall((void*)0x42234F, PrintDebug);
	//init new objlist
	WriteData((ObjectList**)0x974FF8, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x974FFC, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x975000, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x975004, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x975008, &EggCarrierChaoGardenObjectList);

}
