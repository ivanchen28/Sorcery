#ifndef PLAYER_H
#define PLAYER_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "deck.h"
#include "board.h"
#include "graveyard.h"
#include "hand.h"
#include "ascii_graphics.h"

class Player {
	// player values
    std::string name;
	int playerNum;
	int life;
	int mana;

	// card holders
	Deck deck;
	Hand hand;
	Board board;
	Graveyard graveyard;
    
    public:
    Player(std::string name, int playerNum, std::vector<std::string> deckList);

	std::string getName(); // player name
	bool checkAction(int c);
	int getAttack(int c);
	int getBoardSize();
	CardType getCardType(int c);
	int getCost(int c);
	std::string getCardName(int c);
	int getHandSize();
	int getMana();
	Minion* getMinion(int c);
	Ritual* getRitual();
	bool hasEnchantments(int c);
	bool hasRitual();
	bool hasDead();
	
	void gainMana(int m);
	void useMana(int m);
	void gainActions();
	void useAction(int c);
	bool loseHealth(int lp);
	void healMinion(int c, int dmg);
	bool takeDamage(int c, int dmg); // minion damage, doubles as a check to see if it should be destroyed
	void removeEnchantment(int c);
	void returnMinion(int c);
	void destroyMinion(int c);
	void destroyRitual();
	void reviveMinion();
	void summon(std::string name);

	void draw();
	void discard(int c);
	void play(int c, CardType ct);
	std::unique_ptr<Enchantment> getEnchantment(int c);
	void addEnchantment(std::unique_ptr<Enchantment> ench, int c);
	
	std::vector<std::string> printHand();
	std::vector<std::string> printBoard();
	void printMinion(int c);
};

#endif
