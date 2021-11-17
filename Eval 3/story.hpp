#include "page.hpp"

class Story {
 private:
  std::string story_name;
  int win;
  int lose;
  std::vector<Page> pages_vec;
  std::vector<int> valid_page_nums;

 public:
  Story(char * dir) : story_name(dir), win(0), lose(0) {}
  void store_story() {}
};
