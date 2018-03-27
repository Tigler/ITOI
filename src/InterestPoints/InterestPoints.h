//
// Created by tigler on 28.02.18.
//

#ifndef ITOI_INTERESTPOINTS_H
#define ITOI_INTERESTPOINTS_H


#include <vector>
#include "../Pyramid/Pyramid.h"


struct Point {
public:
    int x;
    int y;
    double intensity;

    Point() {
        this->intensity = this->x = this->y = 0;
    }

    Point(int x, int y, double intensity) {
        this->x = x;
        this->y = y;
        this->intensity = intensity;
    };

    Point(int x, int y, int z, double intensity) {
        this->x = x;
        this->y = y;
        this->intensity = intensity;
    };
};

class InterestPoints {

private:
    std::vector<Point> points;
public:
    InterestPoints();

    ~InterestPoints();

    Point getPoint(int index) const { return points[index]; };

    unsigned long getCount() const { return points.size(); }

    void moravek(const Image &img, int count, int r, double threshold = 0);

    void harris(const Image &img, int v, int r, double threshold = 0);

    const std::vector<Point> &getPoints() const;

    std::vector<Point> locMax(const Image &image,const std::vector<Point> &points);

    void clearData();

private:
    void filterANMS(const std::vector<Point> &angles, const int count);
};


#endif //ITOI_INTERESTPOINTS_H
