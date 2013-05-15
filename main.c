#include <cairo.h>
#include <gtk/gtk.h>


static void DoDrawing(cairo_t *);
static void DraweField(cairo_t *cr);
// the main field
bool gField[3][3];


/*
struct cell 
{
	int count;
	double coordx[100];
	double coordy[100];
};
*/

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
	for (i = 0; i < 3 ; i++ ) 
	{
		for (j = 0; j < 3 ; j++ ) 
		{	
			if (gField[i][j])
			{
				cairo_move_to(cr, i*100+50, j*100+50);
				cairo_line_to(cr, i*100+50, j*100+50);
			}
		}
	}

	glob.count = 0;
	cairo_stroke(cr);    
}

static void DraweField(cairo_t *cr)
{
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 1);
	
	int i;
	for (i = 1; i < 3; ++i)
	{
		cairo_move_to (cr, i*100, 0);
		cairo_line_to (cr, i*100, 300);
		cairo_move_to (cr, 0, i*100);
		cairo_line_to (cr, 300, i*100);
	}
	cairo_stroke(cr);
}

static void CellDenermination(int x, int y)
{
	x /= 100;
	y /= 100;
	gField[x][y] = 1;

}





static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (event->button == 1) 
	{
		int x = event->x;
		int y = event->y;
		CellDenermination(x,y);

		//	glob.coordx[glob.count] = event->x;
		//	glob.coordy[glob.count++] = event->y;
			G_CALLBACK(on_draw_event);
		}

		//if (event->button == 3) {
		//		gtk_widget_queue_draw(widget);
		//}

		return TRUE;
}


int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *darea;
	
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
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 300); 
	gtk_window_set_title(GTK_WINDOW(window), "Crosses and zeros");

	gtk_widget_show_all(window);


	gtk_main();

	return 0;
}
