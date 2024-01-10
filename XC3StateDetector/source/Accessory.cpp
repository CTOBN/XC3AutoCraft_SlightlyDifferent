#include "StatusType.hpp"
#include "StatusBoost.hpp"
#include "Accessory.hpp"

Array<size_t> Accessory::IDList;
Array<String> Accessory::SpecialEffectDetailJapaneseList;
Array<double> Accessory::ProbabilityBraceletList;
Array<double> Accessory::ProbabilityRingList;
Array<double> Accessory::ProbabilityNecklaceList;
Array<double> Accessory::ProbabilityCrownList;
HashTable<String, Array<String>> Accessory::SpecialEffectList;
HashTable<String, Array<String>> Accessory::CompatibilityList;


Accessory::Accessory(const size_t& index)
	: m_index(index)
	, m_statusBoosts({ StatusBoost{StatusType::Anything},
					 StatusBoost{StatusType::Anything},
					 StatusBoost{StatusType::Anything},
					 StatusBoost{StatusType::Anything} })
{
}

Accessory::Accessory(const Accessory& other)
	: m_index(other.m_index)
	, m_statusBoosts(other.m_statusBoosts)
{
}

size_t Accessory::getIndex() const
{
	return m_index;
}

void Accessory::setStatusBoost(const StatusBoost& boost, const size_t i)
{
	m_statusBoosts[i] = boost;
}

void Accessory::setStatusBoosts(const Array<StatusBoost>& boosts)
{
	m_statusBoosts = boosts;
}

/// @brief 他のアクセサリーの各ステータスタイプの数が、このアクセサリーのそれと同じか、それ以上であるかを判定する
/// @param other 比較対象のアクセサリー
/// @return 他のアクセサリーの各ステータスタイプの数が、このアクセサリーのそれと同じか、それ以上であるか
bool Accessory::hasSameStatusTypeOrMore(const Accessory& other) const
{
	HashTable<StatusType, int8> thisStatusTypesCount;
	HashTable<StatusType, int8> otherStatusTypesCount;

	for (size_t i = 0; i < m_statusBoosts.size(); ++i)
	{
		if (m_statusBoosts[i].type == StatusType::Anything)
		{
			continue;
		}
		thisStatusTypesCount[m_statusBoosts[i].type] += 1;
		otherStatusTypesCount[other.m_statusBoosts[i].type] += 1;
	}
	for (auto& pair : otherStatusTypesCount)
	{
		if (thisStatusTypesCount[pair.first] < pair.second)
		{
			return false;
		}
	}
	return true;
}

 Array<StatusBoost> Accessory::getStatusBoosts() const
{
	 return m_statusBoosts;
}	

size_t Accessory::getID(size_t index)
{
	return IDList[index];
}

String Accessory::getSpecialEffectDetailJapanese(size_t index)
{
	return SpecialEffectDetailJapaneseList[index];
}

Array<String> Accessory::getSpecialEffectDetailJapaneseList()
{
	return SpecialEffectDetailJapaneseList;
}


bool Accessory::hasConsecutiveStatus() const
{
	StatusType firstType = m_statusBoosts[0].type;
	for (size_t i = 1; i < m_statusBoosts.size(); ++i)
	{
		if (m_statusBoosts[i].type != firstType)
		{
			return false;
		}
	}
	return true;
}

void Accessory::pushBackID(const size_t& id)
{
	IDList.push_back(id);
}

void Accessory::pushBackSpecialEffectDetailJapanese(const String& specialEffectDetailJapanese)
{
	SpecialEffectDetailJapaneseList.push_back(specialEffectDetailJapanese);
}

void Accessory::pushBackBracelet(const double& probabilityBracelet)
{
	ProbabilityBraceletList.push_back(probabilityBracelet);
}

void Accessory::pushBackRing(const double& probabilityRing)
{
	ProbabilityRingList.push_back(probabilityRing);
}

void Accessory::pushBackNecklace(const double& probabilityNecklace)
{
	ProbabilityNecklaceList.push_back(probabilityNecklace);
}

void Accessory::pushBackCrown(const double& probabilityCrown)
{
	ProbabilityCrownList.push_back(probabilityCrown);
}

double Accessory::getProbability(size_t index, AccessoryType type)
{
	switch (type)
	{
	case AccessoryType::Bangle:
		return ProbabilityBraceletList[index];
	case AccessoryType::Ring:
		return ProbabilityRingList[index];
	case AccessoryType::Collar:
		return ProbabilityNecklaceList[index];
	case AccessoryType::Tiara:
		return ProbabilityCrownList[index];
	default:
		return 0;
	}
}

double Accessory::getProbabilityBracelet(size_t index)
{
	return ProbabilityBraceletList[index];
}

double Accessory::getProbabilityRing(size_t index)
{
	return ProbabilityRingList[index];
}

double Accessory::getProbabilityNecklace(size_t index)
{
	return ProbabilityNecklaceList[index];
}

double Accessory::getProbabilityCrown(size_t index)
{
	return ProbabilityCrownList[index];
}

void Accessory::emplaceSpecialEffectList(String language, Array<String> SpecialEffectListLanguage)
{
	SpecialEffectList.emplace(language, SpecialEffectListLanguage);
}

Array<String> Accessory::getSpecialEffectList(String language)
{
	return SpecialEffectList[language];
}

void Accessory::emplaceCompatibilityList(String language, Array<String> CompatibilityListLanguage)
{
	CompatibilityList.emplace(language, CompatibilityListLanguage);
}

Array<String> Accessory::getCompatibilityList(String language)
{
	return CompatibilityList[language];
}
