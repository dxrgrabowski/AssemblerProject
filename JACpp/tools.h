#pragma once
#include "includes.h"

unsigned int randX();          
void  rand_seed(unsigned int, unsigned int);     

unsigned get_rand_value(int start, int end);

int find_biggest_treshold(
	const Histo* hist,
	unsigned char* r_tresh,
	unsigned char* g_tresh,
	unsigned char* b_tresh);

void define_filling_range(
	unsigned int hist[],
	unsigned short width,
	unsigned short height,
	unsigned char* start,
	unsigned char* end);