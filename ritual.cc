#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ascii_graphics.h"
#include "ritual.h"

// maps card name to pair<description, vector<playCost, chargeCost, charges>>
const std::map<std::string, std::pair<std::string, std::vector<int>>> Ritual::ritualData = {
    {"Dark Ritual", {"At the start of your turn, gain 1 magic.", {0, 1, 5}}},
    {"Aura of Power", {"Whenever a minion enters play under your control, it gains +1/+1.", {1, 1, 4}}},
    {"Standstill", {"Whenever a minion enters play, destroy it.", {3, 2, 4}}}
};

Ritual::Ritual(std::string name) : Card(name) {
    if (ritualData.find(name) != ritualData.end()) {
        std::pair<std::string, std::vector<int>> data = ritualData.find(name)->second;
		if (name == std::string("Dark Ritual")) triggerType = Trigger::Start;
		if (name == std::string("Aura of Power")) triggerType = Trigger::Summon;
		if (name == std::string("Standstill")) triggerType = Trigger::Summon;
        description = data.first;
        playCost = data.second[0];
        chargeCost = data.second[1];
        charges = data.second[2];
        cardType = CardType::Ritual;
    }
    else if (name != "") std::cerr << "System error: ritual.cc could not find ritual " + name << std::endl;
}

bool Ritual::hasCharge() {
	if (charges >= chargeCost) return true;
	return false;
}

void Ritual::recharge(int charge) {
	charges += charge;
}

void Ritual::useCharge() {
	charges -= chargeCost;
}

std::vector<std::string> Ritual::print() {
    return display_ritual(name, playCost, chargeCost, description, charges);
}
