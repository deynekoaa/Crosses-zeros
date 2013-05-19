#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

//define
#define g_circle_radius 45
#define g_field_size 3


//global contains
GtkWidget *window;
GtkWidget *darea;
// the main field
int gField[3][3];
int computerAnswer = 2;
int playerAnswer = 1;
gboolean gcomputerWin = FALSE;
gboolean gfoundDanger = FALSE;
gboolean gplayerWin = FALSE;


static void ClearField();
static void DoDrawing(cairo_t *);
static void DraweField(cairo_t *cr);
static void ComputerWin(int i, int j);
static void ComputerMove(int i, int j);	



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
	DraweField(cr);
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
			if (gField[i][j] == playerAnswer)
			{
				cairo_set_line_width(cr, 6);
				cairo_set_source_rgb(cr, 0.69, 0.19, 0);
				cairo_move_to(cr, i*100+50+g_circle_radius, j*100+50);
				cairo_arc(cr, i*100+50, j*100+50, g_circle_radius, 0, 2 * M_PI);
				cairo_stroke_preserve(cr);
				//cairo_close_path(cr);
			}
			if (gField[i][j] == computerAnswer)
			{
				cairo_set_line_width(cr, 6);
				cairo_set_source_rgb(cr, 0.69, 0.19, 0);
				cairo_move_to(cr, (i+1)*100-5, (j+1)*100-5);
				cairo_line_to(cr, i*100+5, j*100+5);
				cairo_move_to(cr, (i+1)*100-5, j*100+5);
				cairo_line_to(cr, i*100+5, (j+1)*100-5);
				cairo_stroke_preserve(cr);
			}
		}
	}

	//glob.count = 0;
	//cairo_stroke(cr);    
}

static void DraweField(cairo_t *cr)
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
static void CellDenermination(int x, int y)
{
	x /= 100;
	y /= 100;
	gField[x][y] = playerAnswer;
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
				free = i;			
			if ((filling == g_field_size - 1)&&(free >= 0))
			{
				//computer win!
				gcomputerWin = TRUE;
				ComputerMove(free,j);
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



static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (event->button == 1) 
	{
		int x = event->x;
		int y = event->y;
		CellDenermination(x,y);
		gplayerWin = FALSE;
		ChekingPlayerExactlyWin();
		if (gplayerWin)
		{
			//player exatly win, we must say about it
			gtk_widget_queue_draw(darea);
		}
		else
		{
			gcomputerWin = FALSE;
			ChekingComputerWin();
			if (gcomputerWin)
			{
				//computer do step and win, we must draw and say about it
				gtk_widget_queue_draw(darea);
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
						gField[1][1] == computerAnswer;
					}
					else
					{
						int m,k;
						do 
						{
							srand(time(NULL));
							k = (rand()/3)-1;
							m = (rand()/3)-1;
						}
						while (gField[k][m] != 0);
						gField[k][m] = computerAnswer;
					}
					gtk_widget_queue_draw(darea);		
				}
				
			}
		}
		
		
	}

	//if (event->button == 3) {
	//		gtk_widget_queue_draw(widget);
	//}
	return TRUE;
}

	
static void ComputerMove(int i, int j)
{
	gField[i][j] = computerAnswer;
}
	/*
	//new window
	GtkWidget *window2;
    GtkWidget *darea2;

	window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea2 = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window2), darea2);
 
	gtk_widget_add_events(window2, GDK_BUTTON_PRESS_MASK);

	// draw event
	g_signal_connect(G_OBJECT(darea2), "expose-event", 
			G_CALLBACK(on_draw_event), NULL); 
	//destroy event = quit
	g_signal_connect(window2, "destroy",
			G_CALLBACK(gtk_main_quit), NULL);  
	//clicked
	g_signal_connect(window2, "button-press-event", 
			G_CALLBACK(clicked), NULL);
 
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window2), 150, 300); 
	gtk_window_set_title(GTK_WINDOW(window2), "Computer WIN");

	cairo_t *cr2 = gdk_cairo_create(gtk_widget_get_window(window2));
	
	cairo_text_extents_t extents;

	const char *utf8 = "Sorry, computer win!";
	double x,y;

	cairo_select_font_face (cr2, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);

	cairo_set_font_size (cr2, 52.0);
	cairo_text_extents (cr2, utf8, &extents);
	x = 128.0-(extents.width/2 + extents.x_bearing);
	y = 128.0-(extents.height/2 + extents.y_bearing);
	

	cairo_move_to (cr2, x, y);
	cairo_show_text (cr2, utf8);

	gtk_widget_show_all(window2);

	ClearField();
	*/



int main(int argc, char *argv[])
{
	
		
	//glob.count = 0;

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
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
	gtk_window_set_title(GTK_WINDOW(window), "Crosses and zeros");

	gtk_widget_show_all(window);

	ClearField();

	gtk_main();

	return 0;
}
