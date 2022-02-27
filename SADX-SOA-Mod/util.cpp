#include "pch.h"

ModelInfo* LoadBasicModel(const char* name) {
	PrintDebug("Loading basic model: %s... ", name);

	std::string fullPath = "system\\models\\";
	fullPath = fullPath + name + ".sa1mdl";

	ModelInfo* mdl = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	if (mdl->getformat() != ModelFormat_Basic) {
		PrintDebug("Failed!\n");
		delete mdl;
		return nullptr;
	}

	PrintDebug("Done.\n");
	return mdl;
}

ModelInfo* LoadChunkModel(const char* name) {
	PrintDebug("[KH Mod] Loading chunk model: %s... ", name);

	std::string fullPath = "system\\models\\";
	fullPath = fullPath + name + ".sa2mdl";

	ModelInfo* mdl = new ModelInfo(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	if (mdl->getformat() != ModelFormat_Chunk) {
		PrintDebug("Failed!\n");
		delete mdl;
		return nullptr;
	}

	PrintDebug("Done.\n");
	return mdl;
}

void LoadModelListFuncPtr(const char** names, int count, ModelInfo** mdls, ModelInfo* (*func)(const char*)) {
	for (int i = 0; i < count; ++i) {
		mdls[i] = func(names[i]);
	}
}

void FreeMDL(ModelInfo* pointer) {
	if (pointer) {
		PrintDebug("[SHM] Freeing model: %s... \n", pointer->getdescription().c_str());
		delete(pointer);
		pointer = nullptr;
	}
}

// Animation file functions

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions) {
	std::string fullPath = "system\\anims\\";
	fullPath = fullPath + name + ".saanim";

	AnimationFile* anm = new AnimationFile(helperFunctions.GetReplaceablePath(fullPath.c_str()));

	if (anm->getmodelcount() == 0) {
		delete anm;
		*info = nullptr;
	}
	else {
		*info = anm;
	}
};

inline AnimationFile* LoadANM(const char* type, const char* name) {
	std::string fullPath = "system\\";
	fullPath = fullPath + type + "\\" + name + ".saanim";

	AnimationFile* temp = new AnimationFile(HelperFunctionsGlobal.GetReplaceablePath(fullPath.c_str()));

	if (temp->getmodelcount()) {
		PrintDebug("Done.\n");
		return temp;
	}
	else {
		PrintDebug("Failed.\n");
		delete temp;
		return nullptr;
	}
}


void LookAt(NJS_VECTOR* unit, Angle* outx, Angle* outy) {
	if (outy) {
		*outy = static_cast<Angle>(atan2f(unit->x, unit->z) * 65536.0f * 0.1591549762031479f);
	}

	if (outx) {
		Float len = 1.0f / squareroot(unit->z * unit->z + unit->x * unit->x + unit->y * unit->y);

		*outx = static_cast<Angle>((acos(len * 3.3499999f) * 65536.0f * 0.1591549762031479f)
			- (acos(-(len * unit->y)) * 65536.0f * 0.1591549762031479f));
	}
}

void LookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy) {
	if (!from || !to)
		return;

	NJS_VECTOR unit = *to;

	njSubVector(&unit, from);

	if (outy) {
		*outy = static_cast<Angle>(atan2f(unit.x, unit.z) * 65536.0f * 0.1591549762031479f);
	}

	if (outx) {
		if (from->y == to->y) {
			*outx = 0;
		}
		else {
			Float len = 1.0f / squareroot(unit.z * unit.z + unit.x * unit.x + unit.y * unit.y);

			*outx = static_cast<Angle>((acos(len * 3.3499999f) * 65536.0f * 0.1591549762031479f)
				- (acos(-(len * unit.y)) * 65536.0f * 0.1591549762031479f));
		}
	}
}

void PlayerLookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy) {
	LookAt(from, to, outx, outy);

	if (outy) {
		*outy = -(*outy) + 0x4000;
	}
}

void LoadLandTableFile(LandTableInfo** info, const char* name, NJS_TEXLIST* texlist) {
	PrintDebug("[SOA Mod] Loading geometry file: %s... ", name);

	LandTableInfo* lnd = new LandTableInfo(HelperFunctionsGlobal.GetReplaceablePath(name));

	if (lnd->getlandtable() == nullptr) {
		PrintDebug("Failed!\n");
		delete lnd;
		*info = nullptr;
	}
	else {
		PrintDebug("Done.\n");
		lnd->getlandtable()->TexList = texlist;
		*info = lnd;
	}
}

#define Pos3toPos(data) data[0], data[1], data[2]

void DrawObject_Queue(NJS_OBJECT* object, QueuedModelFlagsB flags) {
	njPushMatrixEx();
	njTranslate(nullptr, Pos3toPos(object->pos));
	njRotateXYZ(nullptr, object->ang[0], object->ang[1], object->ang[2]);
	DrawModel_Queue(object->basicdxmodel, QueuedModelFlagsB_EnableZWrite);
	njPopMatrixEx();
}

int GetUVCount(NJS_MESHSET_SADX* meshset) {
	switch (meshset->type_matId)
	{
	case NJD_MESHSET_3:
		return meshset->nbMesh * 3;
	case NJD_MESHSET_4:
		return meshset->nbMesh * 4;
	case NJD_MESHSET_N:
	case NJD_MESHSET_TRIMESH:
		int indices = 0;
		int currentindex = 0;
		int currentmesh = 0;

		while (currentmesh < meshset->nbMesh) {
			int numvrt = meshset->meshes[currentindex] & 0x7FFF;
			indices += numvrt;
			currentindex += numvrt + 1;
			currentmesh++;
		}

		return indices;
	}

	return 0;
}

void AnimateUV_TexID(NJS_MODEL_SADX* model, int texID, Uint16 u, INT16 v) {

	if (IsGamePaused())
	{
		return;
	}

	if (model)
	{
		for (auto i = 0; i < model->nbMeshset; ++i)
		{
			auto mesh = &model->meshsets[i];
			auto mat = &model->mats[mesh->type_matId & ~NJD_MESHSET_MASK];

			if (mesh && mat && mat->attr_texId == texID)
			{
				u %= 0x1000;
				v %= 0x1000;

				for (auto j = 0; j < GetUVCount(mesh); ++j)
				{
					mesh->vertuv[j].u += u;
					mesh->vertuv[j].v += v;
				}
			}
		}
	}
}
