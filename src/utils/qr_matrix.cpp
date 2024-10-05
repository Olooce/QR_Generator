//
// Created by oloo on 01/10/24.
//

#include "qr_matrix.h"
#include <iostream>
#include <opencv2/opencv.hpp>

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
    int row = size - 1;        // Start at the bottom row
    int col = size - 1;        // Start at the rightmost column
    int bitIndex = 0;          // Start from the first bit in dataBits
    bool upward = true;        // Direction of movement

    while (col > 0) {
        // Skip the vertical timing line
        if (col == 6) {
            col -= 1;
        }

        for (int i = 0; i < size; ++i) {
            int currentRow = upward ? (row - i) : (row + i);

            // Make sure the row index is within bounds
            if (currentRow < 0 || currentRow >= size) {
                continue;
            }

            // Place data in the current column
            for (int j = 0; j < 2; ++j) {
                int currentCol = col - j;

                // Skip already occupied cells (position markers, separators, etc.)
                if (matrix[currentRow][currentCol] == -1) {
                    if (bitIndex < dataBits.size()) {
                        matrix[currentRow][currentCol] = dataBits[bitIndex++];
                    } else {
                        // If all data bits are placed, fill the remaining empty cells with 0
                        matrix[currentRow][currentCol] = 0;
                    }
                }
            }
        }

        // Move to the next set of columns
        col -= 2;
        upward = !upward;
    }
}

void QRMatrix::applyMask(int maskPattern) {
    auto applyMaskPattern = [&](int row, int col) {
        switch (maskPattern) {
            case 0:
                return (row + col) % 2 == 0;
            case 1:
                return row % 2 == 0;
            case 2:
                return col % 3 == 0;
            case 3:
                return (row + col) % 3 == 0;
            case 4:
                return ((row / 2) + (col / 3)) % 2 == 0;
            case 5:
                return ((row * col) % 2) + ((row * col) % 3) == 0;
            case 6:
                return (((row * col) % 2) + ((row * col) % 3)) % 2 == 0;
            case 7:
                return (((row + col) % 2) + ((row * col) % 3)) % 2 == 0;
            default:
                return false;
        }
    };

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            // Skip reserved areas (position markers, separators, etc.)
            if (matrix[row][col] == -1) continue;

            // If the mask condition is met, invert the current bit
            if (applyMaskPattern(row, col)) {
                matrix[row][col] ^= 1;  // Invert the bit (0 becomes 1, 1 becomes 0)
            }
        }
    }
}


void QRMatrix::render() const {
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            std::cout << (matrix[y][x] == 1 ? "██" : "  ");
        }
        std::cout << "\n";
    }
}

void QRMatrix::generateImage(const std::string& filename) const {
    int scale = 10; // Scale factor for better visibility
    int imgSize = size * scale;

    // Create a blank image (black background)
    cv::Mat img = cv::Mat::zeros(imgSize, imgSize, CV_8UC1);

    // Draw the QR code
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (matrix[y][x] == 1) {
                // Draw a filled square for black modules
                cv::rectangle(img, cv::Point(x * scale, y * scale),
                              cv::Point((x + 1) * scale - 1, (y + 1) * scale - 1),
                              cv::Scalar(255), cv::FILLED);
            }
        }
    }

    // Save the image to a file
    // cv::imwrite(filename, img);

    if (!cv::imwrite(filename, img)) {
        std::cerr << "Failed to save the image: " << filename << std::endl;
    }

}
