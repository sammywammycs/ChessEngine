#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>
#include <bit>


using namespace std;

float pawnClosetoCentre(long long pawnPos) {
	int x = 8 - fmod(log2(pawnPos), 8);
	int y = std::floor(log2(pawnPos) / 8) + 1;

	if ((x == 4) || (x == 5) && (y == 4)) {
		return 1.5;
	}

}

float eval(std::vector<long long> whitePieces, std::vector<long long> blackPieces) {
	float evaluation = 0;
	vector<long long> whitePawnPositions = getPositions(whitePieces[0]);
	vector<long long> blackPawnPositions = getPositions(blackPieces[0]);

	for (long long x : whitePawnPositions) {
		evaluation += 1 * pawnClosetoCentre(x);
	}

	for (long long x : blackPawnPositions) {
		evaluation -= 1 * pawnClosetoCentre(x);
	}

	evaluation += 3 * popcount(static_cast<unsigned long long>(whitePieces[1]));
	evaluation += 3 * popcount(static_cast<unsigned long long>(whitePieces[2]));
	evaluation += 5 * popcount(static_cast<unsigned long long>(whitePieces[3]));
	evaluation += 9 * popcount(static_cast<unsigned long long>(whitePieces[4]));

	evaluation -= 3 * popcount(static_cast<unsigned long long>(blackPieces[1]));
	evaluation -= 3 * popcount(static_cast<unsigned long long>(blackPieces[2]));
	evaluation -= 5 * popcount(static_cast<unsigned long long>(blackPieces[3]));
	evaluation -= 9 * popcount(static_cast<unsigned long long>(blackPieces[4]));

	return evaluation;
}