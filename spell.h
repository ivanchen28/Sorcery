#ifndef SPELL_H
#define SPELL_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "card.h"

class Spell : public Card {
    // maps card name to tuple<description, playCost, target> (target is true if it targets a minion/rituald)
    static const std::map<std::string, std::tuple<std::string, int, bool>> spellData;
    
	public:
	Spell(std::string name);
    std::vector<std::string> print() override;

    // *** add abilities
};

#endif
