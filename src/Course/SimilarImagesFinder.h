//
// Created by tigler on 21.03.18.
//

#ifndef ITOI_SIMILARIMAGESFINDER_H
#define ITOI_SIMILARIMAGESFINDER_H


#include <vector>

class SimilarImagesFinder {
    std::vector<std::vector<std::pair<std::vector<std::vector<double>>,const char *>>> different;
public:
    void findSimilarImages(const char *path);

    void distribute(const char *path);
};


#endif //ITOI_SIMILARIMAGESFINDER_H
