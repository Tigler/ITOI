#include <iostream>
#include "src/Application.h"
#include <cstring>


int main(int argc, char *argv[]) {
    const char *lab = nullptr;
    const char *path = nullptr;
    const char *sg = nullptr;
    const char *N = nullptr;
    const char *D = nullptr;

    if (argc > 1) {
        int arg = 1;
        while (arg < argc) {
            lab = strcmp(argv[arg], "-lab") == 0 && lab == nullptr ? argv[arg + 1] : lab;
            path = strcmp(argv[arg], "-path") == 0 && path == nullptr ? argv[arg + 1] : path;
            sg = strcmp(argv[arg], "-sg") == 0 && sg == nullptr ? argv[arg + 1] : sg;
            N = strcmp(argv[arg], "-N") == 0 && N == nullptr ? argv[arg + 1] : N;
            D = strcmp(argv[arg], "-D") == 0 && D == nullptr ? argv[arg + 1] : D;
            arg++;
        }
    } else {
        std::cout << "Set default arguments" << std::endl;
    }

    lab = lab == nullptr ? "course" : lab;  //номер лабораторной
    sg = sg == nullptr ? "2" : sg;
    N = N == nullptr ? "15" : N;
    D = D == nullptr ? "75" : D;
    if(path== nullptr){
        std::cout << "path not set" << std::endl;
    }
    auto application = Application();

    if(strcmp(lab,"1")==0){
        application.startLab1("/home/tigler/picture5.jpg",atof(sg));
        std::cout << "Lab1 complete..." << std::endl;
    }
    if(strcmp(lab,"2")==0){
        application.startLab2("/home/tigler/picture5.jpg");
        std::cout << "Lab2 complete..." << std::endl;
    }
    if(strcmp(lab,"3")==0){
        application.startLab3("/home/tigler/picture5.jpg");
        std::cout << "Lab3 complete..." << std::endl;
    }
    if(strcmp(lab,"4")==0){
        application.startLab4("/home/tigler/picture5.jpg");
        std::cout << "Lab4 complete..." << std::endl;
    }
    if(strcmp(lab,"5")==0){
        application.startLab5("/home/tigler/picture5.jpg");
        std::cout << "Lab5 complete..." << std::endl;
    }
    if(strcmp(lab,"course")==0){
        application.course("/home/tigler/testCourse/popugaiOriginal.jpg","/home/tigler/testCourse/folder/",atoi(N),atoi(D));
        std::cout << "course complete..." << std::endl;
    }

    return 0;
}