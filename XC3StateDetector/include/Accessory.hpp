﻿#pragma once
#include "StatusType.hpp"
#include "StatusBoost.hpp" // Include StatusBoost header file


class Accessory
{
public:
	Accessory() = delete;
	Accessory(const size_t& index);
	// コピーコンストラクタ
	Accessory(const Accessory& other);
	size_t getIndex() const;
	void setStatusBoost(const StatusBoost& boost, const int i);
	void setStatusBoosts(const Array<StatusBoost>& boosts);
	Array<StatusBoost> getStatusBoosts() const;
	bool hasSameStatusTypeOrMore(const Accessory& other) const;
	static size_t getID(int index);
	static String getDescriptionEN(int index);
	static String getDescriptionJP(int index);
	static String getDescriptionDetailJP(int index);
	static Array<String> getDescriptionENList();
	static Array<String> getDescriptionJPList();
	static Array<String> getDescriptionDetailJPList();
	bool hasConsecutiveStatus() const;

	Accessory& operator=(const Accessory& other)
	{
		if (this == &other) return *this;

		m_index = other.m_index;
		size_t size = std::min(getStatusBoosts().size(), other.m_statusBoosts.size());
		for (size_t i = 0; i < size; ++i)
		{
			setStatusBoost(other.m_statusBoosts[i], i);
		}
		return *this;
	}


	bool operator==(const Accessory& other) const
	{
		if (this->m_statusBoosts.size() != other.m_statusBoosts.size())
		{
			return false;
		}

		for (size_t i = 0; i < this->m_statusBoosts.size(); ++i)
		{
			if (this->m_statusBoosts[i].type != other.m_statusBoosts[i].type)
			{
				return false;
			}
		}
		return m_index == other.m_index;
	}
	static void pushBackID(const size_t& id);
	static void pushBackDescriptionEN(const String& discriptionEN);
	static void pushBackDescriptionJP(const String& discriptionJP);
	static void pushBackDescriptionDetailJP(const String& discriptionDetailJP);
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
	Array<StatusBoost> m_statusBoosts;
	static Array<size_t> IDList;
	static Array<String> DescriptionENList;
	static Array<String> DescriptionJPList;
	static Array<String> DescriptionDetailJPList;
	static Array<String> StatusTypeList_JP;
	static Array<String> StatusTypeList_EN;
	static Array<String> AlreadyList;
	static Array<String> ProbabilityWristList;
	static Array<String> ProbabilityFingerList;
	static Array<String> ProbabilityNecklacesList;
	static Array<String> ProbabilityCrownsList;

};
