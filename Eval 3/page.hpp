#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Page {
 private:
  unsigned int depth;
  size_t page_num;
  std::string path;
  //std::string dir_name;
  //std::string page_name;
  std::vector<std::string> navigator;
  std::vector<std::string> text;
  std::map<size_t, size_t> option_pagenum;

 public:
  //explicit Page(int n) : page_num(n) {}
  Page() : depth(std::numeric_limits<unsigned int>::max()) {}
  explicit Page(char * filename) :
      depth(std::numeric_limits<unsigned int>::max()),
      path(filename) {}
  unsigned int get_depth() { return depth; }
  void set_depth(unsigned int d) { depth = d; }
  std::vector<std::string> & get_navigator() { return navigator; }
  size_t get_next_page_num(size_t option_num) { return option_pagenum[option_num]; }
  size_t check_page_num(std::string num) {
    //reference: strtol in man page
    //reference:  my Eval 1 function check_int()
    char * endptr;
    errno = 0;
    size_t page_num = strtoul(num.c_str(), &endptr, 10);
    if ((errno == ERANGE && page_num == ULONG_MAX) || (page_num == 0 && errno != 0)) {
      std::cerr << "Page number invalid\n";
      exit(EXIT_FAILURE);
    }
    if (endptr == num.c_str()) {
      std::cerr << "No digits were found\n";
      exit(EXIT_FAILURE);
    }
    if (*endptr != '\0') {
      std::cerr << "Page number invalid\n";
      exit(EXIT_FAILURE);
    }
    return page_num;
  }
  void check_path_format() {
    //store path
    //reference: string::find in cplusplus.com
    //std::cout << "\nstore_page(): \npath: " << path << std::endl;
    std::size_t found;
    std::string num;
    std::string former_part("/page");
    std::string latter_part(".txt");
    found = path.find(former_part);
    //found = path.find("/path");
    if (found != std::string::npos) {
      //dir_name = path.substr(0, found);
      //page_name = path.substr(found + 1);
      //std::cout << "dir_name: " << dir_name << "\npage_name: " << page_name << std::endl;
    }
    else {
      std::cerr << "invalid input file name\n";
      exit(EXIT_FAILURE);
    }
    std::size_t found_tail = path.find(latter_part);
    if (found_tail != std::string::npos) {
      num = path.substr(found + former_part.length(),
                        found_tail - found - former_part.length());
      //std::cout << "num: " << num << std::endl;
    }
    else {
      std::cerr << "invalid input file name\n";
      exit(EXIT_FAILURE);
    }
    page_num = check_page_num(num);
  }
  void store_page() {
    check_path_format();
    //reference: MLP079_sort_cpp
    std::ifstream file;
    //reference: string::c_str in cplusplus.com
    char * cpath = new char[path.length() + 1];
    std::strcpy(cpath, path.c_str());  //remeber to delete[]
    file.open(cpath);
    delete[] cpath;
    if (file.is_open()) {
      std::string line;
      std::vector<std::string> lines;
      while (std::getline(file, line)) {
        lines.push_back(line);
      }
      //store lines into page
      size_t option_num = 1;
      std::vector<std::string>::iterator it = lines.begin();
      if (!it->compare("WIN") || !it->compare("LOSE")) {
        //std::cout << "win page\n\n";
        navigator.push_back(*it);
        ++it;
      }
      else {
        while ((*it)[0] != '#') {
          navigator.push_back(*it);
          //store option_pagenum

          //reference: string::find in cplusplus.com
          std::size_t found;
          std::string option;
          found = it->find(":");
          if (found != std::string::npos) {
            option = it->substr(0, it->length() - found);
          }
          else {
            std::cerr << "Invalid page: option lack ':'\n";
            exit(EXIT_FAILURE);
          }
          size_t option_page_num;
          std::stringstream option_ss(option);
          option_ss >> option_page_num;
          option_pagenum[option_num] = option_page_num;
          //std::cout << "option" << option_num << " - " << option_page_num << std::endl;

          option_num++;
          ++it;
        }
      }
      ++it;
      while (it != lines.end()) {
        text.push_back(*it);
        ++it;
      }
    }
    else {
      std::cerr << "Failed to open file\n";
      exit(EXIT_FAILURE);
    }
    file.close();
    //delete[] cpath;
  }

  friend std::ostream & operator<<(std::ostream & stream, const Page & page);
};

std::ostream & operator<<(std::ostream & stream, const Page & page) {
  //print story in the page
  std::vector<std::string>::const_iterator it = page.text.begin();
  while (it != page.text.end()) {
    stream << *it << std::endl;
    ++it;
  }
  stream << std::endl;
  //if WIN/LOSE page
  if (!page.navigator.begin()->compare("WIN")) {
    stream << "Congratulations! You have won. Hooray!\n";
  }
  else if (!page.navigator.begin()->compare("LOSE")) {
    stream << "Sorry, you have lost. Better luck next time!\n";
  }
  //print choices
  else {
    stream << "What would you like to do?\n";
    stream << std::endl;
    std::vector<std::string>::const_iterator jt = page.navigator.begin();
    int i = 1;
    while (jt != page.navigator.end()) {
      stream << " " << i << ". ";
      size_t j;
      for (j = 0; j < jt->length(); j++) {
        if ((*jt)[j] == ':') {
          break;
        }
      }
      j++;
      stream << jt->substr(j) << std::endl;
      i++;
      ++jt;
    }
  }
  return stream;
}
