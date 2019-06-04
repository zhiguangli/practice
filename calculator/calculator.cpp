#include "calculator.h"
#include <stack>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <assert.h>

/**
 * 去除字符串空格
 */

static void trim(std::string &s, char sep)
{
    size_t index = 0;
    if(!s.empty())
    {
        while( (index = s.find(sep, index)) != std::string::npos)
        {
            s.erase(index, 1);
        }
    }
}

Calculator::Calculator()
{

}

void Calculator::calculate(const std::string &expr)
{
    this->expr = expr;
    result = calculatePostExpression(toPostExpression(expr));
}

/**
 * 将表达式转换成后缀表达式。
 */
std::string Calculator::toPostExpression(const std::string &expr)
{
    std::string infixExpr = fillZeroToExpression(expr);
    trim(infixExpr, ' ');

    std::string postExpr;
    std::size_t i;
    char ch;
    std::stack<char> operators;

    for(i = 0; i < infixExpr.size(); ++i)
    {
        ch = infixExpr[i];

        while(std::isdigit(ch))
        {
           postExpr.push_back(ch);
           ++i;
           ch = infixExpr[i];
           if(!std::isdigit(ch))
           {
               postExpr.push_back(' ');
           }
        }
        
        switch (ch)
        {
            case ')':
                ch = operators.top();
                operators.pop();
                while(ch != '(')
                {
                    postExpr.push_back(ch);
                    postExpr.push_back(' ');
                    ch = operators.top();
                    operators.pop();
                }
                break;
            case '+':
            case '-':
                if(!operators.empty())
                {
                    do
                    {
                        ch = operators.top();
                        operators.pop();
                        if(ch == '(')
                        {
                            operators.push(ch);
                        }
                        else
                        {
                            postExpr.push_back(ch);
                            postExpr.push_back(' ');
                        }
                    }while(!operators.empty() && ch != '(');
                }
                operators.push(infixExpr[i]);
                break;
            case '*':
            case '/':
            case '(':
                operators.push(ch);
                break;
            case '\0':
                break;
            default:
                std::cerr << "input format error!\n";
                break;
        }
    }

    while(!operators.empty())
    {
        ch = operators.top();
        operators.pop();
        postExpr.push_back(ch);
        postExpr.push_back(' ');
    }

    return postExpr;
}

/**
 *计算后缀表达式
 */
std::string Calculator::calculatePostExpression(const std::string &postExpr)
{
    std::string value;
    double x;
    double y;
    std::stack<double> values;

    std::unordered_map<char, std::function<double(double, double)>> ops =
    {
        {'+', [](double x, double y) { return x + y;}},
        {'-', [](double x, double y) { return x - y;}},
        {'*', [](double x, double y) { return x * y;}},
        {'/', [](double x, double y) { assert(y != 0); return x / y;}}
    };

    for(size_t i = 0; i < postExpr.size(); ++i)
    {
        char ch = postExpr[i];
        value.clear();

        while(std::isdigit(ch) || ch == '.')
        {
            value += ch;
            ++i;
            ch = postExpr[i];
            if(ch == ' ')
            {
                values.push(std::stod(value));
            }
        }

        ch = postExpr[i];

        if(ch == '+' || ch == '-' || ch == '*' || ch == '/')
        {
            x = values.top(); values.pop();
            y = values.top(); values.pop();
            values.push(ops[ch](y, x));
        }
    }

    return std::to_string(values.top());
}

/**
 *  填充0， 如果遇到+，-表示符号位而不是操作符
 *  输入前:    8+(6-3)*(-5)+10/2
 *  输入后:    8+(6-3)*(0-5)+10/2
 */

std::string Calculator::fillZeroToExpression(const std::string &expr)
{
    std::string _expr = expr;
    for(auto it = _expr.begin(); it != _expr.end(); ++it)
    {
        char ch = *it;

        if(ch == '(')
        {
            switch(*(it + 1))
            {
            case '+':
            case '-':
                it = _expr.insert(it + 1, '0');
                break;
            }
        }
    }
    return _expr;
}
