#include "StatusType.hpp"

Array<String> StatusTypeStringListEnglish = {
	U"Anything",
	U"HP",
	U"Attack",
	U"HealingPower",
	U"Dexterity",
	U"Agility",
	U"BlockRate",
	U"CriticalRate"
};

Array<String> StatusTypeStringListJapanese = {
	U"任意",
	U"HP",
	U"攻撃力",
	U"回復力",
	U"器用さ",
	U"素早さ",
	U"ガード率",
	U"ｸﾘﾃｨｶﾙ率"
};

HashTable<String, Array<String>> StatusTypeStringList = {
	{U"en-US",  StatusTypeStringListEnglish},
	{U"ja-JP", StatusTypeStringListJapanese}
};

HashTable<StatusType, String> StatusTypeToString_EN = {
	{StatusType::Anything, U"Anything"},
	{StatusType::HP, U"HP"},
	{StatusType::Attack, U"Attack"},
	{StatusType::HealingPower, U"HealingPower"},
	{StatusType::Dexterity, U"Dexterity"},
	{StatusType::Agility, U"Agility"},
	{StatusType::BlockRate, U"BlockRate"},
	{StatusType::CriticalRate, U"CriticalRate"}
};

HashTable<StatusType, String> StatusTypeToString_JP = {
	{StatusType::Anything, U"任意"},
	{StatusType::HP, U"HP"},
	{StatusType::Attack, U"攻撃力"},
	{StatusType::HealingPower, U"回復力"},
	{StatusType::Dexterity, U"器用さ"},
	{StatusType::Agility, U"素早さ"},
	{StatusType::BlockRate, U"ガード率"},
	{StatusType::CriticalRate, U"ｸﾘﾃｨｶﾙ率"}
};

HashTable<String, HashTable<StatusType, String>> StatusTypeToString = {
	{U"en-US", StatusTypeToString_EN},
	{U"ja-JP", StatusTypeToString_JP}
};

HashTable<String, StatusType> StringToStatusType = {
	{U"Anything", StatusType::Anything},
	{U"HP", StatusType::HP},
	{U"Attack", StatusType::Attack},
	{U"HealingPower", StatusType::HealingPower},
	{U"Dexterity", StatusType::Dexterity},
	{U"Agility", StatusType::Agility},
	{U"BlockRate", StatusType::BlockRate},
	{U"CriticalRate", StatusType::CriticalRate},
	{U"任意", StatusType::Anything},
	{U"HP", StatusType::HP},
	{U"攻撃力", StatusType::Attack},
	{U"回復力", StatusType::HealingPower},
	{U"器用さ", StatusType::Dexterity},
	{U"素早さ", StatusType::Agility},
	{U"ガード率", StatusType::BlockRate},
	{U"ｸﾘﾃｨｶﾙ率", StatusType::CriticalRate}
};
