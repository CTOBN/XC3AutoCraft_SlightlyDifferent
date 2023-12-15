#pragma once

class Accsessorie
{
public:
	Accsessorie() = default;
	Accsessorie(const size_t& index);
	size_t getIndex() const;
	static size_t getID(int index);
	static String getDiscriptionEN(int index);
	static String getDiscriptionJP(int index);
	static String getDiscriptionDetailJP(int index);
	static Array<String> getDiscriptionENList();
	static Array<String> getDiscriptionJPList();
	static Array<String> getDiscriptionDetailJPList();

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
	static void pushBackID(const size_t& id);
	static void pushBackDiscriptionEN(const String& discriptionEN);
	static void pushBackDiscriptionJP(const String& discriptionJP);
	static void pushBackDiscriptionDetailJP(const String& discriptionDetailJP);
	static void pushBackAlready(const String& already);
	static void pushBackWrist(const String& wrist);
	static void pushBackFinger(const String& finger);
	static void pushBackNecklaces(const String& necklaces);
	static void pushBackCrowns(const String& crowns);
	static String getAlready(int index);
	static String getProbabilityWrist(int index);
	static String getProbabilityFinger(int index);
	static String getProbabilityNecklaces(int index);
	static String getProbabilityCrowns(int index);



private:
	size_t m_index;
	static Array<size_t> IDList;
	static Array<String> DiscriptionENList;
	static Array<String> DiscriptionJPList;
	static Array<String> DiscriptionDetailJPList;
	static Array<String> AlreadyList;
	static Array<String> ProbabilityWristList;
	static Array<String> ProbabilityFingerList;
	static Array<String> ProbabilityNecklacesList;
	static Array<String> ProbabilityCrownsList;
	Array<String> m_statusList;
};
