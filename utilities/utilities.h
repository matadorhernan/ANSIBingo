#ifndef UTILITIES_H
#define UTILITIES_H

//librerias y headers

#include "../structHeader.h"

//forward funciones utiles
int getRandomIntInRange(int min, int max);
int getUnformattedANSINumber(string formattedANSINumber);
string getTimeStamp();
void gotoxy(Point point);
string colorANSI(string text, int format, int foreground, int background);
StringVector splitString(string delimiter, string text);
string *stringRectangle(Area area, int style);

//forward vector utilities
void shuffleIntVector(IntVector &vector);
void shuffleStringVector(StringVector &vector);
IntVector getVectorFromRange(int min, int max);
void getCardFromVector(StringMatrix &card, IntVector vector, int numbersPerColumn, int skip);

//forward file utilities
StringVector readFile(string filename);
void writeFile(string filename, StringVector content);
void writeScoreData(string filename, Player player);

#endif