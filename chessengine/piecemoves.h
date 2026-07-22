#pragma once

#ifndef PIECE_MOVES_H
#define PIECE_MOVES_H

#include <vector>

std::vector<long long> pawnMoves(long long pawnsPos);
std::vector<long long> bishopMoves(long long bishopPos);
std::vector<long long> knightMoves(long long knightPos);
std::vector<long long> rookMoves(long long rookPos);
std::vector<long long> queenMoves(long long queenPos);
std::vector<long long> kingMoves(long long kingPos);
std::vector<long long> getPositions(long long piecePositions);

#endif