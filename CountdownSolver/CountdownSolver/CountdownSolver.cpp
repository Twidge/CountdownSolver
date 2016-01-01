// CountdownSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Board.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	Board gameBoard;

	gameBoard.SetUpNumbersAndTarget(2, 4);

	if (gameBoard.RecursiveSolve())
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

