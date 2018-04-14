//
// Created by tigler on 16.03.18.
//

#include <cmath>
#include <algorithm>
#include <numeric>
#include "DescriptorFinder.h"

std::vector<std::pair<Point, Point>>
DescriptorFinder::findSimilar(const std::vector<Point> &interestPointsFirst,
                              const std::vector<Point> &interestPointsSecond,
                              std::vector<std::vector<double>> &descFirst, std::vector<std::vector<double>> &descSecond,
                              int count) {

    double treshold = 0.8;
    std::vector<std::pair<Point, Point>> pairs;
    for (int i = 0; i < descFirst.size(); i++) {
        double min = INT32_MAX;
        double minNeighbor = INT32_MAX;
        int minIdx = 0;
        for (int j = 0; j < descSecond.size(); j++) {
            double sum = 0;
            for (int k = 0; k < descFirst[i].size() && k < descSecond[i].size(); k++) {
                double dist = descFirst[i][k] - descSecond[j][k];
                sum += dist * dist;
            }
            if (min > sum) {
                minNeighbor = min;
                min = sum;
                minIdx = j;
            }
        }
        if (min / minNeighbor < treshold) {
            pairs.emplace_back(interestPointsFirst[i], interestPointsSecond[minIdx]);
        }

    }
    std::vector<std::pair<Point, Point>> pairs1;
    for (int i = 0; i < count && i<pairs.size(); ++i) {
        pairs1.push_back(pairs[i]);
    }
    return pairs1;
}

std::vector<std::vector<double>>
DescriptorFinder::findDescriptors(const std::vector<Point> interestPoints, const Image &image) {
    int baskets = 8;
    int barChars = 16;
    int r = 16;
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
                double anglMain = atan2(valY, valX);
                double anglNeighbor =
                        (anglMain - (((int) (anglMain / part)) * part)) < (part / 2) ? part - anglMain : part +
                                                                                                         anglMain;

                double mainPhi = (int) (anglMain / part) * part + (part / 2);
                double neighborPhi = (int) (anglNeighbor / part) * part + (part / 2);

                double mainValue = abs(sqrt(anglMain * anglMain + mainPhi * mainPhi)) * valGrad;
                double neighborValue = abs(sqrt(anglNeighbor * anglNeighbor + neighborPhi * neighborPhi)) * valGrad;


                int mainIdx = (int) (anglMain / part) * (j + r) / (r / barChars);
                int neighborIdx = (int) (anglNeighbor / part) * (j + r) / (r / barChars);

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


std::vector<std::pair<Point, Point>> DescriptorFinder::getSimilarPoints(Image &first, Image &second, int count) {
    auto interestPoints = InterestPoints();
    interestPoints.moravek(first, count, 2, 1000);
    std::vector<Point> firstIps = interestPoints.getPoints();

    interestPoints.clearData();

    interestPoints.moravek(second, count, 2, 1000);
    std::vector<Point> secondIps = interestPoints.getPoints();
    std::vector<std::vector<double>> firstDescriptors = this->findDescriptors(firstIps, first);
    std::vector<std::vector<double>> secondDescriptors = this->findDescriptors(secondIps, second);
    return this->findSimilar(firstIps, secondIps, firstDescriptors, secondDescriptors, count);
}

std::vector<std::pair<Point, Point>>
DescriptorFinder::getSimilarPointsWithRotate(Image &first, Image &second, int count) {
    auto interestPoints = InterestPoints();
    interestPoints.harris(first, count, 2, 1515557500000);
    std::vector<Point> firstIps = interestPoints.getPoints();
    interestPoints.clearData();
    interestPoints.harris(second, count, 2, 1515557500000);

    std::vector<Point> secondIps = interestPoints.getPoints();
    std::vector<std::vector<double>> firstDescriptors = this->findDescriptorsWithRotate(first,firstIps,16,16,8);
    std::vector<std::vector<double>> secondDescriptors = this->findDescriptorsWithRotate(second,secondIps,16,16,8);

    return this->findSimilar(firstIps, secondIps, firstDescriptors, secondDescriptors, count);
}

std::vector<double> DescriptorFinder::normalize(std::vector<double> descriptor) {
    double sum = 0;
    for (double &item : descriptor) {
        sum += item * item;
    }
    sum = sqrt(sum);
    for (double &item : descriptor) {
        item = item / sum;
    }
    return descriptor;
}

bool DescriptorFinder::isSimilar(std::vector<std::vector<double>> first, std::vector<std::vector<double>> second) {
    return false;
}

double DescriptorFinder::orientation(const Image &imgX, const Image &imgY, const Point &ip, const int r) {
    int baskets = 36;
    double part = 2*M_PI / baskets;
    std::vector<double> basks;
    basks.resize(baskets);
    for (int j = -r;j < r;j++) {
        for (int k = -r;k < r;k++) {
            double valX = imgX.getValBlack(j + ip.x, k + ip.y);
            double valY = imgY.getValBlack(j + ip.x, k + ip.y);
            double anglMain = fmod(2*M_PI+atan2(valY, valX), 2*M_PI);
            double valGrad = sqrt(valX * valX + valY * valY);
            double anglNeighbor = anglMain - fmod(anglMain, part);

            double mainValue = fmod(anglMain, part) / part *valGrad;
            double neighborValue = (1 - (fmod(anglMain, part) / part))*valGrad;

            int mainIdx = anglNeighbor/(2*M_PI)*(baskets)+((double)r/baskets);
            int neighborIdx = mainIdx+1;

            if (mainIdx < 0) mainIdx = baskets - 1;
            if (mainIdx > baskets - 1) mainIdx = 0;
            if (neighborIdx < 0) neighborIdx = baskets - 1;
            if (neighborIdx > baskets - 1) neighborIdx = 0;

            basks[mainIdx] += mainValue;
            basks[neighborIdx] += neighborValue;
        }
    }

    double max = basks[0];
    int idx = 0;
    for (int i = 1; i < baskets; i++) {
        if (basks[i] > max) {
            max = basks[i];
            idx = i;
        }
    }
    return idx * part;
}


std::vector<std::vector<double>> DescriptorFinder::findDescriptorsWithRotate(const Image &img, const std::vector<Point> &pts,
                                                             const int r, const int barChars,
                                                             const int baskets) {
    std::vector<std::vector<double>> descriptors;
    auto imgX = img.convolution(Kernel::sobelX());
    auto imgY = img.convolution(Kernel::sobelY());
    double part = 2 * M_PI / baskets;
    double diam =2*r;
    for (Point ip: pts) {
        std::vector<double> res;
        res.resize(barChars* baskets);
        double orient = orientation(imgX, imgY, ip, r);
        for (int j = -r+1; j < r; j++) {
            for (int k = -r+1; k < r; k++) {
                double valX = imgX.getValBlack(ip.x + j, ip.y + k);
                double valY = imgY.getValBlack(ip.x + j, ip.y + k);
                double anglMain = fmod(atan2(valY, valX) + (2 * M_PI) - orient,2*M_PI);

                double valGrad = sqrt(valX * valX + valY * valY);

                double anglNeighbor,shift;
                if((anglMain - (((int) (anglMain / part)) * part)) < (part / 2)){
                    anglNeighbor=anglMain - part;
                    shift = part;
                }else{
                    anglNeighbor=anglMain + part;
                    shift = -part;
                }
                double mainValue = shift / (part)*valGrad;
                double neighborValue = (part - shift) / (part)*valGrad;

                int orientJ = j*cos(orient) + k*sin(orient);
                int orientK = -j*sin(orient) + k*cos(orient);

                int idx = (int)((r + orientJ)*(barChars/4)/ diam + ((double)baskets/r))*(barChars/4) +
                        (int)((r + orientK)*(barChars/4)/ diam + ((double)baskets/r));

                int mainIdx = idx*baskets + (int)(anglNeighbor/part);
                int neighborIdx = idx*baskets +
                        (((int)(anglNeighbor/part)) + 1<baskets?((int)(anglNeighbor/part)) + 1:0);

                if (mainIdx < 0) mainIdx = 0;
                if (mainIdx > res.size() - 1) mainIdx = res.size() - 1;
                if (neighborIdx < 0) neighborIdx = 0;
                if (neighborIdx > res.size() - 1) neighborIdx = res.size() - 1;

                res[mainIdx] += mainValue;
                res[neighborIdx] += neighborValue;
            }
        }
        res = normalize(res);
        for (double &item : res) {
            item = item > 0.2 ? 0.2 : item;
        }
        res = normalize(res);
        descriptors.push_back(res);
    }

    return descriptors;
}







