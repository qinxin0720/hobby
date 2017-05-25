#include "cvi_db.h"
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include <formatio.h>
#include "MainPanel.h"
#include "MacroDefine.h"

int SaveData (int row)
{
	int hdbc, hstmt, resCode; 
	char point1[16];
	char point2[16];
	double uplimit;
	char uplimitUnit[4];
	double lowlimit;
	char lowlimitUnit[4];
	double data;
	char dataUnit[4];
	char result[16];
	
	char Uplimit[32];
	char Lowlimit[32];
	char Data[32];
	
	int	month, day, year;
	int	hours, minutes, seconds;
	char datetime[32];
	
	if ((hdbc = DBConnect ("DSN=RESDataSource")) > 0)
	{
		if ((hstmt = DBActivateSQL (hdbc, "SELECT * FROM TESTDATA")) > 0)
		{
	
			GetSystemDate (&month, &day, &year);
			GetSystemTime (&hours, &minutes, &seconds);
		    sprintf (datetime, "%d-%d-%d %02d:%02d:%02d", year, month, day, hours, minutes, seconds);
	
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (5, row), point1);
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (6, row), point2); 
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, row), &uplimit);
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, row), uplimitUnit);
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, row), &lowlimit);
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, row), lowlimitUnit);
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (11, row), &data);
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, row), dataUnit);
			GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (13, row), result);
	
			Fmt (Uplimit, "%s<%f%s", uplimit, uplimitUnit);
			Fmt (Lowlimit, "%s<%f%s", lowlimit, lowlimitUnit);
			Fmt (Data, "%s<%f%s", data, dataUnit);
	
	
			resCode = DBCreateRecord (hstmt); 
			resCode = DBPutColChar (hstmt, 1, datetime, ""); 
			resCode = DBPutColChar (hstmt, 2, point1, "");
			resCode = DBPutColChar (hstmt, 3, point2, ""); 
			resCode = DBPutColChar (hstmt, 4, Uplimit, ""); 
			resCode = DBPutColChar (hstmt, 5, Lowlimit, ""); 
			resCode = DBPutColChar (hstmt, 6, Data, ""); 
			resCode = DBPutColChar (hstmt, 7, result, ""); 
			resCode = DBPutRecord (hstmt); 
			DBDeactivateSQL (hstmt); 
		}
		else
		{
			MessagePopup ("Warning", "Failed to connect SQL Server!");
			return -1;
		}
		DBDisconnect (hdbc); 
	}
	else
	{
		MessagePopup ("Warning", "Failed to connect SQL Server!");
		return -1;
	}
	return 0;
}
