//
// Created by Johannes Martin on 13.09.21.
//

#include "../include/ForestParser.h"

ForestParser::ForestParser(const std::string &file){
    // open file
    std::ifstream ifs (file, std::ifstream::in);

    // read in file into string
    while (ifs.good()){
        char c = ifs.get();
        if (c != '\n' && c != '\r' && c != '\t' && c != ' ' && c != EOF){ // ignore all whitespace
           forestInp += c;
        }
    }

    // close file
    ifs.close();
}

void ForestParser::buildTree(TreeNode &root){

    // container for groups
    std::smatch groupsMatch;

    std::regex_match(forestInp, groupsMatch, std::regex(forestPattern));

    if (groupsMatch.size() == 4){
        forestOpts = groupsMatch[1].str();
        std::cout << "Parsed forest options: " << forestOpts << std::endl;

        root.label = groupsMatch[2].str();

        std::cout << "Root label: " << root.label << std::endl;

        std::string treeStr = groupsMatch[3].str();

        std::cout << "Tree to be parsed: " << treeStr << std::endl;

        int sonNumStack[MAXLEVEL];
        for (int lvl=0; lvl<MAXLEVEL; ++lvl){
            sonNumStack[lvl] = 0; // start with son number zero on all levels
        }
        TreeNode *nodeStack[MAXLEVEL];
        nodeStack[0] = &root;
        nodes2tree(treeStr, 0, nodeStack, sonNumStack);

    } else {
        std::cerr << "Unrecognized input. Expecting forest input. Exiting." << std::endl;
        exit(1);
    }
}

void ForestParser::nodes2tree(const std::string &nodeStr, int lvl, TreeNode *nodeStack[], int *sonNumStack){

    std::smatch nodeMatch;
    std::regex_match(nodeStr, nodeMatch, std::regex(innerNodePattern));

    std::smatch leafMatch;
    std::regex_match(nodeStr, leafMatch, std::regex(leafPattern));

    std::smatch endNodeMatch;
    std::regex_match(nodeStr, endNodeMatch, std::regex(endNodePattern));

    std::string nextStr; // container for rest of the tree to be parsed recursively

    if (nodeMatch.size() == 3 || leafMatch.size() == 3){
        // matched an inner node or a leaf
        nodeStack[lvl]->insertSonByNum(sonNumStack[lvl]); // insert node into tree

        if (nodeMatch.size() == 3){ // inner node
            nodeStack[lvl+1] = nodeStack[lvl]->son[sonNumStack[lvl]]; // push created node to stack
            sonNumStack[lvl]++; // increase son number for this level
            ++lvl; // increase level
            nodeStack[lvl]->label = nodeMatch[1].str(); // assign matched label
            nextStr = nodeMatch[2].str();
        } else { // leaf
            nodeStack[lvl]->son[sonNumStack[lvl]]->label = leafMatch[1].str();
            sonNumStack[lvl]++; // increase son number for this level
            nextStr = leafMatch[2].str();
        }

        /** DEBUG **/
        std::cout << "Label = " << nodeStack[lvl]->label << std::endl;
        std::cout << nextStr << std::endl;
        /** DEBUG END **/

        nodes2tree(nextStr, lvl, nodeStack, sonNumStack);
    } else if (endNodeMatch.size() == 2){
        sonNumStack[lvl] = 0; // reset son numbers
        --lvl; // decrease level
        nextStr = endNodeMatch[1].str();
        nodes2tree(nextStr, lvl, nodeStack, sonNumStack);
    } else if (lvl == 0){
        return; // arrived at root
    }
    else {
        std::cerr << "Unexpected syntax. Aborting." << std::endl;
        exit(1);
    }
}

