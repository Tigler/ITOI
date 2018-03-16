//
// Created by tigler on 28.02.18.
//

#include <algorithm>
#include <cmath>
#include "InterestPoints.h"

InterestPoints::InterestPoints() = default;

InterestPoints::~InterestPoints() = default;

void InterestPoints::moravek(const Image &img, int count, int r, double threshold) {
    int width = img.getWidth();
    int height = img.getHeight();
    std::vector<Point> angles;

    double minVal, maxVal;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            double skr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (int u = -r; u < r; u++) {
                for (int v = -r; v < r; v++) {
                    double cur = img.getValMirror(i + u, j + v);
                    double d[8];
                    for (int a = 0; a < 3; a++) {
                        for (int b = 0; b < 3; b++) {
                            if (!((a - 1) == 0 && (b - 1) == 0)) {
                                (d[a * 3 + b] = img.getValMirror(i + u + (a - 1), j + v + (b - 1)) - cur);
                            }
                        }
                    }
                    for (int k = 0; k < 9; k++) {
                        if (k != 4)skr[k] += d[k] * d[k];
                    }
                }

            }
            auto min = *std::min_element(&skr[0], &skr[3]);
            auto min1 = *std::min_element(&skr[5], &skr[9]);
            if (min > min1) min = min1;
            if (min < minVal) minVal = min;
            if (min > maxVal) maxVal = min;
            angles.emplace_back(i, j, min);


        }
    }
    double dist = maxVal - minVal;
    std::vector<Point> anglesNorm;
    for(int i = 0; i < angles.size(); i++) {
        angles[i].intensity = (angles[i].intensity - minVal) / dist;
        if (angles[i].intensity > threshold) {
            anglesNorm.push_back(angles[i]);
        }
    }
    filterANMS(anglesNorm, count);
}


void InterestPoints::harris(const Image &image, int count, int r, double threshold) {
    auto imgY = image.convolution(Kernel::sobelY());
    auto imgX = image.convolution(Kernel::sobelX());

    int width = image.getWidth();
    int height = image.getHeight();
    std::vector<Point> angles;
    double A, B, C, A2, C2;
    double minVal, maxVal;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            A = B = C = 0;
            for (int u = i - r; u < i + r; u++) {
                for (int v = j - r; v < j + r; v++) {
                    A += imgX.getValBlack(u, v);
                    B += imgX.getValBlack(u, v) * imgY.getValBlack(u, v);
                    C += imgY.getValBlack(u, v);
                }
            }
            A2 = A * A;
            C2 = C * C;
            double val = abs(
                    ((A2 * C2 - (B * B)) - (0.04 * ((A2 + C2) * (A2 + C2))))); //определитель-[0.04,0.06]*след^2
            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
            angles.emplace_back(i, j, val);
        }
    }

    double dist = maxVal - minVal;
    std::vector<Point> anglesNorm;
    for(int i = 0; i < angles.size(); i++) {
        angles[i].intensity = (angles[i].intensity - minVal) / dist;
        if (angles[i].intensity > threshold) {
            anglesNorm.push_back(angles[i]);
        }
    }
    filterANMS(anglesNorm, count);
}




struct sortPred {
    bool operator()(const std::pair<double,int> &left, const std::pair<double,int> &right) {
        return left.first > right.first;
    }
};

struct sortIntest {
    bool operator()(const Point &left, const Point &right) {
        return left.intensity > right.intensity;
    }
};

void InterestPoints::filterANMS(const std::vector<Point> &angles, const int count) {
    std::vector<Point> angls = angles;
    sort(angls.begin(), angls.end(), sortIntest());
    std::vector<std::pair<double,int>> results;
    for (int i = 1; i < angles.size(); ++i) {
        double minDist = INT32_MAX;
        for (int j = 0; j < i; ++j) {
            double distX = (angls[j].x - angls[i].x);
            double distY = (angls[j].y - angls[i].y);
            double curDist = sqrt(distX * distX + distY * distY);
            minDist = std::min(curDist, minDist);
        }
        results.emplace_back(minDist, i);
    }
    sort(results.begin(), results.end(), sortPred());
    for (int i = 0; i < count; ++i) points.push_back(angls[results[i].second]);
}


const std::vector<Point> &InterestPoints::getPoints() const {
    return points;
}

void InterestPoints::clearData() {
    points.clear();
}

