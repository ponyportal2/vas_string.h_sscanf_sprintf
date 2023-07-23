#ifndef SRC_vas_STRING_H_
#define SRC_vas_STRING_H_

#include <stdbool.h>
#include <stddef.h>

#define vas_size_t unsigned long long
#define vas_NULL (void *)0

#include "vas_sprintf.h"
#include "vas_sscanf.h"

vas_size_t vas_strlen(const char *str);
char *vas_strstr(char *str1, char *str2);
void *vas_memchr(const void *arr, int c, vas_size_t n);
int vas_memcmp(const void *str1, const void *str2, vas_size_t n);
void *vas_memcpy(void *dest, const void *src, vas_size_t n);
void *vas_memmove(void *dest, const void *src, vas_size_t n);
void *vas_memset(void *str, int c, vas_size_t n);
char *vas_strchr(const char *str, int c);
int vas_strcmp(const char *str1, const char *str2);
int vas_strncmp(const char *str1, const char *str2, vas_size_t n);
char *vas_strcpy_helper(char *dest, const char *src, vas_size_t n,
                        bool isNcopy);
char *vas_strcpy(char *dest, const char *src);
char *vas_strncpy(char *dest, const char *src, vas_size_t n);
char *vas_strcat_helper(char *dest, const char *src, vas_size_t n, bool isNcat);
char *vas_strcat(char *dest, const char *src);
char *vas_strncat(char *dest, const char *src, vas_size_t n);
char *vas_strrchr(const char *str, int c);
vas_size_t vas_strspn(const char *stringOne, const char *stringTwo);
vas_size_t vas_strcspn(const char *stringOne, const char *stringTwo);
char *vas_strtok(char *str, const char *delim);
char *vas_strpbrk(const char *str, const char *sym);
char *vas_strerror(int errcode);
bool vas_match(const char *inputCharArr, char inputChar);
vas_size_t vas_strspn_helper(const char *stringOne, const char *stringTwo,
                             bool isCspn);
void *vas_insert(const char *src, const char *str, vas_size_t start_index);
void *vas_to_upper(const char *str);
void *vas_to_lower(const char *str);
void *vas_trim(const char *src, const char *trim_chars);

#endif  // SRC_vas_STRING_H_
