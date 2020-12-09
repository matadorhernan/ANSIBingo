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
    player.timestamp = getTimeStamp();

    //detectar ganador
    bool ended = false;

    //aciertos normal
    int scoredCount = 0;
    //aciertos diagonal secundaria
    int rscoredCount = 0;
    //puntos normal
    int score = 0;
    //puntos diagonal secundaria
    int rscore = 0;
    //lineas completadas
    int scoredrows = 0;
    //valor central para matrices con centros
    int center = 0;

    //para recorrer content al revez
    int ri = player.card.height - 1;

    //recorrer content
    for (int i = 0; i < player.card.height; i++)
    {
        //si es modo diagonal mantener el valor
        scoredCount = (gametype != 3) ? 0 : scoredCount;
        //si es modo horizontal hacer 0
        player.score = (gametype == 1) ? 0 : player.score;

        for (int j = 0; j < player.card.width; j++)
        {

            //si tiene color el content[i][j]
            if ((isFormatted(player.card.content[i][j])))
            {
                //valor int de content[i][j]
                int number = getUnformattedANSINumber(player.card.content[i][j]);

                if (gametype != 3)
                {
                    //aumentar aciertos
                    scoredCount++;

                    //sumar content[i][j] a los puntos
                    player.score += number;

                    //guardar en scored
                    player.scored.content[gametype == 2 ? i : 0][scoredCount] = number;

                    //si ya tiene los aciertos que se ocupan en el renglon
                    if (scoredCount == player.scored.width)
                        //aumentar el numero de renglones acertados
                        scoredrows++;

                    //reportar vicoria
                    if ((ended = (scoredrows == player.scored.height)))
                        break;
                }
                else if (gametype == 3)
                {

                    //encontrado en diagonal primaria, no en centro
                    if (i == j && ri != i)
                    {
                        //registar coincidencia en diagonales primarias
                        player.scored.content[0][scoredCount] = number;
                        //sumar a la cuenta normal
                        score += number;
                        //aumentar aciertos
                        scoredCount++;
                    }
                    //encontrado en diagonal secundaria, no en centro
                    else if (ri == j && ri != i)
                    {
                        //registar coincidencia en diagonales secundarias
                        player.scored.content[1][rscoredCount] = number;
                        //sumar a la cuenta secundaria
                        rscore += number;
                        //aumentar aciertos diagonal secundaria
                        rscoredCount++;
                    }
                    //encontrado en centro
                    else if (ri == i && center == 0)
                    {
                        //registar coincidencia en centros
                        player.scored.content[2][0] = number;
                        //preparar para sumar valor del centro
                        center = number;
                    }

                    //reportar victoria (en matrices con centros y sin centros) de diagonales principales o secundarias
                    if ((ended = (center == 0 && (scoredCount == player.scored.width ||
                                                  rscoredCount == player.scored.width)) ||
                                 (center != 0 && (scoredCount == player.scored.width - 1 ||
                                                  rscoredCount == player.scored.width - 1))))
                    {
                        //hacer suma de puntos si gano con diagonal principal
                        if ((scoredCount == player.scored.width ||
                             scoredCount == player.scored.width - 1))
                            player.score = score + center;
                        //hacer suma de puntos si gano con diagonal secundaria
                        else
                            player.score = rscore + center;

                        //dejar de hacer calculos si ya hay ganador
                        break;
                    }
                }
            }
        }

        //avanzar anti i
        ri--;

        //dejar de hacer calculos si ya hay ganador
        if (ended)
            break;
    }

    //reportar si gano
    return ended;
}
