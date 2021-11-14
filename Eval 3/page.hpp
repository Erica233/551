#include <cstdio>
#include <string>
#include <vector>

class Page {
 private:
  std::vector<std::string> text;

 public:
  friend std::ostream & operator<<(std::ostream & stream, const Page & page);
};

std::ostream & operator<<(std::ostream & stream, const Page & page) {
  return stream;
}