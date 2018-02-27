//
// Created by tigler on 17.02.18.
//

#include "Core.h"
#include <cmath>

Core::Core() = default;

Core::Core(const int width, const int height) {
    this->width = width;
    this->height = height;
    core = std::make_unique<double[]>(width * height);
}

Core::Core(const int width, const int height, const double *core) {
    this->width = width;
    this->height = height;
    this->core = std::make_unique<double[]>(width * height);
    for (int i = 0; i < width * height; i++) {
        this->core[i] = core[i];
    }
}

Core::Core(const Core &k) {
    this->width = k.width;
    this->height = k.height;
    this->core = std::make_unique<double[]>(width * height);
    for (int i = 0; i < width * height; i++) {
        this->core[i] = k.core[i];
    }
}

Core Core::sobelX() {
    double Kx[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    return Core(3, 3, Kx);
}

Core Core::sobelY() {
    double Ky[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    return Core(3, 3, Ky);
}

Core Core::gauss(const double sigma, const int r) {
    Core cr = Core(r, r);
    double doubleSigma = 2 * sigma * sigma;
    double koef = 1 / sqrt(6.28 * sigma);
    double sum = 0;
    int R = r*r;
    for (int i = 0; i < R; i++) {
        cr.core[i] = koef * exp(-(pow(i - (R / 2), 2)) / doubleSigma);
        sum += cr.core[i];
    }
    for (int i = 0; i < R; i++) {
        cr.getCore()[i] /= sum;
    }
    return cr;
}

int Core::getWidth() const {
    return width;
}

int Core::getHeight() const {
    return height;
}

const std::unique_ptr<double[]> &Core::getCore() const {
    return core;
}

double Core::getItem(int index) const {
    return core[index];
}

void Core::setItem(int index, double value) {
    core[index] = value;
}
