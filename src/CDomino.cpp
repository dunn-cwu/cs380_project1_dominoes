/* =============================================
* Assignmet: Project 1
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CDomino.cpp
* Description: Implementation file for CDomino class.
*     Represents a single domino piece. Includes
*     various helper methods. Conditional == and
*     != operators have been overridden to ensure
*     two domino pieces that have been reversed
*     are still considered equal. Ex: [5|6]==[6|5]
* ==============================================
*/

#include <iostream>
#include "CDomino.h"


// Default constructor
CDomino::CDomino() : left(0), right(0)
{
}

// Constructor that takes left and right
// dot values
CDomino::CDomino(int left, int right)
{
	this->left = left;
	this->right = right;
}

// Copy constructor
CDomino::CDomino(const CDomino &other)
{
	this->left = other.left;
	this->right = other.right;
}

// Destructor
CDomino::~CDomino()
{
}

// Returns number of dots on left side
int CDomino::getLeft()
{
	return left;
}

// Returns number of dots on right side
int CDomino::getRight()
{
	return right;
}

// Returns true if both the left and right side have the same number of dots
bool CDomino::isDouble()
{
	return (left == right);
}

// Returns 1 if left side has given # of dots
// Returns 2 if right side has given # of dots
// Returns 0 if neither side has given # of dots
int CDomino::hasSide(int dots)
{
	if (left == dots) 
	{
		return 1;
	}
	else if (right == dots)
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

// Swaps left and right side dots, effectively
// rotating the piece 180 degrees.
void CDomino::swap()
{
	int leftOld = left;
	left = right;
	right = leftOld;
}

// Prints domino to console
void CDomino::print()
{
	std::cout << "[" << this->left << "|" << this->right << "]";
}

// Conditional operator overload
// Ensures equality between two domino pieces, even
// if one has the left and right swapped
bool CDomino::operator==(const CDomino &other) const
{
	return (left == other.left && right == other.right) ||
		   (right == other.left && left == other.right);
}

// Inverse of == operator
bool CDomino::operator!=(const CDomino &other) const
{
	return !(*this == other);
}

/* =========================================
*  End of File
*  =========================================
*/