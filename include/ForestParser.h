//
// Created by Johannes Martin on 13.09.21.
//

#ifndef FORESTTEX4TREE_FORESTPARSER_H
#define FORESTTEX4TREE_FORESTPARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <regex>

#include "Constants.h"
#include "TreeNode.h"

class ForestParser {
public:
    ForestParser(const std::string &file);

    std::string forestInp;
    std::string forestOpts;

    void buildTree(TreeNode &root);

private:
    // first group: options; second group: root label; third group: the tree
    const std::string forestPattern { R"RAW(\\begin\{forest\}(.*?)\[([^\[\]]*)(.*)\]\\end\{forest\})RAW" };
    // first group: label; second group: rest of the tree
    const std::string innerNodePattern { R"RAW(^\[([^\[\]]*)(\[.*))RAW" };
    const std::string leafPattern { R"RAW(^\[([^\[\]]*)\](.*))RAW" };
    const std::string endNodePattern { R"RAW(^\](.*))RAW" };

    void nodes2tree(const std::string &nodeStr, int lvl, TreeNode *nodeStack[], int *sonNumStack);
};


#endif //FORESTTEX4TREE_FORESTPARSER_H
