#include "Queue.h"

/**
 * Implement Queue constructor
 */
Queue::Queue() {
    /* YOUR CODE HERE */
    head = nullptr;
    tail = nullptr;
    numElements = 0;
}

/**
 * Implement the Queue destructor
 */
Queue::~Queue() {
    /* YOUR CODE HERE */
    clear();
}

/**
 * Implement size() correctly
 */
unsigned int Queue::size() {
    /* YOUR CODE HERE */
    return numElements;
}

/**
 * Implement clear() correctly without memory leaks
 */
void Queue::clear() {
    /* YOUR CODE HERE */
    while (numElements > 0) {
        pop(); //not this then?  
    }
}

/**
 * Implement push() correctly
 */
void Queue::push(string s) {
    /* YOUR CODE HERE */
    //if (head == nullptr) { the same but still...
    if (numElements == 0) {
        head = new Node(s);
        tail = head;
        numElements += 1;
    }
    else {
        Node* tempNode = new Node(s);
        tail->next = tempNode;
        tail = tempNode;
        numElements += 1;
    }
}

/**
 * Implement pop() correctly without memory leaks
 */
string Queue::pop() {
    /* YOUR CODE HERE */
    //if (head == nullptr) {
        //return "Nothing left";
    //}
    string frontString = head->data;
    Node* tempNode = head;
    head = head->next;
    delete tempNode;
    numElements -= 1;
    //does c++ take care of dangling pointers?
    if (numElements == 0) {
        tail = nullptr;
    }
    return frontString;
}
