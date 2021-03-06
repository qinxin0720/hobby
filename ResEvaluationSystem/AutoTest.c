#include <Windows.h>
#include <ansi_c.h>
#include <utility.h>
#include <userint.h>
#include "MacroDefine.h"
#include "MainPanel.h"

int StartTestFunctionId = 0;
int TestThreadFunctionPause = 0;
int TestThreadFunctionStop = 0;

int testStatus = 0;
int loopValue = 1;
void ClearResult (void);

int reset (void)
{
	HMODULE loaddll;
	int returnValue;
	int (* fpFun)(int GPIBaddress0, int GPIBaddress1, int timeOut);
	loaddll = LoadLibrary("Keithley2750_driver.dll");
	if (loaddll)
	{
		fpFun = GetProcAddress(loaddll, "ResetEquipment");
		if (fpFun)
		{
			returnValue = (* fpFun) (gpib_address0, gpib_address1, 10);
		}
	}
	FreeLibrary (loaddll);
	return returnValue;
}

int set4wireresmode (void)
{
	HMODULE loaddll;
	int returnValue;
	int (* fpFun)(int GPIBaddress0, int GPIBaddress1, int timeOut);
	loaddll = LoadLibrary("Keithley2750_driver.dll");
	if (loaddll)
	{
		fpFun = GetProcAddress(loaddll, "Set4WireResMode");
		if (fpFun)
		{
			returnValue = (* fpFun) (gpib_address0, gpib_address1, 10);
		}
	}
	FreeLibrary (loaddll);
	return returnValue;
}

int closechannel (int ChannelNum)
{
	HMODULE loaddll;
	int returnValue;
	int (* fpFun)(int GPIBaddress0, int GPIBaddress1, int channelNum, int timeOut);
	loaddll = LoadLibrary("Keithley2750_driver.dll");
	if (loaddll)
	{
		fpFun = GetProcAddress(loaddll, "CloseChannel");
		if (fpFun)
		{
			returnValue = (* fpFun) (gpib_address0, gpib_address1, ChannelNum, 10);
		}
	}
	FreeLibrary (loaddll);
	return returnValue;
}

double fetch (void)
{
	HMODULE loaddll;
	double value;
	char Buffer[101];
	int (* fpFun)(char * receiveBuffer, int GPIBaddress0, int GPIBaddress1, int timeOut);
	loaddll = LoadLibrary("Keithley2750_driver.dll");
	if (loaddll)
	{
		fpFun = GetProcAddress(loaddll, "DataFetch");
		if (fpFun)
		{
			(* fpFun) (Buffer, gpib_address0, gpib_address1, 10);
			value = atof (Buffer);
		}
	}
	FreeLibrary (loaddll);
	return value;
}

int CVICALLBACK StartTestThreadFunction (void *functionData)
{
	int numberOfRows = -1;
	char ChannelNum[4] = {0};
	int channelNum;
	double resValue;
	double lowlimit;
	char lowlimitUnit[8] = {0};
	int lowlimitIndex;
	
	double uplimit;
	char uplimitUnit[8] = {0};
	int uplimitIndex;
	
	char str[8] = {0};
	
	GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows); 
	if (numberOfRows > 0)
	{
		if (reset () > 0)
		{
			Delay (5);
			if (set4wireresmode () > 0)
			{
				Delay (0.1);
				for (int j=1; j<=loopValue; j++)
				{
					ClearResult ();
					sprintf (str, "%d/%d", j, loopValue);
					SetCtrlVal (main_panelHandle, MAIN_PANEL_TEXTMSG_2, str);
					for (int i=1; i<=numberOfRows; i++)
					{
						while (TestThreadFunctionPause)	  //pause
						{
							;
						}
						if (TestThreadFunctionStop)		  //stop
						{
							SetCtrlAttribute (main_panelHandle, MAIN_PANEL_START, ATTR_DIMMED, 0);
							SetCtrlAttribute (main_panelHandle, MAIN_PANEL_STOP, ATTR_DIMMED, 1);
							SetCtrlAttribute (main_panelHandle, MAIN_PANEL_CONTINUE, ATTR_DIMMED, 1);
							return 2;
						}
						SetTableSelection (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (i, 1, 1, 13));
						GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (3, i), ChannelNum);
						channelNum = atoi (ChannelNum);
						if (channelNum == 0)
						{
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (13, i), "ILLEGAL");
							continue;
						}
						if (closechannel (channelNum) > 0)
						{
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, i), "Close");
							Delay (0.1); 
							resValue = fetch ();
							Delay (0.1);
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (8, i), uplimitUnit);
							GetTableCellRingIndexFromValue (main_panelHandle, MAIN_PANEL_TABLE, 0, MakePoint (8, i), &uplimitIndex, uplimitUnit);
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (7, i), &uplimit);
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (10, i), lowlimitUnit);
							GetTableCellRingIndexFromValue (main_panelHandle, MAIN_PANEL_TABLE, 0, MakePoint (10, i), &lowlimitIndex, lowlimitUnit);
							GetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (9, i), &lowlimit);
							if ((resValue <= (uplimit * pow (10, (uplimitIndex-1)*3))) && (resValue >= (lowlimit * pow (10, (lowlimitIndex-1)*3))))
							{
								SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (13, i), ATTR_TEXT_BGCOLOR, VAL_GREEN);
								SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (13, i), "PASS"); 
							}
							else
							{
								SetTableCellAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (13, i), ATTR_TEXT_BGCOLOR, VAL_RED); 
								SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (13, i), "FAIL");
							}
							if (resValue < 1)
							{
								resValue = resValue * 1000;
								SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (11, i), resValue); 
								SetTableCellValFromIndex (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, i), 0);
							}
							else if (resValue < 1000)
							{
								SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (11, i), resValue); 
								SetTableCellValFromIndex (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, i), 1);
							}
							else if (resValue < 1000000)
							{
								resValue = resValue / 1000;
								SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (11, i), resValue); 
								SetTableCellValFromIndex (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, i), 2);
							}
							else
							{
								resValue = resValue / 1000000;
								SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (11, i), resValue); 
								SetTableCellValFromIndex (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (12, i), 3);
							}
							SetTableCellVal (main_panelHandle, MAIN_PANEL_TABLE, MakePoint (4, i), "Open");
							SaveData (i);
						}
						else
						{
							MessagePopup ("Warning", "Instrument Close Channel Failed!");
							continue;
						}
					}
				}
			}
			else
			{
				MessagePopup ("Warning", "Instrument Set 4 Wire Resistance Mode Failed!");
			}
		}
		else
		{
			MessagePopup ("Warning", "Instrument Reset Failed!");
		}
	}
	SetCtrlAttribute (main_panelHandle, MAIN_PANEL_START, ATTR_DIMMED, 0);
	SetCtrlAttribute (main_panelHandle, MAIN_PANEL_STOP, ATTR_DIMMED, 1);
	SetCtrlAttribute (main_panelHandle, MAIN_PANEL_CONTINUE, ATTR_DIMMED, 1);
	return 0;
}

int CVICALLBACK test (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			switch (control)
			{
				case MAIN_PANEL_START:
					TestThreadFunctionStop = 0; 
					TestThreadFunctionPause = 0;
					testStatus = 1;
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_START, ATTR_DIMMED, 1);
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_STOP, ATTR_DIMMED, 0); 
					CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, StartTestThreadFunction, NULL, &StartTestFunctionId);
					CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, StartTestFunctionId, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
					CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, StartTestFunctionId);
					testStatus = 0; 
					MessagePopup ("Message", "Test Task is Finished!");
					break;
				case MAIN_PANEL_PAUSE:
					TestThreadFunctionPause = 1;
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_PAUSE, ATTR_DIMMED, 1); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_CONTINUE, ATTR_DIMMED, 0);
					break;
				case MAIN_PANEL_CONTINUE:
					TestThreadFunctionPause = 0;
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_PAUSE, ATTR_DIMMED, 0); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_CONTINUE, ATTR_DIMMED, 1);
					break;
				case MAIN_PANEL_STOP:
					TestThreadFunctionStop = 1;
					TestThreadFunctionPause = 0;
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_STOP, ATTR_DIMMED, 1); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_PAUSE, ATTR_DIMMED, 0); 
					SetCtrlAttribute (main_panelHandle, MAIN_PANEL_CONTINUE, ATTR_DIMMED, 1);
					break;
			}
			break;
	}
	return 0;
}

void ClearResult (void)
{
	int numberOfRows = -1;
	double * data; 
	char * (* result); 
	GetNumTableRows (main_panelHandle, MAIN_PANEL_TABLE, &numberOfRows); 
	
	if ((data = (double *) malloc (numberOfRows * sizeof(double))) == NULL)
	{
		return;
	}
	if ((result = (char **) malloc (numberOfRows * sizeof(char *))) == NULL)
	{
		return;
	}
	memset (data, 0, sizeof(double)*numberOfRows);
	memset (result, 0, sizeof(char *)*numberOfRows);
	SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 11, numberOfRows, 1), data, VAL_COLUMN_MAJOR); 
	SetTableCellRangeVals (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 13, numberOfRows, 1), result, VAL_COLUMN_MAJOR); 
	SetTableCellRangeAttribute (main_panelHandle, MAIN_PANEL_TABLE, MakeRect (1, 13, numberOfRows, 1), ATTR_TEXT_BGCOLOR, MakeColor (255, 255, 255));
	free (data);
	free (result);
}



