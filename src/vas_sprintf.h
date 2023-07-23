#ifndef SRC_SPRINTF_H_
#define SRC_SPRINTF_H_

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "vas_string.h"

#define MAXLINE 200
#define OUR_ERROR_ "KufLv91ySdu64DYPiXOHGx5Jj9Q2eKcYwnrjxhQG"

#define MISMADCH_ 1
#define ENDET_ 2

typedef struct Specificator {
  short width, precision;
  char type, length;
  bool flag_minus, flag_plus, flag_space, flag_zero, flag_noargumet, flag_hash;
} Specificator;

int vas_sprintf(char* buff, const char* format, ...);
void formatPrint(char* buff, Specificator spec, va_list args);
Specificator parseSpecificator(char** pointerToSpec);
void strcpy_help(char* str1, char* str2, int index);
void strcat_with_spec(char* buff, char* src, Specificator spec);
void itoa(long long num, char* src, Specificator spec);
void reverse(char* string);
void trimE(double inputDouble, char* buf, Specificator spec);
void trimDouble(double inputDouble, char* buf, Specificator spec);
void ourGcvt(long double value, char* buf, Specificator spec);
int zeroOs(char* str);
void g_cut_converter(double floatNum, char* buf, Specificator spec);
void strcpy_help(char* str1, char* str2, int index);
void strchr_help(char* str1, char s);
bool is_digit(char type);
void parseAdditionalArguments(Specificator* spec, va_list args);
void shift(char* buf, int direction);

#endif  // SRC_SPRINTF_H_
