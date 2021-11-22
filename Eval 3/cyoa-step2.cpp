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

  //begin_story()
  std::vector<Page> & pages_vec = story.get_pages_vec();
  std::cout << pages_vec[0];

  size_t page_num = 1;
  size_t choice_num;
  while (std::cin >> choice_num) {
    page_num = pages_vec[page_num - 1].get_next_page_num(choice_num);

    std::cout << pages_vec[page_num - 1];
    if (story.is_end_page(page_num)) {
      break;
    }

    //std::cout << story.get_page_n(page_num);
  }
  return EXIT_SUCCESS;
}
