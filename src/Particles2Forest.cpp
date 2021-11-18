//
// Created by Michael Staneker on 15.10.21.
//

#include "../include/Particles2Forest.h"

Particles2Forest::Particles2Forest(const std::string &file) {

    // open file
    std::ifstream ifs (file, std::ifstream::in);

    // read in file into string
    if (ifs.good()) {

        HighFive::File h5file(file.c_str(), HighFive::File::ReadOnly);

        // containers to be filled
        std::vector<std::vector<double>> x;
        // read datasets from file
        HighFive::DataSet pos = h5file.getDataSet("/x");

        // read data
        pos.read(x);

        for (int i=0; i<x.size(); i++) {
            particles.push_back(Particle{ x[i][0], x[i][1] });
            //std::cout << "x[" << i << "] = (" << x[i][0] << ", " << x[i][1] << ", " << x[i][2] << ")" << std::endl;
        }

    }
}

void Particles2Forest::buildTree(TreeNode &root) {

    root.p = particles[0];
    root.label = std::to_string(0);
    for (int i=1; i<particles.size(); ++i) {
        root.insertParticle(particles[i], std::to_string(i));
    }
}

void Particles2Forest::extendTree(TreeNode *t) {
    if (t != NULL) {
        if (!t->isLeaf()) {
            for (int i=0; i<global::powdim; ++i) {
                if (t->son[i] == nullptr) {
                    t->insertSonByNum(i);
                }
                else {
                    extendTree(t->son[i]);
                }
            }
        }
    }
}

void Particles2Forest::createForest(TreeNode &root, const std::string &outputFile) {

    std::stringstream forest;
    forest << global::fileHeader;
    forest << "\\begin{forest}\n";
    forest << "for tree = {circle, fill=black, text=white, node options={minimum width = 3ex}, font=\\footnotesize}\n";

    traverseForest(root, forest);

    forest << "\\end{forest}\n";
    //std::cout << forest.str() << std::endl;

    std::ofstream ofs;
    ofs = std::ofstream(outputFile);
    ofs << forest.str();
    ofs.close();

    std::cout << "Finished tree plot. Saved to " << outputFile.c_str() << "!" << std::endl;

}

void Particles2Forest::traverseForest(TreeNode &t, std::stringstream &ss) {

    if (t.node == TreeNode::nodeType::pseudoParticle) {
        ss << "[,fill=gray\n";
        for (int i=0; i<global::powdim; i++) {
            if (t.son[i] != nullptr) {
                traverseForest(*t.son[i], ss);
            }
            else {
                ss << "[,fill=white]";
            }
        }
        ss << "\n]";
    }
    else if (t.node == TreeNode::nodeType::particle) {
        ss << "[" << t.label << ",minimum width=4ex]";
    }
    else {
        // not possible (for now)
        throw std::runtime_error("nodeType::commonCoarseNode not implemented yet. Should be avoided!");
    }
}
