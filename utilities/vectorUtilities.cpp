
#include "utilities.h"

/**
  * Revuelve los contenidos de un vector
  * 
  * @param IntVector contenidos de un vector a revolver
  * @return void 
  * 
  */
void shuffleIntVector(IntVector &vector)
{
    if (vector.lineCount > 1)
        for (int i = 0; i < vector.lineCount - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (vector.lineCount - i) + 1);
            int t = vector.content[j];
            vector.content[j] = vector.content[i];
            vector.content[i] = t;
        }
}

/**
  * Revuelve los contenidos de un vector
  * 
  * @param StringVector contenidos de un vector a revolver
  * @return void 
  * 
  */
void shuffleStringVector(StringVector &vector)
{
    if (vector.lineCount > 1)
        for (int i = 0; i < vector.lineCount - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (vector.lineCount - i) + 1);
            string t = vector.content[j];
            vector.content[j] = vector.content[i];
            vector.content[i] = t;
        }
}

/**
  * Devuelve un vector ordenado de numeros min a max
  * 
  * @param int min el minimo del vector
  * @param int max el maximo del vector
  * @return vector de numeros del minimo al maximo 
  *
  */
IntVector getVectorFromRange(int min, int max)
{
    IntVector vector;
    int length = max - min;

    if (length > 1)
    {
        vector.lineCount = length + 1;
        vector.content = new int[length];

        for (int i = 0; i < length + 1; i++)
            vector.content[i] = min + i;
    }

    return vector;
};

void getCardFromVector(StringMatrix &card, IntVector vector, int numbersPerColumn, int skip)
{

    card.content = new string *[card.height];

    for (int i = 0; i < card.height && i < vector.lineCount; i++)
    {
        card.content[i] = new string[card.width];

        StringVector aux;
        aux.lineCount = card.width;
        aux.content = new string[card.width];

        for (int j = 0; j < card.width; j++)
            aux.content[j] = to_string((j < numbersPerColumn) ? vector.content[j + (i * numbersPerColumn) + skip] : 0);

        shuffleStringVector(aux);

        for (int j = 0; j < card.width; j++)
            card.content[i][j] = aux.content[j];
    }
}
