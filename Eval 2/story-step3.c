#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: program wordsFile storyFile\n");
    return EXIT_FAILURE;
  }

  //build cats
  size_t num_catslines = 0;
  char ** cats_lines = read_lines(argv[1], &num_catslines);
  catarray_t * cats = store_words(cats_lines, num_catslines);

  //build story
  size_t num_storylines = 0;
  char ** storylines = read_lines(argv[2], &num_storylines);
  tell_story(storylines, num_storylines, cats, 0);

  free_lines(storylines, num_storylines);
  free_cats(cats);
  free_lines(cats_lines, num_catslines);

  return EXIT_SUCCESS;
}
