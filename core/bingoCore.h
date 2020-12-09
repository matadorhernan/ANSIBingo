#ifndef BINGO_CORE_H
#define BINGO_CORE_H

#include "../views/leaderboard/leaderboardviews.h"

//forwards
BingoGame initBingo();
bool bingoCore(BingoGame &game);

void handleAlias(BingoGame &game);
void handleCards(BingoGame &game);
bool handleStats(BingoGame &game);

bool searchCard(StringMatrix &card, int number, GameType gametype);
bool checkPlayerVictory(Player &player, GameType gametype);

#endif