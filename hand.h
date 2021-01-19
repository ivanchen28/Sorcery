#ifndef HAND_H
#define HAND_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "card.h"

class Hand {
	std::vector<std::unique_ptr<Card>> cards;

	public:
	Hand();
	int numCards();
	void drawCard(std::unique_ptr<Card>);
	Card * getCard(int i);
	CardType getCardType(int i);
	std::unique_ptr<Card> moveCard(int i);
	void deleteCard(int i);

	std::vector<std::string> printHand();
};

#endif
