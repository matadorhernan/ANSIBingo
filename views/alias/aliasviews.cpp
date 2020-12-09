#include "aliasviews.h"

/**
 * Muestra dos osos con los nombres de los jugadores
 * 
 * @param string aliasA el alias del primero jugador
 * @param string aliasB el alias del segundo jugador
 * @return void
 */
void displayAlias(string aliasA, string aliasB)
{
    //limpia el body
    cleanFrameBody();

    //Un oso en Characteres de 14x25 chars
    StringVector playerFile = readFile("data/player.txt");

    gotoxy({30, 26});
    cout << colorANSI(aliasA.empty() ? "Jugador A" : aliasA + " (Jugador A)", 1, 196, 256);

    gotoxy({10 + 75, 26});
    cout << colorANSI(aliasB.empty() ? "Jugador B" : aliasB + " (Jugador B)", 1, 33, 256);

    //la pantalla mide 147 * 37
    for (int i = 0; i < playerFile.lineCount; i++)
    {
        //primer oso
        gotoxy({30, 9 + i});
        cout << colorANSI(playerFile.content[i], 1, 196, 256) << endl;

        //segundo oso
        gotoxy({10 + 75, 9 + i});
        cout << colorANSI(playerFile.content[i], 1, 33, 256) << endl;
    }
}

/**
 * Muestra un query y retorna un alias validado
 * 
 * @param string query lo que se le pregunta al usuario
 * @return string el alias
 */
string getQueryAlias(string query)
{
    string alias;
    bool validAlias = false;

    while (!validAlias)
    {

        //limpia el input
        cleanFrameInput();

        //imprimir query en consola
        gotoxy({6, 33});
        cout << query;

        //obtener opcion
        getline(cin, alias);

        //validar opcion
        validAlias = !alias.empty() &&      //no puede estar vacia
                     isAlias(alias, 4, 12); //necesita ser alias

        //dar informacion al usuario
        if (!validAlias)
        {
            gotoxy({6, 33});
            cout << colorANSI("Alias invalido, intentalo de nuevo...", 1, 9, 256);
            Sleep(3000);
        }
    }

    return alias;
}
