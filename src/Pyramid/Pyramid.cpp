//
// Created by tigler on 23.02.18.
//

#include "Pyramid.h"
#include <cmath>

void Pyramid::create(Image img, const int scales, const double sigma) {
    Kernel cr = Kernel::gauss(sigma);
    Image p = img.convolution(cr);
    p = p.convolution(cr);
    images.clear();
    double octave = 1;
    while (p.getWidth() >= 2 && p.getHeight() >= 2) {
        double sigmaCur = sigma;
        double interval = pow(2, 1.0 / scales);
        Image p1;
        for (int i = 0;i < scales;i++) {
            double sigmaPrev = sigmaCur;
            sigmaCur *= sigma * pow(interval,i+1);
            double deltaSigma = sqrt(sigmaCur * sigmaCur - sigmaPrev * sigmaPrev);
            Kernel cr1 = Kernel::gauss(deltaSigma);
            p1 = p.convolution(cr1);
            pyramidInfo.emplace_back(octave,sigma, sigmaCur);
            images.emplace_back(p1);
        }
        p = p1.small2();
        octave++;
    }
}

const std::vector<Image> &Pyramid::getImages() const {
    return images;
}

const std::vector<Pyramid::PyramidStruct> &Pyramid::getPyramidInfo() const {
    return pyramidInfo;
}


Pyramid::Pyramid() = default;
