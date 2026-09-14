#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>
#include "eval.h"
#include "output.h"
#include <utility>

using namespace std;

const int MAX_PLY = 64;

vector<vector<long long>> pv_table(MAX_PLY, vector<long long>(MAX_PLY));
vector<int> pv_length(MAX_PLY, 0);

pair<vector<long long>, vector<long long>> getTempPosition(long long move, vector<long long> yourPieces, vector<long long> oppPieces, bool white) {
	vector<long long> mover = yourPieces;
	vector<long long> opp;

	bool moved = false;
	for (long long& x : mover) {
		if (!moved && (x & move) != 0 && (x ^ move) != (x | move)) {
			x ^= move;
			moved = true;
		}
	}

	for (long long x : oppPieces) {
		opp.push_back(x & ~move);
	}

	if (white) {
		return { mover, opp };
	}
	else {
		return { mover, opp };
	}
}

pair<int, vector<long long>> findBest(vector<long long> yourPieces, vector<long long> oppPieces, int depth, int ply, int alpha, int beta, bool white) {
	bool turn = (ply % 2 == 0);

	if (depth == 0) {
		pv_length[ply] = ply;
		int score = white ? eval(yourPieces, oppPieces) : -eval(oppPieces, yourPieces);
		return { score, {} };
	}

	vector<long long> moves = getMoves(yourPieces, oppPieces, turn);

	if (moves.empty()) {
		pv_length[ply] = ply;
		int score = white ? eval(yourPieces, oppPieces) : -eval(oppPieces, yourPieces);
		return { score, {} };
	}

	int bestScore = alpha;
	bool foundMove = false;

	for (long long x : moves) {
		pair<vector<long long>, vector<long long>> tempPosition = getTempPosition(x, yourPieces, oppPieces, turn);

		int score = -findBest(tempPosition.second, tempPosition.first, depth - 1, ply + 1, -beta, -bestScore, !white).first;

		if (score > bestScore) {
			bestScore = score;
			foundMove = true;

			pv_table[ply][ply] = x;
			for (int next = ply + 1; next < pv_length[ply + 1]; next++) {
				pv_table[ply][next] = pv_table[ply + 1][next];
			}
			pv_length[ply] = pv_length[ply + 1];

			if (bestScore >= beta) {
				break;
			}
		}
	}

	if (!foundMove) {
		pv_length[ply] = ply;
	}

	return { bestScore, vector<long long>(pv_table[ply].begin() + ply,
										   pv_table[ply].begin() + pv_length[ply]) };
}