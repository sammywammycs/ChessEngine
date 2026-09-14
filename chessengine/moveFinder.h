#pragma once

#ifndef MOVE_FINDER_H
#define MOVE_FINDER_H

#include <vector>
#include <utility>

using namespace std;

pair<vector<long long>, vector<long long>> getTempPosition(long long move, vector<long long>yourPieces, vector<long long> oppPieces);
pair<int, vector<long long>> findBest(vector<long long> yourPieces, vector<long long> oppPieces, int depth, int ply, int alpha, int beta, bool white);

#endif