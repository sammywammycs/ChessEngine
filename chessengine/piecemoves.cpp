#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>

using namespace std;

vector<long long> getPositions(long long pieceBitmap) {
	vector<long long> indivPiecePos;

	for (int i = 0; i < 64; i++) {
		int temp = pow(2, i);
		if ((pieceBitmap & temp) > 0) {
			indivPiecePos.push_back(temp);
		}
	}
	return indivPiecePos;
}

vector<long long> pawnMoves(long long pawnsPos) {
	std::vector<long long> indivPawnsPos = getPositions(pawnsPos);
	std::vector<long long> moveArr;

	for (long long x : indivPawnsPos) {
		moveArr.push_back((x * 256) + x);
		moveArr.push_back((x * 256 * 256) + x);
	}

	return moveArr;
}

vector<long long> bishopMoves(long long bishopPos) {
	std::vector<long long> indivBishopPos = getPositions(bishopPos);
	std::vector<long long> moveArr;

	for (long long x : indivBishopPos) {
		int logpower = fmod(log2(x),8);
		int logpowerfloor = std::floor(log2(x) / 8);

		long long temp = x * 512;
		long long temp2 = x * 128;
		long long temp3 = x / 512;
		long long temp4 = x / 128;

		for (int i = 1; i < 8 - logpower; i++) {
			moveArr.push_back(temp + x);
			temp *= 512;
		}

		for (int i = 1; i < logpower + 1; i++) {
			moveArr.push_back(temp2 + x);
			temp2 *= 128;
		}

		int lim1 = min(logpower, logpowerfloor);
		int lim2 = min(8 - logpower, logpowerfloor);

		for (int i = 0; i < lim1; i++) {
			moveArr.push_back(temp3 + x);
			temp3 /= 512;
		}

		for (int i = 0; i < lim1; i++) {
			moveArr.push_back(temp4 + x);
			temp4 /= 128;
		}

	}
	return moveArr;
}

vector<long long> knightMoves(long long knightPos) {
	vector<long long> indivKnightsPos = getPositions(knightPos);
	vector<long long> moveArr;
	
	for (long long pos : indivKnightsPos) {
		int x = 8 - fmod(log2(pos), 8);
		int y = std::floor(log2(pos) / 8) + 1;

		int left = x - 1;
		int right = 8 - x;
		int up = 8 - y;
		int down = y - 1;

		if (right > 1 && up > 0) { moveArr.push_back(pos + (pos * 64)); }
		if (left > 1 && up > 0) { moveArr.push_back(pos + (pos * 1024)); }
		if (right > 0 && up > 1) { moveArr.push_back(pos + (pos * 32768)); }
		if (left > 0 && up > 1) { moveArr.push_back(pos + (pos * 131072)); }
		if (left > 1 && down > 0) { moveArr.push_back(pos + (pos / 64)); }
		if (right > 1 && down > 0) { moveArr.push_back(pos + (pos / 1024)); }
		if (left > 0 && down > 1) { moveArr.push_back(pos + (pos / 32768)); }
		if (right > 0 && down > 1) { moveArr.push_back(pos + (pos / 131072)); }

	}

	return moveArr;
}

vector<long long> rookMoves(long long rookPos) {
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
			moveArr.push_back(pos + (pos * temp));
			temp *= 2;
		}

		temp = 2;

		for (int i = 0; i < right; i++) {
			moveArr.push_back(pos + (pos / temp));
			temp *= 2;
		}

		for (int i = 0; i < up; i++) {
			moveArr.push_back(pos + (pos * temp2));
			temp2 *= 256;
		}

		temp2 = 256;

		for (int i = 0; i < down; i++) {
			moveArr.push_back(pos + (pos / temp2));
			temp2 *= 256;
		}
	}
	return moveArr;
}

vector<long long> queenMoves(long long queenPos) {
	vector<long long> moveArr;

	vector<long long> diagmoves = bishopMoves(queenPos);
	vector<long long> sidemoves = rookMoves(queenPos);

	for (long long x : diagmoves) {
		moveArr.push_back(x);
	}
	for (long long x : sidemoves) {
		moveArr.push_back(x);
	}

	return moveArr;
}

vector<long long> kingMoves(long long kingPos) {
	vector<long long> moveArr;

	int x = 8 - fmod(log2(kingPos), 8);
	int y = std::floor(log2(kingPos) / 8) + 1;

	int left = x - 1;
	int right = 8 - x;
	int up = 8 - y;
	int down = y - 1;

	if (left > 1) { moveArr.push_back(kingPos + (kingPos * 2)); }
	if (right > 1) { moveArr.push_back(kingPos + (kingPos / 2)); }
	if (up > 1) { moveArr.push_back(kingPos + (kingPos * 256)); }
	if (down > 1) { moveArr.push_back(kingPos + (kingPos / 256)); }

	if (left > 1 && up > 1) { moveArr.push_back(kingPos + (kingPos * 512)); }
	if (right > 1 && up > 1) { moveArr.push_back(kingPos + (kingPos * 128)); }
	if (right > 1 && down > 1) { moveArr.push_back(kingPos + (kingPos / 512)); }
	if (left > 1 && down > 1) { moveArr.push_back(kingPos + (kingPos / 128)); }

	return moveArr;
}