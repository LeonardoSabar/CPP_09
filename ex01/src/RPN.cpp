#include "../include/RPN.hpp"

rpn::rpn(){}

rpn::~rpn(){}

rpn::rpn(const rpn &other){
    this->_stack = other._stack;
}

rpn &rpn::operator=(const rpn &other){
    if (this != &other)
        this->_stack = other._stack;
    return *this;
}

void rpn::run(std::string expression){
    if (!checkInput(expression))
        return ;
    for (size_t i = 0; i < expression.size(); i++){
        if (expression[i] == ' ')
            continue;
        if (std::isdigit(expression[i]))
            this->_stack.push(expression[i] - '0');
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' 
            || expression[i] == '/'){
                if (this->_stack.size() < 2){
                    std::cerr << "Error: insuficiente number of operands." << std::endl;
                    return ;
                }
                calculate(expression[i]);
            }
        }
    std::cout << this->_stack.top() << std::endl;
}

void rpn::calculate(char op){
    int b = this->_stack.top(); this->_stack.pop();
    int a = this->_stack.top(); this->_stack.pop();

    switch (op)
    {
        case '+': 
            this->_stack.push(a + b);
            break;
        case '-': 
            this->_stack.push(a - b);
            break;
        case '*': 
            this->_stack.push(a * b);
            break;
        case '/': 
            if (b == 0)
                throw std::runtime_error("Error: Division by zero.");
            this->_stack.push(a / b);
            break;
    }
}

bool rpn::checkInput(std::string str){
    if (str.empty() || isOnlySpace(str)){
        std::cerr << "Error: empty input." << std::endl;
        return false;
    }
    std::istringstream iss(str);
    std::string token;
    while (iss >> token){
        if(token == "+" || token == "-" || token == "*" || token == "/")
            continue;
        if (!checkValidNumber(token))
            return false;
    }
    return true;
}

bool rpn::checkValidNumber(std::string str){
    std::istringstream iss(str);
    int num;
    if (!(iss >> num) && !iss.eof()){
        std::cerr << "Error: invalid operator '" << str << "'." << std::endl;
        return false;
    }
    if (num < 0 || num > 9){
        std::cerr << "Error: invalid number '" << str << "'." << std::endl;
        return false;
    }
    return true;
}

bool isOnlySpace(const std::string &str) {
    for (size_t i = 0; i < str.size(); ++i) {
        if (!std::isspace(str[i])) {
            return false;
        }
    }
    return true;
}