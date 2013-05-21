#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


//#define DEBUG

#define g_circle_radius 45
#define g_field_size 3

void ComputerMove(int gField[g_field_size][g_field_size], int *computerAnswer, int i, int j);
void ChekingPlayerExactlyWin(int gField[g_field_size][g_field_size], gboolean *gplayerWin, int *playerAnswer, int *computerAnswer);
void ChekingPlayerCanWin(int gField[g_field_size][g_field_size], gboolean *gfoundDanger, int *playerAnswer, int *computerAnswer);
void ChekingComputerWin(int gField[g_field_size][g_field_size], gboolean *gcomputerWin, int *playerAnswer, int *computerAnswer);