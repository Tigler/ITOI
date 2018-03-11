//
// Created by tigler on 16.02.18.
//

#include "Image.h"
#include <cmath>


Image::Image(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    image.resize(width*height);
}

Image::~Image() = default;

double Image::getValByXY(const int x, const int y) const {
    if (this->width == 0) return 0;
    if (this->height == 0) return 0;
    if (width == 0 || height == 0) return 0;
    if (x >= 0 && x < width && y >= 0 && y < height) return image[x + y * width];
    if (x < 0) {
        if (y < 0) {
            return getValByXY(-x, -y);
        } else if (y < height) {
            return getValByXY(-x, y);
        } else {
            return getValByXY(-x, 2 * height - y - 1);
        }
    } else if (x < width) {
        if (y < 0) {
            return getValByXY(x, -y);
        } else {
            return getValByXY(x, 2 * height - y - 1);
        }
    } else {
        if (y < 0) {
            return getValByXY(2 * width - x - 1, -y);
        } else if (y < height) {
            return getValByXY(2 * width - x - 1, y);
        } else {
            return getValByXY(2 * width - x - 1, 2 * height - y - 1);
        }
    }
}

void Image::setValByXY(const int x, const int y, const double c) {
    if (x < 0 || x > width || y < 0 || y > height) return;
    image[x + y * width] = c;
}


Image Image::convolution(const Kernel &cr) const {
    auto resultImage = Image(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sum = 0;
            for (int j = 0; j < cr.getHeight(); j++) {
                for (int i = 0; i < cr.getWidth(); i++) {
                    int pixelPosX = x + (i - (cr.getWidth() / 2));
                    int pixelPosY = y + (j - (cr.getHeight() / 2));
                    double p = getValByXY(pixelPosX, pixelPosY);
                    double coreItem = cr.getItem(i + j * cr.getWidth());
                    sum += p * coreItem;
                }
            }
            resultImage.setValByXY(x, y, sum);
        }
    }
    return resultImage;
}


Image Image::sobel() {
    auto sobelY = Kernel::sobelY();
    auto sobelX = Kernel::sobelX();

    auto conImageY = convolution(sobelY);
    auto conImageX = convolution(sobelX);

    auto resultImage = Image(width, height);

    for (int i = 0; i < width * height; i++) {
        double x = conImageX.getImageItem(i);
        double y = conImageY.getImageItem(i);
        resultImage.setImageItem(i,sqrt(x * x + y * y));
    }
    return resultImage;
}

unsigned int Image::getWidth() const {
    return width;
}

void Image::setWidth(unsigned int width) {
    Image::width = width;
}

unsigned int Image::getHeight() const {
    return height;
}

void Image::setHeight(unsigned int height) {
    Image::height = height;
}

Image Image::small2() {
    auto result = Image(width / 2, height / 2);
    for (int i = 0; i < width / 2; i++) {
        for (int j = 0; j < height / 2; j++) {
            double sum = 0;
            sum += image[2 * i + 2 * j * width];
            sum += image[2 * i + 1 + 2 * j * width];
            sum += image[2 * i  + (2 * j + 1) * width];
            sum += image[2 * i + 1 + (2 * j + 1) * width];
            sum /= 4;
            result.image[i + j * (width / 2)] = sum;
        }
    }
    return result;
}

void Image::setImageItem(const int i, const double item) {
    image[i] = item;
}

double Image::getImageItem(const int i) {
    return image[i];
}

Image::Image(Image &image) {
    this->width=image.width;
    this->height=image.height;
    this->image=image.image;
}





