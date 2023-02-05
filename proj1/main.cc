/* Copyright 2023 Siegfrij */

#include <proj1/calculate.h>

int main(int argc, char* argv[]) {
    std::cout << "Before" << std::endl;

    csce311::Calculate calculate(argc-1, (argv+1));
    std::cout << "Done" << std::endl;
    return 0;
}
