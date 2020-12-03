#ifndef CARD_VIEWS_H
#define CARD_VIEWS_H

#include "../alias/aliasviews.h"

void displayCard(Player player, Point point);
void callNumber(string text, int number, int duration);
void displayCards(Player playerA, Player playerB);
void callPlayer(string text, Player player, int duration);
void displayReport(Player player, Point point);

#endif