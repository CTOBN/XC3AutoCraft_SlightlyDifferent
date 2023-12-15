#include "Accsessorie.hpp"

Accsessorie::Accsessorie(const size_t& index, const size_t& id, const String& discriptionEN, const String& discriptionJP)
	: m_index{ index }
	, m_id{ id }
	, m_discriptionEN{ discriptionEN }
	, m_discriptionJP{ discriptionJP }
{
	setStatusList({ U"", U"", U"", U"" });
}

size_t Accsessorie::getIndex() const
{
	return m_index;
}

size_t Accsessorie::getID() const
{
	return m_id;
}

String Accsessorie::getDiscriptionEN() const
{
	return m_discriptionEN;
}

String Accsessorie::getDiscriptionJP() const
{
	return m_discriptionJP;
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
