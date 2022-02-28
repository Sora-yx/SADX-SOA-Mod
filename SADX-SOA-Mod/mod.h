#pragma once

void ReadConfig(const char* path, const HelperFunctions& helperFunctions);

extern HelperFunctions HelperFunctionsGlobal;

void init_PirateIsle();
void Garden_TimeOfDay(ObjectMaster* obj);
uint8_t getTimeOfDay();

enum TimeOfDayE {
	day,
	evening,
	night
};