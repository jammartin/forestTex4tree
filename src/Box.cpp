//
// Created by Johannes Martin on 10.09.21.
//

#include "../include/Box.h"

Box::Box(){}

Box::Box(double lowerBound, double upperBound){
    for (int d=0; d<global::dim; ++d){
        lower[d] = lowerBound;
        upper[d] = upperBound;
    }
}

void Box::getCenter(double *center){
    for(int d=0; d<global::dim; ++d){
        center[d] = .5 * (upper[d] + lower[d]);
    }
}

int Box::getSonBox(Box &sonBox, Particle &p) {
    int sonIndex_ = 0;

    int d = 0;
    if (p.x < 0.5 * (upper[d] + lower[d])) {
        sonBox.lower[d] = lower[d];
        sonBox.upper[d] = 0.5 * (lower[d] + upper[d]);
    }
    else {
        sonIndex_ += 1;
        sonBox.lower[d] = 0.5 * (lower[d] + upper[d]);
        sonBox.upper[d] = upper[d];
    }

    d = 1;
    if (p.y < 0.5 * (upper[d] + lower[d])) {
        sonBox.lower[d] = lower[d];
        sonBox.upper[d] = 0.5 * (lower[d] + upper[d]);
    }
    else {
        sonIndex_ += 2;
        sonBox.lower[d] = 0.5 * (lower[d] + upper[d]);
        sonBox.upper[d] = upper[d];
    }

    return sonIndex_;
}