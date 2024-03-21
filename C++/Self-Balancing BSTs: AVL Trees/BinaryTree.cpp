#include "BinaryTree.h"

/**
 * Implement balanceFactors() correctly
 */


int heightHelper(BinaryTree::Node * currNode) {
    //initialize
    //int height = 0;
    if (currNode == nullptr) {
        return 0;
    }
    int leftHeight;
    int rightHeight;


    if (currNode->leftChild != nullptr) {
        leftHeight = 1 + heightHelper(currNode->leftChild);
    }
    else {
        leftHeight = 0;
    }

    if (currNode->rightChild != nullptr) {
        rightHeight = 1 + heightHelper(currNode->rightChild);
    }
    else {
        rightHeight = 0;
    }

    if (rightHeight > leftHeight) {
        return rightHeight;
    }
    else {
        return leftHeight;
    }
}

//helper method for getting balancefactor...maybe unnecessary
//dont need to add 1 to height again since we are subtracting
int balanceFactorsHelper(BinaryTree::Node * currNode) {
    if (currNode == nullptr) {
        return 0;
    }
    int leftHeight = heightHelper(currNode->leftChild);
    int rightHeight = heightHelper(currNode->rightChild);
    return (rightHeight - leftHeight);
}

void preOrderTraversalStore(unordered_map<int,int> * factorsMap, 
        BinaryTree::Node * currNode) {
    if (currNode == nullptr) {
        return;
    }
    factorsMap->insert({currNode->label, balanceFactorsHelper(currNode)});

    if (currNode->leftChild != nullptr) {
        preOrderTraversalStore(factorsMap, currNode->leftChild);
    }
    if (currNode->rightChild != nullptr) {
        preOrderTraversalStore(factorsMap, currNode->rightChild);
    }
}

unordered_map<int,int> BinaryTree::balanceFactors() {
    /* YOUR CODE HERE */
    unordered_map<int,int> factorsMap;//I could do this too? = unordered_map<int,int>();
    //factorsMap[0] = 1;
    //now do some traversal, order doesnt matter
    preOrderTraversalStore(&factorsMap, root);
    return factorsMap;
}





