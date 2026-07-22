#include <iostream>
#include <bitset>
#include "output.h"

void printBinaryGrid(long long num) {
    std::bitset<64> bits(num);

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int bitIndex = 63 - (row * 8 + col);
            std::cout << bits[bitIndex] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}