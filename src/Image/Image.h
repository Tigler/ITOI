//
// Created by tigler on 16.02.18.
//

#ifndef ITOI_COMMON_H
#define ITOI_COMMON_H

#include <vector>
#include "memory"
#include "../Kernel/Kernel.h"

class Image {
    unsigned int width;
    unsigned int height;
    std::vector<double> image;
public:
    Image() = default;

    Image &operator=(const Image&)= default;

    Image(Image &&) = default;

    Image(Image &image);

    Image(unsigned int w, unsigned int h);

    Image convolution(const Kernel &k) const;

    Image sobel();

    void setValByXY(const int x, const int y, const double c);

    double getValByXY(const int x, const int y) const;

    Image small2();

    unsigned int getWidth() const;

    void setWidth(unsigned int width);

    unsigned int getHeight() const;

    void setHeight(unsigned int height);

    void setImageItem(const int i, const double item);

    double getImageItem(const int i);

    static double ownValue(const Image &imgX, const Image &imgY, int x0, int y0, int width, int height);

    ~Image();
};


#endif //ITOI_COMMON_H
