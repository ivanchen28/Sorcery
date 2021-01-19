#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "ascii_graphics.h"
#include "spell.h"

// maps card name to tuple<description, playCost, target> (target is true if it targets a minion/rituald)
const std::map<std::string, std::tuple<std::string, int, bool>> Spell::spellData = {
    {"Banish", {"Destroy target minion or ritual.", 2, true}},
    {"Unsummon", {"Return target minion to its owner's hand", 1, true}},
    {"Recharge", {"Your ritual gains 3 charges.", 1, false}},
    {"Disenchant", {"Destroy the top enchantment on a target minion.", 1, true}},
    {"Raise Dead", {"Ressurect the top minion in your graveyard and set its defence to 1.", 1, false}},
    {"Blizzard", {"Deal 2 damage to all minions", 3, false}}
};

Spell::Spell(std::string name) : Card(name) {
    if (spellData.find(name) != spellData.end()) {
        std::tuple<std::string, int, bool> data = spellData.find(name)->second;
        description = std::get<0>(data);
        playCost = std::get<1>(data);
        if (std::get<2>(data)) cardType = CardType::SpellTarget; // spell targets a minion or ritual
        else cardType = CardType::SpellNormal; // spell does not target a minion or ritual, targets player that activated
    }
    else if (name != "") std::cerr << "System error: spell.cc could not find spell " + name << std::endl;
}

std::vector<std::string> Spell::print() {
    return display_spell(name, playCost, description);
}
