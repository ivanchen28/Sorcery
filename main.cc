#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "sorcery.h"

int main(int argc, char **argv) {
    // handle arguments
    std::ifstream initFile;
    bool hasFile = false;
    bool testing = false;
    std::string deck1Name = "default.deck", deck2Name = "default.deck";
	std::string init("-init");
	std::string d1("-deck1");
	std::string d2("-deck2");
	std::string test("-testing");
    
    // handle errors in case of command-line argument error
    for (int i = 1; i < argc; i++) {
        if (argv[i] == init) {
            i++;
            // replace with try/catch block
            initFile = std::ifstream(argv[i]);
            if (initFile.fail()) {
                std::cerr << "Cannot open -init file." << std::endl;
                exit(1);
            }
            hasFile = true;
        }
        else if (argv[i] == d1) {
            i++;
            deck1Name = argv[i];
        }
        else if (argv[i] == d2) {
            i++;
            deck2Name = argv[i];
        }
        else if (argv[i] == test) testing = true;
    }

    // load player decks into vectors of strings
    std::ifstream deck1File = std::ifstream(deck1Name), deck2File = std::ifstream(deck2Name);
    std::vector<std::string> deck1, deck2;
    if (deck1File.fail() || deck2File.fail()) {
        std::cerr << "Cannot open deck file." << std::endl;
        exit(1);
    }
    std::string cardname;
    while (std::getline(deck1File, cardname)) deck1.push_back(cardname);
    while (std::getline(deck2File, cardname)) deck2.push_back(cardname);

	if (!testing) {
		unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle (deck1.begin(), deck1.end(), std::default_random_engine(seed1));
		unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count() + 69;
		shuffle (deck2.begin(), deck2.end(), std::default_random_engine(seed2));
	}

    // prompt for names & create Sorcery object
    std::string p1Name, p2Name;
    if (hasFile && !(initFile.eof())) std::getline(initFile, p1Name);
    else {
        std::cout << "Player 1's name?" << std::endl;
        std::getline(std::cin, p1Name);
    }
    if (hasFile && !(initFile.eof())) std::getline(initFile, p2Name);
    else {
        std::cout << "Player 2's name?" << std::endl;
        std::getline(std::cin, p2Name);
    }
    Sorcery sorcery = Sorcery(p1Name, p2Name, deck1, deck2, testing);
    

    // handle inputs
    bool quit = false;
    std::string s;
    while ((!quit && !sorcery.hasWinner()) && std::getline(hasFile ? initFile : std::cin, s)) {
        std::istringstream iss{s};
        std::string cmd;
        iss >> cmd;
        if (cmd == "attack") {
            int c, t;
            if (iss >> c) {
				if (iss >> t) sorcery.attack(c, t);
				else sorcery.attackDirect(c);
			}
			else std::cerr << "Cards and targets are selected by number!" << std::endl;
        }
        else if (cmd == "board") {
            sorcery.board();
        }
        else if (cmd == "end") {
            sorcery.end();
        }
        else if (cmd == "hand") {
            sorcery.hand();
        }
		else if (cmd == "help") {
			sorcery.help();
		}
        else if (cmd == "inspect") {
            int c;
            if (iss >> c) sorcery.inspect(c);
			else std::cerr << "Cards and targets are selected by number!" << std::endl;
        }
        else if (cmd == "play") {
            int c, p;
			char t;
           	if (iss >> c) {
				if (iss >> p && iss >> t) sorcery.playTarget(c, p, t);
				else sorcery.play(c);
			}
			else std::cerr << "Cards and targets are selected by number!" << std::endl;
        }
        else if (cmd == "use") {
            int c, p;
			char t;
            if (iss >> c) {
				if (iss >> p && iss >> t) sorcery.useTarget(c, p, t);
				else sorcery.use(c);
			}
			else std::cerr << "Cards and targets are selected by number!" << std::endl;
        }
		else if (cmd == "discard" && testing) {
            int c;
            if (iss >> c) sorcery.discard(c);
			else std::cerr << "Cards and targets are selected by number!" << std::endl;
        }
		else if (cmd == "draw" && testing) {
            sorcery.draw();
        }
        else if (cmd == "quit") quit = true;
        else if (cmd != "") {
            std::cerr << "Unrecognized command: " << cmd << "!" << std::endl;
        }

        // check if end of initFile
       	if (initFile.eof()) hasFile = false;
    }
    return 0;
}
