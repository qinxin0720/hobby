#ifndef MACRODEFINE_H_
#define MACRODEFINE_H_

extern int main_panelHandle;
extern int setting_panelHandle;
extern int about_panelHandle;


extern int edit_mode;
extern int testStatus;
extern int loopValue;
extern int gpib_address0;
extern int gpib_address1;


extern void SetNewRowAttribute (int RowNum);
extern void openConfigFile (void);
extern int SaveData (int row);
extern int CVICALLBACK quit (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2);
extern int CVICALLBACK test (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2);

#endif
