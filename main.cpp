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

#include "views/card/cardviews.h" //aqui estan todos los recursos como gotoxy(x, y) y color(text)

//forwards
BingoGame initBingo();
bool bingoCore(BingoGame &game);
void handleAlias(BingoGame &game);
void handleCards(BingoGame &game);
bool searchCard(StringMatrix &card, int number, GameType gametype);
bool checkPlayerVictory(Player &player, GameType gametype);
void handleStats(BingoGame &game);

//::::::::::::::::::::::::::::::::::::::
//                MAIN
//::::::::::::::::::::::::::::::::::::::

int main(int argc, char const *argv[])
{
    //song se lo quite porque aburre
    // mciSendString(TEXT("play resources\\all.mp3 repeat"), NULL, 0, NULL);

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
    {
        //al final de ejecutar bingocore decidir si ejecutar otra vez
        canRepeat = bingoCore(game);
    }

    //limipia la consola antes de cerrar la ejecucion
    system("cls");

    //retorna 0 y termina la ejecucion
    return 0;
}

BingoGame initBingo()
{
    BingoGame game;

    //es el titulo del frame en rojo
    game.title = "Easy Bingo Inc. 2020";

    //los jugadores, hacer espacio para solo 2
    game.players = new Player[2];

    //leer menu desde disco
    StringVector menuFile = readFile("data/menu.txt");
    //leer el sunmenu de estadisticas desde dico
    StringVector subMenuFile = readFile("data/submenu.txt");

    //hacer menu con menuFile
    game.menu.items = new Menu[menuFile.lineCount];
    game.menu.itemCount = menuFile.lineCount;

    for (int i = 0; i < menuFile.lineCount; i++)
    {
        game.menu.items[i].text = menuFile.content[i];
        // game.menu.items[i].enabled = i == 0 || i == 5;
        game.menu.items[i].enabled = true;

        //estadisticas
        if (i == 4)
        {

            //hacer menu con menuFile
            game.menu.items[i].items = new Menu[subMenuFile.lineCount];
            game.menu.items[i].itemCount = subMenuFile.lineCount;

            for (int j = 0; j < subMenuFile.lineCount; j++)
            {
                game.menu.items[i].items[j].text = subMenuFile.content[j];
                game.menu.items[i].items[j].enabled = true;
            }

            //propiedades del submenu que no existen en subMenuFile
            game.menu.items[i].query = "Cual opcion desea?: ";
        }
    }

    //propiedades del menu que no existen en menuFile
    game.menu.text = "Menu Principal";
    game.menu.query = "Cual opcion desea?: ";

    return game;
}

bool bingoCore(BingoGame &game)
{
    //si no cambia se repite todo
    bool canRepeat = true;
    bool isPlaying = false;

    //hacer gui
    displayFrame(game.title);

    //imprimir menu en body
    displayMenu(game.menu);

    //obtener opcion
    int selectedMain = stoi(getQueryMenu(game.menu));

    //llama las funciones usando la seleccion del menu
    switch (selectedMain)
    {
    case 1:
        //cambiar los aliases
        handleAlias(game);
        break;
    case 2:
        game.gameType = kHorizontal;
        isPlaying = true;
        break;
    case 3:
        game.gameType = kFullCard;
        isPlaying = true;
        break;
    case 4:
        game.gameType = kMainDiagonal;
        isPlaying = true;
        break;
    case 5:
    {
        //para opciones de estadisticas
        handleStats(game);
        break;
    }
    case 6:
    {
        //salir del juego
        canRepeat = false;
        break;
    }
    }

    if (isPlaying)
    {
        //jugador ganador
        int pos;

        //hacer cartas
        handleCards(game);

        //los revuelve otra vez antes de iniciar porque si no se sigue el orden en que se llenan
        shuffleIntVector(game.numbers);

        //detectar encontrado
        bool found = false;

        //detectar ganador
        bool ended = false;

        //recorrer banco de numeros que mide 100 (100 numeros)
        for (int i = 0; i < 100; i++)
        {

            //limpiar frame body
            cleanFrameBody();

            //mostrar tarjetas
            displayCards(game.players[0], game.players[1]);

            //presentar el numero que sale
            callNumber("Salio el numero ", game.numbers, i, 50);

            //mostrar tarjetas
            displayCards(game.players[0], game.players[1]);

            //revisar jugadores, 2 jugadores
            for (int j = 0; j < 2; j++)
                //revisar si tiene numero
                if ((found = searchCard(game.players[j].card, game.numbers.content[i], game.gameType)))
                    //revisar si gana
                    if ((ended = checkPlayerVictory(game.players[j], game.gameType)))
                    {
                        pos = j;
                        break;
                    }

            if (ended)
            {
                //limpiar frame body
                cleanFrameBody();

                //presentar el numero que sale
                callNumber("Salio el numero ", game.numbers, i, 50);

                //mostrar tarjetas
                displayCards(game.players[0], game.players[1]);

                //dar reporte de juego para cada jugador
                for (int j = 0; j < 2; j++)
                    displayReport(game.players[j], {9 + (j == 0 ? 0 : 50), 22});

                //output de felicitacion
                callPlayer("Felicidades # !!!! Acumulaste: # puntos. #", game.players[pos], 4000);

                //guardar datos del ganador en txt
                writeScoreData("data/leaderboard.txt", game.players[pos]);
                break;
            }
        }
    }

    //en caso de error no repetir
    return canRepeat;
}

void handleAlias(BingoGame &game)
{
    // for (int i = 0; i < 4; i++)
    // {
    //     game.menu.items[i + 1].enabled = true;
    // }

    for (int i = 0; i < 2; i++)
    {
        displayAlias(game.players[0].name, game.players[1].name);
        game.players[i].name = getQueryAlias(
            string("Nombre del Jugador" +
                   string((i == 0)
                              ? " A: "
                              : " B: ")));
    }
}

void handleCards(BingoGame &game)
{
    //obtener los numeros del 1 al 100 y revolver
    game.numbers = getVectorFromRange(1, 100);
    shuffleIntVector(game.numbers);

    //hacer dimension random entre 3 y 6
    int randomDimension = getRandomIntInRange(3, 6);
    //controlar los numeros no 0s por renglon
    int numbersPerColumn = game.gameType == 3 ? randomDimension : game.gameType == 2 ? 9 : 5;
    //controlar los numeros de donde incia el llenado de la siguiente carta
    int skip = game.gameType == 3 ? randomDimension * randomDimension : game.gameType == 2 ? 27 : 15;

    for (int i = 0; i < 2; i++)
    {

        //la tarjeta de bingo
        game.players[i].card.height = game.gameType == 3 ? randomDimension : 3;
        game.players[i].card.width = game.gameType == 3 ? randomDimension : 9;

        //tarjeta scored
        int width = game.players[i].scored.width = numbersPerColumn;
        int height = game.players[i].scored.height =
            game.gameType != 1 ? 3 : 1;

        //memoria dinamica de matriz scored default en 0s, los 0s no valen
        game.players[i].scored.content = new int *[height];
        for (int j = 0; j < height; j++)
        {
            game.players[i].scored.content[j] = new int[width];
            for (int k = 0; k < width; k++)
                game.players[i].scored.content[j][k] = 0;
        }

        //obtener tarjetas de manera dinamica
        getCardFromVector(game.players[i].card, game.numbers, numbersPerColumn, i == 0 ? i : skip);
    }
}

bool searchCard(StringMatrix &card, int number, GameType gametype)
{
    //revisar si esta el numero
    bool found = false;

    //el contrario de i
    int ri = card.height - 1;
    //recorre la matriz card.content
    for (int i = 0; i < card.height; i++)
    {
        for (int j = 0; j < card.width; j++)
        {
            //el numero de la tarjeta sin formato
            int cardNumber = getUnformattedANSINumber(card.content[i][j]);

            //revisar si lo encontro
            if ((found = gametype != 3 ? cardNumber == number : cardNumber == number && (j == i || j == ri)))
            {
                //pinta el numero encontrado de verde
                card.content[i][j] = colorANSI(card.content[i][j], 1, 10, 256);
                //termina el loop j; si ya se encontro
                break;
            }
        }

        //recorre ri
        ri--;
        //termina el loop i y ri; si ya se encontro
        if (found)
            break;
    }

    //regresar si lo encontro
    return found;
}

bool checkPlayerVictory(Player &player, GameType gametype)
{
    //inicializar player
    player.score = 0;
    player.scoredCount = 0;
    player.timestamp = getTimeStamp();

    //detectar ganador
    bool ended = false;

    //cuantos aciertos
    int scoredNumber = 0;
    int rscoredNumber = 0;

    //cuantos puntos
    int rscore = 0;
    int score = 0;

    //cuantas filas horizontales completa para cartas normales
    int scoredrows = 0;

    //valor central para diagonales simetricas
    int center = 0;

    int ri = player.card.height - 1;
    for (int i = 0; i < player.card.height; i++)
    {
        //si es modo diagonal mantener el valor
        scoredNumber = (gametype != 3) ? 0 : scoredNumber;

        //si es modo horizontal hacer 0
        player.score = (gametype == 1) ? 0 : player.score;

        for (int j = 0; j < player.card.width; j++)
        {
            //ver si tiene color
            bool formatted = isFormatted(player.card.content[i][j]);
            int number = getUnformattedANSINumber(player.card.content[i][j]);

            //tiene color
            if (formatted)
            {

                if (gametype != 3)
                {

                    //cuando encuentra un numero en verde aumentar los aciertos
                    player.scoredCount++;

                    //sumarlo a los puntos
                    player.score += number;

                    //guardar en scored
                    player.scored.content[gametype == 2 ? i : 0][scoredNumber] = number;

                    //avanzar numero de aciertos
                    scoredNumber++;

                    //si ya tiene los aciertos que se ocupan en el renglon
                    if (scoredNumber == player.scored.width)
                        //aumentar el numero de renglones acertados
                        scoredrows++;

                    //reportar vicoria
                    if ((ended = (scoredrows == player.scored.height)))
                        break;
                }
                else if (gametype == 3)
                {

                    if (i == j && ri != i)
                    {
                        //diagonales primarias; no centros
                        player.scored.content[0][scoredNumber] = number;
                        score += number;
                        scoredNumber++;
                    }
                    else if (ri == j && ri != i)
                    {
                        //diagonales secundarias; no centros
                        player.scored.content[1][rscoredNumber] = number;
                        rscore += number;
                        rscoredNumber++;
                    }
                    else if (ri == i && center == 0)
                    {
                        //centros
                        player.scored.content[2][0] = number;
                        center = number;
                    }

                    //contar aciertos
                    player.scoredCount = rscoredNumber + scoredNumber + (center == 0 ? center : 1);

                    //reportar victoria
                    if ((ended = (center == 0 && (rscoredNumber == player.scored.width ||
                                                  scoredNumber == player.scored.width)) ||
                                 (center != 0 && (rscoredNumber == player.scored.width - 1 ||
                                                  scoredNumber == player.scored.width - 1))))
                    {
                        //hacer suma de puntos
                        if ((rscoredNumber == player.scored.width ||
                             rscoredNumber == player.scored.width - 1))
                            player.score = rscore + center;
                        else
                            player.score = score + center;

                        //dejar de hacer calculos si ya hay ganador
                        break;
                    }
                }
            }
        }

        ri--;

        //si solo se ganan tantas filas como se ocupan ganar juego
        if (ended)
            break;
    }

    return ended;
}

void handleStats(BingoGame &game)
{
    displayMenu(game.menu.items[4]);
    //para obtener input
    int selectedStats = stoi(
        getQueryMenu(
            game.menu.items[4]));

    switch (selectedStats)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        //no se hace nada
        break;
    }
}