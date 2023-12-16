#include "StatusType.hpp"
#include "StatusBoost.hpp"
#include "Accsessorie.hpp"

Array<size_t> Accsessorie::IDList;
Array<String> Accsessorie::DiscriptionENList;
Array<String> Accsessorie::DiscriptionJPList;
Array<String> Accsessorie::DiscriptionDetailJPList;
Array<String> Accsessorie::AlreadyList;
Array<String> Accsessorie::ProbabilityWristList;
Array<String> Accsessorie::ProbabilityFingerList;
Array<String> Accsessorie::ProbabilityNecklacesList;
Array<String> Accsessorie::ProbabilityCrownsList;



Accsessorie::Accsessorie(const size_t& index)
	: m_index(index)
	, m_statusBoosts({ StatusBoost{StatusType::Undefined},
					 StatusBoost{StatusType::Undefined},
					 StatusBoost{StatusType::Undefined},
					 StatusBoost{StatusType::Undefined} })
{
}

Accsessorie::Accsessorie(const Accsessorie& other)
	: m_index(other.m_index)
	, m_statusBoosts(other.m_statusBoosts)
{
}

size_t Accsessorie::getIndex() const
{
	return m_index;
}

void Accsessorie::setStatusBoost(const StatusBoost& boost, const int i)
{
	m_statusBoosts[i] = boost;
}

 Array<StatusBoost> Accsessorie::getStatusBoosts() const
{
	 return m_statusBoosts;
}	

size_t Accsessorie::getID(int index)
{
	return IDList[index];
}

String Accsessorie::getDiscriptionEN(int index)
{
	return DiscriptionENList[index];
}

String Accsessorie::getDiscriptionJP(int index)
{
	return DiscriptionJPList[index];
}

String Accsessorie::getDiscriptionDetailJP(int index)
{
	return DiscriptionDetailJPList[index];
}

Array<String> Accsessorie::getDiscriptionENList()
{
	return DiscriptionENList;
}

Array<String> Accsessorie::getDiscriptionJPList()
{
	return DiscriptionJPList;
}

Array<String> Accsessorie::getDiscriptionDetailJPList()
{
	return DiscriptionDetailJPList;
}


bool Accsessorie::hasConsencutiveStatus() const
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

void Accsessorie::pushBackID(const size_t& id)
{
	IDList.push_back(id);
}

void Accsessorie::pushBackDiscriptionEN(const String& discriptionEN)
{
	DiscriptionENList.push_back(discriptionEN);
}

void Accsessorie::pushBackDiscriptionJP(const String& discriptionJP)
{
	DiscriptionJPList.push_back(discriptionJP);
}

void Accsessorie::pushBackDiscriptionDetailJP(const String& discriptionDetailJP)
{
	DiscriptionDetailJPList.push_back(discriptionDetailJP);
}

void Accsessorie::pushBackAlready(const String& already)
{
	AlreadyList.push_back(already);
}

void Accsessorie::pushBackWrist(const String& wrist)
{
	ProbabilityWristList.push_back(wrist);
}

void Accsessorie::pushBackFinger(const String& finger)
{
	ProbabilityFingerList.push_back(finger);
}

void Accsessorie::pushBackNecklaces(const String& necklaces)
{
	ProbabilityNecklacesList.push_back(necklaces);
}

void Accsessorie::pushBackCrowns(const String& crowns)
{
	ProbabilityCrownsList.push_back(crowns);
}

String Accsessorie::getAlready(int index)
{
	return AlreadyList[index];
}

String Accsessorie::getProbabilityWrist(int index)
{
	return ProbabilityWristList[index];
}

String Accsessorie::getProbabilityFinger(int index)
{
	return ProbabilityFingerList[index];
}

String Accsessorie::getProbabilityNecklaces(int index)
{
	return ProbabilityNecklacesList[index];
}

String Accsessorie::getProbabilityCrowns(int index)
{
	return ProbabilityCrownsList[index];
}
