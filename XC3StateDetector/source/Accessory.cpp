#include "StatusType.hpp"
#include "StatusBoost.hpp"
#include "Accessory.hpp"

Array<size_t> Accessory::IDList;
Array<String> Accessory::DescriptionEnglishList;
Array<String> Accessory::DescriptionJapaneseList;
Array<String> Accessory::DescriptionDetailJapaneseList;
Array<String> Accessory::AlreadyList;
Array<double> Accessory::ProbabilityBraceletList;
Array<double> Accessory::ProbabilityRingList;
Array<double> Accessory::ProbabilityNecklaceList;
Array<double> Accessory::ProbabilityCrownList;



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

String Accessory::getDescriptionEnglish(size_t index)
{
	return DescriptionEnglishList[index];
}

String Accessory::getDescriptionJapanese(size_t index)
{
	return DescriptionJapaneseList[index];
}

String Accessory::getDescriptionDetailJapanese(size_t index)
{
	return DescriptionDetailJapaneseList[index];
}

Array<String> Accessory::getDescriptionEnglishList()
{
	return DescriptionEnglishList;
}

Array<String> Accessory::getDescriptionJapaneseList()
{
	return DescriptionJapaneseList;
}

Array<String> Accessory::getDescriptionDetailJapaneseList()
{
	return DescriptionDetailJapaneseList;
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

void Accessory::pushBackDescriptionEnglish(const String& descriptionEnglish)
{
	DescriptionEnglishList.push_back(descriptionEnglish);
}

void Accessory::pushBackDescriptionJapanese(const String& descriptionJapanese)
{
	DescriptionJapaneseList.push_back(descriptionJapanese);
}

void Accessory::pushBackDescriptionDetailJapanese(const String& descriptionDetailJapanese)
{
	DescriptionDetailJapaneseList.push_back(descriptionDetailJapanese);
}

void Accessory::pushBackAlready(const String& already)
{
	AlreadyList.push_back(already);
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

String Accessory::getAlready(size_t index)
{
	return AlreadyList[index];
}

double Accessory::getProbability(size_t index, AccessoryType type)
{
	switch (type)
	{
	case AccessoryType::Bracelet:
		return ProbabilityBraceletList[index];
	case AccessoryType::Ring:
		return ProbabilityRingList[index];
	case AccessoryType::Necklace:
		return ProbabilityNecklaceList[index];
	case AccessoryType::Crown:
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
