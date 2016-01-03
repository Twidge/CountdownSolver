// CountdownSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <ctime>

int _tmain(int argc, _TCHAR* argv[])
{
	srand(static_cast<unsigned int>(time(NULL)));

	Board gameBoard;

	gameBoard.SetUpNumbersAndTarget(2, 4);

	std::cout << gameBoard;

	bool t_canBeSolved = gameBoard.RecursiveSolve();

	if (t_canBeSolved)
	{
		std::cout << "Board can be solved.\n";
	}

	else
	{
		std::cout << "Board cannot be solved.\n";
	}

	std::cin.get();

	return 0;
}

