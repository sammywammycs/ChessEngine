#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <bit>
#include "output.h"
#include "piecemoves.h"
#include "eval.h"

using namespace std;

int test() {
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::microseconds;

	long long whitepawns = 65280;
	long long whiteknight = 66;
	long long whitebishop = 36;
	long long whiterook = 129;
	long long whitequeen = 16;
	long long whiteking = 8;

	long long blackpawns = 65280;
	long long blackknight = 66;
	long long blackbishop = 36;
	long long blackrook = 129;
	long long blackqueen = 16;
	long long blackking = 8;

	vector<long long> whitePieces = { whitepawns, whiteknight, whitebishop, whiterook, whitequeen, whiteking };
	vector<long long> blackPieces = { blackpawns, blackknight, blackbishop, blackrook, blackqueen, blackking };

	auto start1 = chrono::high_resolution_clock::now();
	std::vector<long long> pawnmoves = pawnMoves(whitepawns);
	auto end1 = chrono::high_resolution_clock::now();
	auto length1 = duration_cast<microseconds>(end1 - start1);

	auto start2 = chrono::high_resolution_clock::now();
	std::vector<long long> bishopmoves = bishopMoves(whitebishop);
	auto end2 = chrono::high_resolution_clock::now();
	auto length2 = duration_cast<microseconds>(end2 - start2);

	auto start3 = chrono::high_resolution_clock::now();
	std::vector<long long> knightmoves = knightMoves(whiteknight);
	auto end3 = chrono::high_resolution_clock::now();
	auto length3 = duration_cast<microseconds>(end3 - start3);

	auto start4 = chrono::high_resolution_clock::now();
	std::vector<long long> rookmoves = rookMoves(whiterook);
	auto end4 = chrono::high_resolution_clock::now();
	auto length4 = duration_cast<microseconds>(end4 - start4);

	auto start5 = chrono::high_resolution_clock::now();
	std::vector<long long> queenmoves = queenMoves(whitequeen);
	auto end5 = chrono::high_resolution_clock::now();
	auto length5 = duration_cast<microseconds>(end5 - start5);

	auto start6 = chrono::high_resolution_clock::now();
	std::vector<long long> kingmoves = kingMoves(whiteking);
	auto end6 = chrono::high_resolution_clock::now();
	auto length6 = duration_cast<microseconds>(end6 - start6);

	cout << "pawns took " << length1.count() << "microseconds" << endl;
	cout << "bishops took " << length2.count() << "microseconds" << endl;
	cout << "knights took " << length3.count() << "microseconds" << endl;
	cout << "rooks took " << length4.count() << "microseconds" << endl;
	cout << "queen took " << length5.count() << "microseconds" << endl;
	cout << "king took " << length6.count() << "microseconds" << endl;
}