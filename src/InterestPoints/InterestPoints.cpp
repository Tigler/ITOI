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
    for (int i = r; i < width - r; i++) {
        for (int j = r; j < height - r; j++) {
            double skr[] = {0, 0, 0, 0, 0, 0, 0, 0};
            for (int u = -r; u < r; u++) {
                for (int v = -r; v < r; v++) {
                    double d[8];
                    double cur = img.getValByXY(i + u, j + v);
                    d[0] = cur - img.getValByXY(i + u, j + v - 1);
                    d[1] = cur - img.getValByXY(i + u + 1, j + v);
                    d[2] = cur - img.getValByXY(i + u, j + v + 1);
                    d[3] = cur - img.getValByXY(i + u - 1, j + v);
                    d[4] = cur - img.getValByXY(i + u + 1, j + v - 1);
                    d[5] = cur - img.getValByXY(i + u + 1, j + v + 1);
                    d[6] = cur - img.getValByXY(i + u - 1, j + v + 1);
                    d[7] = cur - img.getValByXY(i + u - 1, j + v - 1);
                    for (int k = 0; k < 8; k++) {
                        skr[k] += d[k] * d[k];
                    }
                }
            }
            auto min = std::min(skr[0], skr[8]);
            if(min>threshold){
                angles.emplace_back(i, j,min);
            }
        }
    }

    std::sort(angles.begin(), angles.end(), [](auto &first, auto &second) {
        return first.intensity > second.intensity;
    });
    filterANMS(angles, count);
}


void InterestPoints::harris(const Image &image, int count, int r, double threshold) {

    auto kY = Kernel::sobelY();
    auto kX = Kernel::sobelX();
    auto imgY = image.convolution(kY);
    auto imgX = image.convolution(kX);

    int width = image.getWidth();
    int height = image.getHeight();
    std::vector<double> intensities;
    std::vector<Point> angles;
    intensities.resize(width * height);
    for (int i = r; i < width - r; i++) {
        for (int j = r; j < height - r; j++) {
            double lambdaMin = Image::ownValue(imgX, imgY, i, j, r, r);
            if (lambdaMin > threshold) {
                angles.emplace_back(i, j, lambdaMin);
            }
        }
    }

    std::sort(angles.begin(), angles.end(), [](auto &first, auto &second) {
        return first.intensity > second.intensity;
    });
    filterANMS(angles, count);
}

void InterestPoints::filterANMS(const std::vector<Point> &angles, const int count) {
    auto anglesCount = angles.size();
    std::vector<int> radiuses;
    radiuses.resize(anglesCount);
    int maxRadius = 0;
    for (int i = 0; i < anglesCount; i++) {
        int minRadius = INT32_MAX;
        for (int j = 0; j < i; j++) {
            int dX = angles[j].x - angles[i].x;
            int dY = angles[j].y - angles[i].y;
            int radius = (int) sqrt(dX * dX + dY * dY);
            if (radius < minRadius) {
                minRadius = radius;
            }
        }
        radiuses[i] = minRadius;
        if (i > 0 && minRadius > maxRadius) maxRadius = minRadius;
    }
    radiuses[0] = maxRadius + 1;

    std::vector<bool> suitPointsFlags;
    suitPointsFlags.resize(anglesCount);
    for (int i = 0; i < anglesCount; i++) {
        suitPointsFlags[i] = false;
    }

    std::vector<Point> suitPoints;
    suitPoints.resize(count);

    int radius = maxRadius;
    suitPoints[0] = angles[0];
    suitPointsFlags[0] = true;

    int suitCount = 1;
    while (suitCount < count) {
        for (int i = 0; i < anglesCount; i++) {
            if (suitPointsFlags[i]) continue;
            if (radiuses[i] >= radius) {
                suitPoints[suitCount++] = angles[i];
                suitPointsFlags[i] = true;
            }
            if (suitCount >= count) break;
        }
        radius--;
    }

    for (int i = 0; i < count; i++) {
        points.push_back(suitPoints[i]);
    }
}

const std::vector<Point> &InterestPoints::getPoints() const {
    return points;
}

void InterestPoints::clearData() {
    points.clear();
}

