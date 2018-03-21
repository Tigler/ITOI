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
            angles.emplace_back(i, j, min);
        }
    }
    std::vector<Point> anglesNorm;
    for (int i = 0; i < angles.size(); i++) {
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
    double A, B, C;
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
                angles.emplace_back(i, j, val);
        }
    }

    std::vector<Point> anglesNorm;
    for (int i = 0; i < angles.size(); i++) {
        if (angles[i].intensity > threshold) {
            anglesNorm.push_back(angles[i]);
        }
    }
    anglesNorm = locMax(image,imgX,imgY,anglesNorm);
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

bool isFirstMax(int a, int b, int c){
    if(a>b && a>c){
        return true;
    }
    return 0;
}

std::vector<Point> InterestPoints::locMax(Image image,Image imageX,Image imageY,const std::vector<Point> points){
    std::vector<Point> result;
    for (auto point : points) {
        double valX = imageX.getValBlack(point.x, point.y);
        double valY = imageY.getValBlack(point.x, point.y);
        int angl = 180.0*atan2(valY, valX)/M_PI;

        if(angl>=0&&angl<45) {
            if (!isFirstMax(image.getValBlack(point.x, point.y), image.getValBlack(point.x, point.y + 1),
                            image.getValBlack(point.x, point.y - 1))) {
                result.push_back(point);
            }
        }
        if(angl>=45&&angl<90) {
            if (!isFirstMax(image.getValBlack(point.x, point.y), image.getValBlack(point.x + 1, point.y + 1),
                            image.getValBlack(point.x - 1, point.y - 1))) {
                result.push_back(point);
            }
        }
        if(angl>=90&&angl<135) {
            if (!isFirstMax(image.getValBlack(point.x, point.y), image.getValBlack(point.x + 1, point.y),
                            image.getValBlack(point.x - 1, point.y))) {
                result.push_back(point);
            }
        }
        if(angl>=135) {
            if (!isFirstMax(image.getValBlack(point.x, point.y), image.getValBlack(point.x - 1, point.y + 1),
                            image.getValBlack(point.x + 1, point.y - 1))) {
                result.push_back(point);
            }
        }
    }
    return result;
}


