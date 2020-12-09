#include "bingocore.h"

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
        game.menu.items[i].enabled = i == 0 || i == 5;

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
    //repetir todo
    bool canRepeat = true;

    //modo jugar
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
        bool canSubRepeat = true;
        while (canSubRepeat)
            canSubRepeat = handleStats(game);
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
            callNumber("Salio el numero ", game.numbers, i, REFRESH_RATE);

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
                Score score;
                score.gameType = (game.gameType == 3 ? "Diagonal" : game.gameType == 2 ? "Carta Completa" : "Horizontal");
                score.name = game.players[pos].name;
                score.score = to_string(game.players[pos].score);
                score.timestamp = game.players[pos].timestamp;

                writeScoreData("data/leaderboard.txt", score);
                break;
            }
        }
    }

    //devolver si se repite
    return canRepeat;
}
