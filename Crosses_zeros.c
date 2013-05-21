#include "Crosses_zeros.h"


// the main field
int gField[g_field_size][g_field_size];
//global contains
gboolean IsGameContinue = TRUE;
GtkWidget *window;
GtkWidget *darea;
int computerAnswer = 2;
int playerAnswer = 1;
gboolean gcomputerWin = FALSE;
gboolean gfoundDanger = FALSE;
gboolean gplayerWin = FALSE;
gboolean gdeadHeat = FALSE;


static void ClearField()
{
	int i,j;
	for(i = 0; i < g_field_size; i++)
	{
		for(j = 0; j < g_field_size; j++)
		{
			gField[i][j] = 0;
		}
	}
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	cr = gdk_cairo_create(gtk_widget_get_window(widget));
	DrawField(cr);
	DoDrawing(cr);
	cairo_destroy(cr);
	return FALSE;
}

static void DoDrawing(cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 0.5);

	int i, j;
	for (i = 0; i < g_field_size ; i++ ) 
	{
		for (j = 0; j < g_field_size ; j++ ) 
		{	
			cairo_set_source_rgb(cr, 0, 0, 0);
			if (gField[i][j] == playerAnswer)
			{
				cairo_new_path(cr);
				cairo_set_line_width(cr, 6);
				cairo_set_source_rgb(cr, 0.69, 0.19, 0);
				cairo_move_to(cr, i*100+50+g_circle_radius, j*100+50);
				cairo_arc(cr, i*100+50, j*100+50, g_circle_radius, 0, 2 * M_PI);
				cairo_stroke_preserve(cr);
				cairo_set_source_rgb(cr, 0, 0, 0);
				//cairo_close_path(cr);
			}
			else
			if (gField[i][j] == computerAnswer)
			{
				cairo_new_path(cr);
				cairo_set_line_width(cr, 6);
				cairo_set_source_rgb(cr, 0.29, 0.60, 0);
				cairo_move_to(cr, (i+1)*100-5, (j+1)*100-5);
				cairo_line_to(cr, i*100+5, j*100+5);
				cairo_move_to(cr, (i+1)*100-5, j*100+5);
				cairo_line_to(cr, i*100+5, (j+1)*100-5);
				cairo_stroke_preserve(cr);
				cairo_set_source_rgb(cr, 0, 0, 0);
				//cairo_close_path(cr);
			}
		}
	}   
}

static void DrawField(cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 4);
	
	int i;
	for (i = 1; i < g_field_size; ++i)
	{
		cairo_move_to (cr, i*100, 0);
		cairo_line_to (cr, i*100, 300);
		cairo_move_to (cr, 0, i*100);
		cairo_line_to (cr, 300, i*100);
	}
	cairo_stroke(cr);
}

//determinate where mouse coordinate
static gboolean CellDenermination(int x, int y)
{
	x /= 100;
	y /= 100;
	if (gField[x][y] == 0)
	{
		gField[x][y] = playerAnswer;
		return TRUE;
	}
	return FALSE;
}




static gboolean DeadHeat()
{
	int i,j;
	int filled = 0;
	for (i = 0; i < g_field_size; i++)
	{
		for (j = 0; j < g_field_size; j++)
		{
			if (gField[i][j] != 0)
				filled++;
		}
	}
	if (filled == (g_field_size*g_field_size))
		gdeadHeat = TRUE;
	else
		gdeadHeat = FALSE;

}

static void ComputerStupidMove()
{
	int m,k;
	do 
	{
		k = (rand()%g_field_size);
		m = (rand()%g_field_size);
	}
	while (gField[k][m] != 0);
	gField[k][m] = computerAnswer;
}



static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (event->button == 1) 
	{
		#ifdef DEBUG
		char* str = "2";
			ResultWindow(str);
		#endif
		int x = event->x;
		int y = event->y;
		if(!CellDenermination(x,y) || !IsGameContinue)
		{
			return;
		}
		gplayerWin = FALSE;
		ChekingPlayerExactlyWin(&gField, &gplayerWin, &g_field_size, &playerAnswer, &computerAnswer);
		if (gplayerWin)
		{
			//player exatly win, we must say about it
			gtk_widget_queue_draw(darea);
			IsGameContinue = FALSE;
			char* string = "Congratulation, you win!";
			ResultWindow(string);
			
		}
		else
		{
			gdeadHeat = FALSE;
			DeadHeat();
			if (gdeadHeat)
			{
				gtk_widget_queue_draw(darea);
				IsGameContinue = FALSE;
				char* string = "Dead heat!";
				ResultWindow(string);
			}
			else
			{
				gcomputerWin = FALSE;
				ChekingComputerWin(&gField, &gcomputerWin, &g_field_size, &playerAnswer, &computerAnswer);
				if (gcomputerWin)
				{
					//computer do step and win, we must draw and say about it
					gtk_widget_queue_draw(darea);
					IsGameContinue = FALSE;
					char* string = "Sorry, you lost!";
					ResultWindow(string);
				}
				else 
				{
					gfoundDanger = FALSE;
					ChekingPlayerCanWin(&gField, &gfoundDanger, &g_field_size, &playerAnswer, &computerAnswer);
					if (gfoundDanger)
					{
						//computer block player win step, and continue game
						gtk_widget_queue_draw(darea);
					}
					else
					{
						if (gField[g_field_size/2][g_field_size/2] == 0)
						{
							gField[g_field_size/2][g_field_size/2] = computerAnswer;
						}
						else
						{
							ComputerStupidMove();
						}
						gtk_widget_queue_draw(darea);		
					}	
				}
			}		
		}
	}

	return TRUE;
}

	
static void ComputerMove(int i, int j)
{
	gField[i][j] = computerAnswer;
}

static void btnPlayer_clicked(GtkWidget *widget, gpointer data)
{
	IsGameContinue = TRUE;
	gtk_widget_queue_draw(darea);
	gtk_widget_destroy(GTK_WIDGET(data));
}

static void btnComputer_clicked(GtkWidget *widget, gpointer data)
{
	IsGameContinue = TRUE;
	ComputerStupidMove();
	gtk_widget_queue_draw(darea);
	gtk_widget_destroy(GTK_WIDGET(data));

}

static void close_window(GtkWidget *widget, gpointer window)
{
    gtk_widget_destroy(GTK_WIDGET(widget));
}

static void ChouseFirstPlayerWindow()
{
	GtkWidget *window2;
  	GtkWidget *fixed;
  	GtkWidget *btnPlayer;
  	GtkWidget *btnComputer;
  	GtkWidget *info;

  	char* string = "Who will go first?";
  	IsGameContinue = FALSE;

  	window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_title(GTK_WINDOW(window2), "Choice of the first player");
  	gtk_widget_set_size_request(window2, 300, 100);
	gtk_window_set_resizable(GTK_WINDOW(window2), FALSE);
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);

  	fixed = gtk_fixed_new();
  	gtk_container_add(GTK_CONTAINER(window2), fixed);

  	info = gtk_label_new(string);
  	gtk_fixed_put(GTK_FIXED(fixed), info, 30, 15);
  	gtk_widget_set_size_request(info, 240, 40);


  	btnPlayer = gtk_button_new_with_label("Player");
  	gtk_fixed_put(GTK_FIXED(fixed), btnPlayer, 45, 60);
  	gtk_widget_set_size_request(btnPlayer, 80, 35);

  	btnComputer = gtk_button_new_with_label("Computer");
  	gtk_fixed_put(GTK_FIXED(fixed), btnComputer, 165, 60);
  	gtk_widget_set_size_request(btnComputer, 80, 35);

  	g_signal_connect(G_OBJECT(btnPlayer), "clicked", 
       G_CALLBACK(btnPlayer_clicked), window2);

  	g_signal_connect(G_OBJECT(btnComputer), "clicked", 
       G_CALLBACK(btnComputer_clicked), window2);
  	
  	g_signal_connect(G_OBJECT(window), "destroy", 
    	G_CALLBACK(gtk_main_quit), NULL);

  	gtk_widget_show_all(window2);
}

static void ResultWindow(char* string)
{
	#ifdef DEBUG
  		string = "Congratulation, you win!";
  	#endif

	GtkWidget *resultWindow;
	resultWindow = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO,GTK_BUTTONS_OK, string,NULL);
  	g_signal_connect_swapped (resultWindow, "response", G_CALLBACK (gtk_main_quit), NULL);
	gtk_dialog_run (GTK_DIALOG (resultWindow));
}




int main(int argc, char *argv[])
{
	
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), darea);
 
	gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

	// draw event
	g_signal_connect(G_OBJECT(darea), "expose-event", 
			G_CALLBACK(on_draw_event), NULL); 
	//destroy event = quit
	g_signal_connect(window, "destroy",
			G_CALLBACK(gtk_main_quit), NULL);  
	//clicked
	g_signal_connect(window, "button-press-event", 
			G_CALLBACK(clicked), NULL);
 
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	//gtk_window_set_resizable(GTK_WINDOW(window), FALSE); 
	//gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
	gtk_widget_set_size_request(window, 300, 300);
	gtk_window_set_title(GTK_WINDOW(window), "Crosses and zeros");
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	gtk_widget_show_all(window);

	srand(time(NULL));
	ChouseFirstPlayerWindow();
	ClearField();

	gtk_main();

	return 0;
}
