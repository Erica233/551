#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>
#include <stdlib.h>

#include "provided.h"

//any functions you want your main to use
char ** read_lines(char *, size_t *);
catarray_t * tell_story(char **, size_t, catarray_t *, int);
char * storycat(char *, const char *, size_t);
void free_lines(char **, size_t);
catarray_t * store_words(char **, size_t);
catarray_t * init_cats();
void free_cats(catarray_t *);
catarray_t * insert_cats(catarray_t *, char *, char *);
category_t init_category();
void free_category(category_t);
catarray_t * get_word(const char **, char *, catarray_t *, category_t *, int);
int check_valid_cat(char *, catarray_t *);
void print_category(category_t);
category_t * insert_used_word(const char *, category_t *);
catarray_t * remove_used(char *, const char *, catarray_t *);

#endif
