#pragma once
#include <vector>
#include <iostream>
#include <functional>
#include "sytax_tree.h"

namespace compiler {
    void syntax_tree::generateIntermediateCode() {
        std::vector<std::string> intermediateCode; // 存储中间代码的容器
        std::function<void(syntax_node*)> generateCode;
        
        std::function<void(syntax_node*)> traverseAndGenerateCode = [&](syntax_node* node) {
        // 遍历每个子节点
        for (auto child : node->ne) {
            // 对每个子节点调用generateCode
            generateCode(child);
        }
        };

        std::function<std::string(syntax_node*)> generateExpression = [&](syntax_node* node) {
            if (node->ne.size() == 1) {
                return node->ne.front()->value;
            } else {
                std::string left = generateExpression(node->ne.front());
                std::string op = node->value;
                std::string right = generateExpression(node->ne.back());
                return "(" + left + " " + op + " " + right + ")";
            }
        };

        std::function<void(syntax_node*)> generateCode = [&](syntax_node* node) {
            if (node->value == "read" && node-> id == 1) {
                // 生成读取输入的中间代码
                std::string code = "read " + node->ne.front()->value;
                intermediateCode.push_back(code);
            }
            else if (node->value == "if" && node-> id == 1) {
                // 生成条件语句的中间代码
                std::string condition = generateExpression(node->ne.front());
                std::string trueLabel = "label" + std::to_string(intermediateCode.size() + 1);
                std::string endLabel = "label" + std::to_string(intermediateCode.size() + 3);
                std::string code = "if " + condition + " goto " + trueLabel;
                intermediateCode.push_back(code);
                generateCode(node->ne.back());
                code = "goto " + endLabel;
                intermediateCode.push_back(code);
                code = trueLabel + ":";
                intermediateCode.push_back(code);
                traverseAndGenerateCode(node->ne.back());
                code = endLabel + ":";
                intermediateCode.push_back(code);
            }
            else if (node->value == "repeat") {
                // 生成循环语句的中间代码
                std::string startLabel = "label" + std::to_string(intermediateCode.size() + 1);
                std::string endLabel = "label" + std::to_string(intermediateCode.size() + 2);
                std::string code = startLabel + ":";
                intermediateCode.push_back(code);
                traverseAndGenerateCode(node->ne.back());
                std::string condition = generateExpression(node->ne.front());
                code = "if " + condition + " goto " + endLabel;
                intermediateCode.push_back(code);
                code = "goto " + startLabel;
                intermediateCode.push_back(code);
                code = endLabel + ":";
                intermediateCode.push_back(code);
            }
            else if (node->value == ":=" && node -> id == 2) {
                // 生成赋值语句的中间代码
                std::string leftOperand = node->ne.front()->value;
                std::string rightOperand = generateExpression(node->ne.back());
                std::string code = leftOperand + " := " + rightOperand;
                intermediateCode.push_back(code);
            }
            else if (node->value == "+" && node -> id == 2) {
                // 生成加法语句的中间代码
                std::string leftOperand = node->ne.front()->value;
                std::string rightOperand = generateExpression(node->ne.back());
                std::string code = leftOperand + " + " + rightOperand;
                intermediateCode.push_back(code);
            }
            else if (node->value == "-" && node -> id == 2) {
                // 生成减法语句的中间代码
                std::string leftOperand = node->ne.front()->value;
                std::string rightOperand = generateExpression(node->ne.back());
                std::string code = leftOperand + " - " + rightOperand;
                intermediateCode.push_back(code);
            }
            else if (node->value == "*" && node -> id == 2) {
                // 生成减法语句的中间代码
                std::string leftOperand = node->ne.front()->value;
                std::string rightOperand = generateExpression(node->ne.back());
                std::string code = leftOperand + " * " + rightOperand;
                intermediateCode.push_back(code);
            }
            else if (node->value == "write" && node -> id == 1) {
                // 生成输出语句的中间代码
                std::string operand = generateExpression(node->ne.front());
                std::string code = "write " + operand;
                intermediateCode.push_back(code);
            }
        };

        traverseAndGenerateCode(head); // 从根节点开始生成中间代码

        // 打印生成的中间代码
        for (const auto& code : intermediateCode) {
            std::cout << code << std::endl;
        }
    }
}
