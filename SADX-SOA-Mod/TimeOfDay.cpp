#include "pch.h"

uint8_t timeDay = day;
int timeDayTimer = 0;
const int timeDayValue = 7380;

uint8_t getTimeOfDay()
{
	return timeDay;
}

void TimeOfDay_OnFrames()
{
	if (GameState != 15)
		return;

	timeDayTimer++;
}

void TimeOfDay_Manage(EntityData1* data)
{
	if (timeDayTimer >= timeDayValue)
	{
		timeDayTimer = 0;
		data->Action++;
	}
}

void Garden_SetNextTimeOfDay()
{
	switch (timeDay)
	{
	case day:
		SetPaletteBlendMode();
		SetPaletteBlend(0, 5);
		SetPaletteBlendFactor(0.5f);
		timeDay = evening;
		break;
	case evening:
		SetPaletteBlendMode();
		SetPaletteBlend(0, 6);
		SetPaletteBlendFactor(0.5f);
		timeDay = night;
		break;
	case night:
	default:
		UnsetPaletteBlendMode();
		timeDay = day;
		break;
	}

	return;
}

void Garden_ForceTimeOfDay(int time)
{
	if (time > night || time < day)
		time = day;

	timeDay = time;

	switch (timeDay)
	{
	case evening:
		SetPaletteBlendMode();
		SetPaletteBlend(0, 5);
		SetPaletteBlendFactor(0.5f);
		return;
	case night:
		SetPaletteBlendMode();
		SetPaletteBlend(0, 6);
		SetPaletteBlendFactor(0.5f);
		return;
	}
}

void Garden_TimeOfDay(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	switch (data->Action)
	{
	case 0:

		if (timeDayOption != 0) {
			Garden_ForceTimeOfDay(timeDayOption - 1);
			CheckThingButThenDeleteObject(obj);
			return;
		}
		else {
			data->Action++;
		}
	
		break;
	case 1:
		TimeOfDay_OnFrames();
		TimeOfDay_Manage(data);
		break;
	case 2:
		data->InvulnerableTime = 0;
		LoadFlashScreen(0x00000000, 2);
		data->Action++;
		break;
	case 3:
		if (++data->InvulnerableTime == 115)
		{
			Garden_SetNextTimeOfDay();
			data->Action++;
		}
		break;
	case 4:
	
		if (timeDay == night) {
			MusicList[MusicIDs_chao].Name = "Reflection";

		}
		else if (timeDay == day) {
			MusicList[MusicIDs_chao].Name = "Sky_Pirate_Isle";
		}

		if (timeDay != evening)
		{
			StopMusic();
			InitializeSoundManager();
			PlayMusic(MusicIDs_chao);
		}
		
		data->Action = 1;
		return;
	default:
		data->Action = 1;
		break;
	}
}