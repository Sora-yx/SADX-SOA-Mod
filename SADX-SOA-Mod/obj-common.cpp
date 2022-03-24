#include "pch.h"

static ModelInfo* saveMDL = nullptr;
static ModelInfo* saveMDLChild = nullptr;
static AnimationFile* saveAnim = nullptr;

CollisionData saveCol = { 0, CI_FORM_SPHERE, 0x77, 0, 0,{ 0, 10, 0 }, 8.0f, 5.0f, 0.0f };

void SaveObj_display(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	if (MissedFrames || isLeavingGarden)
		return;

	njSetTexture(CurrentLandTable->TexList);
	njPushMatrixEx();
	njTranslateEx(&data->Position);
	Direct3D_SetZFunc(1u);
	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();
	njRotateY(0, data->Rotation.y);
	njScaleV(0, &data->Scale);
	NJS_ACTION Action = { data->Object, saveAnim->getmotion() };
	njCnkAction_Queue(&Action, FrameCounterUnpaused % saveAnim->getmotion()->nbFrame, QueuedModelFlagsB_EnableZWrite);
	Direct3D_UnsetChunkModelRenderState();
	Direct3D_ResetZFunc();
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


void init_ObjsCommon()
{
	saveMDL = LoadChunkModel("SavePirate");
	saveMDLChild = LoadBasicModel("saveChild");
	saveAnim = LoadObjectAnim("SaveAnim");
}