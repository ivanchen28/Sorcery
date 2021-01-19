#ifndef BOARD_H
#define BOARD_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "minion.h"
#include "ritual.h"
#include "enchantment.h"

class Board {
	std::vector<std::unique_ptr<Minion>> minions;
	std::unique_ptr<Ritual> ritual;

	public:
	Board();
	int numMinions();
	void addMinion(std::unique_ptr<Minion>);
	Minion * getMinion(int i);
	void gainActions();
	std::unique_ptr<Minion> removeMinion(int i);
	void destroyRitual();
	
	void addRitual(std::unique_ptr<Ritual>);
	Ritual * getRitual();

	std::vector<std::string> printBoard(int playerNum, std::string name, int life, int mana, std::vector<std::string> graveyard);
	std::vector<std::string> printMinion(int i);
};

#endif
