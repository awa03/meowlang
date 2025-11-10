#ifndef DUMP_HPP
#define DUMP_HPP

#include "parser/node_types.hpp"
#include <iostream>
#include <string>
namespace utils {

using namespace parser;

inline void dump_ast(const statement* node, int indent = 0) {
    if (!node) return;
    
    std::string spacing(indent * 2, ' ');
    
    switch(node->kind) {
        case PROGRAM: {
            auto* prog = static_cast<const program*>(node);
            std::cout << spacing << "Program {\n";
            for (const auto& stmt : prog->body) {
                dump_ast(stmt.get(), indent + 1);
            }
            std::cout << spacing << "}\n";
            break;
        }
        
        case NUMERIC_LITERAL: {
            auto* lit = static_cast<const numeric_literal*>(node);
            std::cout << spacing << "NumericLiteral: " << lit->symbol << "\n";
            break;
        }
        
        case IDENTIFIER: {
            auto* id = static_cast<const identifier*>(node);
            std::cout << spacing << "Identifier: " << id->symbol << "\n";
            break;
        }
        
        case BINARY_EXP: {
            auto* bin = static_cast<const binary_exp*>(node);
            std::cout << spacing << "BinaryExpression: " << bin->op << " {\n";
            std::cout << spacing << "  left:\n";
            dump_ast(bin->left.get(), indent + 2);
            std::cout << spacing << "  right:\n";
            dump_ast(bin->right.get(), indent + 2);
            std::cout << spacing << "}\n";
            break;
        }

        default:
            std::cout << spacing << "Unknown Node Type: " << node->kind << "\n";
            break;
    }
}

inline void dump_program(const program& prog) {
    dump_ast(&prog);
}

} 

#endif
