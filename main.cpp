#include <fstream>
#include ".\codes\lex.cpp"
int main(){
    std::ifstream input("input.txt"); 
    std::string line;

    if (input.is_open()) {
        // reading the input string
        std::string s="";
        while (getline(input, line)) {
            s=s+line;
        }
        // tokenizing the input string
        std::vector<Token> tokenz=tokenize(s);
        std::string tavaken[5]={"STRING","NUMBER", "SYMBOL","IDENTIFIER", "RESERVEDWORD"};
        std::cout << "Tokenized string:\n";
        for (const auto& token : tokenz) {
        std::cout << "[" << tavaken[token.type]<< ", " << token.value << "]" << std::endl;
        }
        // creating the token table
        std::vector<Token> tokenzSorted = tokenz;
        std::cout << "Token Table:\n";
        std::sort(tokenzSorted.begin(), tokenzSorted.end(),typecmp);
        for (const auto& token : tokenzSorted) {
            std::cout << "[" << tavaken[token.type]<< ", " << token.value <<  "\t h(k)= " << token.hash << "]" << std::endl;
        }





    input.close();
    } else {
        std::cerr << "Unable to open file input.txt" << std::endl;
    }
    return 0;
};
