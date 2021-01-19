#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "ascii_graphics.h"
#include "board.h"

Board::Board() {
}

int Board::numMinions() {
	return minions.size();
}

void Board::addMinion(std::unique_ptr<Minion> m) {
	// check to see if there is enough space is done by the Player class
	minions.push_back(std::move(m));
}

Minion * Board::getMinion(int i) {
	// check to see if minion is valid is done by the Player class
	return minions[i - 1].get();
}

void Board::gainActions() {
	for (size_t i = 0; i < minions.size(); i++) minions[i].get()->gainAction();
}

std::unique_ptr<Minion> Board::removeMinion(int i) {
	// check to see if minion is valid is done by the Player class
	std::unique_ptr<Minion> temp = std::move(minions[i - 1]);
	temp.get()->resetBoost();
	minions.erase(minions.begin() + (i - 1));
	return temp;
}

void Board::destroyRitual() {
	ritual = nullptr;
}

void Board::addRitual(std::unique_ptr<Ritual> r) {
	ritual = std::move(r);
}

Ritual * Board::getRitual() {
	if (ritual) return ritual.get();
	else return nullptr;
}

std::vector<std::string> Board::printBoard(int playerNum, std::string name, int life, int mana, std::vector<std::string> graveyard) {
	std::vector<std::string> minionRow(11, "");
	for (size_t i = 0; i < 5; i++) {
		if (i < minions.size()) minionRow = combine_templates(minionRow, minions[i].get()->print());
		else minionRow = combine_templates(minionRow, CARD_TEMPLATE_BORDER);
	}
	std::vector<std::string> ritualRow(11, "");
	if (ritual) ritualRow = combine_templates(ritualRow, ritual.get()->print());
	else ritualRow = combine_templates(ritualRow, CARD_TEMPLATE_BORDER);
	ritualRow = combine_templates(ritualRow, CARD_TEMPLATE_EMPTY);
	ritualRow = combine_templates(ritualRow, display_player_card(playerNum, name, life, mana));
	ritualRow = combine_templates(ritualRow, CARD_TEMPLATE_EMPTY);
	ritualRow = combine_templates(ritualRow, graveyard);
	
	std::vector<std::string> out;
	if (playerNum == 1) {
		out = ritualRow;
		out.insert(out.end(), minionRow.begin(), minionRow.end());
	}
	else if (playerNum == 2) {
		out = minionRow;
		out.insert(out.end(), ritualRow.begin(), ritualRow.end());
	}
	return out;
}

std::vector<std::string> Board::printMinion(int i) {
	// check to see if minion is valid is done by the Player class
	return minions[i - 1].get()->printEnchantments();
}
