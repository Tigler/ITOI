//
// Created by tigler on 13.02.18.
//

#include <cstring>
#include "Application.h"
#include "ImageReaderWriter/ImageReader.h"
#include "ImageReaderWriter/ImageWriter.h"

Application::Application() = default;

void Application::startLab1(const char* path, double sg) {
    auto imageWriter = ImageWriter();
    auto imageReader = ImageReader();
    auto image = imageReader.read(path);

    auto img = image.sobel();
    imageWriter.write("./result/sobel.jpg",img);

    auto img2 = image.convolution(Kernel::gauss(sg));
    imageWriter.write("./result/gauss.jpg",img2);
}

void Application::startLab2(const char* path) {
    auto imageWriter = ImageWriter();
    auto imageReader = ImageReader();
    auto image = imageReader.read(path);

    auto pyramid = Pyramid();
    pyramid.create(image,5,1.6);

    auto count = pyramid.getImages().size();

    int i = 0;
    while (i< count) {
        std::string octave = std::to_string(pyramid.getPyramidInfo()[i].octave);
        char const *oct = octave.c_str();

        std::string scale = std::to_string(pyramid.getPyramidInfo()[i].scale);
        char const *sc = scale.c_str();

        char result[100];
        strcpy(result,"./result/pyramid");
        const char *t = strcat(result,oct);
        strcpy(result,t);
        t = strcat(result,"_");
        strcpy(result,t);
        t = strcat(result,sc);
        strcpy(result,t);
        imageWriter.write(strcat(result,".jpg"),pyramid.getImage(i));
        i++;
    }
}

void Application::startLab3(const char* path) {

    auto imageWriter = ImageWriter();
    auto imageReader = ImageReader();

    auto image = imageReader.read(path);

    auto interestPoints = InterestPoints();

    interestPoints.moravek(image, 100, 2, 15);
    imageWriter.writeWithPoints("./result/morawek.jpg",image,interestPoints.getPoints());

    interestPoints.clearData();

    interestPoints.harris(image, 100, 2, 15000);
    imageWriter.writeWithPoints("./result/harris.jpg",image,interestPoints.getPoints());
}

void Application::startLab4(const char* path) {

}

void Application::startLab5(const char* path) {

}

void Application::startLab6(const char* path) {

}

void Application::startLab7(const char* path) {

}

void Application::startLab8(const char* path) {

}

void Application::startLab9(const char* path) {

}


