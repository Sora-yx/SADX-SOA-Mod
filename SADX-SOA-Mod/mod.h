#pragma once

extern std::string modpath;

extern bool isLeavingGarden;
void ReadConfig(const char* path, const HelperFunctions& helperFunctions);

extern HelperFunctions HelperFunctionsGlobal;

void Garden_TimeOfDay(ObjectMaster* obj);
uint8_t getTimeOfDay();
void Garden_ForceTimeOfDay(int time);
extern uint8_t timeDayOption;
void init_ChaoFixes();

enum TimeOfDayE {
	day,
	evening,
	night
};

enum timeDayOption {
	progressive,
	alwaysDay,
	alwaysEvening,
	alwaysNight
};

enum CustomChaoArea
{
	PirateIsle = 5,
	PirateSecretBase = 8,
};

enum SOALezvel
{
	ShrineIsle = 33
};


#define TARGET_DYNAMIC(name) ((decltype(name##_r)*)name##_t->Target())