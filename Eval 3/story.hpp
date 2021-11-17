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
    std::stringstream num;
    num << i;
    std::string filename = story_name + former_part + num.str() + latter_part;
    //reference: string::c_str in cplusplus.com (remember to delete[])
    char * cfilename = new char[filename.length() + 1];
    std::strcpy(cfilename, filename.c_str());

    Page page1(cfilename);
    page1.store_page();
    std::cout << page1;
    delete[] cfilename;
    //concate to page1.txt
    //open page1.txt
    /*
    while (1) {
      Page curr_page(filename);
      curr_page.store_page();
      pages_vec.push_back(curr_page);
      }*/
  }
};
fm1
