//
// Created by oloo on 01/10/24.
//

#include "qr_matrix.h"
#include <iostream>

QRMatrix::QRMatrix(int version) : version(version), size(21 + 4 * (version - 1)) {
    matrix = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));
}

void QRMatrix::addPositionMarkers() {
    // Implement logic to add the position markers (7x7 modules each)
}

void QRMatrix::placeData(const std::vector<int>& dataBits) {
    // Implement logic to place data bits and error correction bits on the matrix
}

void QRMatrix::applyMask(int maskPattern) {
    // Apply one of the 8 mask patterns and calculate penalty score
    // Choose the best mask with the lowest penalty
}

void QRMatrix::render() const {
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            std::cout << (matrix[y][x] == 1 ? "██" : "  ");
        }
        std::cout << "\n";
    }
}
