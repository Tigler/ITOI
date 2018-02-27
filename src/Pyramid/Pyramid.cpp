//
// Created by tigler on 23.02.18.
//

#include "Pyramid.h"
#include "../Image/Image.h"
#include <cmath>

void Pyramid::create(Image *img, const int scales, const double sigma) {
    Core cr = Core::gauss(sigma,3);
    std::unique_ptr<Image> p = img->convolution(cr);
    p = p->convolution(cr);
    images.clear();
    double octave = 1;
    while (p->getWidth() >= 2 && p->getHeight() >= 2) {
        double sigmaCur = sigma;
        double deltaSigma = pow(2, 1.0 / scales);
        std::unique_ptr<Image> p1;
        for (int i = 0;i < scales;i++) {
            double sigmaPrev = sigmaCur;
            sigmaCur *= sigma * pow(deltaSigma,i+1);
            double deltaSigma = sqrt(sigmaCur * sigmaCur - sigmaPrev * sigmaPrev);
            Core cr1 = Core::gauss(sigma,3);
            p1 = p->convolution(cr1);
            p1->setPyramidStruct(Image::PyramidStruct(octave,sigma, sigmaCur));
            images.push_back(*p1);
        }
        p = p1->small2();
        octave++;
    }
}

const std::vector<Image> &Pyramid::getImages() const {
    return images;
}


Pyramid::Pyramid() = default;
