
#include "bingocore.h"

void handleAlias(BingoGame &game)
{
    for (int i = 0; i < 4; i++)
        game.menu.items[i + 1].enabled = true;

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

        //sin aciertos y sin puntos
        game.players[i].scoredCount = 0;
        game.players[i].score = 0;

        //obtener tarjetas de manera dinamica
        getCardFromVector(game.players[i].card, game.numbers, numbersPerColumn, i == 0 ? i : skip);
    }
}

bool handleStats(BingoGame &game)
{

    //si se repite
    bool canSubRepeat = true;

    //estadisticas por modo
    bool isgameType = false;
    GameType gameType;

    //carga todo del archivo de puntuajes
    ScoreVector scoreVector = readScoreData("data/leaderboard.txt");

    //muestra submenu
    displayMenu(game.menu.items[4]);

    //obtener input en el submenu stats
    int selectedStats = stoi(
        getQueryMenu(
            game.menu.items[4]));

    switch (selectedStats)
    {
    case 1:
        //handle all
        if (scoreVector.itemCount == 0)
        {
            cleanFrameInput();
            gotoxy({6, 33});
            cout << "Aun no a ganado nadie...";
            system("pause");
            return true;
        }
        displayStats(scoreVector);
        break;
    case 2:
    {
        //handle by player
        string aliasQuery = getQueryAlias("Nombre del jugador a buscar: ");
        ScoreVector filterPlayer;

        //filtra puntuajes por nombre de jugador
        filterPlayer.score = new Score[scoreVector.itemCount];
        filterPlayer.itemCount = 0;

        //recorre vector de puntuajes
        for (int i = 0; i < scoreVector.itemCount; i++)
            if (aliasQuery.compare(scoreVector.score[i].name) == 0)
            {
                filterPlayer.score[filterPlayer.itemCount] = scoreVector.score[i];
                filterPlayer.itemCount++;
            }

        if (scoreVector.itemCount == 0 || filterPlayer.itemCount == 0)
        {
            cleanFrameInput();
            gotoxy({6, 33});
            cout << "Jugador no registrado...";
            system("pause");
            return true;
        }

        displayStats(filterPlayer);
        break;
    }
    case 3:
        //handle horizontal
        isgameType = true;
        gameType = kHorizontal;
        break;
    case 4:
        //handle full card
        isgameType = true;
        gameType = kFullCard;
        break;
    case 5:
        //handle diagonal
        isgameType = true;
        gameType = kMainDiagonal;
        break;
    case 6:
        //salir del loop submenu
        canSubRepeat = false;
        break;
    }

    if (isgameType)
    {
        //filtra puntuajes por typo de juego
        ScoreVector filterGameType;
        string gameTypeQuery = (gameType == 3 ? "Diagonal" : gameType == 2 ? "Carta Completa" : "Horizontal");
        filterGameType.score = new Score[scoreVector.itemCount];
        filterGameType.itemCount = 0;

        //recorre vector de puntuajes
        for (int i = 0; i < scoreVector.itemCount; i++)
            if (gameTypeQuery.compare(scoreVector.score[i].gameType) == 0)
            {
                filterGameType.score[filterGameType.itemCount] = scoreVector.score[i];
                filterGameType.itemCount++;
            }

        //cancela imprimir estadisticas
        if (scoreVector.itemCount == 0 || filterGameType.itemCount == 0)
        {
            cleanFrameInput();
            gotoxy({6, 33});
            cout << "Nadie a ganado en modo " << gameTypeQuery << "...";
            system("pause");
            return true;
        }

        //imprime estadisticas
        displayStats(filterGameType);
    }

    return canSubRepeat;
}