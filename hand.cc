#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ascii_graphics.h"
#include "hand.h"
#include "card.h"

Hand::Hand() {
}

int Hand::numCards() {
	return static_cast<int>(cards.size());
}

void Hand::drawCard(std::unique_ptr<Card> c) {
	cards.push_back(std::move(c));
}

Card * Hand::getCard(int i) {
	return cards[i - 1].get();
}

CardType Hand::getCardType(int i) {
	return cards[i - 1].get()->getType();
}

std::unique_ptr<Card> Hand::moveCard(int i) {
	std::unique_ptr<Card> temp = std::move(cards[i - 1]);
	cards.erase(cards.begin() + (i - 1));
	return std::move(temp);
}

void Hand::deleteCard(int i) {
	cards.erase(cards.begin() + (i - 1));
}

std::vector<std::string> Hand::printHand() {
	std::vector<std::string> empty(1, "Your hand is empty!");
	if (cards.size() == 0) return empty;
	std::vector<std::string> out(11, "\n");
	for (size_t i = 0; i < cards.size(); i++) out = combine_templates(out, cards[i].get()->print());
	return out;
}
