#include <iostream>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <vector>
#include <stack>
#include "interpreter.h"
using namespace std;


const string OS = "WINDOWS";






void usage(){
    cout << "\n---USAGE INSTRUCTIONS---" << endl;
    cout << "Put the file you want to run in the FILES folder." << endl;
    cout << "Open command prompt in the " << OS <<  " folder." << endl;
    cout << "Type in command prompt: ./brainfuck FILENAME" << endl;
    cout << "If you want overflow to reset to 0, then ./brainfuck FILENAME 1 (Not Recommended)\n" << endl;;

}


int main(int argc, char *argv[]){

    if (argc <= 1){                     //make sure to update
        usage();
        return 1;
    }
    bool overFlow = false;
    if (argc == 3){
        char c = *argv[2];
        if (c != '1'){
            cout << "Ignoring overflow reset arguement. Check README for usage." << endl;
        }
        else{
            overFlow = true;
        }
    }

    
    InterpreterBF *ifb = new InterpreterBF(string(argv[1]), 30000, overFlow);


    int status = ifb->execute();


    delete ifb;
   


    return status;

}