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

struct NodeExpr
{
    std::variant<NodeIntLit, NodeBinaryExprPlus, NodeBinaryExprMinus, NodeBinaryExprTimes, NodeGroupedExpr, NodeBinaryExprDivision, NodeExprIdentifier, NodeExprPow> node;    
};

struct NodeStmtExit{
    NodeExpr expr;
};

struct NodeStmtVar{
    Token identifier;
    NodeExpr expr;
};

struct NodeStmtPow {
    NodeExpr base;
    NodeExpr exponent;
};


struct NodeStmt{
    std::variant<NodeStmtExit, NodeStmtVar, NodeStmtPow> node;
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
                auto base = parsePrimaryExpression();
                if (!base) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::COMMA)) {
                    return {};
                }
                consume();

                auto exponent = parsePrimaryExpression();
                if (!exponent) return {};

                if (!(peak().has_value() && peak().value().type == TokenType::CLOSEPAREN)) {
                    return {};
                }
                consume();
                node_expr = NodeExpr{ NodeExprPow{std::make_shared<NodeExpr>(base.value()), std::make_shared<NodeExpr>(exponent.value())} };
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
                if (peak().value().type == TokenType::INT_LIT) {
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
            else if (peak().has_value() && peak().value().type == TokenType::VAR){
                consume();
                if (peak().has_value() && peak().value().type == TokenType::IDENTIFIER){
                    auto stmt_var = NodeStmtVar{consume()};
                    if (peak().has_value() && peak().value().type == TokenType::EQUALS){
                        consume();
                        if (auto expr = parseExpression()){
                            stmt_var.expr = expr.value();
                            return NodeStmt{ NodeStmtVar{stmt_var.identifier, stmt_var.expr} };
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
