#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
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
  //std::map<size_t, size_t> option_pagenum;
  std::vector<size_t> option_pagenums;

 public:
  //explicit Page(int n) : page_num(n) {}
  Page() : depth(std::numeric_limits<unsigned int>::max()) {}
  explicit Page(char * filename) :
      depth(std::numeric_limits<unsigned int>::max()),
      path(filename) {}
  unsigned int get_depth() { return depth; }
  void set_depth(unsigned int d) { depth = d; }
  std::vector<std::string> & get_navigator() { return navigator; }
  std::vector<size_t> & get_option_pagenums() { return option_pagenums; }
  size_t get_num_options() { return option_pagenums.size(); }
  size_t get_next_page_num(size_t option_num) {
    //std::cout << "in Page class: get_next_page_num():\n";
    return option_pagenums[option_num - 1];
  }
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
      std::cerr << "Page number invalid: No digits were found\n";
      exit(EXIT_FAILURE);
    }
    if (*endptr != '\0') {
      std::cerr << "Page number invalid: num + char\n";
      exit(EXIT_FAILURE);
    }
    return page_num;
  }
  //reference: string::find in cplusplus.com
  std::size_t extract_str(std::string str, std::string remove_part) {
    std::size_t found = str.find(remove_part);
    if (found == std::string::npos) {
      std::cerr << "invalid: not found remove_part\n";
      exit(EXIT_FAILURE);
    }
    return found;
  }
  void check_path_format() {
    //store path
    std::string former_part("/page");
    std::string latter_part(".txt");
    std::size_t found = extract_str(path, former_part);
    std::size_t found_tail = path.find(latter_part);
    std::string num = path.substr(found + former_part.length(),
                                  found_tail - found - former_part.length());
    page_num = check_page_num(num);
  }
  size_t store_navigator(std::vector<std::string> & lines) {
    //store lines into page
    size_t line_slash;
    if (!lines[0].compare("WIN") || !lines[0].compare("LOSE")) {
      //std::cout << "win page\n\n";
      navigator.push_back(lines[0]);
      if (lines[1][0] != '#') {
        std::cerr << "lack slash\n";
        exit(EXIT_FAILURE);
      }
      line_slash = 1;
    }
    else {
      bool slash = false;
      for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i][0] != '#') {
          //store option_pagenum
          std::size_t found = extract_str(lines[i], ":");
          std::string option = lines[i].substr(0, found);
          option_pagenums.push_back(check_page_num(option));
          navigator.push_back(lines[i].substr(found + 1));
        }
        else {
          slash = true;
          line_slash = i;
          break;
        }
      }
      if (slash == false) {
        std::cerr << "No slash\n";
        exit(EXIT_FAILURE);
      }
    }
    return line_slash;
  }
  void store_page() {
    check_path_format();
    //std::cout << "after check_path_format\n";
    //reference: MLP079_sort_cpp
    std::ifstream file;
    //reference: string::c_str in cplusplus.com
    char * cpath = new char[path.length() + 1];
    std::strcpy(cpath, path.c_str());  //remember to delete[]
    file.open(cpath);
    delete[] cpath;
    if (file.is_open()) {
      std::string line;
      std::vector<std::string> lines;
      while (std::getline(file, line)) {
        lines.push_back(line);
      }
      size_t line_slash;
      line_slash = store_navigator(lines);
      for (size_t i = line_slash + 1; i < lines.size(); i++) {
        text.push_back(lines[i]);
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
    for (size_t i = 0; i < page.navigator.size(); i++) {
      stream << " " << i + 1 << ". ";
      stream << page.navigator[i] << std::endl;
    }
  }
  return stream;
}
