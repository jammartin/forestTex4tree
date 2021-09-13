//
// Created by Johannes Martin on 10.09.21.
//

#ifndef FORESTTEX4TREE_TREENODE_H
#define FORESTTEX4TREE_TREENODE_H

#include <string>

#include "Constants.h"
#include "Particle.h"
#include "Box.h"

class TreeNode {
public:
    enum nodeType {
        particle, pseudoParticle
    };

    TreeNode();
    TreeNode(Box &box);

    Particle p;
    Box box;
    TreeNode *son[POWDIM];
    nodeType node;

    // only for plotting
    std::string label;

    void insertSonByNum(int sonNum);
};

#endif //FORESTTEX4TREE_TREENODE_H
