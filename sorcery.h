#ifndef SORCERY_H
#define SORCERY_H 1

#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "player.h"

class Sorcery {
    std::unique_ptr<Player> player1, player2;
    Player *activeP, *inactiveP;
    int playerNum;      // current player, 1 or 2
    int turnNum;        // turn number, could eventually be used for extra cards/features
	bool winner = false;
	bool testing;

    public:
    Sorcery(std::string name1, std::string name2, std::vector<std::string> deck1, std::vector<std::string> deck2, bool testing);
    void start();
    void attack(int c, int t);
    void attackDirect(int c);
    void board();
    void end();
    void hand();
    void help();
    void inspect(int c);
    void play(int c);
	void playTarget(int c, int p, char t);
    void use(int c);
	void useTarget(int c, int p, char t);
    void discard(int c);
    void draw();
	bool hasWinner();

	void triggerDestruction(Minion *destMinion, Player *owner);
	void triggerAbilities(Trigger type);
	void activateAbility(std::string cardName, Player *owner);
	void destroyMinions();
};

#endif
