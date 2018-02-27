//
// Created by tigler on 16.02.18.
//

#ifndef ITOI_IMAGEWRITER_H
#define ITOI_IMAGEWRITER_H

#include "../Image/Image.h"

class ImageWriter {

public:
    ImageWriter();

    void write(const char* path,Image* image);
};


#endif //ITOI_IMAGEWRITER_H
