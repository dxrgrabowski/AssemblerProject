#include "pch.h"

#include "includes.h"
#include "tools.h""
#include <stdlib.h>

int init_hist(
    const unsigned char* raw_buffer,
    const unsigned short width,
    const unsigned short height,
    Histo* histo,
    const double accuracy)
{
    unsigned int count = 0;

    const unsigned int left_border = (unsigned int)(accuracy * width);
    const unsigned int top_border = (unsigned int)(accuracy * height);
    const unsigned int right_border = (unsigned int)(width - (int)(accuracy * width));
    const unsigned int bot_border = (unsigned int)(height - (int)(accuracy * height));

    for (int i = 0; i <= 255; i++)
    {
        histo->hist_r[i] = 0;
        histo->hist_g[i] = 0;
        histo->hist_b[i] = 0;
    }

    for (unsigned int row = 0; row < height; row += 2)
    {
        for (unsigned int col = 0; col < width; col += 2)
        {
            if (col < left_border || col > right_border ||
                row < top_border || row > bot_border)
            {
                const unsigned char pixel_r = R(col, row);
                const unsigned char pixel_g = G(col, row);
                const unsigned char pixel_b = B(col, row);

                histo->hist_r[pixel_r]++;
                histo->hist_g[pixel_g]++;
                histo->hist_b[pixel_b]++;
                count++;
            }
        }
    }
    return 0;
}

int init_edge_arrays(
    Arrays* arrays,
    const unsigned short width,
    const unsigned short height)
{
    arrays->left_array = (unsigned short*)malloc(height * sizeof(unsigned short));
    arrays->top_array = (unsigned short*)malloc(width * sizeof(unsigned short));

    arrays->bot_array = (unsigned short*)calloc(width, sizeof(unsigned short));
    arrays->right_array = (unsigned short*)calloc(height, sizeof(unsigned short));

    if (arrays->top_array == NULL || arrays->bot_array == NULL || arrays->left_array == NULL || arrays->right_array == NULL)
    {
        free(arrays->left_array);
        free(arrays->right_array);
        free(arrays->top_array);
        free(arrays->bot_array);

        return 1;
    }

    for (int col = 0; col < width; col++)
    {
        arrays->top_array[col] = height - 1;
    }

    for (int row = 0; row < height; row++)
    {
        arrays->left_array[row] = width - 1;
    }

    return 0;
}


int find_fill_range(
    const unsigned short width,
    const unsigned short height,
    Histo* histo)
{
    unsigned char r_start, r_end;
    define_filling_range(histo->hist_r, width, height, &r_start, &r_end);

    unsigned char g_start, g_end;
    define_filling_range(histo->hist_g, width, height, &g_start, &g_end);

    unsigned char b_start, b_end;
    define_filling_range(histo->hist_b, width, height, &b_start, &b_end);

    histo->r_start = r_start;
    histo->g_start = g_start;
    histo->b_start = b_start;

    histo->r_end = r_end;
    histo->g_end = g_end;
    histo->b_end = b_end;

    return 0;
}

int define_treshold(Histo* hist, const unsigned char upper_background_tresh)
{
    unsigned int max_value_r = 0;
    unsigned int max_value_g = 0;
    unsigned int max_value_b = 0;

    unsigned char max_index_r = 0;
    unsigned char max_index_g = 0;
    unsigned char max_index_b = 0;

    for (int i = HISTOGRAM_ANALYZE_OFFSET; i <= 255; i++)
    {
        if (hist->hist_r[i] > max_value_r)
        {
            max_value_r = hist->hist_r[i];
            max_index_r = (unsigned char)i;
        }
        if (hist->hist_g[i] > max_value_g)
        {
            max_value_g = hist->hist_g[i];
            max_index_g = (unsigned char)i;
        }
        if (hist->hist_b[i] > max_value_b)
        {
            max_value_b = hist->hist_b[i];
            max_index_b = (unsigned char)i;
        }
    }

    hist->tresh_r =
        (unsigned char)(upper_background_tresh +
            ((max_index_r - upper_background_tresh) *
                HISTOGRAM_TRESHOLD_FLOAT));

    hist->tresh_g =
        (unsigned char)(upper_background_tresh +
            ((max_index_g - upper_background_tresh) *
                HISTOGRAM_TRESHOLD_FLOAT));

    hist->tresh_b =
        (unsigned char)(upper_background_tresh +
            ((max_index_b - upper_background_tresh) *
                HISTOGRAM_TRESHOLD_FLOAT));

    return 0;
}


int tresh_border_line(
    const unsigned char* raw_buffer,
    const unsigned short width,
    const unsigned short height,
    const Histo* histo,
    const Arrays* arrays)
{
    unsigned short col, row;
    unsigned char r, g, b;

    const unsigned int max_x_offset = (unsigned int)(EDGE_ANALYZE_PERCENTAGE * width);
    const unsigned int max_y_offset = (unsigned int)(EDGE_ANALYZE_PERCENTAGE * height);

    unsigned char r_tresh, g_tresh, b_tresh;

    find_biggest_treshold(histo, &r_tresh, &g_tresh, &b_tresh);

    for (row = 0; row < height; row++)
    {

        //LEFT TO RIGHT
        for (col = 0; col < max_x_offset; col++)
        {
            r = R(col, row);
            g = G(col, row);
            b = B(col, row);

            if (r >= r_tresh || g >= g_tresh || b >= b_tresh)
            {
                arrays->left_array[row] = col;
                break;
            }
        }

        // RIGHT TO LEFT
        for (col = width - 1; col > width - 1 - max_x_offset && col < width; col--)
        {
            r = R(col, row);
            g = G(col, row);
            b = B(col, row);

            if (r >= r_tresh || g >= g_tresh || b >= b_tresh)
            {
                arrays->right_array[row] = col;
                break;
            }
        }
    }

    for (col = 0; col < width; col++)
    {
        //TOP TO BOTTOM
        for (row = 0; row < max_y_offset; row++)
        {
            r = R(col, row);
            g = G(col, row);
            b = B(col, row);

            if (r >= r_tresh || g >= g_tresh || b >= b_tresh)
            {
                arrays->top_array[col] = row;
                break;
            }
        }

        // BOTTOM TO TOP
        for (row = height - 1; row > height - 1 - max_y_offset && row < height; row--)
        {
            r = R(col, row);
            g = G(col, row);
            b = B(col, row);

            if (r >= r_tresh || g >= g_tresh || b >= b_tresh)
            {
                arrays->bot_array[col] = row;
                break;
            }
        }
    }

    return 0;
}

int remove_setoff(const Arrays* arrays, unsigned short width, const unsigned short height)
{
    // defines max setoff size (if setoff will be bigger, it won't be corrected)
    //int max_setoff_x = (int) (width * MAX_SETOFF_LENGTH);
    const int max_setoff_y = (int)(height * MAX_SETOFF_LENGTH);

    for (int row = (int)(height * 0.04); row <= (int)(height * 0.96); row++)
    {
        const unsigned short last_l = (arrays->left_array[row - 3] +
            arrays->left_array[row - 2] +
            arrays->left_array[row - 1]) / 3;

        if (abs(last_l - arrays->left_array[row]) > ACTIVATION_JUMP)
        {
            for (int j = row; j <= row + max_setoff_y; j++)
            {
                if (arrays->left_array[j] >= last_l - VAL_DIFF_RETURN &&
                    arrays->left_array[j] <= last_l + VAL_DIFF_RETURN)
                {

                    for (int z = row; z != j; z++)
                        arrays->left_array[z] = last_l;

                    row = j;
                    break;
                }
            }
        }
    }

    for (int row = (int)(height * 0.04); row <= (int)(height * 0.96); row++)
    {
        const unsigned short last_r = (arrays->right_array[row - 3] +
            arrays->right_array[row - 2] +
            arrays->right_array[row - 1]) / 3;

        if (abs(last_r - arrays->right_array[row]) > ACTIVATION_JUMP)
        {
            for (int j = row; j <= row + max_setoff_y; j++)
            {
                if (arrays->right_array[j] >= last_r - VAL_DIFF_RETURN &&
                    arrays->right_array[j] <= last_r + VAL_DIFF_RETURN)
                {

                    for (int z = row; z != j; z++)
                        arrays->right_array[z] = last_r;

                    row = j;
                    break;
                }
            }
        }
    }

    return 0;
}


int gnaw_edges(const Arrays* arrays, const unsigned short width, const unsigned short height)
{
    const int intensity = (int)((width + height) / 2 * GNAW_INTENSITY);

    for (int col = 0; col < width; col++)
    {
        if (arrays->top_array[col] + intensity < height)
            arrays->top_array[col] += intensity;

        if (arrays->bot_array[col] - intensity >= 0)
            arrays->bot_array[col] -= intensity;
    }

    for (int row = 0; row < height; row++)
    {
        if (arrays->left_array[row] + intensity < width)
            arrays->left_array[row] += intensity;

        if (arrays->right_array[row] - intensity >= 0)
            arrays->right_array[row] -= intensity;
    }

    return 0;
}

extern "C" __declspec(dllexport) unsigned char* border_random_fill(
    unsigned char* raw_buffer,
    int width,
    int height,
    const Arrays &arrays,
    const Histo &hist)
{
    unsigned char r, g, b;
    rand_seed(255, 0);

    for (int row = 0; row < height; row++)
    {
        // LEFT TO RIGHT
        for (int col = arrays.left_array[row]; col >= 0; col--)
        {
            r = (unsigned char)get_rand_value(hist.r_start, hist.r_end);
            g = (unsigned char)get_rand_value(hist.g_start, hist.g_end);
            b = (unsigned char)get_rand_value(hist.b_start, hist.b_end);
            R(col, row) = r;
            G(col, row) = g;
            B(col, row) = b;
        }
        // RIGHT TO LEFT
        for (int col = arrays.right_array[row]; col < width; col++)
        {
            r = (unsigned char)get_rand_value(hist.r_start, hist.r_end);
            g = (unsigned char)get_rand_value(hist.g_start, hist.g_end);
            b = (unsigned char)get_rand_value(hist.b_start, hist.b_end);
            R(col, row) = r;
            G(col, row) = g;
            B(col, row) = b;
        }
    }

    for (int col = 0; col < width; col++)
    {
        // TOP TO BOTTOM
        for (int row = arrays.top_array[col]; row >= 0; row--)
        {
            r = (unsigned char)get_rand_value(hist.r_start, hist.r_end);
            g = (unsigned char)get_rand_value(hist.g_start, hist.g_end);
            b = (unsigned char)get_rand_value(hist.b_start, hist.b_end);
            R(col, row) = r;
            G(col, row) = g;
            B(col, row) = b;
        }
        // BOTTOM TO TOP
        for (int row = arrays.bot_array[col]; row < height; row++)
        {
            r = (unsigned char)get_rand_value(hist.r_start, hist.r_end);
            g = (unsigned char)get_rand_value(hist.g_start, hist.g_end);
            b = (unsigned char)get_rand_value(hist.b_start, hist.b_end);
            R(col, row) = r;
            G(col, row) = g;
            B(col, row) = b;
        }
    }

    return raw_buffer;
}