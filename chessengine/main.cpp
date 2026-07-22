#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <bit>
#include "output.h"
#include "piecemoves.h"

using namespace std;

float eval(long long whitePieces[6]) {
	float evaluation = 0;
	
	evaluation += 1 * popcount(static_cast<unsigned long long>(whitePieces[0]));
	evaluation += 3 * popcount(static_cast<unsigned long long>(whitePieces[1]));
	evaluation += 3 * popcount(static_cast<unsigned long long>(whitePieces[2]));
	evaluation += 5 * popcount(static_cast<unsigned long long>(whitePieces[3]));
	evaluation += 9 * popcount(static_cast<unsigned long long>(whitePieces[4]));

	return evaluation;
}

int main() {
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::microseconds;

	auto start = chrono::high_resolution_clock::now();

	long long whitepawns = 65280;
	long long whiteknight = 66;
	long long whitebishop = 36;
	long long whiterook = 129;
	long long whitequeen = 16;
	long long whiteking = 8;

	long long whitePosition[6] = { whitepawns, whiteknight, whitebishop, whiterook, whitequeen, whiteking };

	auto start1 = chrono::high_resolution_clock::now();
	std::vector<long long> pawnmoves = pawnMoves(whitepawns);
	auto end1 = chrono::high_resolution_clock::now();
	auto length1 = duration_cast<microseconds>(end1 - start1);

	auto start2 = chrono::high_resolution_clock::now();
	std::vector<long long> bishopmoves = bishopMoves(whitebishop);
	auto end2 = chrono::high_resolution_clock::now();
	auto length2 = duration_cast<microseconds>(end2 - start2);

	cout << "pawns took " << length1.count() << "microsends" << endl;
	cout << "bishops took " << length2.count() << "microsends" << endl;

	return 0;
}