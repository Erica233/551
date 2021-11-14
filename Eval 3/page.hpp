#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Page {
 private:
  std::vector<std::string> navigator;
  std::vector<std::string> text;

 public:
  Page(char * filename) {
    std::ifstream file;
    file.open(filename);
    if (file.is_open()) {
      std::string line;
      std::vector<std::string> lines;
      while (std::getline(file, line)) {
        lines.push_back(line);
      }
      //print story in the page
      std::vector<std::string>::iterator it = lines.begin();
      while ((*it)[0] != '#') {
        ++it;
      }
      ++it;
      while (it != lines.end()) {
        std::cout << *it << std::endl;
        ++it;
      }
      std::cout << std::endl;
      //if WIN/LOSE page
      if (!lines.begin()->compare("WIN")) {
        std::cout << "Congratulations! You have won. Hooray!\n";
      }
      else if (!lines.begin()->compare("LOSE")) {
        std::cout << "Sorry, you have lost. Better luck next time!\n";
      }

      //print choices
      else {
        std::cout << "What would you like to do?\n";
        std::cout << std::endl;
        std::vector<std::string>::iterator jt = lines.begin();
        int i = 1;
        while ((*jt)[0] != '#') {
          std::cout << " " << i << ". ";
          size_t j;
          for (j = 0; j < jt->length(); j++) {
            if ((*jt)[j] == ':') {
              break;
            }
          }
          j++;
          std::cout << jt->substr(j) << std::endl;
          i++;
          ++jt;
        }
      }
    }
    else {
      std::cerr << "Failed to open file\n";
    }
  }

  friend std::ostream & operator<<(std::ostream & stream, const Page & page);
};

std::ostream & operator<<(std::ostream & stream, const Page & page) {
  return stream;
}
