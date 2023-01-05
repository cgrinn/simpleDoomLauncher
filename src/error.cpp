#include "error.h"
#include <iostream>

void fatalError(std::string errorMessage)
{
    std::cout << errorMessage << std::endl;
    exit(1);
}

void logError(std::string errorMessage)
{
    std::cout << errorMessage << std::endl;
}