#ifndef VALIDATORS_H
#define VALIDATORS_H

//librerias y headers

#include "../utilities/utilities.h"

//forward funciones validadoras
bool isInRange(int number, int min, int max);
bool isDigits(string digits);
bool isFormatted(string str);
bool isFile(string filename);
bool isAlias(string alias, int min, int max);

#endif