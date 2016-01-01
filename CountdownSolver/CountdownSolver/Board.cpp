#include "Board.h"
#include <iostream>
#include <random>

// CONSTRUCTORS

Board::Board()
{
	// Populate m_availableBigNumbers with one of each big number

	for (int l_index = 0; l_index < G_BIG_NUMBERS_SIZE; l_index++)
	{
		m_availableBigNumbers.push_back(G_BIG_NUMBERS[l_index]);
	}

	// Populate m_availableSmallNumbers with two of each small number (arranged 1, 1, 2, 2, ...)

	for (int l_index = 0; l_index < G_SMALL_NUMBERS_SIZE; l_index++)
	{
		m_availableSmallNumbers.push_back(G_SMALL_NUMBERS[l_index / 2]);
	}
}

Board::Board(const Board&)
{
	// Disabled
}

// GETTERS

// METHODS

bool Board::RecursiveSolve()
{
	if (RecursiveSolve({ 0, m_chosenNumbers }))
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Board::RecursiveSolve(IntermediateStep const& s_nextStep)
{
	// Checks if the target is reachable given the chosen numbers, returns true if yes and false if not

	if (!m_readyForSolve)
	{
		SetUpNumbersAndTarget(0, 6); // In case somebody calls RecursiveSolve before SetUp...
	}

	// Check if the achieved number is the target number

	if (s_nextStep.numberSoFar == m_target)
	{
		return true;
	}

	else if (s_nextStep.numberSoFar != m_target && s_nextStep.numbersRemaining.size() == 0)
	{
		return false;
	}

	else
	{
		for (unsigned int l_chosenNumberIndex = 0; l_chosenNumberIndex < s_nextStep.numbersRemaining.size(); l_chosenNumberIndex++)
		{
			std::vector<int> t_unusedNumbersSoFar = std::vector<int>();

			for (unsigned int l_secondChosenNumberIndex = 0; l_secondChosenNumberIndex < s_nextStep.numbersRemaining.size();
				l_secondChosenNumberIndex++)
			{
				if (l_secondChosenNumberIndex != l_chosenNumberIndex)
				{
					t_unusedNumbersSoFar.push_back(s_nextStep.numbersRemaining[l_secondChosenNumberIndex]);
				}
			}

			// Solve again after adding the next candidate number

			if (RecursiveSolve({ s_nextStep.numberSoFar + s_nextStep.numbersRemaining[l_chosenNumberIndex], t_unusedNumbersSoFar }))
			{
				return true;
			}

			// Solve again after subtracting the next candidate number, or by subtracting the number so far from the candidate number

			if (RecursiveSolve({ s_nextStep.numberSoFar - s_nextStep.numbersRemaining[l_chosenNumberIndex], t_unusedNumbersSoFar }))
			{
				return true;
			}

			if (RecursiveSolve({ s_nextStep.numbersRemaining[l_chosenNumberIndex] - s_nextStep.numberSoFar, t_unusedNumbersSoFar }))
			{
				return true;
			}

			// Solve again after multiplying by the candidate number

			if (RecursiveSolve({ s_nextStep.numberSoFar * s_nextStep.numbersRemaining[l_chosenNumberIndex], t_unusedNumbersSoFar }))
			{
				return true;
			}

			// Solve again after dividing by the candidate number, or by dividing the number so far by the candidate number

			if (s_nextStep.numberSoFar != 0)
			{
				if (s_nextStep.numbersRemaining[l_chosenNumberIndex] % s_nextStep.numberSoFar == 0)
				{
					if (RecursiveSolve({ s_nextStep.numberSoFar / s_nextStep.numbersRemaining[l_chosenNumberIndex], t_unusedNumbersSoFar }))
					{
						return true;
					}
				}
			}

			if (s_nextStep.numberSoFar != 0)
			{
				if (s_nextStep.numberSoFar % s_nextStep.numbersRemaining[l_chosenNumberIndex] == 0)
				{
					if (RecursiveSolve({ s_nextStep.numbersRemaining[l_chosenNumberIndex] / s_nextStep.numberSoFar, t_unusedNumbersSoFar }))
					{
						return true;
					}
				}
			}
		}

		// If no successful method is found, conclude this branch cannot be solved

		return false;
	}
}

void Board::ResetBoard()
{
	// Resets the board to the state it was in when created

	m_target = G_BASE_TARGET;

	// Repopulate m_availableBigNumbers and m_availableSmallNumbers
	// {OPT}: Faster way of doing this?

	m_availableBigNumbers.clear();
	m_availableSmallNumbers.clear();

	for (int l_index = 0; l_index < G_BIG_NUMBERS_SIZE; l_index++)
	{
		m_availableBigNumbers.push_back(G_BIG_NUMBERS[l_index]);
	}

	for (int l_index = 0; l_index < G_SMALL_NUMBERS_SIZE; l_index++)
	{
		m_availableSmallNumbers.push_back(G_SMALL_NUMBERS[l_index / 2]);
	}
}

void Board::SetUpNumbersAndTarget(unsigned int bigNumbers, unsigned int smallNumbers)
{
	/*
		Chooses bigNumbers big numbers and smallNumbers small numbers at random from the face-down cards
	*/

	ResetBoard();

	// Check conditions on bigNumbers - needs to be between 0 and G_BIG_NUMBERS_SIZE
	// (also between 0 and G_CHOSEN_NUMBERS_SIZE if we change those numbers in the future)

	if (bigNumbers > G_BIG_NUMBERS_SIZE)
	{
		std::cout << "Error: attempting to choose more big numbers than is possible!\n";
	}

	else if (bigNumbers > G_CHOSEN_NUMBERS_SIZE)
	{
		std::cout << "Error: attempting to choose more big numbers than will fit on the board!\n";
	}

	// Check conditions on smallNumbers - needs to be between 0 and G_CHOSEN_NUMBERS_SIZE
	// (also between 0 and G_SMALL_NUMBERS_SIZE if we change those numbers in the future)

	else if (smallNumbers > G_CHOSEN_NUMBERS_SIZE)
	{
		std::cout << "Error: attempting to choose more small numbers than will fit on the board!\n";
	}

	else if (smallNumbers > G_SMALL_NUMBERS_SIZE)
	{
		std::cout << "Error: attempting to choose more small numbers than is possible!\n";
	}

	// Check conditions on their sum - needs to be equal to G_CHOSEN_NUMBERS_SIZE

	else if (smallNumbers + bigNumbers != G_CHOSEN_NUMBERS_SIZE)
	{
		std::cout << "Error: have not chosen the correct amount of numbers.\n";
	}

	// Else, populate m_chosenNumbers with bigNumbers big numbers and smallNumbers small numbers
	// Then pick a target from 101 to 999

	else
	{
		// {OPT}: This minimises calls to rand() but uses vector::erase(). Which is faster? Is there a way that avoids both?

		for (unsigned int l_choice = 0; l_choice < G_CHOSEN_NUMBERS_SIZE; l_choice++)
		{
			if (l_choice < bigNumbers)
			{
				int t_numberChosen = rand() % m_availableBigNumbers.size();

				m_chosenNumbers.push_back(m_availableBigNumbers[t_numberChosen]);

				m_availableBigNumbers.erase(m_availableBigNumbers.begin() + t_numberChosen);
			}

			else if (l_choice < bigNumbers + smallNumbers)
			{
				int t_numberChosen = rand() % m_availableSmallNumbers.size();

				m_chosenNumbers.push_back(m_availableSmallNumbers[t_numberChosen]);

				m_availableSmallNumbers.erase(m_availableSmallNumbers.begin() + t_numberChosen);
			}
		}

		m_target = (rand() % (G_MAX_TARGET - G_MIN_TARGET)) + G_MIN_TARGET;
	}

	m_readyForSolve = true;
}

// OPERATOR OVERLOADS

void Board::operator= (const Board& a_board)
{
	
}