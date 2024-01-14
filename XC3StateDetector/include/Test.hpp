#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "Accessory.hpp"


TEST_CASE("testing the hasSameStatusOrSuperior function") {
	// アクセサリーの生成
	Accessory desired{ 1, { StatusType::Anything, StatusType::Anything, StatusType::Anything, StatusType::Anything } };
	Accessory recognized1{ 1, { StatusType::Agility, StatusType::Agility, StatusType::Agility, StatusType::Agility } };
	Accessory recognized2{ 1, { StatusType::Agility, StatusType::Agility, StatusType::Agility, StatusType::HealingPower } };
	Accessory recognized3{ 1, { StatusType::Agility, StatusType::Agility, StatusType::HealingPower , StatusType::HealingPower } };

	// hasSameStatusTypeOrMore関数のテスト
	CHECK(recognized1.hasSameStatusOrSuperior(desired) == true);
	CHECK(recognized2.hasSameStatusOrSuperior(desired) == true);
	CHECK(recognized3.hasSameStatusOrSuperior(desired) == true);
}

TEST_CASE("testing the hasSameStatusOrSuperior function") {
	// アクセサリーの生成
	Accessory desired{ 1, { StatusType::CriticalRate, StatusType::CriticalRate, StatusType::Anything, StatusType::Anything } };
	Accessory recognized1{ 1, { StatusType::Agility, StatusType::Agility, StatusType::Agility, StatusType::Agility } };
	Accessory recognized2{ 1, { StatusType::Agility, StatusType::Agility, StatusType::Agility, StatusType::HealingPower } };
	Accessory recognized3{ 1, { StatusType::Agility, StatusType::Agility, StatusType::HealingPower , StatusType::HealingPower } };
	Accessory recognized4{ 1, { StatusType::CriticalRate, StatusType::CriticalRate, StatusType::HealingPower , StatusType::HealingPower } };
	Accessory recognized5{ 1, { StatusType::Agility, StatusType::Agility, StatusType::CriticalRate , StatusType::CriticalRate } };
	Accessory recognized6{ 1, { StatusType::Agility, StatusType::CriticalRate, StatusType::HealingPower , StatusType::HealingPower } };

	// hasSameStatusTypeOrMore関数のテスト
	CHECK(recognized1.hasSameStatusOrSuperior(desired) == false);
	CHECK(recognized2.hasSameStatusOrSuperior(desired) == false);
	CHECK(recognized3.hasSameStatusOrSuperior(desired) == false);
	CHECK(recognized4.hasSameStatusOrSuperior(desired) == true);
	CHECK(recognized5.hasSameStatusOrSuperior(desired) == true);
	CHECK(recognized6.hasSameStatusOrSuperior(desired) == false);
}

TEST_CASE("testing the hasSameStatusOrSuperior function") {
	// アクセサリーの生成
	Accessory desired{ 1, { StatusType::CriticalRate, StatusType::CriticalRate, StatusType::CriticalRate, StatusType::Anything } };
	Accessory recognized1{ 1, { StatusType::Attack, StatusType::Attack, StatusType::Attack, StatusType::Attack } };
	Accessory recognized2{ 1, { StatusType::Attack, StatusType::Attack, StatusType::Attack, StatusType::HealingPower } };
	Accessory recognized3{ 1, { StatusType::CriticalRate, StatusType::CriticalRate, StatusType::CriticalRate , StatusType::HealingPower } };
	Accessory recognized4{ 1, { StatusType::CriticalRate, StatusType::CriticalRate, StatusType::HealingPower , StatusType::HealingPower } };
	Accessory recognized5{ 1, { StatusType::Agility, StatusType::Agility, StatusType::CriticalRate , StatusType::CriticalRate } };
	Accessory recognized6{ 1, { StatusType::Agility, StatusType::CriticalRate, StatusType::HealingPower , StatusType::HealingPower } };

	// hasSameStatusTypeOrMore関数のテスト
	CHECK(recognized1.hasSameStatusOrSuperior(desired) == false);
	CHECK(recognized2.hasSameStatusOrSuperior(desired) == false);
	CHECK(recognized3.hasSameStatusOrSuperior(desired) == true);
	CHECK(recognized4.hasSameStatusOrSuperior(desired) == false);
	CHECK(recognized5.hasSameStatusOrSuperior(desired) == false);
	CHECK(recognized6.hasSameStatusOrSuperior(desired) == false);
}
