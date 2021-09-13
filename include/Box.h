//
// Created by Johannes Martin on 10.09.21.
//

#ifndef FORESTTEX4TREE_BOX_H
#define FORESTTEX4TREE_BOX_H

#include "Constants.h"

class Box {
public:
    double lower[DIM];
    double upper[DIM];

    Box();
    Box(double lowerBound, double upperBound);

    void getCenter(double *center);
};


#endif //FORESTTEX4TREE_BOX_H
