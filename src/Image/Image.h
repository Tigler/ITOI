//
// Created by tigler on 16.02.18.
//

#ifndef ITOI_COMMON_H
#define ITOI_COMMON_H

#include "memory"
#include "../Core/Core.h"

class Image {
public:
    struct PyramidStruct {
        int octave;
        double sigma;
        double scale;

        PyramidStruct(){}

        PyramidStruct(int octave, double sigma, double scale) {
            this->octave = octave;
            this->sigma = sigma;
            this->scale = scale;
        }
    };

private:
    unsigned int width;
    unsigned int height;
    double *image;
    PyramidStruct pyramidStruct;
public:
    Image(unsigned int w, unsigned int h);

    std::unique_ptr<Image> convolution(const Core &k) const;

    std::unique_ptr<Image> sobel();

    void setValByXY(const int x, const int y, const double c);

    double getValByXY(const int x, const int y) const;

    Image dif(const Image & a, const Image & b, int width, int height);

    std::unique_ptr<Image> small2();

    unsigned int getWidth() const;

    void setWidth(unsigned int width);

    unsigned int getHeight() const;

    void setHeight(unsigned int height);

    double *getImage() const;

    const PyramidStruct &getPyramidStruct() const;

    void setPyramidStruct(const PyramidStruct &pyramidStruct);

    ~Image();
};


#endif //ITOI_COMMON_H
