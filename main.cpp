#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include "Matrix.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 700;
const int GRID_SIZE = 28;
const int NUM_FILTERS = 8;

Matrix inputImage(GRID_SIZE, GRID_SIZE);

const float BTN_X = -0.9f;
const float BTN_Y = -0.85f;
const float BTN_W = 0.3f;
const float BTN_H = 0.1f;

void drawDigit(int n, float x, float y, float w, float h) {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    if(n!=1 && n!=4) {
        glVertex2f(x, y); glVertex2f(x+w, y);
    }
    if(n!=1 && n!=4 && n!=7 && n!=9) {
        glVertex2f(x, y-h); glVertex2f(x+w, y-h);
    }
    if(n!=0 && n!=1 && n!=7) {
        glVertex2f(x, y-h/2); glVertex2f(x+w, y-h/2);
    }
    if(n!=1 && n!=2 && n!=3 && n!=7) {
        glVertex2f(x, y); glVertex2f(x, y-h/2);
    }
    if(n==0 || n==2 || n==6 || n==8) {
        glVertex2f(x, y-h/2); glVertex2f(x, y-h);
    }
    if(n!=5 && n!=6) {
        glVertex2f(x+w, y); glVertex2f(x+w, y-h/2);
    }
    if(n!=2) {
        glVertex2f(x+w, y-h/2); glVertex2f(x+w, y-h);
    }
    glEnd();
}

void drawFrame(float x, float y, float w, float h) {
    glLineWidth(1.0f); glColor3f(0.5f, 0.5f, 0.5f); glBegin(GL_LINE_LOOP);
    glVertex2f(x, y); glVertex2f(x+w, y); glVertex2f(x+w, y-h); glVertex2f(x, y-h); glEnd();
}

void drawClearButton() {
    glColor3f(0.9f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(BTN_X, BTN_Y);
    glVertex2f(BTN_X + BTN_W, BTN_Y);
    glVertex2f(BTN_X + BTN_W, BTN_Y - BTN_H);
    glVertex2f(BTN_X, BTN_Y - BTN_H);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(BTN_X + BTN_W*0.4f, BTN_Y - BTN_H*0.2f);
    glVertex2f(BTN_X + BTN_W*0.6f, BTN_Y - BTN_H*0.8f);
    glVertex2f(BTN_X + BTN_W*0.6f, BTN_Y - BTN_H*0.2f);
    glVertex2f(BTN_X + BTN_W*0.4f, BTN_Y - BTN_H*0.8f);
    glEnd();
}

void drawMatrixGrid(const Matrix& m, float xStart, float yStart, float width, float height, bool forceBlackBg = false) {
    if (forceBlackBg) {
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS); glVertex2f(xStart, yStart); glVertex2f(xStart+width, yStart); glVertex2f(xStart+width, yStart-height); glVertex2f(xStart, yStart-height); glEnd();
    }
    drawFrame(xStart, yStart, width, height);
    float cellW = width / m.cols;
    float cellH = height / m.rows;
    glBegin(GL_QUADS);
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            float val = m(i, j);
            if (val > 0.01f) {
                if (forceBlackBg) glColor3f(val, val, val);
                else glColor3f(0.0f, val, val);
                float x0 = xStart + j * cellW; float y0 = yStart - i * cellH;
                glVertex2f(x0, y0); glVertex2f(x0+cellW, y0); glVertex2f(x0+cellW, y0-cellH); glVertex2f(x0, y0-cellH);
            }
        }
    }
    glEnd();
}

void drawProbabilitiesVertical(const Matrix& probs, float xStart, float yStart, float width, float totalHeight) {
    float rowHeight = totalHeight / 10.0f;
    float maxBarWidth = width * 0.7f;
    float maxP = -1.0f;
    int bestDigit = -1;
    for (int i=0; i<10; ++i) if (probs(i,0) > maxP) { maxP = probs(i,0); bestDigit = i; }
    for (int i = 0; i < 10; ++i) {
        float p = probs(i, 0);
        float currentY = yStart - i * rowHeight;
        if (i == bestDigit && p > 0.1f) glColor3f(0.0f, 1.0f, 0.0f); else glColor3f(0.8f, 0.8f, 0.8f);
        drawDigit(i, xStart, currentY - rowHeight*0.2f, rowHeight*0.3f, rowHeight*0.5f);
        float barLen = p * maxBarWidth;
        float barX = xStart + rowHeight * 0.6f;
        float barY = currentY - rowHeight * 0.2f;
        float barH = rowHeight * 0.5f;
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(barX, barY); glVertex2f(barX+maxBarWidth, barY);
        glVertex2f(barX+maxBarWidth, barY-barH); glVertex2f(barX, barY-barH);
        glEnd();
        if (i == bestDigit && p > 0.1f) glColor3f(0.0f, 1.0f, 0.0f); else glColor3f(0.8f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(barX, barY); glVertex2f(barX+barLen, barY);
        glVertex2f(barX+barLen, barY-barH); glVertex2f(barX, barY-barH);
        glEnd();
    }
}

void handleInput(GLFWwindow* window) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double mx, my; glfwGetCursorPos(window, &mx, &my);
        float glX = (mx / SCREEN_WIDTH) * 2.0f - 1.0f;
        float glY = -((my / SCREEN_HEIGHT) * 2.0f - 1.0f);
        if (glX > BTN_X && glX < BTN_X + BTN_W && glY < BTN_Y && glY > BTN_Y - BTN_H) {
            inputImage.setZero();
            return;
        }
        float boxX = -0.95f; float boxY = 0.9f; float boxW = 1.1f; float boxH = 1.7f; // Ajusté pour laisser place au bouton
        if (glX > boxX && glX < boxX + boxW && glY < boxY && glY > boxY - boxH) {
            float nX = (glX - boxX) / boxW; float nY = (boxY - glY) / boxH;
            int cX = (int)(nX * 28); int cY = (int)(nY * 28);
            float r = 1.8f;
            for (int i = -2; i <= 2; ++i) {
                for (int j = -2; j <= 2; ++j) {
                    if (i*i + j*j <= r*r + 0.5f) {
                        int tR = cY + i; int tC = cX + j;
                        if (tR>=0 && tR<28 && tC>=0 && tC<28)
                            inputImage(tR, tC) = std::min(1.0f, inputImage(tR, tC) + 0.5f);
                    }
                }
            }
        }
    }
}

int main() {
    std::cout << "Chargement V3 (Sans Normalisation)..." << std::endl;
    std::vector<Matrix> kernels;
    Matrix convBias(NUM_FILTERS, 1);
    for (int k = 0; k < NUM_FILTERS; ++k) {
        Matrix kMat(3, 3);
        if (!kMat.load("conv_k" + std::to_string(k) + ".txt")) { std::cerr << "Manque conv_k" << k << std::endl; return 1; }
        kernels.push_back(kMat);
    }
    if (!convBias.load("conv_bias.txt")) return 1;
    Matrix denseWeights(10, 13 * 13 * NUM_FILTERS); if (!denseWeights.load("dense_weights.txt")) return 1;
    Matrix denseBias(10, 1); if (!denseBias.load("dense_bias.txt")) return 1;
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Adobe Internship - Glass Box OCR", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        handleInput(window);
        Matrix globalFlattened(13 * 13 * NUM_FILTERS, 1);
        Matrix firstFeatureMap(26, 26);
        for (int k = 0; k < NUM_FILTERS; ++k) {
            Matrix fm = inputImage.convolve(kernels[k]);
            float b = convBias(k, 0);
            for(float& val : fm.data) val += b;
            fm = fm.applyReLU();
            if (k == 0) firstFeatureMap = fm;
            Matrix pooled = fm.maxPooling(2);
            for (int i = 0; i < pooled.rows * pooled.cols; ++i) {
                globalFlattened.data[k * 169 + i] = pooled.data[i];
            }
        }
        Matrix logits = denseWeights.multiply(globalFlattened).add(denseBias);
        Matrix probs = softmax(logits);

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawMatrixGrid(inputImage, -0.95f, 0.9f, 1.1f, 1.7f, true);

        drawClearButton();

        drawMatrixGrid(firstFeatureMap, 0.2f, 0.9f, 0.35f, 0.7f, false);

        drawProbabilitiesVertical(probs, 0.6f, 0.9f, 0.35f, 1.8f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}