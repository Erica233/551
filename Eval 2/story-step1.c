#include <assert.h>
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
  tell_story(lines, num_lines, NULL, 0);

  free_lines(lines, num_lines);

  return EXIT_SUCCESS;
}
