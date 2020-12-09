#include "leaderboardviews.h"

void displayStats(ScoreVector vector)
{
    // limpia el body
    cleanFrameBody();

    //cuadros para los titulos de la tabla de puntuaje
    StringVector nameRectangle = stringRectangle({36, 3}, 1);
    StringVector gameTypeRectangle = stringRectangle({29, 3}, 1);
    StringVector scoreRectangle = stringRectangle({19, 3}, 1);
    StringVector dateRectangle = stringRectangle({55, 3}, 1);

    //dibujar rectangulos
    for (int i = 0; i < nameRectangle.lineCount; i++)
    {
        //dibujar rectangulo para titulo alias
        gotoxy({4, 6 + i});
        cout << nameRectangle.content[i];
        //dibujar rectangulo para titulo modo
        gotoxy({40, 6 + i});
        cout << gameTypeRectangle.content[i];
        //dibujar rectangulo para titulo puntos
        gotoxy({69, 6 + i});
        cout << scoreRectangle.content[i];
        //dibujar rectangulo para titulo fecha
        gotoxy({88, 6 + i});
        cout << dateRectangle.content[i];
    }

    //imprimir titulos
    gotoxy({6, 7});
    cout << "Alias";
    gotoxy({42, 7});
    cout << "Modo";
    gotoxy({71, 7});
    cout << "Puntos";
    gotoxy({90, 7});
    cout << "Fecha";

    //calcular paginas
    int pages = (int)ceil((double)(((double)vector.itemCount) / ((double)PAGE_LIMIT)));

    //imprimir content de vector
    for (int i = 0; i < pages; i++)
    {
        for (int j = 0; j < PAGE_LIMIT && (i * PAGE_LIMIT) + j != vector.itemCount; j++)
        {
            gotoxy({6, 9 + j});
            cout << vector.score[(i * PAGE_LIMIT) + j].name;
            gotoxy({42, 9 + j});
            cout << vector.score[(i * PAGE_LIMIT) + j].gameType;
            gotoxy({71, 9 + j});
            cout << vector.score[(i * PAGE_LIMIT) + j].score;
            gotoxy({90, 9 + j});
            cout << vector.score[(i * PAGE_LIMIT) + j].timestamp;
        }

        //avisa si existen mas paginas
        if (i != pages - 1)
        {
            cleanFrameInput();
            gotoxy({6, 33});
            cout << "Imprimiendo siguiente pagina...";
            //pausa la ejecucion para ver los resultados
            system("pause");
        }
    }

    //pausa la ejecucion para ver los resultados
    cleanFrameInput();
    gotoxy({6, 33});
    system("pause");
}
