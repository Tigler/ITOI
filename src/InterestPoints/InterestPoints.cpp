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
                            (d[a * 3 + b] = img.getValMirror(i + u + (a - 1), j + v + (b - 1)) - cur);
                        }
                    }
                    for (int k = 0; k < 9; k++) {
                        skr[k] += d[k] * d[k];
                    }
                }

            }
            skr[4] = std::numeric_limits<double>::max();
            auto min = *std::min_element(std::begin(skr), std::end(skr));
            if (min < minVal) minVal = min;
            if (min > maxVal) maxVal = min;
            angles.emplace_back(i, j, min);
        }
    }
    double dist = maxVal - minVal;
    std::vector<Point> anglesNorm;
    for (int i = 0; i < angles.size(); i++) {
        angles[i].intensity = (angles[i].intensity - minVal) / dist;
        if (angles[i].intensity > threshold) {
            anglesNorm.push_back(angles[i]);
        }
    }
    anglesNorm = locMax(anglesNorm);
    filterANMS(anglesNorm, count);
}


void InterestPoints::harris(const Image &image, int count, int r, double threshold) {
    auto imgY = image.convolution(Kernel::sobelY());
    auto imgX = image.convolution(Kernel::sobelX());

    int width = image.getWidth();
    int height = image.getHeight();
    std::vector<Point> angles;
    double A, B, C;
    double minVal, maxVal;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            A = B = C = 0;
            for (int u = i - r; u < i + r; u++) {
                for (int v = j - r; v < j + r; v++) {
                    A += imgX.getValBlack(u, v)*imgX.getValBlack(u, v);
                    B += imgX.getValBlack(u, v) * imgY.getValBlack(u, v);
                    C += imgY.getValBlack(u, v)*imgY.getValBlack(u, v);
                }
            }
            double val =
                    ((A * C - (B * B)) - (0.04 * ((A + C) * (A + C)))); //определитель-[0.04,0.06]*след^2
            if(val>0) {
                if (val < minVal) minVal = val;
                if (val > maxVal) maxVal = val;
                angles.emplace_back(i, j, val);
            }
        }
    }

    double dist = maxVal - minVal;
    std::vector<Point> anglesNorm;
    for (int i = 0; i < angles.size(); i++) {
        angles[i].intensity = (angles[i].intensity - minVal) / dist;
        if (angles[i].intensity > threshold) {
            anglesNorm.push_back(angles[i]);
        }
    }
    anglesNorm = locMax(anglesNorm);
    filterANMS(anglesNorm, count);
}

void InterestPoints::filterANMS(const std::vector<Point> &angles, const int count) {
    std::vector<std::pair<double, int>> results;
    for (int i = 0; i < angles.size(); ++i) {
        double minPoint = INT32_MAX;
        for (int j = 0; j < angles.size(); ++j) {
            if ((angles[i].x != angles[j].x && angles[i].y != angles[j].y) &&
                angles[i].intensity < 0.9 * angles[j].intensity) {
                double distX = (angles[j].x - angles[i].x);
                double distY = (angles[j].y - angles[i].y);
                double curDist = sqrt(distX * distX + distY * distY);
                if (curDist < minPoint){
                    minPoint = curDist;
                }

            }
        }
        results.emplace_back(minPoint, i);
    }
    sort(results.begin(), results.end(), [](auto &first, auto &second) { return first.first > second.first; });
    for (int i = 0; i < count && i<results.size(); ++i) {
        points.push_back(angles[results[i].second]);
    }
}


const std::vector<Point> &InterestPoints::getPoints() const {
    return points;
}

void InterestPoints::clearData() {
    points.clear();
}

std::vector<Point> InterestPoints::locMax(const std::vector<Point> points){
    int r = 3;
    std::vector<Point> result;
    for (auto point : points) {
        bool f = false;
        for (auto &res : result) {
            double distX = point.x - res.x;
            double distY = point.y - res.y;
            double curDist = sqrt(distX * distX + distY * distY);
            if(point.intensity > res.intensity && curDist < r){
                res = point;
                f = true;
                break;
            }
        }
        if(!f){
            result.push_back(point);
        }
    }
    return result;
}


