//
// Created by oloo on 01/10/24.
//

#ifndef QR_MATRIX_H
#define QR_MATRIX_H

#include <vector>

class QRMatrix {
public:
    explicit QRMatrix(int version);
    void addPositionMarkers();
    void placeData(const std::vector<int>& dataBits);
    void applyMask(int maskPattern);
    void render() const;

private:
    int version;
    int size;
    std::vector<std::vector<int>> matrix;
};

void QRMatrix(int version) : version(version), size(21 + 4 * (version - 1)) {
    matrix = std::vector<std::vector<int>>(size, std::vector<int>(size, -1));
}


#endif //QR_MATRIX_H
