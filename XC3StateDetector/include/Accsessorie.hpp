#pragma once

class Accsessorie
{
public:
	Accsessorie() = default;
	Accsessorie(const size_t& index, const size_t& id, const String& discriptionEN, const String& discriptionJP);
	size_t getIndex() const;
	size_t getID() const;
	String getDiscriptionEN() const;
	String getDiscriptionJP() const;
	void setStatusList(const Array<String>& statusList);
	Array<String> getStatusList() const;
	bool hasConsencutiveStatus() const;
	bool operator==(const Accsessorie& other) const
	{
		for (size_t i = 0; i < m_statusList.size(); ++i)
		{
			if (m_statusList[i] != other.m_statusList[i])
				return false;
		}
		return m_index == other.m_index;
	}

private:
	size_t m_index = 0;
	size_t m_id = 0;
	String m_discriptionEN;
	String m_discriptionJP;
	Array<String> m_statusList;
};
