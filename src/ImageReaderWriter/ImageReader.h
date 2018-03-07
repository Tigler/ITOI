//
// Created by tigler on 13.02.18.
//

#ifndef ITOI_IMAGEREADER_H
#define ITOI_IMAGEREADER_H


#include "../Image/Image.h"

class ImageReader {
public:
    ImageReader();

    Image read(const char *path);
};


#endif //ITOI_IMAGEREADER_H
