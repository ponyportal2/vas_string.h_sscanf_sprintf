#ifndef SRC_TEST_H_
#define SRC_TEST_H_

#include <check.h>
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vas_sprintf.h"
#include "vas_sscanf.h"
#include "vas_string.h"

#define SIZE 8096
#define MAX_ERRLOG_SIZE 3
#define TC_STRUCT_SIZE 25.000
#define MAX_C 2
#define MIN_C 0
#define MAX_N 2
#define MIN_N 0
#define FILENAME "goodTestCases.txt"
#define START_STRUCT_SIZE 6
#define EXTRA_TESTS_COUNT 5

struct tcase_ {
  const char* name;
};

struct tcase_ tcases[] = {
    {"memchr_tc"},  {"memcmp_tc"},   {"memcpy_tc"},  {"memset_tc"},
    {"memmove_tc"}, {"strcat_tc"},   {"strncat_tc"}, {"strchr_tc"},
    {"strcmp_tc"},  {"strncmp_tc"},  {"strcpy_tc"},  {"strncpy_tc"},
    {"strcspn_tc"}, {"strerror_tc"}, {"strlen_tc"},  {"strpbrk_tc"},
    {"strrchr_tc"}, {"strspn_tc"},   {"strstr_tc"},  {"strtok_tc"},
    {"strspn_tc"},  {"to_upper"},    {"to_lower"},   {"insert"},
    {"trim"},       {"sscanf"},      {"sprintf"}};

struct tcase_ special_test[] = {{"sscanf_bonus_test"},
                                {"sscanf_extra_test"},
                                {"sscanf_test"},
                                {"sprintf_extra_test"},
                                {"sprintf_test"}};

struct test_struct {
  char* str1;
  char* str2;
  int c;
  int n;
};

typedef struct list {
  char* str1;
  struct list* next;
} list;

struct test_struct test[SIZE] = {
    {"shrek is shrek", "shrek is shrek", 's', 6},
    {"\n", "\0", 'j', 10},
    {"ABOB\nA", "dgdfgdfg123", '\n', 6},
    {"ere\0re", "\n\t\0", '8', 5},
    {"0\0\n \0\t habooba\n", "2\0\n 3\t\0 habooba\n", '0', 8},
    {"b\0\n \0\t habooba\n", "2\n 3\t\0 habooba\n", '0', 8}};

int is_special(char name[SIZE]);
size_t reverser(char* src, char* pat, char* res);
void printAllCases(size_t size);
size_t set_test(char* filename, list** tmp);
list* set_params(char* filename, list* tests_);
list* add_elem(list* elem, char* str1);
void destroy(list* root);
int found_pattern(char* line, char* pattern);
void print_error(char name_test[SIZE], int index, int counter);
void print_log(double n_checks);
void add_cases(TCase** tc, size_t index, size_t size);
Suite* create_str_suite(size_t size);
TCase* create_tc(size_t index, size_t size);

#endif  // SRC_TEST_H_
