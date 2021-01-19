#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "player.h"

template <typename SOURCE_TYPE, typename DEST_TYPE>
bool dynamic_unique_cast(std::unique_ptr<DEST_TYPE> & dest, std::unique_ptr<SOURCE_TYPE> & source) {
    if (!source) {
        dest.reset();
        return true;
    }
    DEST_TYPE * dest_ptr = dynamic_cast<DEST_TYPE *>(source.get());
    if (!dest_ptr) return false;
    source.release();
    dest.reset(dest_ptr);
    return true;
}

Player::Player(std::string name, int playerNum, std::vector<std::string> deckList) : name(name), playerNum(playerNum) {
    deck.initializeDeck(deckList);
	life = 20;
	mana = 3;
}

bool Player::checkAction(int c) {
	return board.getMinion(c)->hasAction();
}
int Player::getAttack(int c) {
	return board.getMinion(c)->getAttack();
}
int Player::getBoardSize() {
	return (int) board.numMinions();
}
CardType Player::getCardType(int c) {
	return hand.getCardType(c);
}
int Player::getCost(int c) {
	return hand.getCard(c)->getCost();
}
std::string Player::getCardName(int c) {
	return hand.getCard(c)->getName();
}
Minion * Player::getMinion(int c) {
	return board.getMinion(c);
}
Ritual * Player::getRitual() {
	return board.getRitual();
}
int Player::getHandSize() {
	return (int) hand.numCards();
}
std::string Player::getName() {
	return name;
}
bool Player::hasEnchantments(int c) {
	return board.getMinion(c)->hasEnchantments();
}
bool Player::hasRitual() {
	if (board.getRitual()) return true;
	return false;
}
bool Player::hasDead() {
	if (graveyard.numMinions() > 0) return true;
	else return false;
}

void Player::gainMana(int m) {
	mana += m;
}

int Player::getMana() {
	return mana;
}

void Player::useMana(int m) {
	mana -= m;
	if (mana < 0) mana = 0;
}

void Player::gainActions() {
	board.gainActions();
}

void Player::useAction(int c) {
	board.getMinion(c)->useAction();
}

bool Player::loseHealth(int lp) {
	life -= lp;
	if (life <= 0) return true; // player loses
	else return false; // player is still alive
}

void Player::healMinion(int c, int dmg) {
	board.getMinion(c)->heal(dmg);
}

bool Player::takeDamage(int c, int dmg) {
	return board.getMinion(c)->takeDamage(dmg); // returns true if minion is destroyed, false otherwise; used for destroy trigger
}

void Player::removeEnchantment(int c) {
	board.getMinion(c)->removeEnchantment(false);
}

void Player::returnMinion(int c) {
	board.getMinion(c)->removeEnchantment(true);
	hand.drawCard(std::move(board.removeMinion(c)));
}

void Player::destroyMinion(int c) {
	board.getMinion(c)->removeEnchantment(true);
	graveyard.addMinion(std::move(board.removeMinion(c)));
}

void Player::destroyRitual() {
	board.destroyRitual();
}

void Player::reviveMinion() {
	board.addMinion(std::move(graveyard.reviveTop()));
	board.getMinion(getBoardSize())->setBaseDefence(1);
}

void Player::summon(std::string name) {
	board.addMinion(std::make_unique<Minion>(name));
}

void Player::draw() {
	hand.drawCard(deck.drawCard());
}

void Player::discard(int c) {
	hand.deleteCard(c);
}

// enum class CardType{Minion, MinionActivated, SpellNormal, SpellTarget, Ritual, Enchantment, Null};
void Player::play(int c, CardType ct) {
	if (ct == CardType::Minion || ct == CardType::MinionActivated) {
		auto mcast = std::make_unique<Minion>("");
		auto ccast = hand.moveCard(c);
		// need to cast Card type into Minion type to place on board
		if (dynamic_unique_cast(mcast, ccast)) board.addMinion(std::move(mcast));
		else std::cerr << "System error: Minion cast" << std::endl;
	}
	else if (ct == CardType::Ritual) {
		auto rcast = std::make_unique<Ritual>("");
		auto ccast = hand.moveCard(c);
		// need to cast Card type into Ritual type to place on board
		if (dynamic_unique_cast(rcast, ccast)) board.addRitual(std::move(rcast));
		else std::cerr << "System error: Ritual cast" << std::endl;
	}
}

std::unique_ptr<Enchantment> Player::getEnchantment(int c) {
	auto ecast = std::make_unique<Enchantment>("");
	auto ccast = hand.moveCard(c);
	if (dynamic_unique_cast(ecast, ccast)) return std::move(ecast);
	else std::cerr << "System error: Enchantment cast" << std::endl;
	return nullptr;
}

void Player::addEnchantment(std::unique_ptr<Enchantment> ench, int c) {
	board.getMinion(c)->addEnchantment(std::move(ench));
}

std::vector<std::string> Player::printHand() {
	return hand.printHand();
}

std::vector<std::string> Player::printBoard() {
	return board.printBoard(playerNum, name, life, mana, graveyard.printTop());
}

void Player::printMinion(int c) {
	std::vector<std::string> temp;
	temp = board.printMinion(c);
	for (size_t i = 0; i < temp.size(); i++) std::cout << temp[i] << std::endl;
}
