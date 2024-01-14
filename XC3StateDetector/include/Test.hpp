#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "Accessory.hpp"


TEST_CASE("testing the hasSameStatusTypeOrMore function 1") {
	// アクセサリーの生成
	Accessory accessory1{ 1, { StatusType::Agility, StatusType::Agility, StatusType::Agility, StatusType::Agility } };
	Accessory accessory2{ 1, { StatusType::Anything, StatusType::Anything, StatusType::Agility, StatusType::Agility } };

	// hasSameStatusTypeOrMore関数のテスト
	CHECK(accessory1.hasSameStatusTypeOrMore(accessory2) == true);
	CHECK(accessory2.hasSameStatusTypeOrMore(accessory1) == false);
}

TEST_CASE("testing the hasSameStatusTypeOrMore function 2") {
	// アクセサリーの生成
	Accessory accessory1{ 1, { StatusType::Anything, StatusType::Anything, StatusType::Anything, StatusType::Anything } };
	Accessory accessory2{ 1, { StatusType::Anything, StatusType::Anything, StatusType::Anything, StatusType::Anything } };

	// hasSameStatusTypeOrMore関数のテスト
	CHECK(accessory1.hasSameStatusTypeOrMore(accessory2) == true);
	CHECK(accessory2.hasSameStatusTypeOrMore(accessory1) == true);
}

TEST_CASE("testing the hasSameStatusTypeOrMore function 3") {
	// アクセサリーの生成
	Accessory desiredAccessory{ 1, { StatusType::HP, StatusType::HP, StatusType::Anything, StatusType::Anything } };
	Accessory recognizedAccessory{ 1, { StatusType::HealingPower, StatusType::HealingPower, StatusType::HP, StatusType::HP } };

	// hasSameStatusTypeOrMore関数のテスト
	CHECK(recognizedAccessory.hasSameStatusTypeOrMore(desiredAccessory) == true);
}
