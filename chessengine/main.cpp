#include <iostream>
#include <cmath>
#include <vector>
#include "output.h"

using namespace std;

float eval(long long whitePieces[6], long long blackPieces[6]) {
	return 0;
}

long long moveList(long long whitepawns) {
	int pawns = 0;

	cout << whitepawns << endl;

	for (int i = 0; i < 64; i++) {
		int temp = pow(2, i);
		if ((whitepawns & temp) > 0){
			pawns++;
		}
	}

	std::vector<long long> pawnsPos;

	for (int i = 0; i < 64; i++) {
		int temp = pow(2, i);
		if ((whitepawns & temp) > 0) {
			pawnsPos.push_back(pow(2,i));
		}
	}

	std::vector<long long> moveArr;

	for (long long x : pawnsPos) {
		moveArr.push_back((x * 256) + x);
		moveArr.push_back((x * 256 * 256) + x);
	}

	std::vector<long long> stateAfterMove;
	
	for (long long x : moveArr) {
		stateAfterMove.push_back(whitepawns ^ x);
	}

	for (long long x : stateAfterMove) {
		cout << x << endl;
	}

	printBinaryGrid(stateAfterMove[3]);

	return 0;

}

int main() {
	long long whitepawns = 65280;
	long long checkpawn = 1024;
	moveList(whitepawns);
	return 0;
}