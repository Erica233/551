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
  void store_story() {
    std::cout << "store_story(): \nstory_name: " << story_name << "\nwin=" << win
              << " lose=" << lose << std::endl;
    int i = 1;
    std::string former_part("/page");
    std::string latter_part(".txt");
    while (1) {
      std::cout << "\n-------------" << i << "th page: ----------------\n";
      std::stringstream num;
      num << i;
      std::string filename = story_name + former_part + num.str() + latter_part;
      //reference: string::c_str in cplusplus.com (remember to delete[])
      char * cfilename = new char[filename.length() + 1];
      std::strcpy(cfilename, filename.c_str());

      std::ifstream file;
      file.open(cfilename);

      if (!file.is_open()) {
        std::cout << "failed to open\n";
        delete[] cfilename;
        if (i == 1) {
          //delete[] cfilename;
          std::cerr << "No page1.txt\n";
          exit(EXIT_FAILURE);
        }
        else {
          std::cout << "last page\n";
          break;
        }
      }
      file.close();

      Page page(cfilename);
      page.store_page();

      delete[] cfilename;

      pages_vec.push_back(page);
      std::cout << pages_vec[i - 1];

      i++;
    }
  }
};
