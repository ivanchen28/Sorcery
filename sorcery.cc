#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "sorcery.h"

Sorcery::Sorcery(std::string name1, std::string name2, std::vector<std::string> deck1, std::vector<std::string> deck2, bool testing) : testing(testing) {
    player1 = std::make_unique<Player>(name1, 1, deck1);
    player2 = std::make_unique<Player>(name2, 2, deck2);
    playerNum = 1, turnNum = 1;
    activeP = player1.get();
    inactiveP = player2.get();
	for (int i = 0; i < 4; i++) player1.get()->draw();
	for (int i = 0; i < 4; i++) player2.get()->draw();
    start();
}

// start eventually to be made a private method called only by Sorcery constructor and the end command
void Sorcery::start() {
    std::cout << activeP->getName() << "'s turn!" << std::endl;
    if (activeP->getHandSize() < 5) activeP->draw();
    activeP->gainMana(1);
	activeP->gainActions();
    triggerAbilities(Trigger::Start);
}

// player commands
void Sorcery::attack(int c, int t) {
	if (c > 0 && c <= activeP->getBoardSize()) {
		if (t > 0 && t <= inactiveP->getBoardSize()) {
			if (activeP->checkAction(c)) {
				int atkA = activeP->getAttack(c);
				int atkI = inactiveP->getAttack(t);
				if (atkA >= 0 && atkI >= 0) {
					// successful attack
					activeP->useAction(c);
					bool destroyedA = activeP->takeDamage(c, atkI);
					bool destroyedI = inactiveP->takeDamage(t, atkA);
					if (destroyedA) if (activeP->getMinion(c)->getTrigger() == Trigger::Destroy) triggerDestruction(activeP->getMinion(c), activeP);
					if (destroyedI) if (inactiveP->getMinion(t)->getTrigger() == Trigger::Destroy) triggerDestruction(inactiveP->getMinion(t), inactiveP);
					destroyMinions();
				}
			}
			else std::cerr << "This minion does not have an action!" << std::endl;
		}
		else if (inactiveP->getBoardSize() == 0) std::cerr << "Your opponent controls no minions!" << std::endl;
		else std::cerr << "Select a target between 1 and " << inactiveP->getBoardSize() << "!" << std::endl;
	}
	else if (activeP->getBoardSize() == 0) std::cerr << "You control no minions!" << std::endl;
	else std::cerr << "Select a minion between 1 and " << activeP->getBoardSize() << "!" << std::endl;
}

void Sorcery::attackDirect(int c) {
	if (c > 0 && c <= activeP->getBoardSize()) {
		if (activeP->checkAction(c)) {
			int atk = activeP->getAttack(c);
			if (atk >= 0) {
				// successful attack
				activeP->useAction(c);
				bool dead = inactiveP->loseHealth(atk);
				if (dead) {
					std::cout << activeP->getName() << " wins!" << std::endl;
					winner = true;
				}
			}
		}
		else std::cerr << "This minion does not have an action available!" << std::endl;
	}
	else if (activeP->getBoardSize() == 0) std::cerr << "You control no minions!" << std::endl;
	else std::cerr << "Select a minion between 1 and " << activeP->getBoardSize() << "!" << std::endl;
}

void Sorcery::board() {
	std::vector<std::string> temp;

	// print player 1's board
	temp = player1->printBoard();
	std::cout << EXTERNAL_BORDER_CHAR_TOP_LEFT;
	for (int i = 0; i < 165; i++) std::cout << EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
	std::cout << EXTERNAL_BORDER_CHAR_TOP_RIGHT << std::endl;

	// print center graphic
	for (size_t i = 0; i < temp.size(); i++) std::cout << EXTERNAL_BORDER_CHAR_UP_DOWN << temp[i] << EXTERNAL_BORDER_CHAR_UP_DOWN << std::endl;
	for (size_t i = 0; i < CENTRE_GRAPHIC.size(); i++) std::cout << CENTRE_GRAPHIC[i] << std::endl;

	// print player 2's board
	temp = player2->printBoard();
	for (size_t i = 0; i < temp.size(); i++) std::cout << EXTERNAL_BORDER_CHAR_UP_DOWN << temp[i] << EXTERNAL_BORDER_CHAR_UP_DOWN << std::endl;
	std::cout << EXTERNAL_BORDER_CHAR_BOTTOM_LEFT;
	for (int i = 0; i < 165; i++) std::cout << EXTERNAL_BORDER_CHAR_LEFT_RIGHT;
	std::cout << EXTERNAL_BORDER_CHAR_BOTTOM_RIGHT << std::endl;
}

void Sorcery::end() {
    triggerAbilities(Trigger::End);

	// switching of players
    if (playerNum == 1) {
        playerNum = 2;
        activeP = player2.get();
        inactiveP = player1.get();
    }
    else if (playerNum == 2) {
        playerNum = 1;
        activeP = player1.get();
        inactiveP = player2.get();
    }
    turnNum++;
    start();
}

void Sorcery::hand() {
    std::vector<std::string> temp;
	temp = activeP->printHand();
	for (size_t i = 0; i < temp.size(); i++) std::cout << temp[i];
	std::cout << std::endl;
}

void Sorcery::help() {
    std::cout << "Commands: help -- Display this message." << std::endl;
    std::cout << "			end -- End the current player’s turn." << std::endl;
    std::cout << "			quit -- End the game." << std::endl;
    std::cout << "			attack minion other-minion -- Orders minion to attack other-minion." << std::endl;
    std::cout << "			attack minion -- Orders minion to attack the opponent." << std::endl;
    std::cout << "			play card [target-player target-card] -- Play card, optionally targeting target-card owned by target-player." << std::endl;
    std::cout << "			use minion [target-player target-card] -- Use minion’s special ability, optionally targeting target-card owned by target-player." << std::endl;
    std::cout << "			inspect minion -- View a minion’s card and all enchantments on that minion." << std::endl;
    std::cout << "			hand -- Describe all cards in your hand." << std::endl;
    std::cout << "			board -- Describe all cards on the board." << std::endl;
}

void Sorcery::inspect(int c) {
	if (c > 0 && c <= activeP->getBoardSize()) activeP->printMinion(c);
	else if (activeP->getBoardSize() == 0) std::cerr << "You control no minions!" << std::endl;
	else std::cerr << "Select a minion between 1 and " << activeP->getBoardSize() << "!" << std::endl;
}

// playing minions, rituals, and spells that don't target
void Sorcery::play(int c) {
	CardType ct = activeP->getCardType(c);
	if (ct == CardType::Enchantment || ct == CardType::SpellTarget) {
		std::cerr << "This card requires a target to be played!" << std::endl;
		return;
	}
	if (activeP->getMana() < activeP->getCost(c) && !testing) {
		std::cerr << "You do not have enough mana!" << std::endl;
		return;
	}
	if (c > 0 && c <= activeP->getHandSize()) {
		if (ct== CardType::Minion || ct == CardType::MinionActivated) {
			if (activeP->getBoardSize() < 5) {
				activeP->useMana(activeP->getCost(c));
				activeP->play(c, ct);
				triggerAbilities(Trigger::Summon);
				destroyMinions();
			}
			else std::cerr << "Your board is full!" << std::endl;
		}
		else if (ct == CardType::SpellNormal) {
			// activate non-targeting spell
			std::string cardName = activeP->getCardName(c);
			if (cardName == std::string("Blizzard")) {
				for (int i = 1; i <= activeP->getBoardSize(); i++) activeP->takeDamage(i, 2);
				for (int i = 1; i <= inactiveP->getBoardSize(); i++) activeP->takeDamage(i, 2);
				// separate so that the blizzard finishes first before cards start being destroyed
				for (int i = 1; i <= activeP->getBoardSize(); i++) {
					if (activeP->takeDamage(i, 0)) if (activeP->getMinion(i)->getTrigger() == Trigger::Destroy) triggerDestruction(activeP->getMinion(i), activeP);
				}
				for (int i = 1; i <= inactiveP->getBoardSize(); i++) {
					if (inactiveP->takeDamage(i, 0)) if (inactiveP->getMinion(i)->getTrigger() == Trigger::Destroy) triggerDestruction(inactiveP->getMinion(i), inactiveP);
				}
				destroyMinions();
			}
			else if (cardName == std::string("Recharge")) {
				if (activeP->hasRitual()) {
					activeP->getRitual()->recharge(3);
				}
				else {
					std::cerr << "You do not have a ritual!" << std::endl;
					return;
				}
			}
			else if (cardName == std::string("Raise Dead")) {
				if (activeP->getBoardSize() < 5) {
					if (activeP->hasDead()) {
						activeP->reviveMinion();
						triggerAbilities(Trigger::Summon);
						destroyMinions();
					}
					else std::cerr << "Your graveyard is empty!" << std::endl;
					return;
				}
				else {
					std::cerr << "Your board is full!" << std::endl;
					return;
				}
			}
			// add other minion targeting spells here
			activeP->useMana(activeP->getCost(c));
			activeP->discard(c);
		}
		else if (ct == CardType::Ritual) {
			// play ritual
			activeP->useMana(activeP->getCost(c));
			activeP->play(c, ct);
		}
	}
    else if (activeP->getHandSize() == 0) std::cerr << "Your hand is empty!" << std::endl;
	else std::cerr << "Select a card between 1 and " << activeP->getHandSize() << "!" << std::endl;
}

// playing spells that target and enchantments
void Sorcery::playTarget(int c, int p, char t) {
	t = t - 48;
	CardType ct = activeP->getCardType(c);
	if (ct != CardType::Enchantment && ct != CardType::SpellTarget) {
		std::cerr << "This card cannot be played with a target!" << std::endl;
		return;
	}
	if (activeP->getMana() < activeP->getCost(c) && !testing) {
		std::cerr << "You do not have enough mana!" << std::endl;
		return;
	}
	if (c > 0 && c <= activeP->getHandSize()) {
		// check for player
		Player *targetPlayer = nullptr;
		if (p != 1 && p != 2) {
			std::cerr << "Select player 1 or 2!" << std::endl;
			return;
		}
		else if (p == 1) targetPlayer = player1.get();
		else if (p == 2) targetPlayer = player2.get();

		// check target exists
		if (targetPlayer->getBoardSize() == 0 && !targetPlayer->hasRitual()) std::cerr << "Target player's board is empty!" << std::endl;
		else if (t == 'r' - 48) {
			if (targetPlayer->hasRitual()) {
				std::string cardName = activeP->getCardName(c);
				if (ct == CardType::Enchantment) std::cerr << "Rituals cannot be enchanted!" << std::endl;
				else {
					// activate spell to target ritual
					if (cardName == std::string("Banish")) targetPlayer->destroyRitual();
					// add other ritual targeting spells here
					else {
						std::cerr << "This card cannot target rituals!" << std::endl;
						return;
					}
					activeP->useMana(activeP->getCost(c));
					activeP->discard(c);
				}
			}
			else std::cerr << "Target player has no ritual!" << std::endl;
		}
		else if (targetPlayer->getBoardSize() == 0) std::cerr << "Target player controls no minions!" << std::endl;
		else if (t > 0 && t <= targetPlayer->getBoardSize()) {
			if (ct == CardType::SpellTarget) {
				// activate spell to target minion
				std::string cardName = activeP->getCardName(c);
				if (cardName == std::string("Unsummon")) {
					if (targetPlayer->getHandSize() < 5) targetPlayer->returnMinion(t);
					else {
						std::cerr << "Target player's hand is full!" << std::endl;
						return;
					}
				}
				else if (cardName == std::string("Banish")) {
					if (targetPlayer->getMinion(t)->getTrigger() == Trigger::Destroy) triggerDestruction(targetPlayer->getMinion(t), targetPlayer);
					targetPlayer->destroyMinion(t);
					destroyMinions();
				}
				else if (cardName == std::string("Disenchant")) {
					if (targetPlayer->hasEnchantments(t)) {
						targetPlayer->removeEnchantment(t);
						// check if the enchantment removal destroyed the minion by dealing 0 damage to it
						if (targetPlayer->takeDamage(t, 0)) {
							if (targetPlayer->getMinion(t)->getTrigger() == Trigger::Destroy) triggerDestruction(targetPlayer->getMinion(t), targetPlayer);
							destroyMinions();
						}
					}
					else {
						std::cerr << "This minion does not have an enchantment!" << std::endl;
						return;
					}
				}
				// add other minion targeting spells here
				else {
					std::cerr << "This card cannot target minions!" << std::endl;
					return;
				}
				activeP->useMana(activeP->getCost(c));
				activeP->discard(c);
			}
			else if (ct == CardType::Enchantment) {
				// enchant minion
				activeP->useMana(activeP->getCost(c));
				auto ench = activeP->getEnchantment(c);
				targetPlayer->addEnchantment(std::move(ench), t);
				// check if the enchantment destroyed the minion by dealing 0 damage to it
				if (targetPlayer->takeDamage(t, 0)) {
					if (targetPlayer->getMinion(t)->getTrigger() == Trigger::Destroy) triggerDestruction(targetPlayer->getMinion(t), targetPlayer);
					destroyMinions();
				}
			}
		}
		else std::cerr << "Select a target between 1 and " << targetPlayer->getBoardSize() << (targetPlayer->hasRitual() ? " or r!" : "!") << std::endl;
	}
	else if (activeP->getHandSize() == 0) std::cerr << "Your hand is empty!" << std::endl;
	else std::cerr << "Select a card between 1 and " << activeP->getHandSize() << "!" << std::endl;
}

void Sorcery::use(int c) {
	if (c > 0 && c <= activeP->getBoardSize()) {
		if (activeP->checkAction(c)) {
			if (activeP->getMana() >= activeP->getMinion(c)->getEffectCost() || testing) {
				std::string minionName = activeP->getMinion(c)->getName();
				if (minionName == std::string("Apprentice Summoner")) {
					if (activeP->getBoardSize() < 5) {
						activeP->useAction(c);
						activeP->useMana(activeP->getMinion(c)->getEffectCost());
						activeP->summon("Air Elemental");
						triggerAbilities(Trigger::Summon);
						destroyMinions();
					}
					else std::cerr << "Your board is full!" << std::endl;
				}
				else if (minionName == std::string("Master Summoner")) {
					if (activeP->getBoardSize() < 5) {
						activeP->useAction(c);
						activeP->useMana(activeP->getMinion(c)->getEffectCost());
						int count = 0;
						while (count < 3 && activeP->getBoardSize() < 5) {
							activeP->summon("Air Elemental");
							triggerAbilities(Trigger::Summon);
							destroyMinions();
							count++;
						}
					}
					else std::cerr << "Your board is full!" << std::endl;
				}
				else std::cerr << "This minion's effect requires a target!" << std::endl;
			}
			else std::cerr << "You do not have enough mana!" << std::endl;
			// add other non-targeting minion effects here
		}
		else std::cerr << "This minion does not have an action!" << std::endl;
	}
	else if (activeP->getBoardSize() == 0) std::cerr << "You control no minions!" << std::endl;
	else std::cerr << "Select a minion between 1 and " << activeP->getBoardSize() << "!" << std::endl;
}


void Sorcery::useTarget(int c, int p, char t) {
	if (c > 0 && c <= activeP->getBoardSize()) {
    	// activeP->use(c);
		// other effects?
		// check for player
		Player *targetPlayer = nullptr;
		if (p != 1 && p != 2) {
			std::cerr << "Select player 1 or 2!" << std::endl;
			return;
		}
		else if (p == 1) targetPlayer = player1.get();
		else if (p == 2) targetPlayer = player2.get();
		// check target exists
		if (targetPlayer->getBoardSize() == 0 && !targetPlayer->hasRitual()) std::cerr << "Target player's board is empty!" << std::endl;
		else if (t == 'r') {
			if (targetPlayer->hasRitual()) {
				// target ritual, but there are currently no effects that target ritual
				std::cerr << "This minion's effect cannot target rituals!" << std::endl;
			}
			else std::cerr << "Target player has no ritual!" << std::endl;
		}
		t = t - 48;
		if (targetPlayer->getBoardSize() == 0) std::cerr << "Target player controls no minions!" << std::endl;
		else if (t > 0 && t <= targetPlayer->getBoardSize()) {
			if (activeP->checkAction(c)) {
				if (activeP->getMana() >= activeP->getMinion(c)->getEffectCost() || testing) {
					std::string minionName = activeP->getMinion(c)->getName();
					if (minionName == std::string("Novice Pyromancer")) {
						activeP->useAction(c);
						if (targetPlayer->takeDamage(t, 1)) {
							activeP->useMana(activeP->getMinion(c)->getEffectCost());
							if (targetPlayer->getMinion(t)->getTrigger() == Trigger::Destroy) triggerDestruction(targetPlayer->getMinion(t), targetPlayer);
							destroyMinions();
						}
					}
					// add other targeting minion effects here
					else std::cerr << "This minion's effect does not target!" << std::endl;
				}
				else std::cerr << "You do not have enough mana!" << std::endl;
			}
			else std::cerr << "This minion does not have an action!" << std::endl;
		}
		else std::cerr << "Select a target between 1 and " << targetPlayer->getBoardSize() << (activeP->hasRitual() ? " or r!" : "!") << std::endl;
	}
	else if (activeP->getBoardSize() == 0) std::cerr << "You control no minions!" << std::endl;
	else std::cerr << "Select a minion between 1 and " << activeP->getBoardSize() << "!" << std::endl;
}

// -testing only commands
void Sorcery::draw() {
	if (5 > activeP->getHandSize()) {
		activeP->draw();
	}
	else std::cerr << "Your hand is full!" << std::endl;
}

void Sorcery::discard(int c) {
	if (c > 0 && c <= activeP->getHandSize()) activeP->discard(c);
	else if (activeP->getBoardSize() == 0) std::cerr << "Your hand is empty!" << std::endl;
	else std::cerr << "Select a card between 1 and " << activeP->getBoardSize() << "!" << std::endl;
}

bool Sorcery::hasWinner() {
	return winner;
}

void Sorcery::triggerDestruction(Minion *destMinion, Player *owner) {
	std::string name = destMinion->getName();
	Player *otherPlayer = nullptr; // not the player who controls the minion that is destroyed
	if (owner == player1.get()) otherPlayer = player2.get();
	else if (owner == player2.get()) otherPlayer = player1.get();
	else {
		std::cerr << "System error: owner of destroyed minion not found" << std::endl;
		return;
	}
	if (name == std::string("Bomb") && destMinion->getDestroyTrigger()) {
		for (int i = 1; i <= otherPlayer->getBoardSize(); i++) {
			otherPlayer->takeDamage(i, destMinion->getAttack());
			destMinion->setDestroyTrigger(false);
		}
		// separate so that the effect finishes first and that card is destroyed before checking for other effects
		for (int i = 1; i <= otherPlayer->getBoardSize(); i++) {
			if (otherPlayer->takeDamage(i, 0)) {
				// if any other cards are destroyed with destruction effects, their effects need to be triggered too
				if (otherPlayer->getMinion(i)->getTrigger() == Trigger::Destroy) triggerDestruction(otherPlayer->getMinion(i), otherPlayer);
			}
		}
	}
	// add other triggered on destruction minion effects here
}

// destroys all minions with 0 health or less on the board, after all effects have been activated
void Sorcery::destroyMinions() {
	int activeSize = activeP->getBoardSize();
	for (int i = 1; i <= activeSize; i++) {
		activeP->getMinion(i)->setDestroyTrigger(true);
		if (activeP->takeDamage(i, 0)) {
			activeP->destroyMinion(i);
			i--;
			activeSize--;
			// one minion is now gone so all minions behind it move up a slot
		}
	}
	int inactiveSize = inactiveP->getBoardSize();
	for (int i = 1; i <= inactiveSize; i++) {
		inactiveP->getMinion(i)->setDestroyTrigger(true);
		if (inactiveP->takeDamage(i, 0)) {
			inactiveP->destroyMinion(i);
			i--;
			inactiveSize--;
			// one minion is now gone so all minions behind it move up a slot
		}
	}
}

void Sorcery::triggerAbilities(Trigger type) {
	for (int i = 1; i <= activeP->getBoardSize(); i++) {
		if (activeP->getMinion(i)->getTrigger() == type) activateAbility(activeP->getMinion(i)->getName(), activeP);
	}
	if (activeP->getRitual()) {
		if (activeP->getRitual()->getTrigger() == type && activeP->getRitual()->hasCharge()) {
			activateAbility(activeP->getRitual()->getName(), activeP);
		}
	}
	for (int i = 1; i <= inactiveP->getBoardSize(); i++) {
		if (inactiveP->getMinion(i)->getTrigger() == type) activateAbility(inactiveP->getMinion(i)->getName(), inactiveP);
	}
	if (inactiveP->getRitual()) {
		if (inactiveP->getRitual()->getTrigger() == type && inactiveP->getRitual()->hasCharge()) {
			activateAbility(inactiveP->getRitual()->getName(), inactiveP);
		}
	}
}

void Sorcery::activateAbility(std::string cardName, Player *owner) {
	// owner is owner of the card whose ability activated
	// if a card is summoned it will automatically be the last minion in line
	if (cardName == std::string("Dark Ritual")) {
		if (owner == activeP) {
			activeP->gainMana(1);
			activeP->getRitual()->useCharge();
		}
	}
	else if (cardName == std::string("Aura of Power")) {
		if (owner == activeP) {
			Minion * summoned = activeP->getMinion(activeP->getBoardSize());
			summoned->boostAttack(1);
			summoned->heal(1);
			activeP->getRitual()->useCharge();
		}
	}
	else if (cardName == std::string("Standstill")) {
		Minion * summoned = activeP->getMinion(activeP->getBoardSize());
		if (summoned->getDefence() > 0) {
			activeP->takeDamage(activeP->getBoardSize(), summoned->getDefence());
			if (summoned->getTrigger() == Trigger::Destroy) triggerDestruction(summoned, activeP);
			owner->getRitual()->useCharge();
		}
	}
	else if (cardName == std::string("Fire Elemental")) {
		Minion * summoned = activeP->getMinion(activeP->getBoardSize());
		if (owner == inactiveP && summoned->getDefence() > 0) {
			if (activeP->takeDamage(activeP->getBoardSize(), 1)) {
				if (summoned->getTrigger() == Trigger::Destroy) triggerDestruction(summoned, activeP);
			}
		}
	}
	if (cardName == std::string("Potion Seller")) {
		if (owner == activeP) for (int i = 1; i <= activeP->getBoardSize(); i++) activeP->getMinion(i)->heal(1);
	}
}
