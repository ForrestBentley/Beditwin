/*************************************************************************/
/* Program          : BEDIT5PM.c                 v2.0                    */
/* Author           : Forrest Bentley                                    */
/* Date Written     : January 15, 1992                                   */
/*                                                                       */
/* Description      :                                                    */
/*     This program will process all window commands.                    */
/*                                                                       */
/*************************************************************************/
//*                        Maintenance Log
//*
//* Name            Date       Desc
//* --------------- --------   --------------------------------------------
//* Forrest Bentley 06/24/1994 Added a close to file_debug_out in WM_CLOSE
//* Forrest Bentley 02/15/2014 Fixed Banner bug of using last pVariable
//* Forrest Bentley 08/15/2014 Added WM_Paint using PaintFile
//* Forrest Bentley 08/22/2014 Added Shaddow to ball paint
//* Forrest Bentley 08/25/2014 Box3D,Ellipse3D, Box, Ellipse
//* Forrest Bentley 08/27/2014 Added Circle, Rectangle
//* Forrest Bentley 08/29/2014 Added TextTilt
//* Forrest Bentley 09/02/2014 Improved 3D Ball and Box
//* Forrest Bentley 09/10/2014 Paint on MainWindow
//* Forrest Bentley 09/29/2014 Added Bitmap processing
//* Forrest Bentley 10/01/2014 Fixed memory leak for Bitmaps (added DeleteDC)
//* Forrest Bentley 10/06/2014 Added Object names to PaintFile
//* Forrest Bentley 11/25/2014 Changed Arrow Box to Proper Arrow Points
//* Forrest Bentley 11/26/2014 Filled Arrow
//* Forrest Bentley 11/27/2014 Added Arrow 2,3,4
//* Forrest Bentley 12/03/2014 Added Roundbox,Triangle
//* Forrest Bentley 12/07/2014 Reorganized Triangle letters, added left/right triangles
//* Forrest Bentley 12/08/2014 Added Large Arrows
//* Forrest Bentley 12/12/2014 Removed hilights
//*                            Added Large bent arrow up->right
//* Forrest Bentley 12/15/2014 Added Large bent arrow up->left
//* Forrest Bentley 12/16/2014 Finished all Bent Arrows (Hollow also)
//*
/*************************************************************************/

#include <windows.h>
#include <windowsx.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <MALLOC.h>
#include <conio.h>
#include <io.h>
#include <errno.h>
#include <direct.h>
#include <signal.h>
#include <math.h>

#include "BEDIT.h"
#include "BEDIT4pm.h"
#include "BEDITvar.h"

#define WM_WINDOWINIT  WM_USER+777
#define WM_FOCUSSET    WM_USER+778
#define WM_FOCUSCHG    WM_USER+779
#define WM_PUSHBUTTON  WM_USER+780
#define WM_ENABLEENTRY WM_USER+781
#define WM_MASKENTRY   WM_USER+782

#define PI 3.14159265

/**********************************************************************/
/* BeditwinMainWindProc                                               */
/*    This module will -                                              */
/*                                                                    */
/**********************************************************************/
long FAR PASCAL BeditwinMainWindProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {

	case WM_CREATE:
           WindowCount = 0;
           /********************************/
           /* Create the WindowTimer Object*/
           /********************************/
           pObject = &StartObject;
           do  {
               if  (pObject->pNext) {
                   pObject = pObject->pNext;
               }
           }
           while (pObject->pNext);
       
           pNewObject = (OBJECT *) malloc (sizeof (OBJECT));
           if  (!pNewObject) {
               strcpy (Error, "Unable to create Obj");
               z110_fatal ();
               return 0;
           }
           memset (pNewObject, 0, sizeof (OBJECT));
           strcpy (pNewObject->szName, "WindowTimer");
           strcpy (pNewObject->szType, "WindowTimer");
           strcpy (pNewObject->szWindow, "MAIN");
           pNewObject->hwnd = hwnd;
           pNewObject->id = 7;
       
           pObject->pNext = pNewObject;
           pNewObject->pPrev = pObject;
           pNewObject->pNext = NULL;
           pNewObject->pNextWin = NULL;
           pNewObject->pNextWinObj = NULL;
//           pNewObject->lFont = 0;
//           pNewObject->lFontSize = 0;
           pNewObject->szDraw = (char *) malloc (5);
           if  (!pNewObject->szDraw) {
               strcpy (Error, "Bad Obj Cr Draw");
               z110_fatal ();
               return 0;
           }
           strcpy (pNewObject->szDraw, "");
           pObject = pNewObject;
           test = 'y';
           strcpy (Command, pObject->szName);
           d580_find_variable ();
           test = 'n';
           if  (found != 'y') {
               //z999_open_bug ();
               //sprintf (buff, "Creating Window Object string [%s]!\n", Command);
               //fputs (buff, file_debug_out);
               strcpy (input_line, Command);
               index1 = 0;
               More_Commands = 'y';
               d510_string ();
           }

           /********************************/
           /* Create the WindowInit Object */
           /********************************/
           pObject = &StartObject;
           do  {
               if  (pObject->pNext) {
                   pObject = pObject->pNext;
               }
           }
           while (pObject->pNext);
       
           pNewObject = (OBJECT *) malloc (sizeof (OBJECT));
           if  (!pNewObject) {
               strcpy (Error, "Unable to create Obj");
               z110_fatal ();
               return 0;
           }
           memset (pNewObject, 0, sizeof (OBJECT));
           strcpy (pNewObject->szName, "WindowInit");
           strcpy (pNewObject->szType, "WindowInit");
           strcpy (pNewObject->szWindow, "MAIN");
           pNewObject->hwnd = hwnd;
           pNewObject->id = 7;
       
           pObject->pNext = pNewObject;
           pNewObject->pPrev = pObject;
           pNewObject->pNext = NULL;
           pNewObject->pNextWin = NULL;
           pNewObject->pNextWinObj = NULL;
//           pNewObject->lFont = 0;
//           pNewObject->lFontSize = 0;
           pNewObject->szDraw = (char *) malloc (5);
           if  (!pNewObject->szDraw) {
               strcpy (Error, "Bad Obj Cr Draw");
               z110_fatal ();
               return 0;
           }
           strcpy (pNewObject->szDraw, "");
           pObject = pNewObject;
           test = 'y';
           strcpy (Command, pObject->szName);
           d580_find_variable ();
           test = 'n';
           if  (found == 'y') {
               PostMessage (hwnd, WM_WINDOWINIT, 0, 0);
           }
           else {
               //z999_open_bug ();
               //sprintf (buff, "Creating Window Object string [%s]!\n", Command);
               //fputs (buff, file_debug_out);
               strcpy (input_line, Command);
               index1 = 0;
               More_Commands = 'y';
               d510_string ();
           }
	   break;

        case WM_WINDOWINIT:
            strcpy (Command, "WindowInit");
 
            w580_find_object ();
            d580_find_variable ();
            w200_proc_cmd ();
            return (0L);

//	case WM_SETCURSOR:
//	    if  (cUpdateCursor == 'n') {
//		return 0;
//	    }
//	    break;

        case WM_COMMAND:
            pObject = &StartObject;
            do  {
                if  (pObject->pNext) {
                    pObject = pObject->pNext;
		    if  (pObject->id == (USHORT)wParam) {
                    //if (pObject->hwnd == (HWND)lParam) {
                        strcpy (Command, "");
                        w200_proc_cmd ();
                        return 0;
                    }
                }
            }
            while (pObject->pNext);
	    MessageBox (NULL,
                        "There isn't an Action associated with this Object!",
                        "No Action Error",
                        MB_ICONHAND | MB_OK);
            return 0L;

        case WM_PAINT:
            PaintObjects(hwnd, message);
            return 0;

        case WM_TIMER:
            strcpy (Command, "WindowTimer");
            test = 'y';
            d580_find_variable ();
            test = 'n';
            if  ((found == 'y') &&
                 (strcmp (pVariable->szString, "") != 0)) {
                w580_find_object ();
                KillTimer(BEDITframe, 1);
                w200_proc_cmd ();
            }
            else {
                KillTimer(BEDITframe, 1);
                strcpy (Error, "Set up WindowTimer Variable before turning on the Timer!");
                z110_fatal ();
            }
            break;
                                                                          
        case WM_CLOSE:
           KillTimer(BEDITframe, 1);
           sInterval = 0;
           /********************************/
           /* Create the WRAPUP            */
           /********************************/
           pObject = &StartObject;
           do  {
               if  (pObject->pNext) {
                   pObject = pObject->pNext;
               }
           }
           while (pObject->pNext);
       
           pNewObject = (OBJECT *) malloc (sizeof (OBJECT));
           if  (!pNewObject) {
               strcpy (Error, "Unable to create Obj");
               z110_fatal ();
               return 0;
           }
           memset (pNewObject, 0, sizeof (OBJECT));
           strcpy (pNewObject->szName, "WindowWrap");
           strcpy (pNewObject->szType, "WindowWrap");
           strcpy (pNewObject->szWindow, "MAIN");
           pNewObject->hwnd = hwnd;
           pNewObject->id = 8;
       
           pObject->pNext = pNewObject;
           pNewObject->pPrev = pObject;
           pNewObject->pNext = NULL;
           pNewObject->pNextWin = NULL;
           pNewObject->pNextWinObj = NULL;
//           pNewObject->lFont = 0;
//           pNewObject->lFontSize = 0;
           pNewObject->szDraw = (char *) malloc (5);
           if  (!pNewObject->szDraw) {
               strcpy (Error, "Bad Obj Cr Draw");
               z110_fatal ();
               return 0;
           }
           strcpy (pNewObject->szDraw, "");
           pObject = pNewObject;
           test = 'y';
           strcpy (Command, pObject->szName);
           d580_find_variable ();
           test = 'n';
           if  (file_debug_out) {
               fclose (file_debug_out);
           }
           if  (found == 'y') {
               w200_proc_cmd ();
	   }
           if  (file_debug_out) {
               fclose (file_debug_out);
           }

           remove ("BEDITexec.bat");
           remove ("BEDITexec.sem");
           //************************************
           //* B-Edit Debugging
           //************************************
           if  (cBeditDebug == 'y') {    
                //*******************************
                //* Write the Beditsem.stp file
                //* Indicating the program is done
                //*******************************
                FILE *BeditBugFile;
                char szBugOutput[100];

                BeditBugFile = fopen ("beditsem.stp", "w");
                fputs ("STOP\n", BeditBugFile);
                fclose (BeditBugFile);  

           }
           PostQuitMessage (0);
	       break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

/**********************************************************************/
/* BEDITSecondaryWindProc                                              */
/*    This module will -                                              */
/*                                                                    */
/**********************************************************************/
long FAR PASCAL BeditwinSecondaryWindProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{


    OBJECT *pListObject;
    OBJECT *pWindowObject;
    char   szWindow [100];
    long ulStyle;

    HDC hDC;
    PAINTSTRUCT PtStr;

    HPEN hBlackPen;
    HPEN hRedPen;
    HBRUSH hBrush;
    HBRUSH hShaddow;

    OPENFILE *pPaintFile;
    DATAFILE *pPaintData;

    switch (message) {

        case WM_PAINT:
            PaintObjects(hwnd, message);
            break;

        case WM_CREATE:
     	   break;

        case WM_MENUCHAR:
            return 1L;

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        case WM_CHAR:
            {
                OBJECT *pActiveObject = &StartObject;

                //MessageBox (HWND_DESKTOP, "Key Pressed", "Debugit", MB_OK);

                do  {
                    if  (pActiveObject->pNext) {
                        pActiveObject = pActiveObject->pNext;
                        if  (pActiveObject->hwnd == hwnd) {
                            do  {
                                if  (pActiveObject->pNext) {
                                    pActiveObject = pActiveObject->pNext;
                                    if  (pActiveObject->flStyle & WS_TABSTOP) {
                                        SetFocus (pActiveObject->hwnd);
                                        SendMessage (pActiveObject->hwnd, message, wParam, lParam);
                                        break;
                                    }
                                }
                            }
                            while (memcmp (pActiveObject->szType, "WINDOW", 6) != 0);
                        }
                    }
                }
                while (pActiveObject->pNext);
                break;
            }

        case WM_FOCUSSET:
            {
                OBJECT *pParentObject;

                pObject = &StartObject;
                do  {
                    if  (pObject->pNext) {
                        pObject = pObject->pNext;
                        if  (memcmp (pObject->szType, "WINDOW", 6) == 0) {
                            pParentObject = pObject;
                        }
                    }
                }
                while ((pObject->pNext) &&
                       (pObject->id != (USHORT)wParam));
		if  (pObject->id != (USHORT)wParam) {
                    break;
                }
                if  (memcmp (pObject->szType, "COOBJ", 5) == 0) {
                    HWND hwndFocus;
                    hwndFocus = GetWindow(pObject->hwnd, GW_HWNDNEXT);
           	    SetFocus (GetWindow(hwndFocus, GW_HWNDPREV));
                }
                else {
                    SetFocus (pObject->hwnd);
                }
                if (pParentObject) {
                    SetActiveWindow (pParentObject->hwnd);
                }
                break;
            }

        case WM_ENABLEENTRY:
            {
                OBJECT *pEnaObject;
                pEnaObject = &StartObject;
                do  {
                    if  (pEnaObject->pNext) {
                        pEnaObject = pEnaObject->pNext;
                    }
                }
                while ((pEnaObject->pNext) &&
                       (pEnaObject->id != (USHORT)wParam));
                if  (pEnaObject->id != (USHORT)wParam) {
                    break;
                }
                ulStyle = GetWindowLong (pEnaObject->hwnd, GWL_STYLE);
                ulStyle |= WS_TABSTOP;
                ulStyle |= WS_BORDER;
                SetWindowPos (pEnaObject->hwnd, NULL, pEnaObject->x,
                                 pEnaObject->y, pEnaObject->cx, pEnaObject->cy, SWP_NOZORDER);
                if  (memcmp (pEnaObject->szType, "MLOBJ", 5) == 0) {
//                    WinSendMsg (pEnaObject->hwnd, MLM_SETREADONLY, MPFROMSHORT(FALSE), 0L);
                }
                SetWindowLong (pEnaObject->hwnd, GWL_STYLE, ulStyle);
		pEnaObject->flStyle = ulStyle;
                InvalidateRect (pEnaObject->hwnd, NULL, TRUE);
		return 0L;
	    }

//	case WM_SETCURSOR:
//	    if  (cUpdateCursor == 'n') {
//		return 0;
//	    }
//	    break;

	case WM_COMMAND:
	   pObject = &StartObject;
	   do  {
	       if  (pObject->pNext) {
	           pObject = pObject->pNext;
		   if  (pObject->hwnd == (HWND)lParam) {
		       if  ((memcmp (pObject->szType, "EFOBJ", 5) == 0) ||
                            (memcmp (pObject->szType, "MLOBJ", 5) == 0) ||
                            (memcmp (pObject->szType, "COOBJ", 5) == 0)) {
                           return DefWindowProc(hwnd, message, wParam, lParam);
                       }
                       if  ((memcmp (pObject->szType, "RBOBJ", 5) == 0) ||
                            (memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
                            (memcmp (pObject->szType, "LBOBJ", 5) == 0)) {
                           strcpy (Command, pObject->szName);
                           d580_find_variable ();
                           if  (memcmp (pObject->szType, "LBOBJ", 5) == 0) {
                               if  (strcmp (pVariable->szEdit, "SELECT") == 0) {
                                   w200_proc_cmd ();
                               }
                               return DefWindowProc(hwnd, message, wParam, lParam);
                           }
                           if  (strcmp (pVariable->szEdit, "ACTION") == 0) {
                               PostMessage (hwnd, WM_PUSHBUTTON, wParam, lParam);
                               return DefWindowProc(hwnd, message, wParam, lParam);
                           }
		       }
		       else {
                           PostMessage (hwnd, WM_PUSHBUTTON, wParam, lParam);
                           return DefWindowProc(hwnd, message, wParam, lParam);
                       }
		   }
	       }
	   }
	   while (pObject->pNext);
	   break;

	case WM_PUSHBUTTON:
           if  (!(HWND)lParam) {
               return DefWindowProc(hwnd, message, wParam, lParam);
           }
	   pObject = &StartObject;
	   do  {
	       if  (pObject->pNext) {
                   pObject = pObject->pNext;
                   if  (pObject->hwnd == (HWND)lParam) {
                       w200_proc_cmd ();
                       return DefWindowProc(hwnd, message, wParam, lParam);
                   }
               }
	   }
	   while (pObject->pNext);
           MessageBox (HWND_DESKTOP, "Unknown Button", "Error", MB_OK);
	   MessageBeep (MB_ICONHAND);
           return DefWindowProc(hwnd, message, wParam, lParam);

    case WM_FOCUSCHG:
        pObject = &StartObject;
        do  {
            if  (pObject->pNext) {
                pObject = pObject->pNext;
            }
        }
        while ((pObject->pNext) &&
               (pObject->id != (USHORT)wParam));
        if  (pObject->id != (USHORT)wParam) {
            break;
        }
        strcpy (Command, pObject->szName);
        d580_find_variable ();
//        if  (memcmp (pObject->szType, "COOBJ", 5) == 0) {
//            z999_open_bug ();
//            sprintf (buff, "Focus Change [%s]\n", pObject->szName);
//            fputs (buff, file_debug_out);
//            if  (strcmp (pVariable->szAction, "") != 0) {
//                strcpy (Command, pVariable->szName);
//                w200_proc_cmd ();
//            }
//        }
        if  (memcmp (pObject->szType, "EFOBJ", 5) == 0) {
            /*******************/
            /* MIX             */
            /*******************/
            if  (strcmp (pVariable->szEdit, "MIX") == 0) {
                GetWindowText (pObject->hwnd, buff, Max);
                free (pVariable->szString);
                pVariable->szString = (char *) malloc (strlen (buff) + 5);
                if  (!pVariable->szString) {
		    sprintf (buff, "Unable to store MIX text for [%s]", pObject->szName);
                    z110_fatal ();
                    break;
                }
                strcpy (pVariable->szString, buff);
                if  (pVariable->szString [0] != 0) {
                    for (index3 = 0; index3 < (int) strlen (pVariable->szString); index3++) {
                        if  (pVariable->szString [index3] > 32) {
                            if  (((index3 == 0) && (strlen (pVariable->szString) > 1)) ||
                                 ((index3 > 0) && (pVariable->szString [index3 - 1] == 32))) {
                                pVariable->szString [index3] = (char) toupper (pVariable->szString [index3]);
                            }
                            else {
                                pVariable->szString [index3] = (char) tolower (pVariable->szString [index3]);
                            }
                        }
                    }
                    SetWindowText (pObject->hwnd, pVariable->szString);
                }
            }
            /*******************/
            /* DATE            */
            /*******************/
            if  ((strcmp (pVariable->szEdit, "DATE") == 0) ||
                 (strcmp (pVariable->szEdit, "CURRDATE") == 0)) {
                GetWindowText (pObject->hwnd, buff, Max);
                free (pVariable->szString);
                pVariable->szString = (char *) malloc (15);
                if  (!pVariable->szString) {
                    sprintf (buff, "Unable to store DATE text for [%s]", pObject->szName);
                    z110_fatal ();
                    break;
                }
                w900_date ();
                SetWindowText (pObject->hwnd, pVariable->szString);
            }
            /*******************/
            /* DOLLAR          */
            /*******************/
            if  (strcmp (pVariable->szEdit, "DOLLAR") == 0) {
                GetWindowText (pObject->hwnd, buff, Max);
                free (pVariable->szString);
                pVariable->szString = (char *) malloc (30);
                if  (!pVariable->szString) {
                    sprintf (buff, "Unable to store DOLLAR text for [%s]", pObject->szName);
                    z110_fatal ();
                    break;
                }
                w910_dollar ();
                SetWindowText (pObject->hwnd, pVariable->szString);
            }
            /*******************/
            /* TELEPHONE       */
            /*******************/
            if  (strcmp (pVariable->szEdit, "TELEPHONE") == 0) {
                GetWindowText (pObject->hwnd, buff, Max);
                free (pVariable->szString);
                pVariable->szString = (char *) malloc (20);
                if  (!pVariable->szString) {
                    sprintf (buff, "Unable to store TELEPHONE text for [%s]", pObject->szName);
                    z110_fatal ();
                    break;
                }
                w920_telephone ();
                SetWindowText (pObject->hwnd, pVariable->szString);
            }
            /*******************/
            /* ZERO            */
            /*******************/
            if  (strcmp (pVariable->szEdit, "ZERO") == 0) {
                GetWindowText (pObject->hwnd, buff, Max);
                free (pVariable->szString);
                pVariable->szString = (char *) malloc (pVariable->sSize+5);
                memset (temp, '0', pVariable->sSize - strlen (buff));
                temp [pVariable->sSize - strlen (buff)] = 0;
                strcat (temp, buff);
                if  (!pVariable->szString) {
                    sprintf (buff, "Unable to store ZERO text for [%s]", pObject->szName);
                    z110_fatal ();
                    break;
                }
                strcpy (pVariable->szString, temp);
                SetWindowText (pObject->hwnd, temp);
            }
            /*******************/
            /* Action          */
            /*******************/
            if  (strcmp (pVariable->szAction, "") != 0) {
                w200_proc_cmd ();
                break;
            }
        }
        break;

        case WM_MASKENTRY:
            {
                OBJECT *pMaskObject;
                int    Mask1;
                //HWND   hwndParent;

                pMaskObject = &StartObject;
                do  {
                    if  (pMaskObject->pNext) {
                        pMaskObject = pMaskObject->pNext;
                        if  (strcmp (pMaskObject->szType, "WINDOW") == 0) {
                            //hwndParent = pMaskObject->hwndClient;
                        }
                    }
                }
                while ((pMaskObject->pNext) &&
                       (pMaskObject->id != (unsigned short) wParam));
                if  (pMaskObject->id != (unsigned short) wParam) {
                    return 0;
                }
                GetWindowText (pMaskObject->hwnd, buff, Max);
                if  (strlen (buff) > strlen (pMaskObject->szDraw)) {
                    //MessageBeep (MB_ICONHAND);fgbfgb
                    if  (strlen (buff) > 0) {
                        buff [strlen (buff) - 1] = 0;
                    }
                    SetWindowText (pMaskObject->hwnd, buff);
                    if  (cAutotab == 'y') {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                    }
                    return 0;
                }
                Mask1 = 0;
                do  {
                    if  (buff [Mask1] != 0) {
                        if  (pMaskObject->szDraw [Mask1] == Pattany) {
                            buff [Mask1] = pMaskObject->szDraw [Mask1];
                        }
                        if  (pMaskObject->szDraw [Mask1] == Pattnum) {
                            if  (isdigit (buff [Mask1])) {
                                buff [Mask1] = pMaskObject->szDraw [Mask1];
                            }
                            else {
                                buff [Mask1] = 255;
                            }
                        }
                        if  (pMaskObject->szDraw [Mask1] == Pattalp) {
                            if  ((isalpha (buff [Mask1])) ||
                                 (buff [Mask1] == ' ')) {
                                buff [Mask1] = pMaskObject->szDraw [Mask1];
                            }
                            else {
                                buff [Mask1] = 255;
                            }
                        }
                        if  (pMaskObject->szDraw [Mask1] == Pattspc) {
                            if  ((!isdigit (buff [Mask1])) &&
                                 (!isalpha (buff [Mask1]))) {
                                buff [Mask1] = pMaskObject->szDraw [Mask1];
                            }
                            else {
                                buff [Mask1] = 255;
                            }
                        }
                        if  (pMaskObject->szDraw [Mask1] != buff [Mask1]) {
                            //MessageBeep (MB_ICONHAND);
                            if  (Mask1 == (int) (strlen (buff) - 1)) {
                                GetWindowText (pMaskObject->hwnd, buff, Max);
                                buff [Mask1] = 0;
                                SetWindowText (pMaskObject->hwnd, buff);
                            }
                            return 0;
                        }
                        Mask1++;
                    }
                }
                while (buff [Mask1] != 0);
                if  (strlen (buff) == strlen (pMaskObject->szDraw)) {
                    if  (cAutotab == 'y') {
                        PostMessage (pMaskObject->hwnd, WM_CHAR, (WORD)'\t', 0L);
                    }
                }
                return 0;
            }

        case WM_MOUSEMOVE:
            if  ((strcmp (szPointer, "POINTER") == 0) ||
                 (strcmp (szPointer, "pointer") == 0) ||
                 (strcmp (szPointer, "ARROW") == 0) ||
                 (strcmp (szPointer, "arrow") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_ARROW));
            }
            else
            if  ((strcmp (szPointer, "WAIT") == 0) ||
                 (strcmp (szPointer, "wait") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_WAIT));
            }
            else
            if  ((strcmp (szPointer, "APPICON") == 0) ||
                 (strcmp (szPointer, "appicon") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_ICON));
            }
            else
            if  ((strcmp (szPointer, "ICONERROR") == 0) ||
                 (strcmp (szPointer, "iconerror") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_ICON));
            }
            else
            if  ((strcmp (szPointer, "ICONINFORMATION") == 0) ||
                 (strcmp (szPointer, "iconinformation") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_ICON));
            }
            else
            if  ((strcmp (szPointer, "ICONQUESTION") == 0) ||
                 (strcmp (szPointer, "iconquestion") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_ICON));
            }
            else
            if  ((strcmp (szPointer, "ICONWARNING") == 0) ||
                 (strcmp (szPointer, "iconwarning") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_ICON));
            }
            else
            if  ((strcmp (szPointer, "MOVE") == 0) ||
                 (strcmp (szPointer, "move") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_SIZE));
            }
            else
            if  ((strcmp (szPointer, "SIZENESW") == 0) ||
                 (strcmp (szPointer, "sizenesw") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_SIZENESW));
            }
            else
            if  ((strcmp (szPointer, "SIZENS") == 0) ||
                 (strcmp (szPointer, "sizens") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_SIZENS));
            }
            else
            if  ((strcmp (szPointer, "SIZENWSE") == 0) ||
                 (strcmp (szPointer, "sizenwse") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_SIZENWSE));
            }
            else
            if  ((strcmp (szPointer, "SIZEWE") == 0) ||
                 (strcmp (szPointer, "sizewe") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_SIZEWE));
            }
            else
            if  ((strcmp (szPointer, "TEXT") == 0) ||
                 (strcmp (szPointer, "text") == 0)) {
                SetCursor (LoadCursor (NULL, IDC_CROSS));
            }
            if  (cMouse == 'n') {
                break;
            }
            /***********************************************/
            /* Mouse Move Action                           */
            /***********************************************/
            {
                VARIABLE *pKeepVariable;
                POINT point;
                RECT  rect;
                int   WY;

                //point = MAKEPOINT (lParam);
                //XMouse = (int) point.x;
                //YMouse = (int) point.y;
                XMouse = (int) GET_X_LPARAM(lParam);
                YMouse = (int) GET_Y_LPARAM(lParam);

                //**************************
                //* Convert to OS/2 coord
                //**************************
                //GetWindowRect (hwnd, &rect);
                //WY = rect.bottom - rect.top - 23;
                //YMouse = WY - YMouse - 5;

                strcpy (szIf2, Command);
                strcpy (Command, "WindowMouse");
                pKeepVariable = pVariable;
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    cMouse = 'n';
                    w200_proc_cmd ();
                }
                pVariable = pKeepVariable;
                strcpy (Command, szIf2);
                break;
            }

        case WM_LBUTTONDOWN:
            {
                VARIABLE *pKeepVariable;
                /***********************************************/
                /* Button 1 Down Action                        */
                /***********************************************/
                strcpy (szIf2, Command);
                strcpy (Command, "WindowDown1");
                pKeepVariable = pVariable;
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    w200_proc_cmd ();
                }
                pVariable = pKeepVariable;
                strcpy (Command, szIf2);
                //break;
            }

        case WM_MBUTTONDOWN:
            {
                VARIABLE *pKeepVariable;
                /***********************************************/
                /* Button 2 Down Action                        */
                /***********************************************/
                strcpy (szIf2, Command);
                strcpy (Command, "WindowDown2");
                pKeepVariable = pVariable;
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    w200_proc_cmd ();
                }
                pVariable = pKeepVariable;
                strcpy (Command, szIf2);
                break;
            }

        case WM_RBUTTONDOWN:
            {
                VARIABLE *pKeepVariable;
                /***********************************************/
                /* Button 3 Down Action                        */
                /***********************************************/
                strcpy (szIf2, Command);
                strcpy (Command, "WindowDown3");
                pKeepVariable = pVariable;
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    w200_proc_cmd ();
                }
                pVariable = pKeepVariable;
                strcpy (Command, szIf2);
                break;
            }

        case WM_LBUTTONUP:
            {
                VARIABLE *pKeepVariable;
                /***********************************************/
                /* Button 1 Up Action                          */
                /***********************************************/
                strcpy (szIf2, Command);
                strcpy (Command, "WindowUp1");
                pKeepVariable = pVariable;
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    w200_proc_cmd ();
                }
                pVariable = pKeepVariable;
                strcpy (Command, szIf2);
                break;
            }

        case WM_MBUTTONUP:
            {
                VARIABLE *pKeepVariable;
                /***********************************************/
                /* Button 2 Up Action                          */
                /***********************************************/
                strcpy (szIf2, Command);
                strcpy (Command, "WindowUp2");
                pKeepVariable = pVariable;
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    w200_proc_cmd ();
                }
                pVariable = pKeepVariable;
                strcpy (Command, szIf2);
                break;
            }

        case WM_RBUTTONUP:
            {
                VARIABLE *pKeepVariable;
                /***********************************************/
                /* Button 3 Up Action                          */
                /***********************************************/
                strcpy (szIf2, Command);
                strcpy (Command, "WindowUp3");
                pKeepVariable = pVariable;
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    w200_proc_cmd ();
                }
                pVariable = pKeepVariable;
                strcpy (Command, szIf2);
                break;
            }

                                                                          
    case WM_CLOSE:
        if  (WindowCount > 0) {
            WindowCount--;
        }
        if  (WindowCount < 1) {
            SetActiveWindow (BEDITframe);
        }
        ShowWindow (hwnd, SW_HIDE);

        //return 0;

        /*********************************************/
        /* Locate the window object to null the hwnd */
        /*********************************************/
        pObject = &StartObject;
        do  {
            if  (pObject->pNext) {
                pObject = pObject->pNext;
            }
        }
        while ((pObject->pNext) && (pObject->hwnd != hwnd));
        if  (pObject->hwnd != hwnd) {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        if  (memcmp (pObject->szType, "WINDOW", 6) != 0) {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        pWindowObject = pObject;
        pListObject = pObject;
        /*****************************************************/
        /* Retrieve the window's data                        */
        /*****************************************************/
//        strcpy (Command, pObject->szName);
//        w510_obtain ();

        strcpy (szWindow, pListObject->szWindow);
        do  {
            if  (pListObject->pNext) {
                pListObject = pListObject->pNext;
                /*****************************************************/
                /* Null the object's hwnd                            */
                /*****************************************************/
                if  (strcmp (szWindow, pListObject->szWindow) == 0) {
                    pListObject->hwnd = NULL;
                }
            }
        }
        while ((pListObject->pNext) &&
               (strcmp (szWindow, pListObject->szWindow) == 0));

        pWindowObject->hwnd = NULL;
        pWindowObject->hwndClient = NULL;
        remove ("BEDITexec.bat");
        remove ("BEDITexec.sem");
	return DefWindowProc(hwnd, message, wParam, lParam);

    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

//**********************************************************
//* Paint the objects in the PaintFile on the screens
//**********************************************************
void PaintObjects(HWND hwnd,UINT message) {
    OBJECT *pListObject;
    OBJECT *pWindowObject;
    char   szWindow [100];
    long ulStyle;

    HDC hDC;
    PAINTSTRUCT PtStr;

    static  LOGPEN lpBlackPen = {PS_SOLID, 1, 1, RGB(0,0,0)};
    static  LOGPEN lpRedPen = {PS_SOLID, 1, 1, RGB(255,0,0)};

    HPEN hBlackPen;
    HPEN hRedPen;

    HBRUSH hShaddow;

    OPENFILE *pPaintFile;
    DATAFILE *pPaintData;

    int x,y;

    static short Shift = 0;
    static short nXChar, nCaps, nYChar;
    TEXTMETRIC   tm;

    static HANDLE hTmsRmn;
    LOGFONT LogFont;
    RECT    rect;
    RECT    rectDesktop;

    HPEN hBluePen;
    HBRUSH hBrush;

    switch (message) {

        case WM_PAINT:
            hDC = BeginPaint (hwnd, &PtStr);
            SetMapMode (hDC, MM_ANISOTROPIC);


            if  (hwnd == BEDITframe) {
                //********************************
                //* Create the Banner Font
                //********************************
                memset (&LogFont, 0, sizeof (LOGFONT));
                GetWindowRect (GetDesktopWindow (), &rectDesktop);
                if  (rectDesktop.bottom > 1000) {
                    LogFont.lfHeight = 110;
                    LogFont.lfWidth = 50;
                }
                else {
                    LogFont.lfHeight = 110;
                    LogFont.lfWidth = 50;
                }
                LogFont.lfQuality = PROOF_QUALITY;
                LogFont.lfItalic = 1;
                lstrcpy (LogFont.lfFaceName,"Tms Rmn");
                hTmsRmn = CreateFontIndirect (&LogFont);
                SelectObject (hDC, hTmsRmn);
                GetTextMetrics (hDC, &tm);
                nXChar = tm.tmAveCharWidth;
                nYChar = (tm.tmHeight + tm.tmExternalLeading) * 1.2;
                nCaps = (tm.tmPitchAndFamily & 1 ?3 : 2) * nXChar / 2;

                //**********************************
                //* Paint the Banner
                //**********************************
                GetWindowRect (hwnd, &rect);
                rect.bottom = rect.bottom - rect.top;
                rect.right = rect.right - rect.left;
    
                GetWindowRect (GetDesktopWindow (), &rectDesktop);
                if  (rectDesktop.bottom > 1000) {
                    Shift = nYChar;                // Desktop Resolution
                }
                else {
                    Shift = (short) - (nYChar / 4); // Laptop Resolution
                }
    
                strcpy (Command, "BannerLine1");
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    SelectObject (hDC, hTmsRmn);
                    SetTextColor(hDC, (COLORREF) RGB(0,0,255)); //* Blue
                    TextOut (hDC, ((rect.right - (nXChar * strlen(pVariable->szString))) / 2) - nXChar, nYChar+Shift, pVariable->szString, strlen(pVariable->szString));
                }
                strcpy (Command, "BannerLine2");
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    SelectObject (hDC, hTmsRmn);
                    SetTextColor(hDC, (COLORREF) RGB(0,0,255)); //* Blue
                    TextOut (hDC, ((rect.right - (nXChar * strlen(pVariable->szString))) / 2) - nXChar, (short)nYChar*2+Shift, pVariable->szString, strlen(pVariable->szString));
                }
                strcpy (Command, "BannerLine3");
                test = 'y';
                d580_find_variable ();
                test = 'n';
                if  (found == 'y') {
                    SelectObject (hDC, hTmsRmn);
                    SetTextColor(hDC, (COLORREF) RGB(0,0,255)); //* Blue
                    TextOut (hDC, ((rect.right - (nXChar * strlen(pVariable->szString))) / 2) - nXChar, nYChar*3+Shift, pVariable->szString, strlen(pVariable->szString));
                }
                DeleteObject(hTmsRmn);
            }

            hBlackPen = CreatePenIndirect(&lpBlackPen);
            hRedPen = CreatePenIndirect(&lpRedPen);

            //*******************************
            //* Find the PaintFile
            //*******************************
            pPaintFile = &StartOpenFile;
            do  {
                if  (!pPaintFile->pNext) {
                    break;
                }
                pPaintFile = pPaintFile->pNext;
            }
            while (strcmp (pPaintFile->szFileName, "PaintFile") != 0);
            if  (strcmp (pPaintFile->szFileName, "PaintFile") != 0) {
                EndPaint (hwnd, &PtStr);
                DeleteObject(hBlackPen);
                DeleteObject(hRedPen);
                break;
            }

            //*******************************
            //* Move to the top of the PaintFile
            //*******************************
            pPaintData = pPaintFile->pData;
            do  {
                if  (pPaintData->pPrev) {
                    pPaintData = pPaintData->pPrev;
                }
            }
            while (pPaintData->pPrev);

            //*******************************
            //* Find the Current Window
            //*******************************
            if  (hwnd != BEDITframe) {
                pObject = &StartObject;
                do  {
                    if  (pObject->pNext) {
                        pObject = pObject->pNext;
                    }
                }
                while ((pObject->pNext) &&
                       (pObject->hwnd != hwnd));
                if  (pObject->hwnd != hwnd) {
                    EndPaint (hwnd, &PtStr);
                    DeleteObject(hBlackPen);
                    DeleteObject(hRedPen);
                    break;
                }
            }

            //*******************************
            //* Check to see if it should be painted
            //*******************************
            {
                char szName   [100];
                char szObjectName [100];
                char szStyle  [100];
                char szX      [100];
                int  iXpos;
                char szY      [100];
                int  iYpos;
                char szCX     [100];
                int  iCX;
                char szCY     [100];
                int  iCY;

                char szFont   [50+1];
                char szText   [250+1];

                int iShaddowX;
                int iShaddowCX;
                int iShaddowY;
                int iShaddowCY;

                char szRed    [100];
                int  iRed;
                char szGreen  [100];
                int  iGreen;
                char szBlue   [100];
                int  iBlue;

                char szFilename [100];
           
                int  iLinePos;
                int  iTarget;

                int  iLineLen;

                do {
                    if  (pPaintData->pNext) {
                        pPaintData = pPaintData->pNext;
                    }
                    //**********************************
                    //* Parse the Line
                    //**********************************
                    iLineLen = strlen (pPaintData->szLine);
                    //***********************
                    //* Window Name
                    //***********************
                    iLinePos = 0;
                    iTarget = 0;
                    do  {
                        if  (pPaintData->szLine[iLinePos] != ',') {
                            szName[iTarget] = pPaintData->szLine[iLinePos];
                            iLinePos++;
                            iTarget++;
                        }
                    }
                    while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                    szName[iTarget] = 0;

                    //***********************
                    //* Object Name
                    //***********************
                    iTarget = 0;
                    if  (iLinePos < iLineLen) {
                        iLinePos++;
                    }
                    do  {
                        if  (pPaintData->szLine[iLinePos] != ',') {
                            szObjectName[iTarget] = pPaintData->szLine[iLinePos];
                            iLinePos++;
                            iTarget++;
                        }
                    }
                    while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                    szObjectName[iTarget] = 0;

                    //***********************
                    //* Style
                    //***********************
                    iTarget = 0;
                    if  (iLinePos < iLineLen) {
                        iLinePos++;
                    }
                    do  {
                        if  (pPaintData->szLine[iLinePos] != ',') {
                            szStyle[iTarget] = pPaintData->szLine[iLinePos];
                            if  (iLinePos < iLineLen) {
                                iLinePos++;
                            }
                            iTarget++;
                        }
                    }
                    while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                    szStyle[iTarget] = 0;

                    //***********************
                    //* Xpos
                    //***********************
                    iTarget = 0;
                    if  (iLinePos < iLineLen) {
                        iLinePos++;
                    }
                    do  {
                        if  (pPaintData->szLine[iLinePos] != ',') {
                            szX[iTarget] = pPaintData->szLine[iLinePos];
                            if  (iLinePos < iLineLen) {
                                iLinePos++;
                            }
                            iTarget++;
                        }
                    }
                    while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                    szX[iTarget] = 0;
                    iXpos = (int) strtol ((char*)szX, (char**)&p, 10);
                    //***********************
                    //* Ypos
                    //***********************
                    iTarget = 0;
                    if  (iLinePos < iLineLen) {
                        iLinePos++;
                    }
                    do  {
                        if  (pPaintData->szLine[iLinePos] != ',') {
                            szY[iTarget] = pPaintData->szLine[iLinePos];
                            if  (iLinePos < iLineLen) {
                                iLinePos++;
                            }
                            iTarget++;
                        }
                    }
                    while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                    szY[iTarget] = 0;
                    iYpos = (int) strtol ((char*)szY, (char**)&p, 10);

                    //z999_open_bug ();
                    //sprintf (buff, "szName[%s] szObjectName[%s] szStyle[%s] iXpos[%i] iYpos[%i]\n",szName, szObjectName, szStyle, iXpos, iYpos);
                    //fputs (buff, file_debug_out);

                    //***********************
                    //* CX
                    //***********************
                    if  ((strcmp (szStyle, "Point") != 0) &&
                         (strcmp (szStyle, "Bitmap") != 0) && 
                         (strcmp (szStyle, "BitmapInvert") != 0)) {
                        iTarget = 0;
                        if  (iLinePos < iLineLen) {
                            iLinePos++;
                        }
                        do  {
                            if  (pPaintData->szLine[iLinePos] != ',') {
                               szCX[iTarget] = pPaintData->szLine[iLinePos];
                                if  (iLinePos < iLineLen) {
                                    iLinePos++;
                                }
                                iTarget++;
                            }
                        }
                        while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                        szCX[iTarget] = 0;
                        iCX = (int) strtol ((char*)szCX, (char**)&p, 10);
                    }
                    //***********************
                    //* CY
                    //***********************
                    if  ((strcmp (szStyle, "Point") != 0) &&
                         (strcmp (szStyle, "Bitmap") != 0) &&
                         (strcmp (szStyle, "BitmapInvert") != 0)) {
                        iTarget = 0;
                        if  (iLinePos < iLineLen) {
                            iLinePos++;
                        }
                        do  {
                            if  (pPaintData->szLine[iLinePos] != ',') {
                                szCY[iTarget] = pPaintData->szLine[iLinePos];
                                if  (iLinePos < iLineLen) {
                                    iLinePos++;
                                }
                               iTarget++;
                            }
                       }
                        while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                        szCY[iTarget] = 0;
                        iCY = (int) strtol ((char*)szCY, (char**)&p, 10);
                    }
                    if  ((strcmp (szStyle, "Bitmap") == 0) ||
                         (strcmp (szStyle, "BitmapInvert") == 0) ||
                         (strcmp (szStyle, "BitmapFit") == 0) ||
                         (strcmp (szStyle, "BitmapFitInvert") == 0)) {
                        //***********************
                        //* Bitmap Filename
                        //***********************
                        memset (szFilename, 0, sizeof(szFilename));
                        iTarget = 0;
                        if  (iLinePos < iLineLen) {
                            iLinePos++;
                        }
                        do  {
                            if  (pPaintData->szLine[iLinePos] != ',') {
                                szFilename[iTarget] = pPaintData->szLine[iLinePos];
                                if  (iLinePos < iLineLen) {
                                    iLinePos++;
                                }
                                iTarget++;
                            }
                        }
                        while ((pPaintData->szLine[iLinePos] != ',') && ((int)pPaintData->szLine[iLinePos] > 31) && (iTarget < 95) && (iLinePos < iLineLen));
                        szFilename[iTarget] = 0;

                                //z999_open_bug ();
                                //sprintf (buff, "szStyle[%s] szFilename[%s] iXpos[%i] iYpos[%i] iCX[%i] iCY[%i]\n",szStyle, szFilename, iXpos, iYpos, iCX, iCY);
                                //fputs (buff, file_debug_out);
                    }
                    else {
                        //***********************
                        //* Red
                        //***********************
                        iTarget = 0;
                        if  (iLinePos < iLineLen) {
                            iLinePos++;
                        }
                        do  {
                            if  (pPaintData->szLine[iLinePos] != ',') {
                                szRed[iTarget] = pPaintData->szLine[iLinePos];
                                if  (iLinePos < iLineLen) {
                                    iLinePos++;
                                }
                                iTarget++;
                            }
                        }
                        while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                        szRed[iTarget] = 0;
                        iRed = (int) strtol ((char*)szRed, (char**)&p, 10);
                        //***********************
                        //* Green
                        //***********************
                        iTarget = 0;
                        if  (iLinePos < iLineLen) {
                            iLinePos++;
                        }
                        do  {
                            if  (pPaintData->szLine[iLinePos] != ',') {
                                szGreen[iTarget] = pPaintData->szLine[iLinePos];
                                if  (iLinePos < iLineLen) {
                                    iLinePos++;
                                }
                                iTarget++;
                            }
                        }
                        while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                        szGreen[iTarget] = 0;
                        iGreen = (int) strtol ((char*)szGreen, (char**)&p, 10);
                        //***********************
                        //* Blue
                        //***********************
                        iTarget = 0;
                        if  (iLinePos < iLineLen) {
                            iLinePos++;
                        }
                        do  {
                            if  (pPaintData->szLine[iLinePos] != ',') {
                                szBlue[iTarget] = pPaintData->szLine[iLinePos];
                                if  (iLinePos < iLineLen) {
                                    iLinePos++;
                                }
                                iTarget++;
                            }
                        }
                        while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 90) && (iLinePos < iLineLen));
                        szBlue[iTarget] = 0;
                        iBlue = (int) strtol ((char*)szBlue, (char**)&p, 10);


                        //***********************
                        //* Text
                        //***********************
                        if  ((strcmp (szStyle, "Text") == 0) ||
                             (strcmp (szStyle, "TextTilt") == 0)) {
                            //*********
                            //* Font
                            //*********
                            memset (szFont, 0, sizeof(szFont));
                            iTarget = 0;
                            if  (iLinePos < iLineLen) {
                                iLinePos++;
                            }
                            do  {
                                if  (pPaintData->szLine[iLinePos] != ',') {
                                    szFont[iTarget] = pPaintData->szLine[iLinePos];
                                    if  (iLinePos < iLineLen) {
                                        iLinePos++;
                                    }
                                    iTarget++;
                                }
                            }
                            while ((pPaintData->szLine[iLinePos] != ',') && (iTarget < 45) && (iLinePos < iLineLen));
                            szFont[iTarget] = 0;
                            //*********
                            //* Text
                            //*********
                            memset (szText, 0, sizeof(szText));
                            iTarget = 0;
                            if  (iLinePos < iLineLen) {
                                iLinePos++;
                            }
                            do  {
                                if  (pPaintData->szLine[iLinePos] != ',') {
                                    szText[iTarget] = pPaintData->szLine[iLinePos];
                                    if  (iLinePos < iLineLen) {
                                        iLinePos++;
                                    }
                                    iTarget++;
                                }
                            }
                            while ((iTarget < 250) && (iLinePos < iLineLen));
                            szText[iTarget] = 0;
                        }
                    }

                    if  ((hwnd != BEDITframe) && (pObject) && (strcmp (szName, pObject->szName) == 0) ||
                         ((hwnd == BEDITframe) && (strcmp (szName, "MainWindow") == 0))) {
                        int iHilightPos;
                        int iHilightSize;

                        //*****************************************
                        //* Paint the Objects
                        //*****************************************
                        SelectObject(hDC, hBlackPen);

                        hBrush = CreateSolidBrush(RGB(iRed,iGreen,iBlue));
                        hShaddow = CreateSolidBrush(RGB(0,0,0));


                        //**************************
                        //* Circle
                        //**************************
                        if  (strcmp (szStyle, "Circle") == 0) {
                            float fXpos;
                            float fYpos;
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);
                            fXpos = iXpos - (iCX/2);
                            iXpos = (int)fXpos;
                            fYpos = iYpos - (iCY/2);
                            iYpos = (int)fYpos;
                            Arc (hDC, iXpos,iYpos,iXpos+iCX,iYpos+iCY,iXpos,iYpos,iXpos,iYpos);
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Ellipse - Filled
                        //**************************
                        if  (strcmp (szStyle, "Ellipse") == 0) {
                            float fXpos;
                            float fYpos;
                            SelectObject(hDC, hBrush);
                            fXpos = iXpos - (iCX/2);
                            iXpos = (int)fXpos;
                            fYpos = iYpos - (iCY/2);
                            iYpos = (int)fYpos;
                            Ellipse (hDC, iXpos,iYpos,iXpos+iCX,iYpos+iCY);
                        }
                        //**************************
                        //* Ellipse3D - Filled
                        //**************************
                        if  (strcmp (szStyle, "Ellipse3D") == 0) {
                            float fXpos;
                            float fYpos;
                            fXpos = iXpos - (iCX/2);
                            iXpos = (int)fXpos;
                            fYpos = iYpos - (iCY/2);
                            iYpos = (int)fYpos;
                            iHilightPos = iCX * 0.2;
                            iHilightSize = iCX * 0.2;

                            iShaddowX = iXpos + (iCX * .1);
                            iShaddowCX = iCX;
                            iShaddowY = iYpos + (iCY * .9);
                            iShaddowCY = iCY * .1;

                            SelectObject(hDC, hShaddow);
                            Ellipse (hDC, iShaddowX,iShaddowY,iShaddowX+iShaddowCX,iShaddowY+iShaddowCY);
                            //Ellipse (hDC, iXpos+1,iYpos+1,iXpos+iCX+1,iYpos+iCY+1);

                            SelectObject(hDC, hBrush);
                            Ellipse (hDC, iXpos,iYpos,iXpos+iCX,iYpos+iCY);
                            SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                            Ellipse (hDC, iXpos+iHilightPos,iYpos+iHilightPos,iXpos+iHilightPos+iHilightSize,iYpos+iHilightPos+iHilightSize);
                            //Ellipse (hDC, iXpos+iCX-iHilightPos*1.5,iYpos+iCY-iHilightPos*1.5,iXpos+iCX-iHilightPos*1.5+iHilightSize/2,iYpos+iCY-iHilightPos*1.5+iHilightSize/2);
                        }

                        //**************************
                        //* Box - Filled
                        //**************************
                        if  (strcmp (szStyle, "Box") == 0) {
                            SelectObject(hDC, hBrush);
                            Rectangle (hDC, iXpos,iYpos,iXpos+iCX,iYpos+iCY);
                        }
                        //**************************
                        //* Boxround - Filled
                        //**************************
                        if  (strcmp (szStyle, "Boxround") == 0) {
                            SelectObject(hDC, hShaddow);
			    RoundRect (hDC, iXpos+3,iYpos+3,iXpos+iCX+3,iYpos+iCY+3,15,15);
                            SelectObject(hDC, hBrush);
			    RoundRect (hDC, iXpos,iYpos,iXpos+iCX,iYpos+iCY,15,15);
                        }
                        //**************************
                        //* Box 3D - Filled
                        //**************************
                        if  (strcmp (szStyle, "Box3D") == 0) {
                            iShaddowX = iXpos + 1;
                            iShaddowCX = iCX;
                            iShaddowY = iYpos + 1;
                            iShaddowCY = iCY;

                            SelectObject(hDC, hShaddow);
                            Rectangle (hDC, iShaddowX,iShaddowY,iShaddowX+iShaddowCX,iShaddowY+iShaddowCY);
                            Rectangle (hDC, iShaddowX,iShaddowY,iShaddowX+iShaddowCX+1,iShaddowY+iShaddowCY+1);

                            SelectObject(hDC, hBrush);
                            Rectangle (hDC, iXpos,iYpos,iXpos+iCX,iYpos+iCY);

                            {
                                HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));
                                HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));
                                SelectObject(hDC, hPen);
                                MoveToEx(hDC,iXpos+2,iYpos+2,NULL);
                                LineTo(hDC,iXpos+iCX-3,iYpos+2);
                                SelectObject(hDC, hPenShaddow);
                                LineTo(hDC,iXpos+iCX-3,iYpos+iCY-3);

                                SelectObject(hDC, hPen);
                                MoveToEx(hDC,iXpos+2,iYpos+2,NULL);
                                LineTo(hDC,iXpos+2,iYpos+iCY-3);
                                SelectObject(hDC, hPenShaddow);
                                LineTo(hDC,iXpos+iCX-3,iYpos+iCY-3);

                                DeleteObject(hPen);
                                DeleteObject(hPenShaddow);

                            }

                        }
                        //**************************
                        //* Rectangle
                        //**************************
                        if  (strcmp (szStyle, "Rectangle") == 0) {
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iYpos);
                            LineTo(hDC,iCX,iCY);
                            LineTo(hDC,iXpos,iCY);
                            LineTo(hDC,iXpos,iYpos);
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* TriangleA
                        //**************************
                        if  (strcmp (szStyle, "TriangleA") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iCenter = iXpos + (iCX / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            SelectObject(hDC, hPenShaddow);
			    Points[0].x = iCenter+3;
			    Points[0].y = iYpos+3;
			    Points[1].x = iCX+3;
			    Points[1].y = iCY+3;
			    Points[2].x = iXpos+3;
			    Points[2].y = iCY+3;
			    pPoints = Points;
			    SelectObject(hDC, hShaddow);
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hPen);
			    Points[0].x = iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iCX;
			    Points[1].y = iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iCY;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 3);

                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));

                                //SelectObject(hDC, hPenShaddow);
                                //MoveToEx(hDC,iCenter,iYpos+3,NULL);
                                //LineTo(hDC,iCX-3,iCY-3);
                                //LineTo(hDC,iXpos+3,iCY-3);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iCenter,iYpos+3);

                                //DeleteObject(hPen);

                            }

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* TriangleB
                        //**************************
                        if  (strcmp (szStyle, "TriangleB") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iCenter = iXpos + (iCX / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            SelectObject(hDC, hPenShaddow);
                            SelectObject(hDC, hShaddow);
			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iCX+2;
			    Points[1].y = iYpos+2;
			    Points[2].x = iCenter+2;
			    Points[2].y = iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos;
			    Points[1].x = iCX;
			    Points[1].y = iYpos;
			    Points[2].x = iCenter;
			    Points[2].y = iCY;
			    pPoints = Points;
                            SelectObject(hDC, hPen);
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //**************************
                        //* TriangleC
                        //**************************
                        if  (strcmp (szStyle, "TriangleC") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iCenter = iYpos + (iCY / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+2;
			    Points[1].y = iCY+2;
			    Points[2].x = iCX+2;
			    Points[2].y = iCenter+2;
			    pPoints = Points;
                            SelectObject(hDC, hPenShaddow);
			    SelectObject(hDC, hShaddow);
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos;
			    Points[1].y = iCY;
			    Points[2].x = iCX;
			    Points[2].y = iCenter;
			    pPoints = Points;
                            SelectObject(hDC, hPen);
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* TriangleD
                        //**************************
                        if  (strcmp (szStyle, "TriangleD") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iCenter = iYpos + (iCY / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

			    Points[0].x = iCX+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iCX+2;
			    Points[1].y = iCY+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iCenter+2;
			    pPoints = Points;
                            SelectObject(hDC, hPenShaddow);
			    SelectObject(hDC, hShaddow);
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iCX;
			    Points[0].y = iYpos;
			    Points[1].x = iCX;
			    Points[1].y = iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iCenter;
			    pPoints = Points;
                            SelectObject(hDC, hPen);
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* TriangleE
                        //**************************
                        if  (strcmp (szStyle, "TriangleE") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

			    Points[0].x = iCX+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iCX+2;
			    Points[1].y = iCY+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iCY+2;
			    pPoints = Points;
			    SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iCX;
			    Points[0].y = iYpos;
			    Points[1].x = iCX;
			    Points[1].y = iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iCY;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* TriangleF
                        //**************************
                        if  (strcmp (szStyle, "TriangleF") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iCX+2;
			    Points[1].y = iCY+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iCY+2;
			    pPoints = Points;
			    SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos;
			    Points[1].x = iCX;
			    Points[1].y = iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iCY;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //**************************
                        //* TriangleG
                        //**************************
                        if  (strcmp (szStyle, "TriangleG") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

			    Points[0].x = iXpos+3;
			    Points[0].y = iYpos+3;
			    Points[1].x = iCX+3;
			    Points[1].y = iYpos+3;
			    Points[2].x = iCX+3;
			    Points[2].y = iCY+3;
			    pPoints = Points;
			    SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos;
			    Points[1].x = iCX;
			    Points[1].y = iYpos;
			    Points[2].x = iCX;
			    Points[2].y = iCY;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* TriangleH
                        //**************************
                        if  (strcmp (szStyle, "TriangleH") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iCX+2;
			    Points[1].y = iYpos+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iCY+2;
			    pPoints = Points;
			    SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos;
			    Points[1].x = iCX;
			    Points[1].y = iYpos;
			    Points[2].x = iXpos;
			    Points[2].y = iCY;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //***************************************************************
			//* Hollow Triangles
			//***************************************************************
                        //**************************
                        //* TriangleI
                        //**************************
                        if  (strcmp (szStyle, "TriangleI") == 0) {

			    int  iCenter = iXpos + (iCX / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iCenter,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            LineTo(hDC,iXpos,iCY);
                            LineTo(hDC,iCenter,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* TriangleJ
                        //**************************
                        if  (strcmp (szStyle, "TriangleJ") == 0) {

			    int  iCenter = iXpos + (iCX / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iYpos);
                            LineTo(hDC,iCenter,iCY);
                            LineTo(hDC,iXpos,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* TriangleK
                        //**************************
                        if  (strcmp (szStyle, "TriangleK") == 0) {

			    int  iCenter = iYpos + (iCY / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iXpos,iCY);
                            LineTo(hDC,iCX,iCenter);
                            LineTo(hDC,iXpos,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* TriangleL
                        //**************************
                        if  (strcmp (szStyle, "TriangleL") == 0) {

			    int  iCenter = iYpos + (iCY / 2);
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iCX,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            LineTo(hDC,iXpos,iCenter);
                            LineTo(hDC,iCX,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* TriangleM
                        //**************************
                        if  (strcmp (szStyle, "TriangleM") == 0) {

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iCX,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            LineTo(hDC,iXpos,iCY);
                            LineTo(hDC,iCX,iYpos);

                            DeleteObject(hPen);
                        }

                        //**************************
                        //* TriangleN
                        //**************************
                        if  (strcmp (szStyle, "TriangleN") == 0) {

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            LineTo(hDC,iXpos,iCY);
                            LineTo(hDC,iXpos,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* TriangleO
                        //**************************
                        if  (strcmp (szStyle, "TriangleO") == 0) {

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iYpos);
                            LineTo(hDC,iCX,iCY);
                            LineTo(hDC,iXpos,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* TriangleP
                        //**************************
                        if  (strcmp (szStyle, "TriangleP") == 0) {

                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

                            iCX = iXpos + iCX;
                            iCY = iYpos + iCY;

                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iYpos);
                            LineTo(hDC,iXpos,iCY);
                            LineTo(hDC,iXpos,iYpos);

                            DeleteObject(hPen);
                        }

                        //**************************
                        //* Diamond
                        //**************************
                        if  (strcmp (szStyle, "Diamond") == 0) {
			    POINT Points[4];
			    POINT *pPoints;

                            //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            //SelectObject(hDC, hPen);

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+2+(iCX/2);
			    Points[1].y = iYpos+2+(iCY/2);
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+2+iCY;
			    Points[3].x = iXpos+2-(iCX/2);
			    Points[3].y = iYpos+2+(iCY/2);
			    pPoints = Points;
                            SelectObject(hDC, hShaddow);
			    Polygon (hDC, pPoints, 4);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+(iCX/2);
			    Points[1].y = iYpos+(iCY/2);
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCY;
			    Points[3].x = iXpos-(iCX/2);
			    Points[3].y = iYpos+(iCY/2);
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 4);


                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));
                                //HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

                                //MoveToEx(hDC,iXpos,iYpos+2,NULL);

                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+(iCX/2)-2,iYpos+(iCY/2));
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos,iYpos+iCY-2);
                                //LineTo(hDC,iXpos-(iCX/2)+2,iYpos+(iCY/2));
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos,iYpos+2);

                                //DeleteObject(hPen);
                                //DeleteObject(hPenShaddow);

                            }

                            //DeleteObject(hPen);
                        }

                        //**************************
                        //* Line
                        //**************************
                        if  (strcmp (szStyle, "Line") == 0) {
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow
                        //**************************
                        if  (strcmp (szStyle, "Arrow") == 0) {
			    POINT Points[3];
			    POINT *pPoints;
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            {
				int   X;
				int   Y;
                                int   Z;
				float A;
				float B;
				float Work;
				int   X3;
				int   Y3;
				int   X4;
				int   Y4;
				int   Angle = 20;

				//******************************
				//* Arrow Point 1
				//******************************
				X = iCX - iXpos;
				Y = iYpos - iCY;
				if  (X == 0) X = 1;
				if  (Y == 0) Y = 1;
				Z = sqrt((X * X) + (Y * Y));
				Work = (float) X / (float)Z;
				A = acos (Work) * 180 / PI;
				B = 90 - A;

				if  (iYpos < iCY) {
				    X3 = iCX + (10 * cos((B - Angle + 90) * PI / 180));
				    Y3 = iCY - (10 * sin((B - Angle + 90) * PI / 180));
				}
				else {
				    X3 = iCX - (10 * sin((B - Angle) * PI / 180));
				    Y3 = iCY + (10 * cos((B - Angle) * PI / 180));
				}

                            	//LineTo(hDC,X3,Y3);

				//******************************
				//* Arrow Point 2
				//******************************
                           	//MoveToEx(hDC,iCX,iCY,NULL);
				if  (iYpos < iCY) {
				    X4 = iCX + (10 * cos((B + Angle + 90) * PI / 180));
				    Y4 = iCY - (10 * sin((B + Angle + 90) * PI / 180));
				}
				else {
				    X4 = iCX - (10 * sin((B + Angle) * PI / 180));
				    Y4 = iCY + (10 * cos((B + Angle) * PI / 180));
				}

                            	//LineTo(hDC,X4,Y4);
			    	Points[0].x = iCX;
			   	Points[0].y = iCY;
			    	Points[1].x = X3;
			    	Points[1].y = Y3;
			    	Points[2].x = X4;
			    	Points[2].y = Y4;
			    	pPoints = Points;
				SelectObject(hDC, hBrush);
			    	Polygon (hDC, pPoints, 3);
                            }
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow2
                        //**************************
                        if  (strcmp (szStyle, "Arrow2") == 0) {
			    POINT Points[3];
			    POINT *pPoints;
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            {
				int   X;
				int   Y;
                                int   Z;
				float A;
				float B;
				float Work;
				int   X3;
				int   Y3;
				int   X4;
				int   Y4;
				int   Angle = 20;

				//******************************
				//* Arrow Point 1
				//******************************
				X = iCX - iXpos;
				Y = iYpos - iCY;
				if  (X == 0) X = 1;
				if  (Y == 0) Y = 1;
				Z = sqrt((X * X) + (Y * Y));
				Work = (float) X / (float)Z;
				A = acos (Work) * 180 / PI;
				B = 90 - A;

				if  (iYpos < iCY) {
				    X3 = iCX + (10 * cos((B - Angle + 90) * PI / 180));
				    Y3 = iCY - (10 * sin((B - Angle + 90) * PI / 180));
				}
				else {
				    X3 = iCX - (10 * sin((B - Angle) * PI / 180));
				    Y3 = iCY + (10 * cos((B - Angle) * PI / 180));
				}

                            	LineTo(hDC,X3,Y3);

				//******************************
				//* Arrow Point 2
				//******************************
                           	MoveToEx(hDC,iCX,iCY,NULL);
				if  (iYpos < iCY) {
				    X4 = iCX + (10 * cos((B + Angle + 90) * PI / 180));
				    Y4 = iCY - (10 * sin((B + Angle + 90) * PI / 180));
				}
				else {
				    X4 = iCX - (10 * sin((B + Angle) * PI / 180));
				    Y4 = iCY + (10 * cos((B + Angle) * PI / 180));
				}

                            	LineTo(hDC,X4,Y4);
			    	//Points[0].x = iCX;
			   	//Points[0].y = iCY;
			    	//Points[1].x = X3;
			    	//Points[1].y = Y3;
			    	//Points[2].x = X4;
			    	//Points[2].y = Y4;
			    	//pPoints = Points;
				//SelectObject(hDC, hBrush);
			    	//Polygon (hDC, pPoints, 3);
                            }
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow3
                        //**************************
                        if  (strcmp (szStyle, "Arrow3") == 0) {
			    POINT Points[3];
			    POINT *pPoints;
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            {
				int   X;
				int   Y;
                                int   Z;
				float A;
				float B;
				float Work;
				int   X3;
				int   Y3;
				int   X4;
				int   Y4;
				int   Angle = 20;

				//******************************
				//* Arrow Point 1
				//******************************
				X = iCX - iXpos;
				Y = iYpos - iCY;
				if  (X == 0) X = 1;
				if  (Y == 0) Y = 1;
				Z = sqrt((X * X) + (Y * Y));
				Work = (float) X / (float)Z;
				A = acos (Work) * 180 / PI;
				B = 90 - A;

				if  (iYpos < iCY) {
				    X3 = iCX + (10 * cos((B - Angle + 90) * PI / 180));
				    Y3 = iCY - (10 * sin((B - Angle + 90) * PI / 180));
				}
				else {
				    X3 = iCX - (10 * sin((B - Angle) * PI / 180));
				    Y3 = iCY + (10 * cos((B - Angle) * PI / 180));
				}

                            	//LineTo(hDC,X3,Y3);

				//******************************
				//* Arrow Point 2
				//******************************
                           	//MoveToEx(hDC,iCX,iCY,NULL);
				if  (iYpos < iCY) {
				    X4 = iCX + (10 * cos((B + Angle + 90) * PI / 180));
				    Y4 = iCY - (10 * sin((B + Angle + 90) * PI / 180));
				}
				else {
				    X4 = iCX - (10 * sin((B + Angle) * PI / 180));
				    Y4 = iCY + (10 * cos((B + Angle) * PI / 180));
				}

                            	//LineTo(hDC,X4,Y4);
			    	Points[0].x = iCX;
			   	Points[0].y = iCY;
			    	Points[1].x = X3;
			    	Points[1].y = Y3;
			    	Points[2].x = X4;
			    	Points[2].y = Y4;
			    	pPoints = Points;
				SelectObject(hDC, hBrush);
			    	Polygon (hDC, pPoints, 3);

				//******************************
				//* Switch Points
				//******************************
				Work = iXpos;
				iXpos = iCX;
				iCX = Work;
				Work = iYpos;
				iYpos = iCY;
				iCY = Work;

				//******************************
				//* Arrow Point 3
				//******************************
				X = iCX - iXpos;
				Y = iYpos - iCY;
				if  (X == 0) X = 1;
				if  (Y == 0) Y = 1;
				Z = sqrt((X * X) + (Y * Y));
				Work = (float) X / (float)Z;
				A = acos (Work) * 180 / PI;
				B = 90 - A;

				if  (iYpos < iCY) {
				    X3 = iCX + (10 * cos((B - Angle + 90) * PI / 180));
				    Y3 = iCY - (10 * sin((B - Angle + 90) * PI / 180));
				}
				else {
				    X3 = iCX - (10 * sin((B - Angle) * PI / 180));
				    Y3 = iCY + (10 * cos((B - Angle) * PI / 180));
				}

                            	//LineTo(hDC,X3,Y3);

				//******************************
				//* Arrow Point 4
				//******************************
                           	//MoveToEx(hDC,iCX,iCY,NULL);
				if  (iYpos < iCY) {
				    X4 = iCX + (10 * cos((B + Angle + 90) * PI / 180));
				    Y4 = iCY - (10 * sin((B + Angle + 90) * PI / 180));
				}
				else {
				    X4 = iCX - (10 * sin((B + Angle) * PI / 180));
				    Y4 = iCY + (10 * cos((B + Angle) * PI / 180));
				}

                            	//LineTo(hDC,X4,Y4);
			    	Points[0].x = iCX;
			   	Points[0].y = iCY;
			    	Points[1].x = X3;
			    	Points[1].y = Y3;
			    	Points[2].x = X4;
			    	Points[2].y = Y4;
			    	pPoints = Points;
				SelectObject(hDC, hBrush);
			    	Polygon (hDC, pPoints, 3);
                            }
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow4
                        //**************************
                        if  (strcmp (szStyle, "Arrow4") == 0) {
			    POINT Points[3];
			    POINT *pPoints;
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iCX,iCY);
                            {
				int   X;
				int   Y;
                                int   Z;
				float A;
				float B;
				float Work;
				int   X3;
				int   Y3;
				int   X4;
				int   Y4;
				int   Angle = 20;

				//******************************
				//* Arrow Point 1
				//******************************
				X = iCX - iXpos;
				Y = iYpos - iCY;
				if  (X == 0) X = 1;
				if  (Y == 0) Y = 1;
				Z = sqrt((X * X) + (Y * Y));
				Work = (float) X / (float)Z;
				A = acos (Work) * 180 / PI;
				B = 90 - A;

				if  (iYpos < iCY) {
				    X3 = iCX + (10 * cos((B - Angle + 90) * PI / 180));
				    Y3 = iCY - (10 * sin((B - Angle + 90) * PI / 180));
				}
				else {
				    X3 = iCX - (10 * sin((B - Angle) * PI / 180));
				    Y3 = iCY + (10 * cos((B - Angle) * PI / 180));
				}

                            	LineTo(hDC,X3,Y3);

				//******************************
				//* Arrow Point 2
				//******************************
                           	MoveToEx(hDC,iCX,iCY,NULL);
				if  (iYpos < iCY) {
				    X4 = iCX + (10 * cos((B + Angle + 90) * PI / 180));
				    Y4 = iCY - (10 * sin((B + Angle + 90) * PI / 180));
				}
				else {
				    X4 = iCX - (10 * sin((B + Angle) * PI / 180));
				    Y4 = iCY + (10 * cos((B + Angle) * PI / 180));
				}

                            	LineTo(hDC,X4,Y4);
			    	//Points[0].x = iCX;
			   	//Points[0].y = iCY;
			    	//Points[1].x = X3;
			    	//Points[1].y = Y3;
			    	//Points[2].x = X4;
			    	//Points[2].y = Y4;
			    	//pPoints = Points;
				//SelectObject(hDC, hBrush);
			    	//Polygon (hDC, pPoints, 3);

				//******************************
				//* Switch Points
				//******************************
				Work = iXpos;
				iXpos = iCX;
				iCX = Work;
				Work = iYpos;
				iYpos = iCY;
				iCY = Work;

				//******************************
				//* Arrow Point 3
				//******************************
				X = iCX - iXpos;
				Y = iYpos - iCY;
				if  (X == 0) X = 1;
				if  (Y == 0) Y = 1;
				Z = sqrt((X * X) + (Y * Y));
				Work = (float) X / (float)Z;
				A = acos (Work) * 180 / PI;
				B = 90 - A;

				if  (iYpos < iCY) {
				    X3 = iCX + (10 * cos((B - Angle + 90) * PI / 180));
				    Y3 = iCY - (10 * sin((B - Angle + 90) * PI / 180));
				}
				else {
				    X3 = iCX - (10 * sin((B - Angle) * PI / 180));
				    Y3 = iCY + (10 * cos((B - Angle) * PI / 180));
				}
                           	MoveToEx(hDC,iCX,iCY,NULL);
                            	LineTo(hDC,X3,Y3);

				//******************************
				//* Arrow Point 4
				//******************************
                           	MoveToEx(hDC,iCX,iCY,NULL);
				if  (iYpos < iCY) {
				    X4 = iCX + (10 * cos((B + Angle + 90) * PI / 180));
				    Y4 = iCY - (10 * sin((B + Angle + 90) * PI / 180));
				}
				else {
				    X4 = iCX - (10 * sin((B + Angle) * PI / 180));
				    Y4 = iCY + (10 * cos((B + Angle) * PI / 180));
				}

                            	LineTo(hDC,X4,Y4);
			    	//Points[0].x = iCX;
			   	//Points[0].y = iCY;
			    	//Points[1].x = X3;
			    	//Points[1].y = Y3;
			    	//Points[2].x = X4;
			    	//Points[2].y = Y4;
			    	//pPoints = Points;
				//SelectObject(hDC, hBrush);
			    	//Polygon (hDC, pPoints, 3);
                            }
                            DeleteObject(hPen);
                        }

                        //****************************************************
                        //* Large Arrows
                        //****************************************************
                        //**************************
                        //* Arrow 5 - Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow5") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iHeight;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    iHeight = iCY / 3;
			    iCenter = iCY / 2;

                            SelectObject(hDC, hPenShaddow);
			    SelectObject(hDC, hShaddow);
			    Rectangle (hDC, iXpos+2,iYpos+iHeight+2,iXpos+iCX-iCenter+5,iYpos+iCY-iHeight+2);
                            SelectObject(hDC, hPen);
                            SelectObject(hDC, hBrush);
			    Rectangle (hDC, iXpos,iYpos+iHeight,iXpos+iCX-iCenter,iYpos+iCY-iHeight);

                            SelectObject(hDC, hPenShaddow);
			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    SelectObject(hDC, hShaddow);
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hPen);
			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 3);

                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));

                                //SelectObject(hDC, hPen);
                                //MoveToEx(hDC,iXpos+2,iYpos+iHeight+2,NULL);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+iHeight+2);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+4);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCX-2,iYpos+iCenter);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+iCY-5);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+iCY-iHeight-2);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+2,iYpos+iCY-iHeight-2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+2,iYpos+iHeight+2);

                                //DeleteObject(hPen);

                            }

                            DeleteObject(hPenShaddow);
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 6 - Left
                        //**************************
                        if  (strcmp (szStyle, "Arrow6") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iHeight;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));
                            SelectObject(hDC, hPenShaddow);

			    iHeight = iCY / 3;
			    iCenter = iCY / 2;

			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCY+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    SelectObject(hDC, hShaddow);
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hPen);
			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hPenShaddow);
                            SelectObject(hDC, hShaddow);
			    Rectangle (hDC, iXpos+iCenter+2,iYpos+iHeight+2,iXpos+iCX+2,iYpos+iCY-iHeight+2);
                            SelectObject(hDC, hPen);
                            SelectObject(hDC, hBrush);
			    Rectangle (hDC, iXpos+iCenter,iYpos+iHeight,iXpos+iCX,iYpos+iCY-iHeight);

                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));

                                //SelectObject(hDC, hPenShaddow);
                                //MoveToEx(hDC,iXpos+iCenter-1,iYpos+4,NULL);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+iHeight+2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCX-2,iYpos+iHeight+2);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCX-2,iYpos+iCY-iHeight-2);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+iCY-iHeight-2);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+iCY-5);
                                //LineTo(hDC,iXpos+2,iYpos+iCenter);

                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+4);

                                //DeleteObject(hPen);
                            }

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //**************************
                        //* Arrow 7 - Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow7") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    iWidth = iCX / 3;
			    iCenter = iCX / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iWidth+2,iYpos+iCenter+2,iXpos+iCX-iWidth+2,iYpos+iCY+2);
			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iWidth,iYpos+iCenter,iXpos+iCX-iWidth,iYpos+iCY);
			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    SelectObject(hDC, hBrush);
			    Polygon (hDC, pPoints, 3);

                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));

                                //SelectObject(hDC, hPenShaddow);
                                //MoveToEx(hDC,iXpos+iCenter,iYpos+4,NULL);
                                //LineTo(hDC,iXpos+iCX-5,iYpos+iCenter-2);
                                //LineTo(hDC,iXpos+iCX-iWidth-2,iYpos+iCenter-2);
                                //LineTo(hDC,iXpos+iCX-iWidth-2,iYpos+iCY-2);
                                //LineTo(hDC,iXpos+iWidth+2,iYpos+iCY-2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iWidth+2,iYpos+iCenter-2);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+6,iYpos+iCenter-2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCenter,iYpos+4);

                                //DeleteObject(hPen);
                            }

                            DeleteObject(hPenShaddow);
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 8 - Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow8") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    iWidth = iCX / 3;
			    iCenter = iCX / 2;

                            SelectObject(hDC, hPenShaddow);
                            SelectObject(hDC, hShaddow);
			    Rectangle (hDC, iXpos+iWidth+2,iYpos+2,iXpos+iCX-iWidth+2,iYpos+iCY-iCenter+2);
			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCY-iCenter+2;
			    Points[2].x = iXpos+iCenter+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iWidth,iYpos,iXpos+iCX-iWidth,iYpos+iCY-iCenter);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+iCenter;
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));

                                //SelectObject(hDC, hPen);
                                //MoveToEx(hDC,iXpos+iWidth+2,iYpos+2,NULL);
                                //LineTo(hDC,iXpos+iCX-iWidth-3,iYpos+2);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCX-iWidth-3,iYpos+iCY-iCenter+2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCX-5,iYpos+iCY-iCenter+2);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCenter,iYpos+iCY-4);
                                //LineTo(hDC,iXpos+6,iYpos+iCY-iCenter+2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iWidth+2,iYpos+iCY-iCenter+2);
                                //LineTo(hDC,iXpos+iWidth+2,iYpos+2);

                                DeleteObject(hPen);
                            }

                            DeleteObject(hPenShaddow);
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 9 - Left/Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow9") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iHeight;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));
                            SelectObject(hDC, hPen);

			    iHeight = iCY / 3;
			    iCenter = iCY / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iCenter+2,iYpos+iHeight+2,iXpos+iCX-iCenter+2,iYpos+iCY-iHeight+2);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCY+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iCenter,iYpos+iHeight,iXpos+iCX-iCenter,iYpos+iCY-iHeight);

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));

                                //SelectObject(hDC, hPen);
                                //MoveToEx(hDC,iXpos+iCenter+2,iYpos+iHeight+2,NULL);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+iHeight+2);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+4);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCX-2,iYpos+iCenter);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+iCY-5);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCX-iCenter+2,iYpos+iCY-iHeight-2);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+iCY-iHeight-2);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+iCY-5);
                                //LineTo(hDC,iXpos+2,iYpos+iCenter);

                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+4);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCenter-1,iYpos+iHeight+2);

                                //DeleteObject(hPen);

                            }

                            DeleteObject(hPenShaddow);
                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 10 - Up/Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow10") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    iWidth = iCX / 3;
			    iCenter = iCX / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iWidth+2,iYpos+iCenter+2,iXpos+iCX-iWidth+2,iYpos+iCY-iCenter+2);

			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCY-iCenter+2;
			    Points[2].x = iXpos+iCenter+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iWidth,iYpos+iCenter,iXpos+iCX-iWidth,iYpos+iCY-iCenter);

			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;

			    Polygon (hDC, pPoints, 3);
			    Points[0].x = iXpos;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+iCenter;
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            {
                                //HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(255,255,255));

                                //SelectObject(hDC, hPenShaddow);
                                //MoveToEx(hDC,iXpos+iCX-iWidth-3,iYpos+iCenter-2,NULL);
                                //LineTo(hDC,iXpos+iCX-iWidth-3,iYpos+iCY-iCenter+2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCX-5,iYpos+iCY-iCenter+2);
                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCenter,iYpos+iCY-4);
                                //LineTo(hDC,iXpos+6,iYpos+iCY-iCenter+2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iWidth+2,iYpos+iCY-iCenter+2);
                                //LineTo(hDC,iXpos+iWidth+2,iYpos+iCenter-2);

                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+6,iYpos+iCenter-2);
                                //SelectObject(hDC, hPen);
                                //LineTo(hDC,iXpos+iCenter,iYpos+4);

                                //SelectObject(hDC, hPenShaddow);
                                //LineTo(hDC,iXpos+iCX-5,iYpos+iCenter-2);
                                //LineTo(hDC,iXpos+iCX-iWidth-2,iYpos+iCenter-2);

                                //DeleteObject(hPen);
                            }

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //**************************
                        //* Arrow 11 - Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow11") == 0) {

			    int  iHeight;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));


			    iHeight = iCY / 3;
			    iCenter = iCY / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos+iHeight,NULL);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iHeight);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-iHeight);
                            LineTo(hDC,iXpos,iYpos+iCY-iHeight);
                            LineTo(hDC,iXpos,iYpos+iHeight);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 12 - Left
                        //**************************
                        if  (strcmp (szStyle, "Arrow12") == 0) {

			    int  iHeight;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

			    iHeight = iCY / 3;
			    iCenter = iCY / 2;

                            MoveToEx(hDC,iXpos+iCenter,iYpos,NULL);
                            LineTo(hDC,iXpos+iCenter,iYpos+iHeight);

                            LineTo(hDC,iXpos+iCX,iYpos+iHeight);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-iHeight);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-iHeight);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos);

                            DeleteObject(hPen);

                        }

                        //**************************
                        //* Arrow 13 - Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow13") == 0) {

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    iWidth = iCX / 3;
			    iCenter = iCX / 2;

                            SelectObject(hDC, hPen);

                            MoveToEx(hDC,iXpos+iCenter,iYpos,NULL);
                            LineTo(hDC,iXpos+iCX,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+iCY);
                            LineTo(hDC,iXpos+iWidth,iYpos+iCY);
                            LineTo(hDC,iXpos+iWidth,iYpos+iCenter);
                            LineTo(hDC,iXpos,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 14 - Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow14") == 0) {

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));


			    iWidth = iCX / 3;
			    iCenter = iCX / 2;

                            SelectObject(hDC, hPen);

                            MoveToEx(hDC,iXpos+iWidth,iYpos,NULL);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iWidth,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 15 - Left/Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow15") == 0) {

			    int  iHeight;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            SelectObject(hDC, hPen);

			    iHeight = iCY / 3;
			    iCenter = iCY / 2;

                            MoveToEx(hDC,iXpos+iCenter,iYpos+iHeight,NULL);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iHeight);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-iHeight);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-iHeight);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos);
                            LineTo(hDC,iXpos+iCenter,iYpos+iHeight);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 16 - Up/Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow16") == 0) {

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    iWidth = iCX / 3;
			    iCenter = iCX / 2;

                            SelectObject(hDC, hPen);

                            MoveToEx(hDC,iXpos+iCX-iWidth,iYpos+iCenter,NULL);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iWidth,iYpos+iCenter);
                            LineTo(hDC,iXpos,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+iCenter);

                            DeleteObject(hPen);
                        }

                        //**************************
                        //* Arrow 20 - Up to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow20") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+2,iYpos+(iWidth/2)+2,iXpos+iWidth+2,iYpos+iCY+2);
			    Rectangle (hDC, iXpos+2,iYpos+(iWidth/2)+2,iXpos+iCX-iCenter+2,iYpos+iCenter-(iWidth/2)+2);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+(iCenter/2)+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos,iYpos+(iWidth/2),iXpos+iWidth,iYpos+iCY);
			    Rectangle (hDC, iXpos,iYpos+(iWidth/2),iXpos+iCX-iCenter,iYpos+iCenter-(iWidth/2));

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+(iCenter/2);
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 21 - Up to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow21") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iCenter+2,iYpos+(iWidth/2)+2,iXpos+iCX+2,iYpos+(iWidth/2)+iWidth+2);
			    Rectangle (hDC, iXpos+iCX-iWidth+2,iYpos+(iWidth/2)+2,iXpos+iCX+2,iYpos+iCY+2);

			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+(iCenter/2)+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iCenter,iYpos+(iWidth/2),iXpos+iCX,iYpos+(iWidth/2)+iWidth);
			    Rectangle (hDC, iXpos+iCX-iWidth,iYpos+(iWidth/2),iXpos+iCX,iYpos+iCY);

			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+(iCenter/2);
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 22 - Down to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow22") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+2,iYpos+2,iXpos+iWidth+2,iYpos+iCY-(iWidth/2)+2);
			    Rectangle (hDC, iXpos+2,iYpos+iCY-(iWidth/2)-iWidth+2,iXpos+iCX-iCenter+2,iYpos+iCY-(iWidth/2)+2);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCY-(iCenter/2)+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos,iYpos,iXpos+iWidth,iYpos+iCY-(iWidth/2));
			    Rectangle (hDC, iXpos,iYpos+iCY-(iWidth/2)-iWidth,iXpos+iCX-iCenter,iYpos+iCY-(iWidth/2));

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCY-(iCenter/2);
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 23 - Down to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow23") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iCX-iWidth+2,iYpos+2,iXpos+iCX+2,iYpos+iCY-(iWidth/2)+2);
			    Rectangle (hDC, iXpos+iCenter+2,iYpos+iCY-(iWidth/2)-iWidth+2,iXpos+iCX+2,iYpos+iCY-(iWidth/2)+2);

			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCY+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCY-(iCenter/2)+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iCX-iWidth,iYpos,iXpos+iCX,iYpos+iCY-(iWidth/2));
			    Rectangle (hDC, iXpos+iCenter,iYpos+iCY-(iWidth/2)-iWidth,iXpos+iCX,iYpos+iCY-(iWidth/2));

			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCY-(iCenter/2);
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //**************************
                        //* Arrow 24 - Right Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow24") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+2,iYpos+2,iXpos+iCX-(iWidth/2)+2,iYpos+iWidth+2);
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth+2,iYpos+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY-iCenter+2);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCY-iCenter+2;
			    Points[2].x = iXpos+iCX-(iCenter/2)+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos,iYpos,iXpos+iCX-(iWidth/2),iYpos+iWidth);
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth,iYpos,iXpos+iCX-(iWidth/2),iYpos+iCY-iCenter);

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+iCX-(iCenter/2);
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 25 - Left Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow25") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+2,iXpos+iCX+2,iYpos+iWidth+2);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+2,iXpos+(iWidth/2)+iWidth+2,iYpos+iCY-iCenter+2);

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCY-iCenter+2;
			    Points[2].x = iXpos+(iCenter/2)+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos,iXpos+iCX,iYpos+iWidth);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos,iXpos+(iWidth/2)+iWidth,iYpos+iCY-iCenter);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+(iCenter/2);
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 26 - Right Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow26") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+2,iYpos+iCY-iWidth+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY+2);
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth+2,iYpos+iCenter+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY+2);

			    Points[0].x = iXpos+iCX-(iCenter/2)+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos,iYpos+iCY-iWidth,iXpos+iCX-(iWidth/2),iYpos+iCY);
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCenter,iXpos+iCX-(iWidth/2),iYpos+iCY);

			    Points[0].x = iXpos+iCX-(iCenter/2);
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 27 - Left Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow27") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+iCY-iWidth+2,iXpos+iCX+2,iYpos+iCY+2);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+iCenter+2,iXpos+(iWidth/2)+iWidth+2,iYpos+iCY+2);

			    Points[0].x = iXpos+(iCenter/2)+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+iCY-iWidth,iXpos+iCX,iYpos+iCY);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+iCenter,iXpos+(iWidth/2)+iWidth,iYpos+iCY);

			    Points[0].x = iXpos+(iCenter/2);
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }


                        //**************************
                        //* Arrow 30 - Up to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow30") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+(iWidth/2)+2,iXpos+(iWidth/2)+iWidth+2,iYpos+iCY-iCenter+2);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+(iWidth/2)+2,iXpos+iCX-iCenter+2,iYpos+iCenter-(iWidth/2)+2);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+(iCenter/2)+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCY-iCenter+2;
			    Points[2].x = iXpos+(iCenter/2)+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+(iWidth/2),iXpos+(iWidth/2)+iWidth,iYpos+iCY-iCenter);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+(iWidth/2),iXpos+iCX-iCenter,iYpos+iCenter-(iWidth/2));

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+(iCenter/2);
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+(iCenter/2);
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 31 - Up to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow31") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iCenter+2,iYpos+(iWidth/2)+2,iXpos+iCX-(iWidth/2)+2,iYpos+(iWidth/2)+iWidth+2);
			    Rectangle (hDC, iXpos+iCX-iWidth-(iWidth/2)+2,iYpos+(iWidth/2)+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY-iCenter+2);

			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+(iCenter/2)+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCY-iCenter+2;
			    Points[2].x = iXpos+iCX-(iCenter/2)+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iCenter,iYpos+(iWidth/2),iXpos+iCX-(iWidth/2),iYpos+(iWidth/2)+iWidth);
			    Rectangle (hDC, iXpos+iCX-iWidth-(iWidth/2),iYpos+(iWidth/2),iXpos+iCX-(iWidth/2),iYpos+iCY-iCenter);

			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+(iCenter/2);
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+iCX-(iCenter/2);
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 32 - Down to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow32") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+iCenter+2,iXpos+(iWidth/2)+iWidth+2,iYpos+iCY-(iWidth/2)+2);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+iCY-(iWidth/2)-iWidth+2,iXpos+iCX-iCenter+2,iYpos+iCY-(iWidth/2)+2);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCY-(iCenter/2)+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+(iCenter/2)+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+iCenter,iXpos+(iWidth/2)+iWidth,iYpos+iCY-(iWidth/2));
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+iCY-(iWidth/2)-iWidth,iXpos+iCX-iCenter,iYpos+iCY-(iWidth/2));

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCY-(iCenter/2);
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+(iCenter/2);
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 33 - Down to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow33") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iCX-iWidth-(iWidth/2)+2,iYpos+iCenter+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY-(iWidth/2)+2);
			    Rectangle (hDC, iXpos+iCenter+2,iYpos+iCY-(iWidth/2)-iWidth+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY-(iWidth/2)+2);

			    Points[0].x = iXpos+iCenter+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCY+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCY-(iCenter/2)+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+iCX-(iCenter/2)+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iCX-iWidth-(iWidth/2),iYpos+iCenter,iXpos+iCX-(iWidth/2),iYpos+iCY-(iWidth/2));
			    Rectangle (hDC, iXpos+iCenter,iYpos+iCY-(iWidth/2)-iWidth,iXpos+iCX-(iWidth/2),iYpos+iCY-(iWidth/2));

			    Points[0].x = iXpos+iCenter;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCY;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCY-(iCenter/2);
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+iCX-(iCenter/2);
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //**************************
                        //* Arrow 34 - Right Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow34") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+iCenter+2,iYpos+(iWidth/2)+2,iXpos+iCX-(iWidth/2)+2,iYpos+iWidth+(iWidth/2)+2);
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth+2,iYpos+(iWidth/2)+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY-iCenter+2);

			    Points[0].x = iXpos+iCX-iCenter+2;
			    Points[0].y = iYpos+iCY-iCenter+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCY-iCenter+2;
			    Points[2].x = iXpos+iCX-(iCenter/2)+2;
			    Points[2].y = iYpos+iCY+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+2;
			    Points[0].y = iYpos+(iCenter/2)+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+2;
			    Points[2].x = iXpos+iCenter+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+iCenter,iYpos+(iWidth/2),iXpos+iCX-(iWidth/2),iYpos+iWidth+(iWidth/2));
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth,iYpos+(iWidth/2),iXpos+iCX-(iWidth/2),iYpos+iCY-iCenter);

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+iCX-(iCenter/2);
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos+(iCenter/2);
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos;
			    Points[2].x = iXpos+iCenter;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 35 - Left Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow35") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+(iWidth/2),iXpos+iCX-iCenter,iYpos+iWidth+(iWidth/2));
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+(iWidth/2),iXpos+(iWidth/2)+iWidth,iYpos+iCY-iCenter);

			    Points[0].x = iXpos;
			    Points[0].y = iYpos+iCY-iCenter;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCY-iCenter;
			    Points[2].x = iXpos+(iCenter/2);
			    Points[2].y = iYpos+iCY;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

			    Points[0].x = iXpos+iCX-iCenter;
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+(iCenter/2);
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 36 - Right Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow36") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+2,iYpos+iCY-iWidth+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY+2);
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth+2,iYpos+iCenter+2,iXpos+iCX-(iWidth/2)+2,iYpos+iCY+2);

			    Points[0].x = iXpos+iCX-(iCenter/2)+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCX+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+iCX-iCenter+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos,iYpos+iCY-iWidth,iXpos+iCX-(iWidth/2),iYpos+iCY);
			    Rectangle (hDC, iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCenter,iXpos+iCX-(iWidth/2),iYpos+iCY);

			    Points[0].x = iXpos+iCX-(iCenter/2);
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCX;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos+iCX-iCenter;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }
                        //**************************
                        //* Arrow 37 - Left Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow37") == 0) {
			    POINT Points[3];
			    POINT *pPoints;

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));
                            HPEN hPenShaddow = CreatePen(PS_SOLID, 1, (COLORREF) RGB(20,20,20));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hShaddow);
                            SelectObject(hDC, hPenShaddow);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+iCY-iWidth+2,iXpos+iCX+2,iYpos+iCY+2);
			    Rectangle (hDC, iXpos+(iWidth/2)+2,iYpos+iCenter+2,iXpos+(iWidth/2)+iWidth+2,iYpos+iCY+2);

			    Points[0].x = iXpos+(iCenter/2)+2;
			    Points[0].y = iYpos+2;
			    Points[1].x = iXpos+iCenter+2;
			    Points[1].y = iYpos+iCenter+2;
			    Points[2].x = iXpos+2;
			    Points[2].y = iYpos+iCenter+2;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            SelectObject(hDC, hBrush);
                            SelectObject(hDC, hPen);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+iCY-iWidth,iXpos+iCX,iYpos+iCY);
			    Rectangle (hDC, iXpos+(iWidth/2),iYpos+iCenter,iXpos+(iWidth/2)+iWidth,iYpos+iCY);

			    Points[0].x = iXpos+(iCenter/2);
			    Points[0].y = iYpos;
			    Points[1].x = iXpos+iCenter;
			    Points[1].y = iYpos+iCenter;
			    Points[2].x = iXpos;
			    Points[2].y = iYpos+iCenter;
			    pPoints = Points;
			    Polygon (hDC, pPoints, 3);

                            DeleteObject(hPen);
                            DeleteObject(hPenShaddow);
                        }

                        //**************************
                        //* Arrow 40 - Hollow Up to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow40") == 0) {

			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos+iCY,NULL);
                            LineTo(hDC,iXpos,iYpos+(iWidth/2));
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+(iWidth/2));
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+(iCenter/2));
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+iWidth,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+iWidth,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 41 - Hollow Up to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow41") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos+iCX,iYpos+iCY,NULL);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+iCY);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+iCenter,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos,iYpos+(iCenter/2));
                            LineTo(hDC,iXpos+iCenter,iYpos);
                            LineTo(hDC,iXpos+iCenter,iYpos+(iWidth/2));
                            LineTo(hDC,iXpos+iCX,iYpos+(iWidth/2));
                            LineTo(hDC,iXpos+iCX,iYpos+iCY);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 42 - Hollow Down to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow42") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iXpos+iWidth,iYpos);
                            LineTo(hDC,iXpos+iWidth,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-(iCenter/2));
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos,iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 43 - Hollow Down to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow43") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos+iCX-iWidth,iYpos,NULL);
                            LineTo(hDC,iXpos+iCX,iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY-(iCenter/2));
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-iWidth,iYpos);

                            DeleteObject(hPen);
                        }

                        //**************************
                        //* Arrow 44 - Hollow Right Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow44") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos,NULL);
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos);
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX-(iCenter/2),iYpos+iCY);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+iWidth);
                            LineTo(hDC,iXpos,iYpos+iWidth);
                            LineTo(hDC,iXpos,iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 45 - Hollow Left Down
                        //**************************
                        if  (strcmp (szStyle, "Arrow45") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos+(iWidth/2),iYpos,NULL);
                            LineTo(hDC,iXpos+iCX,iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+iWidth);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+iWidth);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+(iCenter/2),iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+(iWidth/2),iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+(iWidth/2),iYpos);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 46 - Hollow Right Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow46") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos+iCY-iWidth,NULL);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCY-iWidth);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-(iCenter/2),iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY-iWidth);

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 47 - Hollow Left Up
                        //**************************
                        if  (strcmp (szStyle, "Arrow47") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos+iCX,iYpos+iCY-iWidth,NULL);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY);
                            LineTo(hDC,iXpos+(iWidth/2),iYpos+iCY);
                            LineTo(hDC,iXpos+(iWidth/2),iYpos+iCenter);
                            LineTo(hDC,iXpos,iYpos+iCenter);
                            LineTo(hDC,iXpos+(iCenter/2),iYpos);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+iCenter);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+iCY-iWidth);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-iWidth);

                            DeleteObject(hPen);
                        }

                        //**************************
                        //* Arrow 50 - Hollow Up to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow50") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos+(iWidth/2),iYpos+(iWidth/2),NULL);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+(iWidth/2));
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+(iCenter/2));
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+(iCenter/2),iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+(iWidth/2),iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+(iWidth/2),iYpos+(iWidth/2));

                            DeleteObject(hPen);
                        }
                        //**************************
                        //* Arrow 51 - Hollow Up to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow51") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos+(iCenter/2),NULL);
                            LineTo(hDC,iXpos+iCenter,iYpos);
                            LineTo(hDC,iXpos+iCenter,iYpos+(iWidth/2));
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos+(iWidth/2));
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX-(iCenter/2),iYpos+iCY);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+iCenter,iYpos+(iWidth/2)+iWidth);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos,iYpos+(iCenter/2));

                        }
                        //**************************
                        //* Arrow 52 - Hollow Down to Right
                        //**************************
                        if  (strcmp (szStyle, "Arrow52") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos+iCenter,NULL);
                            LineTo(hDC,iXpos+(iCenter/2),iYpos);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+iCenter);
                            LineTo(hDC,iXpos+(iWidth/2)+iWidth,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCX,iYpos+iCY-(iCenter/2));
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos+(iWidth/2),iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos+(iWidth/2),iYpos+iCenter);
                            LineTo(hDC,iXpos,iYpos+iCenter);
                        }
                        //**************************
                        //* Arrow 53 - Hollow Down to left
                        //**************************
                        if  (strcmp (szStyle, "Arrow53") == 0) {
			    int  iWidth;
			    int  iCenter;
   
                            HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF) RGB(iRed,iGreen,iBlue));

			    if  (iCX > iCY) {
			        iCenter = iCY / 2;
			    }
			    else {
			        iCenter = iCX / 2;
			    }

			    iWidth = iCenter / 2;

                            SelectObject(hDC, hPen);
                            MoveToEx(hDC,iXpos,iYpos+iCY-(iCenter/2),NULL);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-iCenter);
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCY-(iWidth/2)-iWidth);
                            LineTo(hDC,iXpos+iCX-(iWidth/2)-iWidth,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-iCenter,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-(iCenter/2),iYpos);
                            LineTo(hDC,iXpos+iCX,iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos+iCenter);
                            LineTo(hDC,iXpos+iCX-(iWidth/2),iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY-(iWidth/2));
                            LineTo(hDC,iXpos+iCenter,iYpos+iCY);
                            LineTo(hDC,iXpos,iYpos+iCY-(iCenter/2));

                        }

                        //**************************
                        //* Point
                        //**************************
                        if  (strcmp (szStyle, "Point") == 0) {
                            SetPixel(hDC,iXpos,iYpos,(COLORREF) RGB(iRed,iGreen,iBlue));
                        }

                        //**************************
                        //* Text
                        //**************************
                        if  ((strcmp (szStyle, "Text") == 0) ||
                             (strcmp (szStyle, "TextTilt") == 0)) {
                            LOGFONT LogFont;
                            HANDLE  hFont;

                            if  (strcmp (szFont, "Default") == 0) {
                                SetTextColor(hDC, (COLORREF) RGB(iRed,iGreen,iBlue));
                                SetBkMode (hDC, TRANSPARENT);
                                TextOut (hDC, iXpos, iYpos, szText, strlen(szText));
                            }
                            else {
                                memset (&LogFont,0,sizeof(LOGFONT));
                                LogFont.lfHeight = iCX;
                                LogFont.lfWidth = LogFont.lfHeight * .6;
                                lstrcpy (LogFont.lfFaceName, szFont);
                                if   (strcmp (szStyle, "TextTilt") == 0) {
                                     if  (iCY < 0) {
                                         iCY = 0;
                                     }
                                     if  (iCY > 360) {
                                         iCY = 360;
                                     }
                                     iCY = iCY * 10;
                                     LogFont.lfEscapement = iCY;
                                }
                                hFont = CreateFontIndirect(&LogFont);
                                SelectObject(hDC, hFont);
                                SetTextColor(hDC, (COLORREF) RGB(iRed,iGreen,iBlue));
                                SetBkMode (hDC, TRANSPARENT);
                                TextOut (hDC, iXpos, iYpos, szText, strlen(szText));

                                DeleteObject(hFont);
                            }
                        }

                        //**************************
                        //* Bitmap
                        //**************************
                        if  (strcmp (szStyle, "Bitmap") == 0) {

    		            HBITMAP       hBitmap, hOldBitmap;
                            HDC           hMemDC;
                            BITMAP        bm;     

    			    hBitmap = (HBITMAP)LoadImage( NULL, szFilename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    			    if  (hBitmap == NULL) {
       			        z999_open_bug ();
       			        sprintf (buff, "Error Loading Bitmap[%s]\n",szFilename);
       			        fputs (buff, file_debug_out);
    			    }
                            else {
      		            	GetObject(hBitmap, sizeof(BITMAP), &bm);
                  		hMemDC = CreateCompatibleDC(hDC);
                  		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

                                BitBlt(hDC, iXpos, iYpos, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCAND);

                  		SelectObject(hMemDC, hOldBitmap);
      		            	DeleteObject(hBitmap);
      		            	DeleteObject(hOldBitmap);
                                DeleteDC(hMemDC);
            		    }
                        }
                        //**************************
                        //* Bitmap Invert
                        //**************************
                        if  (strcmp (szStyle, "BitmapInvert") == 0) {

    		            HBITMAP       hBitmap, hOldBitmap;
                            HDC           hMemDC;
                            BITMAP        bm;     

    			    hBitmap = (HBITMAP)LoadImage( NULL, szFilename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    			    if  (hBitmap == NULL) {
       			        z999_open_bug ();
       			        sprintf (buff, "Error Loading Bitmap[%s]\n",szFilename);
       			        fputs (buff, file_debug_out);
    			    }
                            else {
      		            	GetObject(hBitmap, sizeof(BITMAP), &bm);
                  		hMemDC = CreateCompatibleDC(hDC);
                  		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

                                BitBlt(hDC, iXpos, iYpos, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCINVERT);

                  		SelectObject(hMemDC, hOldBitmap);
      		            	DeleteObject(hBitmap);
      		            	DeleteObject(hOldBitmap);
                                DeleteDC(hMemDC);
            		    }
                        }
                        //**************************
                        //* BitmapFit
                        //**************************
                        if  (strcmp (szStyle, "BitmapFit") == 0) {

    		            HBITMAP       hBitmap, hOldBitmap;
                            HDC           hMemDC;
                            BITMAP        bm;     

    			    hBitmap = (HBITMAP)LoadImage( NULL, szFilename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    			    if  (hBitmap == NULL) {
       			        z999_open_bug ();
       			        sprintf (buff, "Error Loading Bitmap[%s]\n",szFilename);
       			        fputs (buff, file_debug_out);
    			    }
                            else {
      		            	GetObject(hBitmap, sizeof(BITMAP), &bm);
                  		hMemDC = CreateCompatibleDC(hDC);
                  		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
 
                                if  (iCY == 0) {
                                    //********************************
                                    //* Maintain Bitmap Aspect Ratio
                                    //********************************
                                    double Height = bm.bmHeight;
                                    double Width = bm.bmWidth;
                                    double Aspect = Height / Width;
                                    iCY = Aspect * iCX;
                                }

                                StretchBlt(hDC, iXpos, iYpos, iCX, iCY, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);

                  		SelectObject(hMemDC, hOldBitmap);
      		            	if  (!DeleteObject(hBitmap)) {
       			            z999_open_bug ();
       			            sprintf (buff, "Error DeleteObject1\n");
       			            fputs (buff, file_debug_out);
                                }
      		            	if  (!DeleteObject(hOldBitmap)) {
       			            z999_open_bug ();
       			            sprintf (buff, "Error DeleteObject2\n");
       			            fputs (buff, file_debug_out);
                                }
                                DeleteDC(hMemDC);
            		    }
                        }
                        //**************************
                        //* BitmapFit Invert
                        //**************************
                        if  (strcmp (szStyle, "BitmapFitInvert") == 0) {

    		            HBITMAP       hBitmap, hOldBitmap;
                            HDC           hMemDC;
                            BITMAP        bm;     

    			    hBitmap = (HBITMAP)LoadImage( NULL, szFilename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
    			    if  (hBitmap == NULL) {
       			        z999_open_bug ();
       			        sprintf (buff, "Error Loading Bitmap[%s]\n",szFilename);
       			        fputs (buff, file_debug_out);
    			    }
                            else {
      		            	GetObject(hBitmap, sizeof(BITMAP), &bm);
                  		hMemDC = CreateCompatibleDC(hDC);
                  		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
 
                                if  (iCY == 0) {
                                    //********************************
                                    //* Maintain Bitmap Aspect Ratio
                                    //********************************
                                    double Height = bm.bmHeight;
                                    double Width = bm.bmWidth;
                                    double Aspect = Height / Width;
                                    iCY = Aspect * iCX;
                                }

                                StretchBlt(hDC, iXpos, iYpos, iCX, iCY, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCINVERT);

                  		SelectObject(hMemDC, hOldBitmap);
      		            	if  (!DeleteObject(hBitmap)) {
       			            z999_open_bug ();
       			            sprintf (buff, "Error DeleteObject1\n");
       			            fputs (buff, file_debug_out);
                                }
      		            	if  (!DeleteObject(hOldBitmap)) {
       			            z999_open_bug ();
       			            sprintf (buff, "Error DeleteObject2\n");
       			            fputs (buff, file_debug_out);
                                }
                                DeleteDC(hMemDC);
            		    }
                        }

                        DeleteObject(hBrush);
                        DeleteObject(hShaddow);
                    }
                }
                while (pPaintData->pNext);
            }

            EndPaint (hwnd, &PtStr);
            DeleteObject(hBlackPen);
            DeleteObject(hRedPen);
            break;
    }

}

/************************************************************************/
/*                                                                      */
/*                         End of program BEDIT5PM.c                    */
/*                                                                      */
/************************************************************************/
/* Formatted on 09/21/90 at 14:24:23 by Forrest Bentley's C-Format v1.4 */
