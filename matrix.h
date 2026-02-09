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
};