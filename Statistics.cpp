#include "Statistics.h"

unsigned short Statistics::sweetsAmount = 0;
float Statistics::timePlayed = 0.f;
bool Statistics::isWon = false;

void Statistics::Refresh()
{
	sweetsAmount = 0;
	timePlayed = 0;
	isWon = false;
}