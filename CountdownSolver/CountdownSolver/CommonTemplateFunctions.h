#include <vector>
#include <iostream>

template<typename T>
std::vector<std::vector<T>> GenerateSubsets(std::vector<T> const& set, int targetSize)
{
#ifdef DEBUG
	std::cout << "Calling GenerateSubsets, set size is: " << set.size() << ", target size is: " << targetSize << ".\n";
#endif

	unsigned int t_maxSize = 31;

	std::vector<std::vector<T>> end = std::vector<std::vector<T>>();

	if (set.size() > t_maxSize)
	{
		#ifdef DEBUG
		std::cout << "Set size greater than 31. GenerateSubsets() function cancelled.";
		#endif

		return end;
	}

	for (int i = 0; i < pow(2.0, double(set.size())); i++)
	{
		int onBits = 0;

		for (unsigned int j = 1; j <= set.size(); j++)
		{
			if ((i % (int)pow(2.0, j)) - (i % (int)pow(2.0, j - 1)) == pow(2.0, j - 1))
				onBits++;
		}

		if (onBits == targetSize)
		{
			std::vector<T> foo = std::vector<T>();

			for (unsigned int j = 1; j <= set.size(); j++)
			{
				if ((i % (int)pow(2.0, j)) - (i % (int)pow(2.0, j - 1)) == pow(2.0, j - 1))
					foo.push_back(set[j - 1]);
			}

			end.push_back(foo);
		}
	}

#ifdef DEBUG
	std::cout << "GenerateSubsets returning.\n";
#endif

	return end;
}

template<typename T>
std::vector<T> SetComplement(std::vector<T> parentSet, std::vector<T> subset)
{
	std::vector<int> t_indicesUsed = std::vector<int>();
	bool t_subsetIsNotASubset = false;

	for (unsigned int l_subsetIndex = 0; l_subsetIndex < subset.size(); l_subsetIndex++)
	{
		for (unsigned int l_parentSetIndex = 0; l_parentSetIndex < parentSet.size(); l_parentSetIndex++)
		{
			t_subsetIsNotASubset = true;

			if (subset[l_subsetIndex] == parentSet[l_parentSetIndex])
			{
				bool t_indexValid = true;

				for (unsigned int l_indicesUsedIndex = 0; l_indicesUsedIndex < t_indicesUsed.size(); l_indicesUsedIndex++)
				{
					if (l_parentSetIndex == t_indicesUsed[l_indicesUsedIndex])
					{
						t_indexValid = false;
					}
				}

				if (t_indexValid)
				{
					t_indicesUsed.push_back(l_parentSetIndex);
					t_subsetIsNotASubset = false;
					break;
				}
			}
		}
	}

	if (t_subsetIsNotASubset)
	{
		std::cout << "Error in SetComplement(): subset is not actually a subset of parent set!\n";
		std::cin.get();
	}

	std::vector<T> t_setComplement = std::vector<T>();

	for (unsigned int l_parentSetIndex = 0; l_parentSetIndex < parentSet.size(); l_parentSetIndex++)
	{
		bool t_shouldIncludeInComplement = true;

		for (unsigned int l_indicesUsedIndex = 0; l_indicesUsedIndex < t_indicesUsed.size(); l_indicesUsedIndex++)
		{
			if (l_parentSetIndex == t_indicesUsed[l_indicesUsedIndex])
			{
				t_shouldIncludeInComplement = false;
				break;
			}
		}

		if (t_shouldIncludeInComplement)
		{
			t_setComplement.push_back(parentSet[l_parentSetIndex]);
		}
	}

	return t_setComplement;
}

template<typename T>
void UnsortedAddIfDistinct(T const& toBeAdded, std::vector<T>& targetVector)
{
	bool t_shouldAdd = true;

	for (std::vector<T>::iterator nextIndexToCheck = targetVector.begin(); nextIndexToCheck != targetVector.end(); nextIndexToCheck++)
	{
		if (*nextIndexToCheck == toBeAdded)
		{
			t_shouldAdd = false;
			break;
		}
	}

	if (t_shouldAdd)
	{
		targetVector.push_back(toBeAdded);
	}
}