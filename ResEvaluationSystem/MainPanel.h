/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ABOUT                            1
#define  ABOUT_COMMANDBUTTON              2       /* control type: command, callback function: HideAboutPanel */
#define  ABOUT_TEXTMSG                    3       /* control type: textMsg, callback function: (none) */
#define  ABOUT_TEXTMSG_2                  4       /* control type: textMsg, callback function: (none) */

#define  MAIN_PANEL                       2
#define  MAIN_PANEL_DECORATION            2       /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_START                 3       /* control type: pictButton, callback function: test */
#define  MAIN_PANEL_PAUSE                 4       /* control type: pictButton, callback function: test */
#define  MAIN_PANEL_CONTINUE              5       /* control type: pictButton, callback function: test */
#define  MAIN_PANEL_STOP                  6       /* control type: pictButton, callback function: test */
#define  MAIN_PANEL_MOVEUP                7       /* control type: pictButton, callback function: editTable */
#define  MAIN_PANEL_MOVEDOWN              8       /* control type: pictButton, callback function: editTable */
#define  MAIN_PANEL_DELETE                9       /* control type: pictButton, callback function: editTable */
#define  MAIN_PANEL_ADD                   10      /* control type: pictButton, callback function: editTable */
#define  MAIN_PANEL_CLEAR                 11      /* control type: pictButton, callback function: editTable */
#define  MAIN_PANEL_SAVE                  12      /* control type: pictButton, callback function: fileIO */
#define  MAIN_PANEL_QUIT_EDITMODE         13      /* control type: pictButton, callback function: editTable */
#define  MAIN_PANEL_OPEN                  14      /* control type: pictButton, callback function: fileIO */
#define  MAIN_PANEL_TABLE                 15      /* control type: table, callback function: tableCB */
#define  MAIN_PANEL_COMMANDBUTTON         16      /* control type: command, callback function: quit */
#define  MAIN_PANEL_TEXTMSG_4             17      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG_3             18      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG_2             19      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG               20      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_DECORATION_2          21      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_6          22      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_5          23      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_4          24      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_3          25      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_TIMER                 26      /* control type: timer, callback function: GetDatetime */

#define  PANEL2                           3
#define  PANEL2_GPIB_ADDRESS0             2       /* control type: numeric, callback function: (none) */
#define  PANEL2_GPIB_ADDRESS1             3       /* control type: numeric, callback function: (none) */
#define  PANEL2_PICTUREBUTTON             4       /* control type: pictButton, callback function: SaveConfig */
#define  PANEL2_COMMANDBUTTON             5       /* control type: command, callback function: hideGpigAddressSetting */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_FILE                     2
#define  MENUBAR_FILE_OPEN                3       /* callback function: menuCB */
#define  MENUBAR_FILE_SAVE                4       /* callback function: menuCB */
#define  MENUBAR_FILE_EDIT                5       /* callback function: menuCB */
#define  MENUBAR_FILE_QUIT                6       /* callback function: menuCB */
#define  MENUBAR_TEST                     7
#define  MENUBAR_TEST_START               8       /* callback function: menuCB */
#define  MENUBAR_TEST_STOP                9       /* callback function: menuCB */
#define  MENUBAR_TEST_SINGLE_TEST         10      /* callback function: menuCB */
#define  MENUBAR_TEST_LOOP_TEST           11      /* callback function: menuCB */
#define  MENUBAR_SETTING                  12
#define  MENUBAR_SETTING_SET_GPIB_ADDRESS 13      /* callback function: menuCB */
#define  MENUBAR_HELP                     14
#define  MENUBAR_HELP_ABOUT               15      /* callback function: menuCB */


     /* Callback Prototypes: */

int  CVICALLBACK editTable(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK fileIO(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetDatetime(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK HideAboutPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK hideGpigAddressSetting(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK menuCB(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveConfig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK tableCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK test(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
