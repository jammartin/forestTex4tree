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
    ranges = new keytype[numProcesses + 1];
}

TikzGenerator::~TikzGenerator(){
    // write end of tikzpicture to file
    ofs << "\\end{tikzpicture}\n";
    ofs.close();

    delete ranges;
}

void TikzGenerator::setNumProcesses(int numProcesses) {
    this->numProcesses = numProcesses;
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

void TikzGenerator::colorDefinitions() {
    for (int i=0; i<numProcesses; ++i) {
        ofs << "    \\def\\color" << static_cast<char>('a' + i) << "{" << global::colors[i] << "}\n";
    }
}

// works only for DIM = 2
void TikzGenerator::treeBoxes2tikzColored(TreeNode *t, keytype k, int lvl) {
    if (t != nullptr){
        int proc = -1;
        for (int j=0; j<numProcesses; ++j){
            if (getKey(k, lvl) >= ranges[j] && getKey(k, lvl) < ranges[j+1]) proc = j;
        }
        if (proc != -1) {
            ofs << "    \\draw [fill=\\color" << static_cast<char>('a' + proc) << "](" << t->box.lower[0] << ", " << t->box.lower[1]
                << ") rectangle ("
                << t->box.upper[0] << ", " << t->box.upper[1] << ");\n";
        }
        for (int i=0; i<global::powdim; ++i){
            treeBoxes2tikzColored(t->son[i], k | ((keytype)i << (global::dim*(global::maxTreeLvl-lvl-1))), lvl+1);
        }
    }
}

// only works with integers as labels
void TikzGenerator::drawParticles(TreeNode *t, bool randomizeParticlePosition, bool center){
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
            else if (center) {
                t->box.getCenter(pos);
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
            drawParticles(t->son[i], randomizeParticlePosition, center);
        }
    }
}

// only works with integers as labels
void TikzGenerator::getParticleKeys(TreeNode *t, keytype k, int lvl, std::vector<keytype> &particleKeys,
                                    std::vector<std::string> &particleLabels) {

    if (t != nullptr){
        for (int i=0; i<global::powdim; ++i){
            getParticleKeys(t->son[i], k | ((keytype)i << (global::dim*(global::maxTreeLvl-lvl-1))), lvl+1,
                            particleKeys, particleLabels);
        }
        std::smatch particleMatch;
        std::regex_match(t->label, particleMatch, std::regex(particlePattern));
        if (particleMatch.size() == 2){
            std::cout << "Label: " << t->label << std::endl;
            particleKeys.push_back(getKey(k, lvl));
            particleLabels.push_back(t->label);
        }
    }

}

void TikzGenerator::determineRanges(TreeNode *t, bool hilbert){

    if (hilbert) getKey = &Lebesgue2Hilbert;
    std::vector<keytype> realParticleKeys;
    std::vector<std::string> realParticleLabels;
    getParticleKeys(t, 0UL, 0, realParticleKeys, realParticleLabels);
    std::vector<std::pair<keytype, std::string>> zipped;
    Utils::zip(realParticleKeys, realParticleLabels, zipped);
    //std::sort(realParticleKeys.begin(), realParticleKeys.end());
    std::sort(zipped.begin(), zipped.end());
    Utils::unzip(zipped, realParticleKeys, realParticleLabels);
    int numParticles = realParticleKeys.size();
    int targetNumParticles = numParticles/numProcesses;
    ranges[0] = 0;
    for (int i=1; i<=numProcesses; i++) {
        ranges[i] = realParticleKeys[i * targetNumParticles];
    }
    ranges[numProcesses] = ~((keytype)0);
    std::ofstream ofsRanges;
    ofsRanges = std::ofstream("range.tex");
    if(ofs.good()){
        ofsRanges << "\\begin{tikzpicture}\n";
        ofsRanges << "  \\def\\ptsize{4.0pt}\n";
        ofsRanges << "  \\draw [very thick](0,0) -- (10,0);\n";
        for (int i=1; i<numProcesses; ++i) {
            std::cout << "range[" << i << "] = " << ranges[i] << std::endl;
            ofsRanges << "  \\def\\vert" << static_cast<char>('a' + i) << "{" << 10/(double)(1UL<<16) * (double)ranges[i]<< "}\n";
        }
        for (int i=1; i<numProcesses; ++i) {
            ofsRanges << "  \\draw [very thick](\\vert" << static_cast<char>('a' + i) << ",0.25) -- (\\vert" << static_cast<char>('a' + i) << ",-0.75);\n";
        }
        double previousParticlePosition = -10.0;
        double particlePosition;
        for (int i=0; i<realParticleKeys.size(); ++i) {
            particlePosition = 10./(double)(1UL<<16) * (double)realParticleKeys[i];
            if (particlePosition - previousParticlePosition < 0.25) {
                particlePosition = previousParticlePosition + 0.25;
            }
            ofsRanges << "  \\coordinate (" << static_cast<char>('A' + i) << ") at (" << particlePosition
                        << ", 0) ;\n";
            ofsRanges << "  \\node at (" << particlePosition << ", 0.5) {" << realParticleLabels[i] << "};\n";
            previousParticlePosition = particlePosition;
        }
        ofsRanges << "  \\foreach \\p in {";
        int proc;
        int temp = 0;
        for (int i=0; i<realParticleKeys.size() - 1; ++i) {
            proc = i/targetNumParticles;
            if (temp == proc) {
                ofsRanges << static_cast<char>('A' + i);// << ", ";
                if ((i+1)/targetNumParticles == proc) {
                    ofsRanges << ", ";
                }
            }
            else {
                ofsRanges << "}\n";
                ofsRanges << "      \\fill[fill=" << global::colors[temp] << ",draw=black,thick] (\\p) circle (\\ptsize);\n";
                ofsRanges << "  \\foreach \\p in {";
                ofsRanges << static_cast<char>('A' + i) << ", ";

            }
            temp = proc;
        }
        ofsRanges << ", " << static_cast<char>('A' + realParticleKeys.size() - 1) << "}\n";
        ofsRanges << "      \\fill[fill=" << global::colors[temp] << ",draw=black,thick] (\\p) circle (\\ptsize);\n";

        ofsRanges << "  \\draw [->, very thick] (\\vert"
                    << static_cast<char>('a' + 1) << "/2, -1.2) -- node[below, yshift=-0.5cm]{process "
                    << 0 << "} (\\vert" << static_cast<char>('a' + 1) << "/2, -0.4);\n";
        for (int i=1; i<numProcesses-1; ++i) {
            ofsRanges << "  \\draw [->, very thick] ({\\vert"
                << static_cast<char>('a' + i) << "+(\\vert" << static_cast<char>('a' + i + 1) << "-\\vert" << static_cast<char>('a' + i)
                << ")/2}, -1.2) -- node[below, yshift=-0.5cm]{process " << i << "} ({\\vert"
                << static_cast<char>('a' + i) << "+(\\vert" << static_cast<char>('a' + i + 1) << " -\\vert"
                << static_cast<char>('a' + i) << ")/2}, -0.4);\n";
        }
        if (numProcesses > 1) {
            ofsRanges << "  \\draw [->, very thick] ({\\vert"
                << static_cast<char>('a' + numProcesses-1) << "+(10-\\vert" << static_cast<char>('a' + numProcesses-1)
                << ")/2}, -1.2) -- node[below, yshift=-0.5cm]{process " << numProcesses-1 << "} ({\\vert"
                << static_cast<char>('a' + numProcesses-1) << "+(10-\\vert"
                << static_cast<char>('a' + numProcesses-1) << ")/2}, -0.4);\n";
        }
    }
    ofsRanges << "\\end{tikzpicture}\n";
    ofsRanges.close();
    //for (int i=0; i<=numProcesses; i++) {
    //    std::cout << "ranges[" << i << "] = " << ranges[i] << std::endl;
    //}

}

void TikzGenerator::createSFC(TreeNode *t, bool hilbert, bool colorize, bool addParticlesCentered){

    if (hilbert) getKey = &Lebesgue2Hilbert;

    if (colorize) {
        determineRanges(t, hilbert);
        colorDefinitions();
        treeBoxes2tikzColored(t, 0UL, 0);
    }
    else {
        treeBoxes2tikz(t);
    }

    // TODO: flag or different approach?
    if (true) {
        std::map<keytype, double*> posByKey {};
        getSFC(posByKey, t, 0UL, 0);
        std::map<keytype, double *>::iterator posIt = posByKey.begin();
        while (true) {
            double *posStart = posIt->second;
            ++posIt;
            if (posIt != posByKey.end()) {
                ofs << "    \\draw [line width=1.0pt](" << posStart[0] << ", " << posStart[1] << ") -- (";
                ofs << posIt->second[0] << ", " << posIt->second[1] << ");\n";
            } else {
                break;
            }
        }
    }

    if (addParticlesCentered) {
        drawParticles(t, false, true);
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