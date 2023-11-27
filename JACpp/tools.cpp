#include "pch.h"

#include "tools.h"

static unsigned int SEED_X = 521288629;
static unsigned int SEED_Y = 362436069;

void rand_seed(const unsigned int seed1, const unsigned int seed2)
{
    if (seed1) SEED_X = seed1;   /* use default seeds if parameter is 0 */
    if (seed2) SEED_Y = seed2;
}

unsigned int randX()
{
    static unsigned int a = 18000, b = 30903;

    SEED_X = a * (SEED_X & 65535) + (SEED_X >> 16);
    SEED_Y = b * (SEED_Y & 65535) + (SEED_Y >> 16);

    return ((SEED_X << 16) + (SEED_Y & 65535));
}

unsigned get_rand_value(const int start, const int end)
{
    const int range = end - start + 1;
    const int unsigned randomValue = randX() % range + start;

    return randomValue;
}

int find_biggest_treshold(
    const Histo* hist,
    unsigned char* r_tresh,
    unsigned char* g_tresh,
    unsigned char* b_tresh)
{
    if (hist->tresh_r >= hist->tresh_g && hist->tresh_r >= hist->tresh_b)
    {
        *r_tresh = (unsigned char)(hist->tresh_r);
        *g_tresh = 255;
        *b_tresh = 255;
    }
    else if (hist->tresh_g > hist->tresh_r && hist->tresh_g > hist->tresh_b)
    {
        *r_tresh = 255;
        *g_tresh = (unsigned char)(hist->tresh_g);
        *b_tresh = 255;
    }
    else
    {
        *r_tresh = 255;
        *g_tresh = 255;
        *b_tresh = (unsigned char)(hist->tresh_b);
    }

    return 0;
}

void define_filling_range(
    unsigned int hist[],
    const unsigned short width,
    const unsigned short height,
    unsigned char* start,
    unsigned char* end)
{
    unsigned int max_value = 0;
    unsigned int max_index = 0;

    const unsigned int lower_threshold = (unsigned int)(0.009 * (height * width) / 3);
    const unsigned int upper_threshold = (unsigned int)(0.009 * (height * width) / 3);

    for (int i = HISTOGRAM_ANALYZE_OFFSET; i <= 255; i++)
    {
        if (hist[i] > max_value)
        {
            max_value = hist[i];
            max_index = i;
        }
    }

    int i = (int)max_index;

    while (i >= 0 && hist[i] >= lower_threshold)
        i--;

	unsigned char lower_bound = (unsigned char)(i + 1);

    // Search upwards from the maximum value to find the upper bound
    i = (int)max_index;
    while (i <= 255 && hist[i] >= upper_threshold)
        i++;

    unsigned char upper_bound = (unsigned char)(i - 1);

    if (upper_bound <= lower_bound)
    {
        if (upper_bound <= 255 - 2)
        {
            upper_bound = (unsigned char)(lower_bound + 2);
        }
        else
        {
            lower_bound = (unsigned char)(upper_bound - 2);
        }
    }

    *start = lower_bound;
    *end = upper_bound;
}