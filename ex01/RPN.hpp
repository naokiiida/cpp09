#ifndef RPN_HPP
# define RPN_HPP

#include <string>
#include <stack>

class RPN
{
    public:
        RPN(void);
        RPN(const RPN& other);
        RPN &operator=(const RPN &other);
        ~RPN();
        RPN(const std::string& expression);
        int calculate();
    private:
    	std::string _expression;
        std::stack<long> _stack;
        static bool isValidNumber(const char& token);
};

#endif
