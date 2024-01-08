#pragma once
#include "pch.h"

#include <iostream>
#include <vector>

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>

int CalculateMean(const std::vector<int>& pixels) {
    int sum = 0;
    for (int pixel : pixels) {
        sum += pixel;
    }
    return sum / pixels.size();
}

// Funkcja pomocnicza do przetwarzania obszaru obrazu za pomoc¹ filtru Kuwahara
void ProcessRegion(const unsigned char* inputImage, unsigned char* outputImage, int width, int height, int x, int y) {
    const int filterSize = 3;
    const int halfFilterSize = filterSize / 2;

    std::vector<int> means[3];

    for (int c = 0; c < 3; ++c) {
        means[c].resize(filterSize, 0);
    }

    for (int i = -halfFilterSize; i <= halfFilterSize; ++i) {
        for (int j = -halfFilterSize; j <= halfFilterSize; ++j) {
            for (int c = 0; c < 3; ++c) {
                int px = x + j;
                int py = y + i;

                // Check if the pixel coordinates are within the bounds of the image
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    int index = (py * width + px) * 3 + c;
                    int pixel = inputImage[index];
                    means[c][i + halfFilterSize] += pixel;
                }
            }
        }
    }

    for (int c = 0; c < 3; ++c) {
        means[c][0] = CalculateMean(means[c]);
    }

    int minVariance = INT_MAX;
    int selectedMean[3];

    for (int i = 0; i < filterSize; ++i) {
        int variance[3] = { 0, 0, 0 };
        for (int j = 0; j < filterSize; ++j) {
            for (int c = 0; c < 3; ++c) {
                int px = x - halfFilterSize + j;
                int py = y - halfFilterSize + i;

                // Check if the pixel coordinates are within the bounds of the image
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    int index = (py * width + px) * 3 + c;
                    int pixel = inputImage[index];
                    int diff = pixel - means[c][i];
                    variance[c] += diff * diff;
                }
            }
        }

        int totalVariance = variance[0] + variance[1] + variance[2];
        if (totalVariance < minVariance) {
            minVariance = totalVariance;
            selectedMean[0] = means[0][i];
            selectedMean[1] = means[1][i];
            selectedMean[2] = means[2][i];
        }
    }

    for (int c = 0; c < 3; ++c) {
        int index = (y * width + x) * 3 + c;
        outputImage[index] = selectedMean[c];
    }
}

// Funkcja przetwarzaj¹ca obraz za pomoc¹ filtru Kuwahara z wielow¹tkowoœci¹
void ApplyKuwaharaFilter(const unsigned char* inputImage, unsigned char* outputImage, int width, int height, int numThreads) {
    const int filterSize = 3;
    const int halfFilterSize = filterSize / 2;

    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; ++t) {
        int startY = t * height / numThreads;
        int endY = (t + 1) * height / numThreads;

        threads.emplace_back([&inputImage, &outputImage, width, height](int startY, int endY) {
            for (int y = startY + halfFilterSize; y < endY - halfFilterSize; ++y) {
                for (int x = halfFilterSize; x < width - halfFilterSize; ++x) {
                    ProcessRegion(inputImage, outputImage, width, height, x, y);
                }
            }
            }, startY, endY);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

extern "C" __declspec(dllexport) unsigned char* KuwaharaFilter(const unsigned char* inputImage, int width, int height, int numThreads) {
    unsigned char* outputImage = new unsigned char[width * height * 3];
    ApplyKuwaharaFilter(inputImage, outputImage, width, height, numThreads);
    return outputImage;
}


void SetPixelValue(unsigned char* imageData, int width, int x, int y, unsigned char value) {
    imageData[y * width + x] = value;
}

unsigned char GetPixelValue(const unsigned char* imageData, int width, int x, int y) {
    return imageData[y * width + x];
}

// Funkcja pomocnicza: Obliczanie rozmycia Gaussa dla danego piksela
unsigned char GaussianBlur(const unsigned char* imageData, int width, int height, int x, int y) {
    float kernel[3][3] = { {1.0f, 2.0f, 1.0f},
                          {2.0f, 4.0f, 2.0f},
                          {1.0f, 2.0f, 1.0f} };
    float sum = 0.0f;
    float totalWeight = 16.0f; // Suma wag kernela

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = x + i;
            int newY = y + j;

            // Sprawdzenie granic obrazu
            if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                unsigned char pixelValue = GetPixelValue(imageData, width, newX, newY);
                float weight = kernel[i + 1][j + 1];
                sum += weight * static_cast<float>(pixelValue);
            }
            else {
                totalWeight -= kernel[i + 1][j + 1];
            }
        }
    }

    return static_cast<unsigned char>(sum / totalWeight);
}

extern "C" __declspec(dllexport)
unsigned char* KuwaharaFilter2(const unsigned char* inputImage, int width, int height) {
    unsigned char* outputImage = new unsigned char[width * height];

    // Pêtle przechodz¹ce przez wszystkie piksele obrazu
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char minVariance = UCHAR_MAX;
            unsigned char average[4] = { 0 };

            // Obliczanie wartoœci dla ka¿dego z czterech kwadratów Kuwahara
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    unsigned char val1 = GaussianBlur(inputImage, width, height, x + i, y + j);
                    unsigned char val2 = GaussianBlur(inputImage, width, height, x + i + 1, y + j);
                    unsigned char val3 = GaussianBlur(inputImage, width, height, x + i, y + j + 1);
                    unsigned char val4 = GaussianBlur(inputImage, width, height, x + i + 1, y + j + 1);

                    average[i * 2 + j] = (val1 + val2 + val3 + val4) / 4;
                }
            }

            // Obliczanie wariancji dla ka¿dego kwadratu
            for (int i = 0; i < 4; ++i) {
                unsigned char variance = 0;
                for (int j = 0; j < 4; ++j) {
                    variance += (average[i] - average[j]) * (average[i] - average[j]);
                }
                if (variance < minVariance) {
                    minVariance = variance;
                    SetPixelValue(outputImage, width, x, y, average[i]);
                }
            }
        }
    }

    return outputImage;
}

