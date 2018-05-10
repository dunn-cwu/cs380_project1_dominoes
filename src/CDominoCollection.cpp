/* =============================================
* Assignmet: Project 1
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CDominoCollection.cpp
* Description: Implementation file for CDominoCollection
*     class. Used as a generic collection to store
*     domino pieces for the table, Boneyard pile,
*     and player hands. Implemented as a doubly
*     linked list.
* ==============================================
*/

#include <cstdlib>
#include <vector>
#include <iostream>
#include "CDominoCollection.h"

using namespace std;

// Constructor for DominoListNode, used in linked list
DominoListNode::DominoListNode(const CDomino &domino) : prevPtr(0), nextPtr(0)
{
	this->data = CDomino(domino);
}

// Default constructor
CDominoCollection::CDominoCollection() : rootNodePtr(0), size(0)
{
}

// Destructor, clears list and releases memory allocated
// by DominoListNode objects
CDominoCollection::~CDominoCollection()
{
	// Release allocated memory
	this->clear();
}

// Returns the number of dominoes in the collection
int CDominoCollection::getSize()
{
	return size;
}

// Generates all 28 double-six domino pieces
// and fills this container with them
void CDominoCollection::generateAllPieces()
{
	clear(); // Clear out old pieces

	// Create domino pieces
	for (int l = 0; l <= 6; l++)
	{
		for (int r = 0; r <= l; r++)
		{
			addToStart(CDomino(l, r)); // Add domino to linked list
		}
	}
}

// Shuffles/randomizes all of the dominoes in the collection
// Uses a std::vector as temporary storage
void CDominoCollection::shuffle()
{
	if (size <= 1) { return; } // Not enough pieces to randomize

	std::vector<CDomino> tmpVect(size); // Create vector for temp storage of pieces

	// Randomly remove pieces and place them
	// in the temp storage vector until there
	// are no pieces left in the linked list
	while (size > 0)
	{
		int randIndex = randInt(0, size);

		CDomino randDom;
		getPiece(randIndex, randDom);

		tmpVect.push_back(randDom);
		remove(randIndex);
	}

	// Add all pieces in the temp vector back into the main
	// linked list
	for (unsigned int i = 0; i < tmpVect.size(); i++)
	{
		addToStart(tmpVect[i]);
	}

	// Clear the temp storage vector
	tmpVect.clear();
}

// Deletes all dominoes in the collection
// and releases memory allocated for
// the linked list nodes
void CDominoCollection::clear()
{
	if (rootNodePtr && size > 0) // Check for null ptr
	{
		DominoListNode* curNode = rootNodePtr;

		// Loop through each node in linked list
		// and call delete on it
		while (curNode)
		{
			DominoListNode* nextNode = curNode->nextPtr;
			delete curNode;
			curNode = nextNode;
		}

		rootNodePtr = 0;
		size = 0;
	}
}

// Returns true if there are no domino pieces
// in this collection
bool CDominoCollection::isEmpty()
{
	return (size == 0);
}

// Returns true of this collection contains the given
// domino piece. Note that domino pieces can be swapped/
// rotated, and this function will still return true if
// the rotated version is in the collection
bool CDominoCollection::contains(const CDomino &domino)
{
	return (this->find(domino) > -1);
}

// Add a domino piece to the end of the linked list
bool CDominoCollection::addToEnd(const CDomino &domino)
{
	// Make sure domino is not already in the collection
	if (contains(domino)) { return false; }
	
	// Allocate a new DominoListNode
	DominoListNode* newNode = this->createNode(domino);

	if (!rootNodePtr) // Is the collection empty?
	{
		rootNodePtr = newNode; // Set new node as root node
	}
	else
	{
		DominoListNode* endNode = rootNodePtr;

		// Traverse linked list until the end is reached
		while (endNode->nextPtr) {
			endNode = endNode->nextPtr;
		}

		// Update pointers on old last node
		// and new last node
		endNode->nextPtr = newNode;
		newNode->prevPtr = endNode;
	}

	size++; // Increment collection size
	return true;
}

// Adds a domino piece to the start of the linked list
bool CDominoCollection::addToStart(const CDomino &domino)
{
	// Make sure domino is not already in the collection
	if (contains(domino)) { return false; }

	// Allocate a new DominoListNode
	DominoListNode* newNode = this->createNode(domino);

	// Get current root node
	DominoListNode* oldRoot = rootNodePtr;

	// Replace root node with new node
	rootNodePtr = newNode;

	if (oldRoot) // If collection wasn't empty
	{
		// Update pointers on old root node
		// and new root node
		oldRoot->prevPtr = rootNodePtr;
		rootNodePtr->nextPtr = oldRoot;
	}

	size++; // Increment collection size
	return true;
}

// Simple wrapper method, allocates a new DominoListNode object
DominoListNode* CDominoCollection::createNode(const CDomino &domino)
{
	DominoListNode* newNode = new DominoListNode(domino);
	return newNode;
}

// Attempts to remove the given linked list node from the
// collection. Returns true if successful, or false
// if the given node was a nullptr
bool CDominoCollection::remove(DominoListNode* removeNode)
{
	// Check for null ptr
	if (!removeNode) { return false; }

	// Get links to given node
	DominoListNode* prevNode = removeNode->prevPtr;
	DominoListNode* nextNode = removeNode->nextPtr;

	// Check if node is root node
	if (removeNode == rootNodePtr)
	{
		rootNodePtr = nextNode; // Update root node ptr
	}

	// Update prev node ptr if necessary
	if (prevNode) {
		prevNode->nextPtr = nextNode;
	}

	// Update next node ptr if necessary
	if (nextNode)
	{
		nextNode->prevPtr = prevNode;
	}

	// Delete allocated memory for node
	delete removeNode;

	// Decrement collection size
	size--;

	return true;
}

// Wrapper method for main remove method.
// Accepts domino as argument. Will
// attempt to find given domino in
// collection.
bool CDominoCollection::remove(const CDomino &domino)
{
	if (size <= 0) { return false; }

	DominoListNode* curNode = rootNodePtr;

	// Traverse linked list. Stops when domino is found,
	// or end is reached
	while (curNode && curNode->data != domino)
	{
		curNode = curNode->nextPtr;
	}

	if (curNode->data != domino)
	{
		return false; // Domino wasn't found in collection
	}
	else
	{
		return this->remove(curNode); // Remove domino node
	}
	
}

// Wrapper method for main remove method.
// Accepts linked list index as argument.
bool CDominoCollection::remove(int index)
{
	// Index out of bounds?
	if (index < 0 || index >= size) { return false; }

	DominoListNode* removeNode = rootNodePtr;

	// Traverse linked list until we reach
	// the specified index. Store node ptr.
	for (int i = 0; i < index; i++)
	{
		removeNode = removeNode->nextPtr;
		if (!removeNode) { return false;  }
	}

	// Remove domino node
	return this->remove(removeNode);
}

// Returns index of domino in linked list,
// or -1 if the domino is not found.
int CDominoCollection::find(const CDomino &domino)
{
	int searchIndex = 0;

	DominoListNode* curNode = rootNodePtr;

	// Traverse linked list looking for given domino
	while (curNode)
	{
		// Found it?
		if (curNode->data == domino) { return searchIndex; }

		curNode = curNode->nextPtr;
		searchIndex++;
	}

	// Didn't find it
	return -1;
}

// Returns the index of the first domino piece in the collection
// that has a side with the given number of dots. Returns -1 if 
// no such piece is found.
int CDominoCollection::findWithDots(int dots)
{
	int searchIndex = 0;

	DominoListNode* curNode = rootNodePtr;

	// Traverse linked list looking for a domino that has a side
	// with the given dots
	while (curNode)
	{
		// Found it?
		if (curNode->data.hasSide(dots)) { return searchIndex; }

		curNode = curNode->nextPtr;
		searchIndex++;
	}

	// Didn't find it
	return -1;
}

// returns true if the collection is not empty,
// sets given passed-by-reference CDomino object
// to root node piece
bool CDominoCollection::getFirst(CDomino &out)
{
	return this->getPiece(0, out);
}

// returns true if the collection is not empty,
// sets given passed-by-reference CDomino object
// to last node piece in the collection
bool CDominoCollection::getLast(CDomino &out)
{
	return this->getPiece(size - 1, out);
}

// returns true if the collection is not empty,
// sets given passed-by-reference CDomino object
// to node piece at the given index in the collection
bool CDominoCollection::getPiece(int index, CDomino &out)
{
	// Index out of bounds?
	if (index < 0 || index >= size) { return false; }

	DominoListNode* curNode = rootNodePtr;

	// Travers linked list up to given index
	for (int i = 0; i < index; i++)
	{
		curNode = curNode->nextPtr;
	}

	// Set passed-by-reference domino
	out = curNode->data;

	return true;
}

// Prints the current domino collection as a
// numbered list, intended to display a
// players hand.
void CDominoCollection::printAsNumberedList()
{
	if (size == 0) // Collection is empty
	{
		cout << "    [Empty]" << endl;
	}
	else
	{
		DominoListNode* curNode = rootNodePtr;

		int index = 0;

		// Loop through each node in linked list,
		// and write it to the console in a list
		// form.
		while (curNode)
		{
			index++;

			cout << "    " << index << ": ";
			curNode->data.print();
			cout << endl;

			curNode = curNode->nextPtr;
		}
	}
}

// Prints the current domino collection as a
// table/game board, intended to display all
// domino pieces played on the table.
// Cuts out the middle pieces if there are
// more than MAX_DOMINO_DISPLAY dominoes
// in the collection to prevent console
// text wrap around.
void CDominoCollection::printAsChainedList()
{
	if (size == 0) // Collection is empty
	{
		cout << "    [Empty]" << endl;
	}
	else
	{
		DominoListNode* curNode = rootNodePtr;

		cout << "    ";

		if (size <= MAX_DOMINO_DISPLAY)
		{
			// If there are less than MAX_DOMINO_DISPLAY dominoes
			// in this collection, display them normally
			while (curNode)
			{
				curNode->data.print();
				curNode = curNode->nextPtr;
			}

			cout << endl;
		}
		else
		{
			// If there are more than MAX_DOMINO_DISPLAY dominoes
			// in this collection, cut out as many pieces in the middle
			// as necessary so that only MAX_DOMINO_DISPLAY pieces are
			// displayed in the console.

			int index = 0;

			while (curNode)
			{
				if (index < MAX_DOMINO_DISPLAY / 2 || size - index <= MAX_DOMINO_DISPLAY / 2)
				{
					curNode->data.print();
				}

				if (index == MAX_DOMINO_DISPLAY / 2)
				{
					cout << " (...) "; // Write middle separator
				}
				
				curNode = curNode->nextPtr;
				index++;
			}

			std::cout << std::endl;
		}
	}
}

// Simple wrapper method to generate a random number
// within a given range.
int CDominoCollection::randInt(int low, int hi)
{
	return low + (rand() % (hi - low + 1));
}

/* =========================================
*  End of File
*  =========================================
*/