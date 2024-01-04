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
	static String getSpecialEffectEnglish(size_t index);
	static String getSpecialEffectJapanese(size_t index);
	static String getSpecialEffectDetailJapanese(size_t index);
	static Array<String> getSpecialEffectEnglishList();
	static Array<String> getSpecialEffectJapaneseList();
	static Array<String> getSpecialEffectDetailJapaneseList();
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
	static void pushBackSpecialEffectEnglish(const String& specialEffectEnglish);
	static void pushBackSpecialEffectJapanese(const String& specialEffectJapanese);
	static void pushBackSpecialEffectJapanese(const String& specialEffectDetailJapanese);
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
	static Array<String> DescriptionEnglishList;
	static Array<String> DescriptionJapaneseList;
	static Array<String> DescriptionDetailJapaneseList;
	static Array<String> StatusTypeListJapanese;
	static Array<String> StatusTypeListEnglish;
	static Array<String> AlreadyList;
	static Array<double> ProbabilityBraceletList;
	static Array<double> ProbabilityRingList;
	static Array<double> ProbabilityNecklaceList;
	static Array<double> ProbabilityCrownList;

};
