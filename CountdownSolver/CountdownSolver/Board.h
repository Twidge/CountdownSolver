#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>

const unsigned int G_CHOSEN_NUMBERS_SIZE = 6;
const unsigned int G_BIG_NUMBERS_SIZE = 4;
const unsigned int G_SMALL_NUMBERS_SIZE = 20;

const unsigned int G_BIG_NUMBERS[G_BIG_NUMBERS_SIZE] = { 25, 50, 75, 100 };
const unsigned int G_SMALL_NUMBERS[G_SMALL_NUMBERS_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

const unsigned int G_BASE_TARGET = 101;
const unsigned int G_MAX_TARGET = 999;
const unsigned int G_MIN_TARGET = 101;

/*
	Board represents the physical Countdown numbers board, which has:
		1. Six chosen numbers: the player specifies how many "big" numbers (25, 50, 75, 100) and how many "small" numbers (1 to 10)
		   they would like. The numbers are then chosen from 3. and 4. (see below) at random given these constraints.
		2. Target number: the number the player is attempting to reach using the chosen six numbers.
		3. Four face-down large numbers: one of each
		4. Twenty face-down small numbers: two of each
*/

struct IntermediateStep
{
	IntermediateStep(std::vector<int> nRemaining, std::vector<int> nReached)
		: targetReached(false), numbersRemaining(nRemaining), numbersReached(nReached) {}

	bool targetReached;
	std::vector<int> numbersRemaining = std::vector<int>();
	std::vector<int> numbersReached = std::vector<int>();
};

struct InfoAboutNumbersReached
{
	InfoAboutNumbersReached(bool tReached, std::vector<int> nReached)
		: targetReached(tReached), numbersReached(nReached) {}

	InfoAboutNumbersReached()
		: targetReached(false), numbersReached(std::vector<int> {0}) {};

	bool targetReached;
	std::vector<int> numbersReached;
};

class Board
{
private :

	std::vector<int> m_chosenNumbers;
	std::vector<int> m_availableBigNumbers;
	std::vector<int> m_availableSmallNumbers;

public :

	unsigned int m_target = G_BASE_TARGET;

	// CONSTRUCTORS
	Board();
	Board(Board const&);

	// GETTERS

	// METHODS
	bool RecursiveSolve() const;
	InfoAboutNumbersReached RecursiveSolve(IntermediateStep const&) const;
	void ResetBoard();
	void SetUpNumbersAndTarget(unsigned int, unsigned int);

	// OPERATOR OVERLOADS
	void operator= (Board const&);
	friend std::ostream& operator<<(std::ostream&, const Board&);
};

#endif