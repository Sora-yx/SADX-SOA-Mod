#include "pch.h"

Trampoline* Chao_Main_t;
Trampoline* Chao_DetectWater_t;
extern NJS_VECTOR startpos;

enum StatusChao : __int16 {
	StatusChao_Held = 0x1000,

	// Custom ones:
	StatusChao_FlyPlayer = 0x2000,
	StatusChao_Attacked = 0x4000
};

enum ChaoState : __int16 {
	ChaoState_Unk1 = 0x1,
	ChaoState_Water = 0x4,
};

enum ChaoLeashModes {
	ChaoLeashMode_Disabled,
	ChaoLeashMode_Free,
	ChaoLeashMode_Held,
	ChaoLeashMode_Fly
};

struct ChaoLeash {
	ChaoLeashModes mode;
	unsigned int id;
};

struct ChaoData2_ {
	char gap0[4];
	NJS_VECTOR field_4;
	NJS_VECTOR field_10;
	char gap1C[16];
	int field_2C;
	char gap30[4];
	float field_34;
	char gap38[4];
	float field_3C;
	__int16 field_40;
	__int16 field_42;
	char gap44[4];
	float field_48;
	char gap_4C[20];
	NJS_VECTOR OtherPosition;
	char gap_6C[64];
	float field_AC;
	char field_B0[4];
	float field_B4;
	float field_B8;
	float field_BC;
	float field_C0;
	float field_C4;
	float field_C8;
	float field_CC;
	float field_D0;
	float idk1;
	float idk2;
	float WaterHeight;
	char gap_d4[309];
	char field_26B;
};

FunctionPointer(void, GetActiveCollisions, (float x, float y, float z, float radius), 0x43ACD0);
FunctionPointer(int, GetGroundYPosition_CheckIntersection, (Mysterious64Bytes* a1, NJS_OBJECT* a2), 0x452B30);


bool IsChaoInWater(ChaoData1* chaodata1, ChaoData2_* chaodata2)
{
	Mysterious64Bytes colthing;
	colthing.Position = { chaodata1->entity.Position.x, chaodata1->entity.Position.y + 1.0f, chaodata1->entity.Position.z };

	// Get all collisions within a radius
	GetActiveCollisions(colthing.Position.x, colthing.Position.y, colthing.Position.z, 200.0f);
	float max = 10000000.0f;
	int top_flags = 0;

	// For all found collisions
	if (DynamicCOLCount_B > 0)
	{
		for (int i = 0; i < DynamicCOLCount_B; ++i)
		{
			DynamicCOL* col = &DynamicCOLArray_LandTable[i];

			// Check if the collision is above, then only save if the lowest ever found
			if (GetGroundYPosition_CheckIntersection(&colthing, col->Model))
			{
				if (colthing.struct_v3_b.SomeFlag == 1
					&& colthing.Position.y < colthing.struct_v3_b.Distance
					&& colthing.struct_v3_b.Distance < max)
				{
					max = colthing.struct_v3_b.Distance;
					top_flags = col->Flags;
				}
			}
		}
	}

	// If the collision directly above is water
	if (top_flags & (0x400002 | ColFlags_Water) && chaodata1->entity.Position.y - 1.0f < max)
	{
		chaodata2->WaterHeight = max;
		return true;
	}

	return false;
}

FunctionPointer(void, RunChaoBehaviour, (ObjectMaster* obj, void* func), 0x71EF10);


BOOL __cdecl Chao_DetectWater_r(ObjectMaster* obj)
{
	if (CurrentLevel != LevelIDs_ECGarden)
	{
		FunctionPointer(BOOL, origin, (ObjectMaster* obj), Chao_DetectWater_t->Target());
		return origin(obj);
	}

	auto data1 = (ChaoData1*)obj->Data1;
	auto data2 = (ChaoData2_*)obj->Data2;

	auto& state = data1->unknown_d[0].field_0;

	if (data1->entity.Status & StatusChao_Held)
	{
		state &= ~ChaoState_Unk1;
	}
	else
	{
		if (IsChaoInWater(data1, data2))
		{
			if (!(state & ChaoState_Unk1))
			{
				state |= ChaoState_Unk1;
				RunChaoBehaviour(obj, (void*)0x73C110);
			}

			if (data2->field_4.y < 0.0f)
			{
				data2->field_4.y = data2->field_4.y * 0.1f;
			}

			state |= ChaoState_Water;
			return TRUE;
		}
		else
		{
			state &= ~(ChaoState_Water | ChaoState_Unk1);
		}
	}

	return FALSE;
}

void Chao_Main_R(ObjectMaster* obj) {
	EntityData1* data = obj->Data1;

	if (CurrentLevel == LevelIDs_ECGarden)
	{
		if (obj->Data1)
		{
			if (obj->Data1->Position.y <= -5)
			{
				obj->Data1->Position = startpos;
				obj->Data1->Position.y = 0;
			}
		}
	}

	ObjectFunc(origin, Chao_Main_t->Target());
	origin(obj);
}

void init_ChaoFixes()
{
	 Chao_DetectWater_t = new Trampoline(0x73C200, 0x73C207, Chao_DetectWater_r);
	//Chao_Main_t = new Trampoline((int)Chao_Main, (int)Chao_Main + 0x6, Chao_Main_R);
}