#include "Helper.hpp"
#include "HCTree.hpp"


int main(int argc, char* argv[]) {
    
    //FancyInputStream input(argv[1]);
    //open the output stream a bit earlier
    //FancyOutputStream output(argv[2]);
    if (argc != 3) {
        error("Incorrect input");
    } 
    FancyInputStream input(argv[1]);
    //open the output stream a bit earlier
    FancyOutputStream output(argv[2]);
    int indicator = 0;
    vector<int> freqs = vector<int>(256, 0);
    //does the file size return the total number of bytes?
    //I could also manually calculate it. Lets try that.
    //output.write_int(input.filesize());
    //modification for empty case?
    int fileSize = 0;
    indicator = input.read_byte();
    if (indicator < 0) {
        return 0;
    }
    while (indicator != -1) {
        //increment it, but not when its done
        if (indicator >= 0) {
            freqs[indicator] += 1;
            fileSize += 1;
        }
        indicator = input.read_byte();
    }
    //freqs[10] -= 1;
    //go back to beginning
    input.reset();
    //set up output
    //FancyOutputStream output(argv[2]);
    //do naive approach
    //256 * 4 bytes
    //it seems like not copying the tree is harder in a way?
    /*
    for (int i = 0; i < 256; i++) {
        output.write_int(freqs[i]);
    }
    */
    ///*
    for (int i = 0; i < 256; i++) {
        int original = freqs[i];
        int first = (original & 255);
        int second = ((original >> 4) & 255);
        int third = ((original >> 8) & 255);
        output.write_byte(first);
        output.write_byte(second);
        output.write_byte(third);
    }
    //*/
    //write size
    //output.write_int(input.filesize());
    output.write_int(fileSize);
    HCTree encoding = HCTree();
    encoding.build(freqs);
    indicator = 0;
    while (indicator != -1) {
        indicator = input.read_byte();
        //encode, but not if its done
        if (indicator >= 0) {
            //write encoded version to output
            encoding.encode(indicator, output);
        }
    }
    //delete calls destructor?
    //encoding;
    return 0;
}
