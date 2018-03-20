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

    void writeWithRelationPoints(const char *path, Image imageLeft,Image imageRight, std::vector<std::pair<Point,Point>> pointPairs);
};


#endif //ITOI_IMAGEWRITER_H
