#ifndef STRUCTHEADER_H //begin header guard
#define STRUCTHEADER_H

/*
::::::::::::::::::::::::::::::::::::::
        HEADERS Y NAMESPACES
::::::::::::::::::::::::::::::::::::::
*/

#include <Windows.h>
#include <string>   //getline
#include <string.h> //stoi()
#include <math.h>
#include <iostream>
#include <iomanip> //setw()
#include <fstream> //w r
#include <ctime>   //timestamps
#include <climits>
#include <cctype> //isspace

//namespaces
using namespace std; //mala practica al importar el namespace std hacia el global namespace

/*
::::::::::::::::::::::::::::::::::::::
        ESTRUCTURAS COMPUESTAS 
::::::::::::::::::::::::::::::::::::::
*/
enum GameType
{
    kHorizontal = 1,
    kFullCard,
    kMainDiagonal
};

/**
 * Una estructura para representar una cordenada  
 */
struct Point
{
    int x;
    int y;
};

/**
 * Una estructura para representar una area  
 */
struct Area
{
    int width;
    int height;
};

/**
 * Una estructura para representar contenidos a forma de vectores de string
 */
struct StringVector
{
    int lineCount;   //para saber cuantas lineas hay que imprimir
    string *content; //el contenido de cada linea
};

/**
 * Una estructura para representar contenidos a forma de matriz de string
 */
struct StringMatrix
{
    int height;       //para saber cuantas lineas hay que imprimir
    int width;        //para saber cuantas lineas hay que imprimir
    string **content; //el contenido de cada linea
};

/**
 * Una estructura para representar contenidos a forma de vectores de int
 */
struct IntVector
{
    int lineCount; //para saber cuantas lineas hay que imprimir
    int *content;  //el contenido de cada linea
};

/**
 * Una estructura para representar contenidos a forma de matriz de int
 */
struct IntMatrix
{
    int height;    //para saber cuantas lineas hay que imprimir
    int width;     //para saber cuantas lineas hay que imprimir
    int **content; //el contenido de cada linea
};

/**
 * Una estructura para representar el menu
 */
struct Menu
{
    bool enabled;       //para saber si puede acceder
    string text;        //el texto de la opcion
    string query;       //lo que debe preguntar el submenu
    int itemCount;      //el numero de opciones en el submenu
    struct Menu *items; //opciones
};

/**
 * Una estructura para representar un jugador de bingo
 */
struct Player
{
    string name;       //alias
    StringMatrix card; //la tarjeta del jugador como matriz de estructuras AcsiiText
    IntMatrix scored;  //los numeros del bingo
    int score;
    int scoredCount;
    string timestamp; //tiempo desde el sistema al ganar
};

/**
 * Una estructura para representar el juego de bingo
 */
struct BingoGame
{
    string title;
    Menu menu;
    GameType gameType; //tipo de juego
    Player *players;   //los jugadores
    IntVector numbers; //los numeros del bingo
};

#endif //end header guard