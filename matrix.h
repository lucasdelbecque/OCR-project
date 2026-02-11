#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>

class Matrix {
public:
    int rows;
    int cols;
    std::vector<float> data;

    Matrix(int r, int c);

    float& operator()(int r, int c);
    const float& operator()(int r, int c) const;

    void setZero();

    void setRandom();

    void print() const;

    Matrix add(const Matrix& other) const;

    Matrix multiply(const Matrix& other) const;

    Matrix applyReLU() const;

    Matrix convolve(const Matrix& kernel) const;

    Matrix maxPooling(int step = 2) const;

    Matrix flatten() const;

    bool load(const std::string& filename);

};

Matrix softmax(const Matrix& input);