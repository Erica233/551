#include "pandemic.h"

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define SEVEN_DAY 7
#define POP_BASE 100000

/* read each country's name and population into struct */
country_t parseLine(char * line) {
  //WRITE ME
  country_t ans;
  ans.name[0] = '\0';
  ans.population = 0;

  //read country name into struct
  int i = 0;
  while (line[i] != ',') {
    if (line[i] == '\n') {
      fprintf(stderr, "Empty line\n");
      exit(EXIT_FAILURE);
    }
    if (i == (MAX_NAME_LEN - 1)) {
      fprintf(stderr, "Country name is too long\n");
      exit(EXIT_FAILURE);
    }
    ans.name[i] = line[i];
    i++;
  }
  ans.name[i] = '\0';

  //read population into struct
  i++;
  if (line[i] == '\n') {
    fprintf(stderr, "Missing population\n");
    exit(EXIT_FAILURE);
  }
  //citation: exapmle code of strtol in UNIX man page
  char * endptr;
  errno = 0;
  uint64_t population = strtoull(&line[i], &endptr, 10);
  if ((errno == ERANGE && population == ULONG_MAX) || (population == 0 && errno != 0)) {
    fprintf(stderr, "Population invalid\n");
    exit(EXIT_FAILURE);
  }
  if (endptr == &line[i]) {
    fprintf(stderr, "No digits were found\n");
    exit(EXIT_FAILURE);
  }
  ans.population = population;

  return ans;
}

/* calculate the seven-day running average of daily cases */
void calcRunningAvg(unsigned * data, size_t n_days, double * avg) {
  //WRITE ME
  for (size_t i = 0; i < (n_days - 6); i++) {
    uint64_t sum = 0;
    for (size_t j = 0; j < SEVEN_DAY; j++) {
      sum += data[i + j];
    }
    avg[i] = (double)sum / SEVEN_DAY;
  }
}

/* calculate cumulative number of cases on the 100000 population base */
void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  //WRITE ME
  uint64_t sum = 0;
  for (size_t i = 0; i < n_days; i++) {
    sum += data[i];
    cum[i] = (double)sum / ((double)pop / POP_BASE);
  }
}

/* Print out the country name with the maximum number of daily cases */
void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  //WRITE ME
  //find the max number of cases for each country
  uint64_t max_cases[n_countries];
  uint64_t max_per_country;
  for (size_t i = 0; i < n_countries; i++) {
    max_per_country = 0;
    for (size_t j = 0; j < n_days; j++) {
      if (data[i][j] > max_per_country) {
        max_per_country = data[i][j];
      }
    }
    max_cases[i] = max_per_country;
  }

  //find the country with max cases
  uint64_t total_max = 0;
  size_t max_country_index = 0;
  int ties = 0;
  for (size_t i = 0; i < n_countries; i++) {
    if (max_cases[i] == total_max) {
      ties += 1;
    }
    if (max_cases[i] > total_max) {
      ties = 1;
      total_max = max_cases[i];
      max_country_index = i;
    }
  }

  if (ties > 1) {
    printf("There is a tie between at least two countries\n");
  }
  if (ties == 1) {
    printf("%s has the most daily cases with %" PRIu64 "\n",
           countries[max_country_index].name,
           max_cases[max_country_index]);
  }
}
