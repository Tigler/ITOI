//
// Created by tigler on 16.02.18.
//

#include "Image.h"
#include <cmath>


Image::Image(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    image.resize(width * height);
}

Image::~Image() = default;

double Image::getValRepeat(const int x, const int y) const {
    int x0 = x, y0 = y;
    if (x < 0) x0 = 0;
    if (x >= width) x0 = width - 1;
    if (y < 0) y0 = 0;
    if (y >= height) y0 = height - 1;
    return image[x0 + y0 * width];
}

double Image::getValMirror(const int x, const int y) const {
    int x0 = x, y0 = y;
    if (x < 0) x0 = -x;
    if (x >= width) x0 = 2 * width - x - 1;
    if (y < 0) y0 = -y;
    if (y >= height) y0 = 2 * height - y - 1;
    return image[x0 + y0 * width];
}

double Image::getValWrapp(const int x, const int y) const {
    int x0 = x, y0 = y;
    if (x < 0) x0 = x + width;
    if (y < 0) y0 = y + height;
    if (x >= width) x0 = (x - width) + 1;
    if (y >= height) y0 = (y - height) + 1;
    return image[x0 + y0 * width];
}

double Image::getValBlack(const int x, const int y) const {
    int x0 = x, y0 = y;
    if (x < 0 || x >= width) x0 = 0;
    if (y < 0 || y >= height) y0 = 0;
    return image[x0 + y0 * width];
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
                    int posX = x + (i - (cr.getWidth() / 2));
                    int posY = y + (j - (cr.getHeight() / 2));
                    sum += getValBlack(posX, posY) * cr.getItem(i + j * cr.getWidth());
                }
            }
            resultImage.setValByXY(x, y, sum);
        }
    }
    return resultImage;
}


Image Image::sobel() {
    auto conImageY = this->convolution(Kernel::sobelY());
    auto conImageX = this->convolution(Kernel::sobelX());
    auto resultImage = Image(width, height);
    for (int i = 0; i < width * height; i++) {
        double x = conImageX.getImageItem(i);
        double y = conImageY.getImageItem(i);
        resultImage.setImageItem(i, sqrt(x * x + y * y));
    }
    return resultImage;
}

Image Image::mergeImages(const Image &imageLeft, const Image &imageRight) {
    int height = imageLeft.getHeight() >= imageRight.getHeight() ? imageRight.getHeight() : imageLeft.getHeight();
    int commonWidth = imageLeft.getWidth() + imageRight.getWidth();

    auto result = Image(commonWidth, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < commonWidth; j++) {
            if (j < imageLeft.getWidth()) {
                result.setValByXY(j, i, imageLeft.getValBlack(j, i));
            } else {
                result.setValByXY(j, i, imageRight.getValBlack(j - imageLeft.getWidth(), i));
            }
        }
    }

    return result;
}

Image Image::cutTop(const Image &image, int countRow) {
    Image result = Image(image.getWidth(), (image.getHeight() - countRow));
    for (int i = 0; i < image.getHeight() - countRow; i++) {
        for (int j = 0; j < image.getWidth(); j++) {
            result.setValByXY(j, i, image.getValBlack(j, i + countRow));
        }
    }
    return result;
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

Image Image::doubleReduce() {
    auto result = Image(width / 2, height / 2);
    for (int i = 0; i < width / 2; i++) {
        for (int j = 0; j < height / 2; j++) {
            double sum = 0;
            sum += image[2 * i + 2 * j * width];
            sum += image[(2 * i + 1) + 2 * j * width];
            sum += image[2 * i + (2 * j + 1) * width];
            sum += image[(2 * i + 1) + (2 * j + 1) * width];
            result.image[i + j * (width / 2)] = sum / 4;
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

Image::Image(const Image &image) {
    this->width = image.width;
    this->height = image.height;
    this->image = image.image;
}

Image Image::separab(const Kernel &k) const {
    Kernel kernel = Kernel(k);
    Image result = this->convolution(kernel);
    int w = kernel.getWidth();
    kernel.setWidth(kernel.getHeight());
    kernel.setHeight(w);
    result = result.convolution(kernel);
    return result;
}

Image Image::rotate(const Image &image) {
    auto result = Image(image.getHeight(), image.getWidth());
    int wOr = image.getWidth(), hOr = image.getHeight();
//    for (int x = 0; x < wOr; x++) {
//        for (int y = 0; y < hOr; y++) {
////            result.setValByXY(wOr - x - 1,(hOr - y)/2,image.getValBlack(x,y));
////            result.setValByXY(x,(hOr + y)/2,image.getValBlack(x,y));
//
//            result.setValByXY(x, hOr - y, image.getValBlack(x, y));
//        }
//    }
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            result.setValByXY(image.getHeight()-1-j, i, image.getValBlack(i,j));
        }
    }
    return result;
}





