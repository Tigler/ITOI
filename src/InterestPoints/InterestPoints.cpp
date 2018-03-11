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
                    d[0] = img.getValByXY(i + u, j + v - 1) - cur;
                    d[1] = img.getValByXY(i + u + 1, j + v) - cur;
                    d[2] = img.getValByXY(i + u, j + v + 1) - cur;
                    d[3] = img.getValByXY(i + u - 1, j + v) - cur;
                    d[4] = img.getValByXY(i + u + 1, j + v - 1) - cur;
                    d[5] = img.getValByXY(i + u + 1, j + v + 1) - cur;
                    d[6] = img.getValByXY(i + u - 1, j + v + 1) - cur;
                    d[7] = img.getValByXY(i + u - 1, j + v - 1) - cur;
                    for (int k = 0; k < 8; k++) {
                        skr[k] += d[k] * d[k];
                    }
                }
            }
            auto min = std::min_element(&skr[0], &skr[8]);
            if (*min > threshold) {
                angles.emplace_back(i, j, *min);
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
    double A, B, C = 0;
    double A2, C2 = 0;
    for (int i = r; i < width - r; i++) {
        for (int j = r; j < height - r; j++) {
            A = imgX.getValByXY(i, j);
            A2 = A * A;
            B = imgX.getValByXY(i, j) * imgY.getValByXY(i, j);
            C = imgY.getValByXY(i, j);
            C2 = C * C;
            double val = abs(
                    ((A2 * C2 - (B * B)) - (0.04 * ((A2 + C2) * (A2 + C2))))); //определитель-[0.04,0.06]*след^2
            if (val > threshold) {
                angles.emplace_back(i, j, val);
            }
        }
    }

    std::sort(angles.begin(), angles.end(), [](auto &first, auto &second) {
        return first.intensity > second.intensity;
    });
    filterANMS(angles, count);
}

struct sortPred {
    bool operator()(const std::pair<float,int> &left, const std::pair<float,int> &right) {
        return left.first > right.first;
    }
};


void InterestPoints::filterANMS(const std::vector<Point> &angles, const int count) {

    std::vector<std::pair<float,int>> results;
    for (unsigned int i = 1;i<angles.size();++i){
        float minDist = INT32_MAX;
        for (unsigned int j=0;j<i;++j){
            float exp1 = (angles[j].x-angles[i].x);
            float exp2 = (angles[j].y-angles[i].y);
            float curDist = sqrt(exp1*exp1+exp2*exp2);
            minDist = std::min(curDist,minDist);
        }
        results.emplace_back(minDist,i);
    }
    sort(results.begin(),results.end(),sortPred());
    for (int i=0;i<count;++i) points.push_back(angles[results[i].second]);
}


const std::vector<Point> &InterestPoints::getPoints() const {
    return points;
}

void InterestPoints::clearData() {
    points.clear();
}

