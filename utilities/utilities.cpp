#include "utilities.h"

//::::::::::::::::::::::::::::::::::::::
//            UTILIDADES
//::::::::::::::::::::::::::::::::::::::

/**
  * Devuelve numero random generado usando rand
  * 
  * @param int min el minimo a retornar
  * @param int max el maximo a retornar
  * @return int el numero random generado
  *
  */
int getRandomIntInRange(int min, int max)
{
  return rand() % (max - min + 1) + min;
}

/**
  * Devuelve numero si
  * 
  * @param string formattedANSINumber el numero con colores o sin colores 
  * @return int el numero sin formato
  *
  */
int getUnformattedANSINumber(string formattedANSINumber)
{
  int unformattedNumber = 0;

  StringVector tokens = splitString("m", formattedANSINumber);

  if (tokens.lineCount == 0)
    unformattedNumber = stoi(formattedANSINumber);
  else
    unformattedNumber = stoi(tokens.content[1]);

  return unformattedNumber;
}

/**
  * Devuelve el tiempo del sistema
  * 
  * @param void
  * @return string el tiempo del sistema
  *
  */
string getTimeStamp()
{
  time_t now = time(NULL);
  tm *timeStamp = localtime(&now);

  char time[32];

  // Sa, 31.12.2020 20:20:00
  strftime(time, 32, "%a, %d.%m.%Y %H:%M:%S", timeStamp);

  return string(time);
}

/**
  * Cambia la localizacion de el siguiente output en la consola
  * 
  * @param int x la columna en la consola
  * @param int y el renglon en la consola
  * @return void 
  *
  */
void gotoxy(Point point)
{
  HANDLE hCout = GetStdHandle(STD_OUTPUT_HANDLE);

  COORD coord;
  coord.X = point.x;
  coord.Y = point.y;

  SetConsoleCursorPosition(hCout, coord);
}

/**
  * Para dar formato a cadenas de texto bajo el estandard ACSII
  * 
  * @param ColorString text el texto a modificar bajo el estandard ACSII
  * @return string el texto con color en instruccion ACSII codigos de escape @see https://en.wikipedia.org/wiki/ANSI_escape_code
  * 
  */
string colorANSI(string text, int format, int foreground, int background)
{

  return "\033[" +
         ((format >= 1 && format <= 3) ? to_string(format) : "0") + ';' +
         (foreground >= 0 && foreground <= 255 ? "38;5;" + to_string(foreground) : "39") + ';' +
         (background >= 0 && background <= 255 ? "48;5;" + to_string(background) : "49") + "m" +
         text +
         "\033[0m"; //clean after
}

/**
  * Para obtener strings separados con un delimitante desde un string
  * 
  * @param string delimiter el separador
  * @param string text el texto a saparar
  * @return StringVector el texto como vector separado 
  */
StringVector splitString(string delimiter, string text)
{
  string textCpy = text;
  size_t pos = 0;
  int i = 0;

  //cuantos tokens hay en el string
  for (i = 0; (pos = textCpy.find(delimiter)) != string::npos; i++)
    textCpy.erase(0, pos + delimiter.length());

  //todos los items separados por el delimeter
  StringVector content;
  content.lineCount = i;
  content.content = new string[i];

  for (i = 0; (pos = text.find(delimiter)) != string::npos; i++)
  {
    //guarda el token
    content.content[i] = text.substr(0, pos);
    //borra lo guardado en texto para seguir cargando el siguiente
    text.erase(0, pos + delimiter.length());
  }

  return content;
}

/**
  * Calcula un rectangulo linea por linea como vector de strings
  * 
  * @param Area area del rectangulo, 
  * @param int style numero de bordes (1 o 2)
  * @return string * un rectangulo linea por linea como vector de strings
  * 
  */
string *stringRectangle(Area area, int style)
{
  //aqui vamos a dibujar los renglones que se ocupan para el rectangulo
  string *canvas = new string[area.height];
  //un guard para que el style no deje de ser 2 o 1, el 1 es default
  style = (style != 2 && style != 1) ? 0 : (style - 1) * 6;

  //los chars que ocupamos para dibujar rectangulos
  char box[12] = {
      '\xDA', // ┌
      '\xBF', // ┐
      '\xC0', // └
      '\xD9', // ┘
      '\xB3', // │
      '\xC4', // ─
      '\xC9', // ╔
      '\xBB', // ╗
      '\xC8', // ╚
      '\xBC', // ╝
      '\xBA', // ║
      '\xCD', // ═
  };

  //el borde horizontal
  string Line(area.width - 2, box[style + 5]); //a line

  //fill del rectangulo con ' '
  string Fill(area.width - 2, ' ');

  //el borde horizontal superior
  canvas[0] = box[style] + Line + box[style + 1] + '\n';

  //los bordes verticales y el fill
  for (int a = 1; a < area.height - 1; a++)
    canvas[a] = box[style + 4] + Fill + box[style + 4] + '\n';

  //el borde horizontal inferior
  canvas[area.height - 1] = box[style + 2] + Line + box[style + 3];

  //rectangulo construido
  return canvas;
}
