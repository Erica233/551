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

  //begin_story()
  story.start_story(std::cin);
  return EXIT_SUCCESS;
}
