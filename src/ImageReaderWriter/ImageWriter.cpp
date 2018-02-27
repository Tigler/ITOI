//
// Created by tigler on 16.02.18.
//

#include <cstdio>
#include "ImageWriter.h"
#include "../Image/Image.h"

extern "C" {
#include "jpeglib.h"
};

int max1(unsigned char m,int p){
    return m+p>=255?255:m+p;
}

void ImageWriter::write(const char* path,Image* image) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE *outfile;
    JSAMPROW row_pointer[1];
    int row_stride;


    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(path, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", path);
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);


    cinfo.image_width = image->getWidth();
    cinfo.image_height = image->getHeight();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, 75, TRUE);

    jpeg_start_compress(&cinfo, TRUE);


    row_stride = image->getWidth()*3;
    
    unsigned char* imageForOut = new unsigned char[image->getHeight()*image->getWidth()*3];

    for (int i = 0; i < image->getHeight(); i++) {
        for (int j = 0; j < image->getWidth()*3; j+=3) {
            imageForOut[i*image->getWidth()*3+j]=image->getImage()[i*image->getWidth()+(j==0?0:j/3)];
            imageForOut[i*image->getWidth()*3+j+1]=image->getImage()[i*image->getWidth()+(j==0?0:j/3)];
            imageForOut[i*image->getWidth()*3+j+2]=image->getImage()[i*image->getWidth()+(j==0?0:j/3)];
        }
    }

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &imageForOut[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }



    jpeg_finish_compress(&cinfo);
    fclose(outfile);

    jpeg_destroy_compress(&cinfo);

}



ImageWriter::ImageWriter() = default;
