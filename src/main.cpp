#include "qr_encoder.h"
#include "reed_solomon.h"
#include "qr_matrix.h"
#include <iostream>

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

    // Step 6: Render the QR code
    qrMatrix.render();

    return 0;
}
