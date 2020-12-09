//structHeader.h
#include <Windows.h>
#include <string>
#include <string.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <climits>
#include <cctype>
#define REFRESH_RATE 200
#define PAGE_LIMIT 20
using namespace std;
enum GameType
{
    kHorizontal = 1,
    kFullCard,
    kMainDiagonal
};
struct Point
{
    int x;
    int y;
};
struct Area
{
    int width;
    int height;
};
struct StringVector
{
    int lineCount;
    string *content;
};
struct StringMatrix
{
    int height;
    int width;
    string **content;
};
struct IntVector
{
    int lineCount;
    int *content;
};
struct IntMatrix
{
    int height;
    int width;
    int **content;
};
struct Menu
{
    bool enabled;
    string text;
    string query;
    int itemCount;
    struct Menu *items;
};
struct Player
{
    string name;
    StringMatrix card;
    IntMatrix scored;
    int score;
    int scoredCount;
    string timestamp;
};
struct Score
{
    string name;
    string gameType;
    string score;
    string timestamp;
};
struct ScoreVector
{
    Score *score;
    int itemCount;
};
struct BingoGame
{
    string title;
    Menu menu;
    GameType gameType;
    Player *players;
    IntVector numbers;
};
//utlidades.h
void gotoxy(Point point);
int getRandomIntInRange(int min, int max);
string getTimeStamp();
string colorANSI(string text, int format, int foreground, int background);
int getUnformattedANSINumber(string formattedANSINumber);
StringVector splitString(string delimiter, string text);
StringVector stringRectangle(Area area, int style);
void shuffleIntVector(IntVector &vector);
void shuffleStringVector(StringVector &vector);
IntVector getVectorFromRange(int min, int max);
void getCardFromVector(StringMatrix &card, IntVector vector, int numbersPerColumn, int skip);
StringVector readFile(string filename);
void writeFile(string filename, StringVector content);
ScoreVector readScoreData(string filename);
void writeScoreData(string filename, Score score);
//validators.h
bool isInRange(int number, int min, int max);
bool isDigits(string digits);
bool isFormatted(string str);
bool isFile(string filename);
bool isAlias(string alias, int min, int max);
//views.h
void cleanView(Point start, Point end);
void cleanFrameBody();
void cleanFrameInput();
void displayFrame(string title);
//watermarkviews.h
void displayWatermark();
//menuviews.h
void displayMenu(Menu menu);
string getQueryMenu(Menu menu);
//aliasviews.h
void displayAlias(string aliasA, string aliasB);
string getQueryAlias(string query);
//cardviews.h
void displayCard(Player player, Point point);
void displayCards(Player playerA, Player playerB);
void displayReport(Player player, Point point);
void callNumber(string text, IntVector vector, int index, int duration);
void callPlayer(string text, Player player, int duration);
//leaderboardviews.h
void displayStats(ScoreVector vector);
//bingoCore.h
BingoGame initBingo();
bool bingoCore(BingoGame &game);
void handleAlias(BingoGame &game);
void handleCards(BingoGame &game);
bool handleStats(BingoGame &game);
bool searchCard(StringMatrix &card, int number, GameType gametype);
bool checkPlayerVictory(Player &player, GameType gametype);
//main
int main(int argc, char const *argv[])
{
    srand(time(NULL));
    bool canRepeat = true;
    BingoGame game = initBingo();
    displayWatermark();
    while (canRepeat)
    {
        canRepeat = bingoCore(game);
    }
    system("cls");
    return 0;
}
//funciones utilities.cpp
int getRandomIntInRange(int min, int max)
{
    return rand() % (max - min + 1) + min;
}
int getUnformattedANSINumber(string formattedANSINumber)
{
    StringVector tokens = splitString("m", formattedANSINumber);
    return tokens.lineCount == 0 ? stoi(formattedANSINumber) : stoi(tokens.content[1]);
}
string getTimeStamp()
{
    time_t now = time(NULL);
    tm *timeStamp = localtime(&now);
    char time[32];
    strftime(time, 32, "%a, %d.%m.%Y %H:%M:%S", timeStamp);
    return string(time);
}
void gotoxy(Point point)
{
    HANDLE hCout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = point.x;
    coord.Y = point.y;
    SetConsoleCursorPosition(hCout, coord);
}
string colorANSI(string text, int format, int foreground, int background)
{

    return "\033[" +
           ((format >= 1 && format <= 3) ? to_string(format) : "0") + ';' +
           (foreground >= 0 && foreground <= 255 ? "38;5;" + to_string(foreground) : "39") + ';' +
           (background >= 0 && background <= 255 ? "48;5;" + to_string(background) : "49") + "m" +
           text +
           "\033[0m";
}
StringVector splitString(string delimiter, string text)
{
    string textCpy = text;
    size_t pos = 0;
    int i = 0;
    for (i = 0; (pos = textCpy.find(delimiter)) != string::npos; i++)
        textCpy.erase(0, pos + delimiter.length());
    StringVector content;
    content.lineCount = i;
    content.content = new string[i];
    for (i = 0; (pos = text.find(delimiter)) != string::npos; i++)
    {
        content.content[i] = text.substr(0, pos);
        text.erase(0, pos + delimiter.length());
    }
    return content;
}
StringVector stringRectangle(Area area, int style)
{
    StringVector canvas;
    canvas.lineCount = area.height;
    canvas.content = new string[area.height];
    style = (style != 2 && style != 1) ? 0 : (style - 1) * 6;
    char box[12] = {'\xDA', '\xBF', '\xC0', '\xD9', '\xB3', '\xC4', '\xC9', '\xBB', '\xC8', '\xBC', '\xBA', '\xCD'};
    string Line(area.width - 2, box[style + 5]); //a line
    string Fill(area.width - 2, ' ');
    canvas.content[0] = box[style] + Line + box[style + 1] + '\n';
    for (int a = 1; a < area.height - 1; a++)
        canvas.content[a] = box[style + 4] + Fill + box[style + 4] + '\n';
    canvas.content[area.height - 1] = box[style + 2] + Line + box[style + 3];
    return canvas;
}
//funciones fileUtilities.cpp
StringVector readFile(string filename)
{
    StringVector fc;
    string str;
    fstream file(filename);
    fc.lineCount = 0;
    if (file.is_open())
    {
        int i = 0;
        while (getline(file, str))
            i++;
        fc.lineCount = i;
        file.clear();
        file.seekg(0);
        fc.content = new string[i];
        i = 0;
        while (getline(file, str))
        {
            fc.content[i] = str;
            i++;
        }
        file.close();
    }
    return fc;
}
void writeFile(string filename, StringVector content)
{
    string str;
    fstream file;
    file.open(filename, ios::app);
    if (file.is_open())
    {
        for (int i = 0; i < content.lineCount; i++)
            file << content.content[i] << endl;
        file.close();
    }
}
ScoreVector readScoreData(string filename)
{
    ScoreVector scoreVector;
    StringVector scoreData = readFile(filename);
    scoreVector.itemCount = scoreData.lineCount;
    scoreVector.score = new Score[scoreData.lineCount];
    for (int i = 0; i < scoreData.lineCount; i++)
    {
        StringVector scoreToken = splitString("_", scoreData.content[i]);
        scoreVector.score[i].name = scoreToken.content[0];
        scoreVector.score[i].gameType = scoreToken.content[1];
        scoreVector.score[i].score = scoreToken.content[2];
        scoreVector.score[i].timestamp = scoreToken.content[3];
    }
    return scoreVector;
}
void writeScoreData(string filename, Score score)
{
    StringVector scoreData;
    scoreData.lineCount = 1;
    scoreData.content = new string[1];
    scoreData.content[0] = score.name + "_" + score.gameType + "_" + score.score + "_" + score.timestamp + "_";
    writeFile(filename, scoreData);
}
//vectorUtilities.cpp
void shuffleIntVector(IntVector &vector)
{
    if (vector.lineCount > 1)
        for (int i = 0; i < vector.lineCount - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (vector.lineCount - i) + 1);
            int t = vector.content[j];
            vector.content[j] = vector.content[i];
            vector.content[i] = t;
        }
}
void shuffleStringVector(StringVector &vector)
{
    if (vector.lineCount > 1)
        for (int i = 0; i < vector.lineCount - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (vector.lineCount - i) + 1);
            string t = vector.content[j];
            vector.content[j] = vector.content[i];
            vector.content[i] = t;
        }
}
IntVector getVectorFromRange(int min, int max)
{
    IntVector vector;
    int length = max - min;
    if (length > 1)
    {
        vector.lineCount = length + 1;
        vector.content = new int[length];
        for (int i = 0; i < length + 1; i++)
            vector.content[i] = min + i;
    }
    return vector;
};
void getCardFromVector(StringMatrix &card, IntVector vector, int numbersPerColumn, int skip)
{
    card.content = new string *[card.height];
    for (int i = 0; i < card.height && i < vector.lineCount; i++)
    {
        card.content[i] = new string[card.width];
        StringVector aux;
        aux.lineCount = card.width;
        aux.content = new string[card.width];
        for (int j = 0; j < card.width; j++)
            aux.content[j] = to_string((j < numbersPerColumn) ? vector.content[j + (i * numbersPerColumn) + skip] : 0);
        shuffleStringVector(aux);
        for (int j = 0; j < card.width; j++)
            card.content[i][j] = aux.content[j];
    }
}
//validators.cpp
bool isInRange(int number, int min, int max)
{
    return number >= min && number <= max;
}
bool isDigits(string str)
{
    return str.find_first_not_of("0123456789") == string::npos;
}
bool isFormatted(string str)
{
    return str.at(0) == '\033';
}
bool isFile(string filename)
{
    bool valid = false;
    fstream file(filename);
    if ((valid = file.is_open()))
        file.close();
    return valid;
}
bool isAlias(string alias, int min, int max)
{
    return isalpha(alias.at(0)) && isInRange(alias.length(), min, max);
}
//views.cpp
void cleanView(Point start, Point end)
{
    string cleanLine(end.x - start.x, ' ');
    for (int i = 0; i < end.y - start.y; i++)
    {
        gotoxy({start.x, start.y + i});
        cout << cleanLine;
    }
}
void cleanFrameBody()
{
    cleanView({3, 6}, {144, 30});
}
void cleanFrameInput()
{
    cleanView({3, 32}, {144, 35});
}
void displayFrame(string title)
{
    system("cls");
    Area fullScreen = {147, 37};
    StringVector frame = stringRectangle(fullScreen, 1);
    StringVector header = stringRectangle({fullScreen.width - 4, 4}, 2);
    StringVector body = stringRectangle({fullScreen.width - 4, fullScreen.height - 11}, 2);
    StringVector footer = stringRectangle({fullScreen.width - 4, 5}, 1);
    for (int i = 0; i < fullScreen.height; i++)
    {
        gotoxy({0, i});
        cout << frame.content[i];
    }
    for (int i = 0; i < fullScreen.height - 11; i++)
    {
        if (i < 4)
        {
            gotoxy({2, 1 + i});
            cout << colorANSI(header.content[i], 1, 192, 9);
        }
        gotoxy({2, 5 + i});
        cout << body.content[i];
        if (i < 5)
        {
            gotoxy({2, fullScreen.height - 6 + i});
            cout << footer.content[i];
        }
    }
    string titleFrame(fullScreen.width - 7, ' ');
    for (long long unsigned int i = 0; i < title.length(); i++)
        titleFrame.at(i + 3) = title.at(i);
    gotoxy({3, 3});
    cout << colorANSI(titleFrame, 1, 192, 9);
}
//watermarkviews.cpp
void displayWatermark()
{
    system("cls");
    StringVector content = readFile("data/logo.txt");
    for (int i = 0; i < content.lineCount; i++)
    {
        gotoxy({18, i + 8});
        cout << colorANSI(content.content[i], 1, i < 20 ? 32 + i : 256, 256);
    }
    gotoxy({60, 30});
    system("pause");
    system("cls");
}
//menuviews.cpp
void displayMenu(Menu menu)
{
    cleanFrameBody();
    gotoxy({6, 7});
    cout << menu.text;
    for (int i = 0; i < menu.itemCount; i++)
    {
        gotoxy({6, 9 + i});
        cout << i + 1 << ") "
             << left << setw(32)
             << menu.items[i].text
             << colorANSI(
                    ((menu.items[i].enabled) ? "[Hablitado]" : "[Deshablitado] "),
                    0,
                    ((menu.items[i].enabled) ? 10 : 9),
                    256)
             << endl;
    }
}
string getQueryMenu(Menu menu)
{
    string selectedOption;
    bool validOption = false;
    while (!validOption)
    {
        cleanFrameInput();
        gotoxy({6, 33});
        cout << menu.query;
        getline(cin, selectedOption);
        validOption = !selectedOption.empty() &&
                      isDigits(selectedOption) &&
                      isInRange(stoi(selectedOption), 1, menu.itemCount) &&
                      menu.items[stoi(selectedOption) - 1].enabled;
        if (!validOption)
        {
            gotoxy({6, 33});
            cout << colorANSI("Esta opcion puede estar deshabilitada o ser invalida, intentalo de nuevo...", 1, 9, 256);
            Sleep(3000);
        }
    }
    return selectedOption;
}
//aliasviews.cpp
void displayAlias(string aliasA, string aliasB)
{
    cleanFrameBody();
    StringVector playerFile = readFile("data/player.txt");
    gotoxy({30, 26});
    cout << colorANSI(aliasA.empty() ? "Jugador A" : aliasA + " (Jugador A)", 1, 196, 256);
    gotoxy({10 + 75, 26});
    cout << colorANSI(aliasB.empty() ? "Jugador B" : aliasB + " (Jugador B)", 1, 33, 256);
    for (int i = 0; i < playerFile.lineCount; i++)
    {
        gotoxy({30, 9 + i});
        cout << colorANSI(playerFile.content[i], 1, 196, 256) << endl;
        gotoxy({10 + 75, 9 + i});
        cout << colorANSI(playerFile.content[i], 1, 33, 256) << endl;
    }
}
string getQueryAlias(string query)
{
    string alias;
    bool validAlias = false;
    while (!validAlias)
    {
        cleanFrameInput();
        gotoxy({6, 33});
        cout << query;
        getline(cin, alias);
        validAlias = !alias.empty() &&     //no puede estar vacia
                     isAlias(alias, 4, 8); //necesita ser alias
        if (!validAlias)
        {
            gotoxy({6, 33});
            cout << colorANSI("Alias invalido, intentalo de nuevo...", 1, 9, 256);
            Sleep(3000);
        }
    }
    return alias;
}
//cardviews.cpp
void displayCard(Player player, Point point)
{
    for (int i = 0; i < player.card.height; i++)
        for (int j = 0; j < player.card.width; j++)
        {
            gotoxy({point.x + (4 * j), point.y + (2 * i)});
            cout << setw(4) << player.card.content[i][j];
        }
}
void callNumber(string text, IntVector vector, int index, int duration)
{
    cleanFrameInput();
    StringVector bankFrame = stringRectangle({30, 24}, 1);
    for (int i = 0; i < bankFrame.lineCount; i++)
    {
        gotoxy({112, 6 + i});
        cout << bankFrame.content[i];
    }
    gotoxy({112 + 3, 6 + 2});
    cout << colorANSI("Ya salieron: ", 1, 11, 256);
    int col = 0;
    int row = 0;
    for (int i = 0; i < index + 1; i++)
    {
        if (col == 6)
        {
            col = 0;
            row++;
        }
        gotoxy({112 + 3 + (4 * col), 7 + 3 + row});
        cout << setw(4) << vector.content[i];
        col++;
    }
    gotoxy({6, 33});
    cout << colorANSI(string(text + to_string(vector.content[index])), 1, 33, 256);
    Sleep(duration);
}
void displayCards(Player playerA, Player playerB)
{
    gotoxy({9, 8});
    cout << colorANSI(playerA.name.empty() ? "Jugador A" : playerA.name + " (Jugador A)", 1, 196, 256);
    gotoxy({9 + 50, 8});
    cout << colorANSI(playerB.name.empty() ? "Jugador B" : playerB.name + " (Jugador B)", 1, 33, 256);
    displayCard(playerA, {9, 10});
    displayCard(playerB, {50 + 9, 10});
}
void displayReport(Player player, Point point)
{
    int k, l;
    k = l = 0;
    for (int i = 0; i < player.scored.height; i++)
        for (int j = 0; j < player.scored.width; j++)
            if (player.scored.content[i][j] != 0)
            {
                if (k == 7)
                {
                    k = 0;
                    l++;
                }
                gotoxy({point.x + (4 * (k)) + 20, point.y + 3 + l});
                cout << left << setw(4) << player.scored.content[i][j];
                k++;
            }
    gotoxy({point.x, point.y});
    cout << left << setw(20)
         << "Tiempo: " << player.timestamp;
    gotoxy({point.x, point.y + 2});
    cout << left << setw(20)
         << "Aciertos: " << player.scoredCount;
    gotoxy({point.x, point.y + 3});
    cout << left << setw(20)
         << "Lista de Numeros: ";
}
void callPlayer(string text, Player player, int duration)
{
    StringVector tokens = splitString("#", text);
    cleanFrameInput();
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
//leaderboardviews.cpp
void displayStats(ScoreVector vector)
{
    cleanFrameBody();
    StringVector nameRectangle = stringRectangle({36, 3}, 1);
    StringVector gameTypeRectangle = stringRectangle({29, 3}, 1);
    StringVector scoreRectangle = stringRectangle({19, 3}, 1);
    StringVector dateRectangle = stringRectangle({55, 3}, 1);
    for (int i = 0; i < nameRectangle.lineCount; i++)
    {
        gotoxy({4, 6 + i});
        cout << nameRectangle.content[i];
        gotoxy({40, 6 + i});
        cout << gameTypeRectangle.content[i];
        gotoxy({69, 6 + i});
        cout << scoreRectangle.content[i];
        gotoxy({88, 6 + i});
        cout << dateRectangle.content[i];
    }
    gotoxy({6, 7});
    cout << "Alias";
    gotoxy({42, 7});
    cout << "Modo";
    gotoxy({71, 7});
    cout << "Puntos";
    gotoxy({90, 7});
    cout << "Fecha";
    int pages = (int)ceil((double)(((double)vector.itemCount) / ((double)PAGE_LIMIT)));
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
        if (i != pages - 1)
        {
            cleanFrameInput();
            gotoxy({6, 33});
            cout << "Imprimiendo siguiente pagina...";
            system("pause");
        }
    }
    cleanFrameInput();
    gotoxy({6, 33});
    system("pause");
}

//bingoCore.cpp file
BingoGame initBingo()
{
    BingoGame game;
    game.title = "Easy Bingo Inc. 2020";
    game.players = new Player[2];
    StringVector menuFile = readFile("data/menu.txt");
    StringVector subMenuFile = readFile("data/submenu.txt");
    game.menu.items = new Menu[menuFile.lineCount];
    game.menu.itemCount = menuFile.lineCount;
    for (int i = 0; i < menuFile.lineCount; i++)
    {
        game.menu.items[i].text = menuFile.content[i];
        game.menu.items[i].enabled = i == 0 || i == 5;
        if (i == 4)
        {
            game.menu.items[i].items = new Menu[subMenuFile.lineCount];
            game.menu.items[i].itemCount = subMenuFile.lineCount;
            for (int j = 0; j < subMenuFile.lineCount; j++)
            {
                game.menu.items[i].items[j].text = subMenuFile.content[j];
                game.menu.items[i].items[j].enabled = true;
            }
            game.menu.items[i].query = "Cual opcion desea?: ";
        }
    }
    game.menu.text = "Menu Principal";
    game.menu.query = "Cual opcion desea?: ";
    return game;
}
bool bingoCore(BingoGame &game)
{
    bool canRepeat = true;
    bool isPlaying = false;
    displayFrame(game.title);
    displayMenu(game.menu);
    int selectedMain = stoi(getQueryMenu(game.menu));
    switch (selectedMain)
    {
    case 1:
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
        bool canSubRepeat = true;
        while (canSubRepeat)
            canSubRepeat = handleStats(game);
        break;
    }
    case 6:
    {
        canRepeat = false;
        break;
    }
    }

    if (isPlaying)
    {
        int pos;
        handleCards(game);
        shuffleIntVector(game.numbers);
        bool found = false;
        bool ended = false;
        for (int i = 0; i < 100; i++)
        {
            cleanFrameBody();
            displayCards(game.players[0], game.players[1]);
            callNumber("Salio el numero ", game.numbers, i, REFRESH_RATE);
            displayCards(game.players[0], game.players[1]);
            for (int j = 0; j < 2; j++)
                if ((found = searchCard(game.players[j].card, game.numbers.content[i], game.gameType)))
                {
                    game.players[j].scoredCount++;
                    if ((ended = checkPlayerVictory(game.players[j], game.gameType)))
                    {
                        pos = j;
                        break;
                    }
                }
            if (ended)
            {
                cleanFrameBody();
                callNumber("Salio el numero ", game.numbers, i, 50);
                displayCards(game.players[0], game.players[1]);
                for (int j = 0; j < 2; j++)
                    displayReport(game.players[j], {9 + (j == 0 ? 0 : 50), 22});
                callPlayer("Felicidades # !!!! Acumulaste: # puntos. #", game.players[pos], 4000);
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
    return canRepeat;
}
//handlers.cpp
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
    game.numbers = getVectorFromRange(1, 100);
    shuffleIntVector(game.numbers);
    int randomDimension = getRandomIntInRange(3, 6);
    int numbersPerColumn = game.gameType == 3 ? randomDimension : game.gameType == 2 ? 9 : 5;
    int skip = game.gameType == 3 ? randomDimension * randomDimension : game.gameType == 2 ? 27 : 15;
    for (int i = 0; i < 2; i++)
    {
        game.players[i].card.height = game.gameType == 3 ? randomDimension : 3;
        game.players[i].card.width = game.gameType == 3 ? randomDimension : 9;
        int width = game.players[i].scored.width = numbersPerColumn;
        int height = game.players[i].scored.height =
            game.gameType != 1 ? 3 : 1;
        game.players[i].scored.content = new int *[height];
        for (int j = 0; j < height; j++)
        {
            game.players[i].scored.content[j] = new int[width];
            for (int k = 0; k < width; k++)
                game.players[i].scored.content[j][k] = 0;
        }
        game.players[i].scoredCount = 0;
        game.players[i].score = 0;
        getCardFromVector(game.players[i].card, game.numbers, numbersPerColumn, i == 0 ? i : skip);
    }
}
bool handleStats(BingoGame &game)
{
    bool canSubRepeat = true;
    bool isgameType = false;
    GameType gameType;
    ScoreVector scoreVector = readScoreData("data/leaderboard.txt");
    displayMenu(game.menu.items[4]);
    int selectedStats = stoi(
        getQueryMenu(
            game.menu.items[4]));

    switch (selectedStats)
    {
    case 1:
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
        string aliasQuery = getQueryAlias("Nombre del jugador a buscar: ");
        ScoreVector filterPlayer;
        filterPlayer.score = new Score[scoreVector.itemCount];
        filterPlayer.itemCount = 0;
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
        isgameType = true;
        gameType = kHorizontal;
        break;
    case 4:
        isgameType = true;
        gameType = kFullCard;
        break;
    case 5:
        isgameType = true;
        gameType = kMainDiagonal;
        break;
    case 6:
        canSubRepeat = false;
        break;
    }
    if (isgameType)
    {
        ScoreVector filterGameType;
        string gameTypeQuery = (gameType == 3 ? "Diagonal" : gameType == 2 ? "Carta Completa" : "Horizontal");
        filterGameType.score = new Score[scoreVector.itemCount];
        filterGameType.itemCount = 0;
        for (int i = 0; i < scoreVector.itemCount; i++)
            if (gameTypeQuery.compare(scoreVector.score[i].gameType) == 0)
            {
                filterGameType.score[filterGameType.itemCount] = scoreVector.score[i];
                filterGameType.itemCount++;
            }
        if (scoreVector.itemCount == 0 || filterGameType.itemCount == 0)
        {
            cleanFrameInput();
            gotoxy({6, 33});
            cout << "Nadie a ganado en modo " << gameTypeQuery << "...";
            system("pause");
            return true;
        }
        displayStats(filterGameType);
    }
    return canSubRepeat;
}
//card.cpp
bool searchCard(StringMatrix &card, int number, GameType gametype)
{
    bool found = false;
    int ri = card.height - 1;
    for (int i = 0; i < card.height; i++)
    {
        for (int j = 0; j < card.width; j++)
        {
            int cardNumber = getUnformattedANSINumber(card.content[i][j]);
            if ((found = gametype != 3 ? cardNumber == number : cardNumber == number && (j == i || j == ri)))
            {
                card.content[i][j] = colorANSI(card.content[i][j], 1, 10, 256);
                break;
            }
        }
        ri--;
        if (found)
            break;
    }
    return found;
}
bool checkPlayerVictory(Player &player, GameType gametype)
{
    player.score = 0;
    player.timestamp = getTimeStamp();
    bool ended = false;
    int scoredCount = 0;
    int rscoredCount = 0;
    int score = 0;
    int rscore = 0;
    int scoredrows = 0;
    int center = 0;
    int ri = player.card.height - 1;
    for (int i = 0; i < player.card.height; i++)
    {
        scoredCount = (gametype != 3) ? 0 : scoredCount;
        player.score = (gametype == 1) ? 0 : player.score;
        for (int j = 0; j < player.card.width; j++)
        {
            if ((isFormatted(player.card.content[i][j])))
            {
                int number = getUnformattedANSINumber(player.card.content[i][j]);
                if (gametype != 3)
                {
                    scoredCount++;
                    player.score += number;
                    player.scored.content[gametype == 2 ? i : 0][scoredCount] = number;
                    if (scoredCount == player.scored.width)
                        scoredrows++;
                    if ((ended = (scoredrows == player.scored.height)))
                        break;
                }
                else if (gametype == 3)
                {
                    if (i == j && ri != i)
                    {
                        player.scored.content[0][scoredCount] = number;
                        score += number;
                        scoredCount++;
                    }
                    else if (ri == j && ri != i)
                    {
                        player.scored.content[1][rscoredCount] = number;
                        rscore += number;
                        rscoredCount++;
                    }
                    else if (ri == i && center == 0)
                    {
                        player.scored.content[2][0] = number;
                        center = number;
                    }
                    if ((ended = (center == 0 && (scoredCount == player.scored.width ||
                                                  rscoredCount == player.scored.width)) ||
                                 (center != 0 && (scoredCount == player.scored.width - 1 ||
                                                  rscoredCount == player.scored.width - 1))))
                    {
                        if ((scoredCount == player.scored.width ||
                             scoredCount == player.scored.width - 1))
                            player.score = score + center;
                        else
                            player.score = rscore + center;
                        break;
                    }
                }
            }
        }
        ri--;
        if (ended)
            break;
    }
    return ended;
}
