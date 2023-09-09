#pragma once
#include <vector>
#include <optional>
#include <string>
#include "Tokenizer.hpp"
#include <variant>
#include <memory>

struct NodeIntLit
{
    Token token;
};
struct NodeFloatLit
{
    Token token;
};


struct NodeExpr;

struct NodeBinaryExprPlus
{
    Token token;
    std::shared_ptr<NodeExpr> left;
    std::shared_ptr<NodeExpr> right;
};

struct NodeBinaryExprMinus
{
    Token token;
    std::shared_ptr<NodeExpr> left;
    std::shared_ptr<NodeExpr> right;
};

struct NodeBinaryExprTimes
{
    Token token;
    std::shared_ptr<NodeExpr> left;
    std::shared_ptr<NodeExpr> right;
};

struct NodeBinaryExprDivision
{
    Token token;
    std::shared_ptr<NodeExpr> left;
    std::shared_ptr<NodeExpr> right;
};


struct NodeGroupedExpr
{
    std::shared_ptr<NodeExpr> innerExpr;
};

struct NodeExprIdentifier
{
    Token token;
};

struct NodeExprPow{
    std::shared_ptr<NodeExpr> base;
    std::shared_ptr<NodeExpr> exponent;
};

struct NodeExprSqrt{
    std::shared_ptr<NodeExpr> base;
};

struct NodeExprSin{
    std::shared_ptr<NodeExpr> base;
};

struct NodeExprCos{
    std::shared_ptr<NodeExpr> base;
};

struct NodeExprTan{
    std::shared_ptr<NodeExpr> base;
};

struct NodeExprLog
{
    std::shared_ptr<NodeExpr> base;
    std::shared_ptr<NodeExpr> exponent;
};

struct NodeExprLn
{
    std::shared_ptr<NodeExpr> base;
};


struct NodeExpr
{
    std::variant<NodeIntLit, NodeBinaryExprPlus, NodeBinaryExprMinus, 
                NodeBinaryExprTimes, NodeGroupedExpr, NodeBinaryExprDivision, 
                NodeExprIdentifier, NodeExprPow, NodeExprSqrt,
                NodeExprSin, NodeExprCos, NodeExprTan,
                NodeExprLog, NodeExprLn
                > node;    
};

struct NodeStmtExit{
    NodeExpr expr;
};

struct NodeStmtVarINT{
    Token identifier;
    NodeExpr expr;
};

struct NodeStmtVarFLOAT{
    Token identifier;
    NodeExpr expr;
};


struct NodeStmtPow {
    NodeExpr base;
    NodeExpr exponent;
};


struct NodeStmt{
    std::variant<NodeStmtExit, NodeStmtVarINT, NodeStmtPow, NodeStmtVarFLOAT> node;
};

struct Node
{
    std::vector<NodeStmt> node;
};


class Parser {
    public:
        Parser(std::vector<Token> tokens) : tokens(move(tokens)) {};

        std::optional<Node> parse() {
            Node node;
            while (peak().has_value())
            {
                if (auto expr = parseStatement()) {
                    node.node.push_back(expr.value());
                }
                else {
                    exit(EXIT_FAILURE);
                }
            }

            return node;
        };


        std::optional<NodeExpr> parseExpression() {
            std::optional<NodeExpr> node_expr;

            if (peak().has_value() && peak().value().type == TokenType::POW && peak(1).value().type == TokenType::OPENPAREN) {
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::COMMA)) {
                    return {};
                }
                consume();

                auto exponent = parseExpression();
                if (!exponent) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprPow{std::make_shared<NodeExpr>(base.value()), std::make_shared<NodeExpr>(exponent.value())} };
            }
            else if (peak().has_value() && peak().value().type == TokenType::SQRT && peak(1).value().type == TokenType::OPENPAREN){
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};
                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprSqrt{std::make_shared<NodeExpr>(base.value())} };
            }
            else if (peak().has_value() && peak().value().type == TokenType::SIN && peak(1).value().type == TokenType::OPENPAREN){
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};
                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprSin{std::make_shared<NodeExpr>(base.value())} };
            }
            else if (peak().has_value() && peak().value().type == TokenType::COS && peak(1).value().type == TokenType::OPENPAREN){
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};
                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprCos{std::make_shared<NodeExpr>(base.value())} };
            }
            else if (peak().has_value() && peak().value().type == TokenType::TAN && peak(1).value().type == TokenType::OPENPAREN){
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};
                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprTan{std::make_shared<NodeExpr>(base.value())} };
            }
            else if (peak().has_value() && peak().value().type == TokenType::LOG && peak(1).value().type == TokenType::OPENPAREN) {
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::COMMA)) {
                    return {};
                }
                consume();

                auto exponent = parseExpression();
                if (!exponent) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprLog{std::make_shared<NodeExpr>(base.value()), std::make_shared<NodeExpr>(exponent.value())} };
            }
            else if (peak().has_value() && peak().value().type == TokenType::LN && peak(1).value().type == TokenType::OPENPAREN) {
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprLn{std::make_shared<NodeExpr>(base.value())} };
            }
            else {
                node_expr = parsePrimaryExpression();
                if (!node_expr) return {};
            }


            while (true) {
                if (peak().has_value() && peak().value().type == TokenType::PLUS_OP) {
                    Token op = consume(); 
                    auto right = parsePrimaryExpression();
                    if (!right) return {}; 

                    node_expr = NodeExpr{ NodeBinaryExprPlus{op, std::make_shared<NodeExpr>(node_expr.value()), std::make_shared<NodeExpr>(right.value())} };
                } 
                else if (peak().has_value() && peak().value().type == TokenType::MINUS_OP){
                    Token op = consume();
                    auto right = parsePrimaryExpression();
                    if (!right) return {}; 

                    node_expr = NodeExpr{ NodeBinaryExprMinus{op, std::make_shared<NodeExpr>(node_expr.value()), std::make_shared<NodeExpr>(right.value())} };

                }
                else if (peak().has_value() && peak().value().type == TokenType::TIMES_OP){
                    Token op = consume();
                    auto right = parsePrimaryExpression();
                    if (!right) return {}; 

                    node_expr = NodeExpr{ NodeBinaryExprTimes{op, std::make_shared<NodeExpr>(node_expr.value()), std::make_shared<NodeExpr>(right.value())} };

                }
                else if (peak().has_value() && peak().value().type == TokenType::DIVIDE_OP){
                    Token op = consume();
                    auto right = parsePrimaryExpression();
                    if (!right) return {}; 

                    node_expr = NodeExpr{ NodeBinaryExprDivision{op, std::make_shared<NodeExpr>(node_expr.value()), std::make_shared<NodeExpr>(right.value())} };

                }

                else {
                    break;
                }
            }

            return node_expr;
        }   

        std::optional<NodeExpr> parseGroupedExpression() {
            if (peak().has_value() && peak().value().type == TokenType::OPENPAREN) {
                consume();  
                auto innerExpr = parseExpression();  
                if (!innerExpr) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume(); 

                return NodeExpr{ NodeGroupedExpr{std::make_shared<NodeExpr>(innerExpr.value())} };
            }
            return {};
        }

        std::optional<NodeExpr> parsePrimaryExpression() {
            if (peak().has_value()) {
                if (peak().value().type == TokenType::INT_LIT || peak().value().type == TokenType::FLOAT_LIT) {
                    return NodeExpr{ NodeIntLit{consume()} };
                } else if (peak().value().type == TokenType::OPENPAREN) {
                    return parseGroupedExpression();
                }
                else if (peak().value().type == TokenType::IDENTIFIER){
                    return NodeExpr{ NodeExprIdentifier{consume()} };
                }
            }
            return {};
        }   
        

        std::optional<NodeStmt> parseStatement() {
            if (peak().has_value() && peak().value().type == TokenType::END) {
                consume();
                NodeStmtExit node_stmt_exit;
                if (auto expr = parseExpression()) {
                    node_stmt_exit.expr = expr.value();
                }

                return NodeStmt{ NodeStmtExit{node_stmt_exit.expr} };
            }
            else if (peak().has_value() && peak().value().type == TokenType::INT){
                consume();
                if (peak().has_value() && peak().value().type == TokenType::IDENTIFIER){
                    auto stmt_var = NodeStmtVarINT{consume()};
                    if (peak().has_value() && peak().value().type == TokenType::EQUALS){
                        consume();
                        if (auto expr = parseExpression()){
                            stmt_var.expr = expr.value();
                            return NodeStmt{ NodeStmtVarINT{stmt_var.identifier, stmt_var.expr} };
                        }
                        else{
                            return {};
                        }
                    }
                }
            }
            else if (peak().has_value() && peak().value().type == TokenType::FLOAT){
                consume();
                if (peak().has_value() && peak().value().type == TokenType::IDENTIFIER){
                    auto stmt_var = NodeStmtVarFLOAT{consume()};
                    if (peak().has_value() && peak().value().type == TokenType::EQUALS){
                        consume();
                        if (auto expr = parseExpression()){
                            stmt_var.expr = expr.value();
                            return NodeStmt{ NodeStmtVarFLOAT{stmt_var.identifier, stmt_var.expr} };
                        }
                        else{
                            return {};
                        }
                    }
                }
            }
            else if (peak().has_value() && peak().value().type == TokenType::POW && peak(1).value().type == TokenType::OPENPAREN) {
                consume();
                consume();
                auto base = parseExpression();
                if (!base) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::COMMA)) {
                    return {};
                }
                consume();

                auto exponent = parseExpression();
                if (!exponent) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();

                return NodeStmt{ NodeStmtPow{base.value(), exponent.value()} };
            }
            else{
                return {};
            }
        }


    private:
        std::vector<Token> tokens;
        int index = 0;
        std::optional<Token> peak(int offset = 0) {
            if (index + offset >= tokens.size()) {
                return {};
            }
            return tokens[index + offset];
        }

        Token consume() {
            return tokens[index++];
        }
};
