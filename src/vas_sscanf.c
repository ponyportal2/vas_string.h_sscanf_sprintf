#include "vas_sscanf.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vas_sprintf.h"

#define OUR_ERROR_ "KufLv91ySdu64DYPiXOHGx5Jj9Q2eKcYwnrjxhQG"

#define MISMADCH_ 1
#define ENDET_ 2

struct Specificators;

int vas_sscanf(const char *input, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char inputStatic[16384] = {0};   // local copy of whole "input" parameter
  char formatStatic[16384] = {0};  // local copy of whole "format" parameter
  vas_strcpy(inputStatic, input);
  vas_strcpy(formatStatic, format);
  char currentInputElem[8192] = {0};
  char currentFormatElem[8192] = {0};
  vas_size_t n_counter = 0;
  int status = 0;
  int checkStartScanf = 1;
  int counterForReturn = 0;
  bool stopMove = false;
  bool checkLoaded = false;
  static bool startParse = false;
  static bool startProg = false;
  static bool progStart = false;
  struct Specificators Specif = {-1, -1, '\0', '\0', '\0', {'\0'}};
  while (status != MISMADCH_ && status != ENDET_) {
    bool formatLoaded = false;
    bool inputLoaded = false;
    bool varArgLoaded = false;
    bool checkArgWidth = false;
    Specif.width = -1;
    Specif.argWidth = -1;
    Specif.length = '\0';
    Specif.Specif = '\0';
    Specif.Space = '\0';
    Specif.buff[0] = '\0';
    formatParsing(formatStatic, currentFormatElem, &formatLoaded, &Specif,
                  &checkStartScanf, &status, &startParse, &startProg);
    if (!(status == MISMADCH_)) {
      if (Specif.Specif == 'c' && (Specif.width == -1 || Specif.width == 0)) {
        Specif.width = 1;
      }
      chooseSpecif(&Specif, inputStatic, &stopMove);
      if (Specif.Specif != 'n' && !stopMove) {
        inputParsing(inputStatic, currentInputElem, Specif.width, &inputLoaded,
                     Specif, &n_counter, &progStart);
      } else {
        if (inputStatic[0] == '\0') {
          n_counter--;
        }
        inputLoaded = true;
      }
      if (input[0] == '\0' && counterForReturn == 0) {
        counterForReturn = EOF;
        inputLoaded = false;
      }
      if (!stopMove && Specif.Specif != '%') {
        if (Specif.argWidth != 1) {
          varArgParsingAndAssignment(currentInputElem, &varArgLoaded,
                                     va_arg(args, void *), Specif, &n_counter,
                                     &counterForReturn);
        } else {
          checkArgWidth = true;
          counterForReturn--;
        }
      } else if (Specif.Specif != '%') {
        if (counterForReturn == 0) {
          varArgParsingAndAssignment(currentInputElem, &varArgLoaded,
                                     va_arg(args, void *), Specif, &n_counter,
                                     &counterForReturn);
          if (!varArgLoaded && !checkLoaded) {
            counterForReturn = EOF;
          } else {
            checkLoaded = true;
          }
        }
      } else if (Specif.Specif == '%') {
        varArgLoaded = true;
        counterForReturn--;
      }
      if (formatLoaded == true && inputLoaded == true &&
          (varArgLoaded == true || checkArgWidth == true)) {
        if (!checkLoaded) {
          counterForReturn++;
        }
        checkArgWidth = false;
      } else {
        status = ENDET_;
      }
    }
  }
  va_end(args);
  startParse = false;
  startProg = false;
  progStart = false;
  return counterForReturn;
}

void getNextFormatElem(char *input, char elem[8192], int *checkStartScanf,
                       bool *startParse, bool *startProg) {
  char *returnStr = vas_NULL;
  static char *currentFormatString = vas_NULL;
  bool checkFirst = true;
  if (*checkStartScanf == 1) {
    checkFirst = true;
  } else {
    checkFirst = false;
  }
  if (*checkStartScanf) {
    currentFormatString = input;
    *checkStartScanf = 0;
  }
  if (*currentFormatString == '%' && !checkFirst) {
    currentFormatString++;
    returnStr = currentFormatString;
  }
  if (!(*startParse) && *currentFormatString != '%') {
    *startParse = true;
    returnStr = currentFormatString;
  } else {
    *startProg = true;
    *startParse = true;
    while (*currentFormatString != '\0' && *currentFormatString != '%') {
      currentFormatString++;
    }
    if (*currentFormatString != '\0') {
      currentFormatString++;
      returnStr = currentFormatString;
    } else {
      currentFormatString = vas_NULL;
    }
  }
  if (returnStr != vas_NULL) {
    vas_strcpy(elem, returnStr);
  } else {  // if return is vas_NULL then we return "error" string
    vas_strcpy(elem, OUR_ERROR_);
  }
}

char formatParsing(char formatStatic[16384], char currentFormatElem[8192],
                   bool *formatLoaded, struct Specificators *Specif,
                   int *checkStartScanf, int *status, bool *startParse,
                   bool *startProg) {
  getNextFormatElem(formatStatic, currentFormatElem, checkStartScanf,
                    startParse, startProg);
  if (vas_strcmp(currentFormatElem, OUR_ERROR_) != 0) {
    *formatLoaded = true;
    if (*currentFormatElem == '*' && *startProg) {
      (*Specif).argWidth = 1;
      currentFormatElem++;
    } else if (*currentFormatElem >= '0' && *currentFormatElem <= '9') {
      (*Specif).width = vas_atoi(currentFormatElem);
      while (*currentFormatElem >= '0' && *currentFormatElem <= '9') {
        currentFormatElem++;
      }
    }
    char nextSym[2];
    vas_sprintf(nextSym, "%c", *currentFormatElem);
    if (vas_strpbrk(nextSym, "lLh")) {
      (*Specif).length = *currentFormatElem;
      currentFormatElem++;
    }
    vas_sprintf(nextSym, "%c", *currentFormatElem);
    int i = 0;
    if (vas_strpbrk(nextSym, "cdieEfgGosuxXpn")) {
      (*Specif).Specif = *currentFormatElem;
      currentFormatElem++;
      vas_sprintf(nextSym, "%c", *currentFormatElem);
      if (vas_strpbrk(nextSym, "\n \t")) {
        (*Specif).Space = ' ';
      }
      while (*currentFormatElem != '%') {
        (*Specif).buff[i] = *currentFormatElem;
        i++;
        currentFormatElem++;
      }
      (*Specif).buff[i] = '\0';
    } else if (nextSym[0] == '%') {
      (*Specif).Specif = *currentFormatElem;
      currentFormatElem++;
      while (*currentFormatElem != '%') {
        (*Specif).buff[i] = *currentFormatElem;
        i++;
        currentFormatElem++;
      }
    } else if (*currentFormatElem != '\0') {
      *status = 0;
      (*Specif).Specif = '%';
      (*Specif).Space = ' ';
      while (*currentFormatElem != '%') {
        (*Specif).buff[i] = *currentFormatElem;
        i++;
        currentFormatElem++;
      }
    }
  }
  currentFormatElem++;
  *startProg = true;
  return *currentFormatElem;
}

void chooseSpecif(struct Specificators *Specif, char inputStatic[16384],
                  bool *stopMove) {
  if ((*Specif).Specif == 'd' || (*Specif).Specif == 'u') {
    ifSpecIsD(Specif, inputStatic, stopMove);
  } else if ((*Specif).Specif == 'i') {
    ifSpecIsI(Specif, inputStatic, stopMove);
  } else if ((*Specif).Specif == 'f' || (*Specif).Specif == 'e' ||
             (*Specif).Specif == 'E' || (*Specif).Specif == 'g' ||
             (*Specif).Specif == 'G') {
    ifSpecIsF(Specif, inputStatic, stopMove);
  } else if ((*Specif).Specif == 'o') {
    ifSpecIsO(Specif, inputStatic, stopMove);
  } else if ((*Specif).Specif == 'x' || (*Specif).Specif == 'X' ||
             (*Specif).Specif == 'p') {
    ifSpecIsX(Specif, inputStatic, stopMove);
  }
}

void someJump(char inputStatic[16384], int *startParse, int wid) {
  int j = 0;
  while (vas_match("\t \n", inputStatic[j])) {
    (*startParse)++;
    j++;
  }
  if ((inputStatic[j] == '+' || inputStatic[j] == '-') &&
      (wid > 1 || wid == -1)) {
    (*startParse)++;
  }
}

void ifSpecIsD(struct Specificators *Specif, char inputStatic[16384],
               bool *stopMove) {
  int startParse = 0;
  int wid = (*Specif).width;
  someJump(inputStatic, &startParse, wid);
  if ((*Specif).width == -1) {
    (*Specif).width = startParse;
    while (inputStatic[(*Specif).width] >= '0' &&
           inputStatic[(*Specif).width] <= '9') {
      (*Specif).width++;
    }
    if ((*Specif).width == startParse) {
      *stopMove = true;
    }
  } else {
    int findNotNumber = (*Specif).width;
    (*Specif).width = startParse;
    int i = startParse;
    bool checkIfAnyNumber = false;
    while (inputStatic[i] >= '0' && inputStatic[i] <= '9' &&
           findNotNumber > 0) {
      findNotNumber--;
      i++;
      (*Specif).width++;
      checkIfAnyNumber = true;
    }
    if (!checkIfAnyNumber) {
      *stopMove = true;
    }
  }
}

void ifSpecIsI(struct Specificators *Specif, char inputStatic[16384],
               bool *stopMove) {
  int startParse = 2;
  int wid = (*Specif).width;
  someJump(inputStatic, &startParse, wid);
  if (inputStatic[startParse - 2] != '0' &&
      inputStatic[startParse - 1] !=
          'x') {  // если первое не 0 - то число десятичное
    ifSpecIsD(Specif, inputStatic, stopMove);
  } else {
    if (inputStatic[startParse - 1] ==
        'x') {  // если второе х - то шестнадцатиричное
      helpHexPars(Specif, inputStatic, stopMove, startParse);
    } else if (inputStatic[startParse - 2] ==
               '0') {  // если первое 0, а второе не х, то восьмиричное
      startParse--;
      startParse--;
      helpOctPars(Specif, inputStatic, stopMove, startParse);
    }
  }
}

bool checkInfNan(char inputStatic[16384], int i) {
  bool check;
  if (((inputStatic[i] == 'i' || inputStatic[i] == 'I') &&
       (inputStatic[i + 1] == 'n' || inputStatic[i + 1] == 'N') &&
       (inputStatic[i + 2] == 'f' || inputStatic[i + 2] == 'F')) ||
      ((inputStatic[i] == 'N' || inputStatic[i] == 'n') &&
       (inputStatic[i + 1] == 'a' || inputStatic[i + 1] == 'A') &&
       (inputStatic[i + 2] == 'N' || inputStatic[i + 2] == 'n'))) {
    check = true;
  } else {
    check = false;
  }
  return check;
}

void ifSpecIsF(struct Specificators *Specif, char inputStatic[16384],
               bool *stopMove) {
  int startParse = 0;
  int wid = (*Specif).width;
  someJump(inputStatic, &startParse, wid);
  int i = startParse;
  FspecifHelp(Specif, inputStatic, stopMove, i, startParse);
}

void FspecifHelp(struct Specificators *Specif, char inputStatic[16384],
                 bool *stopMove, int i, int startParse) {
  int findNotNumber = (*Specif).width;
  int checkDot = 0;
  (*Specif).width = startParse;
  bool checkNan = false;
  if (checkInfNan(inputStatic, (*Specif).width) &&
      (findNotNumber >= 3 || findNotNumber <= -1)) {
    (*Specif).width = (*Specif).width + 3;
    checkNan = true;
  }
  while (((inputStatic[i] >= '0' && inputStatic[i] <= '9' && !checkNan) ||
          (inputStatic[i] == '.' && checkDot == 0 && !checkNan)) &&
         (findNotNumber > 0 || findNotNumber <= -1)) {
    if (inputStatic[i] == '.') {
      checkDot = 1;
    }
    findNotNumber--;
    (*Specif).width++;
    i++;
  }
  if ((inputStatic[i] == 'e' || inputStatic[i] == 'E') && !checkNan) {
    i++;
    (*Specif).width++;
    if (inputStatic[i] == '+' || inputStatic[i] == '-') {
      i++;
      (*Specif).width++;
    }
    while ((inputStatic[i] >= '0' && inputStatic[i] <= '9')) {
      i++;
      (*Specif).width++;
    }
  }
  if ((*Specif).width == startParse ||
      ((*Specif).width == startParse + 1 && inputStatic[startParse] == '.' &&
       !checkNan)) {
    *stopMove = true;
  }
}

void ifSpecIsO(struct Specificators *Specif, char inputStatic[16384],
               bool *stopMove) {
  int startParse = 0;
  int wid = (*Specif).width;
  someJump(inputStatic, &startParse, wid);
  helpOctPars(Specif, inputStatic, stopMove, startParse);
}

bool checkHex(char inputStatic[16384], int i) {
  bool check;
  if (((inputStatic[i + 2] >= '0' && inputStatic[i + 2] <= '9') ||
       (inputStatic[i + 2] >= 'a' && inputStatic[i + 2] <= 'f') ||
       (inputStatic[i + 2] >= 'A' && inputStatic[i + 2] <= 'F'))) {
    check = true;
  } else {
    check = false;
  }
  return check;
}

void ifSpecIsX(struct Specificators *Specif, char inputStatic[16384],
               bool *stopMove) {
  int startParse = 0;
  int wid = (*Specif).width;
  someJump(inputStatic, &startParse, wid);
  if ((inputStatic[startParse] == '0' && inputStatic[startParse + 1] == 'x' &&
       (wid > 2 || wid <= -1)) &&
      (checkHex(inputStatic, startParse))) {
    startParse++;
    startParse++;
  }
  helpHexPars(Specif, inputStatic, stopMove, startParse);
}

void helpHexPars(struct Specificators *Specif, char inputStatic[16384],
                 bool *stopMove, int startParse) {
  int findNotNumber = (*Specif).width;
  (*Specif).width = startParse;
  int i = startParse;
  while ((checkHex(inputStatic, i - 2)) &&
         (findNotNumber > 0 || findNotNumber <= -1)) {
    findNotNumber--;
    i++;
    (*Specif).width++;
  }
  if ((*Specif).width == startParse) {
    *stopMove = true;
  }
}

void helpOctPars(struct Specificators *Specif, char inputStatic[16384],
                 bool *stopMove, int startParse) {
  int findNotNumber = (*Specif).width;
  (*Specif).width = startParse;
  int i = startParse;
  while (inputStatic[i] >= '0' && inputStatic[i] <= '7' &&
         (findNotNumber > 0 || findNotNumber <= -1)) {
    findNotNumber--;
    (*Specif).width++;
    i++;
  }
  if ((*Specif).width == startParse) {
    *stopMove = true;
  }
}

void varArgParsingAndAssignment(char currentInputElem[8192], bool *varArgLoaded,
                                void *currentVarArg,
                                struct Specificators Specif,
                                vas_size_t *n_counter, int *counterForReturn) {
  char tempCurrentInputElem[8192] = {0};
  vas_strcpy(tempCurrentInputElem, currentInputElem);
  if (Specif.Specif == 'i') {
    assignI(tempCurrentInputElem, varArgLoaded, currentVarArg, Specif);
  } else if (Specif.Specif == 's' || Specif.Specif == 'c') {
    assignS(tempCurrentInputElem, varArgLoaded, currentVarArg);
  } else if (Specif.Specif == 'f' || Specif.Specif == 'e' ||
             Specif.Specif == 'E' || Specif.Specif == 'g' ||
             Specif.Specif == 'G') {
    assignF(tempCurrentInputElem, varArgLoaded, currentVarArg, Specif);
  } else if (Specif.Specif == 'd' || Specif.Specif == 'u') {
    assignD(tempCurrentInputElem, varArgLoaded, currentVarArg, Specif);
  } else if (Specif.Specif == 'o') {
    assignO(tempCurrentInputElem, varArgLoaded, currentVarArg, Specif);
  } else if (Specif.Specif == 'x' || Specif.Specif == 'X') {
    assignX(tempCurrentInputElem, varArgLoaded, currentVarArg, Specif);
  } else if (Specif.Specif == 'n') {
    assignN(varArgLoaded, currentVarArg, n_counter, counterForReturn);
  } else if (Specif.Specif == 'p') {
    assignP(tempCurrentInputElem, varArgLoaded, currentVarArg);
  }
}
void hexToBase(struct Specificators Specif, int checkSign, void *currentVarArg,
               char *temp) {
  if (Specif.length == 'l') {
    *((long int *)currentVarArg) = checkSign * hexToBaseTenLong(temp);
  } else if (Specif.length == 'h') {
    *((short *)currentVarArg) = checkSign * hexToBaseTenShort(temp);
  } else {
    *((int *)currentVarArg) = checkSign * hexToBaseTen(temp);
  }
}

void octToBase(struct Specificators Specif, int checkSign, void *currentVarArg,
               char *temp) {
  if (Specif.length == 'l') {
    *((long int *)currentVarArg) = checkSign * octToBaseTenLong(temp);
  } else if (Specif.length == 'h') {
    *((short *)currentVarArg) = checkSign * octToBaseTenShort(temp);
  } else {
    *((int *)currentVarArg) = checkSign * octToBaseTen(temp);
  }
}

void tenToBase(struct Specificators Specif, int checkSign, void *currentVarArg,
               char *temp) {
  if (Specif.length == 'l') {
    *((long int *)currentVarArg) = checkSign * vas_atoiLong(temp);
  } else if (Specif.length == 'h') {
    *((short *)currentVarArg) = checkSign * vas_atoiShort(temp);
  } else {
    *((int *)currentVarArg) = checkSign * vas_atoi(temp);
  }
}

void assignI(char inCurrentInputElem[8192], bool *varArgLoaded,
             void *currentVarArg, struct Specificators Specif) {
  if (currentVarArg != vas_NULL) {
    char *temp = inCurrentInputElem;
    int checkSign = 1;
    int i = 0;
    while (vas_match("\t \n", inCurrentInputElem[i])) {
      i++;
      temp++;
    }
    if (inCurrentInputElem[i] == '-') {
      checkSign = -1;
      i = i + 2;
      temp++;
    } else if (inCurrentInputElem[i] == '+') {
      checkSign = 1;
      i = i + 2;
      temp++;
    } else {
      i = i + 1;
    }
    *varArgLoaded = true;
    if (inCurrentInputElem[i] == 'x') {
      temp++;
      temp++;
      hexToBase(Specif, checkSign, currentVarArg, temp);
    } else if (inCurrentInputElem[i - 1] == '0') {
      temp++;
      octToBase(Specif, checkSign, currentVarArg, temp);
    } else {
      tenToBase(Specif, checkSign, currentVarArg, temp);
    }
  } else {
    *varArgLoaded = false;
  }
}

void assignS(char currentInputElem[8192], bool *varArgLoaded,
             void *currentVarArg) {
  if (currentVarArg != vas_NULL) {
    *varArgLoaded = true;
    vas_strcpy(currentVarArg, currentInputElem);
  } else {
    *varArgLoaded = false;
  }
}

void assignF(char inCurrentInputElem[8192], bool *varArgLoaded,
             void *currentVarArg, struct Specificators Specif) {
  if (currentVarArg != vas_NULL) {
    *varArgLoaded = true;
    if (Specif.length == 'L') {
      *((long double *)currentVarArg) = vas_atofLong(inCurrentInputElem);
    } else if (Specif.length == 'l') {
      *((double *)currentVarArg) = vas_atof(inCurrentInputElem);
    } else {
      *((float *)currentVarArg) = vas_atof(inCurrentInputElem);
    }
  } else {
    *varArgLoaded = false;
  }
}

void assignD(char inCurrentInputElem[8192], bool *varArgLoaded,
             void *currentVarArg, struct Specificators Specif) {
  if (currentVarArg != vas_NULL) {
    *varArgLoaded = true;
    tenToBase(Specif, 1, currentVarArg, inCurrentInputElem);
  } else {
    *varArgLoaded = false;
  }
}

void assignO(char inCurrentInputElem[8192], bool *varArgLoaded,
             void *currentVarArg, struct Specificators Specif) {
  if (currentVarArg != vas_NULL) {
    char *temp = inCurrentInputElem;
    int checkSign = 1;
    int i = 0;
    while (vas_match("\t \n", inCurrentInputElem[i])) {
      i++;
      temp++;
    }
    if (inCurrentInputElem[i] == '-') {
      checkSign = -1;
      temp++;
    } else if (inCurrentInputElem[i] == '+') {
      checkSign = 1;
      temp++;
    }
    *varArgLoaded = true;
    octToBase(Specif, checkSign, currentVarArg, temp);
  } else {
    *varArgLoaded = false;
  }
}

void assignX(char inCurrentInputElem[8192], bool *varArgLoaded,
             void *currentVarArg, struct Specificators Specif) {
  if (currentVarArg != vas_NULL) {
    char *temp = inCurrentInputElem;
    int checkSign = 1;
    int i = 0;
    while (vas_match("\t \n", inCurrentInputElem[i])) {
      i++;
      temp++;
    }
    if (inCurrentInputElem[i] == '-') {
      checkSign = -1;
      temp++;
      i++;
    } else if (inCurrentInputElem[i] == '+') {
      checkSign = 1;
      temp++;
      i++;
    }
    if (inCurrentInputElem[i] == '0' && inCurrentInputElem[i + 1] == 'x') {
      temp++;
      temp++;
    }
    *varArgLoaded = true;
    hexToBase(Specif, checkSign, currentVarArg, temp);
  } else {
    *varArgLoaded = false;
  }
}

void assignN(bool *varArgLoaded, void *currentVarArg, vas_size_t *n_counter,
             int *counterForReturn) {
  if (currentVarArg != vas_NULL) {
    *varArgLoaded = true;
    *((int *)currentVarArg) = *(int *)n_counter;
    *counterForReturn = *counterForReturn - 1;
  } else {
    *varArgLoaded = false;
  }
}

void assignP(char inCurrentInputElem[8192], bool *varArgLoaded,
             void *currentVarArg) {
  if (currentVarArg != vas_NULL) {
    char *temp = inCurrentInputElem;
    *varArgLoaded = true;
    *((long int *)currentVarArg) = hexToBaseTenLong(temp);
  } else {
    *varArgLoaded = false;
  }
}

void inputParsing(char inputFlip[16384], char currentInputElem[8192], int wid,
                  bool *inputLoaded, struct Specificators Specif,
                  vas_size_t *n_counter, bool *startProg) {
  fillOneByOne(inputFlip, currentInputElem, wid, Specif, n_counter, startProg);
  if (vas_strcmp(currentInputElem, OUR_ERROR_) != 0) {
    *inputLoaded = true;
  }
}

void fillOneByOne(char input[16384], char currentInputElem[8192], int wid,
                  struct Specificators Specif, vas_size_t *n_counter,
                  bool *startProg) {
  int i = 0, j = 0;
  bool checkWid = false;
  if (wid == -1) {
    checkWid = true;
  }
  if (Specif.Specif != '%') {
    while (input[i] != '\0' && (j < wid || checkWid)) {
      if (i == 0) {
        while (vas_match("\t \n", input[i]) && Specif.Specif == 's') {
          i++;
        }
      }
      if (vas_match("\t \n", input[i]) == false) {
        currentInputElem[j] = input[i];
        j++;
      } else if (Specif.Specif ==
                 's') {  // если поймали разделитель - прерываем
        currentInputElem[j] = '\0';
        i--;
        j = wid;
        checkWid = false;
      } else {
        currentInputElem[j] = input[i];
        j++;
      }
      i++;
    }
  }
  currentInputElem[j] = '\0';
  chopLeft(input, i, n_counter, Specif, startProg);
}

void chopLeft(char input[16384], int howMany, vas_size_t *n_counter,
              struct Specificators Specif, bool *startProg) {
  int j = 0, counter = 0;
  vas_size_t length = vas_strlen(input);
  bool startParse = true;
  for (int i = howMany; input[i] != '\0'; i++, j++) {
    if (Specif.Space == ' ') {
      while (input[i] != '\0' && vas_match("\t \n", input[i]) && startParse) {
        i++;
      }
      startParse = false;
    }
    counter++;
    input[j] = input[i];
  }
  *n_counter = *n_counter + (int)length - counter;
  input[j] = '\0';
  if (Specif.buff[0] != '\0' || Specif.Specif == '%') {
    chopIfBuff(input, n_counter, Specif, startProg);
  }
  *startProg = true;
}

void chopIfBuff(char input[16384], vas_size_t *n_counter,
                struct Specificators Specif, bool *startProg) {
  int k = 0, j = 0, i = 0;
  bool checkBuff = false;
  while ((Specif.buff[k] != '\0' && input[i] != '\0') || Specif.Specif == '%') {
    if (vas_match("\t \n", Specif.buff[k]) || Specif.Specif == '%') {
      while (input[i] != '\0' && vas_match("\t \n", input[i])) {
        i++;
      }
      if (*startProg) {
        k++;
      }
    }
    while (Specif.buff[k] != '\0' && vas_match("\t \n", Specif.buff[k])) {
      k++;
    }
    if ((Specif.buff[k] == input[i] && Specif.buff[k] != '\0') ||
        (((Specif.Specif == '%' && input[i] == Specif.Specif)))) {
      i++;
      k++;
      checkBuff = true;
      if (Specif.Specif == '%') {
        while (input[i] != '\0' && vas_match("\t \n", input[i])) {
          i++;
        }
      }
    } else {
      if (Specif.buff[k] != '\0') {
        input[0] = '\0';
        checkBuff = false;
      }
      break;
    }
    if (Specif.Specif == '%') {
      Specif.Specif = '\0';
      k--;
      k--;
    }
  }
  if (checkBuff && Specif.buff[k] == '\0') {
    *n_counter = *n_counter + 1;
  }
  j = 0;
  if (checkBuff) {
    *n_counter = *n_counter + (int)vas_strlen(Specif.buff);
    while (input[i] != '\0') {
      input[j] = input[i];
      i++;
      j++;
    }
    input[j] = '\0';
  }
}

unsigned long hexToBaseTenLong(char *hexVal) {
  unsigned long returnValue = 0;
  int hexLen = vas_strlen(hexVal);
  for (int i = 0; i < hexLen; i++) {
    if (hexVal[i] >= 'a' && hexVal[i] <= 'f') {
      returnValue = returnValue + (hexVal[i] - 87) * pow(16, hexLen - i - 1);
    } else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
      returnValue = returnValue + (hexVal[i] - 55) * pow(16, hexLen - i - 1);
    } else if (hexVal[i] >= '0' && hexVal[i] <= '9') {
      returnValue = returnValue + (hexVal[i] - 48) * pow(16, hexLen - i - 1);
    }
  }
  return returnValue;
}

unsigned int hexToBaseTen(char *hexVal) {
  unsigned int returnValue = 0;
  int hexLen = vas_strlen(hexVal);
  for (int i = 0; i < hexLen; i++) {
    if (hexVal[i] >= 'a' && hexVal[i] <= 'f') {
      returnValue = returnValue + (hexVal[i] - 87) * pow(16, hexLen - i - 1);
    } else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
      returnValue = returnValue + (hexVal[i] - 55) * pow(16, hexLen - i - 1);
    } else if (hexVal[i] >= '0' && hexVal[i] <= '9') {
      returnValue = returnValue + (hexVal[i] - 48) * pow(16, hexLen - i - 1);
    }
  }
  return returnValue;
}

unsigned short int hexToBaseTenShort(char *hexVal) {
  unsigned short int returnValue = 0;
  int hexLen = vas_strlen(hexVal);
  for (int i = 0; i < hexLen; i++) {
    if (hexVal[i] >= 'a' && hexVal[i] <= 'f') {
      returnValue = returnValue + (hexVal[i] - 87) * pow(16, hexLen - i - 1);
    } else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
      returnValue = returnValue + (hexVal[i] - 55) * pow(16, hexLen - i - 1);
    } else if (hexVal[i] >= '0' && hexVal[i] <= '9') {
      returnValue = returnValue + (hexVal[i] - 48) * pow(16, hexLen - i - 1);
    }
  }
  return returnValue;
}

unsigned int octToBaseTen(char *octVal) {
  int returnValue = 0;
  int octLen = vas_strlen(octVal);
  for (int i = 0; i < octLen; i++) {
    if (octVal[i] >= '0' && octVal[i] <= '7') {
      returnValue = returnValue + (octVal[i] - '0') * pow(8, octLen - i - 1);
    }
  }
  return returnValue;
}

unsigned long int octToBaseTenLong(char *octVal) {
  long int returnValue = 0;
  int octLen = vas_strlen(octVal);
  for (int i = 0; i < octLen; i++) {
    if (octVal[i] >= '0' && octVal[i] <= '7') {
      returnValue = returnValue + (octVal[i] - '0') * pow(8, octLen - i - 1);
    }
  }
  return returnValue;
}

unsigned short int octToBaseTenShort(char *octVal) {
  short int returnValue = 0;
  int octLen = vas_strlen(octVal);
  for (int i = 0; i < octLen; i++) {
    if (octVal[i] >= '0' && octVal[i] <= '7') {
      returnValue = returnValue + (octVal[i] - '0') * pow(8, octLen - i - 1);
    }
  }
  return returnValue;
}

int vas_atoi(char *str) {
  int length = vas_strlen(str);
  int returnValue = 0;
  int sign = 1;
  int i = 0;
  if (str[i] == '+' || str[i] == '-') {
    if (str[i] == '-') {
      sign = -1;
    }
    i++;
  }
  for (; i < length; i++) {
    if (!(str[i] >= '0' && str[i] <= '9')) {
      i = length;
    } else {
      returnValue = returnValue * 10 + (str[i] - 48);
    }
  }
  return sign * returnValue;
}

short int vas_atoiShort(char *str) {
  int length = vas_strlen(str);
  short int returnValue = 0;
  int sign = 1;
  int i = 0;
  if (str[i] == '+' || str[i] == '-') {
    if (str[i] == '-') {
      sign = -1;
    }
    i++;
  }
  for (; i < length; i++) {
    if (!(str[i] >= '0' && str[i] <= '9')) {
      i = length;
    } else {
      returnValue = returnValue * 10 + (str[i] - 48);
    }
  }
  return sign * returnValue;
}

long int vas_atoiLong(char *str) {
  int length = vas_strlen(str);
  long int returnValue = 0;
  int sign = 1;
  int i = 0;
  if (str[i] == '+' || str[i] == '-') {
    if (str[i] == '-') {
      sign = -1;
    }
    i++;
  }
  for (; i < length; i++) {
    if (!(str[i] >= '0' && str[i] <= '9')) {
      i = length;
    } else {
      returnValue = returnValue * 10 + (str[i] - 48);
    }
  }
  return sign * returnValue;
}

double vas_atof(char *str) {
  double returnValue = 0;
  int counterE = 0;
  int sign = 1;
  int i = 0;
  bool exit = false;
  if ((str[i] == 'i' || str[i] == 'I') &&
      (str[i + 1] == 'n' || str[i + 1] == 'N') &&
      (str[i + 2] == 'f' || str[i + 2] == 'F')) {
    returnValue = INFINITY;
  } else if ((str[i] == 'N' || str[i] == 'n') &&
             (str[i + 1] == 'a' || str[i + 1] == 'A') &&
             (str[i + 2] == 'N' || str[i + 2] == 'n')) {
    returnValue = NAN;
  } else {
    if (str[i] == '+' || str[i] == '-') {
      if (str[i] == '-') {
        sign = -1;
      }
      i++;
    }
    if (str[i] == '.') {
      exit = true;
    }
    while (str[i] >= '0' && str[i] <= '9' && !exit) {
      returnValue = returnValue * 10 + (str[i] - 48);
      i++;
    }
    if (str[i] == '.') {
      i++;
    }
    int pow1 = 1;
    while (str[i] >= '0' && str[i] <= '9') {
      returnValue = returnValue + (str[i] - 48.0) / (10 * pow1);
      i++;
      pow1 = pow1 * 10;
    }
    if (str[i] == 'e') {
      i++;
    }
    int signE = 1;
    if (str[i] == '+' || str[i] == '-') {
      if (str[i] == '-') {
        signE = -1;
      }
      i++;
    }
    while (str[i] >= '0' && str[i] <= '9') {
      counterE = counterE * 10 + (str[i] - 48);
      i++;
    }
    returnValue = returnValue * pow(10, signE * counterE);
  }
  return sign * returnValue;
}

long double vas_atofLong(char *str) {
  long double returnValue = 0;
  int counterE = 0;
  int sign = 1;
  int i = 0;
  bool exit = false;
  if ((str[i] == 'i' || str[i] == 'I') &&
      (str[i + 1] == 'n' || str[i + 1] == 'N') &&
      (str[i + 2] == 'f' || str[i + 2] == 'F')) {
    returnValue = INFINITY;
  } else if ((str[i] == 'N' || str[i] == 'n') &&
             (str[i + 1] == 'a' || str[i + 1] == 'A') &&
             (str[i + 2] == 'N' || str[i + 2] == 'n')) {
    returnValue = NAN;
  } else {
    if (str[i] == '+' || str[i] == '-') {
      if (str[i] == '-') {
        sign = -1;
      }
      i++;
    }
    if (str[i] == '.') {
      exit = true;
    }
    while (str[i] >= '0' && str[i] <= '9' && !exit) {
      returnValue = returnValue * 10 + (str[i] - 48);
      i++;
    }
    if (str[i] == '.') {
      i++;
    }
    int pow1 = 1;
    while (str[i] >= '0' && str[i] <= '9') {
      returnValue = returnValue + (str[i] - 48.0) / (10 * pow1);
      i++;
      pow1 = pow1 * 10;
    }
    if (str[i] == 'e') {
      i++;
    }
    int signE = 1;
    if (str[i] == '+' || str[i] == '-') {
      if (str[i] == '-') {
        signE = -1;
      }
      i++;
    }
    while (str[i] >= '0' && str[i] <= '9') {
      counterE = counterE * 10 + (str[i] - 48);
      i++;
    }
    returnValue = returnValue * pow(10, signE * counterE);
  }
  return sign * returnValue;
}
