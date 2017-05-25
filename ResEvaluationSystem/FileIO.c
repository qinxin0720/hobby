#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include "MacroDefine.h"
#include "MainPanel.h"

int saveFileFunctionId = 0;

int mystrncpy (char * outputString, const int outputStringlen, const char inputString[], const char symbol)
{
	int len = strlen (inputString);
	assert ((outputString != NULL) && (inputString != NULL));
	if (len > 0)
	{
		for (int i=0; i<len; i++)
		{
			if (symbol == inputString[i])
			{
				if (outputStringlen <= i)
				{
					return -1;
				}
				else
				{
					break;
				}
			}
			if ( i == len)
			{
				return -3;
			}
		}				
		for (int i=0; i<len; i++)
		{
			outputString[i] = inputString[i];
			if (symbol == inputString[i])
			{
				outputString[i+1] = 0;
				return 1;
			}
		}
	}
	return -2;
}

void openConfigFile (void)
{
	FILE * fp;
	char pathString[MAX_PATHNAME_LEN];
	char tempString[MAX_PATHNAME_LEN];
	
	GetProjectDir (pathString);
	strcat (pathString, "\\config\\gpib_address.config");
	fp = fopen (pathString, "r");
	if (fp != NULL)
	{
		fgets (tempString, MAX_PATHNAME_LEN, fp);
		gpib_address0 = atoi (tempString+14);
		fgets (tempString, MAX_PATHNAME_LEN, fp);
		gpib_address1 = atoi (tempString+14);
		fclose (fp);
	}
	else
	{
		gpib_address0 = 0;
		gpib_address1 = 0;
		MessagePopup ("Warning", "GPIB Configuration File is Missed!");
	}
}

void saveConfigFile (void)
{
	int fileHandle;
	char pathString[MAX_PATHNAME_LEN];
	char tempString[MAX_PATHNAME_LEN];
	
	GetProjectDir (pathString);
	strcat (pathString, "\\config\\gpib_address.config");
	fileHandle = OpenFile (pathString, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_ASCII);
	GetCtrlVal (setting_panelHandle, PANEL2_GPIB_ADDRESS0, &gpib_address0);
	GetCtrlVal (setting_panelHandle, PANEL2_GPIB_ADDRESS1, &gpib_address1);
	Fmt (tempString, "%s<%s%d\n", "gpib_address0=", gpib_address0);
	WriteFile (fileHandle, tempString, strlen (tempString)); 
	Fmt (tempString, "%s<%s%d", "gpib_address1=", gpib_address1);
	WriteFile (fileHandle, tempString, strlen (tempString));
	CloseFile (fileHandle);
}

void SetRangeRowAttribute (int TotalRow)
{
	char slot[2] = {0};
	char pair[8] = {0};
	SetCtrlAttribute (main_panelHandle, MAIN_PANEL_TABLE, ATTR_VERTICAL_GRID_COLOR, VAL_BLACK);
	SetCtrlAttribute (main_panelHandle, MAIN_PANEL_TABLE, ATTR_HORIZONTAL_GRID_COLOR, VAL_BLACK);
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, TotalRow, 12), ATTR_TEXT_POINT_SIZE, 20);  
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, TotalRow, 3), ATTR_TEXT_BGCOLOR, MakeColor (192, 192, 192));

	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, TotalRow, 2), ATTR_CELL_TYPE, VAL_CELL_RING); 
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 4, TotalRow, 1), ATTR_CELL_TYPE, VAL_CELL_RING); 
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 7, TotalRow, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC); 
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, TotalRow, 1), ATTR_CELL_TYPE, VAL_CELL_RING); 
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 9, TotalRow, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC); 
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, TotalRow, 1), ATTR_CELL_TYPE, VAL_CELL_RING);
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 11, TotalRow, 1), ATTR_CELL_TYPE, VAL_CELL_NUMERIC);  
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 12, TotalRow, 1), ATTR_CELL_TYPE, VAL_CELL_RING);
	for (int i=1; i<6; i++)
	{
		Fmt (slot, "%d", i);
		InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, TotalRow, 1), i-1, slot);
	}
	
	for (int i=1; i<21; i++)
	{
		sprintf (pair, "%02d-%02d", i, i+20);
		InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 2, TotalRow, 1), i-1, pair);
	}
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 4, TotalRow, 1), ATTR_TEXT_BGCOLOR, MakeColor (55, 0, 55)); 
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 4, TotalRow, 1), ATTR_TEXT_COLOR, MakeColor (255, 255, 155));
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 4, TotalRow, 1), 0, "Open");
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 4, TotalRow, 1), 1, "Close"); 
	
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 5, TotalRow, 1), ATTR_TEXT_BGCOLOR, MakeColor (204, 102, 255));
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 6, TotalRow, 1), ATTR_TEXT_BGCOLOR, MakeColor (204, 102, 255));
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 7, TotalRow, 1), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 255));
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, TotalRow, 1), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 255));
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, TotalRow, 1), ATTR_TEXT_FONT, VAL_MENU_FONT); 
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, TotalRow, 1), 0, "m次");
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, TotalRow, 1), 1, "次");
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, TotalRow, 1), 2, "k次"); 
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, TotalRow, 1), 3, "M次"); 
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 9, TotalRow, 1), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 0));
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, TotalRow, 1), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 0));
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, TotalRow, 1), ATTR_TEXT_FONT, VAL_MENU_FONT); 
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, TotalRow, 1), 0, "m次");
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, TotalRow, 1), 1, "次");
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, TotalRow, 1), 2, "k次"); 
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, TotalRow, 1), 3, "M次"); 
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 12, TotalRow, 1), ATTR_TEXT_FONT, VAL_MENU_FONT); 
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 12, TotalRow, 1), 0, "m次");
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 12, TotalRow, 1), 1, "次");
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 12, TotalRow, 1), 2, "k次"); 
	InsertTableCellRangeRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 12, TotalRow, 1), 3, "M次");
	
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 3, TotalRow, 1), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 4, TotalRow, 1), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 11, TotalRow, 1), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 12, TotalRow, 1), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 13, TotalRow, 1), ATTR_CELL_MODE, VAL_INDICATOR);
}

int CVICALLBACK saveFileThreadFunction (void *functionData)
{
	int numberOfRows = -1;
	int fileHandle, j;
	char tempString[MAX_PATHNAME_LEN] = {0}; 
	char tempString1[MAX_PATHNAME_LEN] = {0}; 
	
	char * (* slot);
	char * (* pair); 
	char * (* channel); 
	char * (* point1); 
	char * (* point2); 
	double * uplimit;
	char * (* uplimitUnit);
	double * lowlimit;
	char * (* lowlimitUnit); 
	
	GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows);
	if (numberOfRows > 0)
	{
		if ((slot = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
		{
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((pair = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
		{
			free (slot);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((channel = (char **) malloc (numberOfRows * sizeof(char *))) ==NULL)
		{
			free (slot);
			free (pair);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((point1 = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
		{
			free (slot);
			free (pair);
			free (channel);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((point2 = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
		{
			free (slot);
			free (pair);
			free (channel);
			free (point1);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((uplimit = (double *)malloc(numberOfRows * sizeof(double))) == NULL)
		{
			free (slot);
			free (pair);
			free (channel);
			free (point1);
			free (point2);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((uplimitUnit = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
		{
			free (slot);
			free (pair);
			free (channel);
			free (point1);
			free (point2);
			free (uplimit);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((lowlimit = (double *)malloc(numberOfRows * sizeof(double))) == NULL)
		{
			free (slot);
			free (pair);
			free (channel);
			free (point1);
			free (point2);
			free (uplimit);
			free (uplimitUnit);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
		if ((lowlimitUnit = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
		{
			free (slot);
			free (pair);
			free (channel);
			free (point1);
			free (point2);
			free (uplimit);
			free (uplimitUnit);
			free (lowlimit);
			MessagePopup ("Warning", "Out of memory!");
			return -1;
		}
	
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, numberOfRows, 1), slot, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 2, numberOfRows, 1), pair, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 3, numberOfRows, 1), channel, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 5, numberOfRows, 1), point1, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 6, numberOfRows, 1), point2, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 7, numberOfRows, 1), uplimit, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, numberOfRows, 1), uplimitUnit, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 9, numberOfRows, 1), lowlimit, VAL_COLUMN_MAJOR);
		GetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, numberOfRows, 1), lowlimitUnit, VAL_COLUMN_MAJOR);
	
		fileHandle = OpenFile ((char *)functionData, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_ASCII); 
		strcpy (tempString, "[SEQUENCE]\n");
		WriteFile (fileHandle, tempString, strlen (tempString));
		Fmt (tempString1, "%s<%d", numberOfRows);
		strcpy (tempString, "number=");
		strcat (tempString, tempString1);
		strcat (tempString, ",\n");
		WriteFile (fileHandle, tempString, strlen (tempString));
		j = 0;
		for (int i=0; i<numberOfRows; i++)
		{
			strcpy (tempString, "[NODE");
			j ++;
			Fmt (tempString1, "%s<%d", j); 
			strcat (tempString, tempString1);
			strcat (tempString, "]\n");
			WriteFile (fileHandle, tempString, strlen (tempString));
			//---slot
			strcpy (tempString, "slot=");
			strcat (tempString, slot[i]);
			strcat (tempString, ",");
			//---pair---
			strcat (tempString, "pair=");
			strcat (tempString, pair[i]);
			strcat (tempString, ",");
			//---channel---
			strcat (tempString, "channel=");
			strcat (tempString, channel[i]);
			strcat (tempString, ",");
			//---point1---
			strcat (tempString, "point1=");
			strcat (tempString, point1[i]);
			strcat (tempString, ",");
			//---point2---
			strcat (tempString, "point2=");
			strcat (tempString, point2[i]);
			strcat (tempString, ",");
			//---uplimit---
			strcat (tempString, "uplimit=");
			Fmt (tempString1, "%s<%f", uplimit[i]); 
			strcat (tempString, tempString1);
			strcat (tempString, ",");
			//---uplimitUnit---
			strcat (tempString, "uplimitUnit=");
			strcat (tempString, uplimitUnit[i]);
			strcat (tempString, ",");
			//---lowlimit----------
			strcat (tempString, "lowlimit=");
			Fmt (tempString1, "%s<%f", lowlimit[i]);
			strcat (tempString, tempString1);
			strcat (tempString, ",");
			//---lowlimitUnit---
			strcat (tempString, "lowlimitUnit=");
			strcat (tempString, lowlimitUnit[i]);
			strcat (tempString, ",");
			//-----------------------------------
			strcat (tempString, "\n");
			WriteFile (fileHandle, tempString, strlen (tempString));
		}
		strcpy (tempString, "[END]\n");
		WriteFile (fileHandle, tempString, strlen (tempString));
		CloseFile (fileHandle);
		
		free (slot);
		free (pair);
		free (channel);
		free (point1);
		free (point2);
		free (uplimit);
		free (uplimitUnit);
		free (lowlimit);
		free (lowlimitUnit);
	}
	return 0;
}


int openFile (char fileName[])
{
	FILE * fp;
	int i, j;
	int numberOfRows = -1;
	
	char * pCompare;
	char tempString[MAX_PATHNAME_LEN] = {0}; 
	
	char * (* slot);
	char (* Slot)[2];
	
	char * (* pair); 
	char (* Pair)[8];
	
	char * (* channel); 
	char (* Channel)[4];
	
	char * (* point1); 
	char (* Point1)[16];
	
	char * (* point2);
	char (* Point2)[16];
	
	double * uplimit;
	char * (* uplimitUnit);
	char (* UplimitUnit)[4];
	
	double * lowlimit;
	char * (* lowlimitUnit);
	char (* LowlimitUnit)[4];
	
	if ((fp = fopen (fileName, "r")) != NULL)
	{
		i = 0;
		DeleteTableRows (main_panelHandle, MAIN_PANEL_TABLE, 1, -1); 
		fgets(tempString, MAX_PATHNAME_LEN, fp);
		if ((pCompare = strstr (tempString, "[SEQUENCE]")) != NULL)
		{
			fgets(tempString, MAX_PATHNAME_LEN, fp);
			if ((pCompare = strstr (tempString, "number=")) != NULL)
			{
				numberOfRows = atoi (pCompare+7);
				if ((slot = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
				{
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if((Slot = (char *) malloc (sizeof(char)*numberOfRows*2)) == NULL)
				{   
					free (slot);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((pair = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
				{
					free (slot);
					free (Slot); 
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if((Pair = (char *) malloc (sizeof(char)*numberOfRows*8)) == NULL)
				{
					free (slot);
					free (Slot);
					free (pair);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((channel = (char **) malloc (numberOfRows * sizeof(char *))) ==NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if((Channel = (char *) malloc (sizeof(char)*numberOfRows*4)) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((point1 = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if((Point1 = (char *) malloc (sizeof(char)*numberOfRows*16)) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((point2 = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if((Point2 = (char *) malloc (sizeof(char)*numberOfRows*16)) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					free (point2); 
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((uplimit = (double *) malloc (numberOfRows * sizeof(double))) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					free (point2);
					free (Point2);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((uplimitUnit = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					free (point2);
					free (Point2);
					free (uplimit);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if((UplimitUnit = (char *) malloc (sizeof(char)*numberOfRows*4)) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					free (point2);
					free (Point2);
					free (uplimit);
					free (uplimitUnit);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((lowlimit = (double *) malloc (numberOfRows * sizeof(double))) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					free (point2);
					free (Point2);
					free (uplimit);
					free (uplimitUnit);
					free (UplimitUnit); 
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if ((lowlimitUnit = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					free (point2);
					free (Point2);
					free (uplimit);
					free (uplimitUnit);
					free (UplimitUnit);
					free (lowlimit);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				if((LowlimitUnit = (char *) malloc (sizeof(char)*numberOfRows*4)) == NULL)
				{
					free (slot);
					free (Slot);
					free (Pair);
					free (pair);
					free (channel);
					free (Channel);
					free (point1);
					free (Point1);
					free (point2);
					free (Point2);
					free (uplimit);
					free (uplimitUnit);
					free (UplimitUnit);
					free (lowlimit);
					free (LowlimitUnit);
					MessagePopup ("Warning", "Out of memory!");
					return -1;
				}
				fgets(tempString, MAX_PATHNAME_LEN, fp); 
				pCompare = strstr (tempString, "[NODE");
				while (pCompare != NULL)
				{
					fgets(tempString, MAX_PATHNAME_LEN, fp); 
					//---slot--
					pCompare = strstr (tempString, "slot=");
					if(pCompare != NULL)
					{
						mystrncpy (Slot[i], MAX_PATHNAME_LEN, pCompare+5, '\,');
						Slot[i][strlen (Slot[i])-1] = 0;  
					}
					//---pair--
					pCompare = strstr (tempString, "pair=");
					if(pCompare != NULL)
					{
						mystrncpy (Pair[i], MAX_PATHNAME_LEN, pCompare+5, '\,');
						Pair[i][strlen (Pair[i])-1] = 0;  
					}
					//---channel--
					pCompare = strstr (tempString, "channel=");
					if(pCompare != NULL)
					{
						mystrncpy (Channel[i], MAX_PATHNAME_LEN, pCompare+8, '\,');
						Channel[i][strlen (Channel[i])-1] = 0;  
					}
					//---point1--
					pCompare = strstr (tempString, "point1=");
					if(pCompare != NULL)
					{
						mystrncpy (Point1[i], MAX_PATHNAME_LEN, pCompare+7, '\,');
						Point1[i][strlen (Point1[i])-1] = 0;  
					}
					//---point2--
					pCompare = strstr (tempString, "point2=");
					if(pCompare != NULL)
					{
						mystrncpy (Point2[i], MAX_PATHNAME_LEN, pCompare+7, '\,');
						Point2[i][strlen (Point2[i])-1] = 0;  
					}
					//---uplimit--
					pCompare = strstr (tempString, "uplimit=");
					if(pCompare != NULL)
					{
						uplimit[i] = atof (pCompare+8);  
					}
					//---uplimitUnit--
					pCompare = strstr (tempString, "uplimitUnit=");
					if(pCompare != NULL)
					{
						mystrncpy (UplimitUnit[i], MAX_PATHNAME_LEN, pCompare+12, '\,');
						UplimitUnit[i][strlen (UplimitUnit[i])-1] = 0;  
					}
					//---lowlimit--
					pCompare = strstr (tempString, "lowlimit=");
					if(pCompare != NULL)
					{
						lowlimit[i] = atof (pCompare+9);  
					}
					//---lowlimitUnit--
					pCompare = strstr (tempString, "lowlimitUnit=");
					if(pCompare != NULL)
					{
						mystrncpy (LowlimitUnit[i], MAX_PATHNAME_LEN, pCompare+13, '\,');
						LowlimitUnit[i][strlen (LowlimitUnit[i])-1] = 0;  
					}
					fgets(tempString, MAX_PATHNAME_LEN, fp);
					pCompare = strstr (tempString, "[NODE");
					i ++;
				}
				fclose (fp);
				InsertTableRows (main_panelHandle, MAIN_PANEL_TABLE, 1, numberOfRows, 1);  
				for (j=0; j<numberOfRows; j++)
				{
					slot[j] = Slot[j];
					pair[j] = Pair[j];
					channel[j] = Channel[j];
					point1[j] = Point1[j];
					point2[j] = Point2[j];
					uplimitUnit[j] = UplimitUnit[j];
					lowlimitUnit[j] = LowlimitUnit[j];
				}
				SetRangeRowAttribute (numberOfRows);
				
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, numberOfRows, 1), slot, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 2, numberOfRows, 1), pair, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 3, numberOfRows, 1), channel, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 5, numberOfRows, 1), point1, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 6, numberOfRows, 1), point2, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 7, numberOfRows, 1), uplimit, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 8, numberOfRows, 1), uplimitUnit, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 9, numberOfRows, 1), lowlimit, VAL_COLUMN_MAJOR);
				SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 10, numberOfRows, 1), lowlimitUnit, VAL_COLUMN_MAJOR);
				
				free (slot);
				free (Slot);
				free (pair);
				free (Pair);
				free (channel);
				free (Channel);
				free (point1);
				free (Point1);
				free (point2);
				free (Point2);
				free (uplimit);
				free (uplimitUnit);
				free (UplimitUnit);
				free (lowlimit);
				free (lowlimitUnit);
				free (LowlimitUnit);
			}
			else
			{
				MessagePopup ("Warning", "The file is damaged!");
				return -1;
			}
		}
		else
		{
			MessagePopup ("Warning", "The file is damaged!"); 
			return -1;
		}
	}
	else
	{
		MessagePopup ("Warning", "File open filed!");
	}
	return 0;
}

int CVICALLBACK fileIO (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	char defaultDir[MAX_PATHNAME_LEN] = {0};
	char pathName[MAX_PATHNAME_LEN] = {0};
	switch (event)
	{
		case EVENT_COMMIT:
			switch (control)
			{
				case MAIN_PANEL_SAVE:
					GetProjectDir (defaultDir);
					if (FileSelectPopupEx (defaultDir, "*.d", "*.d", "Please select a file!", VAL_SAVE_BUTTON, 0, 0, pathName) > 0)
					{
						CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, saveFileThreadFunction, pathName, &saveFileFunctionId);
						CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, saveFileFunctionId, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
						CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, saveFileFunctionId);
					}
					break;
				case MAIN_PANEL_OPEN:
					if (testStatus == 0)
					{
						GetProjectDir (defaultDir);
						if (FileSelectPopupEx (defaultDir, "*.d", "*.d", "Please select a file!", VAL_SELECT_BUTTON, 0, 0, pathName) > 0)
						{
							openFile (pathName);
						}
					}
					else
					{
						MessagePopup ("Warning", "Testing tasks in progress!");
					}
					break;
			}
			break;
	}
	return 0;
}

void CVICALLBACK menuCB (int menuBar, int menuItem, void *callbackData,
						 int panel)
{
	int numberOfRows;
	char responseBuffer[9] = {0};
	switch (menuItem)
	{
		case MENUBAR_FILE_OPEN:
			fileIO (0, MAIN_PANEL_OPEN, EVENT_COMMIT, NULL, 0, 0);
			GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows);
			if (!edit_mode)
			{
				if (numberOfRows > 0)
				{
					SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, numberOfRows, 13), ATTR_CELL_MODE, VAL_INDICATOR);
				}
			}
			break;
		case MENUBAR_FILE_SAVE:
			fileIO (0, MAIN_PANEL_SAVE, EVENT_COMMIT, NULL, 0, 0);
			break;
		case MENUBAR_FILE_EDIT:
			if (testStatus == 0)
			{
				edit_mode = 1;
				SetCtrlAttribute (main_panelHandle, MAIN_PANEL_MOVEUP, ATTR_VISIBLE, 1);
				SetCtrlAttribute (main_panelHandle, MAIN_PANEL_MOVEDOWN, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (main_panelHandle, MAIN_PANEL_DELETE, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (main_panelHandle, MAIN_PANEL_ADD, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (main_panelHandle, MAIN_PANEL_CLEAR, ATTR_VISIBLE, 1); 
				SetCtrlAttribute (main_panelHandle, MAIN_PANEL_QUIT_EDITMODE, ATTR_VISIBLE, 1);
				GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows);
				if (numberOfRows > 0)
				{
					SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, numberOfRows, 2), ATTR_CELL_MODE, VAL_HOT);
					SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 5, numberOfRows, 6), ATTR_CELL_MODE, VAL_HOT);
				}
			}
			else
			{
				MessagePopup ("Warning", "Testing tasks in progress!"); 
			}
			break;
		case MENUBAR_SETTING_SET_GPIB_ADDRESS:
			if (testStatus == 0)
			{
				InstallPopup (setting_panelHandle);
			}
			else
			{
				MessagePopup ("Warning", "Testing tasks in progress!");
			}
			break;
		case MENUBAR_FILE_QUIT:
			quit (0, 0, EVENT_COMMIT, NULL, 0, 0);
			break;
		case MENUBAR_TEST_START:
			test (0, MAIN_PANEL_START, EVENT_COMMIT, NULL, 0, 0);
			break;
		case MENUBAR_TEST_STOP:
			test (0, MAIN_PANEL_STOP, EVENT_COMMIT, NULL, 0, 0);
			break;
		case MENUBAR_TEST_SINGLE_TEST:
			if (testStatus == 0)
			{
				loopValue = 1;
				SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG, "Single Test"); 
				SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG_2, "0/1"); 
				SetMenuBarAttribute (menuBarHandle, MENUBAR_TEST_SINGLE_TEST, ATTR_CHECKED, 1); 
				SetMenuBarAttribute (menuBarHandle, MENUBAR_TEST_LOOP_TEST, ATTR_CHECKED, 0);
			}
			else
			{
				MessagePopup ("Warning", "Testing tasks in progress!"); 
			}
			break;
		case MENUBAR_TEST_LOOP_TEST:
			if (testStatus == 0)
			{
				PromptPopup ("Message", "Please input loopValue!", responseBuffer, (size_t)8);
				loopValue = atoi (responseBuffer);
				if (loopValue >= 2)
				{
					SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG, "Loop Test");
					sprintf (responseBuffer, "0/%d", loopValue);
					SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG_2, responseBuffer); 
					SetMenuBarAttribute (menuBarHandle, MENUBAR_TEST_SINGLE_TEST, ATTR_CHECKED, 0); 
					SetMenuBarAttribute (menuBarHandle, MENUBAR_TEST_LOOP_TEST, ATTR_CHECKED, 1);
				}
				else
				{
					loopValue = 1;
					SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG, "Single Test"); 
					SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG_2, "0/1");
					SetMenuBarAttribute (menuBarHandle, MENUBAR_TEST_SINGLE_TEST, ATTR_CHECKED, 1); 
					SetMenuBarAttribute (menuBarHandle, MENUBAR_TEST_LOOP_TEST, ATTR_CHECKED, 0);
				}
			}
			else
			{
				MessagePopup ("Warning", "Testing tasks in progress!"); 
			}
			break;
		case MENUBAR_HELP_ABOUT:
			InstallPopup (about_panelHandle);
			break;
	}
}

int CVICALLBACK SaveConfig (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			saveConfigFile ();
			HidePanel (setting_panelHandle);
			break;
	}
	return 0;
}
