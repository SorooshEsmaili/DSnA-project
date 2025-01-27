#include <iostream>
#include <string>
#include <vector>
#include <regex>

enum TokenType {
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    COMMENT,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::regex identifier_regex("[a-zA-Z_][a-zA-Z_0-9]*");
    std::regex integer_regex("[0-9]+");
    std::regex float_regex("[0-9]*\\.[0-9]+");
    std::regex string_literal_regex("\"[^\"]*\"");
    std::regex comment_regex("//.*");

    std::sregex_iterator it(input.begin(), input.end(), identifier_regex);
    std::sregex_iterator end;

    while (it != end) {
        std::smatch match = *it;
        std::string match_str = match.str();
        if (std::regex_match(match_str, identifier_regex)) {
            tokens.push_back({IDENTIFIER, match_str});
        } else if (std::regex_match(match_str, integer_regex)) {
            tokens.push_back({INTEGER_LITERAL, match_str});
        } else if (std::regex_match(match_str, float_regex)) {
            tokens.push_back({FLOAT_LITERAL, match_str});
        } else if (std::regex_match(match_str, string_literal_regex)) {
            tokens.push_back({STRING_LITERAL, match_str.substr(1, match_str.size() - 2)});
        } else if (std::regex_match(match_str, comment_regex)) {
            tokens.push_back({COMMENT, match_str});
        } else {
            tokens.push_back({UNKNOWN, match_str});
        }
        it++;
    }

    return tokens;
}

int main() {
    std::string input = "int x = 3; float y = 3.14; \"Hello, world!\" // This is a comment";
    std::vector<Token> tokens = tokenize(input);

    for (const auto& token : tokens) {
        std::cout << "Type: " << token.type << ", Value: " << token.value << std::endl;
    }

    return 0;
}