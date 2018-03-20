//
// Created by tigler on 16.03.18.
//

#include <cmath>
#include <algorithm>
#include "DescriptorFinder.h"


std::vector<std::pair<Point, Point>>
DescriptorFinder::findSimilar(const std::vector<Point> &interestPointsFirst,
                              const std::vector<Point> &interestPointsSecond,
                              std::vector<std::vector<double>> &descFirst, std::vector<std::vector<double>> &descSecond, int count) {

    double treshold =0.8;
    std::vector<std::pair<Point, Point>> pairs;
    for (int i = 0; i < descFirst.size(); i++) {
        double min=INT32_MAX;
        double minNeighbor=INT32_MAX;
        int minIdx=0;
        for (int j = 0; j < descSecond.size(); j++) {
            interestPointsSecond[j];
            double sum = 0;
            for (int k = 0; k < descFirst[i].size() && k < descSecond[i].size(); k++) {
                double dist = descFirst[i][k] - descSecond[j][k];
                sum += dist * dist;
            }
            if(min>sum){
                minNeighbor = min;
                min = sum;
                minIdx=j;
            }
        }
        if(min/minNeighbor<treshold){
            pairs.emplace_back(interestPointsFirst[i], interestPointsSecond[minIdx]);
        }

    }

    return pairs;
}

std::vector<std::vector<double>>
DescriptorFinder::findDescriptors(const std::vector<Point> interestPoints, const Image &image) {
    int baskets = 4;
    int barChars = 4;
    int r = 8;
    Image imgX = image.convolution(Kernel::sobelX());
    Image imgY = image.convolution(Kernel::sobelY());
    std::vector<std::vector<double>> result;
    double part = 2 * M_PI / baskets;
    for (Point ip : interestPoints) {
        std::vector<double> descriptor;
        descriptor.resize(barChars * baskets);
        for (int j = -r; j < r; j++) {
            for (int k = -r; k < r; k++) {
                double valX = imgX.getValBlack(ip.x + j, ip.y + k);
                double valY = imgY.getValBlack(ip.x + j, ip.y + k);

                double valGrad = sqrt(valX * valX + valY * valY);
                double anglMain = 1.0 / tan(valY / valX);
                double anglNeighbor = (anglMain-((anglMain/part)*part))<(part/2)?part-anglMain:part+anglMain;

                double mainPhi = (int)(anglMain / part) * part + (part/2);
                double neighborPhi = (int)(anglNeighbor / part) * part + (part/2);

                double mainValue = (1 - (abs(anglMain - mainPhi) / part)) * valGrad;
                double neighborValue = (1 - (abs(anglNeighbor - neighborPhi) / part)) * valGrad;


                int mainIdx = (int)(anglMain / part)*(j+r)/(r/barChars);
                int neighborIdx = (int)(anglNeighbor / part)*(j+r)/(r/barChars);

                if (mainIdx < 0) mainIdx = 0;
                if (mainIdx > descriptor.size() - 1) mainIdx = descriptor.size() - 1;
                if (neighborIdx < 0) neighborIdx = 0;
                if (neighborIdx > descriptor.size() - 1) neighborIdx = descriptor.size() - 1;

                descriptor[mainIdx] += mainValue;
                descriptor[neighborIdx] += neighborValue;
            }
        }

        descriptor = normalize(descriptor);
        for (double &item : descriptor) {
            item = item > 0.2 ? 0.2 : item;
        }
        descriptor = normalize(descriptor);
        result.push_back(descriptor);
    }
    return result;

}

std::vector<std::pair<Point, Point>> DescriptorFinder::getSimilarPoints(Image &first, Image &second,int count) {
    auto interestPoints = InterestPoints();

    interestPoints.moravek(first, count, 4, 0.1);
    std::vector<Point> firstIps = interestPoints.getPoints();

    interestPoints.clearData();

    interestPoints.moravek(second, count, 4, 0.1);
    std::vector<Point> secondIps = interestPoints.getPoints();

    std::vector<std::vector<double>> firstDescriptors = this->findDescriptors(firstIps, first);

    std::vector<std::vector<double>> secondDescriptors = this->findDescriptors(secondIps, second);

    return this->findSimilar(firstIps, secondIps, firstDescriptors, secondDescriptors, 12);
}

std::vector<double> DescriptorFinder::normalize(std::vector<double> descriptor) {
    double sum = 0;
    for (double &item : descriptor) {
        sum += item;
    }
    for (double &item : descriptor) {
        item = item / sum;
    }
    return descriptor;
}
