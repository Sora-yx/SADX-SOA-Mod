#pragma once

enum class DiffuseIDs : int32_t {
	None = -1,
	Objects,
	Level,
	Character,
	Custom1,
	Custom2,
	Custom3,
	Custom4,
	Custom5
};

enum class SpecularIDs : int32_t {
	None = -1,
	AltObject,
	Object,
	Character,
	AltCharacter,
	Custom1,
	Custom2,
	Custom3,
	Level
};

void LightingAPI_InitEnd();
void SetCurrentPalettes(DiffuseIDs diffuse, SpecularIDs specular);
void ResetPalettes();
void RegisterLandPalette(NJS_OBJECT* object);
void UnregisterLandPalette(NJS_OBJECT* object);
void UnsetPaletteBlendMode();
void SetPaletteBlendMode();
void SetDiffusePaletteBlend(DiffuseIDs src, DiffuseIDs dst);
void SetSpecularPaletteBlend(SpecularIDs src, SpecularIDs dst);
void SetPaletteBlendFactor(float f);