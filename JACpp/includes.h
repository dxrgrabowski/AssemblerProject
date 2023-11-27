#pragma once
#include "pch.h"
extern "C" __declspec(dllexport)


// Defines the % from every edge to analyze
#define EDGE_ANALYZE_PERCENTAGE 0.9
// Defines the threshold of pixels that are marked as a border
#define HISTOGRAM_TRESHOLD_FLOAT 0.33
// Defines the value (0-255) to start histogram analyze, increase for very dim or images with o
#define HISTOGRAM_ANALYZE_OFFSET 1

// The intensity of moving the border to the center of the image
#define GNAW_INTENSITY 0.004

//remove_setoff procedure 
#define VAL_DIFF_RETURN 3
#define ACTIVATION_JUMP 10
#define MAX_SETOFF_LENGTH 0.015 //value as image dimension multiplier

#define IMG_OK 0

#define ALLIGN_4(n) (( ((n) + 3) / 4) * 4)

#define B(x,y) raw_buffer[((y) * ALLIGN_4(width * 3) + (x) * 3)]
#define G(x,y) raw_buffer[((y) * ALLIGN_4(width * 3) + (x) * 3) + 1]
#define R(x,y) raw_buffer[((y) * ALLIGN_4(width * 3) + (x) * 3) + 2]

//each array contains the pixel position to which the image is to be filled
typedef struct
{
	unsigned short* top_array;
	unsigned short* bot_array;
	unsigned short* left_array;
	unsigned short* right_array;
}Arrays;

typedef struct
{
	unsigned char tresh_r, tresh_g, tresh_b;

	unsigned char r_start, r_end;
	unsigned char g_start, g_end;
	unsigned char b_start, b_end;

	unsigned int hist_r[256];
	unsigned int hist_g[256];
	unsigned int hist_b[256];
}Histo;

// -----------------------------------------------------------------------------

// step1 Initializes the histogram with specified accuracy (% edge from image edge)
int init_hist(
	const unsigned char* raw_buffer,
	unsigned short width,
	unsigned short height,
	Histo* histo,
	double accuracy);

// step2 Initializes the arrays used for border filling with default values
int init_edge_arrays(
	Arrays* arrays,
	unsigned short width,
	unsigned short height);


// step3 Defines the filling range for each channel based on the histogram
void define_filling_range(
	unsigned int hist[],
	unsigned short width,
	unsigned short height,
	unsigned char* start,
	unsigned char* end);

// step4 Finds the peak ranges in the histogram for all channels
int find_fill_range(
	unsigned short width,
	unsigned short height,
	Histo* histo);

// step5 Defines tresholds based on histogram and background color value
int define_treshold(Histo* hist, unsigned char upper_background_tresh);

// step6 Performs threshold-based border line detection
int tresh_border_line(
	const unsigned char* raw_buffer,
	unsigned short width,
	unsigned short height,
	const Histo* histo,
	const Arrays* arrays);


// optional-step7 Removes setoff (sudden value changes) in the arrays
int remove_setoff(
	const Arrays* arrays,
	unsigned short width,
	unsigned short height);

// step7 Increments arrays with desired intensity (moves the border inside the sheet)
int gnaw_edges(
	const Arrays* arrays,
	unsigned short width,
	unsigned short height);

// step8 Fills the border using random fill algorithm (random number in the peak range) 
int border_random_fill(
	unsigned char* raw_buffer,
	unsigned short width,
	unsigned short height,
	const Arrays* arrays,
	const Histo* hist);


