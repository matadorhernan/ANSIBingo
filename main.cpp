/**
  *  LICENCE
  *    None
  *
  *  NAME
  *    Bingo Inc 2020
  *
  *  DEPENDENCIES
  *    You must run this project with a Windows 10 + system and run the command specified bellow RUN
  *
  *  DESCRIPTION
  *    A funny bingo game, documented using c++ standard
  *    @see isocpp.org
  *
  *  ASSIGNMENT
  *    Assignment Type: Final Project
  *    Course: Programming I
  *    Date: 22/11/2020
  *
  *  RUN
  *    nmake bingo
  *
  *  METADATA
  *    @author   Rodriguez Moreno
  *    @author   Romero Hernandez
  *    @author   Rojas Arteaga
  *    @author   Nunez Michaus
  *   
  *    @since    22 November 2020   
  *    @version  1.0
  * 
  */

//::::::::::::::::::::::::::::::::::::::
//              HEADERS
//::::::::::::::::::::::::::::::::::::::

#include "core/bingocore.h" //aqui estan el bingo

//::::::::::::::::::::::::::::::::::::::
//                MAIN
//::::::::::::::::::::::::::::::::::::::

int main(int argc, char const *argv[])
{
  //song se lo quite porque aburre
  mciSendString(TEXT("play resources\\all.mp3 repeat"), NULL, 0, NULL);

  //para los rand()
  srand(time(NULL));

  //iniciamos repitiendo la ejecucion
  bool canRepeat = true;

  //abrir un juego
  BingoGame game = initBingo();

  //muestra el logo y pausa
  displayWatermark();

  //repeticion del bingo si regresa falso entonces no
  while (canRepeat)
    //al final de ejecutar bingocore decidir si ejecutar otra vez
    canRepeat = bingoCore(game);

  //limipia la consola antes de cerrar la ejecucion
  system("cls");

  //retorna 0 y termina la ejecucion
  return 0;
}
