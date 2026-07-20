#pragma once

#ifndef PIECE_MOVES_H
#define PIECE_MOVES_H

#include <vector>

std::vector<long long> pawnMoves(long long pawnsPos);
std::vector<long long> bishopMoves(long long pawnsPos);
std::vector<long long> knightMoves(long long pawnsPos);
std::vector<long long> rookMoves(long long pawnsPos);
std::vector<long long> queenMoves(long long pawnsPos);
std::vector<long long> kingMoves(long long pawnsPos);

#endif