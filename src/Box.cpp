//
// Created by Johannes Martin on 10.09.21.
//

#include "../include/Box.h"

Box::Box(){}

Box::Box(double lowerBound, double upperBound){
    for (int d=0; d<DIM; ++d){
        lower[d] = lowerBound;
        upper[d] = upperBound;
    }
}

void Box::getCenter(double *center){
    for(int d=0; d<DIM; ++d){
        center[d] = .5 * (upper[d] + lower[d]);
    }
}