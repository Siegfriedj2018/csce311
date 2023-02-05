/* Copyright 2023 Siegfrij */

#include <proj1/calculate.h>
#include <string>

namespace csce311 {

Calculate::Calculate(int size, char* input[]) {
    readInAndCalculate(size, (input));
    printResult();
}

void Calculate::readInAndCalculate(int size, char* input[]) {
    std::cout << "before" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cout << "Inside " << i << std::endl;
        if(atof(input[i]) == 0 && (*(input[i]) == 'x' || *(input[i]) == '/')) {
            operators_.push(input[i]);
            operand_.push_back(atof(input[i+1]));
            ++i;
            multiplyDivide();
            continue;
        } else if (atof(input[i]) == 0) {
            operators_.push(input[i]);
            continue;
        }
        std::cout << input[i] << std::endl;
        operand_.push_back(atof(input[i]));
        std::cout << "pushed on stack" << std::endl;
    }
    if (!operators_.empty()) {
        addSub();
    }
}

void Calculate::multiplyDivide() {
    double num1 = 0.0, num2 = 0.0;
    char operation = *(operators_.top());
    std::cout << "Inside MD" << std::endl;
    if (operation == 'x' || operation == '/') {
        operators_.pop();
        num1 = operand_.back();
        operand_.pop_back();
        num2 = operand_.back();
        operand_.pop_back();
        if (operation == '/') {
            std::cout << "Dividing: " << num1 << " / " << num2 << std::endl;
            if (num1 > num2) {
                operand_.push_back(num1 / num2);
            } else {
                operand_.push_back(num2 / num1);
            }// if error or wrong value swap num1 and num2
        } else {
            std::cout << "Multiplying: " << num1 << " x " << num2 << std::endl;
            operand_.push_back(num1 * num2);
        }
    }
}

void Calculate::addSub() {
    std::cout << "Inside addsub" << std::endl;
    double num1 = 0.0, num2 = 0.0;
    char operation = *(operators_.top());

    while (!operators_.empty()) {
        operators_.pop();
        num1 = operand_.front();
        operand_.pop_front();
        num2 = operand_.front();
        operand_.pop_front();
        std::cout << num1 << " " << operation << " " << num2 << std::endl;
        if (operation == '+') {
            std::cout << "Adding: " << num1 << " + " << num2 << std::endl;
            operand_.push_front(num1 + num2);
        } else {
            std::cout << "Subtracting: " << num1 << " - " << num2 << std::endl;
            operand_.push_front(num1 - num2);
        }
    }
}

void Calculate::printResult() {
    std::cout << operand_.back() << std::endl;
}


} // namespace311
