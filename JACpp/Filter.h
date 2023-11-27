#pragma once
#include "pch.h"

#include <iostream>
#include <vector>

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

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
//extern "C" __declspec(dllexport) void ApplyKuwaharaFilter(unsigned char* inputImage, unsigned char* outputImage, int width, int height) {
//	constexpr int regionSize = 5;
//
//    auto CalculateAverageColor = [&](int x, int y) {
//        int rSum = 0, gSum = 0, bSum = 0;
//        for (int i = x; i < x + regionSize && i < width; ++i) {
//            for (int j = y; j < y + regionSize && j < height; ++j) {
//                int idx = (i * width + j) * 3;
//                rSum += inputImage[idx];
//                gSum += inputImage[idx + 1];
//                bSum += inputImage[idx + 2];
//            }
//        }
//        int numPixels = regionSize * regionSize;
//        return RGBColor{ static_cast<unsigned char>(rSum / numPixels),
//                         static_cast<unsigned char>(gSum / numPixels),
//                         static_cast<unsigned char>(bSum / numPixels) };
//        };
//
//    int numThreads = std::thread::hardware_concurrency();
//    std::vector<std::thread> threads;
//    int step = height / numThreads;
//
//    for (int t = 0; t < numThreads; ++t) {
//        threads.emplace_back([=, &inputImage, &outputImage]() {
//            int startRow = t * step;
//            int endRow = (t == numThreads - 1) ? height : (startRow + step);
//            for (int i = startRow; i < endRow; ++i) {
//                for (int j = 0; j < width; ++j) {
//                    int x = i - regionSize / 2;
//                    int y = j - regionSize / 2;
//                    if (x < 0) x = 0;
//                    if (y < 0) y = 0;
//                    int xEnd = min(height, x + regionSize);
//                    int yEnd = min(width, y + regionSize);
//
//                    std::vector<RGBColor> regions[4];
//
//                    for (int k = 0; k < 4; ++k) {
//                        int xStart = x + k / 2 * regionSize / 2;
//                        int yStart = y + k % 2 * regionSize / 2;
//                        regions[k].resize(regionSize * regionSize);
//                        int idx = 0;
//
//                        for (int xRegion = xStart; xRegion < xEnd; ++xRegion) {
//                            for (int yRegion = yStart; yRegion < yEnd; ++yRegion) {
//                                int idxRegion = (xRegion * width + yRegion) * 3;
//                                regions[k][idx++] = RGBColor{ inputImage[idxRegion],
//                                                             inputImage[idxRegion + 1],
//                                                             inputImage[idxRegion + 2] };
//                            }
//                        }
//                    }
//
//                    // Obliczanie œredniej wartoœci koloru i wariancji w obszarach
//                    RGBColor avgColors[4];
//                    double variances[4];
//
//                    for (int k = 0; k < 4; ++k) {
//                        avgColors[k] = CalculateAverageColor(x + k / 2 * regionSize / 2, y + k % 2 * regionSize / 2);
//
//                        double variance = 0;
//                        for (int idx = 0; idx < regionSize * regionSize; ++idx) {
//                            int dr = regions[k][idx].r - avgColors[k].r;
//                            int dg = regions[k][idx].g - avgColors[k].g;
//                            int db = regions[k][idx].b - avgColors[k].b;
//                            variance += dr * dr + dg * dg + db * db;
//                        }
//                        variances[k] = variance / (regionSize * regionSize);
//                    }
//
//                    // Wybieramy obszar o najmniejszej wariancji
//                    int minVarianceIndex = std::min_element(variances, variances + 4) - variances;
//                    outputImage[(i * width + j) * 3] = avgColors[minVarianceIndex].r;
//                    outputImage[(i * width + j) * 3 + 1] = avgColors[minVarianceIndex].g;
//                    outputImage[(i * width + j) * 3 + 2] = avgColors[minVarianceIndex].b;
//                }
//            }
//            });
//    }
//
//    for (auto& thread : threads) {
//        thread.join();
//    }
//}


//int CalculateMean(const std::vector<int>& pixels) {
//    int sum = 0;
//    for (int pixel : pixels) {
//        sum += pixel;
//    }
//    return sum / pixels.size();
//}

//extern "C" __declspec(dllexport) void ApplyKuwaharaFilter(const unsigned char* inputImage, unsigned char* outputImage, int width, int height) {
//    const int filterSize = 3;
//    const int halfFilterSize = filterSize / 2;
//
//    for (int y = halfFilterSize; y < height - halfFilterSize; ++y) {
//        for (int x = halfFilterSize; x < width - halfFilterSize; ++x) {
//            std::vector<int> means[3];
//
//            for (int c = 0; c < 3; ++c) {
//                means[c].resize(filterSize, 0);
//            }
//
//            for (int i = -halfFilterSize; i <= halfFilterSize; ++i) {
//                for (int j = -halfFilterSize; j <= halfFilterSize; ++j) {
//                    for (int c = 0; c < 3; ++c) {
//                        int index = ((y + i) * width + (x + j)) * 3 + c;
//                        int pixel = inputImage[index];
//                        means[c][i + halfFilterSize] += pixel;
//                    }
//                }
//            }
//
//            for (int c = 0; c < 3; ++c) {
//                means[c][0] = CalculateMean(means[c]);
//            }
//
//            int minVariance = INT_MAX;
//            int selectedMean[3];
//
//            for (int i = 0; i < filterSize; ++i) {
//                int variance[3] = { 0, 0, 0 };
//                for (int j = 0; j < filterSize; ++j) {
//                    for (int c = 0; c < 3; ++c) {
//                        int index = ((y - halfFilterSize + i) * width + (x - halfFilterSize + j)) * 3 + c;
//                        int pixel = inputImage[index];
//                        int diff = pixel - means[c][i];
//                        variance[c] += diff * diff;
//                    }
//                }
//
//                int totalVariance = variance[0] + variance[1] + variance[2];
//                if (totalVariance < minVariance) {
//                    minVariance = totalVariance;
//                    selectedMean[0] = means[0][i];
//                    selectedMean[1] = means[1][i];
//                    selectedMean[2] = means[2][i];
//                }
//            }
//
//            for (int c = 0; c < 3; ++c) {
//                int index = (y * width + x) * 3 + c;
//                outputImage[index] = selectedMean[c];
//            }
//        }
//    }
//}


