#include "Matrix.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

// --- 1. LE CONSTRUCTEUR ---
Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    data.resize(r * c, 0.0f);
}

// --- 2. ACCÉDER AUX DONNÉES ---
float& Matrix::operator()(int r, int c) {
    if (r >= rows || c >= cols || r < 0 || c < 0) {
        throw std::out_of_range("Indice de matrice invalide");
    }
    return data[r * cols + c];
}

const float& Matrix::operator()(int r, int c) const {
    if (r >= rows || c >= cols || r < 0 || c < 0) {
        throw std::out_of_range("Indice de matrice invalide");
    }
    return data[r * cols + c];
}

// --- 3. UTILITAIRES ---
void Matrix::setZero() {
    std::fill(data.begin(), data.end(), 0.0f);
}

void Matrix::setRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    // Distribution normale (moyenne 0, écart-type 1)
    std::normal_distribution<float> dis(0.0f, 1.0f);
    for (auto& val : data) {
        val = dis(gen);
    }
}

void Matrix::print() const {
    std::cout << "Matrix (" << rows << "x" << cols << "):" << std::endl;
    for (int r = 0; r < rows; ++r) {
        std::cout << "[ ";
        for (int c = 0; c < cols; ++c) {
            std::cout << std::fixed << std::setprecision(2) << (*this)(r, c) << " ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}

// --- 4. MATHÉMATIQUES ---

Matrix Matrix::add(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Dimensions incompatibles pour l'addition");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < data.size(); ++i) {
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}

Matrix Matrix::multiply(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Multiplication impossible : A.cols != B.rows");
    }
    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {

            float sum = 0.0f;
            for (int k = 0; k < cols; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

Matrix Matrix::applyReLU() const {
    Matrix result(rows, cols);
    for (size_t i = 0; i < data.size(); ++i) {
        result.data[i] = std::max(0.0f, data[i]);
    }
    return result;
}

Matrix Matrix::convolve(const Matrix& kernel) const {
    if (kernel.rows % 2 == 0 || kernel.cols % 2 == 0) {
        throw std::invalid_argument("taille paire");
    }
    int newRows = rows - kernel.rows + 1;
    int newCols = cols - kernel.cols + 1;
    Matrix result(newRows, newCols);
    int kCenterX = kernel.cols / 2;
    int kCenterY = kernel.rows / 2;
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            float sum = 0.0f;
            for (int m = 0; m < kernel.rows; ++m) {
                for (int n = 0; n < kernel.cols; ++n) {
                    int r = i + m;
                    int c = j + n;
                    sum += (*this)(r, c) * kernel(m, n);
                }
            }
            result(i, j) = sum;
        }
    }
    return result;
}

Matrix Matrix::maxPooling(int step) const {
    int newRows = rows / step;
    int newCols = cols / step;
    Matrix result(newRows, newCols);
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            float maxVal = -1e9f;
            for (int m = 0; m < step; ++m) {
                for (int n = 0; n < step; ++n) {
                    int r = i * step + m;
                    int c = j * step + n;
                    if (r < rows && c < cols) {
                        float val = (*this)(r, c);
                        if (val > maxVal) maxVal = val;
                    }
                }
            }
            result(i, j) = maxVal;
        }
    }
    return result;
}

Matrix Matrix::flatten() const {
    Matrix result(rows * cols, 1);
    for (int i = 0; i < rows * cols; ++i) {
        result.data[i] = data[i];
    }
    return result;
}

Matrix softmax(const Matrix& input) {
    Matrix result(input.rows, input.cols);
    float sumExp = 0.0f;

    for (size_t i = 0; i < input.data.size(); ++i) {
        float val = std::exp(input.data[i]);
        result.data[i] = val;
        sumExp += val;
    }

    for (size_t i = 0; i < result.data.size(); ++i) {
        result.data[i] /= sumExp;
    }

    return result;
}

bool Matrix::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir " << filename << std::endl;
        return false;
    }

    // On lit les valeurs une par une et on remplit la matrice
    for (int i = 0; i < rows * cols; ++i) {
        if (!(file >> data[i])) {
            std::cerr << "Erreur : Pas assez de données dans " << filename << std::endl;
            return false;
        }
    }

    std::cout << "Charge : " << filename << " (" << rows << "x" << cols << ")" << std::endl;
    return true;
}