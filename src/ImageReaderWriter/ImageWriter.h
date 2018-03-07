//
// Created by tigler on 16.02.18.
//

#ifndef ITOI_IMAGEWRITER_H
#define ITOI_IMAGEWRITER_H

#include "../Image/Image.h"
#include "../InterestPoints/InterestPoints.h"

class ImageWriter {

public:
    ImageWriter();

    void write(const char* path,Image image);

    void writeWithPoints(const char* path,Image image,std::vector<Point> points);
};


#endif //ITOI_IMAGEWRITER_H
