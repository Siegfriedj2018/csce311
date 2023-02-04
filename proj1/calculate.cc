/* Copyright 2023 Siegfrij */

#include <proj1/inc/calculate.h>
#include <string>

namespace csce311 {

Calculate::Calculate(int size, char* input[]) {
    // readInExpression(size, (input));
    // calculateResult();
    // printResult();
}

void Calculate::readInExpression(int size, char* input[]) {
    for (int i = 0; i < size - 2; ++i) {
        operand_.push(atof(input[2*i + 1]));
        operators_.push(static_cast<char*>(input[2*i]));
    }
}

void Calculate::calculateResult() {
    int num1 = 0, num2 = 0;
    while (operators_.top() == "x") {
        operators_.pop();
        num1 = operand_.top();
        operand_.pop();
        num2 = operand_.top();
        operand_.pop();
        operand_.push(num1 * num2);
    }
}

void Calculate::printResult() {
    std::cout << operand_.top() << std::endl;
}


} // namespace311
