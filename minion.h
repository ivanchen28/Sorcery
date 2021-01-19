#ifndef MINION_H
#define MINION_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "card.h"
#include "enchantment.h"
#include "ascii_graphics.h"

class Minion : public Card {
    // maps card name to pair<description, vector<atk, def, abilityCost>>
    static const std::map<std::string, std::pair<std::string, std::vector<int>>> minionData;

	// base values are minion base values
	// attack boost is from external effects such as Aura of Power, can only apply on summon so abilities ignored
	bool action = false;
	bool destroyTrigger = true;
    int baseAttack;
    int baseDefence;
	int attackBoost = 0;
	int damageTaken = 0;
    int effectCost;
	std::vector<std::unique_ptr<Enchantment>> enchantments;

	public:
	Minion(std::string name);
	void gainAction();
	bool getDestroyTrigger();
	void setDestroyTrigger(bool val);
	int getAttack();
	int getDefence();
	void setBaseDefence(int i);
	int getEffectCost();
	bool hasEnchantments();
	bool hasAction();
	void boostAttack(int i);
	void resetBoost();
	void heal(int i); // heal from all effects
	bool takeDamage(int dmg);
	void useAction();
	std::vector<std::string> print() override;

	void addEnchantment(std::unique_ptr<Enchantment> ench);
	void removeEnchantment(bool all); // if true, all enchantments are removed instead of just the top one
    std::vector<std::string> printEnchantments();

    // *** add abilities
};

#endif
