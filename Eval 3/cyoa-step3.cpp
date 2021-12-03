#include "story.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: program inputFolder\n";
    return EXIT_FAILURE;
  }
  //store story
  Story story(argv[1]);
  story.store_story();
  story.check_valid_story();

  //calculate and print out story depth
  story.search<std::queue<std::vector<size_t> > >();
  story.print_depth();
  return EXIT_SUCCESS;
}
