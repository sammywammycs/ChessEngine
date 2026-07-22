#pragma once

#ifndef EVAL_H
#define EVAL_H

#include <vector>

float eval(std::vector<long long> whitePieces, std::vector<long long> blackPieces);
float pawnClosetoCentre(long long pawnPos);

#endif
