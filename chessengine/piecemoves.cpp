#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>

using namespace std;

vector<long long> pawnMoves(long long pawnsPos) {
	std::vector<long long> indivPawnsPos;

	for (int i = 0; i < 64; i++) {
		int temp = pow(2, i);
		if ((pawnsPos & temp) > 0) {
			indivPawnsPos.push_back(temp);
		}
	}

	std::vector<long long> moveArr;

	for (long long x : indivPawnsPos) {
		moveArr.push_back((x * 256) + x);
		moveArr.push_back((x * 256 * 256) + x);
	}

	return moveArr;
}

vector<long long> bishopMoves(long long bishopPos) {
	std::vector<long long> indivBishopPos;

	for (int i = 0; i < 64; i++) {
		int temp = pow(2, i);
		if ((bishopPos & temp) > 0) {
			indivBishopPos.push_back(temp);
		}
	}

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
	}
	return moveArr;
}

vector<long long> knightMoves(long long knightPos) {
	std::vector<long long> indivPawnsPos;

	for (int i = 0; i < 64; i++) {
		int temp = pow(2, i);
		if ((knightPos & temp) > 0) {
			indivPawnsPos.push_back(temp);
		}
	}
}