#include <algorithm>
#include <queue>
#include <set>
#include <stack>

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
  std::vector<std::vector<size_t> > neighbors;
  std::set<std::vector<size_t> > win_paths;

 public:
  Story(char * dir) : story_name(dir), win(0), lose(0) {}
  void printDepth() {
    for (size_t x = 0; x < pages_vec.size(); x++) {
      std::cout << "page " << x + 1 << "'s depth: " << pages_vec[x].get_depth() << "\n";
    }
  }
  void print_depth() {
    for (size_t i = 0; i < pages_vec.size(); i++) {
      std::cout << "Page " << i + 1;
      if (pages_vec[i].get_depth() == std::numeric_limits<unsigned int>::max()) {
        std::cout << " is not reachable\n";
      }
      else {
        std::cout << ":" << pages_vec[i].get_depth() << std::endl;
      }
    }
  }
  void print_path(std::vector<size_t> & curr_path) {
    std::cout << "curr_path: ";
    for (size_t i = 0; i < curr_path.size(); i++) {
      std::cout << curr_path[i] << ",";
    }
    std::cout << "\n";
  }
  void print_visited(std::set<size_t> & visited) {
    std::cout << "visited: ";
    for (std::set<size_t>::iterator it = visited.begin(); it != visited.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << "\n";
  }
  size_t find_choice(size_t curr_pagenum, size_t next_pagenum) {
    for (size_t i = 0; i < neighbors[curr_pagenum - 1].size(); i++) {
      if (next_pagenum == neighbors[curr_pagenum - 1][i]) {
        return i + 1;
      }
    }
    return 0;
  }

  void print_win_paths() {
    if (win_paths.size() == 0) {
      std::cout << "This story is unwinnable!\n";
    }
    for (std::set<std::vector<size_t> >::iterator it = win_paths.begin();
         it != win_paths.end();
         ++it) {
      for (size_t j = 0; j < it->size(); j++) {
        size_t curr_pagenum = (*it)[j];
        if (j + 1 == it->size()) {
          std::cout << (*it)[j] << "(win)";
          break;
        }
        size_t next_pagenum = (*it)[j + 1];
        size_t choice_num = find_choice(curr_pagenum, next_pagenum);
        std::cout << (*it)[j] << "(" << choice_num << "),";
      }
      std::cout << "\n";
    }
  }
  template<typename T>
  T & peek(std::stack<T> & stack) {
    return stack.top();
  }
  template<typename T>
  T & peek(std::queue<T> & queue) {
    return queue.front();
  }
  template<typename WorkList>
  void search() {
    //reference: psuedo-code in AOP Chapter 25.3.3
    size_t i = 1;
    std::set<size_t> visited;
    WorkList todo;
    std::vector<size_t> path;
    path.push_back(i);

    pages_vec[i - 1].set_depth(0);

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
  template<typename T>
  T & last(std::queue<T> & queue) {
    return queue.back();
  }
  template<typename T>
  T & last(std::stack<T> & stack) {
    return stack.top();
  }
  std::vector<Page> & get_pages_vec() { return pages_vec; }
  bool is_win_page(size_t n) {
    std::vector<size_t>::iterator win =
        std::find(win_page_num.begin(), win_page_num.end(), n);
    if (win != win_page_num.end()) {
      return true;
    }
    return false;
  }
  bool is_end_page(size_t n) const {
    std::vector<size_t>::const_iterator lose =
        std::find(lose_page_num.begin(), lose_page_num.end(), n);
    std::vector<size_t>::const_iterator win =
        std::find(win_page_num.begin(), win_page_num.end(), n);
    if (win != win_page_num.end() || lose != lose_page_num.end()) {
      return true;
    }
    return false;
  }
  Page & get_page_n(size_t n) { return pages_vec[n - 1]; }
  size_t get_story_size() { return pages_vec.size(); }
  //reference: strtol in man page
  //reference:  my Eval 1 function check_int()
  bool is_valid_choice(std::string num, size_t page_num) const {
    char * endptr;
    errno = 0;
    size_t choice_num = strtoul(num.c_str(), &endptr, 10);
    if ((errno == ERANGE && choice_num == ULONG_MAX) || choice_num == 0) {
      return false;
    }
    if (endptr == num.c_str()) {
      return false;
    }
    if (choice_num < 1 || choice_num > pages_vec[page_num - 1].get_num_options()) {
      return false;
    }
    return true;
  }
  void check_valid_story() {
    valid_page_num.insert(1);
    for (size_t i = 0; i < pages_vec.size(); i++) {
      std::vector<std::string> & navigator = pages_vec[i].get_navigator();
      if (!navigator[0].compare("WIN")) {
        win = 1;
        win_page_num.push_back(i + 1);
      }
      else if (!navigator[0].compare("LOSE")) {
        lose = 1;
        lose_page_num.push_back(i + 1);
      }
      else {
        std::vector<size_t> & option_pagenums = pages_vec[i].get_option_pagenums();
        for (size_t j = 0; j < option_pagenums.size(); j++) {
          if (0 < option_pagenums[j] && option_pagenums[j] <= pages_vec.size()) {
            neighbors[i].push_back(option_pagenums[j]);
            valid_page_num.insert(option_pagenums[j]);
          }
          else {
            std::cerr << "Invalid story: a option to a page not included\n";
            exit(EXIT_FAILURE);
          }
        }
      }
    }
    if (valid_page_num.size() != pages_vec.size() || win != 1 || lose != 1) {
      std::cerr << "Invalid story: lack reference or win/lose pages\n";
      exit(EXIT_FAILURE);
    }
  }
  bool check_page_available(int page_num, char * cfilename) {
    std::ifstream file;
    file.open(cfilename);
    if (!file.is_open()) {
      if (page_num == 1) {
        std::cerr << "No page1.txt\n";
        exit(EXIT_FAILURE);
      }
      else {
        return false;
      }
    }
    file.close();
    return true;
  }
  void store_story() {
    int i = 1;
    std::string former_part("/page");
    std::string latter_part(".txt");
    while (1) {
      std::stringstream num;
      num << i;
      std::string filename = story_name + former_part + num.str() + latter_part;
      //reference: string::c_str in cplusplus.com (remember to delete[])
      char * cfilename = new char[filename.length() + 1];
      std::strcpy(cfilename, filename.c_str());
      if (!check_page_available(i, cfilename)) {
        delete[] cfilename;
        break;
      }
      Page page(cfilename);
      page.store_page();
      delete[] cfilename;
      pages_vec.push_back(page);
      i++;
    }
    neighbors.resize(pages_vec.size());
  }
  friend std::ostream & operator<<(std::ostream & stream, const Story & story);
};

std::ostream & operator<<(std::ostream & stream, const Story & story) {
  std::cout << story.pages_vec[0];
  size_t page_num = 1;
  size_t choice_num;
  std::string input;
  while (std::cin >> input) {
    if (!story.is_valid_choice(input, page_num)) {
      std::cout << "That is not a valid choice, please try again\n";
      continue;
    }
    char * endptr;
    choice_num = strtoul(input.c_str(), &endptr, 10);
    page_num = story.pages_vec[page_num - 1].get_next_page_num(choice_num);
    std::cout << story.pages_vec[page_num - 1];
    if (story.is_end_page(page_num)) {
      break;
    }
    if (*endptr != '\0') {
      std::cout << "That is not a valid choice, please try again\n";
    }
  }
  if (!story.is_end_page(page_num)) {
    std::cerr << "end with not ending page\n";
    exit(EXIT_FAILURE);
  }
  return stream;
}
