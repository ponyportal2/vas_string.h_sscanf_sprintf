#include "vas_string.h"

#include <stdio.h>
#include <stdlib.h>

#include "vas_strerror_codes.h"

vas_size_t vas_strlen(const char *str) {
  vas_size_t len = 0;
  while (*str != '\0') {
    str++;
    len++;
  }
  return (vas_size_t)len;
}

char *vas_strstr(char *str1, char *str2) {
  vas_size_t i = 0, j = 0;
  char *a = vas_NULL;
  if (str1[0] == '\0' && str2[0] != '\0') {
    a = vas_NULL;
  } else if (str1[0] == '\0' && str2[0] == '\0') {
    a = "";
  } else {
    while (str1[i] != '\0' && a == vas_NULL) {
      if (str1[i] == str2[j]) {
        j++;
      } else {
        j = 0;
      }
      if (str2[j] == '\0') {
        if (j == vas_strlen(str2) && j != 0) {
          a = str1 + i - j + 1;
        } else if (j == vas_strlen(str2) && j == 0) {
          a = str1;
        }
      }
      i++;
    }
  }
  return a;
}

void *vas_memchr(const void *arr, int c, vas_size_t n) {
  void *sym = vas_NULL;
  void *tmp = (void *)arr;
  if (n > 0) {
    while ((*((int *)tmp) != c && *((unsigned char *)tmp) != c) && n > 1) {
      n--;
      tmp++;
    }
    if (*((int *)tmp) == c) {
      sym = tmp;
    } else if (*((unsigned char *)tmp) == c) {
      sym = tmp;
    }
  }
  return sym;
}

int vas_memcmp(const void *str1, const void *str2, vas_size_t n) {
  int result = 0;
  char *tmp1 = (char *)str1;
  char *tmp2 = (char *)str2;
  while (n > 0) {
    result = *(char *)tmp1 - *(char *)tmp2;
    if (result != 0) {
      n = 0;
    } else {
      tmp1++;
      tmp2++;
      n--;
    }
  }
  return result;
}

void *vas_memcpy(void *dest, const void *src, vas_size_t n) {
  char *tmp = dest;
  const char *s = src;
  while (n > 0) {
    *tmp = *s;
    tmp++;
    s++;
    n--;
  }
  return dest;
}

void *vas_memmove(void *dest, const void *src, vas_size_t n) {
  char *tmp = dest;
  const char *s = src;
  char move[n];
  for (vas_size_t i = 0; i < n; i++) {
    move[i] = *s;
    s++;
  }
  for (vas_size_t i = 0; i < n; i++) {
    *tmp = move[i];
    tmp++;
  }
  return dest;
}

void *vas_memset(void *str, int c, vas_size_t n) {
  unsigned char *tmp = (unsigned char *)str;
  for (vas_size_t i = 0; i < n; i++) {
    *tmp = c;
    tmp++;
  }
  if (n == 0) {
  }
  return str;
}

char *vas_strchr(const char *str, int c) {
  char *sym = vas_NULL;
  int check = 1;
  while (*str && check) {
    if (*str == c) {
      sym = (char *)str;
      check = 0;
    }
    str++;
  }
  if (c == '\0') {
    sym = "";
  }
  return sym;
}

int vas_strcmp(const char *str1, const char *str2) {
  int check = 1, result = 0;
  while (*str1 && *str2 && check) {
    if (*str1 - *str2 != 0) {
      result = *str1 - *str2;
      check = 0;
    } else {
      str1++;
      str2++;
    }
  }
  if (!*str1 && !*str2) {
    result = 0;
  } else if (!*str2) {
    result = *str1;
  } else if (!*str1) {
    result = -*str2;
  }

  return result;
}

int vas_strncmp(const char *str1, const char *str2, vas_size_t n) {
  int check = 1, result = 0;
  while (*str1 && *str2 && check && n) {
    if (*str1 - *str2 != 0) {
      result = *str1 - *str2;
      check = 0;
    } else {
      str1++;
      str2++;
      n--;
    }
  }
  if (!*str1 && !*str2 && n) {
    result = 0;
  } else if (!*str2 && n) {
    result = *str1;
  } else if (!*str1 && n) {
    result = -*str2;
  }

  return result;
}

vas_size_t vas_strspn_helper(const char *stringOne, const char *stringTwo,
                             bool isCspn) {
  int i = 0;
  vas_size_t returnValue = 0;
  bool whileBreak = false;
  bool check = false;

  while (stringOne[i] != '\0' && whileBreak != true) {
    if ((check = vas_match(stringTwo, stringOne[i])) == isCspn) {
      whileBreak = true;
      returnValue = i;
    }
    i++;
    if (stringOne[i] == '\0' && check == false && isCspn == true) {
      returnValue = i;
    }
  }
  if (stringOne[i] == '\0' && check == true && isCspn == false) {
    returnValue = i;
  }
  return (vas_size_t)returnValue;
}

// проверяет есть ли символ inputChar в массиве inputCharArr:
bool vas_match(const char *inputCharArr, char inputChar) {
  int match = false;
  int i = 0;
  while (inputCharArr[i] != '\0') {
    if (inputCharArr[i] == inputChar) {
      match = true;
    }
    i++;
  }
  return match;
}

vas_size_t vas_strspn(const char *stringOne, const char *stringTwo) {
  return vas_strspn_helper(stringOne, stringTwo, false);
}

vas_size_t vas_strcspn(const char *stringOne, const char *stringTwo) {
  return vas_strspn_helper(stringOne, stringTwo, true);
}

char *vas_strrchr(const char *str, int c) {
  bool whileBreak = false;
  char *returnValue = vas_NULL;
  long long int i = vas_strlen(str) - 1;
  if (c == '\0') {
    returnValue = "";
  }
  while (i >= 0 && whileBreak != true) {
    if (str[i] == c) {
      returnValue = (char *)str + i;
      whileBreak = true;
    }
    i--;
  }
  return returnValue;
}

char *vas_strcpy(char *dest, const char *src) {
  return vas_strcpy_helper(dest, src, 0, false);
}

char *vas_strncpy(char *dest, const char *src, vas_size_t n) {
  return vas_strcpy_helper(dest, src, n, true);
}

char *vas_strcpy_helper(char *dest, const char *src, vas_size_t n,
                        bool isNcopy) {
  int i = 0;
  vas_size_t counter = 0;
  if (isNcopy == true) {
    while (src[i] != '\0' && counter < n) {
      dest[i] = src[i];
      counter++;
      i++;
    }
    while (counter < n) {
      dest[i] = '\0';
      counter++;
      i++;
    }
  } else {
    while (src[i] != '\0') {
      dest[i] = src[i];
      i++;
    }
    dest[i] = '\0';
  }
  return (char *)dest;
}

char *vas_strcat(char *dest, const char *src) {
  return vas_strcat_helper(dest, src, 0, false);
}

char *vas_strncat(char *dest, const char *src, vas_size_t n) {
  return vas_strcat_helper(dest, src, n, 1);
}

char *vas_strcat_helper(char *dest, const char *src, vas_size_t n,
                        bool isNcat) {
  vas_size_t destLen = vas_strlen(dest);
  int i = 0;
  vas_size_t counter = 0;
  if (isNcat == 1) {
    while (src[i] != '\0' && counter < n) {
      dest[destLen + i] = src[i];
      counter++;
      i++;
    }
  } else {
    while (src[i] != '\0') {
      dest[destLen + i] = src[i];
      i++;
    }
  }
  dest[destLen + i] = '\0';
  return (char *)dest;
}

char *vas_strtok(char *str, const char *delim) {
  static char *new_str;
  char *tmp = str;
  int check = 1;
  if (str != vas_NULL) {
    new_str = str;
  } else if (!new_str) {  // если строка закончилась, возвращаем 0
    tmp = 0;
    check = 0;
  }
  if (check != 0) {
    vas_size_t check1 =
        vas_strspn(new_str, delim);  // есть ли сейчас разделитель

    str = new_str + check1;  // перепрыгиваем разделитель
    tmp = new_str + check1;
    vas_size_t check2 =
        vas_strcspn(str, delim);  // длина до следующего разделителя
    new_str = str + check2;  // перепрыгиваем до следующего разделителя
    if (new_str == str) {  // для случая когда стартовая строка пустая
      tmp = 0;
      new_str = 0;
    } else {
      if (*new_str != 0) {  // зануляем разделитель
        *new_str = 0;
        new_str++;
      } else {
        new_str = vas_NULL;  // если строка закончилась то vas_NULL
      }
    }
  }
  return tmp;  // возвращаем строку до зануленного разделителя
}

char *vas_strpbrk(const char *str, const char *sym) {
  char *temp;
  vas_size_t check = vas_strcspn(str, sym);
  if (check == vas_strlen(str)) {
    temp = vas_NULL;
  } else {
    temp = (char *)str + check;
  }
  return temp;
}

char *vas_strerror(int errcode) {
  (void)errcode;
  char *error = vas_NULL;
#ifdef APPLE
  int max = 106;
  static char unknown[50] = "Unknown error: ";
#endif
#ifdef LINUX
  int max = 133;
  static char unknown[50] = "Unknown error ";
#endif
#if defined(LINUX) || defined(APPLE)
  if (errcode >= 0 && errcode <= max) {
#ifdef APPLE
    error = ErrorNames[errcode];
#else
    error = ErrorNamesLinux[errcode];
#endif
  } else {
    char num_error[20];
    vas_sprintf(num_error, "%d", errcode);  // поменять на свой
    vas_size_t i = vas_strlen(unknown);
    while (unknown[i] != ' ') {
      unknown[i] = '\0';
      i--;
    }
    vas_strcat(unknown, num_error);
    error = unknown;
  }
#endif
  return error;
}

// Part 5 Специальные функции обработки строк (вдохновленные классом String в
// языке C#)
void *vas_insert(const char *src, const char *str, vas_size_t start_index) {
  char *temp = vas_NULL;
  if (start_index < vas_strlen(src)) {
    vas_size_t size = 1;
    temp = calloc(vas_strlen(src) + 2 + vas_strlen(str), sizeof(char));
    for (vas_size_t i = 0; i < start_index; i++) {
      temp[i] = src[i];
    }
    for (vas_size_t i = start_index; (i - start_index) < vas_strlen(str); i++) {
      temp[i] = str[i - start_index];
    }
    for (size = start_index + vas_strlen(str);
         size <= (vas_strlen(src) + vas_strlen(str)); size++) {
      temp[size] = src[size - vas_strlen(str)];
    }
    temp[size] = '\0';
  }
  return temp;
}

void *vas_to_upper(const char *str) {
  char *temp = vas_NULL;
  if (str != vas_NULL) {
    temp = calloc(vas_strlen(str) + 1, sizeof(char));
    vas_size_t i = 0;
    for (i = 0; i < vas_strlen(str); i++) {
      if (str[i] >= 97 && str[i] <= 122) {
        temp[i] = str[i] - 32;
      } else {
        temp[i] = str[i];
      }
    }
    temp[i] = '\0';
  }
  return temp;
}

void *vas_to_lower(const char *str) {
  char *temp = vas_NULL;
  if (str != vas_NULL) {
    temp = calloc(vas_strlen(str) + 1, sizeof(char));
    vas_size_t i = 0;
    for (i = 0; i < vas_strlen(str); i++) {
      if (str[i] >= 65 && str[i] <= 90) {
        temp[i] = str[i] + 32;
      } else {
        temp[i] = str[i];
      }
    }
    temp[i] = '\0';
  }
  return temp;
}

void *vas_trim(const char *src, const char *trim_chars) {
  char *temp = vas_NULL;
  if (src != vas_NULL && trim_chars != vas_NULL) {
    vas_size_t counts = 0;
    vas_size_t counte = 0;
    for (vas_size_t i = 0; i < vas_strlen(src); i++) {
      for (vas_size_t j = 0; j < vas_strlen(trim_chars); j++) {
        if (src[i] == trim_chars[j]) {
          counts++;
          break;
        }
      }
      if (counts - 1 != i) {
        break;
      }
    }
    for (vas_size_t i = vas_strlen(src) - 1; i > counts; i--) {
      for (vas_size_t j = 0; j < vas_strlen(trim_chars); j++) {
        if (src[i] == trim_chars[j]) {
          counte++;
          break;
        }
      }
      if (counte != vas_strlen(src) - i) {
        break;
      }
    }
    if (counts + counte <= vas_strlen(src) && (counts + counte) != 0) {
      temp = calloc(vas_strlen(src) - counts - counte + 1, sizeof(char));
      vas_size_t i = 0;
      for (i = counts; i < vas_strlen(src) - counte; i++) {
        temp[i - counts] = src[i];
      }
      temp[i - counts] = '\0';
    }
  }
  return temp;
}
