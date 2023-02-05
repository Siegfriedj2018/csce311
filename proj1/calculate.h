/* Copyright 2023 Siegfrij */

#ifndef PROJ1_CALCULATE_H_
#define PROJ1_CALCULATE_H_

// includes here
#include <iostream>  // using std::cout, std::endl;
#include <string>    // using std::stof, std::string
#include <stack>     // using std::stack<double>, std::stack<char>
#include <deque>     // using std::deque

namespace csce311 {

class Calculate {
  public:
    Calculate(int size, char* input[]);
    void readInAndCalculate(int size, char* input[]);
    void multiplyDivide();
    void addSub();
    void printResult();

  private:
    std::deque<double> operand_;
    std::stack<char*> operators_;
};

}   // namespace csce311

#endif  // PROJ1_CALCULATE_H_
