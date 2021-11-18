//
// Created by Johannes Martin on 10.09.21.
//

#ifndef FORESTTEX4TREE_BOX_H
#define FORESTTEX4TREE_BOX_H

#include "global.h"
#include "Particle.h"

class Box {
public:
    double lower[global::dim];
    double upper[global::dim];

    Box();
    Box(double lowerBound, double upperBound);

    void getCenter(double *center);
    int getSonBox(Box &sonBox, Particle &p);
};


#endif //FORESTTEX4TREE_BOX_H
