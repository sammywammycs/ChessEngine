#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <bit>
#include "output.h"
#include "piecemoves.h"
#include "eval.h"

using namespace std;
long long constant = long long (256);


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

	long long blackpawns = 71776119061217280;
	long long blackknight = 4755801206503243776;
	long long blackbishop = 5188146770730811392;
	long long blackrook = 9295429630892703744;
	long long blackqueen = 1152921504606846976;
	long long blackking = 2305843009213693952;

	vector<long long> whitePieces = { whitepawns, whiteknight, whitebishop, whiterook, whitequeen, whiteking };
	vector<long long> blackPieces = { blackpawns, blackknight, blackbishop, blackrook, blackqueen, blackking };
	vector<vector<vector<long long>>> boardState;
	vector<vector<long long>> startPosition = { whitePieces, blackPieces };


	auto start1 = chrono::high_resolution_clock::now();
	std::vector<long long> pawnmoves = whitepawnMoves(whitepawns, whitePieces, blackPieces);
	auto end1 = chrono::high_resolution_clock::now();
	auto length1 = duration_cast<microseconds>(end1 - start1);

	auto start2 = chrono::high_resolution_clock::now();
	std::vector<long long> bishopmoves = bishopMoves(whitebishop, whitePieces, blackPieces);
	auto end2 = chrono::high_resolution_clock::now();
	auto length2 = duration_cast<microseconds>(end2 - start2);

	auto start3 = chrono::high_resolution_clock::now();
	std::vector<long long> knightmoves = knightMoves(whiteknight, whitePieces, blackPieces);
	auto end3 = chrono::high_resolution_clock::now();
	auto length3 = duration_cast<microseconds>(end3 - start3);

	auto start4 = chrono::high_resolution_clock::now();
	std::vector<long long> rookmoves = rookMoves(whiterook, whitePieces, blackPieces);
	auto end4 = chrono::high_resolution_clock::now();
	auto length4 = duration_cast<microseconds>(end4 - start4);

	auto start5 = chrono::high_resolution_clock::now();
	std::vector<long long> queenmoves = queenMoves(whitequeen, whitePieces, blackPieces);
	auto end5 = chrono::high_resolution_clock::now();
	auto length5 = duration_cast<microseconds>(end5 - start5);

	auto start6 = chrono::high_resolution_clock::now();
	std::vector<long long> kingmoves = kingMoves(whiteking, whitePieces, blackPieces);
	auto end6 = chrono::high_resolution_clock::now();
	auto length6 = duration_cast<microseconds>(end6 - start6);

	std::cout << "pawns took " << length1.count() << "microseconds" << endl;
	std::cout << "bishops took " << length2.count() << "microseconds" << endl;
	std::cout << "knights took " << length3.count() << "microseconds" << endl;
	std::cout << "rooks took " << length4.count() << "microseconds" << endl;
	std::cout << "queen took " << length5.count() << "microseconds" << endl;
	std::cout << "king took " << length6.count() << "microseconds" << endl;

	vector<vector<long long>> moves;
	vector<long long> temp;

	float best = 0;
	long long bestMove;

	moves.push_back(temp);
	moves.push_back(temp);
	moves.push_back(temp);
	moves.push_back(temp);
	moves.push_back(temp);
	moves.push_back(temp);

	for (long long x : pawnmoves) {
		moves[0].push_back(x);
	}
	for (long long x : knightmoves) {
		moves[1].push_back(x);
	}
	for (long long x : bishopmoves) {
		moves[2].push_back(x);
	}
	for (long long x : rookmoves) {
		moves[3].push_back(x);
	}
	for (long long x : queenmoves) {
		moves[4].push_back(x);
	}
	for (long long x : kingmoves) {
		moves[5].push_back(x);
	}

	for (long long x : moves[0]) {
		long long temp = startPosition[0][0] ^ x;
		vector<long long> tempWhitePos = startPosition[0]; tempWhitePos[0] = temp;
		float attemptEval = eval(tempWhitePos, startPosition[1]);

		if (attemptEval > best){
			bestMove = x;
			best = attemptEval;
		}
	}



	cout << "Evaluation before: " << eval(whitePieces, blackPieces) << endl;
	cout << "Evaluation after: " << best << endl;

	auto end = chrono::high_resolution_clock::now();
	auto length = duration_cast<microseconds>(end - start);

	cout << "Evalutation took: " << length << "microseconds" << endl;

	long long bitboardBefore = 0;

	for (long long x : whitePieces) {
		bitboardBefore += x;
	}
	for (long long x : blackPieces) {
		bitboardBefore += x;
	}

	printBinaryGrid(bitboardBefore);

	return 0;
}