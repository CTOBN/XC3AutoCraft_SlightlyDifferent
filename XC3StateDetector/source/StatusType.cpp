#include "StatusType.hpp"


HashTable<StatusType, String> StatusTypeToString_EN = {
	{StatusType::Unselected, U"Unselected"},
	{StatusType::HP, U"HP"},
	{StatusType::Attack, U"Attack"},
	{StatusType::HealingPower, U"HealingPower"},
	{StatusType::Dexterity, U"Dexterity"},
	{StatusType::Agility, U"Agility"},
	{StatusType::BlockRate, U"BlockRate"},
	{StatusType::CriticalRate, U"CriticalRate"}
};

HashTable<StatusType, String> StatusTypeToString_JP = {
	{StatusType::Unselected, U"未選択"},
	{StatusType::HP, U"HP"},
	{StatusType::Attack, U"攻撃力"},
	{StatusType::HealingPower, U"回復力"},
	{StatusType::Dexterity, U"器用さ"},
	{StatusType::Agility, U"素早さ"},
	{StatusType::BlockRate, U"ガード率"},
	{StatusType::CriticalRate, U"ｸﾘﾃｨｶﾙ率"}
};

HashTable<String, HashTable<StatusType, String>> StatusTypeToString = {
	{U"English", StatusTypeToString_EN},
	{U"Japanese", StatusTypeToString_JP}
};

HashTable<String, StatusType> StringToStatusType = {
	{U"Undefined", StatusType::Unselected},
	{U"HP", StatusType::HP},
	{U"Attack", StatusType::Attack},
	{U"HealingPower", StatusType::HealingPower},
	{U"Dexterity", StatusType::Dexterity},
	{U"Agility", StatusType::Agility},
	{U"BlockRate", StatusType::BlockRate},
	{U"CriticalRate", StatusType::CriticalRate}
};
