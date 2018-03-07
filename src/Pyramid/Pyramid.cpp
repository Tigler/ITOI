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
        double deltaSigma = pow(2, 1.0 / scales);
        Image p1;
        for (int i = 0;i < scales;i++) {
            double sigmaPrev = sigmaCur;
            sigmaCur *= sigma * pow(deltaSigma,i+1);
            double deltaSigma = sqrt(sigmaCur * sigmaCur - sigmaPrev * sigmaPrev);
            Kernel cr1 = Kernel::gauss(sigma,3);
            p1 = p.convolution(cr1);
            pyramidInfo.emplace_back(octave,sigma, sigmaCur);
            images.emplace_back(p1);
        }
        p = p1.small2();
        octave++;
    }
}

bool Pyramid::isExtremum(int x, int y, int z) {
//    bool min = true;
//    bool max = true;
//    double s = 0;
//    double a = DoG(x, y, z);
//
//    for (int x1 = -1;x1 <= 1;x1++) {
//        for (int y1 = -1;y1 <= 1;y1++) {
//            for (int z1 = -1;z1 <= 1;z1++) {
//                if (x1 == 0 && y1 == 0 && z1 == 0) continue;
//                double b = DoG(x + x1, y + y1, z + z1);
//                if (b > a) max = false;
//                if (b < a) min = false;
//                if (b != a) s++;
//            }
//        }
//    }
//    s /= 26;
//    if (s < 0.75) return false;
//    if (max || min) {
//        return true;
//    }
    return false;
}

const std::vector<Image> &Pyramid::getImages() const {
    return images;
}

const std::vector<Pyramid::PyramidStruct> &Pyramid::getPyramidInfo() const {
    return pyramidInfo;
}


Pyramid::Pyramid() = default;
