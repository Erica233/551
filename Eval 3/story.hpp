#ifndef __STORY_H__
#define __STORY_H__

#include <algorithm>
#include <queue>
#include <set>
#include <stack>

#include "page.hpp"

class Story {
 private:
  std::string story_name;
  std::vector<size_t> win_page_num;
  std::vector<size_t> lose_page_num;
  std::vector<Page> pages_vec;
  std::set<size_t> valid_page_num;
  std::vector<std::vector<size_t> > neighbors;
  std::set<std::vector<size_t> > win_paths;

 public:
  Story(char * dir) : story_name(dir) {}
  void print_depth();
  size_t find_choice(size_t curr_pagenum, size_t next_pagenum);
  void print_win_paths();
  template<typename T>
  T & peek(std::stack<T> & stack) {
    return stack.top();
  }
  template<typename T>
  T & peek(std::queue<T> & queue) {
    return queue.front();
  }
  template<typename WorkList>
  void search();
  template<typename T>
  T & last(std::queue<T> & queue) {
    return queue.back();
  }
  template<typename T>
  T & last(std::stack<T> & stack) {
    return stack.top();
  }
  std::vector<Page> & get_pages_vec() { return pages_vec; }
  bool is_win_page(size_t n);
  bool is_end_page(size_t n) const;
  Page & get_page_n(size_t n) { return pages_vec[n - 1]; }
  size_t get_story_size() { return pages_vec.size(); }
  bool is_valid_choice(std::string num, size_t page_num) const;
  void check_valid_story();
  bool check_page_available(int page_num, char * cfilename);
  void store_story();
  friend std::ostream & operator<<(std::ostream & stream, const Story & story);
};

// Search algorithm.
// Breadth-first search uses queue, calculates the story depth of each page.
// Depth-first uses stack, gets all the cycle-free winning path.
template<typename WorkList>
void Story::search() {
  //reference: psuedo-code in AOP Chapter 25.3.3
  size_t i = 1;
  std::set<size_t> visited;
  WorkList todo;
  std::vector<size_t> path;
  path.push_back(i);
  pages_vec[i - 1].set_depth(0);
  // Add the start path
  todo.push(path);
  int n = 0;
  while (todo.size() != 0) {
    n++;
    std::vector<size_t> curr_path = peek(todo);
    todo.pop();
    size_t curr_pagenum = curr_path[curr_path.size() - 1];
    if (is_win_page(curr_pagenum)) {
      win_paths.insert(curr_path);
    }
    if (visited.find(curr_pagenum) == visited.end()) {
      visited.insert(curr_pagenum);
      // Check each neighbor of current page, and update informations.
      for (size_t j = 0; j < neighbors[curr_pagenum - 1].size(); j++) {
        if (pages_vec[neighbors[curr_pagenum - 1][j] - 1].get_depth() >
            pages_vec[curr_pagenum - 1].get_depth() + 1) {
          pages_vec[neighbors[curr_pagenum - 1][j] - 1].set_depth(
              pages_vec[curr_pagenum - 1].get_depth() + 1);
        }
        todo.push(curr_path);
        last(todo).push_back(neighbors[curr_pagenum - 1][j]);
      }
    }
  }
}

#endif
