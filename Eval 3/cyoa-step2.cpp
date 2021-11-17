//#include "page.hpp"
#include "story.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: program inputFile\n";
    return EXIT_FAILURE;
  }
  Story story(argv[1]);
  story.story_store(argv[1]);
  //store and printpage
  //Page page(argv[1]);
  //  page.store_story(argv[1]);
  //std::cout << page;
  return EXIT_SUCCESS;
}
