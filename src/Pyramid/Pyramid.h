//
// Created by tigler on 23.02.18.
//

#ifndef ITOI_PYRAMID_H
#define ITOI_PYRAMID_H


#include "../Image/Image.h"
#include <vector>

class Pyramid {
public:
    struct PyramidStruct {
        int octave;
        double sigma;
        double scale;
        Image image;

        PyramidStruct() = default;

        PyramidStruct(Image &image,int octave, double sigma, double scale) {
            this->octave = octave;
            this->sigma = sigma;
            this->scale = scale;
            this->image = image;
        }
    };

private:
    std::vector<PyramidStruct> pyramidInfo;
public:
    Pyramid();

    void create(const Image &img, const int countOctaves, const int scales, const double sigmaA, const double sigma0);

    double getCount() const{ return pyramidInfo.size(); }

    double getScale(const int i) const{ return pyramidInfo[i].scale; }

    double getSigma(const int i) const{ return pyramidInfo[i].sigma; }

    double getOctave(const int i) const{ return pyramidInfo[i].octave; }

    void clear(){   pyramidInfo.clear();}

    Image getImage(const int i) { return pyramidInfo[i].image; }
};


#endif //ITOI_PYRAMID_H
