#include "Board.h"
#include "CommonTemplateFunctions.h"
#include <random>
#include <new>

// CONSTRUCTORS

Board::Board()
{
	m_target = G_BASE_TARGET;

	m_chosenNumbers = std::vector<int>();

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

Board::Board(Board const& board)
{
	m_target = board.m_target;
	m_chosenNumbers = board.m_chosenNumbers;
	m_availableBigNumbers = board.m_availableBigNumbers;
	m_availableSmallNumbers = board.m_availableSmallNumbers;
}

// GETTERS

// METHODS

bool Board::RecursiveSolve() const
{
	IntermediateStep t_workingOut = { m_chosenNumbers, std::vector<int> {0} };

	InfoAboutNumbersReached t_endResult = RecursiveSolve(t_workingOut);

	// Check if any of them are equal to the target

	for (unsigned int numbersReachedIndex = 0; numbersReachedIndex < t_endResult.numbersReached.size(); numbersReachedIndex++)
	{
		if (t_endResult.numbersReached[numbersReachedIndex] == m_target)
		{
			t_endResult.targetReached = true;
			break;
		}
	}

	if (t_endResult.targetReached)
	{
		return true;
	}

	else
	{
		return false;
	}
}

InfoAboutNumbersReached Board::RecursiveSolve(IntermediateStep const& s_nextStep) const
{
	InfoAboutNumbersReached t_newNumbers{ false, std::vector<int> {0} };

	// Check if the target has already been achieved

	if (s_nextStep.targetReached)
	{
		t_newNumbers = { true, s_nextStep.numbersReached };
	}

	// Base case: if there is one number left, add all possibilities using that number to the list of numbers reached
	// Also return information about whether the target was reached

	else if (!s_nextStep.targetReached && s_nextStep.numbersRemaining.size() == 1)
	{
		// Try all relevant additions, subtractions, multiplications and divisions for each number in numbersReached
		for (unsigned int l_reachableNumber = 0; l_reachableNumber < s_nextStep.numbersReached.size(); l_reachableNumber++)
		{
			t_newNumbers.numbersReached.push_back(s_nextStep.numbersReached[l_reachableNumber] + s_nextStep.numbersRemaining[0]);

			s_nextStep.numbersReached[l_reachableNumber] >= s_nextStep.numbersRemaining[0] ?
				t_newNumbers.numbersReached.push_back(s_nextStep.numbersReached[l_reachableNumber] - s_nextStep.numbersRemaining[0])
				: t_newNumbers.numbersReached.push_back(s_nextStep.numbersRemaining[0] - s_nextStep.numbersReached[l_reachableNumber]);

			t_newNumbers.numbersReached.push_back(s_nextStep.numbersReached[l_reachableNumber] * s_nextStep.numbersRemaining[0]);

			if (t_newNumbers.numbersReached[l_reachableNumber] % s_nextStep.numbersRemaining[0] == 0)
			{
				t_newNumbers.numbersReached.push_back(s_nextStep.numbersReached[l_reachableNumber] / s_nextStep.numbersRemaining[0]);
			}

			if (s_nextStep.numbersReached[l_reachableNumber] != 0
				&& s_nextStep.numbersRemaining[0] % s_nextStep.numbersReached[l_reachableNumber] == 0)
			{
				t_newNumbers.numbersReached.push_back(s_nextStep.numbersRemaining[0] / s_nextStep.numbersReached[l_reachableNumber]);
			}
		}

		// Check if any of the new numbers are equal to the target
		for (std::vector<int>::iterator l_newNumbersReached = t_newNumbers.numbersReached.begin();
			l_newNumbersReached != t_newNumbers.numbersReached.end(); l_newNumbersReached++)
		{
			if (*l_newNumbersReached == m_target)
			{
				t_newNumbers.targetReached = true;
			}
		}
	}

	// Otherwise, let n = s_nextStep.numbersRemaining.size(), let x be in the interval [1, n/2], let S be a subset of x elements
	// from s_nextStep.numbersRemaining and let T be s_nextStep.numbersRemaining \ S. Let ~ be an operation (+, -, *, /). Let c range
	// through the elements of s_nextStep.numbersReached. Let a, a' be integers from RecursiveSolve(S, {0}) and RecursiveSolve(S, {c})
	// respectively. Let b, b' be integers from RecursiveSolve(T, {0}) and RecursiveSolve(T, {c}) respectively.

	// Return all elements of the form a ~ b' and a' ~ b, wherever these elements are integers, together with a boolean determining
	// whether any of these elements are equal to the target.

	// BUG?: If the below statement is just "else" it doesn't trigger when s_nextStep.numbersRemaining.size() > 1

	else if (s_nextStep.numbersRemaining.size() > 1)
	{
		// For each possible size l_subsetSize of subset...
		for (unsigned int l_subsetSize = 1; l_subsetSize <= s_nextStep.numbersRemaining.size() / 2; l_subsetSize++)
		{
			// Generate all subsets of that size...

			std::vector<std::vector<int>> t_allSubsetsOfThatSize = GenerateSubsets(s_nextStep.numbersRemaining, l_subsetSize);

			// For each subset *l_nextSubsetIndex of that size...

			for (std::vector<std::vector<int>>::iterator l_nextSubsetIndex = t_allSubsetsOfThatSize.begin();
				l_nextSubsetIndex != t_allSubsetsOfThatSize.end();
				l_nextSubsetIndex++)
			{
				// Determine set complement t_subsetComplement of *l_nextSubsetIndex in s_nextStep.numbersRemaining...

				std::vector<int> t_subsetComplement = SetComplement(s_nextStep.numbersRemaining, *l_nextSubsetIndex);

				// For each element *l_nextNumberReached of s_nextStep.numbersReached...

				for (std::vector<int>::const_iterator l_nextNumberReached = s_nextStep.numbersReached.begin();
					l_nextNumberReached != s_nextStep.numbersReached.end();
					l_nextNumberReached++)
				{
					// Get RecursiveSolve(S, {0})

					InfoAboutNumbersReached t_reachableFromZero = RecursiveSolve({ *l_nextSubsetIndex, std::vector<int> {0} });

					// Get RecursiveSolve(T, {c})

					InfoAboutNumbersReached t_reachableWithSetComplementFromNumberReached
						= RecursiveSolve({ t_subsetComplement, std::vector<int> {*l_nextNumberReached} });

					// If target is reachable in one of the below, just skip to the end

					if (t_reachableFromZero.targetReached || t_reachableWithSetComplementFromNumberReached.targetReached)
					{
						t_newNumbers.targetReached = true;
						break;
					}

					// For each element *l_nextReturnedNumberReached of t_reachableFromZero.numbersReached...

					for (std::vector<int>::iterator l_nextReturnedNumberReached = t_reachableFromZero.numbersReached.begin();
						l_nextReturnedNumberReached != t_reachableFromZero.numbersReached.end();
						l_nextReturnedNumberReached++)
					{
						// For each element *l_nextSetComplementReturnedNumberReached of t_reachableWithSetComplementFromNumberReached...

						for (std::vector<int>::iterator l_nextSetComplementReturnedNumberReached = t_reachableWithSetComplementFromNumberReached.numbersReached.begin();
							l_nextSetComplementReturnedNumberReached != t_reachableWithSetComplementFromNumberReached.numbersReached.end();
							l_nextSetComplementReturnedNumberReached++)
						{
							// Do all possible operations on these numbers and add them to t_newNumbers.numbersReached

							try
							{
								if (s_nextStep.numbersRemaining.size() != 6
									|| *l_nextReturnedNumberReached + *l_nextSetComplementReturnedNumberReached == m_target)
								{
									UnsortedAddIfDistinct(*l_nextReturnedNumberReached + *l_nextSetComplementReturnedNumberReached,
										t_newNumbers.numbersReached);
								}

								if (s_nextStep.numbersRemaining.size() != 6
									|| *l_nextReturnedNumberReached - *l_nextSetComplementReturnedNumberReached == m_target
									|| *l_nextSetComplementReturnedNumberReached - *l_nextReturnedNumberReached == m_target)
								{
									*l_nextReturnedNumberReached >= *l_nextSetComplementReturnedNumberReached ?
										UnsortedAddIfDistinct(*l_nextReturnedNumberReached - *l_nextSetComplementReturnedNumberReached,
										t_newNumbers.numbersReached)
										: UnsortedAddIfDistinct(*l_nextSetComplementReturnedNumberReached - *l_nextReturnedNumberReached,
										t_newNumbers.numbersReached);
								}

								if (s_nextStep.numbersRemaining.size() != 6
									|| *l_nextReturnedNumberReached * *l_nextSetComplementReturnedNumberReached == m_target)
								{
									UnsortedAddIfDistinct(*l_nextReturnedNumberReached * *l_nextSetComplementReturnedNumberReached,
										t_newNumbers.numbersReached);
								}

								if (*l_nextSetComplementReturnedNumberReached != 0
									&& *l_nextReturnedNumberReached % *l_nextSetComplementReturnedNumberReached == 0)
								{
									if (s_nextStep.numbersRemaining.size() != 6
										|| *l_nextReturnedNumberReached / *l_nextSetComplementReturnedNumberReached == m_target)
									{
										UnsortedAddIfDistinct(*l_nextReturnedNumberReached / *l_nextSetComplementReturnedNumberReached,
											t_newNumbers.numbersReached);
									}
								}

								if (*l_nextReturnedNumberReached != 0
									&& *l_nextSetComplementReturnedNumberReached % *l_nextReturnedNumberReached == 0)
								{
									if (s_nextStep.numbersRemaining.size() != 6
										|| *l_nextSetComplementReturnedNumberReached / *l_nextReturnedNumberReached == m_target)
									{
										UnsortedAddIfDistinct(*l_nextSetComplementReturnedNumberReached / *l_nextReturnedNumberReached,
											t_newNumbers.numbersReached);
									}
								}
							}
							catch (std::exception& error)
							{
								std::cerr << error.what() << "\n";
							}

							// Could also do nothing and return *l_nextNumberReached!

							UnsortedAddIfDistinct(*l_nextNumberReached, t_newNumbers.numbersReached);
						}
					}
				}

				if(t_newNumbers.targetReached)
				{
					break;
				}
			}

			if (t_newNumbers.targetReached)
			{
				break;
			}
		}
	}

	#ifdef DEBUG
	if (s_nextStep.numbersRemaining.size() >= G_CHOSEN_NUMBERS_SIZE - 1)
	{
		std::cout << "RecursiveSolve() returning for size " << s_nextStep.numbersRemaining.size() << ".\n";
	}
	#endif

	return t_newNumbers;
}

void Board::ResetBoard()
{
	// Resets the board to the state it was in when created

	m_target = G_BASE_TARGET;
	m_chosenNumbers.clear();

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
}

// OPERATOR OVERLOADS

void Board::operator= (Board const& board)
{
	m_target = board.m_target;
	m_chosenNumbers = board.m_chosenNumbers;
	m_availableBigNumbers = board.m_availableBigNumbers;
	m_availableSmallNumbers = board.m_availableSmallNumbers;
}

std::ostream& operator<< (std::ostream& os, const Board& board)
{
	os << "Target is: " << board.m_target << "\n";
	os << "Numbers are: ";

	for (int nextNumber = 0; nextNumber < G_CHOSEN_NUMBERS_SIZE; nextNumber++)
	{
		os << board.m_chosenNumbers[nextNumber] << " ";
	}

	os << "\n";

	return os;
}