#include <algorithm>
#include <set>

#include "page.hpp"

class Story {
 private:
  std::string story_name;
  int win;
  int lose;
  std::vector<size_t> win_page_num;
  std::vector<size_t> lose_page_num;
  std::vector<Page> pages_vec;
  std::set<size_t> valid_page_num;

 public:
  Story(char * dir) : story_name(dir), win(0), lose(0) {}
  //Page& get_pages_vec(){return }
  std::vector<Page> & get_pages_vec() { return pages_vec; }
  bool is_end_page(size_t n) {
    std::vector<size_t>::iterator lose =
        std::find(lose_page_num.begin(), lose_page_num.end(), n);
    std::vector<size_t>::iterator win =
        std::find(win_page_num.begin(), win_page_num.end(), n);
    if (win != win_page_num.end() || lose != lose_page_num.end()) {
      return true;
    }
    return false;
  }
  Page & get_page_n(size_t n) { return pages_vec[n - 1]; }
  size_t get_story_size() { return pages_vec.size(); }
  void check_valid_story() {
    //    std::cout << "check_valid_story(): \n";
    //std::cout << "pages_vec.size(): " << pages_vec.size() << std::endl;
    valid_page_num.insert(1);
    for (size_t i = 0; i < pages_vec.size(); i++) {
      std::vector<std::string> & navigator = pages_vec[i].get_navigator();
      //std::cout << "page: " << i + 1;
      if (!navigator[0].compare("WIN")) {
        //std::cout << "win page\n\n";
        win = 1;
        win_page_num.push_back(i + 1);
      }
      else if (!navigator[0].compare("LOSE")) {
        //std::cout << "lose page\n\n";
        lose = 1;
        lose_page_num.push_back(i + 1);
      }
      else {
        //std::cout << "page with options: \n";
        for (size_t j = 0; j < navigator.size(); j++) {
          //reference: string::find in cplusplus.com
          std::size_t found;
          std::string option;
          found = navigator[j].find(":");
          if (found != std::string::npos) {
            option = navigator[j].substr(0, navigator[j].length() - found);
          }
          else {
            std::cerr << "Invalid page: option lack ':'\n";
            exit(EXIT_FAILURE);
          }
          size_t option_page_num;
          std::stringstream option_ss(option);
          option_ss >> option_page_num;
          //std::cout << "option page number: " << option_page_num << std::endl;
          if (0 < option_page_num && option_page_num <= pages_vec.size()) {
            //std::cout << option_page_num << " is a referenced page number\n";
            valid_page_num.insert(option_page_num);
          }
          else {
            //std::cout << option_page_num << "not referenced page number\n";
            std::cerr << "Invalid story: a option to a page not included\n";
            exit(EXIT_FAILURE);
          }
        }
      }
    }
    for (std::set<size_t>::iterator it = valid_page_num.begin();
         it != valid_page_num.end();
         ++it) {
      //std::cout << "valid_page_num: " << *it << " ";
    }
    if (valid_page_num.size() != pages_vec.size()) {
      std::cout << "pages_vec.size(): " << pages_vec.size() << std::endl;
      std::cout << "valid_page_num.size(): " << valid_page_num.size() << std::endl;
      std::cerr << "Invalid story: some pages not referenced by other page\n";
      exit(EXIT_FAILURE);
    }
    if (win != 1 || lose != 1) {
      std::cerr << "Invalid story: lack lose or win page\n";
      exit(EXIT_FAILURE);
    }
  }

  void store_story() {
    //std::cout << "store_story(): \nstory_name: " << story_name << "\nwin=" << win << " lose=" << lose << std::endl;
    int i = 1;
    std::string former_part("/page");
    std::string latter_part(".txt");
    while (1) {
      //std::cout << "\n-------------" << i << "th page: ----------------\n";
      std::stringstream num;
      num << i;
      std::string filename = story_name + former_part + num.str() + latter_part;
      //reference: string::c_str in cplusplus.com (remember to delete[])
      char * cfilename = new char[filename.length() + 1];
      std::strcpy(cfilename, filename.c_str());

      std::ifstream file;
      file.open(cfilename);

      if (!file.is_open()) {
        delete[] cfilename;
        if (i == 1) {
          std::cerr << "No page1.txt\n";
          exit(EXIT_FAILURE);
        }
        else {
          break;
        }
      }
      file.close();

      Page page(cfilename);
      page.store_page();
      delete[] cfilename;

      pages_vec.push_back(page);
      //std::cout << pages_vec[i - 1];

      i++;
    }
  }
};
