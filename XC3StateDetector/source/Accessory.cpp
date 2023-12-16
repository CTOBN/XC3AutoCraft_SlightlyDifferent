#include "StatusType.hpp"
#include "StatusBoost.hpp"
#include "Accessory.hpp"

Array<size_t> Accessory::IDList;
Array<String> Accessory::DiscriptionENList;
Array<String> Accessory::DiscriptionJPList;
Array<String> Accessory::DiscriptionDetailJPList;
Array<String> Accessory::AlreadyList;
Array<String> Accessory::ProbabilityWristList;
Array<String> Accessory::ProbabilityFingerList;
Array<String> Accessory::ProbabilityNecklacesList;
Array<String> Accessory::ProbabilityCrownsList;



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

void Accessory::setStatusBoost(const StatusBoost& boost, const int i)
{
	m_statusBoosts[i] = boost;
}

 Array<StatusBoost> Accessory::getStatusBoosts() const
{
	 return m_statusBoosts;
}	

size_t Accessory::getID(int index)
{
	return IDList[index];
}

String Accessory::getDiscriptionEN(int index)
{
	return DiscriptionENList[index];
}

String Accessory::getDiscriptionJP(int index)
{
	return DiscriptionJPList[index];
}

String Accessory::getDiscriptionDetailJP(int index)
{
	return DiscriptionDetailJPList[index];
}

Array<String> Accessory::getDiscriptionENList()
{
	return DiscriptionENList;
}

Array<String> Accessory::getDiscriptionJPList()
{
	return DiscriptionJPList;
}

Array<String> Accessory::getDiscriptionDetailJPList()
{
	return DiscriptionDetailJPList;
}


bool Accessory::hasConsencutiveStatus() const
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

void Accessory::pushBackDiscriptionEN(const String& discriptionEN)
{
	DiscriptionENList.push_back(discriptionEN);
}

void Accessory::pushBackDiscriptionJP(const String& discriptionJP)
{
	DiscriptionJPList.push_back(discriptionJP);
}

void Accessory::pushBackDiscriptionDetailJP(const String& discriptionDetailJP)
{
	DiscriptionDetailJPList.push_back(discriptionDetailJP);
}

void Accessory::pushBackAlready(const String& already)
{
	AlreadyList.push_back(already);
}

void Accessory::pushBackWrist(const String& wrist)
{
	ProbabilityWristList.push_back(wrist);
}

void Accessory::pushBackFinger(const String& finger)
{
	ProbabilityFingerList.push_back(finger);
}

void Accessory::pushBackNecklaces(const String& necklaces)
{
	ProbabilityNecklacesList.push_back(necklaces);
}

void Accessory::pushBackCrowns(const String& crowns)
{
	ProbabilityCrownsList.push_back(crowns);
}

String Accessory::getAlready(int index)
{
	return AlreadyList[index];
}

String Accessory::getProbabilityWrist(int index)
{
	return ProbabilityWristList[index];
}

String Accessory::getProbabilityFinger(int index)
{
	return ProbabilityFingerList[index];
}

String Accessory::getProbabilityNecklaces(int index)
{
	return ProbabilityNecklacesList[index];
}

String Accessory::getProbabilityCrowns(int index)
{
	return ProbabilityCrownsList[index];
}
