#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>
#include <bit>
#include <utility>
#include "output.h"

using namespace std;

long long bitMap(vector<long long> whitePieces, vector<long long> blackPieces) {
	long long temp = 0;
	for (long long piece : whitePieces) {
		temp += piece;
	}
	for (long long piece : blackPieces) {
		temp += piece;
	}

	return temp;
}

long long getColision(long long moveEnd, vector<long long> pieces) {
	long long bitboard = 0;

	for (long long x : pieces) {
		bitboard += x;
	}

	return ((bitboard & moveEnd) > 0);
}

vector<long long> getPositions(long long pieceBitmap) {
	vector<long long> indivPiecePos;

	for (int i = 0; i < 64; i++) {
		long long temp = 1LL << i;
		if ((pieceBitmap & temp) != 0) {
			indivPiecePos.push_back(temp);
		}
	}
	return indivPiecePos;
}

vector<long long> whitepawnMoves(long long pawnsPos, vector<long long> whitePieces, vector<long long> blackPieces) {
	std::vector<long long> indivPawnsPos = getPositions(pawnsPos);
	std::vector<long long> moveArr;

	for (long long x : indivPawnsPos) {
		int idx = std::countr_zero((unsigned long long)x);
		int file = idx % 8;
		int rank = idx / 8;

		if (rank < 7) {
			long long fwd1 = 1LL << (idx + 8);
			// a straight push is blocked by ANY piece on that square, friend or foe
			if (!getColision(fwd1, whitePieces) && !getColision(fwd1, blackPieces)) {
				moveArr.push_back(fwd1 + x);

				// double push only legal from the starting rank (rank index 1 = rank 2),
				// and only if the intermediate square was itself empty (checked above)
				if (rank == 1) {
					long long fwd2 = 1LL << (idx + 16);
					if (!getColision(fwd2, whitePieces) && !getColision(fwd2, blackPieces)) {
						moveArr.push_back(fwd2 + x);
					}
				}
			}

			// diagonal captures (pawns can only move here if an enemy piece is present)
			if (file > 0) {
				long long capLeft = 1LL << (idx + 7); // rank+1, file-1
				if (getColision(capLeft, blackPieces)) {
					moveArr.push_back(capLeft + x);
				}
			}
			if (file < 7) {
				long long capRight = 1LL << (idx + 9); // rank+1, file+1
				if (getColision(capRight, blackPieces)) {
					moveArr.push_back(capRight + x);
				}
			}
		}
	}

	return moveArr;
}

vector<long long> blackpawnMoves(long long pawnsPos, vector<long long> whitePieces, vector<long long> blackPieces) {
	std::vector<long long> indivPawnsPos = getPositions(pawnsPos);
	std::vector<long long> moveArr;

	for (long long x : indivPawnsPos) {
		int idx = std::countr_zero((unsigned long long)x);
		int file = idx % 8;
		int rank = idx / 8;

		if (rank > 0) {
			long long fwd1 = 1LL << (idx - 8);
			if (!getColision(fwd1, whitePieces) && !getColision(fwd1, blackPieces)) {
				moveArr.push_back(fwd1 + x);

				// double push only from the starting rank (rank index 6 = rank 7)
				if (rank == 6) {
					long long fwd2 = 1LL << (idx - 16);
					if (!getColision(fwd2, whitePieces) && !getColision(fwd2, blackPieces)) {
						moveArr.push_back(fwd2 + x);
					}
				}
			}

			if (file > 0) {
				long long capLeft = 1LL << (idx - 9); // rank-1, file-1
				if (getColision(capLeft, whitePieces)) {
					moveArr.push_back(capLeft + x);
				}
			}
			if (file < 7) {
				long long capRight = 1LL << (idx - 7); // rank-1, file+1
				if (getColision(capRight, whitePieces)) {
					moveArr.push_back(capRight + x);
				}
			}
		}
	}

	return moveArr;
}

vector<long long> bishopMoves(long long bishopPos, vector<long long> yourPieces, vector<long long> oppPieces) {
	vector<long long> indivBishopPos = getPositions(bishopPos);
	vector<long long> moveArr;

	for (long long x : indivBishopPos) {
		int idx = std::countr_zero((unsigned long long)x);
		int file = idx % 8;
		int rank = idx / 8;

		{
			int cur = idx;
			int steps = min(7 - file, 7 - rank);
			for (int i = 0; i < steps; i++) {
				cur += 9;
				long long temp = 1LL << cur;
				if (getColision(temp, yourPieces)) break;
				bool hitOpp = getColision(temp, oppPieces);
				moveArr.push_back(temp + x);
				if (hitOpp) break;
			}
		}
		{
			int cur = idx;
			int steps = min(file, 7 - rank);
			for (int i = 0; i < steps; i++) {
				cur += 7;
				long long temp = 1LL << cur;
				if (getColision(temp, yourPieces)) break;
				bool hitOpp = getColision(temp, oppPieces);
				moveArr.push_back(temp + x);
				if (hitOpp) break;
			}
		}
		{
			int cur = idx;
			int steps = min(7 - file, rank);
			for (int i = 0; i < steps; i++) {
				cur -= 7;
				long long temp = 1LL << cur;
				if (getColision(temp, yourPieces)) break;
				bool hitOpp = getColision(temp, oppPieces);
				moveArr.push_back(temp + x);
				if (hitOpp) break;
			}
		}
		{
			int cur = idx;
			int steps = min(file, rank);
			for (int i = 0; i < steps; i++) {
				cur -= 9;
				long long temp = 1LL << cur;
				if (getColision(temp, yourPieces)) break;
				bool hitOpp = getColision(temp, oppPieces);
				moveArr.push_back(temp + x);
				if (hitOpp) break;
			}
		}
	}

	return moveArr;
}

vector<long long> knightMoves(long long knightPos, vector<long long> yourPieces, vector<long long> oppPieces) {
	vector<long long> indivKnightsPos = getPositions(knightPos);
	vector<long long> moveArr;

	for (long long pos : indivKnightsPos) {
		int idx = std::countr_zero((unsigned long long)pos);
		int x = 8 - (idx % 8);
		int y = (idx / 8) + 1;

		int left = x - 1;
		int right = 8 - x;
		int up = 8 - y;
		int down = y - 1;

		if (right > 1 && up > 0) { long long d = 1LL << (idx + 6);  if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }
		if (left > 1 && up > 0) { long long d = 1LL << (idx + 10); if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }
		if (right > 0 && up > 1) { long long d = 1LL << (idx + 15); if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }
		if (left > 0 && up > 1) { long long d = 1LL << (idx + 17); if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }
		if (left > 1 && down > 0) { long long d = 1LL << (idx - 6);  if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }
		if (right > 1 && down > 0) { long long d = 1LL << (idx - 10); if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }
		if (left > 0 && down > 1) { long long d = 1LL << (idx - 15); if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }
		if (right > 0 && down > 1) { long long d = 1LL << (idx - 17); if (!getColision(d, yourPieces)) { moveArr.push_back(pos + d); } }

	}

	return moveArr;
}

vector<long long> rookMoves(long long rookPos, vector<long long> yourPieces, vector<long long> oppPieces) {
	vector<long long> indivRooksPos = getPositions(rookPos);
	vector<long long> moveArr;

	for (long long pos : indivRooksPos) {
		int idx = std::countr_zero((unsigned long long)pos);
		int x = 8 - (idx % 8);
		int y = (idx / 8) + 1;

		int left = x - 1;
		int right = 8 - x;
		int up = 8 - y;
		int down = y - 1;

		int cur = idx;
		for (int i = 0; i < left; i++) {
			cur += 1;
			long long temp = 1LL << cur;
			if (getColision(temp, yourPieces)) { break; }
			else if (getColision(temp, oppPieces)) {
				moveArr.push_back(pos + temp);
				break;
			}
			moveArr.push_back(pos + temp);
		}

		cur = idx;
		for (int i = 0; i < right; i++) {
			cur -= 1;
			long long temp = 1LL << cur;
			if (getColision(temp, yourPieces)) { break; }
			else if (getColision(temp, oppPieces)) {
				moveArr.push_back(pos + temp);
				break;
			}
			moveArr.push_back(pos + temp);
		}

		cur = idx;
		for (int i = 0; i < up; i++) {
			cur += 8;
			long long temp = 1LL << cur;
			if (getColision(temp, yourPieces)) { break; }
			else if (getColision(temp, oppPieces)) {
				moveArr.push_back(pos + temp);
				break;
			}
			moveArr.push_back(pos + temp);
		}

		cur = idx;
		for (int i = 0; i < down; i++) {
			cur -= 8;
			long long temp = 1LL << cur;
			if (getColision(temp, yourPieces)) { break; }
			else if (getColision(temp, oppPieces)) {
				moveArr.push_back(pos + temp);
				break;
			}
			moveArr.push_back(pos + temp);
		}
	}
	return moveArr;
}

vector<long long> queenMoves(long long queenPos, vector<long long> yourPieces, vector<long long> oppPieces) {
	vector<long long> moveArr;

	vector<long long> diagmoves = bishopMoves(queenPos, yourPieces, oppPieces);
	vector<long long> sidemoves = rookMoves(queenPos, yourPieces, oppPieces);

	for (long long x : diagmoves) {
		moveArr.push_back(x);
	}
	for (long long x : sidemoves) {
		moveArr.push_back(x);
	}

	return moveArr;
}

vector<long long> kingMoves(long long kingPos, vector<long long> yourPieces, vector<long long> oppPieces) {
	vector<long long> moveArr;

	int idx = std::countr_zero((unsigned long long)kingPos);
	int x = 8 - (idx % 8);
	int y = (idx / 8) + 1;

	int left = x - 1;
	int right = 8 - x;
	int up = 8 - y;
	int down = y - 1;

	if (left > 0 && !(getColision(kingPos * 2, yourPieces))) { moveArr.push_back(kingPos + (kingPos * 2)); }
	if (right > 0 && !(getColision(kingPos / 2, yourPieces))) { moveArr.push_back(kingPos + (kingPos / 2)); }
	if (up > 0 && !(getColision(kingPos * 256, yourPieces))) { moveArr.push_back(kingPos + (kingPos * 256)); }
	if (down > 0 && !(getColision(kingPos / 256, yourPieces))) { moveArr.push_back(kingPos + (kingPos / 256)); }

	if (left > 0 && up > 0 && !(getColision(kingPos * 512, yourPieces))) { moveArr.push_back(kingPos + (kingPos * 512)); }
	if (right > 0 && up > 0 && !(getColision(kingPos * 128, yourPieces))) { moveArr.push_back(kingPos + (kingPos * 128)); }
	if (right > 0 && down > 0 && !(getColision(kingPos / 512, yourPieces))) { moveArr.push_back(kingPos + (kingPos / 512)); }
	if (left > 0 && down > 0 && !(getColision(kingPos / 128, yourPieces))) { moveArr.push_back(kingPos + (kingPos / 128)); }

	return moveArr;
}

// Applies a move (src+dst combined into one bitmask, same encoding used everywhere
// else in this codebase) to a COPY of the board and returns the resulting
// {yourPieces, oppPieces}. Doesn't touch the originals.
static pair<vector<long long>, vector<long long>> applyMove(long long move, vector<long long> yourPieces, vector<long long> oppPieces) {
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

	return { mover, opp };
}

// True if yourPieces' king is currently attacked by any of oppPieces' pseudo-legal moves.
bool inCheck(vector<long long> yourPieces, vector<long long> oppPieces, bool white) {
	vector<long long> oppMoves = getMoves(oppPieces, yourPieces, !white);
	long long kingPos = yourPieces[5];

	for (long long m : oppMoves) {
		if (m & kingPos) {
			return true;
		}
	}
	return false;
}

// Pseudo-legal moves, filtered down to ones that don't leave your own king in check.
// This is what the search should actually call.
vector<long long> getLegalMoves(vector<long long> yourPieces, vector<long long> oppPieces, bool white) {
	vector<long long> pseudoLegal = getMoves(yourPieces, oppPieces, white);
	vector<long long> legal;

	for (long long m : pseudoLegal) {
		pair<vector<long long>, vector<long long>> result = applyMove(m, yourPieces, oppPieces);
		if (!inCheck(result.first, result.second, white)) {
			legal.push_back(m);
		}
	}

	return legal;
}

vector<long long> getMoves(vector<long long> yourPieces, vector<long long> oppPieces, bool white) {
	vector<long long> legalMoves;

	if (white) {
		vector<long long> pawns = whitepawnMoves(yourPieces[0], yourPieces, oppPieces);
		vector<long long> bishop = bishopMoves(yourPieces[2], yourPieces, oppPieces);
		vector<long long> knight = knightMoves(yourPieces[1], yourPieces, oppPieces);
		vector<long long> rook = rookMoves(yourPieces[3], yourPieces, oppPieces);
		vector<long long> queen = queenMoves(yourPieces[4], yourPieces, oppPieces);
		vector<long long> king = kingMoves(yourPieces[5], yourPieces, oppPieces);

		for (long long x : pawns) { legalMoves.push_back(x); }
		for (long long x : bishop) { legalMoves.push_back(x); }
		for (long long x : knight) { legalMoves.push_back(x); }
		for (long long x : rook) { legalMoves.push_back(x); }
		for (long long x : queen) { legalMoves.push_back(x); }
		for (long long x : king) { legalMoves.push_back(x); }
	}
	else {
		vector<long long> pawns = blackpawnMoves(yourPieces[0], oppPieces, yourPieces);
		vector<long long> bishop = bishopMoves(yourPieces[2], yourPieces, oppPieces);
		vector<long long> knight = knightMoves(yourPieces[1], yourPieces, oppPieces);
		vector<long long> rook = rookMoves(yourPieces[3], yourPieces, oppPieces);
		vector<long long> queen = queenMoves(yourPieces[4], yourPieces, oppPieces);
		vector<long long> king = kingMoves(yourPieces[5], yourPieces, oppPieces);

		for (long long x : pawns) { legalMoves.push_back(x); }
		for (long long x : bishop) { legalMoves.push_back(x); }
		for (long long x : knight) { legalMoves.push_back(x); }
		for (long long x : rook) { legalMoves.push_back(x); }
		for (long long x : queen) { legalMoves.push_back(x); }
		for (long long x : king) { legalMoves.push_back(x); }
	}

	return legalMoves;
}
