#include "page.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: program inputFile\n";
    return EXIT_FAILURE;
  }

  //store and print page
  Page page(argv[1]);
  page.store_page();
  std::cout << page;
  return EXIT_SUCCESS;
}
