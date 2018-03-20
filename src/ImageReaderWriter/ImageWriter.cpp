//
// Created by tigler on 16.02.18.
//

#include <cstdio>
#include "ImageWriter.h"

extern "C" {
#include "jpeglib.h"
};

void ImageWriter::write(const char *path, Image image) {
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
    cinfo.image_width = image.getWidth();
    cinfo.image_height = image.getHeight();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = image.getWidth() * 3;

    unsigned char *imageForOut = new unsigned char[image.getHeight() * image.getWidth() * 3];

    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth() * 3; j += 3) {
            imageForOut[i * image.getWidth() * 3 + j] = image.getImageItem(i * image.getWidth() + (j == 0 ? 0 : j / 3));
            imageForOut[i * image.getWidth() * 3 + j + 1] = image.getImageItem(
                    i * image.getWidth() + (j == 0 ? 0 : j / 3));
            imageForOut[i * image.getWidth() * 3 + j + 2] = image.getImageItem(
                    i * image.getWidth() + (j == 0 ? 0 : j / 3));
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

void ImageWriter::writeWithPoints(const char *path, Image image, std::vector<Point> points) {
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
    cinfo.image_width = image.getWidth();
    cinfo.image_height = image.getHeight();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = image.getWidth() * 3;

    unsigned char *imageForOut = new unsigned char[image.getHeight() * image.getWidth() * 3];

    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth() * 3; j += 3) {
            imageForOut[i * image.getWidth() * 3 + j] = image.getImageItem(i * image.getWidth() + (j == 0 ? 0 : j / 3));
            imageForOut[i * image.getWidth() * 3 + j + 1] = image.getImageItem(
                    i * image.getWidth() + (j == 0 ? 0 : j / 3));
            imageForOut[i * image.getWidth() * 3 + j + 2] = image.getImageItem(
                    i * image.getWidth() + (j == 0 ? 0 : j / 3));
        }
    }

    for (int i = 0; i < points.size(); i++) {
        imageForOut[points[i].y * image.getWidth() * 3 + points[i].x * 3] = 255;
        imageForOut[points[i].y * image.getWidth() * 3 + points[i].x * 3 + 1] = 0;
        imageForOut[points[i].y * image.getWidth() * 3 + points[i].x * 3 + 2] = 0;
        for (int t = (-(int)image.getWidth() / 300); t <(int)image.getWidth() / 300; t++) {
            for (int j = -((int)image.getWidth() / 100); j < (int)image.getWidth() / 100; j += 3) {
                imageForOut[(points[i].y+t) * image.getWidth() * 3 + points[i].x * 3 + j] = 255;
                imageForOut[(points[i].y+t) * image.getWidth() * 3 + points[i].x * 3 + j + 1] = 0;
                imageForOut[(points[i].y+t) * image.getWidth() * 3 + points[i].x * 3 + j + 2] = 0;
            }
        }
    }

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &imageForOut[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    //fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}


void ImageWriter::writeWithRelationPoints(const char *path, Image imageLeft,Image imageRight, std::vector<std::pair<Point,Point>> pointPairs) {

    auto image = Image::mergeImages(imageLeft,imageRight);
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
    cinfo.image_width = image.getWidth();
    cinfo.image_height = image.getHeight();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = image.getWidth() * 3;

    unsigned char *imageForOut = new unsigned char[image.getHeight() * image.getWidth() * 3];

    for (int i = 0; i < image.getHeight(); i++) {
        for (int j = 0; j < image.getWidth() * 3; j += 3) {
            imageForOut[i * image.getWidth() * 3 + j] = image.getImageItem(i * image.getWidth() + (j == 0 ? 0 : j / 3));
            imageForOut[i * image.getWidth() * 3 + j + 1] = image.getImageItem(
                    i * image.getWidth() + (j == 0 ? 0 : j / 3));
            imageForOut[i * image.getWidth() * 3 + j + 2] = image.getImageItem(
                    i * image.getWidth() + (j == 0 ? 0 : j / 3));
        }
    }

    for (int i = 0; i < pointPairs.size(); i++) {
        imageForOut[pointPairs[i].first.y * image.getWidth() * 3 + pointPairs[i].first.x * 3] = 255;
        imageForOut[pointPairs[i].first.y * image.getWidth() * 3 + pointPairs[i].first.x * 3 + 1] = 0;
        imageForOut[pointPairs[i].first.y * image.getWidth() * 3 + pointPairs[i].first.x * 3 + 2] = 0;
        for (int t = (-(int)image.getWidth() / 300); t <(int)image.getWidth() / 300; t++) {
            for (int j = -((int)image.getWidth() / 100); j < (int)image.getWidth() / 100; j += 3) {
                imageForOut[(pointPairs[i].first.y+t) * image.getWidth() * 3 + pointPairs[i].first.x * 3 + j] = 255;
                imageForOut[(pointPairs[i].first.y+t) * image.getWidth() * 3 + pointPairs[i].first.x * 3 + j + 1] = 0;
                imageForOut[(pointPairs[i].first.y+t) * image.getWidth() * 3 + pointPairs[i].first.x * 3 + j + 2] = 0;
            }
        }
    }


    for (int i = 0; i < pointPairs.size(); i++) {
        imageForOut[pointPairs[i].second.y * image.getWidth() * 3 + (pointPairs[i].second.x+imageLeft.getWidth()) * 3] = 255;
        imageForOut[pointPairs[i].second.y * image.getWidth() * 3 + (pointPairs[i].second.x+imageLeft.getWidth()) * 3 + 1] = 0;
        imageForOut[pointPairs[i].second.y * image.getWidth() * 3 + (pointPairs[i].second.x+imageLeft.getWidth()) * 3 + 2] = 0;
        for (int t = (-(int)image.getWidth() / 300); t <(int)image.getWidth() / 300; t++) {
            for (int j = -((int)image.getWidth() / 100); j < (int)image.getWidth() / 100; j += 3) {
                imageForOut[(pointPairs[i].second.y+t) * image.getWidth() * 3 + (pointPairs[i].second.x+imageLeft.getWidth()) * 3 + j] = 255;
                imageForOut[(pointPairs[i].second.y+t) * image.getWidth() * 3 + (pointPairs[i].second.x+imageLeft.getWidth()) * 3 + j + 1] = 0;
                imageForOut[(pointPairs[i].second.y+t) * image.getWidth() * 3 + (pointPairs[i].second.x+imageLeft.getWidth()) * 3 + j + 2] = 0;
            }
        }
    }


    for(int i=0;i<pointPairs.size();i++){
        for(int j=pointPairs[i].first.x;j<pointPairs[i].second.x+imageLeft.getWidth();j++) {
            int y = ((int)j*((int)pointPairs[i].second.y-(int)pointPairs[i].first.y) - (int)pointPairs[i].first.x*((int)pointPairs[i].second.y-(int)pointPairs[i].first.y) +
                     (int)pointPairs[i].first.y*(((int)pointPairs[i].second.x+(int)imageLeft.getWidth())-(int)pointPairs[i].first.x))/(((int)pointPairs[i].second.x+(int)imageLeft.getWidth())-(int)pointPairs[i].first.x);
            if(y<0)y=y*(-1);
            if(y<image.getHeight()) {
                imageForOut[y * image.getWidth() * 3 + j * 3] = 255;
                imageForOut[y * image.getWidth() * 3 + j * 3 + 1] = 0;
                imageForOut[y * image.getWidth() * 3 + j * 3 + 2] = 0;
            }
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
