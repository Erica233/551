#ifndef __PAGE_H__
#define __PAGE_H__

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
  std::vector<std::string> navigator;
  std::vector<std::string> text;
  std::vector<size_t> option_pagenums;

 public:
  Page() : depth(std::numeric_limits<unsigned int>::max()) {}
  explicit Page(char * filename) :
      depth(std::numeric_limits<unsigned int>::max()),
      path(filename) {}
  unsigned int get_depth() const { return depth; }
  void set_depth(unsigned int d) { depth = d; }
  std::vector<std::string> & get_navigator() { return navigator; }
  std::vector<size_t> & get_option_pagenums() { return option_pagenums; }
  size_t get_num_options() const { return option_pagenums.size(); }
  size_t get_next_page_num(size_t option_num) const {
    return option_pagenums[option_num - 1];
  }
  size_t check_page_num(std::string num);
  std::size_t extract_str(std::string str, std::string remove_part);
  void check_path_format();
  size_t store_navigator(std::vector<std::string> & lines);
  void store_page();
  friend std::ostream & operator<<(std::ostream & stream, const Page & page);
};

#endif
