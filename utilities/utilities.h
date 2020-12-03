#ifndef UTILITIES_H
#define UTILITIES_H

//librerias y headers

#include "../structHeader.h"

//forward funciones utiles
void gotoxy(Point point);
int getRandomIntInRange(int min, int max);

string getTimeStamp();
string colorANSI(string text, int format, int foreground, int background);
int getUnformattedANSINumber(string formattedANSINumber);
StringVector splitString(string delimiter, string text);
StringVector stringRectangle(Area area, int style);

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