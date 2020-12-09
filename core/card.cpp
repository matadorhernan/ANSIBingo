#include "bingocore.h"

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
    //cuantos puntos
    //cuantas filas horizontales completa para cartas normales
    //valor central para diagonales simetricas
    int scoredNumber, rscoredNumber, rscore, score, scoredrows, center;
    scoredNumber = rscoredNumber = rscore = score = scoredrows = center = 0;

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
