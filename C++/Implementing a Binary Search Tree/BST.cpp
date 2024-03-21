#include "BST.h"
/**
 * Implement the BST constructor
 */
BST::BST() {
    /* YOUR CODE HERE */
    root = nullptr;
    numElements = 0;
}

/**
 * Implement the BST destructor
 */
BST::~BST() {
    /* YOUR CODE HERE */
    clear();
}

/**
 * Implement size() correctly
 */
unsigned int BST::size() const {
    /* YOUR CODE HERE */
    return numElements;
}


void postOrderHelper(BST::Node* startNode) {
    //trivial case?
    /*
    if (startNode == nullptr) {
        return;
    }
    */
    if (startNode->leftChild != nullptr) {
        postOrderHelper(startNode->leftChild);
        //does this make a difference?
        //startNode->leftChild = nullptr;
    }
    if (startNode->rightChild != nullptr) {
        postOrderHelper(startNode->rightChild);
        //startNode->rightChild = nullptr;
    }
    delete startNode;
    //startNode = nullptr;
    return;
}

/**
 * Implement clear() correctly without memory leaks
 */
void BST::clear() {
    /* YOUR CODE HERE */
    //Do a post-order search
    if (root == nullptr) {
        return;
    }
    postOrderHelper(root);
    //root = nullptr;
    numElements = 0;
    return;
}

//how to do clear without helper method?
//goes LRV
//delete all children, then ourself
//this method is amazing


//the error was because this came after????
/*
void postOrderHelper(BST::Node* startNode) {
    if (startNode->leftChild != nullptr) {
        postOrderHelper(startNode->leftChild);
    }
    if (startNode->rightChild != nullptr) {
        postOrderHelper(startNode->rightChild);
    }
    delete startNode;
    return;
}
*/


/**
 * Implement insert() correctly
 */
bool BST::insert(int element) {
    /* YOUR CODE HERE */
    BST::Node* currNode;
    currNode = root;
    //int insertCheck = 0;
    //check if empty
    if (root == nullptr) {
        //use root instead of currNode?
        root = new Node(element);
        numElements += 1;
        return true;
    }
    while (currNode != nullptr) {
        if (element == currNode->data) {
            //false if it exists
            return false;
        }
        //important to use else if...do not make extra if statement
        else if (element < currNode->data) {
            if (currNode->leftChild == nullptr) {
                currNode->leftChild = new Node(element);
                currNode->leftChild->parent = currNode;
                numElements += 1;
                return true;
            }
            else {
                currNode = currNode->leftChild;
            }
        }
        else if (element > currNode->data) {
            if (currNode->rightChild == nullptr) {
                currNode->rightChild = new Node(element);
                currNode->rightChild->parent = currNode;
                numElements += 1;
                return true;
            }
            else {
                currNode = currNode->rightChild;
            }
        }
    }
    //a warning for this?
    return true;
}

/**
 * Implement find() correctly
 */
bool BST::find(const int & query) const {
    /* YOUR CODE HERE */
    BST::Node* currNode;
    currNode = root;
    //int check = 0;
    while (currNode != nullptr) {
        if (query == currNode->data) {
            return true;
        }
        //important to use else if...do not make extra if statement
        else if (query < currNode->data) {
            currNode = currNode->leftChild;
        }
        else if (query > currNode->data) {
            currNode = currNode->rightChild;
        }
    }
    return false;
}

/**
 * Implement the getLeftMostNode() function correctly
 */
BST::Node* BST::getLeftMostNode() {
    /* YOUR CODE HERE */
    BST::Node* currNode; //is buffer accurate here?
    currNode = root;
    if (root == nullptr) {
        return nullptr;
    }
    while (currNode->leftChild != nullptr) {
        currNode = currNode->leftChild;
    }
    return currNode;
}

//how to do this without helper?
/*
BST::Node* postOrderHelper(BST::Node* start) {

    return nullptr;
}
*/

/**
 * Implement the BST::Node successor function correctly
 */
BST::Node* BST::Node::successor() {
    /* YOUR CODE HERE */
    BST::Node* currNode;
    BST::Node* parentNode;
    int childData;
    //int leftCheck = 0;
    if (rightChild != nullptr) {
        currNode = rightChild;
        while (currNode->leftChild != nullptr) {
            currNode = currNode->leftChild;
        }
        return currNode;
    }
    //if there is no right child...look for a parent
    else {
        parentNode = parent;
        childData = data;
        //redundant a bit
        if (parentNode == nullptr) {
            return nullptr;
        }
        while (parentNode != nullptr) {
            //check if leftChild exists
            if (parentNode->leftChild != nullptr) {
                if (parentNode->leftChild->data == childData) {
                    return parentNode;
                }
            }    
            //try another pair
            childData = parentNode->data;
            parentNode = parentNode->parent;
        }
    }
    return nullptr;
}
