#include <algorithm>
#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ascii_graphics.h"
#include "deck.h"
#include "minion.h"
#include "spell.h"
#include "ritual.h"
#include "enchantment.h"

const std::vector<std::string> Deck::minionNames = {"Air Elemental", "Earth Elemental", "Bomb", "Fire Elemental", "Potion Seller", "Novice Pyromancer", "Apprentice Summoner", "Master Summoner"};
const std::vector<std::string> Deck::spellNames = {"Banish", "Unsummon", "Recharge", "Disenchant", "Raise Dead", "Blizzard"};
const std::vector<std::string> Deck::ritualNames = {"Dark Ritual", "Aura of Power", "Standstill"};
const std::vector<std::string> Deck::enchantmentNames = {"Giant Strength", "Enrage", "Delay", "Magic Fatigue", "Silence"};

void Deck::initializeDeck(std::vector<std::string> cardNames) {
	std::reverse(cardNames.begin(), cardNames.end());
	for (auto c : cardNames) {
		if (std::find(minionNames.begin(), minionNames.end(), c) != minionNames.end()) 
				cards.push_back(std::make_unique<Minion>(c));
		else if (std::find(spellNames.begin(), spellNames.end(), c) != spellNames.end()) 
				cards.push_back(std::make_unique<Spell>(c));
		else if (std::find(ritualNames.begin(), ritualNames.end(), c) != ritualNames.end()) 
				cards.push_back(std::make_unique<Ritual>(c));
		else if (std::find(enchantmentNames.begin(), enchantmentNames.end(), c) != enchantmentNames.end()) 
				cards.push_back(std::make_unique<Enchantment>(c));
		else std::cerr << "Could not find card name " + c << std::endl;
	}
}

int Deck::numCards() {
	return cards.size();
}

std::unique_ptr<Card> Deck::drawCard() {
	// check to see if there is a card to draw is done by the Player class
	std::unique_ptr<Card> temp = std::move(cards[cards.size() - 1]);
	cards.erase(cards.begin() + (cards.size() - 1));
	return temp;
}
