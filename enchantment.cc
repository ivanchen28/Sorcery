#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ascii_graphics.h"
#include "enchantment.h"
#include "card.h"

// maps card name to pair<EnchantmentType, vector<description, playCost, (attackMod, defenceMod) or (abilityMod)>>
const std::map<std::string, std::pair<EnchantmentType, std::vector<std::string>>> Enchantment::enchantmentData = {
   {"Giant Strength", {EnchantmentType::StatMod, {"", "1", "+2", "+2"}}},
   {"Enrage", {EnchantmentType::StatMod, {"", "2", "*2", "-2"}}},
   {"Delay", {EnchantmentType::Ability, {"Enchanted minion does not gain an action on their next turn. This enchantment is automatically destroyed after 1 turn.", "1"}}},
   {"Magic Fatigue", {EnchantmentType::AbilityMod, {"Enchanted minion's activated ability costs 2 more", "0", "+2"}}},
   {"Silence", {EnchantmentType::AbilityMod, {"Enchanted minion cannot use abilities", "1", "s-1"}}}
};

Enchantment::Enchantment(std::string name) : Card(name) {
    if (enchantmentData.find(name) != enchantmentData.end()) {
        std::pair<EnchantmentType, std::vector<std::string>> data = enchantmentData.find(name)->second;
		cardType = CardType::Enchantment;
        enchType = data.first;
		description = data.second[0];
		playCost = std::stoi(data.second[1]);
		if (enchType == EnchantmentType::StatMod) {
			attackMod = data.second[2];
			defenceMod = data.second[3];
		}
		else if (enchType == EnchantmentType::AbilityMod) abilityMod = data.second[2];
    }
   	else if (name != "") std::cerr << "System error: enchantment.cc could not find enchantment " + name << std::endl;
}

int Enchantment::modifyAttack(int atk) {
	if (attackMod == "") return atk;
	char op = attackMod.at(0);
	int value = std::stoi(attackMod.substr(1));
	if (op == '*') atk *= value;
	if (op == '+') atk += value;
	if (op == '-') atk -= value;
	if (op == 's') atk = value;
	if (atk < 0) return 0;
	else return atk;
}

int Enchantment::modifyDefence(int def) {
	if (defenceMod == "") return def;
	char op = defenceMod.at(0);
	int value = std::stoi(defenceMod.substr(1));
	if (op == 's') return value;
	if (op == '*') def *= value;
	if (op == '+') def += value;
	if (op == '-') def -= value;
	if (def < 0) return 0;
	return def;
}

int Enchantment::modifyAbility(int cost) {
	if (abilityMod == "" || cost < 0) return cost; // cost is set to -1 to disable the ability
	char op = abilityMod.at(0);
	int value = std::stoi(abilityMod.substr(1));
	if (op == 's') return value;
	if (op == '*') cost *= value;
	if (op == '+') cost += value;
	if (op == '-') cost -= value;
	return cost;
}

std::vector<std::string> Enchantment::print() {
    if (enchType == EnchantmentType::StatMod) return display_enchantment_attack_defence(name, playCost, description, attackMod, defenceMod);
	else return display_enchantment(name, playCost, description);
}
