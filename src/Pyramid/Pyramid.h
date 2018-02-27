//
// Created by tigler on 23.02.18.
//

#ifndef ITOI_PYRAMID_H
#define ITOI_PYRAMID_H


#include "../Image/Image.h"
#include <vector>

class Pyramid {

    std::vector<Image> images;
public:
    Pyramid();

    Image &getImage(int index) { return images[index]; }

    int getCount() const{ return images.size(); }

    void create(Image *img, const int scales, const double sigmaS = 1);

    const std::vector<Image> &getImages() const;
};


#endif //ITOI_PYRAMID_H
