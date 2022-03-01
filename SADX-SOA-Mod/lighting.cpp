#include "pch.h"

static bool IsLantern = false;

void SetCurrentPalettes(DiffuseIDs diffuse, SpecularIDs specular) {
	if (IsLantern)
	{
		set_diffuse((int32_t)diffuse, true);
		set_specular((int32_t)specular, true);
	}
}

void ResetPalettes()
{
	if (IsLantern)
	{
		set_diffuse((int32_t)DiffuseIDs::None, false);
		set_specular((int32_t)DiffuseIDs::None, false);
	}
}

bool LevelPaletteCallback(NJS_MATERIAL* material, Uint32 flags)
{
	set_diffuse((int32_t)DiffuseIDs::Level, false);
	set_specular((int32_t)DiffuseIDs::Level, false);
	return true;
}

void UnsetPaletteBlendMode()
{
	if (IsLantern)
	{
		set_blend_factor(0.0f);
		set_shader_flags(ShaderFlags_Blend, false);
		set_blend(-1, -1);
	}
}

void SetPaletteBlendMode()
{
	if (IsLantern)
	{
		set_shader_flags(ShaderFlags_Blend, true);
	}
}

void SetDiffusePaletteBlend(DiffuseIDs src, DiffuseIDs dst)
{
	if (IsLantern)
	{
		set_diffuse_blend((int)src, (int)dst);
	}
}

void SetSpecularPaletteBlend(SpecularIDs src, SpecularIDs dst)
{
	if (IsLantern)
	{
		set_specular_blend((int)src, (int)dst);
	}
}

void SetPaletteBlendFactor(float f)
{
	if (IsLantern)
	{
		set_blend_factor(f);
	}
}

void SetPaletteBlend(int a1, int a2)
{
	if (IsLantern)
	{
		set_blend(a1, a2);
	}
}

void RegisterLandPalette(NJS_OBJECT* object)
{
	if (IsLantern == false) 
		return;

	if (object->basicdxmodel)
	{
		for (int i = 0; i < object->basicdxmodel->nbMat; ++i) {
			NJS_MATERIAL* landmtl[1] = { &object->basicdxmodel->mats[i] };
			material_register(landmtl, 1, &LevelPaletteCallback);
		}
	}

	if (object->sibling)
	{
		RegisterLandPalette(object->sibling);
	}

	if (object->child)
	{
		RegisterLandPalette(object->child);
	}
}

void UnregisterLandPalette(NJS_OBJECT* object)
{
	if (IsLantern == false) return;

	for (int i = 0; i < object->basicdxmodel->nbMat; ++i)
	{
		NJS_MATERIAL* landmtl[1] = { &object->basicdxmodel->mats[i] };
		material_unregister(landmtl, 1, &LevelPaletteCallback);
	}

	if (object->sibling)
	{
		UnregisterLandPalette(object->sibling);
	}

	if (object->child)
	{
		UnregisterLandPalette(object->child);
	}
}

static std::string shaderPath;

const char* RegisterLevelShader(int32_t level, int32_t act)
{

	if (CurrentLevel != LevelIDs_ECGarden)
		return nullptr;

	shaderPath = "system\\";

	shaderPath += (std::string)"pirate-isle" + "-shaders.bin";

	return HelperFunctionsGlobal.GetReplaceablePath(shaderPath.c_str());
}

void OverridePaletteLoading()
{
	if (IsLantern == true)
	{
		pl_load_register(RegisterLevelShader);
	}
}

void LightingAPI_InitEnd()
{
	//Set up function pointers for Lantern Engine API (borrowed from PkR)

	HMODULE LanternDLL = GetModuleHandle(L"sadx-dc-lighting");

	if (LanternDLL)
	{
		IsLantern = true;
		OverridePaletteLoading();
	}
}