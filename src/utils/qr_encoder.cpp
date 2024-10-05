//
// Created by oloo on 01/10/24.
//

#include "qr_encoder.h"

std::vector<int> encodeData(const std::string& data) {
    std::vector<int> dataBits;

    // Step 1: Add mode indicator for byte mode (0100)
    dataBits.push_back(0);
    dataBits.push_back(1);
    dataBits.push_back(0);
    dataBits.push_back(0);

    // Step 2: Add character count indicator (8-bit for versions up to 40 in byte mode)
    int dataLength = data.length();
    for (int i = 7; i >= 0; --i) {
        dataBits.push_back((dataLength >> i) & 1);
    }

    // Step 3: Add the data bytes as 8-bit binary values
    for (char c : data) {
        for (int i = 7; i >= 0; --i) {
            dataBits.push_back((c >> i) & 1);
        }
    }

    // Step 4: Add terminator (up to 4 bits)
    int terminatorLength = std::min(4, static_cast<int>(dataBits.capacity() - dataBits.size()));
    for (int i = 0; i < terminatorLength; ++i) {
        dataBits.push_back(0);
    }

    // Step 5: Pad with "0" bits to reach a byte boundary
    while (dataBits.size() % 8 != 0) {
        dataBits.push_back(0);
    }

    // Step 6: Pad with alternating bytes 11101100 and 00010001 if more space is required
    while (dataBits.size() < dataLength * 8) {
        std::vector<int> padByte1 = {1, 1, 1, 0, 1, 1, 0, 0};  // 0xEC
        std::vector<int> padByte2 = {0, 0, 0, 1, 0, 0, 0, 1};  // 0x11

        for (int bit : padByte1) {
            dataBits.push_back(bit);
            if (dataBits.size() >= dataLength * 8) break;
        }
        for (int bit : padByte2) {
            dataBits.push_back(bit);
            if (dataBits.size() >= dataLength * 8) break;
        }
    }

    return dataBits;
}
