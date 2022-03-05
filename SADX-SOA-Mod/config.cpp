#include "pch.h"

uint8_t timeDayOption = 0;

void ReadConfig(const char* path, const HelperFunctions& helperFunctions) {
	//Ini file Configuration
	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
	timeDayOption = config->getInt("General", "timeDayOption", progressive);
	delete config;

	return;
}