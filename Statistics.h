#pragma once

#include "Time.h"

class Statistics
{
public:
	
	static void AddSweet() { sweetsAmount++; }

	static void AddTime() { timePlayed += Time::DeltaTime(); }

	static void Refresh();

	static void FinishGame() { isWon = true; }

	static bool IsGameWon() { return isWon; }
	static unsigned short GetCollectedSweets() { return sweetsAmount; }
	static float GetTimePlayed() { return timePlayed; }

	static std::string GetTimePlayedString() { return std::to_string((int)timePlayed / ( 60 * 60) % 60) + " : " + std::to_string((int)timePlayed / ( 60) % 60) + " : " + std::to_string((int)timePlayed  % 60); }

private:
	static unsigned short sweetsAmount;
	static float timePlayed;
	static bool isWon;
};

