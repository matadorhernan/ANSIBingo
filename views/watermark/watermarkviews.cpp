#include "watermarkviews.h"

/**
  * Imprime una vista con la marca de agua, el logo
  * 
  * @param void
  * @return void
  */
void displayWatermark()
{
  //limpia la consola
  system("cls");

  //guarda el contenido de logo.txt en content
  StringVector content = readFile("data/logo.txt");

  //imprime content en consola, ex. cada linea del txt
  for (int i = 0; i < content.lineCount; i++)
  {
    //gotoxy para dejar espacios
    gotoxy({18, i + 8});
    cout << colorANSI(content.content[i], 1, i < 20 ? 32 + i : 256, 256);
  }

  gotoxy({60, 30});
  system("pause");

  //limpia la consola
  system("cls");
}
