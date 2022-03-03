#include "pch.h"

static ModelInfo* DojoDoor[2];
static ModelInfo* Door[2];
static ModelInfo* Laundry[2];
static ModelInfo* saveMDL = nullptr;
static ModelInfo* saveMDLChild = nullptr;
static ModelInfo* chest = nullptr;

CollisionData saveCol = { 0, CI_FORM_SPHERE, 0x77, 0, 0,{ 0, 10, 0 }, 8.0f, 5.0f, 0.0f };
CollisionData doorCol = { 0, CI_FORM_RECTANGLE, 0x77, 0, 0,{ -7, 10, -2 }, 8.0f, 10.0f, 2.0f };
CollisionData dojoCol = { 0, CI_FORM_RECTANGLE, 0x77, 0, 0,{ -7, 10, -2 }, 15.0f, 10.0f, 2.0f, 0x0, 0x0, 0 };

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


void Dojo_Display(ObjectMaster* obj)
{
	if (MissedFrames)
		return;

	EntityData1* data = obj->Data1;

	njSetTexture(CurrentLandTable->TexList);
	njPushMatrix(0);
	njRotateXYZ(0, data->Rotation.x, data->Rotation.y, data->Rotation.z);
	njTranslateV(0, &data->Position);
	dsDrawObject(data->Object);
	njPopMatrix(1u);
}

void Doji_Child(ObjectMaster* obj)
{
	if (ClipSetObject(obj))
		return;

	EntityData1* data = obj->Data1;
	float result = data->field_A;
	float diff = 30;

	switch (data->Action)
	{
	case 0:
		if (IsPlayerInsideSphere(&data->Position, 20))
		{
			if (Controllers[0].PressedButtons & Buttons_Y)
			{
				ForcePlayerAction(0, 12);
				obj->Parent->Data1->Action = 1;
				data->Action++;
			}
		}
		break;
	case 1:

		if (data->Position.y > result)
			data->Position.y--;
		else {
			ForcePlayerAction(0, 24);
			data->Action++;
		}
		break;
	case 2:
		if (++data->InvulnerableTime == 300)
		{
			data->Position.y = result + diff;
			data->Action = 0;
			data->InvulnerableTime = 0;
		}
		break;
	}

	obj->DisplaySub(obj);
}

void Dojo_Main(ObjectMaster* obj)
{
	if (ClipSetObject(obj))
		return;

	EntityData1* data = obj->Data1;
	float result = data->field_A;
	float diff = 30;

	switch (data->Action)
	{
	
	case 0:
		if (IsPlayerInsideSphere(&data->Position, 20))
		{
			if (Controllers[0].PressedButtons & Buttons_Y)
			{
				ForcePlayerAction(0, 12);
				obj->Child->Data1->Action = 1;
				data->Action++;
			}
		}
		break;
	case 1:

		if (data->Position.y > result)
			data->Position.y--;
		else {
			ForcePlayerAction(0, 24);
			data->Action++;
		}


		break;
	case 2:
		if (++data->InvulnerableTime == 300)
		{
			data->Position.y = result + diff;
			data->Action = 0;
			data->InvulnerableTime = 0;
		}
		break;

	}

	if (!data->Action)
		AddToCollisionList(obj->Data1);

	RunObjectChildren(obj);
	obj->DisplaySub(obj);
}


void LoadDojoDoor(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	obj->Data1->Object = DojoDoor[1]->getmodel();

	data->field_A = data->Position.y - 30;

	Collision_Init(obj, &dojoCol, 1, 4u);

	obj->MainSub = Dojo_Main;
	obj->DisplaySub = Dojo_Display;
	ObjectMaster* child = LoadChildObject(LoadObj_Data1, Doji_Child, obj);
	child->Data1->Object = DojoDoor[0]->getmodel();
	child->Data1->Position.z -= 32;
	child->DisplaySub = Dojo_Display;
	child->Data1->field_A = data->Position.y - 30;
}

void Door_Display(ObjectMaster* obj)
{
	if (MissedFrames)
		return;

	EntityData1* data = obj->Data1;

	njSetTexture(CurrentLandTable->TexList);
	njPushMatrix(0);
	njRotateXYZ(0, data->Rotation.x, data->Rotation.y, data->Rotation.z);
	njTranslateV(0, &data->Position);
	dsDrawObject(data->Object);
	njPopMatrix(1u);
}

void Door_Main(ObjectMaster* obj)
{
	if (ClipSetObject(obj))
		return;

	EntityData1* data = obj->Data1;
	float result = data->field_A;
	float diff = 25;

	switch (data->Action)
	{
	case 0:
		if (IsPlayerInsideSphere(&data->Position, 20))
		{
			if (Controllers[0].PressedButtons & Buttons_Y)
			{
				ForcePlayerAction(0, 12);
				data->Action++;
			}
		}
		break;
	case 1:

		if (data->Position.y > result)
			data->Position.y--;
		else {
			ForcePlayerAction(0, 24);
			data->Action++;
		}


		break;
	case 2:
		if (++data->InvulnerableTime == 300)
		{
			data->Position.y = result + diff;
			data->Action = 0;
			data->InvulnerableTime = 0;
		}
		break;

	}

	if (!data->Action)
		AddToCollisionList(obj->Data1);

	obj->DisplaySub(obj);
}

void LoadDoor(ObjectMaster* obj)
{

	if (ClipSetObject(obj))
		return;

	EntityData1* data = obj->Data1;


	if (data->Scale.z == 1)
		data->Object = Door[0]->getmodel();
	else
		data->Object = Door[1]->getmodel();

	data->field_A = data->Position.y - 25;

	Collision_Init(obj, &doorCol, 1, 4u);
	obj->MainSub = Door_Main;
	obj->DisplaySub = Door_Display;
}

void SaveObj_display(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	if (MissedFrames)
		return;

	njSetTexture(CurrentLandTable->TexList);
	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	njRotateY(0, data->Rotation.y);
	DrawObject(data->Object);
	njPopMatrix(1u);


	njPushMatrix(0);
	njTranslateV(0, &data->Position);
	DrawObject(saveMDLChild->getmodel());
	njPopMatrix(1u);
}

void SaveObj_Main(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	if (ClipSetObject(obj))
		return;

	switch (data->Action)
	{
	case 0:
		obj->DisplaySub = SaveObj_display;
		data->Action++;
		break;
	case 1:
		if (IsPlayerInsideSphere(&data->Position, 20))
		{
			if (Controllers[0].PressedButtons & Buttons_Y)
			{
				data->Action++;
				ForcePlayerAction(0, 12);
				SetDebugFontSize(20);
				//SaveSave();
			}
		}
		break;
	case 2:
		DisplayDebugStringFormatted(NJM_LOCATION(2, 2), "SAVED!");
		if (++data->InvulnerableTime == 70)
		{
			ForcePlayerAction(0, 24);
			SetDebugFontSize(16);
			data->Action = 1;
			data->InvulnerableTime = 0;
		}
		break;
	}

	data->Rotation.y += 840;

	AnimateUV_TexID(saveMDLChild->getmodel()->basicdxmodel, 160, 0, -2);
	obj->DisplaySub(obj);
	AddToCollisionList(data);

}

void LoadSave_Obj(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;


	obj->Data1->Object = saveMDL->getmodel();
	obj->MainSub = SaveObj_Main;
	Collision_Init(obj, &saveCol, 1, 4u);
}


void LoadLaundry(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	if (ClipSetObject(obj))
		return;

	if (data->Scale.z == 1)
		obj->Data1->Object = Laundry[0]->getmodel();
	else
		obj->Data1->Object = Laundry[1]->getmodel();

	obj->MainSub = Door_Main;
	obj->DisplaySub = obj_DisplaySubRegular;
}

ObjectListEntry EggCarrierChaoGardenObjectList_list[] = {
	{LoadObj_Data1, 2, 1, 80000, 0, LoadDoor, "Door"},
	{LoadObj_Data1, 2, 1, 80000, 0, LoadDojoDoor, "DojoDoor"},
	{LoadObj_Data1, 3, 1, 80000, 0, nullptr, "Laundry"},
	{LoadObj_Data1, 3, 1, 80000, 0, LoadLaundry, "Laundry2"},
	{LoadObj_Data1, 3, 1, 80000, 0, nullptr, "chest"},
	{LoadObj_Data1, 3, 1, 80000, 0, nullptr, "tree"},
	{LoadObj_Data1, 3, 1, 80000, 0, nullptr, "weed"},
	{LoadObj_Data1, 2, 1, 80000, 0, nullptr, "mill"},
	{LoadObj_Data1, 2, 1, 80000, 0, nullptr, "hanky"},
	{LoadObj_Data1, 2, 1, 80000, 0, LoadSave_Obj, "save"},

};

ObjectList EggCarrierChaoGardenObjectList = { arraylengthandptrT(EggCarrierChaoGardenObjectList_list, int) };

void LoadOBJModels()
{
	DojoDoor[0] = LoadBasicModel("DoorDojo");
	DojoDoor[1] = LoadBasicModel("DoorDojo2");
	Door[0] = LoadBasicModel("Door");
	Door[1] = LoadBasicModel("Door2");
	Laundry[0] = LoadBasicModel("Laundry");
	Laundry[1] = LoadBasicModel("Laundry2");
	saveMDL = LoadBasicModel("Save");
	saveMDLChild = LoadBasicModel("saveChild");
	return;
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

void init_ObjectsHack()
{
	WriteJump((void*)0x536C20, longladder_Display_r);

	//init new objlist
	WriteData((ObjectList**)0x974FF8, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x974FFC, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x975000, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x975004, &EggCarrierChaoGardenObjectList);
	WriteData((ObjectList**)0x975008, &EggCarrierChaoGardenObjectList);
}
