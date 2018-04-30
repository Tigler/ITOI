//
// Created by tigler on 21.03.18.
//

#include "SimilarImagesFinder.h"
#include "../DescriptorFinder.h"
#include "../ImageReaderWriter/ImageReader.h"
#include <string>
#include <iostream>
#include "dirent.h"
#include <functional>
#include <jmorecfg.h>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
bool byDistance (DMatch i,DMatch j) { return (i.distance<j.distance); }

void SimilarImagesFinder::calculateDiscOriginal(const char *path){
    Mat img = imread(path,IMREAD_GRAYSCALE);
    if(!img.data) {
        printf("Не удалось получить файл оригинального изображения");
        return;
    }
    Ptr<xfeatures2d::SIFT> detector = xfeatures2d::SIFT::create();
    std::vector<KeyPoint> keypoints;
    detector->detect(img,keypoints);
    detector->compute(img,keypoints,this->descriptorsOriginal);
}
void SimilarImagesFinder::distribute(const char *path) {
    Mat img = imread(path, IMREAD_GRAYSCALE);
    if(!img.data){
        printf("Не удалось получить файл найденного изображения");
        return;
    }

    Ptr<xfeatures2d::SIFT> detector = xfeatures2d::SIFT::create();

    std::vector<KeyPoint> keypoints;
    detector->detect(img,keypoints);
    Mat descriptors;

    detector->compute(img,keypoints,descriptors);

    BFMatcher matcher(NORM_L2);
    std::vector< DMatch > matches;
    matcher.match(descriptorsOriginal, descriptors, matches);

    std::sort (matches.begin(), matches.end(), byDistance);
    std::vector<DMatch> matchesRes;
    for(DMatch m:matches){
        if(m.distance<this->D){
            matchesRes.push_back(m);
        }
    }

    if(matchesRes.size()>this->N){
        printf(path);printf("\n");
    }
}



bool isImageJpeg(char name[], unsigned short int len){
    for(int i=0;i<len;i++){
        if(name[i]=='.' && name[i+1]=='j' && name[i+2]=='p' && name[i+3]=='g' ||
                name[i]=='.' && name[i+1]=='j' && name[i+2]=='p' && name[i+3]=='e' && name[i+4]=='g'){
            return true;
        }
    }
    return false;
}
void SimilarImagesFinder::listFiles(const std::string &path) {
    if (auto dir = opendir(path.c_str())) {
        while (auto f = readdir(dir)) {
            if (!f->d_name || f->d_name[0] == '.') continue;
            if (f->d_type == DT_DIR)
                listFiles(path + f->d_name + "/");

            if (f->d_type == DT_REG)
                if(isImageJpeg(f->d_name,f->d_reclen))
                    distribute((path + f->d_name).c_str());
        }
        closedir(dir);
    }
}

void SimilarImagesFinder::findSimilarImages(const char* path) {
    listFiles(path);

//    int dictionarySize=200;
//    TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
//    int retries=1;
//
//    int flags=KMEANS_PP_CENTERS;
//    BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
//    Mat dictionary=bowTrainer.cluster(featuresUnclustered);
//    FileStorage fs("dictionary.yml", FileStorage::WRITE);
//    fs << "vocabulary" << dictionary;
//    fs.release();
//
//
//    Mat dictionary;
//    FileStorage fs5("dictionary.yml", FileStorage::READ);
//    fs5["vocabulary"] >> dictionary;
//    fs5.release();
//
//    //create a nearest neighbor matcher
//    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher());
//    int minHessian = 1000000000;
//    Ptr<xfeatures2d::SIFT> detector = xfeatures2d::SIFT::create();
//    //create Sift descriptor extractor
//    Ptr<DescriptorExtractor> extractor(new xfeatures2d::SiftDescriptorExtractor);
//    //create BoF (or BoW) descriptor extractor
//    BOWImgDescriptorExtractor bowDE(detector,matcher);
//    //Set the dictionary with the vocabulary we created in the first step
//    bowDE.setVocabulary(dictionary);
//
////    //To store the image file name
//    char * filename = new char[100];
////    //To store the image tag name - only for save the descriptor in a file
//    char * imageTag = new char[10];
//
//    //open the file to write the resultant descriptor
//    FileStorage fs1("descriptor.yml", FileStorage::WRITE);

    //the image file with the location. change it according to your image file location
//    sprintf(filename,"/home/tigler/picture.jpg");
//    //read the image
//    Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
//    Mat img1=imread("/home/tigler/picture.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//    //To store the keypoints that will be extracted by SIFT
//    std::vector<KeyPoint> keypoints;
//    //Detect SIFT keypoints (or feature points)
//    detector->detect(img,keypoints);
//    std::vector<KeyPoint> keypoints1;
//    //Detect SIFT keypoints (or feature points)
//    detector->detect(img1,keypoints1);
//    //To store the BoW (or BoF) representation of the image
//    Mat bowDescriptor;
//    Mat bowDescriptor1;
//    //extract BoW (or BoF) descriptor from given image
//    bowDE.compute(img,keypoints,bowDescriptor);
//    bowDE.compute(img1,keypoints1,bowDescriptor);
//
//    BFMatcher matcher1(NORM_L2);
//    std::vector< DMatch > matches;
//    matcher->match(bowDescriptor, bowDescriptor1, matches);
//    //prepare the yml (some what similar to xml) file
//    sprintf(imageTag,"img1");
//    fs1 << imageTag << bowDescriptor;
//
//    fs1.release();

}


