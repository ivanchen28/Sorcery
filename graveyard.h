#ifndef GRAVEYARD_H
#define GRAVEYARD_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "minion.h"

class Graveyard {
	std::vector<std::unique_ptr<Minion>> minions;

	public:
	Graveyard();
	int numMinions();
	void addMinion(std::unique_ptr<Minion>);
	std::unique_ptr<Minion> reviveTop();

	std::vector<std::string> printTop();
};

#endif
