/* Copyright 2023 Siegfrij */

#include <proj1/calculate.h>

namespace csce311 {


void Calculate::readInAndCalculate(int size, char* input[]) {
    // Loops through input and read operator and operand
    // if operator is of higher importance(x, /) it does that first
    // then does everything else(+, -)
    for (int i = 0; i < size; ++i) {
        if (atof(input[i]) == 0 && (*(input[i]) == 'x' || *(input[i]) == '/')) {
            operators_.push(input[i]);
            operand_.push_back(atof(input[i+1]));
            ++i;
            multiplyDivide();
            continue;
        } else if (atof(input[i]) == 0) {
            operators_.push(input[i]);
            continue;
        }
        operand_.push_back(atof(input[i]));
    }
    if (!operators_.empty()) {
        addSub();
    }
}

// multiplication and divide function
void Calculate::multiplyDivide() {
    double num1 = 0.0, num2 = 0.0;
    char operation = *(operators_.top());
    if (operation == 'x' || operation == '/') {
        operators_.pop();
        num1 = operand_.back();
        operand_.pop_back();
        num2 = operand_.back();
        operand_.pop_back();
        if (operation == '/') {
            if (num1 > num2) {
                operand_.push_back(num1 / num2);
            } else {
                operand_.push_back(num2 / num1);
            }
        } else {
            operand_.push_back(num1 * num2);
        }
    }
}

// adding and subtracting function
void Calculate::addSub() {
    double num1 = 0.0, num2 = 0.0;
    char operation = *(operators_.top());

    while (!operators_.empty()) {
        operators_.pop();
        num1 = operand_.front();
        operand_.pop_front();
        num2 = operand_.front();
        operand_.pop_front();
        if (operation == '+') {
            operand_.push_front(num1 + num2);
        } else {
            operand_.push_front(num1 - num2);
        }
    }
}

void Calculate::printResult() {
    std::cout << operand_.back() << std::endl;
}

}  // namespace csce311
