#include "Helper.hpp"
#include "HCTree.hpp"


int main(int argc, char* argv[]) {
    //FancyInputStream input(argv[1]);
    //FancyOutputStream output(argv[2]);
    if (argc != 3) {
        error("Incorrect input");
    }
    FancyInputStream input(argv[1]);
    FancyOutputStream output(argv[2]); 

    //need to fix the naive approach
    vector<int> freqs = vector<int>(256, 0);
    /*
    for (int i = 0; i < 256; i++) {
        freqs[i] = input.read_int();
        //cout << freqs[i];
    }
    */
    ///*
    for (int i = 0; i < 256; i++) {
        //will int work like this I hope?
        int first = input.read_byte();
        //empty case
        if (first < 0) {
            return 0;
        }
        int second = input.read_byte();
        int third = input.read_byte();
        freqs[i] = (first) + (second << 4) + (third << 8);
    }
    //*/


    //filesize at the end of header
    int fileSize = input.read_int();
    //cout << fileSize;
    //this line is giving me trouble
    //FancyOutputStream output(argv[2]);
    //the assignment operator is unnecessary?
    HCTree encoding = HCTree();
    encoding.build(freqs);
    //int indicator = 0;
    for (int i = 0; i < fileSize; i++) {
        unsigned char toWrite;
        //cout << toWrite;
        //load a byte from encoding
        toWrite = encoding.decode(input);
        //write it to uncompressed file
        output.write_byte(toWrite);
    }
    //delete encoding;
    return 0;
}
