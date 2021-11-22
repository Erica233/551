#include "story.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: program inputFile\n";
    return EXIT_FAILURE;
  }
  //store story
  Story story(argv[1]);
  story.store_story();
  story.check_valid_story();

  //determine story depth
  
  return EXIT_SUCCESS;
}
