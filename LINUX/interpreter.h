#include <stack>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <unistd.h>
using namespace std;

extern void usage();
extern const string OS;


struct ComboCommand{
    int index;
    int amount;
    char type;
    ComboCommand(int index, char type){
        this->index = index;
        amount = 1;
        this->type = type;
    }
};



class InterpreterBF{
    public:
        InterpreterBF(string fileName = "", int size = 30000, bool overflow = false) : cells(size, 0){
            this->fileName = fileName;
            SIZE = size;
            dataPointer = &cells.at(0);
            dataPointerIndex = 0;
            overFlowFlag = overflow;
        }

        ~InterpreterBF(){
           
        }

        void setFileName(string file){
            fileName = file;
        }

        string getFilePath(){
            string s = "";
            char result[512];
            char *answer = getcwd(result, sizeof(result));
            if (answer){
                s = answer;
            }

            return s;
        }

        int execute(){
            if (fileName == ""){
                usage();
                return 1;
            }
            ifstream ifs;
            string exePath = getFilePath();
            int index = exePath.find(OS);

            if (index == -1){
                cout << "\n Error when looking for the MACOS folder. Consider reinstalling?\n" << endl;
                usage();
                return 1;
            }

            string filePath = exePath.substr(0, index) + "FILES/" + fileName;

            //cout << filePath << endl;

            ifs.open(filePath);

            if (!ifs.is_open()){
                cout << "\n Couldn't open file \"" << fileName << "\"\n" << endl;
                usage();
                return 1;
            }
    
            stack<int> leftBrack;
            string code = "";
            char input;
            index = -1;
            ComboCommand *current = NULL;
            while (!ifs.eof()){         
                ifs.get(input);
                if (ifs.eof()){
                    break;
                }
                switch (input){
                    case '>':
                        ++index;
                        combineCommand(current, index, '>', false);
                        code += input;
                        break;
                    case '<':
                        ++index;
                        combineCommand(current, index, '<', false);
                        code += input;
                        break;
                    case '+':
                       ++index;
                        combineCommand(current, index, '+', false);
                        code += input;
                        break;
                    case '-':
                       ++index;
                        combineCommand(current, index, '-', false);
                        code += input;
                        break;
                    case '.':
                        code += input;
                        ++index;
                        combineCommand(current, index, '.', false);
                        break;
                    case ',':
                        code += input;
                        ++index;
                         combineCommand(current, index, ',', true);
                        break;
                    case '[':
                        code += input;
                        ++index;
                        leftBrack.push(index);
                        combineCommand(current, index, '[', true);
                        break;
                    case ']':
                        code += input;
                        ++index;
                        combineCommand(current, index, ']', true);
                        if (leftBrack.size() != 0){
                            int left = leftBrack.top();
                            leftBrack.pop();
                            pairs[left] = index;
                            pairs[index] = left;
                        }
                        else{
                            cout << "\nIncorrect amount of brackets used.\n" << endl;
                            return 1;
                        }
                        break;
                    default:
                        break;  
                }
            }

            if (current != NULL){
                combo[current->index] = current;
                current = NULL;
            }

            if (code.size() == 0){
                cout << "Poo hoo" << endl;
                return 0;
            }

            int codeIndex = 0;
            bool run = true;
            while (run){
                char command = code.at(codeIndex);
                bool status = false;
                switch (command){
                    case '>':
                    {
                        ComboCommand *c;
                        try{
                            c = combo.at(codeIndex);
                        }
                        catch(out_of_range e){
                            cout << "OUT OF RANGE EXCEPTION CAUGHT AT CODEINDEX " << codeIndex << endl;
                            exit(-1);
                        }
                        
                        status = moveToRightCommand(c->amount);
                        if (!status){
                            return 1;
                        }
                        codeIndex += c->amount;
                        break;
                    }
                    case '<':
                    {
                        ComboCommand *c;
                        try{
                            c = combo.at(codeIndex);
                        }
                        catch(out_of_range e){
                            cout << "OUT OF RANGE EXCEPTION CAUGHT AT CODEINDEX " << codeIndex << endl;
                            exit(-1);
                        }
                        codeIndex += c->amount;
                        status = moveToLeftCommand(c->amount);
                        if (!status){
                            return 1;
                        }
                        break;
                    }
                    case '+':
                    {
                        ComboCommand *c;
                        try{
                            c = combo.at(codeIndex);
                        }
                        catch(out_of_range e){
                            cout << "OUT OF RANGE EXCEPTION CAUGHT AT CODEINDEX " << codeIndex << endl;
                            exit(-1);
                        }
                        codeIndex += c->amount;
                        incrementCommand(c->amount);
                        break;
                    }
                    case '-':
                    {
                        ComboCommand *c;
                        try{
                            c = combo.at(codeIndex);
                        }
                        catch(out_of_range e){
                            cout << "OUT OF RANGE EXCEPTION CAUGHT AT CODEINDEX " << codeIndex << endl;
                            exit(-1);
                        }
                        codeIndex += c->amount;
                        decrementCommand(c->amount);
                        break;
                    }
                    case '.':
                    {
                        ComboCommand *c;
                        try{
                            c = combo.at(codeIndex);
                        }
                        catch(out_of_range e){
                            cout << "OUT OF RANGE EXCEPTION CAUGHT AT CODEINDEX " << codeIndex << endl;
                            exit(-1);
                        }
                        codeIndex += c->amount;
                        outputCommand(c->amount);
                        break;
                    }
                    case ',':
                        status = inputCommand();
                        if (!status){
                            return 1;
                        }
                        ++codeIndex;
                        break;
                    case '[':
                        {
                            int indexToGo = leftBracketCommand(codeIndex);
                            if (indexToGo == -2){
                                return 1;
                            }
                            else if (indexToGo != -1){
                                codeIndex = indexToGo + 1;
                            }
                            else{
                                ++codeIndex;
                            }
                            break;
                        }
                    case ']':
                        {
                            int indexToGo = rightBracketCommand(codeIndex);
                            if (indexToGo == -2){
                                return 1;
                            }
                            else if (indexToGo != -1){
                                codeIndex = indexToGo + 1;
                            }
                            else{
                                ++codeIndex;
                            }
                            break;
                        }
                    default:
                        break;  
                }
                if (codeIndex == code.size()){
                    run = false;
                }
            }


            return 0;

        }

    void combineCommand(ComboCommand *&current, int currentIndex, char type, bool otherType){
        if (otherType){
            if (current != NULL){
                combo[current->index] = current;
                current = NULL;
            }
        }
        else if (current == NULL){
            current = new ComboCommand(currentIndex, type);
        }
        else{
            char typeOfCC = current->type;
            if (typeOfCC != type){
                combo[current->index] = current;
                current = new ComboCommand(currentIndex, type);
            }
            else{
                ++current->amount;
            }
        }
    }

    bool moveToRightCommand(int amountToMove){
        if (dataPointerIndex != SIZE - 1){
            dataPointer += amountToMove;
            dataPointerIndex += amountToMove;
            if (dataPointerIndex >= SIZE){
                cout << "\nGoing out of bounds!!! (Index above " << SIZE << "\n" << endl;
                return false;
            }
            return true;
        }
        else{
            cout << "\nGoing out of bounds!!! (Index above " << SIZE << "\n" << endl;
            return false;
        }
    }

    bool moveToLeftCommand(int amountToMove){
        if (dataPointerIndex != 0){
            dataPointer -= amountToMove;
            dataPointerIndex -= amountToMove;
            if (dataPointerIndex < 0){
                cout << "\nGoing out of bounds!!! (Index below 0)\n" << endl;
                return false;
            }
            return true;
        }
        else{
            cout << "\nGoing out of bounds!!! (Index below 0)\n" << endl;
            return false;
        }
    }

    void incrementCommand(int amountToIncrease){
        *dataPointer = (*dataPointer + amountToIncrease) % 256;
    }

    void decrementCommand(int amountToDecrease){
        *dataPointer = (256 + (*dataPointer - amountToDecrease)) % 256;
    }

    void outputCommand(int amountOfTimes){
        for (int i = 0; i < amountOfTimes; ++i){
            cout << *dataPointer;
        }

    }

    bool inputCommand(){
        string input;

        cin >> input;

        if (input.size() > 0){
            *dataPointer = input.at(0);
            return true;
        }


        cout << "\nError while taking input\n" << endl;

        return false;
        
    }

    int leftBracketCommand(int currentIndex){  //combines both into one
        if (*dataPointer != 0){
            return -1;
        }

        int otherBrack = pairs.at(currentIndex);

        if (otherBrack == -1){
            cout << "\nError occured while looping\n" << endl;
            return -2;
        }
        else{
            return otherBrack;
        }

    }

    int rightBracketCommand(int currentIndex){
        if (*dataPointer == 0){
            return -1;
        }

        
        int otherBrack = pairs.at(currentIndex);

        if (otherBrack == -1){
            cout << "\nError occured while looping\n" << endl;
            return -2;
        }
        else{
            return otherBrack;
        }
    }


    private:
        unordered_map<int,int> pairs;
        unordered_map<int, ComboCommand*> combo;
        string fileName;
        bool overFlowFlag;
        vector<unsigned char> cells;
        unsigned char *dataPointer;
        int dataPointerIndex;
        int SIZE = 30000;
};