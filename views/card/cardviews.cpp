#include "cardviews.h"

/**
  * Imprime una carta de area 'area' en el punto 'point'
  * 
  * @param Player player el jugador de la carta
  * @param Point point el punto donde se imprime
  * @return void
  */
void displayCard(Player player, Point point)
{
    for (int i = 0; i < player.card.height; i++)
    {
        for (int j = 0; j < player.card.width; j++)
        {
            gotoxy({point.x + (4 * j), point.y + (2 * i)});
            cout << setw(4) << player.card.content[i][j];
        }
    }
}

void callNumber(string text, int number, int duration)
{
    //limpia el input
    cleanFrameInput();

    //imprimir la llamada de numero en consola
    gotoxy({6, 33});
    cout << colorANSI(string(text + to_string(number)), 1, 33, 256);

    Sleep(duration);
}

/**
 * Muestra las dos cartas
 * 
 * @param Player playerA primer jugador
 * @param Player playerB segundo jugador
 * @return void
 */
void displayCards(Player playerA, Player playerB)
{
    //mostar alias
    gotoxy({9, 8});
    cout << colorANSI(playerA.name.empty() ? "Jugador A" : playerA.name + " (Jugador A)", 1, 196, 256);
    gotoxy({9 + 70, 8});
    cout << colorANSI(playerB.name.empty() ? "Jugador B" : playerB.name + " (Jugador B)", 1, 33, 256);

    displayCard(playerA, {9, 10});
    displayCard(playerB, {70 + 9, 10});
}

void displayReport(Player player, Point point)
{

    //para hacer la lista
    int k, l;
    k = l = 0;

    //para cada numero acertado
    for (int i = 0; i < player.scored.height; i++)
        for (int j = 0; j < player.scored.width; j++)
            if (player.scored.content[i][j] != 0)
            {
                if (k == 9)
                {
                    k = 0;
                    l++;
                }
                //Lista de numeros A
                gotoxy({point.x + (4 * (k)) + 20, point.y + 4 + l});
                cout << left << setw(4) << player.scored.content[i][j];
                //conteo de aciertos A

                k++;
            }

    //tiempo
    gotoxy({point.x, point.y});
    cout << left << setw(20)
         << "Tiempo: " << player.timestamp;

    //Reporte
    gotoxy({point.x, point.y + 2});
    cout << left << setw(20)
         << "Aciertos: " << player.scoredCount;
    gotoxy({point.x, point.y + 3});
    cout << left << setw(20)
         << "Puntos: " << player.score;

    gotoxy({point.x, point.y + 4});
    cout << left << setw(20)
         << "Lista de Numeros: ";
}

void callPlayer(string text, Player player, int duration)
{
    //string
    StringVector tokens = splitString("#", text);

    //limpia el input
    cleanFrameInput();

    //imprimir la llamada de numero en consola
    gotoxy({6, 33});
    cout << colorANSI(
        tokens.content[0] +
            (player.name.empty() ? "Jugador" : player.name) +
            tokens.content[1] +
            to_string(player.score) +
            tokens.content[2],
        1, 33, 256);
    system("pause");
}