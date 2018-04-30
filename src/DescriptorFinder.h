//
// Created by tigler on 16.03.18.
//

#ifndef ITOI_DESCRIPTORFINDER_H
#define ITOI_DESCRIPTORFINDER_H


#include <vector>
#include "InterestPoints/InterestPoints.h"

class DescriptorFinder {
public:
    static std::vector<double> normalize(std::vector<double> descriptor);

    std::vector<std::vector<double>> findDescriptors(const std::vector<Point> interestPoints, const Image &image);

    std::vector<std::pair<Point,Point>>
    findSimilar(const std::vector<Point> &interestPointsFirst, const std::vector<Point> &interestPointsSecond,
                                  std::vector<std::vector<double>> &descFirst, std::vector<std::vector<double>> &descSecond, int count);

    std::vector<double> orientation(const Image &imgX,const Image &imgY,const Point &ip,const int r);

    std::vector<std::pair<Point,Point>> getSimilarPoints(Image &first,Image &second,int count);

    bool isSimilar(std::vector<std::vector<double>> first,std::vector<std::vector<double>> second);

    std::vector<std::pair<Point,Point>> getSimilarPointsWithRotate(Image &first,Image &second,int count);

    std::vector<std::vector<double>> findDescriptorsWithRotate(const Image &img, const std::vector<Point> &pts,
                                                                 const int r, const int barChars, const int baskets);
};


#endif //ITOI_DESCRIPTORFINDER_H
