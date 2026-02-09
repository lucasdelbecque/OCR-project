#include "Matrix.h"
#include <random>
#include <iostream>
#include <iomanip>

// --- 1. LE CONSTRUCTEUR ---
Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    data.resize(r * c, 0.0f); // On remplit de 0.0f par défaut
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

// --- 4. MATHÉMATIQUES (Le cœur du sujet) ---

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