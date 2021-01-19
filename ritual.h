#ifndef RITUAL_H
#define RITUAL_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "card.h"

class Ritual : public Card {
    // maps card name to pair<description, vector<playCost, chargeCost, charges>>
    static const std::map<std::string, std::pair<std::string, std::vector<int>>> ritualData;

    int chargeCost;
    int charges;
    
	public:
	Ritual(std::string name);
	bool hasCharge();
	void recharge(int charge);
	void useCharge();
    std::vector<std::string> print() override;

    // *** add ritual trigger
};

#endif
