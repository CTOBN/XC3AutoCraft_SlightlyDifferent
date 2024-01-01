﻿#pragma once

enum class StatusType {
	Unselected,
	HP,
	Attack,
	HealingPower,
	Dexterity,
	Agility,
	BlockRate,
	CriticalRate
};

extern HashTable<StatusType, String> StatusTypeToString_EN;
extern HashTable<StatusType, String> StatusTypeToString_JP;
extern HashTable<String, HashTable<StatusType, String>> StatusTypeToString;
extern HashTable<String, StatusType> StringToStatusType;




