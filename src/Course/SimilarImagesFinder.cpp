//
// Created by tigler on 21.03.18.
//

#include "SimilarImagesFinder.h"
#include "../DescriptorFinder.h"
#include "../ImageReaderWriter/ImageReader.h"
#include <string>
#include <functional>
#include <dirent.h>
#include <iostream>
#include <jmorecfg.h>

boolean isImageJpeg(char name[], unsigned short int len){
    for(int i=0;i<len;i++){
        if(name[i]=='.' && name[i+1]=='j' && name[i+2]=='p' && name[i+3]=='g' ||
                name[i]=='.' && name[i+1]=='j' && name[i+2]=='p' && name[i+3]=='e' && name[i+4]=='g'){
            return true;
        }
    }
    return false;
}
void listFiles(const std::string &path, std::function<void(const std::string &)> cb) {
    if (auto dir = opendir(path.c_str())) {
        while (auto f = readdir(dir)) {
            if (!f->d_name || f->d_name[0] == '.') continue;
            if (f->d_type == DT_DIR)
                listFiles(path + f->d_name + "/", cb);

            if (f->d_type == DT_REG)
                if(isImageJpeg(f->d_name,f->d_reclen))
                    cb(path + f->d_name);
        }
        closedir(dir);
    }
}

void SimilarImagesFinder::findSimilarImages(const char* path) {
    listFiles(path, [](const std::string &path) {
        //distribute(path.c_str());
    });
}

void SimilarImagesFinder::distribute(const char *path) {
    ImageReader imageReader = ImageReader();
    auto image = imageReader.read(path);
    DescriptorFinder descriptorFinder = DescriptorFinder();

    auto interestPoints = InterestPoints();
    interestPoints.moravek(image,4,1000);

    auto descriptors = descriptorFinder.findDescriptors(interestPoints.getPoints(),image);
    bool finded = false;
    for(auto similar : different){
        //descriptorFinder.isSimilar(descriptorFinder,similar.);
    }

    if(!finded){
        std::pair<std::vector<std::vector<double>>,const char *> img;
        img.first = descriptors;
        img.second = path;

        std::vector<std::pair<std::vector<std::vector<double>>,const char *>> similars;

        similars.push_back(img);
        different.push_back(similars);
    }

}


