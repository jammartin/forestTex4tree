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
#include "../include/Particles2Forest.h"

int main(int argc, char *argv[]){

    /** Reading command line options **/
    cxxopts::Options options("forest2tikz",
                             "Create a 2D representation of a quadtree in bracket notation as tikz picture.");

    options.add_options()
            ("f,file", "Path to input file", cxxopts::value<std::string>())
            ("t,tree", "Path to output file for tree plot", cxxopts::value<std::string>()->default_value("tree.tex"))
            ("b,box", "Path to output file for box plot", cxxopts::value<std::string>()->default_value("boxes.tex"))
            ("h,help", "Show this help");

    // read and store options provided
    auto opts = options.parse(argc, argv);

    // print help on usage and exit
    if (opts.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    } else if (opts.count("file")) {

        std::string file = opts["file"].as<std::string>();

        Box domain { 0., DOMAINSIZE };
        TreeNode root {domain }; // create empty root node

        if (file.find(std::string{ ".h5" }) != std::string::npos) {
            Particles2Forest particles2Forest{file};

            particles2Forest.buildTree(root);
            particles2Forest.createForest(root, opts["tree"].as<std::string>());

            particles2Forest.extendTree(&root);

            TikzGenerator tikzGenerator{opts["box"].as<std::string>()};

            tikzGenerator.createBoxes(&root, false);

        }
        else if (file.find(std::string{ ".tex" }) != std::string::npos) {
            ForestParser parser{file};

            parser.buildTree(root);

            TikzGenerator tikzGenerator{opts["box"].as<std::string>()};

            tikzGenerator.createBoxes(&root, true);
        }
        else {
            std::cerr << "Provided file neither *.h5 nor *.tex." << std::endl;
            exit(1);
        }

    } else {
        std::cerr << "No file provided. Run with -h on usage. Exiting." << std::endl;
        exit(1);
    }

    return 0;
}

