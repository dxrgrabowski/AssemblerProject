#include "pch.h"
#include "Filter.h"


extern "C" __declspec(dllexport) void burkesDitheringCpp(
    unsigned char* input_image,
    unsigned char* output_image,
    const int width,
    const int height,
    const int start_row,
    const int end_row)
{
    for (int y = start_row; y < end_row; ++y) {
        for (int x = 0; x < width; ++x) {
	        const int index = y * width + x;
	        const int old_pixel = input_image[index];
	        const int new_pixel = (old_pixel < 128) ? 0 : 255; // Thresholding
	        const int error = old_pixel - new_pixel;

        	output_image[index] = new_pixel;

            //if (x > 2 && x < width - 3 && y < height - 2)
            //{
            //    input_image[index + 1] += static_cast<int>((8.0 / 32.0) * error);
            //    input_image[index + 2] += static_cast<int>((4.0 / 32.0) * error);
            //    input_image[index + width - 2] += static_cast<int>((2.0 / 32.0) * error);
            //    input_image[index + width - 1] += static_cast<int>((4.0 / 32.0) * error);
            //    input_image[index + width] += static_cast<int>((8.0 / 32.0) * error);
            //    input_image[index + width + 1] += static_cast<int>((4.0 / 32.0) * error);
            //    input_image[index + width + 2] += static_cast<int>((2.0 / 32.0) * error);
            //    continue;
            //}

            // Diffusion of error to neighboring pixels
            if (x < width - 1)
            {
                input_image[index + 1] += static_cast<int>((8.0 / 32.0) * error);
            }
            if (x < width - 2) 
            {
                input_image[index + 2] += static_cast<int>((4.0 / 32.0) * error);
            }
            if (x > 1 && y < height - 1) 
            {
                input_image[index + width - 2] += static_cast<int>((2.0 / 32.0) * error);
            }
            if (x > 0 && y < height - 1) 
            {
                input_image[index + width - 1] += static_cast<int>((4.0 / 32.0) * error);
            }
            if (y < height - 1) 
            {
                input_image[index + width] += static_cast<int>((8.0 / 32.0) * error);
            }
            if (x < width - 1 && y < height - 1) 
            {
                input_image[index + width + 1] += static_cast<int>((4.0 / 32.0) * error);
            }
            if (x < width - 2 && y < height - 1) 
            {
                input_image[index + width + 2] += static_cast<int>((2.0 / 32.0) * error);
            }
        }
    }
}