/* Copyright 2023 Siegfrij */

#ifndef PROJ1_INC_CALCULATE_H_
#define PROJ1_INC_CALCULATE_H_

// includes here
#include <iostream>  // using std::cout, std::endl;
#include <string>    // using std::stof, std::string
#include <stack>    // using std::stack

namespace csce311 {

class Calculate {
  public:
    Calculate(int size, char* input[]);
    void readInExpression(int size, char* input[]);
    void calculateResult();
    void printResult();

  private:
    std::stack<double> operand_;
    std::stack<std::string> operators_;
};

}   // namespace csce311

#endif  // PROJ1_INC_CALCULATE_H_
