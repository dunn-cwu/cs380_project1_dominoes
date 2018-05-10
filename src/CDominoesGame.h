/* =============================================
* Assignmet: Project 1
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CDominoesGame.h
* Description: Header file for GameOptions struct
*     and CDominoesGame class. Represents the
*     Domino game. Encompasses table, Boneyard
*     pile, and game players. Manages turn logic
*     and win state. Sets up initial game state
*     and player classes.
* ==============================================
*/

#pragma once

#include <string>
#include "CDominoCollection.h"
#include "CPlayer.h"

struct GameOptions
{
public:
	int numPlayers;
	int piecesPerPlayer;
	std::string playerNames[4];
	bool playerIsAI[4];

	GameOptions();
};

class CDominoesGame
{
private:
	CDominoCollection table; // Game table where pieces are played
	CDominoCollection pile;  // Boneyard pile that contains extra pieces
	CPlayer* playerArr;      // Array of active players
	int numPlayers;          // Number of active players
	int curPlayerTurn;       // Index of current player
	int sequentialPasses;    // Used to determine if no players can play pieces
	bool isGameActive;       // True if a game is in session
	int winner;              // Index of winning player, or -1 if no player has won

	bool initPlayerArr(const GameOptions &opt);
	bool checkGameOver();
	void advPlayerTurn();
	void playerTurnSuccess();
	void playerTurnPassed();

public:
	CDominoesGame();
	~CDominoesGame();

	std::string getWinner();
	std::string getCurrentPlayer();
	void clearState();
	void newGame(const GameOptions &opt);
	bool gameActive();
	void processTurn();
	void displayGameBoard();
	void showWinMessage();
};

/* =========================================
*  End of File
*  =========================================
*/