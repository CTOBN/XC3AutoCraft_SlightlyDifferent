#pragma once

enum class StatusType {
	Anything,
	HP,
	Attack,
	HealingPower,
	Dexterity,
	Agility,
	BlockRate,
	CriticalRate,
	Count,
};

extern Array<String> StatusTypeStringListEnglish;
extern Array<String> StatusTypeStringListJapanese;
extern HashTable<String, Array<String>> StatusTypeStringList;
extern HashTable<StatusType, String> StatusTypeToString_EN;
extern HashTable<StatusType, String> StatusTypeToString_JP;
extern HashTable<String, HashTable<StatusType, String>> StatusTypeToString;
extern HashTable<String, StatusType> StringToStatusType;




