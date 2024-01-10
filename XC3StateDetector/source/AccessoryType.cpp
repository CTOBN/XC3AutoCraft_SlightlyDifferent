# include "AccessoryType.hpp"

HashTable<AccessoryType, String> AccessoryTypeToNameEN = {
	{AccessoryType::Undefined, U"Undefined"},
	{AccessoryType::Bangle, U"Bangle"},
	{AccessoryType::Ring, U"Ring"},
	{AccessoryType::Collar, U"Collar"},
	{AccessoryType::Tiara, U"Tiara"}
};

HashTable<AccessoryType, String> AccessoryTypeToNameJP = {
	{AccessoryType::Undefined, U"未定義"},
	{AccessoryType::Bangle, U"腕輪"},
	{AccessoryType::Ring, U"指輪"},
	{AccessoryType::Collar, U"首飾り"},
	{AccessoryType::Tiara, U"冠"}
};

HashTable<String, HashTable<AccessoryType, String>> AccessoryTypeToName = {
	{U"en-US", AccessoryTypeToNameEN},
	{U"ja-JP", AccessoryTypeToNameJP}
};

HashTable<String, AccessoryType> NameToAccessoryType = {
	{U"Undefined", AccessoryType::Undefined},
	{U"Bangle", AccessoryType::Bangle},
	{U"Ring", AccessoryType::Ring},
	{U"Collar", AccessoryType::Collar},
	{U"Tiara", AccessoryType::Tiara}
};


