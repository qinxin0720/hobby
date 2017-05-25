#include <Windows.h> 
#include <odbcinst.h>
#include "cvi_db.h"
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h>
#include <cvirte.h>
#include "LoginPanel.h"
#include "MainPanel.h"
#include "MacroDefine.h"

int g_row = -1;
int g_column = -1;
int edit_mode = 0;
int gpib_address0 = -1;
int gpib_address1 = -1; 

int login_panelHandle = -1;
int main_panelHandle = -1;
int setting_panelHandle = -1;
int about_panelHandle = -1;

int CreatDatabase (void);

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;    /* out of memory */
	login_panelHandle = LoadPanel (0, "LoginPanel.uir", LOGI_PANEL);
	DisplayPanel (login_panelHandle);
	
	SetCtrlAttribute (login_panelHandle, LOGI_PANEL_PASSWORD, ATTR_ENABLE_CHARACTER_MASKING, 1); 
	HWND hWnd = FindWindow (NULL, "Login");	   //Get the handle of login panel.
	ImmAssociateContext (hWnd, NULL);		   //Disable IME.
	
	RunUserInterface ();
	return 0;
}

int CVICALLBACK quit (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if (testStatus == 0)
			{
				QuitUserInterface (0);
			}
			else
			{
				MessagePopup ("Warning", "Testing tasks in progress!");
			}
			break;
	}
	return 0;
}

int CVICALLBACK login (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	char username[16] = {0};
	char userpasswd[16] = {0};
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (login_panelHandle, LOGI_PANEL_USERNAME , username);
			GetCtrlVal (login_panelHandle, LOGI_PANEL_PASSWORD , userpasswd);
			
			if ((strcmp (username, "admin") ==0) && (strcmp (userpasswd, "admin") == 0))
			{
				SetCtrlAttribute (login_panelHandle, LOGI_PANEL_TEXTMSG, ATTR_VISIBLE, 0);
				main_panelHandle = LoadPanel (0, "MainPanel.uir", MAIN_PANEL);
				setting_panelHandle = LoadPanel (main_panelHandle, "MainPanel.uir", PANEL2);
				about_panelHandle = LoadPanel (main_panelHandle, "MainPanel.uir", ABOUT);
				openConfigFile ();
				SetCtrlVal (setting_panelHandle, PANEL2_GPIB_ADDRESS0, gpib_address0);
				SetCtrlVal (setting_panelHandle, PANEL2_GPIB_ADDRESS1, gpib_address1);
				CreatDatabase ();
			
				SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG, "Single Test");
			
				DiscardPanel (login_panelHandle);
				DisplayPanel (main_panelHandle);
			}
			else
			{
				SetCtrlAttribute (login_panelHandle, LOGI_PANEL_TEXTMSG, ATTR_VISIBLE, 1);
			}
			break;
	}
	return 0;
}

int CVICALLBACK tableCB (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	char pos[20] = {0};
	char str[8] = {0};
	
	char slot[2] = {0};
	char pair[8] = {0};
	char channel[8] = {0};
	switch (event)
	{
		case EVENT_COMMIT:

			break;
		case EVENT_TABLE_ROW_COL_LABEL_CLICK:
			g_row = eventData1;
			break;
		case EVENT_VAL_CHANGED:
			if ((eventData2 == 1) || (eventData2 == 2))
			{
				GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (1, eventData1), slot);
				GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (2, eventData1), pair);  
				strcpy (channel, slot);
				strncat (channel, pair, 2);
				SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (3, eventData1), channel); 
			}
			break;
		case EVENT_MOUSE_POINTER_MOVE: 
			strcpy (pos, "MP X:");
			Fmt (str, "%s<%d", eventData2);
			strcat (pos, str);
			strcat (pos, "\ Y:");
			Fmt (str, "%s<%d", eventData1);
			strcat (pos, str);
			SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG_4, pos);
			break;
	}
	return 0;
}


void SetNewRowAttribute (int RowNum)
{
	char slot[2] = {0};
	char pair[8] = {0};
	SetCtrlAttribute (main_panelHandle, MAIN_PANEL_TABLE, ATTR_VERTICAL_GRID_COLOR, VAL_BLACK);
	SetCtrlAttribute (main_panelHandle, MAIN_PANEL_TABLE, ATTR_HORIZONTAL_GRID_COLOR, VAL_BLACK);
	for (int i=1; i<10; i++)
	{
		SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (i, RowNum), ATTR_TEXT_POINT_SIZE, 20);  
	}
	for (int i=1; i<4; i++)
	{
		SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (i, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (192, 192, 192));
	}
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (1, RowNum), ATTR_CELL_TYPE, VAL_CELL_RING); 
	for (int i=1; i<6; i++)
	{
		Fmt (slot, "%d", i);
		InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (1, RowNum), i-1, slot);
	}
	
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (2, RowNum), ATTR_CELL_TYPE, VAL_CELL_RING); 
	for (int i=1; i<21; i++)
	{
		sprintf (pair, "%02d-%02d", i, i+20);
		InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (2, RowNum), i-1, pair);
	}
	
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (55, 0, 55)); 
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, RowNum), ATTR_TEXT_COLOR, MakeColor (255, 255, 255));
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, RowNum), ATTR_CELL_TYPE, VAL_CELL_RING); 
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, RowNum), 0, "Open");
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, RowNum), 1, "Close"); 
	
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (5, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (204, 102, 255));
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (6, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (204, 102, 255));
	
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 255));
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, RowNum), ATTR_CELL_TYPE, VAL_CELL_NUMERIC); 
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 255));
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, RowNum), ATTR_CELL_TYPE, VAL_CELL_RING);
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, RowNum), ATTR_TEXT_FONT, VAL_MENU_FONT); 
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, RowNum), 0, "m次");
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, RowNum), 1, "次");
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, RowNum), 2, "k次"); 
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, RowNum), 3, "M次"); 
	
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 0));
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, RowNum), ATTR_CELL_TYPE, VAL_CELL_NUMERIC); 
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, RowNum), ATTR_TEXT_BGCOLOR, MakeColor (0, 204, 0));
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, RowNum), ATTR_CELL_TYPE, VAL_CELL_RING);
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, RowNum), ATTR_TEXT_FONT, VAL_MENU_FONT); 
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, RowNum), 0, "m次");
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, RowNum), 1, "次");
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, RowNum), 2, "k次"); 
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, RowNum), 3, "M次");
	
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (11, RowNum), ATTR_CELL_TYPE, VAL_CELL_NUMERIC); 
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, RowNum), ATTR_CELL_TYPE, VAL_CELL_RING); 
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, RowNum), ATTR_TEXT_FONT, VAL_MENU_FONT); 
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, RowNum), 0, "m次");
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, RowNum), 1, "次");
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, RowNum), 2, "k次"); 
	InsertTableCellRingItem (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, RowNum), 3, "M次");
	
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (3, RowNum), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, RowNum), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (11, RowNum), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, RowNum), ATTR_CELL_MODE, VAL_INDICATOR);
	SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (13, RowNum), ATTR_CELL_MODE, VAL_INDICATOR);
}



int CVICALLBACK editTable (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int numberOfRows = -1;
	
	char slot[2] = {0};
	char pair[8] = {0};
	char channel[4] = {0};
	char status[8] = {0};
	char point1[16] = {0};
	char point2[16] = {0};
	double uplimit;
	char uplimitUnit[4] = {0};
	double lowlimit;
	char lowlimitUnit[4] = {0};
	
	switch (event)
	{
		case EVENT_COMMIT:
			switch (control)
			{
				case MAIN_PANEL_MOVEUP:
					if (g_row > 0)
					{
						if (g_row >= 2)
						{
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (1, g_row), slot); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (2, g_row), pair); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (3, g_row), channel); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, g_row), status); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (5, g_row), point1); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (6, g_row), point2); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, g_row), &uplimit); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, g_row), uplimitUnit); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, g_row), &lowlimit); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, g_row), lowlimitUnit); 
							DeleteTableRows (main_panelHandle, MAIN_PANEL_TABLE, g_row, 1);
							InsertTableRows (main_panelHandle, MAIN_PANEL_TABLE, g_row-1, 1, 1); 
							SetNewRowAttribute (g_row-1);
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (1, g_row-1), slot); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (2, g_row-1), pair); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (3, g_row-1), channel); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, g_row-1), status); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (5, g_row-1), point1); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (6, g_row-1), point2); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, g_row-1), uplimit); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, g_row-1), uplimitUnit); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, g_row-1), lowlimit); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, g_row-1), lowlimitUnit); 
							g_row -= 1;
						}
					}
					break;
				case MAIN_PANEL_MOVEDOWN:
					GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows);
					if (g_row > 0)
					{
						if (g_row <= numberOfRows-1)
						{
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (1, g_row), slot); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (2, g_row), pair); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (3, g_row), channel); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, g_row), status); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (5, g_row), point1); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (6, g_row), point2); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, g_row), &uplimit); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, g_row), uplimitUnit); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, g_row), &lowlimit); 
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, g_row), lowlimitUnit); 
							DeleteTableRows (main_panelHandle, MAIN_PANEL_TABLE, g_row, 1);
							InsertTableRows (main_panelHandle, MAIN_PANEL_TABLE, g_row+1, 1, 1); 
							SetNewRowAttribute (g_row+1);
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (1, g_row+1), slot); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (2, g_row+1), pair); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (3, g_row+1), channel); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, g_row+1), status); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (5, g_row+1), point1); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (6, g_row+1), point2); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, g_row+1), uplimit); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, g_row+1), uplimitUnit); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, g_row+1), lowlimit); 
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, g_row+1), lowlimitUnit);
							g_row += 1;
						}
					}
					break;
				case MAIN_PANEL_DELETE:
					if (g_row > 0)
					{
						DeleteTableRows (main_panelHandle, MAIN_PANEL_TABLE, g_row, 1);
						g_row = -1;
					}
					break;
				case MAIN_PANEL_ADD:
					InsertTableRows (main_panelHandle, MAIN_PANEL_TABLE, -1, 1, 1);
					GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows);
					SetNewRowAttribute (numberOfRows);
					g_row = -1; 
					break;
				case MAIN_PANEL_CLEAR:
					DeleteTableRows (main_panelHandle, MAIN_PANEL_TABLE, 1, -1); 
					g_row = -1; 
					break;
				case MAIN_PANEL_QUIT_EDITMODE:
					edit_mode = 0;
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_MOVEUP, ATTR_VISIBLE, 0);
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_MOVEDOWN, ATTR_VISIBLE, 0); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_DELETE, ATTR_VISIBLE, 0); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_ADD, ATTR_VISIBLE, 0); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_CLEAR, ATTR_VISIBLE, 0); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_QUIT_EDITMODE, ATTR_VISIBLE, 0); 
					GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows);
					if (numberOfRows > 0)
					{
						SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 1, numberOfRows, 13), ATTR_CELL_MODE, VAL_INDICATOR);
					}
					break;
			}
			break;
	}
	return 0;
}

int CVICALLBACK hideGpigAddressSetting (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (setting_panelHandle);
			break;
	}
	return 0;
}

int CreatDatabase (void)
{
	int hdbc, hstmt;
	char tempString[MAX_PATHNAME_LEN] = {0};
	char tempString1[MAX_PATHNAME_LEN] = {0};
	DisableBreakOnLibraryErrors (); 
	if ((hdbc = DBConnect ("DSN=RESDataSource")) > 0)
	{
		if ((hstmt = DBActivateSQL (hdbc, "SELECT * FROM TESTDATA")) > 0)
		{
			;
		}
		else
		{
			GetProjectDir (tempString);
			strcat (tempString, "\\config\\createTableTESTDATA.sql");
			strcpy (tempString1, "sqlcmd\ -i\ \"");
			strcat (tempString1, tempString);
			strcat (tempString1, "\"");
			system (tempString1);
			if ((hstmt = DBActivateSQL (hdbc, "SELECT * FROM TESTDATA")) > 0)
			{
				;
			}
			else
			{
				MessagePopup ("Warning", "Failed to connect SQL Server!");
				return -2;
			}
		}
	}
	else
	{
		SQLConfigDataSource (NULL, ODBC_ADD_DSN, "SQL Server", "DSN=RESDataSource\0" "DATABASE=RES\0" "Server=localhost\0" "Trusted_Connection=Yes\0");
		GetProjectDir (tempString);
		strcat (tempString, "\\config\\createDatabaseRES.sql");
		strcpy (tempString1, "sqlcmd\ -i\ \"");
		strcat (tempString1, tempString);
		strcat (tempString1, "\"");
		system (tempString1);
		if ((hdbc = DBConnect ("DSN=RESDataSource")) > 0) 
		{
			if ((hstmt = DBActivateSQL (hdbc, "SELECT * FROM TESTDATA")) > 0)
			{
				;
			}
			else
			{
				GetProjectDir (tempString);
				strcat (tempString, "\\config\\createTableTESTDATA.sql");
				strcpy (tempString1, "sqlcmd\ -i\ \"");
				strcat (tempString1, tempString);
				strcat (tempString1, "\"");
				system (tempString1);
				if ((hstmt = DBActivateSQL (hdbc, "SELECT * FROM TESTDATA")) > 0)
				{
					;
				}
				else
				{
					MessagePopup ("Warning", "Failed to connect SQL Server!");
					return -2;
				}
			}
		}
		else
		{
			MessagePopup ("Warning", "Failed to connect SQL Server!");
			return -1;
		}
	}
	DBDeactivateSQL (hstmt);
	DBDisconnect (hdbc);
	return 0;
}


int CVICALLBACK GetDatetime (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	char datetime[32] = {0};
	int year, month, day, hours, minutes, seconds;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			GetSystemDate (&month, &day, &year);
			GetSystemTime (&hours, &minutes, &seconds);
			sprintf (datetime, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hours, minutes, seconds);
			SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG_3, datetime);
			break;
	}
	return 0;
}

int CVICALLBACK HideAboutPanel (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (about_panelHandle);
			break;
	}
	return 0;
}
