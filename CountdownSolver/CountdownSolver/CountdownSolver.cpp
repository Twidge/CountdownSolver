// CountdownSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <ctime>

const unsigned int G_NUMBER_OF_TRIALS = 100;

int _tmain(int argc, _TCHAR* argv[])
{
	srand(static_cast<unsigned int>(time(NULL)));

	Board gameBoard;

	unsigned int t_successCounter = 0;

	for (unsigned int l_trialNumber = 0; l_trialNumber < G_NUMBER_OF_TRIALS; l_trialNumber++)
	{
		gameBoard.SetUpNumbersAndTarget(2, 4);

		if (gameBoard.RecursiveSolve())
		{
			t_successCounter++;
		}

		std::cout << "Trial " << l_trialNumber + 1 << " complete.\n";
	}

	std::cout << t_successCounter << " of " << G_NUMBER_OF_TRIALS << " trials were successful.\n";

	std::cin.get();

	return 0;
}

