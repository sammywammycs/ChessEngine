#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>
#include <bit>
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
		long long temp = pow(2, i);
		if ((pieceBitmap & temp) > 0) {
			indivPiecePos.push_back(temp);
		}
	}
	return indivPiecePos;
}

vector<long long> whitepawnMoves(long long pawnsPos, vector<long long> whitePieces, vector<long long> blackPieces) {
	std::vector<long long> indivPawnsPos = getPositions(pawnsPos);
	std::vector<long long> moveArr;

	for (long long x : indivPawnsPos) {
		if (getColision(x * 256, whitePieces)) { ; } 
		else { 
			moveArr.push_back((x * 256) + x); 
			if (getColision(x * 256 * 256, whitePieces)) { ; } else { moveArr.push_back((x * 256 * 256) + x); }
		}
	}

	return moveArr;
}

vector<long long> blackpawnMoves(long long pawnsPos, vector<long long> whitePieces, vector<long long> blackPieces) {
	std::vector<long long> indivPawnsPos = getPositions(pawnsPos);

	std::vector<long long> moveArr;

	for (long long x : indivPawnsPos) {
		moveArr.push_back((x / 256) + x);
		moveArr.push_back((x / 256 / 256) + x);
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
			long long temp = x;
			int steps = min(7 - file, 7 - rank);
			for (int i = 0; i < steps; i++) {
				temp *= 512;
				if (getColision(temp, yourPieces)) break;
				bool hitOpp = getColision(temp, oppPieces);
				moveArr.push_back(temp + x);
				if (hitOpp) break;
			}
		}
		{
			long long temp = x;
			int steps = min(file, 7 - rank);
			for (int i = 0; i < steps; i++) {
				temp *= 128;
				if (getColision(temp, yourPieces)) break;
				bool hitOpp = getColision(temp, oppPieces);
				moveArr.push_back(temp + x);
				if (hitOpp) break;
			}
		}
		{
			long long temp = x;
			int steps = min(7 - file, rank);
			for (int i = 0; i < steps; i++) {
				temp /= 128;
				if (getColision(temp, yourPieces)) break;
				bool hitOpp = getColision(temp, oppPieces);
				moveArr.push_back(temp + x);
				if (hitOpp) break;
			}
		}
		{
			long long temp = x;
			int steps = min(file, rank);
			for (int i = 0; i < steps; i++) {
				temp /= 512;
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
		int x = 8 - fmod(log2(pos), 8);
		int y = std::floor(log2(pos) / 8) + 1;

		int left = x - 1;
		int right = 8 - x;
		int up = 8 - y;
		int down = y - 1;

		if (right > 1 && up > 0) { if (!getColision(pos * 64, yourPieces)) { moveArr.push_back(pos + (pos * 64)); } }
		if (left > 1 && up > 0) { if (!getColision(pos * 1024, yourPieces)) { moveArr.push_back(pos + (pos * 1024)); } }
		if (right > 0 && up > 1) { if (!getColision(pos * 32768, yourPieces)) { moveArr.push_back(pos + (pos * 32768)); } }
		if (left > 0 && up > 1) { if (!getColision(pos * 131072, yourPieces)) { moveArr.push_back(pos + (pos * 131072)); } }
		if (left > 1 && down > 0) { if (!getColision(pos / 64, yourPieces)) { moveArr.push_back(pos + (pos / 64)); } }
		if (right > 1 && down > 0) { if (!getColision(pos / 1024, yourPieces)) { moveArr.push_back(pos + (pos / 1024)); } } 
		if (left > 0 && down > 1) { if (!getColision(pos / 32768, yourPieces)) { moveArr.push_back(pos + (pos / 32768)); } }
		if (right > 0 && down > 1) { if (!getColision(pos / 131072, yourPieces)) { moveArr.push_back(pos + (pos / 131072)); } }

	}

	return moveArr;
}

vector<long long> rookMoves(long long rookPos, vector<long long> yourPieces, vector<long long> oppPieces) {
	vector<long long> indivRooksPos = getPositions(rookPos);
	vector<long long> moveArr;

	for (long long pos : indivRooksPos) {
		int x = 8 - fmod(log2(pos), 8);
		int y = std::floor(log2(pos) / 8) + 1;

		int left = x - 1;
		int right = 8 - x;
		int up = 8 - y;
		int down = y - 1;

		long long temp = 2;
		long long temp2 = 256;

		for (int i = 0; i < left; i++) {
			if (getColision(pos * temp, yourPieces)) { break; }
			else if (getColision(pos*temp, oppPieces)){
				moveArr.push_back(pos + (pos * temp));
				break;
			}
			moveArr.push_back(pos + (pos * temp));
			temp *= 2;
		}

		temp = 2;

		for (int i = 0; i < right; i++) {
			if (getColision(pos / temp, yourPieces)) { break; }
			else if (getColision(pos / temp, oppPieces)) {
				moveArr.push_back(pos + (pos / temp));
				break;
			}
			moveArr.push_back(pos + (pos / temp));
			temp *= 2;
		}

		for (int i = 0; i < up; i++) {
			if (getColision(pos * temp2, yourPieces)) { break; }
			else if (getColision(pos * temp2, oppPieces)) {
				moveArr.push_back(pos + (pos * temp2));
				break;
			}
			moveArr.push_back(pos + (pos * temp2));
			temp2 *= 256;
		}

		temp2 = 256;

		for (int i = 0; i < down; i++) {
			if (getColision(pos / temp2, yourPieces)) { break; }
			else if (getColision(pos / temp2, oppPieces)) {
				moveArr.push_back(pos + (pos / temp2));
				break;
			}
			moveArr.push_back(pos + (pos / temp2));
			temp2 *= 256;
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

	int x = 8 - fmod(log2(kingPos), 8);
	int y = std::floor(log2(kingPos) / 8) + 1;

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

vector<long long> getMoves(vector<long long> yourPieces, vector<long long> oppPieces, bool white) {
	vector<long long> legalMoves;

	if (white) {
		vector<long long> pawns = whitepawnMoves(yourPieces[0], yourPieces, oppPieces);
		vector<long long> bishop =  bishopMoves(yourPieces[2], yourPieces, oppPieces);
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
