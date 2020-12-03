#ifndef CARD_VIEWS_H
#define CARD_VIEWS_H

#include "../alias/aliasviews.h"

void displayCard(Player player, Point point);
void displayCards(Player playerA, Player playerB);
void displayReport(Player player, Point point);
void callNumber(string text, IntVector vector, int index, int duration);
void callPlayer(string text, Player player, int duration);

#endif