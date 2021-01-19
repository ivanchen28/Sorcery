#ifndef DECK_H
#define DECK_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "card.h"

class Deck {
	static const std::vector<std::string> minionNames;
	static const std::vector<std::string> spellNames;
	static const std::vector<std::string> ritualNames;
	static const std::vector<std::string> enchantmentNames;

	std::vector<std::unique_ptr<Card>> cards;

	public:
	void initializeDeck(std::vector<std::string> deckList);
	int numCards();
	std::unique_ptr<Card> drawCard();
};

#endif
