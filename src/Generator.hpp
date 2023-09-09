#pragma once
#include <optional>
#include <string>
#include <sstream>
#include <unordered_map>
#include "Parser.hpp"

class Generator {
public:
    Generator(Node node) : node(std::move(node)) {}

    void gen_stmt(const NodeStmt& node_stmt){
        struct StmtVisitor{
            Generator* generator;

            void operator()(const NodeStmtExit& node_stmt_exit){
                generator->m_output << "\tstd::cout <<  ";
                generator->gen_expr(node_stmt_exit.expr);
                generator->m_output << " << std::endl;\n";
            }

            void operator()(const NodeStmtVarINT& node_stmt_var){
                generator->m_output << "\tint ";
                generator->m_output << node_stmt_var.identifier.value.value();
                generator->m_output << " = ";
                generator->gen_expr(node_stmt_var.expr);
                generator->m_output << ";\n";

                generator->m_vars[node_stmt_var.identifier.value.value()] = Var{node_stmt_var.identifier.value.value()};
            }  
            void operator()(const NodeStmtVarFLOAT& node_stmt_var){
                generator->m_output << "\tfloat ";
                generator->m_output << node_stmt_var.identifier.value.value();
                generator->m_output << " = ";
                generator->gen_expr(node_stmt_var.expr);
                generator->m_output << ";\n";

                generator->m_vars[node_stmt_var.identifier.value.value()] = Var{node_stmt_var.identifier.value.value()};
            }

            void operator()(const NodeStmtPow& node_stmt_pow){
                generator->m_output << "\tstd::pow(";
                generator->gen_expr(node_stmt_pow.base);
                generator->m_output << ", ";
                generator->gen_expr(node_stmt_pow.exponent);
                generator->m_output << ");\n";
            }

        };
        std::visit(StmtVisitor{this}, node_stmt.node);
    }

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
            void operator()(const NodeExprIdentifier& node_expr_identifier){
                generator->m_output << node_expr_identifier.token.value.value();
            }
            void operator()(const NodeExprPow& node_expr_pow){
                generator->m_output << "std::pow(";
                generator->gen_expr(*node_expr_pow.base);
                generator->m_output << ", ";
                generator->gen_expr(*node_expr_pow.exponent);
                generator->m_output << ")";
            }
            void operator()(const NodeExprSqrt& node_expr_sqrt){
                generator->m_output << "std::sqrt(";
                generator->gen_expr(*node_expr_sqrt.base);
                generator->m_output << ")";
            }
            void operator()(const NodeExprSin& node_expr_sin){
                generator->m_output << "std::sin(";
                generator->gen_expr(*node_expr_sin.base);
                generator->m_output << ")";
            }
            void operator()(const NodeExprCos& node_expr_cos){
                generator->m_output << "std::cos(";
                generator->gen_expr(*node_expr_cos.base);
                generator->m_output << ")";
            }
            void operator()(const NodeExprTan& node_expr_tan){
                generator->m_output << "std::tan(";
                generator->gen_expr(*node_expr_tan.base);
                generator->m_output << ")";
            }
            void operator()(const NodeExprLog& node_expr_log){
                generator->m_output << "customlog(";
                generator->gen_expr(*node_expr_log.base);
                generator->m_output << ", ";
                generator->gen_expr(*node_expr_log.exponent);
                generator->m_output << ")";
            }
            void operator()(const NodeExprLn& node_expr_ln){
                generator->m_output << "std::log(";
                generator->gen_expr(*node_expr_ln.base);
                generator->m_output << ")";
            }
        };

        std::visit(ExprVisitor{this}, node_expr.node);
    }

    std::string generate() {
        m_output << "#include <iostream>" << std::endl;
        m_output << "#include <cmath>" << std::endl;

        m_output << "double customlog(double base, double x) {" << std::endl;
        m_output << "\treturn std::log(x) / std::log(base);" << std::endl;
        m_output << "}\n" << std::endl;
        m_output << "int main() {" << std::endl;

        for (const auto& node_expr : node.node) {
            gen_stmt(node_expr);
        }

        m_output << "\n\treturn 0;\n}" << std::endl;
        return m_output.str();
    }

private:
    Node node;
    std::stringstream m_output;
    struct Var
    {
        std::string name;
    };
    std::unordered_map<std::string, Var> m_vars;
    
};
