/* =============================================
* Assignmet: Project 1 
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: main.cpp
* Description: Program entry point and main game loop.
*     Asks user for game options before passing them
*     to the CDominoesGame class.
* ==============================================
*/

#include <iostream>
#include <string>
#include "CDominoesGame.h"

using namespace std;

// Forward function declarations
GameOptions getGameOptions();
bool getPlayAgain();


int main()
{
	CDominoesGame domGame;
	bool playAgain = true;

	cout << "Welcome to CS380 Dominoes!" << endl;
	cout << "This program was created by Andrew Dunn." << endl << endl;
	system("pause");

	// Main game loop
	while (playAgain)
	{
		GameOptions curRoundOpt = getGameOptions(); // Get player count and player names for next round

		domGame.newGame(curRoundOpt); // Set up new game state

		while (domGame.gameActive())  // Loop until game is over
		{
			domGame.processTurn();      // Process next player's turn
		}

		domGame.showWinMessage();   // Display winner
		playAgain = getPlayAgain(); // Ask user if they want to play again
	}
	
	return 0;
}

// Asks user for the next game's options, such as
// number of players, their names, if they are
// a human player or AI, etc.
GameOptions getGameOptions()
{
	// Prepare GameOptions struct
	GameOptions newOpt;

	// Loop until user inputs a valid player count (2-4)
	while (true)
	{
		system("CLS");

		cout << "Please enter the number of players (2-4): ";

		string ans;
		cin >> ans;

		try
		{
			int num = stoi(ans, nullptr, 10);

			if (num < 2 || num > 4)
			{
				cout << "Error: Enter a number between 2 and 4." << endl << endl;
			}
			else
			{
				newOpt.numPlayers = num;
				break;
			}
		}
		catch (exception ex)
		{
			cout << "Error: Invalid input format." << endl << endl;
		}

		system("pause");
	}

	// Loop through each player and ask for their name,
	// and if they are a human or AI player.
	for (int i = 0; i < newOpt.numPlayers; i++)
	{
		system("CLS");

		string name = "";

		cout << "Please enter the name of Player " << (i + 1) << ": " << flush;

		while (name.length() == 0)
		{
			std::getline(std::cin, name);
		}

		newOpt.playerNames[i] = name;

		// Get AI status
		while (true)
		{
			system("CLS");
			cout << "Is " << name << " a human player (y or n)? ";
			
			string ans = "";
			cin >> ans;

			if (ans == "y" || ans == "Y")
			{
				newOpt.playerIsAI[i] = false;
				break;
			}
			else if (ans == "n" || ans == "N")
			{
				newOpt.playerIsAI[i] = true;
				break;
			}
			else
			{
				cout << "Error: Invalid input." << endl << endl;
				system("pause");
			}
		}
	}

	int startingHand = 0;

	// Loop until user inputs valid starting hand size (6-10 dominoes)
	while (true)
	{
		system("CLS");

		cout << "Please enter the starting hand size (6-10): ";

		string ans;
		cin >> ans;

		try
		{
			int num = stoi(ans, nullptr, 10);

			if (num < 6 || num > 10)
			{
				cout << "Error: Input out of range." << endl << endl;
			}
			else if (num * newOpt.numPlayers > 28)
			{
				cout << "Error: Not enough available pieces for this number of players." << endl << endl;
			}
			else
			{
				startingHand = num;
				break;
			}
		}
		catch (exception ex)
		{
			cout << "Error: Invalid input format." << endl << endl;
		}

		system("pause");
	}

	newOpt.piecesPerPlayer = startingHand;

	// Return selected game options struct
	return newOpt;
}

// Returns true if user wants to play again, otherwise false.
bool getPlayAgain()
{
	while (true)
	{
		string ans;
		cout << endl << "Would you like to play again (Y or N)? ";
		cin >> ans;

		if (ans.length() > 0)
		{
			if (ans == "y" || ans == "Y")
			{
				return true;
			}
			else if (ans == "n" || ans == "N")
			{
				return false;
			}
		}

		cout << endl << "Invalid Input." << endl;
	}
	
}

/* =========================================
*  End of File
*  =========================================
*/