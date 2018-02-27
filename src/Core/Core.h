//
// Created by tigler on 17.02.18.
//

#ifndef ITOI_CORE_H
#define ITOI_CORE_H

#include "memory"

class Core {
    int width, height;
    std::unique_ptr<double[]> core;
public:
    Core();

    Core(int width, int height);

    Core(int width, int height,const double* core);

    Core(const Core &core);

    static Core sobelX();

    static Core sobelY();

    static Core gauss(double sigma);

    static Core gauss(double sigma, int R);

    double getItem(int index) const;

    void setItem(int index, double value);

    int getWidth() const;

    int getHeight() const;

    const std::unique_ptr<double[]> &getCore() const;
};


#endif //ITOI_CORE_H
