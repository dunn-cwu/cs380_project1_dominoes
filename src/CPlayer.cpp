/* =============================================
* Assignmet: Project 1
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CPlayer.cpp
* Description: Implementation file for the CPlayer class.
*     Stores player information and the dominoes
*     in their hand. Manages logic for playing
*     and drawing pieces. Manages player menu and
*     AI logic.
* ==============================================
*/

#include <cstdlib>
#include <iostream>
#include "CPlayer.h"

using namespace std;

// Default constructor
CPlayer::CPlayer()
{
	this->name = "No Name";
	this->isAI = false;
	this->stat_PiecesPlayed = 0;
	this->stat_PiecesDrawn = 0;
	this->hand = CDominoCollection();
}

// Constructor that takes name and isAI args
CPlayer::CPlayer(std::string name, bool isAI)
{
	this->name = name;
	this->isAI = isAI;
	this->stat_PiecesPlayed = 0;
	this->stat_PiecesDrawn = 0;
	this->hand = CDominoCollection();
}

// Destructor. Clears domino pieces in
// player's hand collection
CPlayer::~CPlayer()
{
	hand.clear();
}

// Returns name of player
std::string CPlayer::getName()
{
	return name;
}

// Returns true of the player has no domino pieces
// in his hand. Otherwise false.
bool CPlayer::isHandEmpty()
{
	return (hand.getSize() == 0);


}

// Returns number of pieces player has played
// in the current game
int CPlayer::getPiecesPlayed()
{
	return stat_PiecesPlayed;
}

// Returns number of pieces player has
// drawn in the current game
int CPlayer::getPiecesDrawn()
{
	return stat_PiecesDrawn;
}

// Returns the number of pieces in the players hand
int CPlayer::getPiecesInHand()
{
	return hand.getSize();
}

// Trys to take the given number of pieces from the Boneyard pile
// and places them in the players hand. Returns actual
// number of pieces taken (pile might have less than the
// given number of pieces)
int CPlayer::fillHand(CDominoCollection &pile, int count)
{
	int numTaken = 0;

	while (pile.getSize() > 0 && numTaken < count)
	{
		CDomino piece;

		pile.getFirst(piece); // Get first piece in pile
		pile.remove(piece);   // Remove piece from pile
		hand.addToEnd(piece); // Add piece to players hand
		
		numTaken++;
	}

	return numTaken;
}

// Calls appropriate turn process function (AI or Player). Returns true if
// the player made a move or drew a piece from the pile. Returns false if the
// player passed their turn
bool CPlayer::processTurn(CDominoCollection &table, CDominoCollection &pile)
{
	if (isAI)
	{
		return processAITurn(table, pile);
	}
	else
	{
		return showPlayerMenu(table, pile);
	}
}

// Runs the AI player turn logic. Plays a piece if it can, otherwise
// draws a piece or passes their turn.
bool CPlayer::processAITurn(CDominoCollection &table, CDominoCollection &pile)
{
	system("CLS");

	cout << "--------------------------------------------" << endl;
	cout << "It is now " << name << "'s turn." << std::endl;
	cout << "--------------------------------------------" << endl << endl;

	// Print dominoes on table/game board
	cout << "~ Dominoes on the table ~" << endl;
	table.printAsChainedList();

	// Does the AI player have a playable piece?
	int playablePiece = findPlayablePiece(table);

	// Keep drawing pieces until the AI has a piece
	// they can play, or until the Boneyard is empty
	while (playablePiece == -1 && pile.getSize() > 0)
	{
		CDomino drawn;

		// Attempt to draw a piece from the Boneyard pile
		if (drawPiece(pile, drawn))
		{
			cout << endl << name << " drew a domino from the Boneyard: ";
			drawn.print();
			cout << endl << endl;

			system("pause");

			// Check if drawn piece is playable
			playablePiece = findPlayablePiece(table);
		}
		else
		{
			// Failed to draw piece because pile was empty

			cout << endl << name << " attempted to draw a piece, but the Boneyard was empty :(";
			cout << endl << endl;

			system("pause");
			break;
		}
	}

	if (playablePiece >= 0)
	{
		// AI has a piece that they can play
		CDomino dom;

		// Get domino from hand
		hand.getPiece(playablePiece, dom);

		// Play domino on table
		playPiece(table, dom, false);

		return true;
	}
	else
	{
		// AI player doesn't have a piece that they can play.
		// Pass their turn.

		return false;
	}
}

// Shows player menu for human players. Gives options to play a piece, draw a piece,
// or pass their turn.
bool CPlayer::showPlayerMenu(CDominoCollection &table, CDominoCollection &pile)
{
	while (true)
	{
		system("CLS");

		cout << "--------------------------------------------" << endl;
		cout << "It is now " << name << "'s turn." << std::endl;
		cout << "--------------------------------------------" << endl << endl;

		// Print dominoes on table/game board
		cout << "~ Dominoes on the table ~" << endl;
		table.printAsChainedList();

		// Print player meu options
		cout << endl << "=== Player Menu ===" << endl << endl;
		cout << "    1. Play Piece." << endl;
		cout << "    2. View Number of Pieces in Boneyard." << endl;
		cout << "    3. Draw Piece." << endl;
		cout << "    4. Pass Turn." << endl << endl;
		cout << "Selection: ";

		string selectionStr;
		cin >> selectionStr;

		cout << endl;

		try
		{
			// Take appropriate action based on player
			// input selection
			switch (std::stoi(selectionStr, nullptr, 10))
			{
			case 1:
				printHand();

				cout << endl;

				if (showPlayPieceMenu(table))
				{
					// Player successfully played a piece
					// End turn as success
					return true;
				}

				// Player canceled selection, or selection was invalid.
				continue;
			case 2:
				cout << "There are " << pile.getSize() << " pieces left in the Boneyard." << endl;
				break;
			case 3:
				if (canPlayPiece(table))
				{
					// Check if the player has no pieces they can play. If they do have a domino
					// that they can play, notify them rather than drawing another.
					cout << "Don't draw a piece! You have one that you can play!" << endl;
					break;
				}
				else
				{
					CDomino drawn;
					if (drawPiece(pile, drawn))
					{
						// Draw a piece from the boneyard pile
						cout << name << " drew a domino from the Boneyard: ";
						drawn.print();
						cout << endl;
					}
					else
					{
						// Boneyard pile was empty
						cout << "You cannot draw any pieces because the Boneyard is empty. Pass your turn." << endl;
					}
				}
				break;
			case 4:
				if (canPlayPiece(table))
				{
					// Check if the player has no pieces they can play. If they do have a domino
					// that they can play, notify them rather than passing their turn.
					cout << "You cannot pass your turn, you have a piece that you can play!" << endl;
					break;
				}
				else if (pile.getSize() > 0)
				{
					// Make sure Boneyard pile is empty. If it is not empty, notify player that
					// they should draw a domino instead.
					cout << "Don't just pass your turn, draw a piece from the Boneyard!" << endl;
					break;
				}
				else
				{
					return false; // Pass turn
				}
			default:
				// Player selection was out of range
				cout << "Invalid selection.";
			}
		}
		catch (exception ex)
		{
			// Player input was not an integer
			cout << "Invalid input format.";
		}

		cout << endl;
		system("pause");
	}
}

// Displays a menu for the player to select the piece that
// they want to play then returns true if sucessful. If player 
// attempts to play an invalid piece, an error is displayed 
// and this method returns false.
bool CPlayer::showPlayPieceMenu(CDominoCollection &table)
{
	cout << "Enter Domino ID to play, or '0' to Cancel: ";

	string pieceID;
	cin >> pieceID;

	try
	{
		// Get selection from user
		int pieceIndex = std::stoi(pieceID, nullptr, 10) - 1;

		// User entered '0' to return to the main menu
		if (pieceIndex == -1) 
		{ 
			return false; 
		}

		CDomino selectedDomino;

		// Grab the domino piece that the user selected
		if (hand.getPiece(pieceIndex, selectedDomino))
		{
			// Attempt to play the piece on the table
			return playPiece(table, selectedDomino, true);
		}
		else
		{
			// Selection is out of bounds
			cout << "'" << pieceID << "' is not a valid piece ID for your hand." << endl << endl;
			system("pause");
			return false;
		}
	}
	catch (exception ex)
	{
		// User input was not an integer
		cout << "Invalid input format." << endl << endl;
		system("pause");
		return false;
	}
}

// Attempts to play the given domino piece on the table, and returns true
// if successful. If askSide is true, user will be asked which side they
// want to play the piece if the given domino can be played at either end.
// If askSide is false, the side is randomly chosen.
bool CPlayer::playPiece(CDominoCollection &table, CDomino &domino, bool askSide)
{
	if (table.getSize() == 0)
	{
		// Table is empty, so play the piece
		table.addToStart(domino);
		hand.remove(domino);

		cout << endl << name << " placed the first piece: ";
		domino.print();

		cout << endl << endl;

		stat_PiecesPlayed++;

		system("pause");

		return true;
	}
	else
	{
		CDomino leftChain;
		CDomino rightChain;
		int leftChainDots;
		int rightChainDots;
		bool canPlayLeft = false;
		bool canPlayRight = false;

		table.getFirst(leftChain); // Get domino on left side of chain
		table.getLast(rightChain); // Get domino on right side of chain
		leftChainDots = leftChain.getLeft();    // Get left-most dots of left chain domino
		rightChainDots = rightChain.getRight();  // Get right-most dots of right chain domino

		// Check if to-be-played domino can be played on the left side
		// of the table chain
		if (domino.hasSide(leftChainDots))
		{
			canPlayLeft = true;
		}

		// Check if to-be-played domino can be played on the right side
		// of the table chain
		if (domino.hasSide(rightChainDots))
		{
			canPlayRight = true;
		}

		// If to-be-played domino cannot be placed at either
		// end, display error message and return false
		if (!canPlayLeft && !canPlayRight)
		{
			cout << "Error: Cannot play piece: ";
			domino.print();

			cout << ". No matching piece on table." << endl << endl;

			system("pause");
			return false;
		}

		// If to-be-played domino can be played on either end
		// of the chain, choose a side by either asking the player,
		// or selecing randomly depending on the value of askSide
		if (canPlayLeft && canPlayRight)
		{
			if (askSide)
			{
				// Ask player what side they prefer
				while (true)
				{
					cout << "What side do you want to play this piece on (left or right)? ";

					string ans;
					cin >> ans;

					if (ans == "left")
					{
						canPlayRight = false;
						break;
					}
					else if (ans == "right")
					{
						canPlayLeft = false;
						break;
					}
					else
					{
						cout << "Error: Invalid input." << endl;
						system("pause");
					}
				}
			}
			else
			{
				// Flip a coin to pick a side randomly.
				int coinFlip =  rand() % 2;

				if (coinFlip)
				{
					canPlayRight = false;
				}
				else
				{
					canPlayLeft = false;
				}
			}
		}

		cout << endl << name << " played ";
		domino.print();

		// Play domino piece on selected side
		if (canPlayLeft)
		{
			// Swap/rotate domino if necessary so that dots connect
			// with chain
			if (domino.getRight() != leftChainDots) { domino.swap(); }

			table.addToStart(domino); // Add domino to left side of chain
			hand.remove(domino);      // Remove domino from hand

			cout << " on the left side of the chain." << endl << endl;
		}
		else if (canPlayRight)
		{
			// Swap/rotate domino if necessary so that dots connect
			// with chain
			if (domino.getLeft() != rightChainDots) { domino.swap(); }

			table.addToEnd(domino); // Add domino to right side of chain
			hand.remove(domino);    // Remove domino from hand

			cout << " on the right side of the chain." << endl << endl;
		}

		stat_PiecesPlayed++;

		system("pause");

		return true;
	}
}

// Returns true if there is a piece in the player's hand that can be played on the table
bool CPlayer::canPlayPiece(CDominoCollection &table)
{
	// if table is empty, than any piece in the players hand can be played
	if (table.getSize() == 0 && hand.getSize() > 0) { return true; }

	// Attempt to find a playable piece
	return (findPlayablePiece(table) > -1);
}

// Returns the index of the first piece in the player's hand
// that can be played on the table, or returns -1 if no such piece
// exists.
int CPlayer::findPlayablePiece(CDominoCollection &table)
{
	// Player hand is empty?
	if (hand.getSize() == 0) { return -1; }

	if (table.getSize() == 0) // Nothing on table yet?
	{
		return 0; // Return index to first piece
	}

	CDomino leftChain;
	CDomino rightChain;
	int leftChainDots;
	int rightChainDots;

	table.getFirst(leftChain);
	table.getLast(rightChain);
	leftChainDots = leftChain.getLeft();
	rightChainDots = rightChain.getRight();

	// Get index of a domino piece that has the same number of dots
	// on either side as the left chain piece on the table
	int leftHandAvailIndex = hand.findWithDots(leftChainDots);

	if (leftHandAvailIndex > -1)
	{
		// A piece was found that connects with the left chain
		return leftHandAvailIndex;
	}

	// Get index of a domino piece that has the same number of dots
	// on either side as the right chain piece on the table
	int rightHandAvailIndex = hand.findWithDots(rightChainDots);

	if (rightHandAvailIndex > -1)
	{
		// A piece was found that connects with the right chain
		return rightHandAvailIndex;
	}

	return -1; // No piece found that connects with either side if the chain
}

// Attempts to draw a piece from the pile boneyard.
// Returns false if the pile is empty.
// If successful, sets passed-by-reference drawnDomino to the piece drawn
bool CPlayer::drawPiece(CDominoCollection &pile, CDomino &drawnDomino)
{
	// Is the Boneyard pile empty?
	if (pile.getSize() == 0) { return false; }

	CDomino drawnPiece;
	pile.getFirst(drawnPiece); // Get first piece in Boneyard pile

	pile.remove(drawnPiece);   // Remove piece from Bonyard
	hand.addToEnd(drawnPiece); // Add piece to player hand

	drawnDomino = drawnPiece;

	stat_PiecesDrawn++;

	return true;
}

// Prints all of the pieces in the players current hand as
// a numbered list
void CPlayer::printHand()
{
	cout << name << "'s hand:" << endl;
	hand.printAsNumberedList();
}

/* =========================================
*  End of File
*  =========================================
*/