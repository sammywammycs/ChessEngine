#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <bit>
#include "output.h"
#include "piecemoves.h"
#include "eval.h"

using namespace std;

int main() {
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

	float evaluation = eval(whitePieces, blackPieces);
	vector<long long> startBoard;
	vector<vector<long long>> boardState;
	vector<vector<long long>> whiteMoves;
	vector<vector<long long>> blackMoves;

	startBoard.push_back(whitePieces[0]);
	startBoard.push_back(whitePieces[1]);
	startBoard.push_back(whitePieces[2]);
	startBoard.push_back(whitePieces[3]);
	startBoard.push_back(whitePieces[4]);
	startBoard.push_back(whitePieces[5]);
	startBoard.push_back(blackPieces[0]);
	startBoard.push_back(blackPieces[1]);
	startBoard.push_back(blackPieces[2]);
	startBoard.push_back(blackPieces[3]);
	startBoard.push_back(blackPieces[4]);
	startBoard.push_back(blackPieces[5]);

	boardState.push_back(startBoard);

	whiteMoves.push_back(pawnMoves(boardState[0][0]));
	
	for (vector<long long> pieces : boardState) {
		whiteMoves.push_back(pawnMoves(pieces[0]));
		whiteMoves.push_back(knightMoves(pieces[1]));
		whiteMoves.push_back(bishopMoves(pieces[2]));
		whiteMoves.push_back(rookMoves(pieces[3]));
		whiteMoves.push_back(queenMoves(pieces[4]));
		whiteMoves.push_back(kingMoves(pieces[5]));
	}


	return 0;
}