#include "qr_encoder.h"
#include "reed_solomon.h"
#include "qr_matrix.h"
#include <iostream>
#include <filesystem>

int main() {
    // Step 1: Initialize QR matrix
    QRMatrix qrMatrix(40);
    qrMatrix.addPositionMarkers();

    // Step 2: Encode the data
    std::string data = "Hello, Version 40 QR Code!";
    std::vector<int> dataBits = encodeData(data);

    // Step 3: Generate error correction bits
    std::vector<int> errorBits = generateErrorCorrection(dataBits);

    // Step 4: Place data and error correction bits
    qrMatrix.placeData(dataBits);

    // Step 5: Apply mask
    qrMatrix.applyMask(0);  // Using mask pattern 0

    // Step 6: Generate and save the QR code image in the project's folder
    // Use std::filesystem to get the current working directory
    std::filesystem::path outputPath = std::filesystem::current_path() / "qr_code.png";
    qrMatrix.generateImage(outputPath.string());

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::cout << "Current working directory: " << currentPath << std::endl;

    // Optionally: Render the QR code to the console (for debugging)
    // qrMatrix.render();

    return 0;
}
