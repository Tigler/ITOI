//
// Created by tigler on 13.02.18.
//

#include <cstdio>
#include <algorithm>
#include "ImageReader.h"
#include "jpeglib.h"
#include <iostream>



static double brightness(unsigned char r, unsigned char g, unsigned char b){
    return 0.3*r+0.59*g+0.11*b;
}

Image ImageReader::read(const char* path) {

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        std::cout<<"Не удалось найти файл по заданному пути";
    }

    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr err;

    info.err = jpeg_std_error(&err);
    jpeg_create_decompress(&info);

    jpeg_stdio_src(&info, file);
    jpeg_read_header(&info, true);

    jpeg_start_decompress(&info);

    info.out_color_space=JCS_RGB;
    unsigned int w = info.output_width;
    unsigned int h = info.output_height;
    int numChannels = info.num_components;
    unsigned long dataSize = w * h * numChannels;

    unsigned char *data = (unsigned char *) malloc(dataSize);
    unsigned char *rowptr;
    Image image = Image(w,h);
    image.setWidth(w);
    image.setHeight(h);
    for (int i = 0; i < h; i++) {
        rowptr = data + info.output_scanline * w * numChannels;
        jpeg_read_scanlines(&info, &rowptr, 1);
        for (int j = 0; j < w*3; j+=3) {
            image.setImageItem(i*w+(j==0?0:j/3), brightness(rowptr[j],rowptr[j+1],rowptr[j+2]));
        }
    }

    jpeg_finish_decompress( &info );

    fclose(file);

    return image;
}

ImageReader::ImageReader() = default;


