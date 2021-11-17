#include "story.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: program inputFile\n";
    return EXIT_FAILURE;
  }
  Story story(argv[1]);
  story.store_story();

  return EXIT_SUCCESS;
}
