//
// Created by Johannes Martin on 14.09.21.
//

#include "../include/TikzGenerator.h"

TikzGenerator::TikzGenerator(const std::string &file){
    ofs = std::ofstream(file);
    if(ofs.good()){
        ofs << header;
        // write tikzpicture environment to file
        ofs << "\\begin{tikzpicture}\n";
        ofs << "\\tikzset{particle/.style={minimum width=4ex, circle, fill=black, text=white, font=\\footnotesize}}\n";
    } else {
        std::cerr << "Unable to open output file. Exiting." << std::endl;
        exit(1);
    }
}

TikzGenerator::~TikzGenerator(){
    // write end of tikzpicture to file
    ofs << "\\end{tikzpicture}\n";
    ofs.close();
}

// works only for DIM = 2
void TikzGenerator::treeBoxes2tikz(TreeNode *t){
    if (t != NULL){
        ofs << "    \\draw (" << t->box.lower[0] << ", " << t->box.lower[1] << ") rectangle ("
            << t->box.upper[0] << ", " << t->box.upper[1] << ");\n";
        for (int i=0; i<POWDIM; ++i){
            treeBoxes2tikz(t->son[i]);
        }
    }
}

// only works with integers as labels
void TikzGenerator::drawParticles(TreeNode *t){
    if (t != NULL){
        std::smatch particleMatch;
        std::regex_match(t->label, particleMatch, std::regex(particlePattern));

        if (particleMatch.size() == 2){
            // strip options from label, label must be an integer
            std::string strippedLabel = particleMatch[1].str();

            double pos[DIM];
            bool marginTooLarge = false;

            // create random position in box
            std::random_device rd; // obtain a random number from hardware
            unsigned int seed = rd();
            std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()

            for (int d=0; d<DIM; ++d){
                if (t->box.upper[d] - t->box.lower[d] > 2*margin){
                    std::uniform_real_distribution<double> rndPos(t->box.lower[d] + margin,
                                                                  std::nextafter(t->box.upper[d] - margin,
                                                                                 std::numeric_limits<double>::max()));
                    pos[d] = rndPos(gen);
                } else {
                     marginTooLarge = true;
                }
            }

            if (marginTooLarge){
                std::cout << "drawParticles: margin to large to randomize position of particle "
                          << strippedLabel << " in box.\n";
                t->box.getCenter(pos);
            }

            ofs << "    \\node[particle] at (" << pos[0] << ", " << pos[1]
                << ") {" << strippedLabel << "};\n";
        }

        for (int i=0; i<POWDIM; ++i){
            drawParticles(t->son[i]);
        }
    }
}