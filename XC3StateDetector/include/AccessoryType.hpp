#pragma	once

enum class AccessoryType {
	Undefined,
	Bangle,
	Ring,
	Collar,
	Tiara
};

extern HashTable<AccessoryType, String> AccessoryTypeToNameEN;
extern HashTable<AccessoryType, String> AccessoryTypeToNameJP;
extern HashTable<String, HashTable<AccessoryType, String>> AccessoryTypeToName;
extern HashTable<String, AccessoryType> NameToAccessoryType;



