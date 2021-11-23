//
// Created by Johannes Martin on 10.09.21.
//

#include "../include/TreeNode.h"

TreeNode::TreeNode() {
    for (int i=0; i<global::powdim; ++i){
        son[i] = nullptr; // explicitly setting sons to nullptr when creating new TreeNode
    }
};

TreeNode::TreeNode(Box &_box) : TreeNode(){
    box = _box;
};

bool TreeNode::isLeaf() {
    for (int i=0; i<global::powdim; i++) {
        if (son[i] != nullptr) {
            return false;
        }
    }
    return true;
}

void TreeNode::insertParticle(Particle &p, const std::string &label) {
    Box sonBox {};
    int i = box.getSonBox(sonBox, p);

    if (son[i] == nullptr) {
        if (node == nodeType::particle && isLeaf()) {
            node = nodeType::pseudoParticle;
            Particle pBuffer = this->p;
            son[i] = new TreeNode();
            son[i]->p = p;
            son[i]->box = sonBox;
            son[i]->label = this->label;
            son[i]->node = nodeType::particle;
            this->label = std::string{""};
            insertParticle(pBuffer, label);
        }
        else {
            son[i] = new TreeNode();
            son[i]->p = p;
            son[i]->box = sonBox;
            son[i]->label = label;
            son[i]->node = nodeType::particle;
        }
    }
    else {
        son[i]->insertParticle(p, label);
    }
}

void TreeNode::insertSonByNum(int sonNum){
    double center[global::dim];
    box.getCenter(center);

    son[sonNum] = new TreeNode(); // creating son node
    Box *sonBox = &son[sonNum]->box; // creating reference to son box

    // setting box
    for (int d=0; d<global::dim; ++d){
        if (sonNum % 2){
            sonBox->lower[d] = .5 * (box.upper[d] + box.lower[d]);
            sonBox->upper[d] = box.upper[d];
        } else {
            sonBox->lower[d] = box.lower[d];
            sonBox->upper[d] = .5 * (box.upper[d] + box.lower[d]);
        }
        sonNum = sonNum/2;
    }
}
