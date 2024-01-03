# include "AccessoryType.hpp"

HashTable<AccessoryType, String> AccessoryTypeToNameEN = {
	{AccessoryType::Undefined, U"Undefined"},
	{AccessoryType::Bracelet, U"Bracelet"},
	{AccessoryType::Ring, U"Ring"},
	{AccessoryType::Necklace, U"Necklace"},
	{AccessoryType::Crown, U"Crown"}
};

HashTable<AccessoryType, String> AccessoryTypeToNameJP = {
	{AccessoryType::Undefined, U"未定義"},
	{AccessoryType::Bracelet, U"腕輪"},
	{AccessoryType::Ring, U"指輪"},
	{AccessoryType::Necklace, U"首飾り"},
	{AccessoryType::Crown, U"冠"}
};

HashTable<String, HashTable<AccessoryType, String>> AccessoryTypeToName = {
	{U"English", AccessoryTypeToNameEN},
	{U"Japanese", AccessoryTypeToNameJP}
};

HashTable<String, AccessoryType> NameToAccessoryType = {
	{U"Undefined", AccessoryType::Undefined},
	{U"Bracelet", AccessoryType::Bracelet},
	{U"Ring", AccessoryType::Ring},
	{U"Necklace", AccessoryType::Necklace},
	{U"Crown", AccessoryType::Crown}
};


