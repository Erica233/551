#include "rand_story.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_WORD "cat"
#define DEFAULT_LEN strlen(DEFAULT_WORD)

// Given category and its word, insert them into cats
// It returns the updated cats
catarray_t * insert_cats(catarray_t * cats, char * category, char * word) {
  if (cats->n == 0) {
    cats->arr = malloc(sizeof(*cats->arr));
    assert(cats->arr);
    cats->arr[0].n_words = 1;
    cats->arr[0].name = category;
    cats->arr[0].words = malloc(sizeof(*cats->arr[0].words));
    assert(cats->arr[0].words);
    cats->arr[0].words[0] = word;
    cats->n = 1;
  }
  else {
    int find = 0;
    for (size_t j = 0; j < cats->n; j++) {
      if (strcmp(cats->arr[j].name, category) == 0) {
        //existed category
        cats->arr[j].n_words++;
        cats->arr[j].words = realloc(cats->arr[j].words,
                                     cats->arr[j].n_words * sizeof(*cats->arr[j].words));
        assert(cats->arr[j].words);
        cats->arr[j].words[cats->arr[j].n_words - 1] = word;
        free(category);
        find = 1;
        break;
      }
    }
    if (find == 0) {
      //nonexisted category
      cats->n++;
      cats->arr = realloc(cats->arr, cats->n * sizeof(*cats->arr));
      assert(cats->arr);
      cats->arr[cats->n - 1].name = category;
      cats->arr[cats->n - 1].n_words = 1;
      cats->arr[cats->n - 1].words = malloc(sizeof(*cats->arr[cats->n - 1].words));
      assert(cats->arr[cats->n - 1].words);
      cats->arr[cats->n - 1].words[0] = word;
    }
  }
  return cats;
}

// free the cats
void free_cats(catarray_t * cats) {
  for (size_t j = 0; j < cats->n; j++) {
    free_category(cats->arr[j]);
  }
  free(cats->arr);
  free(cats);
}

// initialize an empty cats
catarray_t * init_cats() {
  catarray_t * cats = malloc(sizeof(*cats));
  assert(cats);
  cats->arr = NULL;
  cats->n = 0;
  return cats;
}

// Parse lines and extract category name and a word,
// then insert the pair into cats.
// It returns the cats after insertions.
catarray_t * store_words(char ** lines, size_t num_lines) {
  char * word = NULL;
  size_t cat_len = 0;
  size_t word_len = 0;
  catarray_t * cats = init_cats();
  for (size_t i = 0; i < num_lines; i++) {
    //extract category
    char * category = lines[i];
    word = strchr(category, ':');
    if (word == NULL) {
      fprintf(stderr, "wrong format of words.txt input\n");
      exit(EXIT_FAILURE);
    }
    cat_len = strlen(category) - strlen(word);
    category = strndup(category, cat_len);
    //extract category and word
    word++;
    word_len = strlen(word) - strlen(strchr(word, '\n'));
    word = strndup(word, word_len);
    
    cats = insert_cats(cats, category, word);
  }
  return cats;
}

// frees an array of strings
void free_lines(char ** lines, size_t num_lines) {
  for (size_t i = 0; i < num_lines; i++) {
    free(lines[i]);
  }
  free(lines);
}

// concatenate two strings
char * storycat(char * dest, const char * src, size_t len) {
  dest = realloc(dest, (strlen(dest) + len + 1) * sizeof(*dest));
  assert(dest);
  strncat(dest, src, len);
  return dest;
}

// Checks whether a string input can be converted into integer.
// Returns 1 if it can, otherwise returns 0.
// citation: exapmle code of strtol in UNIX man page
int check_int(char * category) {
  char * endptr;
  errno = 0;
  unsigned long prev_num = strtoul(category, &endptr, 10);
  if ((errno == ERANGE && prev_num == ULONG_MAX) || (prev_num == 0 && errno != 0)) {
    return 0;
  }
  if (endptr == category) {
    return 0;
  }
  if (*endptr != '\0') {
    return 0;
  }
  return 1;
}

// Initialize an empty category
category_t init_category() {
  category_t a_cat;
  a_cat.name = NULL;
  a_cat.n_words = 0;
  a_cat.words = NULL;
  return a_cat;
}

// helper function: used to print each category
void print_category(category_t a_cat) {
  printf("-----print a category\n");
  printf("name: %s\n", a_cat.name);
  printf("n_words: %zu\n", a_cat.n_words);
  for (size_t i = 0; i < a_cat.n_words; i++) {
    printf("%zu's word in category: %s\n", i, a_cat.words[i]);
  }
}

// frees a category
void free_category(category_t a_cat) {
  free(a_cat.name);
  for (size_t x = 0; x < a_cat.n_words; x++) {
    free(a_cat.words[x]);
  }
  free(a_cat.words);
}

// Prints out the story given lines of story and cats.
// It parses lines of story first, replaces blanks into words from cats.
catarray_t * tell_story(char ** lines,
                        size_t num_lines,
                        catarray_t * cats,
                        int no_reuse) {
  char * start = NULL;
  char * former_part = NULL;
  char * latter_part = NULL;
  category_t used_words = init_category();
  used_words.name = NULL;
  // Parses each line of story
  for (size_t i = 0; i < num_lines; i++) {
    char * story_line = malloc(sizeof(*story_line));
    assert(story_line);
    story_line[0] = '\0';
    start = lines[i];
    // Extracts each blank, and replace them with appropriate word
    while (strchr(start, '_') != NULL) {
      former_part = strchr(start, '_');
      size_t non_cat_len = strlen(start) - strlen(former_part);
      story_line = storycat(story_line, start, non_cat_len);
      former_part++;

      latter_part = strchr(former_part, '_');
      if (latter_part == NULL) {
        fprintf(stderr, "Invalid story template: a blank lack the ending '_'\n");
        exit(EXIT_FAILURE);
      }
      size_t cat_len = strlen(former_part) - strlen(latter_part);
      char * category = strndup(former_part, cat_len);

      const char * replace_word = NULL;
      cats = get_word(&replace_word, category, cats, &used_words, no_reuse);

      free(category);
      story_line = storycat(story_line, replace_word, strlen(replace_word));
      latter_part++;
      start = latter_part;
    }
    story_line = storycat(story_line, start, strlen(start));
    printf("%s", story_line);
    free(story_line);
  }
  free_category(used_words);
  return cats;
}

// Gets a word given the string in blank.
// If the blank is an valid integer, find the matched previously used word.
// If the blank is an valid category, find a word in the category.
// If it requires not to reuse word, then it will remove the word after chosen.
catarray_t * get_word(const char ** word,
                      char * category,
                      catarray_t * cats,
                      category_t * used_words,
                      int no_reuse) {
  if (cats == NULL) {
    *word = DEFAULT_WORD;
    used_words = insert_used_word(*word, used_words);
  }
  else {
    int use_prev = check_int(category);
    long prev_num = strtoul(category, NULL, 10);
    // If the blank is an valid integer, find the matched previously used word
    if (use_prev == 1 && prev_num > 0 && prev_num <= used_words->n_words) {
      *word = used_words->words[used_words->n_words - prev_num];
      used_words = insert_used_word(*word, used_words);
    }
    else if (use_prev == 1 && prev_num < 0) {
      fprintf(stderr, "negative number in blank\n");
      exit(EXIT_FAILURE);
    }
    else {
      int valid_cat = check_valid_cat(category, cats);
      // If the blank is an valid category, find a word in the category
      if (valid_cat == 1) {
        *word = chooseWord(category, cats);
        used_words = insert_used_word(*word, used_words);
        if (no_reuse == 1) {
          cats = remove_used(category, *word, cats);
        }
      }
      else {
        fprintf(stderr, "invalid int or invalid category name\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  return cats;
}

// Removes the given pair of category and word in cats.
// Returns the result cats.
catarray_t * remove_used(char * category, const char * word, catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(category, cats->arr[i].name) == 0) {
      for (size_t j = 0; j < cats->arr[i].n_words; j++) {
        if (strcmp(word, cats->arr[i].words[j]) == 0) {
          if (cats->arr[i].n_words == 1) {
            // If the category only contain one word, then remove the category
            free(cats->arr[i].name);
            free(cats->arr[i].words);
            for (size_t x = i + 1; x < cats->n; x++) {
              cats->arr[x - 1] = cats->arr[x];
            }
            cats->n--;
            if (cats->n == 0) {
              cats->arr = NULL;
            }
            else {
              cats->arr = realloc(cats->arr, cats->n * sizeof(*cats->arr));
              assert(cats->arr);
            }
          }
          else {
            //Otherwise, remove the word
            for (size_t x = j + 1; x < cats->arr[i].n_words; x++) {
              cats->arr[i].words[x - 1] = cats->arr[i].words[x];
            }
            cats->arr[i].n_words--;
            cats->arr[i].words = realloc(
                cats->arr[i].words, cats->arr[i].n_words * sizeof(*cats->arr[i].words));
            assert(cats->arr[i].words);
          }
          return cats;
        }
      }
    }
  }
  fprintf(stderr, "Failed to remove used word\n");
  exit(EXIT_FAILURE);
}

// checks whether a category is valid in the given cats
// returns 1 if it's valid, otherwise returns 0
int check_valid_cat(char * category, catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(category, cats->arr[i].name) == 0) {
      return 1;
    }
  }
  return 0;
}

// Inserts a word into a category, returns the result category.
category_t * insert_used_word(const char * word, category_t * used_words) {
  used_words->n_words++;
  used_words->words =
      realloc(used_words->words, used_words->n_words * sizeof(*used_words->words));
  assert(used_words->words);
      used_words->words[used_words->n_words - 1] = strdup(word);
  return used_words;
}

// Reads a given file, and stores them into an array of strings.
char ** read_lines(char * filename, size_t * num_lines) {
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    perror("Failed to open the input file\n");
    exit(EXIT_FAILURE);
  }

  //reference: textbook Ch12.4
  char ** lines = NULL;
  char * curr = NULL;
  size_t linecap;
  while (getline(&curr, &linecap, f) > 0) {
    lines = realloc(lines, (*num_lines + 1) * sizeof(*lines));
    assert(lines);
    lines[*num_lines] = curr;
    curr = NULL;
    (*num_lines)++;
  }
  free(curr);

  if (fclose(f) != 0) {
    perror("Failed to close the input file\n");
    exit(EXIT_FAILURE);
  }
  return lines;
}
