#include "pch.h"

static ModelInfo* LittleJackMDL = nullptr;
static AnimationFile* LittleJackAnim = nullptr;

NJS_TEXNAME LittleJack_TexNames[7];
NJS_TEXLIST LittleJack_TexList = { arrayptrandlength(LittleJack_TexNames) };

int LJSpeed = 0;
void TurnAndShift_LJ(EntityData1* data1, NJS_VECTOR* isleCenter) {

	float radius = 270.0f;
	data1->Position.x = njSin(LJSpeed) * radius;

	if (data1->CharID > 0)
		data1->Position.z = njCos(LJSpeed) * radius + 4.0;
	else
		data1->Position.z = njCos(LJSpeed) * radius;

	njAddVector(&data1->Position, isleCenter);
	data1->Position.y = isleCenter->y + 12.0;
}

void LJ_doRotation(EntityData1* data) {

	data->Rotation.y = BAMS_SubWrap(data->Rotation.y, 0x8000, 1024);
	data->Rotation.x += 1024;
	data->Rotation.z += 1024;
}

void LJ_TurnAround(EntityData1* data1, NJS_VECTOR* isleCenter) {

	LJ_doRotation(data1);

	LJSpeed += 60;
	TurnAndShift_LJ(data1, isleCenter);

}

void LittleJack_Display(ObjectMaster* obj)
{

	if (MissedFrames)
		return;

	EntityData1* data = obj->Data1;

	njSetTexture(&LittleJack_TexList);
	njPushMatrixEx();
	njTranslateEx(&data->Position);
	Direct3D_SetZFunc(1u);
	SetupWorldMatrix();
	Direct3D_SetChunkModelRenderState();
	njRotateY(0, data->Rotation.y);
	njScaleV(0, &data->Scale);
	NJS_ACTION Action = { data->Object, LittleJackAnim->getmotion() };
	njCnkAction_Queue(&Action, FrameCounterUnpaused % LittleJackAnim->getmotion()->nbFrame, QueuedModelFlagsB_EnableZWrite);
	Direct3D_UnsetChunkModelRenderState();
	Direct3D_ResetZFunc();
	njPopMatrix(1u);

}

void LittleJack_Cameo(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;
	NJS_VECTOR islePos = { 0, 150, 0 };

	switch (data->Action)
	{
	case 0:
		data->Scale = { 4, 4, 4 };
		data->Position = { -262, 95, -269 };
		data->Object = LittleJackMDL->getmodel();
		obj->DisplaySub = LittleJack_Display;
		data->Action++;
		break;
	case 1:
		LJ_TurnAround(data, &islePos);
		break;
	}

	obj->DisplaySub(obj);
}

void LoadLittleJack_ModelAnim()
{
	LittleJackMDL = LoadChunkModel("LittleJackMDL");
	LittleJackAnim = LoadObjectAnim("LJAnim");
	LoadPVM("LJ-Tex", &LittleJack_TexList);
}