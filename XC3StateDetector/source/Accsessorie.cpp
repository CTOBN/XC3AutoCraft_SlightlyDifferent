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
	: m_index{ index }
{
	setStatusList({ U"", U"", U"", U"" });
}

size_t Accsessorie::getIndex() const
{
	return m_index;
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

void Accsessorie::setStatusList(const Array<String>& statusList)
{
	m_statusList = statusList;
}

Array<String> Accsessorie::getStatusList() const
{
	return m_statusList;
}

bool Accsessorie::hasConsencutiveStatus() const
{
	if (m_statusList.size() < 2)
		return false;
	String firstStatus = m_statusList[0];
	for (size_t i = 1; i < m_statusList.size(); ++i)
	{
		if (m_statusList[i] != firstStatus)
			return false;
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

