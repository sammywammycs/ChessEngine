#include <iostream>
#include "piecemoves.h"
#include <vector>
#include <cmath>
#include <bit>
#include <array>
#include <algorithm>
#include "output.h"

using namespace std;

static inline int rankOf(int sq) { return sq / 8; }
static inline int fileOf(int sq) { return sq % 8; }
static inline int mirror(int sq) { return sq ^ 56; } // flip rank, keep file

// Iterate every set bit of bb, calling f(squareIndex) for each, without
// allocating anything. Usage: forEachBit(bitboard, [&](int sq){ ... });
template <typename Func>
static inline void forEachBit(unsigned long long bb, Func&& f) {
	while (bb) {
		int sq = std::countr_zero(bb);
		f(sq);
		bb &= bb - 1; // clear lowest set bit
	}
}

// ---------------------------------------------------------------------------
// Piece-square tables (from White's perspective, a1 = index 0)
// ---------------------------------------------------------------------------

static const array<int, 64> PAWN_PST = {
	  0,   0,   0,   0,   0,   0,   0,   0,
	  5,  10,  10, -20, -20,  10,  10,   5,
	  5,  -5, -10,   0,   0, -10,  -5,   5,
	  0,   0,   0,  20,  20,   0,   0,   0,
	  5,   5,  10,  25,  25,  10,   5,   5,
	 10,  10,  20,  30,  30,  20,  10,  10,
	 50,  50,  50,  50,  50,  50,  50,  50,
	  0,   0,   0,   0,   0,   0,   0,   0,
};

static const array<int, 64> KNIGHT_PST = {
	-50, -40, -30, -30, -30, -30, -40, -50,
	-40, -20,   0,   5,   5,   0, -20, -40,
	-30,   5,  10,  15,  15,  10,   5, -30,
	-30,   0,  15,  20,  20,  15,   0, -30,
	-30,   5,  15,  20,  20,  15,   5, -30,
	-30,   0,  10,  15,  15,  10,   0, -30,
	-40, -20,   0,   0,   0,   0, -20, -40,
	-50, -40, -30, -30, -30, -30, -40, -50,
};

static const array<int, 64> BISHOP_PST = {
	-20, -10, -10, -10, -10, -10, -10, -20,
	-10,   5,   0,   0,   0,   0,   5, -10,
	-10,  10,  10,  10,  10,  10,  10, -10,
	-10,   0,  10,  10,  10,  10,   0, -10,
	-10,   5,   5,  10,  10,   5,   5, -10,
	-10,   0,   5,  10,  10,   5,   0, -10,
	-10,   0,   0,   0,   0,   0,   0, -10,
	-20, -10, -10, -10, -10, -10, -10, -20,
};

static const array<int, 64> ROOK_PST = {
	  0,   0,   5,  10,  10,   5,   0,   0,
	 -5,   0,   0,   0,   0,   0,   0,  -5,
	 -5,   0,   0,   0,   0,   0,   0,  -5,
	 -5,   0,   0,   0,   0,   0,   0,  -5,
	 -5,   0,   0,   0,   0,   0,   0,  -5,
	 -5,   0,   0,   0,   0,   0,   0,  -5,
	  5,  10,  10,  10,  10,  10,  10,   5,
	  0,   0,   0,   0,   0,   0,   0,   0,
};

static const array<int, 64> QUEEN_PST = {
	-20, -10, -10,  -5,  -5, -10, -10, -20,
	-10,   0,   5,   0,   0,   0,   0, -10,
	-10,   5,   5,   5,   5,   5,   0, -10,
	  0,   0,   5,   5,   5,   5,   0,  -5,
	 -5,   0,   5,   5,   5,   5,   0,  -5,
	-10,   0,   5,   5,   5,   5,   0, -10,
	-10,   0,   0,   0,   0,   0,   0, -10,
	-20, -10, -10,  -5,  -5, -10, -10, -20,
};

static const array<int, 64> KING_MID_PST = {
	 20,  30,  10,   0,   0,  10,  30,  20,
	 20,  20,   0,   0,   0,   0,  20,  20,
	-10, -20, -20, -20, -20, -20, -20, -10,
	-20, -30, -30, -40, -40, -30, -30, -20,
	-30, -40, -40, -50, -50, -40, -40, -30,
	-30, -40, -40, -50, -50, -40, -40, -30,
	-30, -40, -40, -50, -50, -40, -40, -30,
	-30, -40, -40, -50, -50, -40, -40, -30,
};

static const array<int, 64> KING_END_PST = {
	-50, -30, -30, -30, -30, -30, -30, -50,
	-30, -30,   0,   0,   0,   0, -30, -30,
	-30, -10,  20,  30,  30,  20, -10, -30,
	-30, -10,  30,  40,  40,  30, -10, -30,
	-30, -10,  30,  40,  40,  30, -10, -30,
	-30, -10,  20,  30,  30,  20, -10, -30,
	-30, -20, -10,   0,   0, -10, -20, -30,
	-50, -40, -30, -20, -20, -30, -40, -50,
};

static inline int pstValue(const array<int, 64>& table, int square, bool isWhite) {
	return table[isWhite ? square : mirror(square)];
}

// ---------------------------------------------------------------------------
// Pawn structure: built directly from the pawn bitboard, no vector involved.
// ---------------------------------------------------------------------------
struct PawnFiles {
	array<int, 8> countPerFile{};        // number of own pawns on each file
	array<unsigned char, 8> ranksMask{}; // bit r set => own pawn on rank r, this file
};

static inline PawnFiles buildPawnFiles(unsigned long long pawnBB) {
	PawnFiles pf{};
	forEachBit(pawnBB, [&](int sq) {
		int f = fileOf(sq);
		int r = rankOf(sq);
		pf.countPerFile[f]++;
		pf.ranksMask[f] |= static_cast<unsigned char>(1u << r);
		});
	return pf;
}

static int doubledPawnPenalty(const PawnFiles& pf) {
	int penalty = 0;
	for (int f = 0; f < 8; f++) {
		if (pf.countPerFile[f] > 1) penalty += (pf.countPerFile[f] - 1) * 15;
	}
	return penalty;
}

static int isolatedPawnPenalty(const PawnFiles& pf) {
	int penalty = 0;
	for (int f = 0; f < 8; f++) {
		if (pf.countPerFile[f] == 0) continue;
		bool leftHasPawn = (f > 0) && pf.countPerFile[f - 1] > 0;
		bool rightHasPawn = (f < 7) && pf.countPerFile[f + 1] > 0;
		if (!leftHasPawn && !rightHasPawn) penalty += pf.countPerFile[f] * 12;
	}
	return penalty;
}

static int passedPawnBonus(unsigned long long ownPawnBB, const PawnFiles& enemyFiles, bool isWhite) {
	int bonus = 0;
	static const int PASSED_BY_RANK_WHITE[8] = { 0, 5, 10, 20, 35, 60, 100, 0 };
	static const int PASSED_BY_RANK_BLACK[8] = { 0, 100, 60, 35, 20, 10, 5, 0 };

	forEachBit(ownPawnBB, [&](int sq) {
		int f = fileOf(sq);
		int r = rankOf(sq);
		bool blocked = false;
		for (int nf = max(0, f - 1); nf <= min(7, f + 1) && !blocked; nf++) {
			unsigned char mask = enemyFiles.ranksMask[nf];
			if (mask == 0) continue;
			if (isWhite) {
				for (int er = r + 1; er < 8; er++) if (mask & (1u << er)) { blocked = true; break; }
			}
			else {
				for (int er = r - 1; er >= 0; er--) if (mask & (1u << er)) { blocked = true; break; }
			}
		}
		if (!blocked) bonus += isWhite ? PASSED_BY_RANK_WHITE[r] : PASSED_BY_RANK_BLACK[r];
		});
	return bonus;
}

// ---------------------------------------------------------------------------
// Rook file bonus: open (no pawns at all) or semi-open (no own pawns)
// ---------------------------------------------------------------------------
static int rookFileBonus(unsigned long long rookBB, const PawnFiles& ownPawnFiles, const PawnFiles& enemyPawnFiles) {
	int bonus = 0;
	forEachBit(rookBB, [&](int sq) {
		int f = fileOf(sq);
		bool ownPawnOnFile = ownPawnFiles.countPerFile[f] > 0;
		bool enemyPawnOnFile = enemyPawnFiles.countPerFile[f] > 0;
		if (!ownPawnOnFile && !enemyPawnOnFile) bonus += 25;      // open file
		else if (!ownPawnOnFile) bonus += 12;                     // semi-open file
		});
	return bonus;
}

// ---------------------------------------------------------------------------
// Game phase (0 = endgame, 1 = midgame), tapers the king's PST.
// ---------------------------------------------------------------------------
static inline int weightedNonPawnMaterial(const vector<long long>& pieces) {
	int knights = pieces.size() > 1 ? popcount(static_cast<unsigned long long>(pieces[1])) : 0;
	int bishops = pieces.size() > 2 ? popcount(static_cast<unsigned long long>(pieces[2])) : 0;
	int rooks = pieces.size() > 3 ? popcount(static_cast<unsigned long long>(pieces[3])) : 0;
	int queens = pieces.size() > 4 ? popcount(static_cast<unsigned long long>(pieces[4])) : 0;
	return knights + bishops + rooks * 2 + queens * 4;
}

// ---------------------------------------------------------------------------
// Main evaluation
// ---------------------------------------------------------------------------
// whitePieces / blackPieces layout (same as the original eval()):
//   [0] pawns, [1] knights, [2] bishops, [3] rooks, [4] queens
//   [5] king  (optional - if not present, king-safety terms are skipped)
int eval(vector<long long> whitePieces, vector<long long> blackPieces) {
	int evaluation = 0;

	unsigned long long whitePawnBB = static_cast<unsigned long long>(whitePieces[0]);
	unsigned long long blackPawnBB = static_cast<unsigned long long>(blackPieces[0]);
	unsigned long long whiteKnightBB = whitePieces.size() > 1 ? static_cast<unsigned long long>(whitePieces[1]) : 0ULL;
	unsigned long long blackKnightBB = blackPieces.size() > 1 ? static_cast<unsigned long long>(blackPieces[1]) : 0ULL;
	unsigned long long whiteBishopBB = whitePieces.size() > 2 ? static_cast<unsigned long long>(whitePieces[2]) : 0ULL;
	unsigned long long blackBishopBB = blackPieces.size() > 2 ? static_cast<unsigned long long>(blackPieces[2]) : 0ULL;
	unsigned long long whiteRookBB = whitePieces.size() > 3 ? static_cast<unsigned long long>(whitePieces[3]) : 0ULL;
	unsigned long long blackRookBB = blackPieces.size() > 3 ? static_cast<unsigned long long>(blackPieces[3]) : 0ULL;
	unsigned long long whiteQueenBB = whitePieces.size() > 4 ? static_cast<unsigned long long>(whitePieces[4]) : 0ULL;
	unsigned long long blackQueenBB = blackPieces.size() > 4 ? static_cast<unsigned long long>(blackPieces[4]) : 0ULL;
	unsigned long long whiteKingBB = whitePieces.size() > 5 ? static_cast<unsigned long long>(whitePieces[5]) : 0ULL;
	unsigned long long blackKingBB = blackPieces.size() > 5 ? static_cast<unsigned long long>(blackPieces[5]) : 0ULL;

	// --- Material (popcount is a single hardware instruction - cheap) ---
	evaluation += 100 * popcount(whitePawnBB) - 100 * popcount(blackPawnBB);
	evaluation += 300 * popcount(whiteKnightBB) - 300 * popcount(blackKnightBB);
	evaluation += 300 * popcount(whiteBishopBB) - 300 * popcount(blackBishopBB);
	evaluation += 500 * popcount(whiteRookBB) - 500 * popcount(blackRookBB);
	evaluation += 900 * popcount(whiteQueenBB) - 900 * popcount(blackQueenBB);

	// --- Piece-square tables (direct bit iteration, no allocation) ---
	forEachBit(whitePawnBB, [&](int sq) { evaluation += pstValue(PAWN_PST, sq, true); });
	forEachBit(blackPawnBB, [&](int sq) { evaluation -= pstValue(PAWN_PST, sq, false); });
	forEachBit(whiteKnightBB, [&](int sq) { evaluation += pstValue(KNIGHT_PST, sq, true); });
	forEachBit(blackKnightBB, [&](int sq) { evaluation -= pstValue(KNIGHT_PST, sq, false); });
	forEachBit(whiteBishopBB, [&](int sq) { evaluation += pstValue(BISHOP_PST, sq, true); });
	forEachBit(blackBishopBB, [&](int sq) { evaluation -= pstValue(BISHOP_PST, sq, false); });
	forEachBit(whiteRookBB, [&](int sq) { evaluation += pstValue(ROOK_PST, sq, true); });
	forEachBit(blackRookBB, [&](int sq) { evaluation -= pstValue(ROOK_PST, sq, false); });
	forEachBit(whiteQueenBB, [&](int sq) { evaluation += pstValue(QUEEN_PST, sq, true); });
	forEachBit(blackQueenBB, [&](int sq) { evaluation -= pstValue(QUEEN_PST, sq, false); });

	// --- King safety (tapered between midgame and endgame tables) ---
	int totalPhaseUnits = weightedNonPawnMaterial(whitePieces) + weightedNonPawnMaterial(blackPieces);
	// Max possible: (2+2+4+4)*2 = 24 at the start of the game. Scale to 0-256
	// and do the taper in integers to avoid floating point in the hot path.
	int phase256 = std::clamp((totalPhaseUnits * 256) / 24, 0, 256);

	if (whiteKingBB) {
		int sq = std::countr_zero(whiteKingBB);
		int mid = pstValue(KING_MID_PST, sq, true);
		int end = pstValue(KING_END_PST, sq, true);
		evaluation += (mid * phase256 + end * (256 - phase256)) / 256;
	}
	if (blackKingBB) {
		int sq = std::countr_zero(blackKingBB);
		int mid = pstValue(KING_MID_PST, sq, false);
		int end = pstValue(KING_END_PST, sq, false);
		evaluation -= (mid * phase256 + end * (256 - phase256)) / 256;
	}

	// --- Pawn structure ---
	PawnFiles whitePF = buildPawnFiles(whitePawnBB);
	PawnFiles blackPF = buildPawnFiles(blackPawnBB);

	evaluation -= doubledPawnPenalty(whitePF);
	evaluation += doubledPawnPenalty(blackPF);

	evaluation -= isolatedPawnPenalty(whitePF);
	evaluation += isolatedPawnPenalty(blackPF);

	evaluation += passedPawnBonus(whitePawnBB, blackPF, true);
	evaluation -= passedPawnBonus(blackPawnBB, whitePF, false);

	// --- Bishop pair ---
	if (popcount(whiteBishopBB) >= 2) evaluation += 30;
	if (popcount(blackBishopBB) >= 2) evaluation -= 30;

	// --- Rook on open/semi-open file ---
	evaluation += rookFileBonus(whiteRookBB, whitePF, blackPF);
	evaluation -= rookFileBonus(blackRookBB, blackPF, whitePF);

	return evaluation;
}