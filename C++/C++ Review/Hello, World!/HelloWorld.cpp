#include <iostream>
#include <string>
using namespace std;

/**
 * Ask user to enter their name, and then greet them
 */
/* YOUR CODE HERE */
int main() {
//string message;
cout << "Enter name:";
string message;
//cin >> message;
getline(cin, message);
cout << "Hello, " + message + "!" << endl;
cout << "Welcome to CSE 100!";
}
