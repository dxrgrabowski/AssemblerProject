// pch.cpp: plik źródłowy odpowiadający wstępnie skompilowanemu nagłówkowi
#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"

#include <stdio.h>
#include <stdlib.h>

#define BMP_MB 0x4d42 // ('MB')


#pragma pack(push, 1) // for single access fread copy
typedef struct bmp_file_header {
    unsigned short  type;
    unsigned int    size;
    unsigned int    reserved;
    unsigned int    bitmap_offset;
    unsigned int    header_size;
    unsigned int    width;
    unsigned int    height;
    unsigned short  planes;
    unsigned short  bits_per_pixel;
    unsigned int    compression;
    unsigned int    bitmap_size;
    unsigned int    horizontal_resolution;
    unsigned int    vertical_resolution;
    unsigned int    num_colors;
    unsigned int    num_important_colors;
} bmp_file_header;
#pragma pack(pop)

EXPORTED_METHOD
int bmp_get_diameters(
    const char* file_name,
    unsigned short* width,
    unsigned short* height)
{
    bmp_file_header
        bmp_header;

    FILE* input_file = fopen(file_name, "rb");
    if (!input_file) { return 0; }
    fread(&bmp_header, sizeof(bmp_header), 1, input_file);
    fclose(input_file);
    if (bmp_header.type != BMP_MB ||
        bmp_header.planes != 1 ||
        bmp_header.bits_per_pixel != 24)
    {
        return 0; // format currently not supported
    }
    *width = (unsigned short)(bmp_header.width);
    *height = (unsigned short)(bmp_header.height);
    return 1;
}

EXPORTED_METHOD
int bmp_decode(const char* file_name, unsigned char* raw_buffer)
{
    bmp_file_header bmp_header;

    FILE* input_file = fopen(file_name, "rb");
    if (!input_file) { return 0; }

    fread(&bmp_header, sizeof(bmp_header), 1, input_file);
    if (bmp_header.type != BMP_MB ||
        bmp_header.planes != 1 ||
        bmp_header.bits_per_pixel != 24 ||
        bmp_header.width == 0)
    {
        fclose(input_file);
        return 0;
    }

    fseek(input_file, (long)bmp_header.bitmap_offset, SEEK_SET);

    const unsigned short width = (unsigned short)bmp_header.width;
    const unsigned short height = (unsigned short)bmp_header.height;

    const unsigned short line_length = ((width * 3 + 3) / 4) * 4;
    const unsigned int buffer_length = (unsigned int)line_length * height;

    fread(raw_buffer, (size_t)buffer_length, 1, input_file);

    fclose(input_file);
    return 1;
}

EXPORTED_METHOD
int bmp_encode(
    const char* file_name,
    const unsigned short width,
    const unsigned short height,
    const unsigned char* raw_buffer)
{

    bmp_file_header
        bmp_header = { 0 };

    if (width < 1 || height < 1) { return 0; }
    const unsigned short line_length = (unsigned short)((width * 3 + 3) / 4) * 4;
    bmp_header.bitmap_offset = sizeof(bmp_header);
    bmp_header.bitmap_size = ((unsigned int)line_length) * height;

    // also greyscale gets encoded in 24 bits
    bmp_header.bits_per_pixel = (unsigned short)24;
    bmp_header.compression = 0;
    bmp_header.header_size = sizeof(bmp_header) - 14;
    bmp_header.height = height;
    bmp_header.horizontal_resolution = 200;
    bmp_header.num_colors = 0;
    bmp_header.num_important_colors = 0;
    bmp_header.planes = 1;
    bmp_header.reserved = 0;
    bmp_header.size =
        ((unsigned int)line_length) * height + (unsigned int)sizeof(bmp_header);
    bmp_header.type = BMP_MB;
    bmp_header.vertical_resolution = 200;
    bmp_header.width = width;

    FILE* output_file = fopen(file_name, "wb");
    if (!output_file) { return 0; }

    fwrite(
        &bmp_header,
        sizeof(bmp_header),
        1,
        output_file);

    fwrite(raw_buffer, bmp_header.bitmap_size, 1, output_file);

    fclose(output_file);
    return 1;
}
