#include "HCTree.hpp"
#include <stack>

//void printNodes

void deleteHelper(HCNode * start) {
    if (start->c0 != nullptr) {
        deleteHelper(start->c0);
    }
    if (start->c1 != nullptr) {
        deleteHelper(start->c1);
    }
    delete start;
    return;
}

HCTree::~HCTree() {
    if (root == nullptr) {
        return;
    }
    deleteHelper(root);
    return;
}

void HCTreePrinter(HCNode * start) {
    if (start == nullptr) {
        cout << "#";
    }
    else {
        if (start->symbol == 0) {
            cout << "0";
        }
        cout << start->symbol;
        HCTreePrinter(start->c0);
        HCTreePrinter(start->c1);
    }
    //cout << "NULL";
}

void HCTree::build(const vector<int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    //handle empty file case in the main functions
    for (int i = 0; i <= 255; i++) {
        //I realized why there should be nonzero entries.
        //If there werent, the 0 nodes would keep being added to each other...
        //actually it wouldnt be wrong but still
        if (freqs[i] != 0) {
            //add leaf node
            HCNode* toInsert = new HCNode(freqs[i], i);
            //add to leaves
            leaves[i] = toInsert;
            pq.push(toInsert);
        }
    }
    //while there are still nodes to be joined
    //could this reference become a problem later?
    while (pq.size() > 1) {
        HCNode* first = pq.top();
        //cout << first->count;
        //cout << first->symbol;
        pq.pop();
        HCNode* second = pq.top();
        //cout << second->count;
        //cout << second->symbol;
        pq.pop();
        //create new node
        HCNode* third = new HCNode((first->count)+(second->count), 255);
        first->p = third;
        second->p = third;
        third->c0 = first;
        third->c1 = second; 
        pq.push(third);
        //cout<< third->count << third->symbol << " ";
    }
    //if its not empty
    if (pq.empty() == false) {
        root = pq.top();
    }
    //HCTreePrinter(root);
    //cout << root->count;
    return;
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    HCNode* currNode = leaves[symbol];
    if (currNode == nullptr) {
        return;
    }
    //currNode should not be a nullptr...
    //unless the file was empty
    //use a stack to encode in right order
    stack<int> toReverse;
    while (currNode->p != nullptr) {
        //use the addresses to compare
        if (currNode->p->c0 == currNode) {
            toReverse.push(0);
            //cout << "0";
            currNode = currNode->p;
        }
        //one of both cases should work
        else if (currNode->p->c1 == currNode) {
            toReverse.push(1);
            //cout << "1";
            currNode = currNode->p;
        }
    }
    //cout << std::endl;
    //sloppy mistake
    while (toReverse.empty() == false) {
        int bit = toReverse.top();
        //cout << bit;
        //write the bits in the right order
        out.write_bit(bit);
        toReverse.pop();
    }
    //cout << " ";
    return;
}

//change from unsigned char to int?
unsigned char HCTree::decode(FancyInputStream & in) const {
    HCNode* currNode = root;
    while (currNode != nullptr) {
        //perhaps make the call first outside the loop? nope
        int readBit = in.read_bit();
        //cout << readBit;
        if (readBit == -1) {
            //what do I do here?
            //return newline?
            //should never get to -1 unless there was nothing to read
            //is this causing an issue?
            //return root symbol, because this means there was nothing encoded
            return root->symbol;
        }
        if ((readBit == 0) && (currNode->c0 != nullptr)) {
            currNode = currNode->c0;
            if ((currNode->c0 == nullptr) && (currNode->c1 == nullptr)) {
                return currNode->symbol;
            }
        }
        else if ((readBit == 1) && (currNode->c1 != nullptr)) {
            currNode = currNode->c1;
            if ((currNode->c0 == nullptr) && (currNode->c1 == nullptr)) {
                return currNode->symbol;
            }
        }
        //this will cover 1 node case?
        //but how could there be.
        else {
            return 0;
        }
        //I found the problem! A bit is not read properly at the end,
        //because when a nullptr is found, that bit isnt reused at
        //the beginning.
        /*
        else {
            return currNode->symbol;
        }  
        */ //Make the check in the 2 cases
    }
    return 0;
}

//DAMN...take into account the newline at the end. 



