#pragma once
#include "pch.h"
#include <cstdlib>

#include "includes.h"

extern "C" __declspec(dllexport) unsigned char* fill_edge_and_punch_hole(unsigned char* raw_buffer, int width, int height, Arrays arrays, Histo histo)
{
    unsigned char bg_tresh = 3;

    unsigned long long  start_img = GetTickCount64();

    init_edge_arrays(&arrays, width, height);

    init_hist(raw_buffer, width, height, &histo, 0.2);

    find_fill_range(width, height, &histo);
    define_treshold(&histo, bg_tresh);

    tresh_border_line(raw_buffer, width, height, &histo, &arrays);

    remove_setoff(&arrays, width, height);
    gnaw_edges(&arrays, width, height);

    //border_random_fill(raw_buffer, width, height, &arrays, &histo);

   // free(arrays.top_array);
    //free(arrays.bot_array);
   // free(arrays.left_array);
   // free(arrays.right_array);

    return raw_buffer;
}
