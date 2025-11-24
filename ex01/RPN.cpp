#include "RPN.hpp"
#include <stdexcept>

// Default constructor
RPN::RPN(void) {}

// Copy constructor
RPN::RPN(const RPN &other)
{
    (void) other;
}

// Assignment operator overload
RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
    {
        (void) other;
    }
    return (*this);
}

// Destructor
RPN::~RPN(void)
{
}

RPN::RPN(const std::string& expression) : _expression(expression)
{
}

bool RPN::isValidNumber(const char& token)
{
    return token >= '0' && token <= '9';
}

int RPN::calculate()
{
    for (size_t i = 0; i < _expression.length(); i++)
    {
        char c = _expression[i];
        if (isValidNumber(c))
        {
            _stack.push(c - '0');
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            if (_stack.size() < 2)
            {
                throw std::runtime_error("Invalid expression");
            }
            int b = _stack.top();
            _stack.pop();
            int a = _stack.top();
            _stack.pop();
            int result = 0;
            switch (c)
            {
                case '+':
                    result = a + b;
                    break;
                case '-':
                    result = a - b;
                    break;
                case '*':
                    result = a * b;
                    break;
                case '/':
                    if (b == 0)
                    {
                        throw std::runtime_error("Division by zero");
                    }
                    result = a / b;
                    break;
            }
            _stack.push(result);
        }
    }
    if (_stack.size() != 1)
    {
        throw std::runtime_error("Invalid expression");
    }
    return _stack.top();
}
