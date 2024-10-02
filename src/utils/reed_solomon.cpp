//
// Created by oloo on 01/10/24.
//

#include "reed_solomon.h"
#include <vector>

// A helper function to multiply two Galois Field elements
int gfMultiply(int a, int b, int primitive) {
    int result = 0;
    while (b > 0) {
        if (b & 1) {
            result ^= a; // If the current bit is set, add a to the result
        }
        a <<= 1; // Multiply a by x
        if (a & 0x100) { // If a overflows beyond 8 bits
            a ^= primitive; // Apply the primitive polynomial
        }
        b >>= 1;
    }
    return result;
}

// A helper function to perform polynomial division in Galois Field
std::vector<int> gfPolynomialDivision(const std::vector<int>& dividend, const std::vector<int>& divisor, int primitive) {
    std::vector<int> remainder = dividend;

    for (int i = 0; i <= dividend.size() - divisor.size(); ++i) {
        int coef = remainder[i];
        if (coef != 0) {
            for (int j = 0; j < divisor.size(); ++j) {
                remainder[i + j] ^= gfMultiply(divisor[j], coef, primitive);
            }
        }
    }

    // Return the remainder after division, which is the error correction codewords
    return std::vector<int>(remainder.end() - (divisor.size() - 1), remainder.end());
}

std::vector<int> generateErrorCorrection(const std::vector<int>& dataBits) {
    // Step 1: Determine the number of error correction codewords
    int version = 40; // TODO: Pass this as an argument
    int numECCodewords = 30; // This value depends on the QR code version and error correction level

    // Step 2: Convert data bits to bytes (8-bit chunks)
    std::vector<int> dataBytes;
    for (int i = 0; i < dataBits.size(); i += 8) {
        int byte = 0;
        for (int j = 0; j < 8; ++j) {
            byte = (byte << 1) | dataBits[i + j];
        }
        dataBytes.push_back(byte);
    }

    // Step 3: Define the generator polynomial for Reed-Solomon
    std::vector<int> generator = {1}; // Start with x^0 (which is 1)
    int primitive = 0x11d; // Primitive polynomial for GF(2^8)

    for (int i = 0; i < numECCodewords; ++i) {
        std::vector<int> term = {1, gfMultiply(2, i, primitive)};
        // Multiply generator by (x - α^i)
        std::vector<int> newGenerator(generator.size() + 1, 0);
        for (int j = 0; j < generator.size(); ++j) {
            newGenerator[j] ^= generator[j];
            newGenerator[j + 1] ^= gfMultiply(generator[j], term[1], primitive);
        }
        generator = newGenerator;
    }

    // Step 4: Perform polynomial division to get the error correction codewords
    std::vector<int> dataPoly = dataBytes;
    dataPoly.resize(dataBytes.size() + numECCodewords, 0); // Append zeroes for the remainder calculation

    std::vector<int> errorCorrectionBytes = gfPolynomialDivision(dataPoly, generator, primitive);

    // Step 5: Convert error correction bytes to bits
    std::vector<int> errorBits;
    for (int byte : errorCorrectionBytes) {
        for (int i = 7; i >= 0; --i) {
            errorBits.push_back((byte >> i) & 1);
        }
    }

    return errorBits;
}

