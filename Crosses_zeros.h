#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <time.h>

//#define DEBUG

#define g_circle_radius 45
#define g_field_size 3

static void ClearField();
static void DoDrawing(cairo_t *cr);
static void DrawField(cairo_t *cr);
static void ComputerWin(int i, int j);
	
static void ChouseFirstPlayerWindow();
static void ResultWindow(char* string);
static gboolean DeadHeat();
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean CellDenermination(int x, int y);
static gboolean DeadHeat();
static void ComputerStupidMove();
static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
static void btnPlayer_clicked(GtkWidget *widget, gpointer data);
static void btnComputer_clicked(GtkWidget *widget, gpointer data);
static void close_window(GtkWidget *widget, gpointer window);
int main(int argc, char *argv[]);