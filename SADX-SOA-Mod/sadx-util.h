#pragma once

VoidFunc(LoadChaoCamCol, 0x72A750);
VoidFunc(Chao_CreateNormalCameraTask, 0x72A570);
VoidFunc(AlMsgFontInit, 0x724E60);
VoidFunc(sub_722500, 0x722500);
VoidFunc(SetChaoOnTheGarden, 0x715F20);
VoidFunc(SetChaoFlag, 0x7290B0);

DataPointer(int, LandTable_CollisionMeshCount, 0x03B36D3C);
FunctionPointer(void, RunChaoBehaviour, (ObjectMaster* obj, void* func), 0x71EF10);



enum StatusChao : __int16 {
	StatusChao_Held = 0x1000,
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

FastcallFunctionPointer(void, DrawChunkModel_, (Sint32* a1, Sint16* a2), 0x7917F0);
void SetupWorldMatrix();
ObjectFunc(init_SetObj, 0x46C3D0);