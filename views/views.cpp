#include "views.h"

//::::::::::::::::::::::::::::::::::::::
//                VIEWS
//::::::::::::::::::::::::::::::::::::::

/**
  * Imprime vacios para limpiar la consola en un rectangulo
  * 
  * @param Point start el punto superior izq de inicio el rectangulo
  * @param Point end el punto inferior derecho donde termina el rectangulo
  * 
  * @return void
  */
void cleanView(Point start, Point end)
{
  //limpia area de input
  string cleanLine(end.x - start.x, ' ');

  //imprime espacios en el area start x end
  for (int i = 0; i < end.y - start.y; i++)
  {
    gotoxy({start.x, start.y + i});
    cout << cleanLine;
  }
}

/**
  * Limpia el body
  * 
  * @param void
  * @return void
  */
void cleanFrameBody()
{
  //limpia el body
  cleanView({3, 6}, {144, 30});
}

/**
  * Limpia el input
  * 
  * @param void
  * @return void
  */
void cleanFrameInput()
{
  //limpia el input
  cleanView({3, 32}, {144, 35});
}

/**
  * Imprime una vista de un panel con titulo body y un campo de input
  * 
  * @param string title titulo del frame
  * @return void
  */
void displayFrame(string title)
{

  //limpiar el anterior
  system("cls");

  //variables para graficos
  Area fullScreen = {147, 37}; //la consola mide

  //el borde que rodea todo
  StringVector frame = stringRectangle(fullScreen, 1);
  //el borde de el header en rojo
  StringVector header = stringRectangle({fullScreen.width - 4, 4}, 2);
  //el borde de el body doble borde
  StringVector body = stringRectangle({fullScreen.width - 4, fullScreen.height - 11}, 2);
  //el borde de el input
  StringVector footer = stringRectangle({fullScreen.width - 4, 5}, 1);

  //imprimir rectangulo frame <<el marco de todos los rectangulos>>
  for (int i = 0; i < fullScreen.height; i++)
  {
    gotoxy({0, i});
    cout << frame.content[i];
  }

  //imprimir body
  for (int i = 0; i < fullScreen.height - 11; i++)
  {

    // imprimir header <<el rojo>>
    if (i < 4)
    {
      //llegar al principio del frame
      gotoxy({2, 1 + i});
      //imprimir header
      cout << colorANSI(header.content[i], 1, 192, 9);
    }

    //imprimir body  <<el de en medio>>
    //pocisionar body justo abajo de header
    gotoxy({2, 5 + i});
    //imprimir body mide fullScreen.height - 11 de alto
    cout << body.content[i];

    //imprimir footer <<el de abajo>>
    if (i < 5)
    {
      //llegar a los ultimos 6 renglones
      gotoxy({2, fullScreen.height - 6 + i});
      //imprimir footer
      cout << footer.content[i];
    }
  }

  //hacer titulo
  string titleFrame(fullScreen.width - 7, ' ');
  for (long long unsigned int i = 0; i < title.length(); i++)
    titleFrame.at(i + 3) = title.at(i);

  //imprimir titulo en header
  gotoxy({3, 3});
  cout << colorANSI(titleFrame, 1, 192, 9);
}
