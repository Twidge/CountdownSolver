#ifndef BOARD_H
#define BOARD_H

#include <vector>

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
	IntermediateStep(int number, std::vector<int> vect) : numberSoFar(number), numbersRemaining(vect) {}

	int numberSoFar;
	std::vector<int> numbersRemaining = std::vector<int>();
};

class Board
{
private :

	std::vector<int> m_chosenNumbers;
	unsigned int m_target = G_BASE_TARGET;
	std::vector<int> m_availableBigNumbers;
	std::vector<int> m_availableSmallNumbers;

	bool m_readyForSolve = false;

	// Disable copy constructor
	Board(const Board&);

public :

	// CONSTRUCTORS
	Board();

	// GETTERS

	// METHODS
	bool RecursiveSolve();
	bool RecursiveSolve(IntermediateStep const&);
	void ResetBoard();
	void SetUpNumbersAndTarget(unsigned int, unsigned int);

	// OPERATOR OVERLOADS
	void operator= (const Board&);
};

#endif