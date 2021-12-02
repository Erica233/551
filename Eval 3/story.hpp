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
  std::vector<std::set<size_t> > neighbor_sets;
  std::vector<std::vector<size_t> > neighbors;
  std::vector<std::vector<size_t> > win_paths;
  std::vector<std::vector<size_t> > win_path_choices;

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
    size_t choice;
    for (size_t i = 0; i < neighbors[curr_pagenum - 1].size(); i++) {
      if (next_pagenum == neighbors[curr_pagenum - 1][i]) {
        choice = i + 1;
      }
    }
    return choice;
  }

  void print_win_paths() {
    if (win_paths.size() == 0) {
      std::cout << "This story is unwinnable!\n";
    }
    for (size_t i = 0; i < win_paths.size(); i++) {
      //std::cout << "curr_path: ";
      for (size_t j = 0; j < win_paths[i].size(); j++) {
        size_t curr_pagenum = win_paths[i][j];
        if (j + 1 == win_paths[i].size()) {
          std::cout << win_paths[i][j] << "(win)";
          break;
        }
        size_t next_pagenum = win_paths[i][j + 1];
        size_t choice_num = find_choice(curr_pagenum, next_pagenum);
        std::cout << win_paths[i][j] << "(" << choice_num << "),";
      }
      std::cout << "\n";
    }
  }
  void dfs() {
    //reference: psuedo-code in AOP Chapter 25.3.3
    size_t i = 1;
    std::set<size_t> visited;
    std::stack<std::vector<size_t> > todo;
    std::vector<size_t> path;
    path.push_back(i);

    pages_vec[i - 1].set_depth(0);

    todo.push(path);
    int n = 0;
    //visited.insert(i);
    while (todo.size() != 0) {
      //std::cout << "------------loop " << n << "\n";
      n++;
      std::vector<size_t> curr_path = todo.top();
      //std::cout << "curr_path: ";
      //print_path(curr_path);
      //std::cout << "\n";
      todo.pop();
      size_t curr_pagenum = curr_path[curr_path.size() - 1];
      //std::cout << "curr_pagenum: " << curr_pagenum << "\n";
      if (is_win_page(curr_pagenum)) {
        //std::cout << "curr_pagenum is win page\n";
        //print_path(curr_path);
        //std::cout << "\n";
        win_paths.push_back(curr_path);
      }
      if (visited.find(curr_pagenum) == visited.end()) {
        //std::cout << "current pagenum is not in visited\n";
        visited.insert(curr_pagenum);
        //std::cout << curr_pagenum << " is visited\n";
        for (size_t j = 0; j < neighbors[curr_pagenum - 1].size(); j++) {
          //std::cout << "for neighbor: " << neighbors[curr_pagenum - 1][j] << "\n";
          if (pages_vec[neighbors[curr_pagenum - 1][j] - 1].get_depth() >
              pages_vec[curr_pagenum - 1].get_depth() + 1) {
            pages_vec[neighbors[curr_pagenum - 1][j] - 1].set_depth(
                pages_vec[curr_pagenum - 1].get_depth() + 1);
          }
          //curr_path.push_back(*it);
          //std::cout << "curr_path: ";
          //print_path(curr_path);
          todo.push(curr_path);
          //std::cout << "todo top before add new node\n";
          //print_path(todo.top());
          todo.top().push_back(neighbors[curr_pagenum - 1][j]);
          //std::cout << "todo top after add new node\n";
          //print_path(todo.top());
          //std::cout << "curr_path: ";
          //print_path(curr_path);
        }
      }
    }
  }
  //template<typename WorkList>
  void search() {
    //reference: psuedo-code in AOP Chapter 25.3.3
    size_t i = 1;

    std::set<size_t> visited;
    std::queue<std::vector<size_t> > todo;
    std::vector<size_t> path;
    path.push_back(i);

    pages_vec[i - 1].set_depth(0);

    todo.push(path);
    //visited.insert(i);
    while (todo.size() != 0) {
      std::vector<size_t> curr_path = todo.front();
      //std::cout << "curr_path: ";
      for (size_t i = 0; i < curr_path.size(); i++) {
        //std::cout << curr_path[i] << " ";
      }
      //std::cout << "\n";
      todo.pop();
      size_t curr_pagenum = curr_path[curr_path.size() - 1];
      //std::cout << "curr_pagenum: " << curr_pagenum << "\n";
      if (is_win_page(curr_pagenum)) {
        //std::cout << "curr_pagenum is win page\n";
      }
      if (visited.find(curr_pagenum) == visited.end()) {
        visited.insert(curr_pagenum);
        //std::cout << curr_pagenum << " is visited\n";
        for (std::set<size_t>::iterator it = neighbor_sets[curr_pagenum - 1].begin();
             it != neighbor_sets[curr_pagenum - 1].end();
             ++it) {
          if (pages_vec[(*it) - 1].get_depth() >
              pages_vec[curr_pagenum - 1].get_depth() + 1) {
            pages_vec[(*it) - 1].set_depth(pages_vec[curr_pagenum - 1].get_depth() + 1);
          }
          curr_path.push_back(*it);
          todo.push(curr_path);
        }
      }
    }
  }
  /*
  template<typename WorkList>
  void search() {
    //reference: psuedo-code in AOP Chapter 25.3.3
    std::set<size_t> visited;
    WorkList todo;
    size_t i = 1;
    pages_vec[i - 1].set_depth(0);
    todo.push(i);
    visited.insert(i);
    size_t curr;
    std::set<size_t>::iterator jt;
    while (todo.size() != 0) {
      curr = todo.front();
      todo.pop();
      for (std::set<size_t>::iterator it = neighbors[curr - 1].begin();
           it != neighbors[curr - 1].end();
           ++it) {
        if (visited.find(*it) == visited.end()) {
          if (pages_vec[(*it) - 1].get_depth() > pages_vec[curr - 1].get_depth() + 1) {
            pages_vec[(*it) - 1].set_depth(pages_vec[curr - 1].get_depth() + 1);
          }
          visited.insert(*it);
          todo.push(*it);
        }
      }
    }
  }
  */
  std::vector<Page> & get_pages_vec() { return pages_vec; }
  bool is_win_page(size_t n) {
    std::vector<size_t>::iterator win =
        std::find(win_page_num.begin(), win_page_num.end(), n);
    if (win != win_page_num.end()) {
      return true;
    }
    return false;
  }
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
  //reference: strtol in man page
  //reference:  my Eval 1 function check_int()
  bool is_valid_choice(std::string num, size_t page_num) {
    //std::cout << "in is_valid_choice(): \n";
    //std::cout << "current page num: " << page_num << " input: " << num << "\n";
    char * endptr;
    errno = 0;
    size_t choice_num = strtoul(num.c_str(), &endptr, 10);
    if ((errno == ERANGE && choice_num == ULONG_MAX) || choice_num == 0) {
      return false;
    }
    if (endptr == num.c_str()) {
      return false;
    }
    /*
    if (*endptr != '\0') {
      //std::cout << "That is not a valid choice, please try again\n";
      return false;
    }
    */
    if (choice_num < 1 || choice_num > pages_vec[page_num - 1].get_num_options()) {
      //std::cout << "is_valid_choice(): returns false ---- choice out of range\n";
      return false;
    }
    //std::cout << "is_valid_choice(): returns true\n";
    return true;
  }
  size_t start_story(std::istream & in) {
    //std::cout << "in start_story(): \n";
    // std::vector<Page> & pages_vec = story.get_pages_vec();
    std::cout << pages_vec[0];
    size_t page_num = 1;
    size_t choice_num;
    std::string input;
    while (in >> input) {
      //check_page_num();
      //std::cout << "input: " << input << "\n";
      if (!is_valid_choice(input, page_num)) {
        std::cout << "That is not a valid choice, please try again\n";
        continue;
      }
      char * endptr;
      choice_num = strtoul(input.c_str(), &endptr, 10);
      page_num = pages_vec[page_num - 1].get_next_page_num(choice_num);
      std::cout << pages_vec[page_num - 1];
      if (is_end_page(page_num)) {
        break;
      }
      if (*endptr != '\0') {
        std::cout << "That is not a valid choice, please try again\n";
      }
      //std::cout << story.get_page_n(page_num);
    }
    if (!is_end_page(page_num)) {
      std::cerr << "end with not ending page\n";
      exit(EXIT_FAILURE);
    }
    return page_num;
  }
  void check_valid_story() {
    //std::cout << "check_valid_story(): \n";
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
        std::vector<size_t> & option_pagenums = pages_vec[i].get_option_pagenums();
        //std::cout << "page with options: \n";
        for (size_t j = 0; j < option_pagenums.size(); j++) {
          if (0 < option_pagenums[j] && option_pagenums[j] <= pages_vec.size()) {
            //std::cout << option_page_num << " is a referenced page number\n";
            neighbor_sets[i].insert(option_pagenums[j]);
            neighbors[i].push_back(option_pagenums[j]);
            //std::cout << "i=" << i << " j=" << j << " neighbors" << option_page_num;
            valid_page_num.insert(option_pagenums[j]);
          }
          else {
            //std::cout << option_page_num << "not referenced page number\n";
            std::cerr << "Invalid story: a option to a page not included\n";
            exit(EXIT_FAILURE);
          }
        }
      }
    }
    //printNeighbor();
    if (valid_page_num.size() != pages_vec.size() || win != 1 || lose != 1) {
      std::cerr << "Invalid story: lack reference or win/lose pages\n";
      exit(EXIT_FAILURE);
    }
  }
  bool check_page_available(int page_num, char * cfilename) {
    std::ifstream file;
    file.open(cfilename);
    if (!file.is_open()) {
      //delete[] cfilename;
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
      if (!check_page_available(i, cfilename)) {
        delete[] cfilename;
        break;
      }
      Page page(cfilename);
      page.store_page();
      delete[] cfilename;
      pages_vec.push_back(page);
      //std::cout << pages_vec[i - 1];
      i++;
    }
    neighbors.resize(pages_vec.size());
    neighbor_sets.resize(pages_vec.size());
    //std::cout << "neighbor.size: " << neighbors.size();
    //printNeighbor();
  }
  void printNeighborSet() {
    std::cout << "neighbor.size: " << neighbor_sets.size() << std::endl;
    for (size_t i = 0; i < neighbor_sets.size(); i++) {
      std::cout << "set size: " << neighbor_sets[i].size() << " page " << i + 1 << ": ";
      for (std::set<size_t>::iterator it = neighbor_sets[i].begin();
           it != neighbor_sets[i].end();
           ++it) {
        std::cout << *it << ", ";
      }
      std::cout << "\n";
    }
  }
  friend std::ostream & operator<<(std::ostream & stream, const Story & story);
};
