#include "StatusType.hpp"


HashTable<StatusType, String> StatusTypeToString_EN = {
	{StatusType::Undefined, U"Undefined"},
	{StatusType::HP, U"HP"},
	{StatusType::Attack, U"Attack"},
	{StatusType::HealingPower, U"HealingPower"},
	{StatusType::Dexterity, U"Dexterity"},
	{StatusType::Agility, U"Agility"},
	{StatusType::BlockRate, U"BlockRate"},
	{StatusType::CriticalRate, U"CriticalRate"}
};

HashTable<StatusType, String> StatusTypeToString_JP = {
	{StatusType::Undefined, U"未定義"},
	{StatusType::HP, U"HP"},
	{StatusType::Attack, U"攻撃力"},
	{StatusType::HealingPower, U"回復力"},
	{StatusType::Dexterity, U"器用さ"},
	{StatusType::Agility, U"素早さ"},
	{StatusType::BlockRate, U"ガード率"},
	{StatusType::CriticalRate, U"クリティカル率"}
};

HashTable<String, HashTable<StatusType, String>> StatusTypeToString = {
	{U"EN", StatusTypeToString_EN},
	{U"JP", StatusTypeToString_JP}
};

HashTable<String, StatusType> StringToStatusType = {
	{U"Undefined", StatusType::Undefined},
	{U"HP", StatusType::HP},
	{U"Attack", StatusType::Attack},
	{U"HealingPower", StatusType::HealingPower},
	{U"Dexterity", StatusType::Dexterity},
	{U"Agility", StatusType::Agility},
	{U"BlockRate", StatusType::BlockRate},
	{U"CriticalRate", StatusType::CriticalRate}
};
