#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <bit>
#include "output.h"
#include "piecemoves.h"

using namespace std;

float eval(long long whitePieces[6]) {
	auto start = chrono::high_resolution_clock::now();
	float evaluation = 0;
	
	evaluation += 1 * popcount(static_cast<unsigned long long>(whitePieces[0]));
	evaluation += 3 * popcount(static_cast<unsigned long long>(whitePieces[1]));
	evaluation += 3 * popcount(static_cast<unsigned long long>(whitePieces[2]));
	evaluation += 5 * popcount(static_cast<unsigned long long>(whitePieces[3]));
	evaluation += 9 * popcount(static_cast<unsigned long long>(whitePieces[4]));

	return evaluation;
}

int main() {
	long long whitepawns = 65280;
	long long whiteknight = 66;
	long long whitebishop = 36;
	long long whiterook = 129;
	long long whitequeen = 16;
	long long whiteking = 8;

	long long whitePosition[6] = { whitepawns, whiteknight, whitebishop, whiterook, whitequeen, whiteking };

	cout << pawnMoves(whitepawns)[6] << endl;
	printBinaryGrid(pawnMoves(whitepawns)[6]);
	printBinaryGrid(whitepawns ^ pawnMoves(whitepawns)[6]);


	return 0;
}