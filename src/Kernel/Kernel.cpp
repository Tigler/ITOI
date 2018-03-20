//
// Created by tigler on 17.02.18.
//

#include "Kernel.h"
#include <cmath>

Kernel::Kernel() = default;

Kernel::Kernel(const int width, const int height) {
    this->width = width;
    this->height = height;
    kernel.resize(width * height);
}

Kernel::Kernel(const int width, const int height, const std::vector<double> kernel) {
    this->width = width;
    this->height = height;
    this->kernel = kernel;
}

Kernel::Kernel(const Kernel &kernel) {
    this->width = kernel.width;
    this->height = kernel.height;
    this->kernel = kernel.kernel;
}

Kernel Kernel::sobelX() {
    std::vector<double> kX = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    return Kernel(3, 3, kX);
}

Kernel Kernel::sobelY() {
    std::vector<double> kY = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    return Kernel(3, 3, kY);
}

Kernel Kernel::gauss(const double sigma, const int r) {
    auto gauss = Kernel(r, r);
    double sum = 0;
    int R = r * r;
    for (int i = 0; i < R; i++) {
        gauss.kernel[i] = (1 / (sqrt(2 * M_PI) * sigma)) * exp(-(pow(i - (R / 2), 2)) / (2 * sigma * sigma));
        sum += gauss.kernel[i];
    }
    for (int i = 0; i < R; i++) {
        gauss.setItem(i, gauss.getItem(i) / sum);
    }
    return gauss;
}

int Kernel::getWidth() const {
    return width;
}

int Kernel::getHeight() const {
    return height;
}

double Kernel::getItem(int i) const {
    return kernel[i];
}

void Kernel::setItem(int i, double value) {
    kernel[i] = value;
}

Kernel Kernel::gauss(double sigma) {
    return gauss(sigma, 6 * sigma);
}

Kernel::Kernel(Kernel &kernel) {
    this->width = kernel.width;
    this->height = kernel.width;
    this->kernel = kernel.kernel;
}