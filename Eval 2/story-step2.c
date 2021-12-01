#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: program inputFileName\n");
    return EXIT_FAILURE;
  }

  size_t num_lines = 0;
  char ** lines = read_lines(argv[1], &num_lines);
  catarray_t * cats = store_words(lines, num_lines);
  printWords(cats);

  free_cats(cats);
  free_lines(lines, num_lines);

  return EXIT_SUCCESS;
}
