#pragma once

#include "StatusType.hpp"

class StatusBoost {
public:
	StatusBoost(StatusType _type);
	StatusType type;
private:
	int16 m_amount;
};
