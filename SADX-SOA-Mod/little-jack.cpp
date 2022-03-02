#include "pch.h"

static ModelInfo* LittleJackMDL = nullptr;
static AnimationFile* LittleJackAnim = nullptr;

NJS_TEXNAME LittleJack_TexNames[7];
NJS_TEXLIST LittleJack_TexList = { arrayptrandlength(LittleJack_TexNames) };

struct PointToLook {
	NJS_VECTOR pos;
	int rotY;
};

PointToLook LJ_PointToLook[4] =
{
	{ {274, 167, -474}, -0x4000 },
	{ {-310, 167, -450}, 0x0},
	{ {-383, 167, 324}, 0x4000 },
	{ {241, 167, 461}, 0x8000 }
};


int LJSpeed = 0;
void TurnAndShift_LJ(EntityData1* data1, NJS_VECTOR* isleCenter) {

	float radius = 320.0f;
	data1->Position.x = njSin(LJSpeed) * radius;

	if (data1->CharID > 0)
		data1->Position.z = njCos(LJSpeed) * radius + 4.0;
	else
		data1->Position.z = njCos(LJSpeed) * radius;

	njAddVector(&data1->Position, isleCenter);
	data1->Position.y = isleCenter->y + 12.0;
}

void LJ_doRotation(EntityData1* data) {


	for (uint8_t i = 0; i < LengthOfArray(LJ_PointToLook); i++)
	{
		float pos = GetDistance(&LJ_PointToLook[i].pos, &data->Position);

		if (pos < 250) {

			if (!i)
				data->Rotation.y = LJ_PointToLook[i].rotY;

			if (data->Rotation.y > LJ_PointToLook[i].rotY)
				data->Rotation.y -= 200;

			if (data->Rotation.y < LJ_PointToLook[i].rotY)
				data->Rotation.y += 200;
		}
	}


	data->Rotation.x += 1024;
	data->Rotation.z += 1024;
}

void LJ_TurnAround(EntityData1* data1, NJS_VECTOR* isleCenter) {

	LJ_doRotation(data1);
	LJSpeed += 40;
	TurnAndShift_LJ(data1, isleCenter);
}

void LittleJack_Display(ObjectMaster* obj)
{

	EntityData1* data = obj->Data1;

	if (MissedFrames || data->Action != 2 || getTimeOfDay() != day)
		return;

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

const int timerCameo = 3760;
void LittleJack_Cameo(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;
	NJS_VECTOR islePos = { 0, 150, 0 };

	switch (data->Action)
	{
	case 0:

		data->Scale = { 4, 4, 4 };
		data->Position = { -262, 120, -269 };
		data->Object = LittleJackMDL->getmodel();
		obj->DisplaySub = LittleJack_Display;
		data->Action++;

		break;
	case 1:
	case 2:

		if (++data->InvulnerableTime == timerCameo)
		{
			data->InvulnerableTime = 0;
			data->Action++;
		}

		LJ_TurnAround(data, &islePos);
		break;
	case 3:
		data->InvulnerableTime = 0;
		data->Rotation = { 0, 0, 0 };
		data->Action++;
		LJSpeed = 0;
		break;
	case 4:
		data->Action = 1;
		return;
	}


	obj->DisplaySub(obj);
}

void LoadLittleJack_ModelAnim()
{
	LittleJackMDL = LoadChunkModel("LittleJackMDL");
	LittleJackAnim = LoadObjectAnim("LJAnim");
	LoadPVM("LJ-Tex", &LittleJack_TexList);
}