//
// Created by tigler on 16.02.18.
//

#include "Image.h"
#include <cmath>
#include "memory"
#include "../Core/Core.h"


Image::Image(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    image = new double[w * h];
}

Image::~Image() {
}

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
            return getValByXY(-x, 2 * height - y - 2);
        }
    } else if (x < width) {
        if (y < 0) {
            return getValByXY(x, -y);
        } else {
            return getValByXY(x, 2 * height - y - 2);
        }
    } else {
        if (y < 0) {
            return getValByXY(2 * width - x - 2, -y);
        } else if (y < height) {
            return getValByXY(2 * width - x - 2, y);
        } else {
            return getValByXY(2 * width - x - 2, 2 * height - y - 2);
        }
    }
}

void Image::setValByXY(const int x, const int y, const double c) {
    if (x < 0 || x > width || y < 0 || y > height) return;
    image[x + y * width] = c;
}


std::unique_ptr<Image> Image::convolution(const Core &cr) const {
    std::unique_ptr<Image> resultImage = std::make_unique<Image>(width, height);
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
            resultImage->setValByXY(x, y, sum);
        }
    }
    return resultImage;
}


std::unique_ptr<Image> Image::sobel() {
    Core sobelY = Core::sobelY();
    Core sobelX = Core::sobelX();

    std::unique_ptr<Image> conImageY = convolution(sobelY);
    std::unique_ptr<Image> conImageX = convolution(sobelX);

    std::unique_ptr<Image> resultImage = std::make_unique<Image>(width, height);

    for (int i = 0; i < width * height; i++) {
        double x = conImageX->getImage()[i];
        double y = conImageY->getImage()[i];
        resultImage->getImage()[i] = sqrt(x * x + y * y);
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

double *Image::getImage() const {
    return image;
}

const Image::PyramidStruct &Image::getPyramidStruct() const {
    return pyramidStruct;
}

void Image::setPyramidStruct(const Image::PyramidStruct &pyramidStruct) {
    Image::pyramidStruct = pyramidStruct;
}

//Image Image::dif(const Image &a, const Image &b, int width, int height) {
//    double scalea = a.getWidth() / (double) width;
//    double scaleb = b.getWidth() / (double) width;
//    std::unique_ptr<Image> resultImage = std::make_unique<Image>(width, height);
//    for (int x = 0; x < width; x++) {
//        for (int y = 0; y < height; y++) {
//            resultImage->getImage()[x + y * width] = a.getValByXY(x * scalea, y * scalea) - b.getValByXY(x * scaleb, y * scaleb);
//        }
//    }
//    return resultImage;
//}

std::unique_ptr<Image> Image::small2() {
    std::unique_ptr<Image> result = std::make_unique<Image>(width / 2, height / 2);
    for (int i = 0; i < width / 2; i++) {
        for (int j = 0; j < height / 2; j++) {
            double sum = 0;
            sum += image[2 * i + 2 * j * width];
            sum += image[2 * i + 2 * j * width];
            sum += image[2 * i + 1 + (2 * j + 1) * width];
            sum += image[2 * i + 1 + (2 * j + 1) * width];
            sum /= 4;

            result->image[i + j * (width / 2)] = sum;
        }
    }
    return result;
}





