//
// Created by Johannes Martin on 10.09.21.
//

#ifndef FORESTTEX4TREE_TREENODE_H
#define FORESTTEX4TREE_TREENODE_H

#include <string>

#include "global.h"
#include "Particle.h"
#include "Box.h"

class TreeNode {
public:
    enum nodeType {
        particle, pseudoParticle
    };

    TreeNode();
    TreeNode(Box &_box);

    bool isLeaf();

    Particle p;
    Box box;
    TreeNode *son[global::powdim];
    nodeType node;

    // only for plotting
    std::string label;

    void insertParticle(Particle &p, const std::string &label=std::string{""});
    void insertSonByNum(int sonNum);
};

#endif //FORESTTEX4TREE_TREENODE_H
