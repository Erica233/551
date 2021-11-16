#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Page {
 private:
  int page_num;
  std::vector<std::string> navigator;
  std::vector<std::string> text;

 public:
  Page(char * filename) {
    //reference: MLP079_sort_cpp
    std::ifstream file;
    file.open(filename);
    if (file.is_open()) {
      std::string line;
      std::vector<std::string> lines;
      while (std::getline(file, line)) {
        lines.push_back(line);
      }
      //store lines into page
      std::vector<std::string>::iterator it = lines.begin();
      while ((*it)[0] != '#') {
        navigator.push_back(*it);
        ++it;
      }
      ++it;
      while (it != lines.end()) {
        text.push_back(*it);
        ++it;
      }
    }
    else {
      std::cerr << "Failed to open file\n";
    }
  }

  friend std::ostream & operator<<(std::ostream & stream, const Page & page);
};

std::ostream & operator<<(std::ostream & stream, const Page & page) {
  //print story in the page
  stream << "overload <<\n";
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
      std::cout << " " << i << ". ";
      stream << " " << i << ". ";

      i++;
      ++jt;
    }
  }
  return stream;
}
