#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <bit>
#include "output.h"
#include "piecemoves.h"
#include "eval.h"
#include "moveFinder.h"

using namespace std;

int test() {
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::microseconds;

	auto start = chrono::high_resolution_clock::now();

	long long whitepawns = 524535;
	long long whiteknight = 66;
	long long whitebishop = 36;
	long long whiterook = 129;
	long long whitequeen = 16;
	long long whiteking = 8;

	long long blackpawns = 71776119061217280;
	long long blackknight = 4755801206503243776;
	long long blackbishop = 2594073385365405696;
	long long blackrook = 9295429630892703744;
	long long blackqueen = 1152921504606846976;
	long long blackking = 576460752303423488;

	vector<long long> whitePieces = { whitepawns, whiteknight, whitebishop, whiterook, whitequeen, whiteking };
	vector<long long> blackPieces = { blackpawns, blackknight, blackbishop, blackrook, blackqueen, blackking };
	vector<vector<long long>> startPosition = { whitePieces, blackPieces };

	printBinaryGrid(whitepawns);


	return 0;
}