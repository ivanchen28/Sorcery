#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ascii_graphics.h"
#include "graveyard.h"
#include "minion.h"

Graveyard::Graveyard() {
}

int Graveyard::numMinions() {
	return minions.size();
}

void Graveyard::addMinion(std::unique_ptr<Minion> m) {
	// check to see if there is enough space is done by the Player class
	minions.push_back(std::move(m));
}

std::unique_ptr<Minion> Graveyard::reviveTop() {
	// check to see if card is valid is done by the Player class
	std::unique_ptr<Minion> temp = std::move(minions[minions.size() - 1]);
	minions.erase(minions.begin() + (minions.size() - 1));
	return temp;
}

std::vector<std::string> Graveyard::printTop() {
	if (minions.size() == 0) return CARD_TEMPLATE_BORDER;
	else return minions[minions.size() - 1].get()->print();
}
