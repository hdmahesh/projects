#include <iostream>
#include <stdio.h>
#include <vector>

#if 0
// Failed Attempt :-(
void
printMatrix(
	std::vector<std::vector<int> >& arr,
	bool horizontal, bool forward,
	int row, int col, int maxIndex)
{
	// printf ("\n row = %d, col = %d\n", row, col);
	int startIndex = horizontal ? col : row;
	for (int index = startIndex;;) {
		if (forward) {
			if (index > maxIndex) break;
		} else {
			if (index < maxIndex) break;
		}

		if (horizontal) {
			std::cout << arr[row][index] << " ";
		} else {
			std::cout << arr[index][col] << " ";
		}

		int incr = forward ? 1 : -1;
		index += incr;
	}
	// std::cout << std::endl << "----" << std::endl;
}

int
main()
{
	std::vector<std::vector<int> > arr;
	int rlimit = 1;
	int climit = 2;
	for (int r = 0; r < rlimit; ++r) {
		arr.push_back(std::vector<int>());
		for (int c = 0; c < climit; ++c) {
			arr[r].push_back(r*climit + c);
		}
	}
	for (int r = 0; r < rlimit; ++r) {
		for (int c = 0; c < climit; ++c) {
			std::cout << arr[r][c] << " ";
		}
		std::cout << std::endl;
	}

	bool debug = false;

	int maxColIndex = climit-1;
	int maxRowIndex = rlimit-1;
	//
	// printMatrix( arr, bool horizontal, bool forward, int row, int col, int maxIndex)
	//
	int rend = rlimit >> 1;
	rend = (rlimit % 2) == 0 ? rend : rend + 1;
	int cend = climit >> 1;
	cend = (climit % 2) == 0 ? cend : cend + 1;
	if (debug) printf ("rend = %d, cend = %d\n", rend, cend);

	for (int row = 0, col = 0; row < rend && col < cend; ++row, ++col) {
		if (debug) printf("\n 1. row = %d, col = %d\n", row, col);

		printMatrix(arr, true,  true,  row,               col,               maxColIndex-col);

		if (row+1 > rend) continue;

		if (debug) printf("\n 2. row = %d, col = %d\n", row + 1, maxColIndex-col);

		printMatrix(arr, false, true,  row + 1,           maxColIndex-col,   maxRowIndex-row);

		if (maxColIndex-col-1 < 0) continue;

		if (debug) printf("\n 3. row = %d, col = %d\n", maxRowIndex - row, maxColIndex-col-1);

		printMatrix(arr, true,  false, maxRowIndex-row,   maxColIndex-col-1, col);

		if (debug) printf("\n 4. row = %d, col = %d\n", maxRowIndex - row - 1, col);

		printMatrix(arr, false, false, maxRowIndex-row-1, col,               row+1);
	}
	std::cout << std::endl;
	return 0;
}
#endif

void
printSpiral(std::vector<std::vector<int> >& arr, int r, int c)
{
	int TR = 0, TC = 0, BR = r-1, BC = c-1;
	int direction = 0;
	while (TR <= BR && TC <= BC) {
		switch (direction) {
			case 0: {
				for (int index = TC; index <= BC; ++index) {
					std::cout << arr[TR][index] << " ";
				}
				++direction;
				++TR;
			} break;
			case 1: {
				for (int index = TR; index <= BR; ++index) {
					std::cout << arr[index][BC] << " ";
				}
				++direction;
				--BC;
			} break;
			case 2: {
				for (int index = BC; index >= TC; --index) {
					std::cout << arr[BR][index] << " ";
				}
				++direction;
				--BR;
			} break;
			case 3: {
				for (int index = BR; index >= TR; --index) {
					std::cout << arr[index][TC] << " ";
				}
				++direction;
				++TC;
			} break;
		}
		direction %= 4;
	}
	std::cout << std::endl;
}

int
main()
{
	std::vector<std::vector<int> > arr;
	int rlimit = 4;
	int climit = 10;
	for (int r = 0; r < rlimit; ++r) {
		arr.push_back(std::vector<int>());
		for (int c = 0; c < climit; ++c) {
			arr[r].push_back(r*climit + c);
		}
	}
	for (int r = 0; r < rlimit; ++r) {
		for (int c = 0; c < climit; ++c) {
			std::cout << arr[r][c] << " ";
		}
		std::cout << std::endl;
	}
	printSpiral(arr, rlimit, climit);
	return 0;
}
