#ifndef CARD_H
#define CARD_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum class CardType{Null, Minion, MinionActivated, SpellNormal, SpellTarget, Ritual, Enchantment};

enum class Trigger{None, Start, Summon, Destroy, End};

class Card {
    protected:
    CardType cardType = CardType::Null;
	Trigger triggerType = Trigger::None;
	std::string name;
	std::string description;
	int playCost;

	public:
	Card(std::string name);
	std::string getName();
    int getCost();
    CardType getType();
	Trigger getTrigger();
    virtual std::vector<std::string> print() = 0;
};

#endif
