#pragma once
#include <string>
#include <vector>
#include <optional>
#include <iostream> 

enum class TokenType{
    INT_LIT,
    FLOAT_LIT,
    PLUS_OP,
    MINUS_OP,
    TIMES_OP,
    DIVIDE_OP,
    MOD,
    OPENPAREN,
    CLOSEPAREN,
    INT,
    FLOAT,
    IDENTIFIER,
    EQUALS,
    END,
    POW,
    COMMA,
    SQRT,
    COS,
    SIN,
    TAN,
    LOG,
    LN
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
                if (buffer == "int"){
                    tokens.push_back({TokenType::INT});
                    buffer.clear();
                }
                else if (buffer == "mod"){
                    tokens.push_back({TokenType::MOD});
                    buffer.clear();
                }
                else if (buffer == "float"){
                    tokens.push_back({TokenType::FLOAT});
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
                else if (buffer == "cos"){
                    tokens.push_back({TokenType::COS});
                    buffer.clear();
                }
                else if (buffer == "sin"){
                    tokens.push_back({TokenType::SIN});
                    buffer.clear();
                }
                else if (buffer == "tan"){
                    tokens.push_back({TokenType::TAN});
                    buffer.clear();
                }
                else if (buffer == "log"){
                    tokens.push_back({TokenType::LOG});
                    buffer.clear();
                }
                else if (buffer == "ln"){
                    tokens.push_back({TokenType::LN});
                    buffer.clear();
                }
                else {
                    tokens.push_back({TokenType::IDENTIFIER, buffer});
                    buffer.clear();
                }
            }
            else if (isdigit(peak().value())){
                buffer.push_back(consume());
                
                // Flag to determine if a '.' is encountered in the number
                bool isFloat = false;

                while(peak().has_value() && (isdigit(peak().value()) || peak().value() == '.')){
                    // If '.' is encountered, set the isFloat flag
                    if (peak().value() == '.') {
                        if (isFloat) {
                            std::cerr << "Error: Multiple '.' in number." << std::endl;
                            // Handle error as appropriate for your application
                        }
                        isFloat = true;
                    }
                    buffer.push_back(consume());
                }
                
                if (isFloat) {
                    tokens.push_back({TokenType::FLOAT_LIT, buffer});
                } else {
                    tokens.push_back({TokenType::INT_LIT, buffer});
                }
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