#include "StatusType.hpp"
#include "StatusBoost.hpp"
#include "Accessory.hpp"

Array<size_t> Accessory::IDList;
Array<String> Accessory::DescriptionENList;
Array<String> Accessory::DescriptionJPList;
Array<String> Accessory::DescriptionDetailJPList;
Array<String> Accessory::AlreadyList;
Array<double> Accessory::ProbabilityWristList;
Array<double> Accessory::ProbabilityFingerList;
Array<double> Accessory::ProbabilityNecklacesList;
Array<double> Accessory::ProbabilityCrownsList;



Accessory::Accessory(const size_t& index)
	: m_index(index)
	, m_statusBoosts({ StatusBoost{StatusType::Undefined},
					 StatusBoost{StatusType::Undefined},
					 StatusBoost{StatusType::Undefined},
					 StatusBoost{StatusType::Undefined} })
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
		if (m_statusBoosts[i].type == StatusType::Undefined)
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

String Accessory::getDescriptionEN(size_t index)
{
	return DescriptionENList[index];
}

String Accessory::getDescriptionJP(size_t index)
{
	return DescriptionJPList[index];
}

String Accessory::getDescriptionDetailJP(size_t index)
{
	return DescriptionDetailJPList[index];
}

Array<String> Accessory::getDescriptionENList()
{
	return DescriptionENList;
}

Array<String> Accessory::getDescriptionJPList()
{
	return DescriptionJPList;
}

Array<String> Accessory::getDescriptionDetailJPList()
{
	return DescriptionDetailJPList;
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

void Accessory::pushBackDescriptionEN(const String& discriptionEN)
{
	DescriptionENList.push_back(discriptionEN);
}

void Accessory::pushBackDescriptionJP(const String& discriptionJP)
{
	DescriptionJPList.push_back(discriptionJP);
}

void Accessory::pushBackDescriptionDetailJP(const String& discriptionDetailJP)
{
	DescriptionDetailJPList.push_back(discriptionDetailJP);
}

void Accessory::pushBackAlready(const String& already)
{
	AlreadyList.push_back(already);
}

void Accessory::pushBackWrist(const double& wrist)
{
	ProbabilityWristList.push_back(wrist);
}

void Accessory::pushBackFinger(const double& finger)
{
	ProbabilityFingerList.push_back(finger);
}

void Accessory::pushBackNecklaces(const double& necklaces)
{
	ProbabilityNecklacesList.push_back(necklaces);
}

void Accessory::pushBackCrowns(const double& crowns)
{
	ProbabilityCrownsList.push_back(crowns);
}

String Accessory::getAlready(size_t index)
{
	return AlreadyList[index];
}

double Accessory::getProbabilityWrist(size_t index)
{
	return ProbabilityWristList[index];
}

double Accessory::getProbabilityFinger(size_t index)
{
	return ProbabilityFingerList[index];
}

double Accessory::getProbabilityNecklaces(size_t index)
{
	return ProbabilityNecklacesList[index];
}

double Accessory::getProbabilityCrowns(size_t index)
{
	return ProbabilityCrownsList[index];
}
