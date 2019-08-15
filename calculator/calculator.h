#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <iostream>

class Calculator
{
public:
    Calculator();

    void calculate(const std::string &expr);
    void output() const { std::cout << expr << " = " << result << std::endl; }
    std::string toPostExpression(const std::string &expr);
private:
    std::string calculatePostExpression(const std::string &postExpr);
    std::string fillZeroToExpression(const std::string &expr);
private:
    std::string expr;
    std::string result;
};

#endif // CALCULATOR_H
