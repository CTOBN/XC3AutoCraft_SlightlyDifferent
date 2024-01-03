#pragma once
#include "AccessoryType.hpp"
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
	void setStatusBoost(const StatusBoost& boost, const size_t i);
	void setStatusBoosts(const Array<StatusBoost>& boosts);
	Array<StatusBoost> getStatusBoosts() const;
	bool hasSameStatusTypeOrMore(const Accessory& other) const;
	static size_t getID(size_t index);
	static String getDescriptionEN(size_t index);
	static String getDescriptionJP(size_t index);
	static String getDescriptionDetailJP(size_t index);
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
	static void pushBackBracelet(const double& probabilitybracelet);
	static void pushBackRing(const double& probabilityRing);
	static void pushBackNecklace(const double& probabilityNecklace);
	static void pushBackCrown(const double& probabilityCrown);
	static String getAlready(size_t index);
	static double getProbability(size_t index, AccessoryType type);
	static double getProbabilityBracelet(size_t index);
	static double getProbabilityRing(size_t index);
	static double getProbabilityNecklace(size_t index);
	static double getProbabilityCrown(size_t index);

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
	static Array<double> ProbabilityBraceletList;
	static Array<double> ProbabilityRingList;
	static Array<double> ProbabilityNecklaceList;
	static Array<double> ProbabilityCrownList;

};
