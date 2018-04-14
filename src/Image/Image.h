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

    Image(const Image &image);

    Image(unsigned int w, unsigned int h);

    Image convolution(const Kernel &k) const;

    Image sobel();

    void setValByXY(const int x, const int y, const double c);

    Image doubleReduce();

    static Image mergeImages(const Image &imageLeft, const Image &imageRight);

    static Image cutTop(const Image &image, int countRow);

    static Image rotate(const Image &image);

    static bool sizeEq(Image &img1, Image &img2){
        return img1.width == img2.width && img1.height == img1.height;
    }

    Image operator-(const Image &right) const {
        //Q_ASSERT(this->width == right.width && this->height == right.height);
        Image result = Image(this->width, this->height);
        for (int i = 0; i < this->width; i++) {
            for (int j = 0; j < this->height; j++) {
                result.setValByXY(i, j, this->getValBlack(i,j) - right.getValBlack(i,j));
            }
        }
        return result;
    }

    Image separab(const Kernel &k)const;

    unsigned int getWidth() const;

    void setWidth(unsigned int width);

    unsigned int getHeight() const;

    void setHeight(unsigned int height);

    void setImageItem(const int i, const double item);

    double getImageItem(const int i);

    double getValRepeat(int x, int y) const;

    double getValMirror(int x, int y) const;

    double getValWrapp(int x, int y) const;

    double getValBlack(int x, int y) const;

    ~Image();
};


#endif //ITOI_COMMON_H
