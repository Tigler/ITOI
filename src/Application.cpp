//
// Created by tigler on 13.02.18.
//

#include <cstring>
#include <cmath>
#include "Application.h"
#include "ImageReaderWriter/ImageReader.h"
#include "ImageReaderWriter/ImageWriter.h"
#include "DescriptorFinder.h"
#include "Course/SimilarImagesFinder.h"

Application::Application() = default;

void Application::startLab1(const char* path, double sg) {
    auto imageWriter = ImageWriter();
    auto imageReader = ImageReader();
    auto image = imageReader.read(path);

    auto  img = image.convolution(Kernel::sobelX());
    imageWriter.write("./result/sobel.jpg",img);

    auto img2 = image.convolution(Kernel::gauss(sg));
    imageWriter.write("./result/gauss.jpg",img2);
}

void Application::startLab2(const char* path) {
    auto imageWriter = ImageWriter();
    auto imageReader = ImageReader();
    auto image = imageReader.read(path);

    auto pyramid = Pyramid();
    pyramid.create(image,5,4,0.5,1.4);

    auto count = pyramid.getCount();

    int i = 0;
    while (i< count) {
        std::string octave = std::to_string(pyramid.getOctave(i));
        char const *oct = octave.c_str();

        std::string sigma = std::to_string(pyramid.getSigma(i));
        char const *sc = sigma.c_str();

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

    interestPoints.moravek(image, 50, 2, 1000);
    imageWriter.writeWithPoints("./result/morawekk.jpg",image,interestPoints.getPoints());

    interestPoints.clearData();

    interestPoints.harris(image, 20, 2, 1115557500000);//
    imageWriter.writeWithPoints("./result/harris.jpg",image,interestPoints.getPoints());
}

void Application::startLab4(const char* path) {
    auto imageWriter = ImageWriter();
    auto imageReader = ImageReader();

    auto image = imageReader.read(path);

    auto image2 = imageReader.read(path);

    image2 = Image::cutTop(image2,20);

    DescriptorFinder descriptorFinder = DescriptorFinder();

    std::vector<std::pair<Point,Point>> pointPairs = descriptorFinder.getSimilarPoints(image,image2,40);


    imageWriter.writeWithRelationPoints("./result/mergeImages.jpg",image,image2,pointPairs);
}

void Application::startLab5(const char* path) {
    auto imageWriter = ImageWriter();
    auto imageReader = ImageReader();

    auto image = imageReader.read(path);

    auto image2 = imageReader.read(path);

    image2 = Image::rotate(image2);
    image2 = Image::rotate(image2);

    DescriptorFinder descriptorFinder = DescriptorFinder();

    std::vector<std::pair<Point,Point>> pointPairs = descriptorFinder.getSimilarPointsWithRotate(image,image2,40);


    imageWriter.writeWithRelationPoints("./result/mergeImagesWithRotate.jpg",image,image2,pointPairs);
}

void Application::startLab6(const char* path) {

}

void Application::startLab7(const char* path) {

}

void Application::startLab8(const char* path) {

}

void Application::startLab9(const char* path) {

}

void Application::course(const char *pathOriginal,const char *dir,const int N,const int D) {
    SimilarImagesFinder similarImagesFinder = SimilarImagesFinder(N,D);
    similarImagesFinder.calculateDiscOriginal(pathOriginal);
    printf("Original: ");printf(pathOriginal);printf("\n");
    printf("Similars: ");printf("\n");
    similarImagesFinder.findSimilarImages(dir);
}


