#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "card.h"

Card::Card(std::string name) : name(name) {
}

std::string Card::getName() {
	return name;
}

int Card::getCost() {
    return playCost;
}

CardType Card::getType() {
    return cardType;
}

Trigger Card::getTrigger() {
	return triggerType;
}
