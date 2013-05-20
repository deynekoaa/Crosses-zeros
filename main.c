#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

//#define DEBUG

//define
#define g_circle_radius 45
#define g_field_size 3


//global contains
GtkWidget *window;
GtkWidget *darea;
// the main field
int gField[g_field_size][g_field_size];
int computerAnswer = 2;
int playerAnswer = 1;
gboolean gcomputerWin = FALSE;
gboolean gfoundDanger = FALSE;
gboolean gplayerWin = FALSE;
gboolean gdeadHeat = FALSE;


static void ClearField();
static void DoDrawing(cairo_t *);
static void DrawField(cairo_t *cr);
static void ComputerWin(int i, int j);
static void ComputerMove(int i, int j);	
static void ResultWindow(char* string);
static gboolean DeadHeat();


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


//executing computer motion
static void ChekingComputerWin()
{
	gcomputerWin = FALSE;
	int i;
	for (i = 0; i < g_field_size; i++)
	{
		if (gcomputerWin) break;
		int j;
		//checking veltical win version 
		int free = -1;
		int filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (gcomputerWin) break;
			
			if (gField[i][j] == computerAnswer)
				filling++;
			if (gField[i][j] == 0)
				free = j;			
			if ((filling == g_field_size -1 )&&(free >= 0))
			{
				//computer win!
				gcomputerWin = TRUE;
				ComputerMove(i,free);
			}
		}

		//checking horizontal win version
		free = -1;
		filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (gcomputerWin) break;
			if (gField[j][i] == computerAnswer)
				filling++;
			if (gField[j][i] == 0)
				free = j;			
			if ((filling == g_field_size - 1)&&(free >= 0))
			{
				//computer win!
				gcomputerWin = TRUE;
				ComputerMove(free,i);
			}
		}

			
		//cheking only once 
		if (i == 0)
		{
			//checking diagonal win version
			free = -1;
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (gcomputerWin) break;
				if (gField[j][j] == computerAnswer)
					filling++;
				if (gField[j][j] == 0)
					free = j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//computer win!
					gcomputerWin = TRUE;
					ComputerMove(free,free);
				}
			}

			//checking reverse diagonal win version
			free = -1;
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (gcomputerWin) break;
				if (gField[g_field_size - j][j] == computerAnswer)
					filling++;
				if (gField[g_field_size - j][j] == 0)
					free = g_field_size - j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//computer win!
					gcomputerWin = TRUE;
					ComputerMove(free,j);
				}
			}
		}
	}
}

//executing computer motion
static void ChekingPlayerCanWin()
{
	gfoundDanger = FALSE;
	int i;
	for (i = 0; i < g_field_size; i++)
	{
		if (gfoundDanger) break;
		int j;
		//checking veltical win version 
		int free = -1;
		int filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (gfoundDanger) break;
			
			if (gField[i][j] == playerAnswer)
				filling++;
			if (gField[i][j] == 0)
				free = j;			
			if ((filling == g_field_size -1 )&&(free >= 0))
			{
				//found dangerous situation
				gfoundDanger = TRUE;
				ComputerMove(i,free);
			}
		}

		//checking horizontal win version
		free = -1;
		filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (gfoundDanger) break;
			if (gField[j][i] == playerAnswer)
				filling++;
			if (gField[j][i] == 0)
				free = j;			
			if ((filling == g_field_size - 1)&&(free >= 0))
			{
				
				//found dangerous situation
				gfoundDanger = TRUE;
				ComputerMove(free,i);
			}
		}

		
		//cheking only once 
		if (i == 0)
		{
			//checking diagonal win version
			free = -1;
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (gfoundDanger) break;
				if (gField[j][j] == playerAnswer)
					filling++;
				if (gField[j][j] == 0)
					free = j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//found dangerous situation
					gfoundDanger = TRUE;
					ComputerMove(free,free);
				}
			}

			//checking reverse diagonal win version
			free = -1;
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (gfoundDanger) break;
				if (gField[g_field_size - j-1][j] == playerAnswer)
					filling++;
				if (gField[g_field_size - j-1][j] == 0)
					free = j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//found dangerous situation
					gfoundDanger = TRUE;
					ComputerMove(g_field_size - free - 1,free);
				}
			}
		}
	}
}

static void ChekingPlayerExactlyWin()
{
	//player exactly win
	gplayerWin = FALSE;
	int i;
	for (i = 0; i < g_field_size; i++)
	{
		if (gplayerWin) break;
		int j;
		//checking veltical version 
		int filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (gplayerWin) break;
			if (gField[i][j] == playerAnswer)
				filling++;
			if (filling == g_field_size)
			{
				gplayerWin = TRUE;
			}
		}

		//checking horizontal version
		filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (gplayerWin) break;
			if (gField[j][i] == playerAnswer)
				filling++;
			if (filling == g_field_size)
			{
				
				gplayerWin = TRUE;
			}
		}

		
		//cheking only once 
		if (i == 0)
		{
			//checking diagonal version
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (gplayerWin) break;
				if (gField[j][j] == playerAnswer)
					filling++;
				if (filling == g_field_size)
				{
					gplayerWin = TRUE;
				}
			}

			//checking reverse diagonal version
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (gplayerWin) break;
				if (gField[g_field_size - j][j] == playerAnswer)
					filling++;
				if (filling == g_field_size)
				{
					gplayerWin = TRUE;
				}
			}
		}
	}
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




static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (event->button == 1) 
	{
		#ifdef DEBUG
			ResultWindow();
		#endif
		int x = event->x;
		int y = event->y;
		if(!CellDenermination(x,y))
		{
			return;
		}
		gplayerWin = FALSE;
		ChekingPlayerExactlyWin();
		if (gplayerWin)
		{
			//player exatly win, we must say about it
			gtk_widget_queue_draw(darea);
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
				char* string = "Dead heat!";
				ResultWindow(string);
			}
			else
			{
				gcomputerWin = FALSE;
				ChekingComputerWin();
				if (gcomputerWin)
				{
					//computer do step and win, we must draw and say about it
					gtk_widget_queue_draw(darea);
					char* string = "Sorry, you lost!";
					ResultWindow(string);
				}
				else 
				{
					gfoundDanger = FALSE;
					ChekingPlayerCanWin();
					if (gfoundDanger)
					{
						//computer block player win step, and continue game
						gtk_widget_queue_draw(darea);
					}
					else
					{
						if (gField[1][1] == 0)
						{
							gField[1][1] = computerAnswer;
						}
						else
						{
							int m,k;
							do 
							{
								srand(time(NULL));
								k = (rand()%3);
								m = (rand()%3);
							}
							while (gField[k][m] != 0);
							gField[k][m] = computerAnswer;
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

void button_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}

void close_window(GtkWidget *widget, gpointer window)
{
    gtk_widget_destroy(GTK_WIDGET(widget));
}

static void ResultWindow(char* string)
{
	GtkWidget *window;
  	GtkWidget *fixed;
  	GtkWidget *button;
  	GtkWidget *info;
  	
  	#ifdef DEBUG
  		string = "Congratulation, you win!";
  	#endif



  	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_title(GTK_WINDOW(window), "Information");
  	gtk_widget_set_size_request(window, 300, 100);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  	fixed = gtk_fixed_new();
  	gtk_container_add(GTK_CONTAINER(window), fixed);

  	info = gtk_label_new(string);
  	gtk_widget_set_size_request(info, 240, 40);
  	gtk_fixed_put(GTK_FIXED(fixed), info, 30, 15);


  	button = gtk_button_new_with_label("OK");
  	gtk_fixed_put(GTK_FIXED(fixed), button, 120, 60);
  	gtk_widget_set_size_request(button, 60, 35);

  	g_signal_connect(G_OBJECT(button), "clicked", 
       G_CALLBACK(button_clicked), window);
  	
  	g_signal_connect(G_OBJECT(window), "destroy", 
    	G_CALLBACK(close_window), NULL);

  	gtk_widget_show_all(window);
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

	ClearField();

	gtk_main();

	return 0;
}
