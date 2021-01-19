#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "minion.h"

// maps card name to pair<description, vector<playCost, atk, def, abilityCost>>
const std::map<std::string, std::pair<std::string, std::vector<int>>> Minion::minionData = {
    {"Air Elemental", {"", {0, 1, 1}}},
    {"Earth Elemental", {"", {3, 4, 4}}},
    {"Bomb", {"Deals damage to all the opponent minions equal to its attack value when it dies.", {2, 1, 2}}},
    {"Fire Elemental", {"Whenever an opponent's minion enters play, deal 1 damage to it.", {2, 2, 2}}},
    {"Potion Seller", {"At the end of your turn, all your minions gain +0/+1.", {2, 1, 3}}},
    {"Novice Pyromancer", {"Deal 1 damage to target minion.", {1, 0, 1, 1}}},
    {"Apprentice Summoner", {"Summon a 1/1 Air Elemental.", {1, 1, 1, 1}}},
    {"Master Summoner", {"Summon up to three 1/1 Air Elementals", {3, 2, 3, 2}}}
};

Minion::Minion(std::string name) : Card(name) {
    if (minionData.find(name) != minionData.end()) {
        std::pair<std::string, std::vector<int>> data = minionData.find(name)->second;
		if (name == std::string("Bomb")) triggerType = Trigger::Destroy;
		if (name == std::string("Fire Elemental")) triggerType = Trigger::Summon;
		if (name == std::string("Potion Seller")) triggerType = Trigger::End;
        description = data.first;
        playCost = data.second[0];
        baseAttack = data.second[1];
        baseDefence = data.second[2];
        if (data.second.size() > 3) {
            cardType = CardType::MinionActivated;
            effectCost = data.second[3];
		}
        else {
            effectCost = -1;
            cardType = CardType::Minion;
        }
    }
    else if (name != "") std::cerr << "System error: minion.cc could not find minion " + name << std::endl;
}

void Minion::gainAction() {
	for (size_t i = 0; i < enchantments.size(); i++) {
		std::string delay = "Delay";
		if (enchantments[i].get()->getName() == delay) {
			enchantments.erase(enchantments.begin() + i);
			// delay only prevents the reset of action point
			// if for some reason it already had an action point that wasn't used, nothing is changed
			return;
		}
	}
	action = true;
}

bool Minion::getDestroyTrigger() {
	return destroyTrigger;
}

void Minion::setDestroyTrigger(bool val) {
	destroyTrigger = val;
}

int Minion::getAttack() {
	int atk = baseAttack + attackBoost;
	for (size_t i = 0; i < enchantments.size(); i++) atk = enchantments[i].get()->modifyAttack(atk);
	return atk;
}

int Minion::getDefence() {
	int def = baseDefence;
	for (size_t i = 0; i < enchantments.size(); i++) def = enchantments[i].get()->modifyDefence(def);
	return def - damageTaken;
}

void Minion::setBaseDefence(int i) {
	baseDefence = i;
}

int Minion::getEffectCost() {
	int cost = effectCost;
	for (size_t i = 0; i < enchantments.size(); i++) cost = enchantments[i].get()->modifyAbility(cost);
	return cost;
}

bool Minion::hasEnchantments() {
	if (enchantments.size() > 0) return true;
	return false;
}

bool Minion::hasAction() {
	return action;
}

void Minion::boostAttack(int i) {
	attackBoost += i;
}

void Minion::resetBoost() {
	attackBoost = 0;
}

void Minion::heal(int i) {
	damageTaken -= i;
}

bool Minion::takeDamage(int dmg) {
	damageTaken += dmg;
	if (getDefence() <= 0) return true; // tell player class to destroy minion, trigger destruction effects
	else return false;
}

void Minion::useAction() {
	if (action) action = false;
	else std::cerr << "System error: minion had no action but used it!" << std::endl;
}

void Minion::addEnchantment(std::unique_ptr<Enchantment> ench) {
	enchantments.push_back(std::move(ench));
}

void Minion::removeEnchantment(bool all) {
	if (all) enchantments.clear();
	else enchantments.erase(enchantments.begin() + (enchantments.size() - 1));
}

std::vector<std::string> Minion::print() {
    if (cardType != CardType::MinionActivated) return display_minion_triggered_ability(name, playCost, getAttack(), getDefence(), description, action);
    else return display_minion_activated_ability(name, playCost, getAttack(), getDefence(), getEffectCost(), description, action);
}

std::vector<std::string> Minion::printEnchantments() {
	std::vector<std::string> out = print();
	for (size_t i = 0; i < enchantments.size(); i = i + 5) {
		std::vector<std::string> row(11, "");
		for (size_t j = i; j < std::min(enchantments.size(), i + 5); j++) row = combine_templates(row, enchantments[j].get()->print());
		out.insert(out.end(), row.begin(), row.end());
	}
	return out;
}
