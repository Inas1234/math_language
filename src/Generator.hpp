#pragma once
#include <optional>
#include <string>
#include <sstream>
#include "Parser.hpp"

class Generator {
public:
    Generator(Node node) : node(std::move(node)) {}

    void gen_expr(const NodeExpr& node_expr) {
        struct ExprVisitor {
            Generator* generator;
            
            void operator()(const NodeIntLit& node_int_lit) {
                generator->m_output << node_int_lit.token.value.value();
            }

            void operator()(const NodeBinaryExprPlus& node_binary_expr_plus) {
                generator->gen_expr(*node_binary_expr_plus.left);
                generator->m_output << "+";
                generator->gen_expr(*node_binary_expr_plus.right);
            }
            void operator()(const NodeBinaryExprMinus& node_binary_expr_minus){
                generator->gen_expr(*node_binary_expr_minus.left);
                generator->m_output << "-";
                generator->gen_expr(*node_binary_expr_minus.right);
            }
            void operator()(const NodeBinaryExprTimes& node_binary_expr_times){
                generator->gen_expr(*node_binary_expr_times.left);
                generator->m_output << "*";
                generator->gen_expr(*node_binary_expr_times.right);
            }
            void operator()(const NodeGroupedExpr& node_grouped_expr){
                generator->m_output << "(";
                generator->gen_expr(*node_grouped_expr.innerExpr);
                generator->m_output << ")";
            }
            void operator()(const NodeBinaryExprDivision& node_binary_expr_division){
                generator->gen_expr(*node_binary_expr_division.left);
                generator->m_output << "/";
                generator->gen_expr(*node_binary_expr_division.right);
            }
        };

        std::visit(ExprVisitor{this}, node_expr.node);
    }

    std::string generate() {
        m_output << "#include <iostream>" << std::endl;
        m_output << "int main() {" << std::endl;
        m_output << "\tstd::cout << ";

        for (const auto& node_expr : node.node) {
            gen_expr(node_expr);
            m_output << " << std::endl;";
        }

        m_output << "\n\treturn 0;\n}" << std::endl;
        return m_output.str();
    }

private:
    Node node;
    std::stringstream m_output;
};
