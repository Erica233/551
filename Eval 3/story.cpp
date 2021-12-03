#include "story.hpp"

// Print the story depth of each page.
void Story::print_depth() {
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

// Given current page number and next page number,
// returns the choice number.
size_t Story::find_choice(size_t curr_pagenum, size_t next_pagenum) {
  for (size_t i = 0; i < neighbors[curr_pagenum - 1].size(); i++) {
    if (next_pagenum == neighbors[curr_pagenum - 1][i]) {
      return i + 1;
    }
  }
  return 0;
}

// Print out all the cycle-free winning paths,
// with both page numbers and choice numbers.
void Story::print_win_paths() {
  if (win_paths.size() == 0) {
    std::cout << "This story is unwinnable!\n";
  }
  for (std::set<std::vector<size_t> >::iterator it = win_paths.begin();
       it != win_paths.end();
       ++it) {
    for (size_t j = 0; j < it->size(); j++) {
      if (j + 1 == it->size()) {
        std::cout << (*it)[j] << "(win)";
        break;
      }
      size_t choice_num = find_choice((*it)[j], (*it)[j + 1]);
      std::cout << (*it)[j] << "(" << choice_num << "),";
    }
    std::cout << "\n";
  }
}

// Check if the given page number is a winning page number.
bool Story::is_win_page(size_t n) {
  std::vector<size_t>::iterator win =
      std::find(win_page_num.begin(), win_page_num.end(), n);
  if (win != win_page_num.end()) {
    return true;
  }
  return false;
}

// Check if the given page number is a ending page number.
bool Story::is_end_page(size_t n) const {
  std::vector<size_t>::const_iterator lose =
      std::find(lose_page_num.begin(), lose_page_num.end(), n);
  std::vector<size_t>::const_iterator win =
      std::find(win_page_num.begin(), win_page_num.end(), n);
  if (win != win_page_num.end() || lose != lose_page_num.end()) {
    return true;
  }
  return false;
}

// reference: strtol in man page
// Check if the input choice is a valid choice.
bool Story::is_valid_choice(std::string num, size_t page_num) const {
  char * endptr;
  errno = 0;
  size_t choice_num = strtoul(num.c_str(), &endptr, 10);
  // If the input is not a number
  if ((errno == ERANGE && choice_num == ULONG_MAX) || choice_num == 0 ||
      endptr == num.c_str()) {
    return false;
  }
  // If the input number is not a valid choice
  if (choice_num < 1 || choice_num > pages_vec[page_num - 1].get_num_options()) {
    return false;
  }
  return true;
}

// Check if the story is valid. If not, exit with failure.
void Story::check_valid_story() {
  valid_page_num.insert(1);
  for (size_t i = 0; i < pages_vec.size(); i++) {
    std::vector<std::string> & navigator = pages_vec[i].get_navigator();
    // Record the win and lose page numbers.
    if (!navigator[0].compare("WIN")) {
      win_page_num.push_back(i + 1);
    }
    else if (!navigator[0].compare("LOSE")) {
      lose_page_num.push_back(i + 1);
    }
    else {
      std::vector<size_t> & option_pagenums = pages_vec[i].get_option_pagenums();
      // Record the reference situation of pages.
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
  if (valid_page_num.size() != pages_vec.size() || win_page_num.size() == 0 ||
      lose_page_num.size() == 0) {
    std::cerr << "Invalid story: lack reference or win/lose pages\n";
    exit(EXIT_FAILURE);
  }
}

// Check if the page exists.
// Returns true if exists, otherwise, return false.
bool Story::check_page_available(int page_num, char * cfilename) {
  std::ifstream file;
  file.open(cfilename);
  if (!file.is_open()) {
    // If there is no page 1, exit with failure.
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

// Store the story pages from page 1, until a page is not exist.
void Story::store_story() {
  int i = 1;
  std::string former_part("/page");
  std::string latter_part(".txt");
  while (1) {
    // Get the next page's path
    std::stringstream num;
    num << i;
    std::string filename = story_name + former_part + num.str() + latter_part;
    //reference: string::c_str in cplusplus.com (remember to delete[])
    char * cfilename = new char[filename.length() + 1];
    std::strcpy(cfilename, filename.c_str());
    // If the next page does not exist, stop storing.
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

// Print the story.
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
    // Print the chosen valid page
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
