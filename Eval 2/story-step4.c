#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  int no_reuse = 0;
  char * catsfilename = NULL;
  char * storyfilename = NULL;
  if (argc == 3) {
    catsfilename = argv[1];
    storyfilename = argv[2];
  }
  else if (argc == 4 && !strcmp(argv[1], "-n")) {
    catsfilename = argv[2];
    storyfilename = argv[3];
    no_reuse = 1;
  }
  else {
    fprintf(stderr, "Usage: program (-n) wordsFile storyFile\n");
    return EXIT_FAILURE;
  }

  //build cats
  size_t num_catslines = 0;
  char ** cats_lines = read_lines(catsfilename, &num_catslines);
  catarray_t * cats = store_words(cats_lines, num_catslines);

  //build story
  size_t num_storylines = 0;
  char ** storylines = read_lines(storyfilename, &num_storylines);
  cats = tell_story(storylines, num_storylines, cats, no_reuse);

  free_lines(storylines, num_storylines);
  free_cats(cats);
  free_lines(cats_lines, num_catslines);

  return EXIT_SUCCESS;
}
