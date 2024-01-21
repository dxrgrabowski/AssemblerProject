#pragma once
#include "pch.h"

#include <vector>
#include <numeric>

extern "C" __declspec(dllexport) void burkesDitheringCpp(
    unsigned char* input_image,
    unsigned char* output_image,
    const int width,
    const int height,
    const int start_row,
    const int end_row);


int bayerMatrix[4][4] = {
    { 0,  8,  2, 10},
    {12,  4, 14,  6},
    { 3, 11,  1,  9},
    {15,  7, 13,  5}
};
unsigned char ditheringFilter[4] = { 6, 3, 5, 2 };

int** generateBayerMatrix(int size) {
    int** bayerMatrix = new int* [size];
    for (int i = 0; i < size; ++i) {
        bayerMatrix[i] = new int[size];
    }

    int level = 1;
    int subSize = size;
    while (subSize > 1) {
        subSize /= 2;
        int halfLevel = level / 2;
        for (int i = 0; i < level; ++i) {
            for (int j = 0; j < level; ++j) {
                bayerMatrix[i][j] = ((bayerMatrix[i % subSize][j % subSize] * 4) + ((i / subSize) * 2) + (j / subSize)) % (level * level);
            }
        }
        level *= 2;
    }

    return bayerMatrix;
}

// Function to free memory allocated for the Bayer matrix
void freeBayerMatrix(int** bayerMatrix, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] bayerMatrix[i];
    }
    delete[] bayerMatrix;
}



// Function to perform ordered error dispersion dithering
extern "C" __declspec(dllexport) void orderedErrorDispersion(unsigned char* inputImage, unsigned char* outputImage, int width, int height, int numThreads) {
    // Define the Bayer matrix size (change this value for different matrix sizes)
    int bayerSize = 4;

    // Generate the Bayer matrix dynamically
    int** bayerMatrix = generateBayerMatrix(bayerSize);

    // Bayer dithering
    //for (int y = 0; y < height; ++y) {
    //    for (int x = 0; x < width; ++x) {
    //        int index = y * width + x;
    //        int threshold = (bayerMatrix[y % bayerSize][x % bayerSize] * 255) / (bayerSize * bayerSize - 1);

    //        // Calculate the new pixel value based on the threshold and the error diffusion
    //        int newValue = (inputImage[index] + threshold <= 255) ? 0 : 255;
    //        int error = inputImage[index] - newValue;
    //        inputImage[index] = static_cast<unsigned char>(newValue);

    //        // Distribute the error to neighboring pixels
    //        if (x < width - 1) inputImage[index + 1] += (error) / 16;
    //        if (x > 0 && y < height - 1) inputImage[index + width - 1] += (error) / 16;
    //        if (y < height - 1) inputImage[index + width] += (error) / 16;
    //        if (x < width - 1 && y < height - 1) inputImage[index + width + 1] += (error) / 16;
    //    }
    //}

    //for (int y = 0; y < height; y++)
    //{
    //    for (int x = 0; x < width; x++)
    //    {
    //        if (x - 1 >= 0 && y + 1 < height && x + 1 < width)
    //        {
    //            unsigned char oldPixel[3];
    //            unsigned char newPixel = 0xFF;
    //            unsigned char others[4][3];
    //            unsigned char otherValues[4];
    //            char error = 0x00;
    //            //unsigned float accumulatedValue = 0f;

    //            if (x + 1 < width && y < height) {
    //                others[0][0] = inputImage[((y)*width + (x + 1)) * 3];
    //                others[0][1] = inputImage[((y)*width + (x + 1)) * 3 + 1];
    //                others[0][2] = inputImage[((y)*width + (x + 1)) * 3 + 2];
    //            }

    //            if (x - 1 >= 0 && y + 1 < height) {
    //                others[1][0] = inputImage[((y + 1) * width + (x - 1)) * 3];
    //                others[1][1] = inputImage[((y + 1) * width + (x - 1)) * 3 + 1];
    //                others[1][2] = inputImage[((y + 1) * width + (x - 1)) * 3 + 2];
    //            }

    //            if (x < width && y + 1 < height) {
    //                others[2][0] = inputImage[((y + 1) * width + x) * 3];
    //                others[2][1] = inputImage[((y + 1) * width + x) * 3 + 1];
    //                others[2][2] = inputImage[((y + 1) * width + x) * 3 + 2];
    //            }

    //            if (x + 1 < width && y + 1 < height) {
    //                others[3][0] = inputImage[((y + 1) * width + (x + 1)) * 3];
    //                others[3][1] = inputImage[((y + 1) * width + (x + 1)) * 3 + 1];
    //                others[3][2] = inputImage[((y + 1) * width + (x + 1)) * 3 + 2];
    //            }

    //            if (oldPixel[0] < 0x80)
    //                newPixel = 0x00;

    //            outputImage[(y * width + x) * 3] = newPixel;
    //            outputImage[(y * width + x) * 3 + 1] = newPixel;
    //            outputImage[(y * width + x) * 3 + 2] = newPixel;

    //            error = oldPixel[0] - newPixel;

    //            for (int i = 0; i < sizeof(ditheringFilter) / sizeof(unsigned char); i++)
    //            {
    //                for (int c = 0; c < 3; c++) {
    //                    otherValues[i] = others[i][c] + error * ditheringFilter[i] * 0.0625;
    //                    if (otherValues[i] < 0)
    //                        otherValues[i] = 0;
    //                    if (otherValues[i] > 255)
    //                        otherValues[i] = 255;
    //                    outputImage[((y + 1) * width + (x + 1)) * 3 + c] = otherValues[i];
    //                }
    //            }
    //        }
    //    }
    //}

    // Free the allocated Bayer matrix memory
    //freeBayerMatrix(bayerMatrix, bayerSize);
}

void floydSteinbergDithering(unsigned char* inputImage, unsigned char* outputImage, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x - 1 >= 0 && y + 1 < height && x + 1 < width)
            {
                unsigned char oldPixel[3];
                unsigned char newPixel = 0xFF;
                unsigned char others[4][3];
                unsigned char otherValues[4];
                char error = 0x00;

                oldPixel[0] = inputImage[y * width + x + 0];
                oldPixel[1] = inputImage[y * width + x + 1];
                oldPixel[2] = inputImage[y * width + x + 2];

                others[0][0] = inputImage[y * width + x + 1];
                others[0][1] = inputImage[y * width + x + 2];
                others[0][2] = inputImage[y * width + x + 3];

                others[1][0] = inputImage[y * width + x - 1];
                others[1][1] = inputImage[y * width + x + 0];
                others[1][2] = inputImage[y * width + x + 1];

                others[2][0] = inputImage[(y + 1) * width + x];
                others[2][1] = inputImage[(y + 1) * width + x];
                others[2][2] = inputImage[(y + 1) * width + x];

                others[3][0] = inputImage[(y + 1) * width + x + 1];
                others[3][1] = inputImage[(y + 1) * width + x + 2];
                others[3][2] = inputImage[(y + 1) * width + x + 3];

                if (oldPixel[0] < 0x80)
                    newPixel = 0x00;

                error = oldPixel[0] - newPixel;
                outputImage[y * width + x + 0] = newPixel;
                outputImage[y * width + x + 1] = newPixel;
                outputImage[y * width + x + 2] = newPixel;

                for (int i = 0; i < sizeof(ditheringFilter) / sizeof(unsigned char); i++)
                {
                    otherValues[i] = others[i][0] + error * ditheringFilter[i] * 0.0625;
                }

                outputImage[y * width + x + 1] = otherValues[0];
                outputImage[y * width + x + 2] = otherValues[0];
                outputImage[y * width + x + 3] = otherValues[0];

            	outputImage[(y + 1) * width + x - 1] = otherValues[1];
                outputImage[(y + 1) * width + x + 0] = otherValues[1];
                outputImage[(y + 1) * width + x + 1] = otherValues[1];

                outputImage[(y + 1) * width + x + 0] = otherValues[2];
                outputImage[(y + 1) * width + x + 1] = otherValues[2];
                outputImage[(y + 1) * width + x + 2] = otherValues[2];

            	outputImage[(y + 1) * width + x + 1] = otherValues[3];
                outputImage[(y + 1) * width + x + 2] = otherValues[3];
                outputImage[(y + 1) * width + x + 3] = otherValues[3];
            }
        }
    }
}

void bayerDithering(unsigned char* inputImage, unsigned char* outputImage, int width, int height) {
    // Iterate through the image pixels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            int oldPixel = inputImage[index];

            // Mapowanie piksela do wartoœci progu z macierzy Bayer'a
            int threshold = (bayerMatrix[x % 4][y % 4] * 255) / 16;

            int newPixel = (oldPixel < threshold) ? 0 : 255; // Thresholding

            outputImage[index] = newPixel;

            int error = oldPixel - newPixel;

            // Diffusion of error to neighboring pixels
            if (x < width - 1) {
                inputImage[index + 1] += (int)((7.0 / 16.0) * error);
            }
            if (x > 0 && y < height - 1) {
                inputImage[index + width - 1] += (int)((3.0 / 16.0) * error);
            }
            if (y < height - 1) {
                inputImage[index + width] += (int)((5.0 / 16.0) * error);
            }
            if (x < width - 1 && y < height - 1) {
                inputImage[index + width + 1] += (int)((1.0 / 16.0) * error);
            }
        }
    }
}
