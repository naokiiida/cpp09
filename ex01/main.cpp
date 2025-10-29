#include "RPN.hpp"
#include <iostream>

int main(int ac, char **av)
{
  if (ac != 2)
  {
    std::cerr << "Usage: " << av[0] << " <expression>\n";
    return 1;
  }

  try
  {
    RPN rpn(av[1]);
    std::cout << rpn.calculate() << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
  }

  return 0;
}
