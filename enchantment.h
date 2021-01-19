#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "card.h"

enum class EnchantmentType{StatMod, AbilityMod, Ability};

class Enchantment : public Card {
    // maps card name to pair<EnchantmentType, vector<description, playCost, (attackMod, defenceMod) or (abilityMod)>>
    static const std::map<std::string, std::pair<EnchantmentType, std::vector<std::string>>> enchantmentData;

	EnchantmentType enchType;
    std::string attackMod;
    std::string defenceMod;
	std::string abilityMod;
    
	public:
	Enchantment(std::string name);
	int modifyAttack(int atk);
	int modifyDefence(int def);
	int modifyAbility(int cost);
    std::vector<std::string> print() override;

    // *** add ability for Delay, and add functionality to enchantments
};

#endif
