//
// Created by tigler on 17.02.18.
//

#ifndef ITOI_CORE_H
#define ITOI_CORE_H

#include <vector>
#include "memory"

class Kernel {
    int width, height;
    std::vector<double> kernel;
public:
    Kernel();

    Kernel(Kernel&&)= default;

    Kernel&operator=(const Kernel&)= default;

    Kernel(Kernel &kernel);

    Kernel(int width, int height);

    Kernel(int width, int height,const std::vector<double> kernel);

    Kernel(const Kernel &kernel);

    static Kernel sobelX();

    static Kernel sobelY();

    static Kernel gauss(double sigma);

    static Kernel gauss(double sigma, int R);

    double getItem(int i) const;

    void setItem(int i, double value);

    void setWidth(const int width){this->width = width;}

    void setHeight(const int height){this->height = height;}

    int getWidth() const;

    int getHeight() const;
};


#endif //ITOI_CORE_H
