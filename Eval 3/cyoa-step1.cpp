#include "page.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: program inputFile\n";
    return EXIT_FAILURE;
  }
  //store and printpage
  Page page(argv[1]);
  page.store_story(argv[1]);
  std::cout << page;
  return EXIT_SUCCESS;
}
