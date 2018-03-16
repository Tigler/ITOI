//
// Created by tigler on 23.02.18.
//

#include "Pyramid.h"
#include <cmath>

void Pyramid::create(const Image &img, const int countOctaves, const int scales, const double sigmaA, const double sigma0) {
    this->clear();
    Image image = Image(img);
    double interval = pow(2, 1.0/scales);
    double octave = countOctaves;
    while ((image.getWidth() >= 2 && image.getHeight() >= 2) && octave > 0) {
        double sigmaCur = sigma0;
        image = image.separab(Kernel::gauss(sqrt(sigmaCur * sigmaCur - sigmaA * sigmaA)));
        pyramidInfo.emplace_back(image, countOctaves - octave, sigmaCur, 0);
        for (int i = 1; i < scales; i++) {
            double sigmaPrev = sigmaCur;
            sigmaCur = sigma0 * pow(interval, i);
            image = image.separab(Kernel::gauss(sqrt(sigmaCur * sigmaCur - sigmaPrev * sigmaPrev)));
            pyramidInfo.emplace_back(image, countOctaves - octave, sigmaCur, i);
        }
        image = image.doubleReduce();
        octave--;
    }
}

Pyramid::Pyramid() = default;
