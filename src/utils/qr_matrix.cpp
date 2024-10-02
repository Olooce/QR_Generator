//
// Created by oloo on 01/10/24.
//

#include "qr_matrix.h"
#include <iostream>

QRMatrix::QRMatrix(int version) : version(version), size(21 + 4 * (version - 1)) {
    matrix = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));
}

void QRMatrix::addPositionMarkers() {
    // Define the pattern for a 7x7 position marker
    std::vector<std::vector<int>> marker = {
        {1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1}
    };

    // Function to place a position marker at a given (row, col)
    auto placeMarker = [&](int row, int col) {
        for (int y = 0; y < 7; ++y) {
            for (int x = 0; x < 7; ++x) {
                matrix[row + y][col + x] = marker[y][x];
            }
        }
    };

    // Place position markers at three corners
    placeMarker(0, 0);                   // Top-left
    placeMarker(0, size - 7);            // Top-right
    placeMarker(size - 7, 0);            // Bottom-left

    // Fill in the separator area (1 module space around the markers)
    auto fillSeparator = [&](int rowStart, int colStart, int width, int height) {
        for (int y = rowStart; y < rowStart + height; ++y) {
            for (int x = colStart; x < colStart + width; ++x) {
                if (x >= 0 && x < size && y >= 0 && y < size && matrix[y][x] == -1) {
                    matrix[y][x] = 0;
                }
            }
        }
    };

    // Top-left marker separator
    fillSeparator(0, 7, 8, 1);       // Horizontal line below
    fillSeparator(7, 0, 1, 8);       // Vertical line to the right

    // Top-right marker separator
    fillSeparator(0, size - 8, 8, 1);    // Horizontal line below
    fillSeparator(7, size - 8, 1, 8);    // Vertical line to the left

    // Bottom-left marker separator
    fillSeparator(size - 8, 0, 1, 8);    // Vertical line to the right
    fillSeparator(size - 8, 7, 8, 1);    // Horizontal line above
}



void QRMatrix::placeData(const std::vector<int>& dataBits) {
    //TODO: Implement logic to place data bits and error correction bits on the matrix
}

void QRMatrix::applyMask(int maskPattern) {
    // TODO: Apply one of the 8 mask patterns and calculate penalty score
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
