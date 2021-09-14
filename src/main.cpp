//
// Created by Johannes Martin on 10.09.21.
//

#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include "../include/TreeNode.h"
#include "../include/Box.h"
#include "../include/ForestParser.h"
#include "../include/TikzGenerator.h"

int main(int argc, char *argv[]){

    /** Reading command line options **/
    cxxopts::Options options("forest2tikz",
                             "Create a 2D representation of a quadtree in bracket notation as tikz picture.");

    options.add_options()
            ("f,file", "Path to input file", cxxopts::value<std::string>())
            ("o,output", "Path to output file", cxxopts::value<std::string>()->default_value("output.tex"))
            ("h,help", "Show this help");

    // read and store options provided
    auto opts = options.parse(argc, argv);

    // print help on usage and exit
    if (opts.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    } else if (opts.count("file")) {

        Box domain { 0., DOMAINSIZE };
        TreeNode root {domain }; // create empty root node
        ForestParser parser { opts["file"].as<std::string>() };
        parser.buildTree(root);

        TikzGenerator tikzGenerator { opts["output"].as<std::string>() };

        tikzGenerator.treeBoxes2tikz(&root);
        tikzGenerator.drawParticles(&root);

    } else {
        std::cerr << "No file provided. Run with -h on usage. Exiting." << std::endl;
        exit(1);
    }

    return 0;
}

