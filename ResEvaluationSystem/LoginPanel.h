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

#define  LOGI_PANEL                       1
#define  LOGI_PANEL_USERNAME              2       /* control type: string, callback function: (none) */
#define  LOGI_PANEL_PASSWORD              3       /* control type: string, callback function: (none) */
#define  LOGI_PANEL_CONFIRM               4       /* control type: pictButton, callback function: login */
#define  LOGI_PANEL_CANCEL                5       /* control type: pictButton, callback function: quit */
#define  LOGI_PANEL_TEXTMSG               6       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK login(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
