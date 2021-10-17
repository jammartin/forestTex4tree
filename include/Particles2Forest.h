//
// Created by Michael Staneker on 15.10.21.
//

#ifndef FORESTTEX4TREE_PARTICLES2FOREST_H
#define FORESTTEX4TREE_PARTICLES2FOREST_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>

#include <highfive/H5File.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5DataSet.hpp>

#include "Particle.h"
#include "TreeNode.h"

class Particles2Forest {

public:

    Particles2Forest(const std::string &file);

    std::string particlesInp;
    std::vector<Particle> particles;

    void buildTree(TreeNode &root);
    void extendTree(TreeNode *t);
    void createForest(TreeNode &root, const std::string &outputFile);

    void traverseForest(TreeNode &t, std::stringstream &ss);

};


#endif //FORESTTEX4TREE_PARTICLES2FOREST_H
