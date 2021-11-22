//
// Created by Johannes Martin on 20.09.21.
//

#ifndef FORESTTEX4TREE_GLOBAL_H
#define FORESTTEX4TREE_GLOBAL_H

#include <string>
#include <algorithm>

namespace global {
    constexpr int dim { 2 };
    constexpr int powdim { 1 << dim }; // 2^dim
    constexpr int maxTreeLvl { 16/dim }; // using 16-bit keys
    constexpr double domainSize { 7. }; // TODO: this should be an input parameter, 14. is good for particles
    const char* const fileHeader { "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
                                   "%%%%% Generated by forestTex4tree %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
                                   "%%%%% https://github.com/jammartin/forestTex4tree %%%%%%%%%%%%%%%%%%%%%%%%%\n"
                                   "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
    };
    const unsigned char DirTable[4][4] =
            { {1,2,0,0}, {0,1,3,1}, {2,0,2,3}, {3,3,1,2} };
    const unsigned char HilbertTable[4][4] =
            { {0,3,1,2}, {0,1,3,2}, {2,3,1,0}, {2,1,3,0} };

    const std::string colors[] = { "Blue", "Red", "Orange", "OliveGreen", "Mulberry", "Apricot", "Violet", "LimeGreen" };
}

#endif //FORESTTEX4TREE_GLOBAL_H
