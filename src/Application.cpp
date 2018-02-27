//
// Created by tigler on 13.02.18.
//

#include <cstring>
#include <stdlib.h>
#include "Application.h"
#include "ImageReaderWriter/ImageReader.h"
#include "ImageReaderWriter/ImageWriter.h"
#include "Pyramid/Pyramid.h"

Application::Application() = default;

void Application::startLab1(const char* path, double sg) {
    ImageWriter *imageWriter = new ImageWriter();
    ImageReader *imageReader = new ImageReader();
    Image *image = imageReader->read(path);

    std::unique_ptr<Image> img = image->sobel();
    imageWriter->write("./result/sobel.jpg",img.get());

    std::unique_ptr<Image> img2 = image->convolution(Core::gauss(sg,3));
    imageWriter->write("./result/gauss.jpg",img2.get());

    free(imageReader);
    free(imageWriter);
}

void Application::startLab2(const char* path) {
    ImageWriter *imageWriter = new ImageWriter();
    ImageReader *imageReader = new ImageReader();
    Image *image = imageReader->read(path);

    std::unique_ptr<Pyramid> p = std::make_unique<Pyramid>();
    p->create(image,5,1.6);

    unsigned long count = p->getImages().size();

    int i = 0;
    char string[6] = "";
    while (i< p->getImages().size()) {
        std::string octave = std::to_string(p->getImage(i).getPyramidStruct().octave);
        char const *oct = octave.c_str();

        std::string scale = std::to_string(p->getImage(i).getPyramidStruct().scale);
        char const *sc = scale.c_str();

        char result[100];
        strcpy(result,"./result/pyramid");
        const char *t = strcat(result,oct);
        strcpy(result,t);
        t = strcat(result,"_");
        strcpy(result,t);
        t = strcat(result,sc);
        strcpy(result,t);
        imageWriter->write(strcat(result,".jpg"),&p->getImage(i));
        i++;
    }

    free(imageReader);
    free(imageWriter);
}

void Application::startLab3() {

}

void Application::startLab4() {

}

void Application::startLab5() {

}

void Application::startLab6() {

}

void Application::startLab7() {

}

void Application::startLab8() {

}

void Application::startLab9() {

}


