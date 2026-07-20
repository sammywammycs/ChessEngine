#include <iostream>
#include "piecemoves.h"
#include <vector>

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