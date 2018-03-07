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

        PyramidStruct() = default;

        PyramidStruct(int octave, double sigma, double scale) {
            this->octave = octave;
            this->sigma = sigma;
            this->scale = scale;
        }
    };

private:
    std::vector<Image> images;
    std::vector<PyramidStruct> pyramidInfo;
public:
    Pyramid();

    Image getImage(int i) { return images[i]; }

    int getCount() const{ return images.size(); }

    void create(Image img, const int scales, const double sigmaS = 1);

    const std::vector<Image> &getImages() const;

    const std::vector<PyramidStruct> &getPyramidInfo() const;

    bool isExtremum(int x, int y, int z);

    double getScale(int i) { return pyramidInfo[i].scale; }
};


#endif //ITOI_PYRAMID_H
