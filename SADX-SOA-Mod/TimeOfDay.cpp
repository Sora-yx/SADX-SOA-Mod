#include "pch.h"

enum TimeOfDayE {
	day,
	evening,
	night
};

uint8_t timeDay = day;
int timeDayTimer = 0;
const int timeDayValue = 1000;

void TimeOfDay_OnFrames()
{
	if (GameState != 15 || timeDay == night)
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
		timeDay = evening;
		break;
	case evening:
		timeDay = night;
		break;
	case night:
	default:
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
}

void Garden_TimeOfDay(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1;

	switch (data->Action)
	{
	case 0:
		Garden_ForceTimeOfDay(evening);
		data->Action++;
		break;
	case 1:
		TimeOfDay_OnFrames();
		TimeOfDay_Manage(data);
		break;
	case 2:
		data->InvulnerableTime = 0;
		LoadFlashScreen(0x00000000, 2);
		Garden_SetNextTimeOfDay();
		data->Action++;
		break;
	case 3:
		if (timeDay == night) {
			StopMusic();
			MusicList[MusicIDs_chao].Name = "Reflection";
		}
		else if (timeDay == day) {
			StopMusic();
			MusicList[MusicIDs_chao].Name = "Sky_Pirate_Isle";
		}
		else {
			data->Action = 1;
			return;
		}

		data->Action++;
		break;
	case 4:
		if (++data->InvulnerableTime == 70)
		{
			InitializeSoundManager();
			PlayMusic(MusicIDs_chao);
			data->Action = 1;
		}
		break;
	default:
		data->Action = 1;
		break;
	}
}