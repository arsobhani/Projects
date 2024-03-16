#include <iostream>
#include "FastSaga.h"
using namespace std;

/**
 * Print each movie in the Fast (& Furious) Saga using an iterator
 */
int main() {
    FastSaga movies;
    for(string title : movies) {
        cout << title << endl;
    }
    return 0;
}
