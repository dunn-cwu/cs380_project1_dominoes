/* =============================================
* Assignmet: Project 1
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: CDomino.h
* Description: Header file for CDomino class.
*     Represents a single domino piece. Includes
*     various helper methods. Conditional == and
*     != operators have been overridden to ensure
*     two domino pieces that have been reversed
*     are still considered equal. Ex: [5|6]==[6|5]
* ==============================================
*/

#pragma once

class CDomino
{
private:
	int left;  // # left side dots
	int right; // # right side dots
public:
	CDomino();
	CDomino(int left, int right);
	CDomino(const CDomino &other);
	~CDomino();
	int getLeft();
	int getRight();
	bool isDouble();
	int hasSide(int dots);
	void swap();
	void print();

	bool operator==(const CDomino &other) const;
	bool operator!=(const CDomino &other) const;
};

/* =========================================
*  End of File
*  =========================================
*/