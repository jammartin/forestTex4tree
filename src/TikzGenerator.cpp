//
// Created by Johannes Martin on 14.09.21.
//

#include "../include/TikzGenerator.h"

TikzGenerator::TikzGenerator(const std::string &file){
    ofs = std::ofstream(file);
    if(ofs.good()){
        ofs << global::fileHeader;
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

void TikzGenerator::createBoxes(TreeNode *t, bool randomizeParticlePosition) {
    treeBoxes2tikz(t);
    drawParticles(t, randomizeParticlePosition);

    std::cout << "Finished box plot!" <<  std::endl;
}

// works only for DIM = 2
void TikzGenerator::treeBoxes2tikz(TreeNode *t){
    if (t != nullptr){
        ofs << "    \\draw (" << t->box.lower[0] << ", " << t->box.lower[1] << ") rectangle ("
            << t->box.upper[0] << ", " << t->box.upper[1] << ");\n";
        for (int i=0; i<global::powdim; ++i){
            treeBoxes2tikz(t->son[i]);
        }
    }
}

// only works with integers as labels
void TikzGenerator::drawParticles(TreeNode *t, bool randomizeParticlePosition){
    if (t != nullptr) {
        std::smatch particleMatch;
        std::regex_match(t->label, particleMatch, std::regex(particlePattern));

        if (particleMatch.size() == 2) {
            // strip options from label, label must be an integer
            std::string strippedLabel = particleMatch[1].str();

            double pos[global::dim];
            bool marginTooLarge = false;

            if (randomizeParticlePosition) {
                // create random position in box
                std::random_device rd; // obtain a random number from hardware
                unsigned int seed = rd();
                std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()

                for (int d = 0; d < global::dim; ++d) {
                    if (t->box.upper[d] - t->box.lower[d] > 2 * margin) {
                        std::uniform_real_distribution<double> rndPos(t->box.lower[d] + margin,
                                                                      std::nextafter(t->box.upper[d] - margin,
                                                                                     std::numeric_limits<double>::max()));
                        pos[d] = rndPos(gen);
                    } else {
                        marginTooLarge = true;
                    }
                }

                if (marginTooLarge) {
                    std::cout << "drawParticles: margin to large to randomize position of particle "
                              << strippedLabel << " in box.\n";
                    t->box.getCenter(pos);
                }
            }
            else {
                if (t->box.upper[0] - t->p.x < margin) {
                    pos[0] = t->box.upper[0] - margin;
                }
                else if (t->p.x - t->box.lower[0] < margin) {
                    pos[0] = t->box.lower[0] + margin;
                }
                else {
                    pos[0] = t->p.x;
                }

                if (t->box.upper[1] - t->p.y < margin) {
                    pos[1] = t->box.upper[1] - margin;
                }
                else if (t->p.y - t->box.lower[1] < margin) {
                    pos[1] = t->box.lower[1] + margin;
                }
                else {
                    pos[1] = t->p.y;
                }

            }

            ofs << "    \\node[particle] at (" << pos[0] << ", " << pos[1]
                << ") {" << strippedLabel << "};\n";
        }

        for (int i=0; i<global::powdim; ++i){
            drawParticles(t->son[i]);
        }
    }
}

void TikzGenerator::createSFC(TreeNode *t, bool hilbert){
    treeBoxes2tikz(t);

    std::map<keytype, double*> posByKey {};

    if (hilbert) getKey = &Lebesgue2Hilbert;

    getSFC(posByKey, t, 0UL, 0);

    std::map<keytype, double*>::iterator posIt = posByKey.begin();
    while(true) {
        double *posStart = posIt->second;
        ++posIt;
        if (posIt != posByKey.end()) {
            ofs << "    \\draw [line width=1.0pt](" << posStart[0] << ", " << posStart[1] << ") -- (";
            ofs << posIt->second[0] << ", " << posIt->second[1] << ");\n";
        } else {
            break;
        }
    }

    std::cout << "Finished space-filling curve plot!" << std::endl;
}

void TikzGenerator::getSFC(std::map<keytype, double*> &posByKey, TreeNode *t, keytype k, int lvl){
    if (t != nullptr){
        for (int i=0; i<global::powdim; ++i){
            getSFC(posByKey, t->son[i], k | ((keytype)i << (global::dim*(global::maxTreeLvl-lvl-1))), lvl+1);
        }
        if (t->isLeaf()){
            double *pos = new double[global::dim];
            t->box.getCenter(pos);
            posByKey[getKey(k, lvl)] = pos;
        }
    }
}

keytype TikzGenerator::Lebesgue2Hilbert(keytype lebesgue, int level){
    keytype hilbert = 0UL;
    int dir = 0;
    for (int lvl=global::maxTreeLvl; lvl>0; --lvl){
        int cell = (lebesgue >> ((lvl-1)*global::dim)) & (keytype)((1<<global::dim)-1);
        hilbert = hilbert << global::dim;
        if (lvl>global::maxTreeLvl-level){
            hilbert += global::HilbertTable[dir][cell];
        }
        dir = global::DirTable[dir][cell];
    }
    return hilbert;
}