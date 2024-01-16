#include "pch.h"
#include "Filter.h"


extern "C" __declspec(dllexport) void burkesDitheringCpp(unsigned char* inputImage, unsigned char* outputImage, int width, int height, int startRow, int endRow)
{
    for (int y = startRow; y < endRow; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            int oldPixel = inputImage[index];
            int newPixel = (oldPixel < 128) ? 0 : 255; // Thresholding

            outputImage[index] = newPixel;

            int error = oldPixel - newPixel;

            // Diffusion of error to neighboring pixels
            if (x < width - 1) {
                inputImage[index + 1] += (int)((8.0 / 32.0) * error);
            }
            if (x < width - 2) {
                inputImage[index + 2] += (int)((4.0 / 32.0) * error);
            }
            if (x > 1 && y < height - 1) {
                inputImage[index + width - 2] += (int)((2.0 / 32.0) * error);
            }
            if (x > 0 && y < height - 1) {
                inputImage[index + width - 1] += (int)((4.0 / 32.0) * error);
            }
            if (y < height - 1) {
                inputImage[index + width] += (int)((8.0 / 32.0) * error);
            }
            if (x < width - 1 && y < height - 1) {
                inputImage[index + width + 1] += (int)((4.0 / 32.0) * error);
            }
            if (x < width - 2 && y < height - 1) {
                inputImage[index + width + 2] += (int)((2.0 / 32.0) * error);
            }
        }
    }
}