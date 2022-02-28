#pragma once

ModelInfo* LoadBasicModel(const char* name);

ModelInfo* LoadChunkModel(const char* name);

void LoadModelListFuncPtr(const char** names, int count, ModelInfo** mdls, ModelInfo* (*func)(const char*));

void FreeMDL(ModelInfo* pointer);

void LoadAnimation(AnimationFile** info, const char* name, const HelperFunctions& helperFunctions);

inline AnimationFile* LoadANM(const char* type, const char* name);

void LookAt(NJS_VECTOR* unit, Angle* outx, Angle* outy);
void LookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy);
void PlayerLookAt(NJS_VECTOR* from, NJS_VECTOR* to, Angle* outx, Angle* outy);
void LoadLandTableFile(LandTableInfo** info, const char* name, NJS_TEXLIST* texlist);
void DrawObject_Queue(NJS_OBJECT* object, QueuedModelFlagsB flags);
void AnimateUV_TexID(NJS_MODEL_SADX* model, int texID, Uint16 u, INT16 v);
void LoadFlashScreen(int color, int time);
void SetDrawingDistances(float level, float skybox);
void SetLevelFog(float layer, float distance, int color);
void FreeLandTableFile(LandTableInfo** info);