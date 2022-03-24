#pragma once

struct POS_XZ
{
	int x;
	int z;
};

struct LAND_ATTR_INDEX
{
	int nbIndex;
	POS_XZ pos[1024];
};

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

VoidFunc(LoadChaoCamCol, 0x72A750);
VoidFunc(Chao_CreateNormalCameraTask, 0x72A570);
VoidFunc(AlMsgFontInit, 0x724E60);
VoidFunc(sub_722500, 0x722500);
VoidFunc(SetChaoOnTheGarden, 0x715F20);
VoidFunc(SetChaoFlag, 0x7290B0);

DataPointer(int, LandTable_CollisionMeshCount, 0x03B36D3C);
FunctionPointer(void, RunChaoBehaviour, (ObjectMaster* obj, void* func), 0x71EF10);

DataArray(LAND_ATTR_INDEX, ChaoWayPoints, 0x3CA6EC8, 16);

FunctionPointer(void, GetActiveCollisions, (float x, float y, float z, float radius), 0x43ACD0);
FunctionPointer(int, GetGroundYPosition_CheckIntersection, (Mysterious64Bytes* a1, NJS_OBJECT* a2), 0x452B30);

FastcallFunctionPointer(void, DrawChunkModel_, (Sint32* a1, Sint16* a2), 0x7917F0);
void SetupWorldMatrix();
ObjectFunc(init_SetObj, 0x46C3D0);

DataPointer(int, dword_3C85EE4, 0x3C85EE4);
VoidFunc(Chao_CreatePlayerControlManager, 0x715350);

DataPointer(ObjectMaster*, tp_0, 0x03C49BC4);

struct DlgSndPrmType
{
	int OpenSnd;
	int CloseSnd;
	int MoveSnd;
	int DecideSnd;
	int CancelSnd;
};


struct __declspec(align(4)) PanelPrmType
{
	float OfsX;
	float OfsY;
	unsigned __int8 PvrIdx;
};

struct __declspec(align(4)) DDlgType
{
	float CntrX;
	float CntrY;
	float BaseZ;
	float SclX;
	float SclY;
	char Csr;
};


struct __declspec(align(4)) DialogPrmType
{
	int DlgStyle;
	void(__cdecl* EachDrawFnc)(DDlgType*);
	NJS_TEXLIST* PnlTlsPtr;
	PanelPrmType* PnlPrmPtr;
	DlgSndPrmType* DlgSndPrmPtr;
	unsigned int DlgColU;
	unsigned int DlgColD;
	float CntrX;
	float CntrY;
	float BaseZ;
	float SzX;
	float SzY;
	float MagX;
	float MagY;
	char CsrMax;
	char CsrCancel;
};

#pragma pack(push, 8)
struct MSGC
{
	unsigned __int8 kind;
	__int16 x;
	__int16 y;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int16 buf_width;
	unsigned __int16 buf_height;
	unsigned __int16 buf_width2;
	unsigned __int16 buf_height2;
	__int16 lx;
	__int16 ly;
	__int16 sx;
	__int16 sy;
	unsigned __int16 color;
	NJS_COLOR fc;
	NJS_COLOR bc;
	float scale;
	void* bitmap;
	unsigned int globalindex;
	NJS_TEXLIST texlist;
	NJS_TEXNAME texname;
	int msgc_flag;
};
#pragma pack(pop)



FunctionPointer(void, OpenDialog, (DialogPrmType* arg_0), 0x0432DB0);
FunctionPointer(ObjectMaster*, SetDialogTask, (), 0x432C60);
DataPointer(DialogPrmType*, DialogAskQuit, 0x007E06B0);
VoidFunc(DialogExec, 0x432DD0);
FunctionPointer(void, sub_40D490, (MSGC* a1), 0x40D490);
DataPointer(MSGC, msgc, 0x03B22E30);
FunctionPointer(void, MSG_Open, (MSGC* a1, __int16 a2, __int16 a3, int a4, int a5, int a6), 0x40E430);
VoidFunc(FreeQueueSound, 0x424460);