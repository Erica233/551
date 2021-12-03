#include "page.hpp"

// Check if the given input can be convert to a number.
// If it can, it will return the page number.
// Otherwise, it will exit with failure.
size_t Page::check_page_num(std::string num) {
  //reference: strtol in man page
  char * endptr;
  errno = 0;
  size_t page_num = strtoul(num.c_str(), &endptr, 10);
  if ((errno == ERANGE && page_num == ULONG_MAX) || (page_num == 0 && errno != 0) ||
      endptr == num.c_str() || *endptr != '\0') {
    std::cerr << "Page number invalid\n";
    exit(EXIT_FAILURE);
  }
  if (!isdigit(num[0])) {
    std::cerr << "Page number is negative\n";
    exit(EXIT_FAILURE);
  }
  return page_num;
}

// Check if the string can find a remove part.
// If can, it will return the start position of the remove part.
// Otherwise, exit with failure
// reference: string::find in cplusplus.com
std::size_t Page::extract_str(std::string str, std::string remove_part) {
  std::size_t found = str.find(remove_part);
  if (found == std::string::npos) {
    std::cerr << "invalid: not found remove_part\n";
    exit(EXIT_FAILURE);
  }
  return found;
}

// Check if the input path is valid. If it is invalid, exit with failure.
void Page::check_path_format() {
  std::string former_part("/page");
  std::string latter_part(".txt");
  std::size_t found = extract_str(path, former_part);
  std::size_t found_tail = path.find(latter_part);
  std::string num = path.substr(found + former_part.length(),
                                found_tail - found - former_part.length());
  page_num = check_page_num(num);
}

// Store the contents into the navigator and option page numbers.
// It will return the line number of '#'.
size_t Page::store_navigator(std::vector<std::string> & lines) {
  size_t line_slash;
  // If it is a ending page, store the navigator
  if (!lines[0].compare("WIN") || !lines[0].compare("LOSE")) {
    navigator.push_back(lines[0]);
    if (lines[1][0] != '#') {
      std::cerr << "lack slash\n";
      exit(EXIT_FAILURE);
    }
    line_slash = 1;
  }
  // If it is a middle page, store the choices.
  else {
    bool slash = false;
    for (size_t i = 0; i < lines.size(); i++) {
      if (lines[i][0] != '#') {
        //store option page numbers and choices
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

// Store the input file contents into Page class
void Page::store_page() {
  check_path_format();

  // reference: MLP079_sort_cpp
  std::ifstream file;
  // reference: string::c_str in cplusplus.com
  char * cpath = new char[path.length() + 1];
  std::strcpy(cpath, path.c_str());

  file.open(cpath);
  delete[] cpath;
  if (file.is_open()) {
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
      lines.push_back(line);
    }
    // Store the contents into navigator part and text part separately.
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
}

// Print out the page
std::ostream & operator<<(std::ostream & stream, const Page & page) {
  // Print text pary
  std::vector<std::string>::const_iterator it = page.text.begin();
  while (it != page.text.end()) {
    stream << *it << std::endl;
    ++it;
  }
  stream << std::endl;
  // Print navigator (choices or ending):
  // If it is a WIN/LOSE page, prints the ending.
  if (!page.navigator.begin()->compare("WIN")) {
    stream << "Congratulations! You have won. Hooray!\n";
  }
  else if (!page.navigator.begin()->compare("LOSE")) {
    stream << "Sorry, you have lost. Better luck next time!\n";
  }
  // Otherwise, print choices.
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
