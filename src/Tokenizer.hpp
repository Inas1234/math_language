#pragma once
#include <string>
#include <vector>
#include <optional>
#include <iostream> 

enum class TokenType{
    INT_LIT,
    PLUS_OP,
    MINUS_OP,
    TIMES_OP,
    DIVIDE_OP,
    OPENPAREN,
    CLOSEPAREN,
    VAR,
    IDENTIFIER,
    EQUALS,
    END,
    POW,
    COMMA,
    SQRT
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer{
public:
    Tokenizer(std::string input) : input(std::move(input)){}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens; 
        std::string buffer = "";
        
        while(peak().has_value()){
            if (isalpha(peak().value())){
                buffer.push_back(consume());
                while(peak().has_value() && isalnum(peak().value())){
                    buffer.push_back(consume());
                }
                if (buffer == "var"){
                    tokens.push_back({TokenType::VAR});
                    buffer.clear();
                }
                else if (buffer == "fin"){
                    tokens.push_back({TokenType::END});
                    buffer.clear();
                }
                else if (buffer == "pow"){
                    tokens.push_back({TokenType::POW});
                    buffer.clear();
                }
                else if (buffer == "sqrt"){
                    tokens.push_back({TokenType::SQRT});
                    buffer.clear();
                }
                else {
                    tokens.push_back({TokenType::IDENTIFIER, buffer});
                    buffer.clear();
                }
            }
            else if (isdigit(peak().value())){
                buffer.push_back(consume());
                while(peak().has_value() && isdigit(peak().value())){
                    buffer.push_back(consume());
                }
                tokens.push_back({TokenType::INT_LIT, buffer});
                buffer.clear();
            }
            else if (peak().value() == '+'){
                tokens.push_back({TokenType::PLUS_OP});
                consume();
            }
            else if (peak().value() == '-'){
                tokens.push_back({TokenType::MINUS_OP});
                consume();
            }
            else if (peak().value() == '*'){
                tokens.push_back({TokenType::TIMES_OP});
                consume();
            }
            else if (peak().value() == '/'){
                tokens.push_back({TokenType::DIVIDE_OP});
                consume();
            }
            else if (peak().value() == '('){
                tokens.push_back({TokenType::OPENPAREN});
                consume();
            }
            else if (peak().value() == ')'){
                tokens.push_back({TokenType::CLOSEPAREN});
                consume();
            }
            else if (peak().value() == '='){
                tokens.push_back({TokenType::EQUALS});
                consume();
            }
            else if (peak().value() == ','){
                tokens.push_back({TokenType::COMMA});
                consume();
            }
            else if (isspace(peak().value())){
                consume();
            }
            else {
                consume();
            }

        }    
        m_index = 0;
        return tokens;
    };

private:
    size_t m_index = 0;
    std::string input;

    std::optional<char> peak(int ahead = 0){
        if (m_index + ahead >= input.size()){
            return {};
        }
        return input[m_index + ahead];
    }

    char consume(){
        return input[m_index++];
    }
};