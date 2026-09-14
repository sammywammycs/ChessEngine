#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>
#include <bit>
#include "output.h"


using namespace std;

float pawnClosetoCentre(long long pawnPos) {
	int x = 8 - fmod(log2(pawnPos), 8);
	int y = std::floor(log2(pawnPos) / 8) + 1;

	if ((x == 4 || x == 5) && (y == 4 || y == 5)) {
		return 1.5;
	}
	else {
		return 1;
	}
}

int eval(vector<long long> whitePieces, vector<long long> blackPieces) {
	int evaluation = 0;
	vector<long long> whitePawnPositions = getPositions(whitePieces[0]);
	vector<long long> blackPawnPositions = getPositions(blackPieces[0]);

	for (long long x : whitePawnPositions) {
		evaluation += 100 * pawnClosetoCentre(x);
	}

	for (long long x : blackPawnPositions) {
		evaluation -= 100 * pawnClosetoCentre(x);
	}

	evaluation += 300 * popcount(static_cast<unsigned long long>(whitePieces[1]));
	evaluation += 300 * popcount(static_cast<unsigned long long>(whitePieces[2]));
	evaluation += 500 * popcount(static_cast<unsigned long long>(whitePieces[3]));
	evaluation += 900 * popcount(static_cast<unsigned long long>(whitePieces[4]));

	evaluation -= 300 * popcount(static_cast<unsigned long long>(blackPieces[1]));
	evaluation -= 300 * popcount(static_cast<unsigned long long>(blackPieces[2]));
	evaluation -= 500 * popcount(static_cast<unsigned long long>(blackPieces[3]));
	evaluation -= 900 * popcount(static_cast<unsigned long long>(blackPieces[4]));

	return evaluation;
}