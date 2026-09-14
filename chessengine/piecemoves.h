#pragma once

#ifndef PIECE_MOVES_H
#define PIECE_MOVES_H

#include <vector>

using namespace std;

long long bitMap(vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> whitepawnMoves(long long pawnsPos, vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> blackpawnMoves(long long pawnsPos, vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> bishopMoves(long long bishopPos, vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> knightMoves(long long knightPos, vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> rookMoves(long long rookPos, vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> queenMoves(long long queenPos, vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> kingMoves(long long kingPos, vector<long long> whitePieces, vector<long long> blackPieces);
vector<long long> getPositions(long long piecePositions);
vector<long long> getMoves(vector<long long> yourPieces, vector<long long> oppPieces, bool white);
long long getColision(long long piecePos, vector<long long> allpieces);

#endif