#include <cstdlib>
#include <fstream>
#include <iostream>

#include "page.hpp"

int main(int argc, char ** argv) {
  //reference: MLP079
  if (argc != 2) {
    std::cerr << "Usage: program inputFile\n";
    return EXIT_FAILURE;
  }
  //store page
  Page page(argv[1]);
  
  return EXIT_SUCCESS;
}
