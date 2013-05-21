#include "Crosses_zeros.h"
#include "Other_Cheking.h"

void ChekingPlayerExactlyWin(int ***gField, gboolean *gplayerWin, int *playerAnswer, int *computerAnswer)
{
	//player exactly win
	*gplayerWin = FALSE;
	int i;
	for (i = 0; i < g_field_size; i++)
	{
		if (*gplayerWin) break;
		int j;
		//checking veltical version 
		int filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (*gplayerWin) break;
			printf("%i\n", i);
			printf("%i\n", j);
			if (*gField[i][j] == (*playerAnswer))
				filling++;
			if (filling == g_field_size)
			{
				*gplayerWin = TRUE;
			}
		}

		//checking horizontal version
		filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (*gplayerWin) break;
			if (*gField[j][i] == *playerAnswer)
				filling++;
			if (filling == g_field_size)
			{
				
				*gplayerWin = TRUE;
			}
		}

		
		//cheking only once 
		if (i == 0)
		{
			//checking diagonal version
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (*gplayerWin) break;
				if (*gField[j][j] == *playerAnswer)
					filling++;
				if (filling == g_field_size)
				{
					*gplayerWin = TRUE;
				}
			}

			//checking reverse diagonal version
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (*gplayerWin) break;
				if (*gField[g_field_size - j][j] == *playerAnswer)
					filling++;
				if (filling == g_field_size)
				{
					*gplayerWin = TRUE;
				}
			}
		}
	}
}




//executing computer motion
void ChekingPlayerCanWin(int ***gField, gboolean *gfoundDanger, int *playerAnswer, int *computerAnswer)
{
	*gfoundDanger = FALSE;
	int i;
	for (i = 0; i < g_field_size; i++)
	{
		if (*gfoundDanger) break;
		int j;
		//checking veltical win version 
		int free = -1;
		int filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (*gfoundDanger) break;
			
			if (*gField[i][j] == *playerAnswer)
				filling++;
			if (*gField[i][j] == 0)
				free = j;			
			if ((filling == g_field_size -1 )&&(free >= 0))
			{
				//found dangerous situation
				*gfoundDanger = TRUE;
				ComputerMove(gField, computerAnswer, i, free);
			}
		}

		//checking horizontal win version
		free = -1;
		filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (*gfoundDanger) break;
			if (*gField[j][i] == *playerAnswer)
				filling++;
			if (*gField[j][i] == 0)
				free = j;			
			if ((filling == g_field_size - 1)&&(free >= 0))
			{
				
				//found dangerous situation
				*gfoundDanger = TRUE;
				ComputerMove(gField, computerAnswer, free, i);
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
				if (*gfoundDanger) break;
				if (*gField[j][j] == *playerAnswer)
					filling++;
				if (*gField[j][j] == 0)
					free = j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//found dangerous situation
					*gfoundDanger = TRUE;
					ComputerMove(gField, computerAnswer, free, free);
				}
			}

			//checking reverse diagonal win version
			free = -1;
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (*gfoundDanger) break;
				if (*gField[g_field_size - j-1][j] == *playerAnswer)
					filling++;
				if (*gField[g_field_size - j-1][j] == 0)
					free = j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//found dangerous situation
					*gfoundDanger = TRUE;
					ComputerMove(gField, computerAnswer, g_field_size - free - 1, free);
				}
			}
		}
	}
}

//executing computer motion
void ChekingComputerWin(int ***gField, gboolean *gcomputerWin, int *playerAnswer, int *computerAnswer)
{
	*gcomputerWin = FALSE;
	int i;
	for (i = 0; i < g_field_size; i++)
	{
		if (*gcomputerWin) break;
		int j;
		//checking veltical win version 
		int free = -1;
		int filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (*gcomputerWin) break;
			
			if (*gField[i][j] == *computerAnswer)
				filling++;
			if (gField[i][j] == 0)
				free = j;			
			if ((filling == g_field_size -1 )&&(free >= 0))
			{
				//computer win!
				*gcomputerWin = TRUE;
				ComputerMove(gField, computerAnswer, i, free);
			}
		}

		//checking horizontal win version
		free = -1;
		filling = 0;
		for (j = 0; j < g_field_size; j++)
		{
			if (*gcomputerWin) break;
			if (*gField[j][i] == *computerAnswer)
				filling++;
			if (gField[j][i] == 0)
				free = j;			
			if ((filling == g_field_size - 1)&&(free >= 0))
			{
				//computer win!
				*gcomputerWin = TRUE;
				ComputerMove(gField, computerAnswer, free, i);
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
				if (*gcomputerWin) break;
				if (*gField[j][j] == *computerAnswer)
					filling++;
				if (gField[j][j] == 0)
					free = j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//computer win!
					*gcomputerWin = TRUE;
					ComputerMove(gField, computerAnswer, free, free);
				}
			}

			//checking reverse diagonal win version
			free = -1;
			filling = 0;
			for (j = 0; j < g_field_size; j++)
			{
				if (*gcomputerWin) break;
				if (*gField[g_field_size - j][j] == *computerAnswer)
					filling++;
				if (gField[g_field_size - j][j] == 0)
					free = g_field_size - j;			
				if ((filling == g_field_size -1 )&&(free >= 0))
				{
					//computer win!
					*gcomputerWin = TRUE;
					ComputerMove(gField, computerAnswer, free, j);
				}
			}
		}
	}
}

void ComputerMove(int ***gField, int *computerAnswer, int i, int j)
{
	*gField[i][j] = *computerAnswer;
}