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

  //find all the cycle-free winning paths

  //determine story depth
  //story.cal_depth();
  story.dfs();
  story.print_win_paths();
  return EXIT_SUCCESS;
}
