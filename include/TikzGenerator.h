//
// Created by Johannes Martin on 14.09.21.
//

#ifndef FORESTTEX4TREE_TIKZGENERATOR_H
#define FORESTTEX4TREE_TIKZGENERATOR_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <regex>
#include <random>
#include <map>

#include "global.h"
#include "TreeNode.h"

typedef std::uint_fast64_t keytype;

class TikzGenerator {
public:
    TikzGenerator(const std::string &file);
    ~TikzGenerator();

    void setNumProcesses(int numProcesses);
    void createBoxes(TreeNode *t, bool randomizeParticlePosition=false);
    void createSFC(TreeNode *t, bool hilbert=false, bool colorize=true, bool addParticlesCentered=false);

private:
    std::ofstream ofs;
    const std::string particlePattern { R"RAW(([0-9]+),?.*)RAW" };

    int numProcesses = 8;
    keytype *ranges;

    static constexpr double margin { global::domainSize/25. };

    void treeBoxes2tikz(TreeNode *t);
    void colorDefinitions();
    void treeBoxes2tikzColored(TreeNode *t, keytype k, int lvl);
    void drawParticles(TreeNode *t, bool randomizeParticlePosition=false, bool center=false);
    void getParticleKeys(TreeNode *t, keytype k, int lvl, std::vector<keytype> &particleKeys,
                         std::vector<std::string> &particleLabels);
    void getSFC(std::map<keytype, double*> &posByKey, TreeNode *t, keytype k, int lvl);
    void determineRanges(TreeNode *t, bool hilbert);

    static keytype Lebesgue(keytype k_, int _){ return k_; }
    static keytype Lebesgue2Hilbert(keytype lebesgue, int level);
    keytype (*getKey)(keytype, int){ &Lebesgue };
};


namespace Utils {
    // see: https://stackoverflow.com/questions/37368787/c-sort-one-vector-based-on-another-one
    template <typename A, typename B>
    void zip(
            const std::vector<A> &a,
            const std::vector<B> &b,
            std::vector<std::pair<A,B>> &zipped)
    {
        for(size_t i=0; i<a.size(); ++i)
        {
            zipped.push_back(std::make_pair(a[i], b[i]));
        }
    }

    template <typename A, typename B>
    void unzip(
            const std::vector<std::pair<A, B>> &zipped,
            std::vector<A> &a,
            std::vector<B> &b)
    {
        for(size_t i=0; i<a.size(); i++)
        {
            a[i] = zipped[i].first;
            b[i] = zipped[i].second;
        }
    }
}


#endif //FORESTTEX4TREE_TIKZGENERATOR_H
