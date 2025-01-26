#include <iostream>
#include <string>
#include <regex>
#include <vector>
// #include "C:\Users\WarwickRen\Desktop\DS project\headers\lex.hpp"
// #include "..\headers\lex.hpp"
// using namespace l;
enum TokenType{
    STRING,
    NUMBER,
    SYMBOL, 
    IDENTIFIER,
    RESERVEDWORD
};

struct Token {
    TokenType type;
    std::string value;
    int hash;
};
bool typecmp(Token t1, Token t2){
    /* sorts the token list to create token table
    first all strings, then all numbers, then all symbols, 
    then all identifiers, then all reserved words*/
    if (t1.type!=t2.type){
        return t1.type < t2.type;}
        else{
            return t1.value < t2.value;
        }
}
std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokenList;
    // regular expressions:
    std::regex STRING_expr("\"[^\"]*\""); // two " marks and anything between them that isn't a " mark
    std::regex SYMBOL_expr("(\\(|\\)|\\||\\[|\\]|,|;|\\+|\\-|\\*|\\/|==|!=|>=|<=|<<|>>|>|<|=|\\{|\\})");
    std::regex RESERVEDWORD_expr("(int|float|void|return|if|while|cin|cout|continue|break|#include|using|iostream|namespace|std|main)");
    std::regex NUMBER_expr("[0-9]+"); //(digit)+
    std::regex IDENTIFIER_expr("[a-zA-Z][a-zA-Z0-9]*"); //letter(letter|digit)*
    // expr = STRING | RESERVEDWORD | NUMBER | IDENTIFIER | SYMBOL 
    std::regex expr("(\"[^\"]*\")|(int|float|void|return|if|while|cin|cout|continue|break|#include|using|iostream|namespace|std|main)|[0-9]+|[a-zA-Z][a-zA-Z0-9]*|(\\(|\\)|\\||\\[|\\]|,|;|\\+|\\-|\\*|\\/|==|!=|>=|<=|<<|>>|>|<|=|\\{|\\})");

    // iterate each expression
    std::sregex_iterator it(input.begin(), input.end(), expr); //1st expression
    std::sregex_iterator end; // last expression
    int pos=0;
    while (it!= end) {
        std::smatch match = *it;
        std::string match_str = match.str();
        if (std::regex_match(match_str, STRING_expr)) {
            tokenList.push_back({STRING, match_str,pos});
        } else if (std::regex_match(match_str, RESERVEDWORD_expr)) {
            tokenList.push_back({RESERVEDWORD, match_str,pos});
        } else if (std::regex_match(match_str, NUMBER_expr)) {
            tokenList.push_back({NUMBER, match_str,pos});
        } else if (std::regex_match(match_str, SYMBOL_expr)) {
            tokenList.push_back({SYMBOL, match_str,pos});
        } else if (std::regex_match(match_str, IDENTIFIER_expr)) {
            tokenList.push_back({IDENTIFIER, match_str,pos});
        } 
        it++,pos++;
        }
        return tokenList;
}