//
// Created by tigler on 21.03.18.
//

#ifndef ITOI_SIMILARIMAGESFINDER_H
#define ITOI_SIMILARIMAGESFINDER_H

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"

#include <vector>

class SimilarImagesFinder {
    cv::Mat descriptorsOriginal;
    cv::Mat featuresUnclustered;
    int N;
    int D;
public:

    SimilarImagesFinder(int N,int D){
        this->N=N;
        this->D=D;
    }

    void findSimilarImages(const char *path);
    void calculateDiscOriginal(const char *path);

    void distribute(const char *path);
    void listFiles(const std::string &path);
};


#endif //ITOI_SIMILARIMAGESFINDER_H
