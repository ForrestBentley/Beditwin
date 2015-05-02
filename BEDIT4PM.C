/*************************************************************************/
/* Program          : BEDIT4PM.c                 v2.0                    */
/* Author           : Forrest Bentley                                    */
/* Date Written     : January 15, 1992                                   */
/*                                                                       */
/* Description      :                                                    */
/*     This program will process all window commands.                    */
/*                                                                       */
//*
//************************************************************************
//* Maintenance Log
//*
//* Date       Author            Description
//* ---------- ----------------- -----------------------------------------
//* 02/16/2014 Forrest Bentley   Fixed Enter beep in Entry Fields      
//* 07/06/2014 Forrest Bentley   Changed w780_timer to setting < 0 is 1/2 second 
//* 10/24/2014 Forrest Bentley   Don't require a good setfocus to 1st object on window (may be only PaintFile objects)
//* 01/05/2014 Forrest Bentley   Added up/down/right/left key press logic
//*
/*************************************************************************/
#include <windows.h>

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

#include "BEDIT.h"
#include "BEDIT4pm.h"
#include "BEDITvar.h"

#define WM_WINDOWINIT  WM_USER+777
#define WM_FOCUSSET    WM_USER+778
#define WM_FOCUSCHG    WM_USER+779
#define WM_PUSHBUTTON  WM_USER+780
#define WM_ENABLEENTRY WM_USER+781
#define WM_MASKENTRY   WM_USER+782

static char szBEDIT2nd [] = "Beditwin2";
char cOldTestChar;

/********************************************************************/
/* This module will :                                               */
/*     process WINDOW commands                                      */
/********************************************************************/
void w100_test_Beditwin () {
    Beditwin = 'y';
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process WINDOW commands                                      */
/********************************************************************/
void w100_window () {
    c120_parse_command ();
    if  ((strcmp (Command, "START") == 0) ||
         (strcmp (Command, "start") == 0)) {
        w110_window_start ();
        return;
    }
    if  ((strcmp (Command, "STOP") == 0) ||
         (strcmp (Command, "stop") == 0)) {
        w120_window_stop ();
        return;
    }

    if  ((strcmp (Command, "OPEN") == 0) ||
         (strcmp (Command, "open") == 0)) {
        w300_window_open ();
        return;
    }
    if  ((strcmp (Command, "SHADE") == 0) ||
         (strcmp (Command, "shade") == 0)) {
        w305_shade ();
        return;
    }
    if  ((strcmp (Command, "UNSHADE") == 0) ||
         (strcmp (Command, "unshade") == 0)) {
        w310_unshade ();
        return;
    }
    if  ((strcmp (Command, "CLOSE") == 0) ||
         (strcmp (Command, "close") == 0)) {
        w400_window_close ();
        return;
    }
    if  ((strcmp (Command, "EXISTS") == 0) ||
         (strcmp (Command, "exists") == 0)) {
        w450_object_exists ();
        return;
    }
    if  ((strcmp (Command, "MODIFY") == 0) ||
         (strcmp (Command, "modify") == 0)) {
        w500_modify ();
        return;
    }
    if  ((strcmp (Command, "CHECK") == 0) ||
         (strcmp (Command, "check") == 0)) {
        w505_check ();
        return;
    }
    if  ((strcmp (Command, "UNCHECK") == 0) ||
         (strcmp (Command, "uncheck") == 0)) {
        w506_uncheck ();
        return;
    }
    if  ((strcmp (Command, "OBTAIN") == 0) ||
         (strcmp (Command, "obtain") == 0)) {
        w510_obtain ();
        return;
    }
    if  ((strcmp (Command, "SHOW") == 0) ||
         (strcmp (Command, "show") == 0)) {
        w520_show ();
        return;
    }
    if  ((strcmp (Command, "HIDE") == 0) ||
         (strcmp (Command, "hide") == 0)) {
        w530_hide ();
        return;
    }
    if  ((strcmp (Command, "SETFOCUS") == 0) ||
         (strcmp (Command, "setfocus") == 0)) {
        w540_setfocus ();
        return;
    }
    if  ((strcmp (Command, "TITLE") == 0) ||
         (strcmp (Command, "title") == 0)) {
        w550_title ();
        return;
    }
    if  ((strcmp (Command, "PAINT") == 0) ||
         (strcmp (Command, "paint") == 0)) {
        c120_parse_command ();
        if  (strcmp (Command, "MainWindow") == 0) {
            InvalidateRect (BEDITframe, NULL, TRUE);
            return;
        }
        w580_find_object ();
	InvalidateRect (pObject->hwnd, NULL, TRUE);
        UpdateWindow (pObject->hwnd);
        return;
    }
    if  ((strcmp (Command, "ENABLE") == 0) ||
         (strcmp (Command, "enable") == 0)) {
        w560_enable ();
        return;
    }
    if  ((strcmp (Command, "DISABLE") == 0) ||
         (strcmp (Command, "disable") == 0)) {
        w570_disable ();
        return;
    }
    if  ((strcmp (Command, "MESSAGE") == 0) ||
         (strcmp (Command, "message") == 0)) {
        w590_message ();
        return;
    }
    if  ((strcmp (Command, "CENTER") == 0) ||
         (strcmp (Command, "center") == 0)) {
        w600_center ();
        return;
    }
    if  ((strcmp (Command, "CENTERDESKTOP") == 0) ||
         (strcmp (Command, "centerdesktop") == 0)) {
        w605_center_desktop ();
        return;
    }
    if  ((strcmp (Command, "CLEAR") == 0) ||
         (strcmp (Command, "clear") == 0)) {
        w610_clear ();
        return;
    }
    if  ((strcmp (Command, "INSERT") == 0) ||
         (strcmp (Command, "insert") == 0)) {
        w700_insert ();
        return;
    }
    if  ((strcmp (Command, "FILEINSERT") == 0) ||
         (strcmp (Command, "fileinsert") == 0)) {
        w705_fileinsert ();
        return;
    }
    if  ((strcmp (Command, "DELETE") == 0) ||
         (strcmp (Command, "delete") == 0)) {
        w710_delete ();
        return;
    }
    if  ((strcmp (Command, "SELECT") == 0) ||
         (strcmp (Command, "select") == 0)) {
        w720_select ();
        return;
    }
    if  ((strcmp (Command, "UNSELECT") == 0) ||
         (strcmp (Command, "unselect") == 0)) {
        w730_unselect ();
        return;
    }
    if  ((strcmp (Command, "POINTER") == 0) ||
         (strcmp (Command, "pointer") == 0)) {
        w740_pointer ();
        return;
    }

    if  ((strcmp (Command, "ACTION") == 0) ||
         (strcmp (Command, "action") == 0)) {
        strcpy (szIf1, "ACTION");
        w750_action ();
        return;
    }
    if  ((strcmp (Command, "SELECTACTION") == 0) ||
         (strcmp (Command, "selectaction") == 0)) {
        strcpy (szIf1, "SELECT");
        w750_action ();
        return;
    }
    if  ((strcmp (Command, "EDIT") == 0) ||
         (strcmp (Command, "edit") == 0)) {
        w760_edit ();
        return;
    }
    if  ((strcmp (Command, "BITMAP") == 0) ||
         (strcmp (Command, "bitmap") == 0)) {
        w765_bitmap ();
        return;
    }
    if  ((strcmp (Command, "FILEBITMAP") == 0) ||
         (strcmp (Command, "filebitmap") == 0)) {
//        w766_filebitmap ();
        c120_parse_command ();
        c120_parse_command ();
        return;
    }
    if  ((strcmp (Command, "PUSH") == 0) ||
         (strcmp (Command, "push") == 0)) {
        w770_push ();
        return;
    }
    if  ((strcmp (Command, "TIMER") == 0) ||
         (strcmp (Command, "timer") == 0)) {
        w780_timer ();
        return;
    }

    if  ((strcmp (Command, "GETFOCUS") == 0) ||
         (strcmp (Command, "getfocus") == 0)) {
        w790_getfocus ();
        return;
    }
    if  ((strcmp (Command, "GETFOCUSWIN") == 0) ||
         (strcmp (Command, "getfocuswin") == 0)) {
        w795_getfocuswin ();
        return;
    }
    if  ((strcmp (Command, "DEFAULT") == 0) ||
         (strcmp (Command, "default") == 0)) {
        w800_default ();
        return;
    }
    if  ((strcmp (Command, "SOUND") == 0) ||
         (strcmp (Command, "sound") == 0)) {
        w810_sound ();
        return;
    }
    if  ((strcmp (Command, "COLOR") == 0) ||
         (strcmp (Command, "color") == 0)) {
        w820_color ();
        return;
    }
    if  ((strcmp (Command, "BACKCOLOR") == 0) ||
         (strcmp (Command, "backcolor") == 0)) {
        w825_backcolor ();
        return;
    }

    if  ((strcmp (Command, "CONCEAL") == 0) ||
         (strcmp (Command, "conceal") == 0)) {
        w830_conceal ();
        return;
    }
    if  ((strcmp (Command, "UNCONCEAL") == 0) ||
         (strcmp (Command, "unconceal") == 0)) {
        w840_unconceal ();
        return;
    }
    if  ((strcmp (Command, "LISTFONT") == 0) ||
         (strcmp (Command, "listfont") == 0)) {
//        w850_listfont ();
        c120_parse_command ();
        return;
    }
    if  ((strcmp (Command, "FONT") == 0) ||
         (strcmp (Command, "font") == 0)) {
//        w860_font ();
        c120_parse_command ();
        c120_parse_command ();
        return;
    }
    if  ((strcmp (Command, "HWND") == 0) ||
         (strcmp (Command, "hwnd") == 0)) {
        w870_hwnd ();
        return;
    }
    if  ((strcmp (Command, "SIZE") == 0) ||
         (strcmp (Command, "size") == 0)) {
        w880_size ();
        return;
    }
    if  ((strcmp (Command, "DRAW") == 0) ||
         (strcmp (Command, "draw") == 0)) {
//        w950_draw ();
        c120_parse_command ();
        c120_parse_command ();
        return;
    }
    if  ((strcmp (Command, "MASK") == 0) ||
         (strcmp (Command, "mask") == 0)) {
        w960_mask ();
        return;
    }
    if  ((strcmp (Command, "MOUSEPOS") == 0) ||
         (strcmp (Command, "mousepos") == 0)) {
        w970_mousepos ();
        return;
    }
    if  ((strcmp (Command, "WINDOWPOS") == 0) ||
         (strcmp (Command, "windowpos") == 0)) {
        w980_windowpos ();
        return;
    }
    if  ((strcmp (Command, "AUTOTAB") == 0) ||
         (strcmp (Command, "autotab") == 0)) {
        c120_parse_command ();
        if  ((strcmp (Command, "ON") == 0) ||
             (strcmp (Command, "on") == 0)) {
            cAutotab = 'y';
        }
        else {
            if  ((strcmp (Command, "OFF") == 0) ||
                 (strcmp (Command, "off") == 0)) {
                cAutotab = 'n';
            }
            else {
                z010_syntax_error ();
            }
        }
        return;
    }
    if  ((strcmp (Command, "CREATEWINDOW") == 0) ||
         (strcmp (Command, "createwindow") == 0)) {
        w250_createwindow ();
        return;
    }
    if  ((strcmp (Command, "CREATECONTROL") == 0) ||
         (strcmp (Command, "createcontrol") == 0)) {
        w260_createcontrol ();
        return;
    }
    if  ((strcmp (Command, "DELETEWINDOW") == 0) ||
         (strcmp (Command, "deletewindow") == 0)) {
        w270_deletewindow ();
        return;
    }
    if  ((strcmp (Command, "DELETECONTROL") == 0) ||
         (strcmp (Command, "deletecontrol") == 0)) {
        w280_deletecontrol ();
        return;
    }
    if  ((strcmp (Command, "CLOSEWINDOWNAME") == 0) ||
         (strcmp (Command, "closewindowname") == 0)) {
        c120_parse_command ();
        if (Command[0] == Subs) {
           d580_find_variable ();
           strcpy (Command, pVariable->szString);
        }
        d580_find_variable ();
        pEnumVar = pVariable;
        EnumWindows (EnumWindowCloseProc, (LPARAM) 0);
        return;
    }
    if  ((strcmp (Command, "FOCUSWINDOWNAME") == 0) ||
         (strcmp (Command, "focuswindowname") == 0)) {
        c120_parse_command ();
        if (Command[0] == Subs) {
           d580_find_variable ();
           strcpy (Command, pVariable->szString);
        }
        d580_find_variable ();
        pEnumVar = pVariable;
        EnumWindows (EnumWindowFocusProc, (LPARAM) 0);
        return;
    }

    z010_syntax_error ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a WINDOW START                                       */
/********************************************************************/
void w110_window_start () {

    static char szBEDITClass [] = "Beditwin";

    float fConvx;
    float fConvy;
    int iNoClose = 0;

    x1 = 500;  /* Resource Number */


    if  ((memcmp (input_line + index1, "NOCLOSE", 7) == 0) ||
         (memcmp (input_line + index1, "noclose", 7) == 0)) {
        c120_parse_command ();
        iNoClose = 1;
    }

    //lpfnSubClass = MakeProcInstance ((FARPROC) w999_subclass_edit, hInstance);
    //if (!lpfnSubClass) {
    //   z999_open_bug ();
    //   sprintf (buff, "MakeProcInstance-Error creating subclass process\n");
    //   fputs (buff, file_debug_out);
    //   MessageBox (HWND_DESKTOP, buff, "Error", MB_OK);
    //}

    pPMCommand = pCommand;
    free (pPMCommand->szLine);
    pPMCommand->szLine = (char *) malloc (Max);
    if  (!pPMCommand->szLine) {
        strcpy (Error, "Bad resize WINDOW START!");
        z110_fatal ();
        return;
    }
    strcpy (pPMCommand->szLine, "");
    strcpy (input_line,"");
    index1 = 0;
    More_Commands = 'n';



    if  (!hPrevInstance) {
        wndclass.style           = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc     = BeditwinMainWindProc;
        wndclass.cbClsExtra      = 0;
        wndclass.cbWndExtra      = 0;
        wndclass.hInstance       = hInstance;
        wndclass.hIcon           = LoadIcon (NULL, IDI_APPLICATION);
//        wndclass.hCursor         = NULL /*LoadCursor (NULL, IDC_ARROW)*/;
        wndclass.hCursor         = LoadCursor (NULL, IDC_ARROW);
        wndclass.hbrBackground   = GetStockObject (WHITE_BRUSH);
        wndclass.lpszMenuName    = NULL;
        wndclass.lpszClassName   = szBEDITClass;
        RegisterClass (&wndclass);
    }

    if  (iNoClose) {
        if  (!hPrevInstance) {
            wndclass2.style           = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
            wndclass2.lpfnWndProc     = BeditwinSecondaryWindProc;
            wndclass2.cbClsExtra      = 0;
            wndclass2.cbWndExtra      = 0;
            wndclass2.hInstance       = hInstance;
            wndclass2.hIcon           = LoadIcon (NULL, IDI_APPLICATION);
            //wndclass2.hCursor         = NULL /*LoadCursor (NULL, IDC_ARROW)*/;
            wndclass2.hCursor         = LoadCursor (NULL, IDC_ARROW);
            wndclass2.hbrBackground   = GetStockObject (WHITE_BRUSH);
            wndclass2.lpszMenuName    = NULL;
            wndclass2.lpszClassName   = szBEDIT2nd;
            RegisterClass (&wndclass2);
         }
    }
    else {
         if  (!hPrevInstance) {
            wndclass2.style           = CS_HREDRAW | CS_VREDRAW;
            wndclass2.lpfnWndProc     = BeditwinSecondaryWindProc;
            wndclass2.cbClsExtra      = 0;
            wndclass2.cbWndExtra      = 0;
            wndclass2.hInstance       = hInstance;
            wndclass2.hIcon           = LoadIcon (NULL, IDI_APPLICATION);
            //wndclass2.hCursor         = NULL /*LoadCursor (NULL, IDC_ARROW)*/;
            wndclass2.hCursor         = LoadCursor (NULL, IDC_ARROW);
            wndclass2.hbrBackground   = GetStockObject (WHITE_BRUSH);
            wndclass2.lpszMenuName    = NULL;
            wndclass2.lpszClassName   = szBEDIT2nd;
            RegisterClass (&wndclass2);
         }
    }

    strcpy (szMainTitle, "BEDITWIN Main Window");

    w115_setup_menu ();
    w120_setup_windows ();

    BEDITframe = CreateWindow ((LPCTSTR)szBEDITClass,
			 (LPCTSTR)"BEDIT",
                         (DWORD)WS_OVERLAPPEDWINDOW,
                         (int) CW_USEDEFAULT,
                         (int) CW_USEDEFAULT,
                         (int) CW_USEDEFAULT,
                         (int) CW_USEDEFAULT,
                         (HWND) NULL,
			 (HMENU) hMenu,
                         (HINSTANCE) hInstance,
                         (LPVOID) NULL);
    BEDITclient = BEDITframe;



//    ShowWindow (hwnd, nCmdShow);
//    ShowWindow (hwnd, SW_SHOWMAXIMIZED);
   
    SetWindowText (BEDITframe, szMainTitle);
    GetWindowRect (GetDesktopWindow (), &rect);
    SetWindowPos (BEDITframe, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom,
                  SWP_SHOWWINDOW);

    /****************************************/
    /* Correct all Dlg Units to Win Units   */
    /****************************************/
    //fConvx = (float) rect.right;
    //fConvx = fConvx / 430;
    //fConvy = (float) rect.bottom;
    //fConvy = fConvy / 235;
    //pObject = &StartObject;
    //do  {
    //    if  (pObject->pNext) {
    //        pObject = pObject->pNext;
    //        pObject->x  = (short) (pObject->x  * fConvx);
    //        pObject->y  = (short) (pObject->y  * fConvy);
    //        pObject->cx = (short) (pObject->cx * fConvx);
    //        pObject->cy = (short) (pObject->cy * fConvy);
    //        if  (strcmp (pObject->szType, "WINDOW") == 0) {
    //            pObject->y = pObject->y - 20;
    //            pObject->cy = pObject->cy + 20;
    //        }
    //    }
    //}
    //while (pObject->pNext);
//    ShowWindow (BEDITframe, nCmdShow);
    ShowWindow (BEDITframe, SW_SHOWMAXIMIZED);
    UpdateWindow (BEDITframe);

    if  (cBeditDebug == 'y') {
        //*******************************
        //* Write the Beditsem.stp file
        //* with the HWND of the main window
        //*******************************
        FILE *BeditBugFile;
        char szBugOutput[100];
        
        BeditBugFile = fopen ("beditsem.stp", "w");
        if  (Beditwin == 'y') {
            sprintf(szBugOutput, "HWND:%ld\n", (long) BEDITframe);
        }
        fputs (szBugOutput, BeditBugFile);
        fclose (BeditBugFile);
    }

    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        if  (msg.message == WM_QUIT) {
            break;
        }
        DispatchMessage (&msg);
    }

//    MessageBox (HWND_DESKTOP, "Debug", "Unregister", MB_OK);

    UnregisterClass (szBEDITClass, hInstance);
    UnregisterClass (szBEDIT2nd, hInstance);

    strcpy (pCommand->szLine, "");
    index1 = 0;
    More_Commands = 'n';

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     setup the Main Menu                                          */
/********************************************************************/
void w115_setup_menu () {

    x2 = 100;          /* Menu Resources */
    //z999_open_bug ();
    //sprintf (buff, "Opening MenuFile\n", Command);
    //fputs (buff, file_debug_out);
    strcpy (szFileName, "MenuFile");
    z200_reset_file_position ();
    do  {
        if  (pDataFile->pPrev) {
            pDataFile = pDataFile->pPrev;
        }
    }
    while (pDataFile->pPrev);
    hMenu = (HMENU) CreateMenu ();
    do  {
        if  (pDataFile->pNext) {
	    pDataFile = pDataFile->pNext;
	    {
		int z1;
		for (z1 = 0; z1 < (int) strlen (pDataFile->szLine); z1++) {
		    if  (pDataFile->szLine [z1] == '~') {
			pDataFile->szLine [z1] = '&';
                    }
		}
	    }
            if  ((memcmp (pDataFile->szLine, "TITLE", 5) == 0) ||
                 (memcmp (pDataFile->szLine, "title", 5) == 0)) {
                strcpy (szMainTitle, pDataFile->szLine + 6);
                szMainTitle [strlen (szMainTitle) - 1] = 0;
            }
            if  ((memcmp (pDataFile->szLine, "MENU", 4) == 0) ||
                 (memcmp (pDataFile->szLine, "menu", 4) == 0)) {
                w116_submenu ();
            }
            if  ((memcmp (pDataFile->szLine, "ITEM", 4) == 0) ||
                 (memcmp (pDataFile->szLine, "item", 4) == 0)) {
                w117_menuitem ();
            }
            if  (memcmp (pDataFile->szLine, "----", 4) == 0) {
                w118_separator ();
            }
        }
    }
    while (pDataFile->pNext);

    if  (x2 > 100) {
	AppendMenu (hMenu, MF_POPUP, (unsigned int) hMenuPopup, szIf1);
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     setup a submenu                                              */
/********************************************************************/
void w116_submenu () {
    if  (x2 > 100) {
        AppendMenu (hMenu, MF_POPUP, (unsigned int) hMenuPopup, szIf1);
    }

    x2++;
    strcpy (szIf1, pDataFile->szLine);
    if  (strlen (szIf1) > 0) {
        szIf1 [strlen (szIf1) - 1] = 0;
    }
    strcpy (szIf2, szIf1 + 5);
    strcpy (szIf1, szIf2);
    hMenuPopup = CreateMenu ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     setup a menuitem                                             */
/********************************************************************/
void w117_menuitem () {
    x1++;

    strcpy (Command, pDataFile->szLine + 5);
    x3 = 0;
    do  {
        if  ((Command [x3] != 0) && (Command [x3] != ',')) {
            x3++;
        }
    }
    while ((Command [x3] != 0) && (Command [x3] != ','));
    if  (Command [x3] != ',') {
        strcpy (Error, "Err MENUITEM, ");
        strcat (Error, pDataFile->szLine);
        z110_fatal ();
        return;
    }
    Command [x3] = 0;
    if  ((strlen (Command + x3 + 1) < 1) || (strlen (Command + x3 + 1) > 99)) {
        strcpy (Error, "Err MENUITEM, ");
        strcat (Error, pDataFile->szLine);
        z110_fatal ();
        return;
    }
    strcpy (szIf2, Command + x3 + 1);
    szIf2 [strlen (szIf2) - 1] = 0;
    AppendMenu (hMenuPopup, MF_STRING, x1, szIf2);

    /********************************/
    /* Create the Object            */
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
        strcpy (Error, "Cr Obj");
        z110_fatal ();
        return;
    }
    memset (pNewObject, 0, sizeof (OBJECT));
    strcpy (pNewObject->szName, Command);
    strcpy (pNewObject->szType, "MSOBJ");
    strcpy (pNewObject->szWindow, "MAIN");
    pNewObject->id = (int) x1;

    pObject->pNext = pNewObject;
    pNewObject->pPrev = pObject;
    pNewObject->pNext = NULL;
    pNewObject->pNextWin = NULL;
    pNewObject->pNextWinObj = NULL;
    pNewObject->szDraw = (char *) malloc (5);
    if  (!pNewObject->szDraw) {
        strcpy (Error, "Bad Obj Cr Draw");
        z110_fatal ();
        return;
    }
    strcpy (pNewObject->szDraw, "");
    pObject = pNewObject;

    /********************************/
    /* Create the Variable          */
    /********************************/
    test = 'y';
    d580_find_variable ();
    test = 'n';
    if  (found != 'y') {
        z999_open_bug ();
        sprintf (buff, " (Error) Creating MENUITEM string [%s]!\n", Command);
        fputs (buff, file_debug_out);
        strcpy (input_line, Command);
        index1 = 0;
        More_Commands = 'y';
        d510_string ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     setup a separator                                            */
/********************************************************************/
void w118_separator () {
    AppendMenu (hMenuPopup, MF_SEPARATOR, 0, NULL);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a WINDOW STOP                                        */
/********************************************************************/
void w120_window_stop () {
    remove ("BEDITexec.bat");
    remove ("BEDITexec.sem");
    PostMessage (BEDITframe, WM_CLOSE, 0, 0L);
//    PostQuitMessage (0);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     setup the windows                                            */
/********************************************************************/
void w120_setup_windows () {

    FSCount = 0;
    strcpy (BEDITargs, ""); /* Used to carry Window Name to child objects */
    //z999_open_bug ();
    //sprintf (buff, "Opening WindowFile\n", Command);
    //fputs (buff, file_debug_out);
    strcpy (szFileName, "WindowFile");
    z200_reset_file_position ();
    do  {
        if  (pDataFile->pPrev) {
            pDataFile = pDataFile->pPrev;
        }
    }
    while (pDataFile->pPrev);
    pPrevObject = NULL;
    do  {
        if  (pDataFile->pNext) {
            pDataFile = pDataFile->pNext;
            if  ((memcmp (pDataFile->szLine, "WINDOW", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "PBOBJ1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "UDOBJ1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "CBOBJ1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "CBOBJA", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "RBOBJ1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "LBOBJM", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "LBOBJ1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "GBOBJ1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "STTXT1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "STTXTF", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "STTXTR", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "COOBJS", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "COOBJD", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "COOBJL", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "MLOBJ1", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "EFOBJL", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "EFOBJC", 6) == 0) ||
                 (memcmp (pDataFile->szLine, "EFOBJR", 6) == 0)) {
                w121_window ("");
            }
        }
    }
    while (pDataFile->pNext);
    if  (pObject) {
        if  (memcmp (pObject->szType, "RBOBJ", 5) == 0) {
            pObject->flStyle &= ~WS_GROUP;
            pObject->flStyle &= ~WS_TABSTOP;
        }
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     setup a window object                                        */
/********************************************************************/
void w121_window (char * szWindowName) {
    static int Windowsizey;
    OBJECT *pNextObject;

    {
	int z1;
	for (z1 = 0; z1 < (int) strlen (pDataFile->szLine); z1++) {
	    if  (pDataFile->szLine [z1] == '~') {
		pDataFile->szLine [z1] = '&';
            }
	}
    }
    pNextObject = NULL;
    /********************************/
    /* Find where to put the object */
    /********************************/    
    if  (strcmp (szWindowName, "") == 0) {
        //*********************
        //* Put object at end
        //*********************
        pObject = &StartObject;
        do  {
            if  (pObject->pNext) {
                pObject = pObject->pNext;
            }
        }
        while (pObject->pNext);
    }
    else {
        strcpy (BEDITargs, szWindowName);
        //*********************
        //* Find Window object
        //*********************
        pObject = &StartObject;
        do  {
            if  (pObject->pNext) {
                pObject = pObject->pNext;
                //{
                //    char szMsg[100];
                //    sprintf (szMsg, "Find Window [%s][%s][%s]", szWindowName, pObject->szType, pObject->szName);
                //    MessageBox (HWND_DESKTOP, szMsg, "Debug", MB_OK);
                //}
            }
        }
        while ((pObject->pNext) && (strcmp (pObject->szName, szWindowName) != 0)); 
        if  (strcmp (pObject->szName, szWindowName) != 0) {
            sprintf (Error, "Windowname not found[%s]", Command);
            z110_fatal ();
            return;
        }
        //*********************
        //* Find next window
        //*********************
        do  {
            if  (pObject->pNext) {
                pObject = pObject->pNext;
                //{
                //    char szMsg[100];
                //    sprintf (szMsg, "Find Next Window [%s][%s][%s]", szWindowName, pObject->szType, pObject->szName);
                //    MessageBox (HWND_DESKTOP, szMsg, "Debug", MB_OK);
                //}
            }
        }
        while ((pObject->pNext) && (strcmp (pObject->szWindow, szWindowName) == 0));
        

        if  (!pObject->pNext) {
            //************************************
            //* Insert after last object in list
            //************************************
            pNextObject = NULL;
        }
        else {
            if (pObject->pPrev) {
               //*********************************
               //* Insert before current WINDOW
               //*********************************
               pNextObject = pObject;
               pObject = pObject->pPrev;
            } 
            else {
               //*********************************
               //* Only 1 WINDOW, insert after top line
               //*********************************
                pNextObject = NULL;
            }
        }
    }

    /********************************/
    /* Create the Object            */
    /********************************/
    pNewObject = (OBJECT *) malloc (sizeof (OBJECT));
    if  (!pNewObject) {
        strcpy (Error, "Cr Win Obj!");
        z110_fatal ();
        return;
    }
    memset (pNewObject, 0, sizeof (OBJECT));
    pNewObject->hwndClient = (HWND) NULL;
    pNewObject->hwnd = (HWND) NULL;
    pNewObject->flFrameFlags = 0;
    pNewObject->flStyle = 0;
    pNewObject->pNextWin = NULL;
    pNewObject->pNextWinObj = NULL;
    strcpy (pNewObject->szEdit, "");
    strcpy (pNewObject->szName, "");
    strcpy (pNewObject->szType, "");
    strcpy (pNewObject->szWindow, "");
//    pNewObject->lFont = 0;
//    pNewObject->lFontSize = 0;
//    pNewObject->hbm = NULL;
    pNewObject->szDraw = (char *) malloc (5);
    if  (!pNewObject->szDraw) {
        strcpy (Error, "Bad Obj Cr Draw");
        z110_fatal ();
        return;
    }
    strcpy (pNewObject->szDraw, "");

    x2 = 0;
    /**********************************/
    /* Type                           */
    /**********************************/
    x3 = 0;
    do  {
        if  ((pDataFile->szLine [x2] != ',') &&
             (pDataFile->szLine [x2] > 30)) {
            pNewObject->szType [x3] = pDataFile->szLine [x2];
            x2++;
            x3++;
        }
    }
    while ((pDataFile->szLine [x2] != ',') &&
           (pDataFile->szLine [x2] > 30));
    if  (pDataFile->szLine [x2] != ',') {
        sprintf (Error, "Syntax Error WindowFile Type [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    pNewObject->szType [x3] = 0;
    /**********************************/
    /* Name                           */
    /**********************************/
    x3 = 0;
    x2++;
    do  {
        if  ((pDataFile->szLine [x2] != ',') &&
             (pDataFile->szLine [x2] > 30)) {
            pNewObject->szName [x3] = pDataFile->szLine [x2];
            x2++;
            x3++;
        }
    }
    while ((pDataFile->szLine [x2] != ',') &&
           (pDataFile->szLine [x2] > 30));
    if  (pDataFile->szLine [x2] != ',') {
        sprintf (Error, "Syntax Error in WindowFile Name [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    pNewObject->szName [x3] = 0;
    /**********************************/
    /* Posx                           */
    /**********************************/
    memset (temp, 0, 20);
    x3 = 0;
    x2++;
    do  {
        if  ((pDataFile->szLine [x2] != ',') &&
             (pDataFile->szLine [x2] > 30)) {
            temp [x3] = pDataFile->szLine [x2];
            x2++;
            x3++;
        }
    }
    while ((pDataFile->szLine [x2] != ',') &&
           (pDataFile->szLine [x2] > 30));
    if  (pDataFile->szLine [x2] != ',') {
        sprintf (Error, "Syntax Error in WindowFile Posx [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    Number = 0;
    Number = (long) strtol (temp, (char**) &p, 10);
//    if  (Number == 0) {
//        sprintf (Error, "Syntax Error in WindowFile Posx=0 [%s]!", pDataFile->szLine);
//        z110_fatal ();
//        return;
//    }
    if  (strcmp (pNewObject->szType, "WINDOW") == 0) {
        Number = Number - 2;
    }
    pNewObject->x = (short) Number;
    /**********************************/
    /* Posy                           */
    /**********************************/
    memset (temp, 0, 20);
    x3 = 0;
    x2++;
    do  {
        if  ((pDataFile->szLine [x2] != ',') &&
             (pDataFile->szLine [x2] > 30)) {
            temp [x3] = pDataFile->szLine [x2];
            x2++;
            x3++;
        }
    }
    while ((pDataFile->szLine [x2] != ',') &&
           (pDataFile->szLine [x2] > 30));
    if  (pDataFile->szLine [x2] != ',') {
        sprintf (Error, "Syntax Error in WindowFile Posy [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    Number = 0;
    Number = (long) strtol (temp, (char**)&p, 10);
//    if  (Number == 0) {
//        sprintf (Error, "Syntax Error in WindowFile Posy=0 [%s]!", pDataFile->szLine);
//        z110_fatal ();
//        return;
//    }
    if  (strcmp (pNewObject->szType, "WINDOW") == 0) {
	Number = Number - 2;
    }

    pNewObject->y = (short) Number;
    /**********************************/
    /* Sizex                          */
    /**********************************/
    memset (temp, 0, 20);
    x3 = 0;
    x2++;
    do  {
        if  ((pDataFile->szLine [x2] != ',') &&
             (pDataFile->szLine [x2] > 30)) {
            temp [x3] = pDataFile->szLine [x2];
            x2++;
            x3++;
        }
    }
    while ((pDataFile->szLine [x2] != ',') &&
           (pDataFile->szLine [x2] > 30));
    if  (pDataFile->szLine [x2] != ',') {
        sprintf (Error, "Syntax Error in WindowFile Sizex [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    Number = 0;
    Number = (long) strtol (temp, (char**)&p, 10);
    if  (Number == 0) {
        sprintf (Error, "Syntax Error in WindowFile Sizex=0 [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    if  (strcmp (pNewObject->szType, "WINDOW") == 0) {
        Number = Number + 4;
    }
    pNewObject->cx = (short) Number;
    /**********************************/
    /* Sizey                          */
    /**********************************/
    memset (temp, 0, 20);
    x3 = 0;
    x2++;
    do  {
        if  ((pDataFile->szLine [x2] != ',') &&
             (pDataFile->szLine [x2] > 30)) {
            temp [x3] = pDataFile->szLine [x2];
            x2++;
            x3++;
        }
    }
    while ((pDataFile->szLine [x2] != ',') &&
           (pDataFile->szLine [x2] > 30));
    if  (pDataFile->szLine [x2] != ',') {
        sprintf (Error, "Syntax Error in WindowFile Sizey [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    Number = 0;
    Number = (long) strtol (temp, (char**)&p, 10);
    if  (Number == 0) {
        sprintf (Error, "Syntax Error in WindowFile Sizey=0 [%s]!", pDataFile->szLine);
        z110_fatal ();
        return;
    }
    if  (strcmp (pNewObject->szType, "WINDOW") == 0) {
	Number = Number + 4;
        Windowsizey = (int) Number;
    }
    //***************************************************
    //* Convert OS/2 lower left to Windows Upper Left   *
    //***************************************************
    //if  (strcmp (pNewObject->szType, "WINDOW") != 0) {
    //    pNewObject->y = Windowsizey - (pNewObject->y + Number);
    //}
    pNewObject->cy = (short) Number;
    /**********************************/
    /* Text                           */
    /**********************************/
    x3 = 0;
    x2++;
    do  {
        if  ((pDataFile->szLine [x2] != ',') &&
             (pDataFile->szLine [x2] > 30)) {
            pNewObject->szText [x3] = pDataFile->szLine [x2];
            x2++;
            x3++;
        }
    }
    while ((pDataFile->szLine [x2] != ',') &&
           (pDataFile->szLine [x2] > 30));
    pNewObject->szText [x3] = 0;

    x1++;
    pNewObject->id = x1;

    pObject->pNext = pNewObject;
    pNewObject->pPrev = pObject;
    pNewObject->pNext = pNextObject;
    if  (pNextObject) {
        pNextObject->pPrev = pNewObject;
    }

    if  (strcmp (pNewObject->szType, "WINDOW") == 0) {

        /* Beditargs used to carry Window Name to child objects */
        strcpy (BEDITargs, pNewObject->szName);

        pNewObject->flFrameFlags = WS_OVERLAPPEDWINDOW;
        FSCount = 0;
        if  (pPrevObject) {
            if  (memcmp (pPrevObject->szType, "RBOBJ", 5) == 0) {
                pPrevObject->flStyle &= ~WS_GROUP;
                pPrevObject->flStyle &= ~WS_TABSTOP;
            }
        }
    }
    if  (strcmp (pNewObject->szType, "PBOBJ1") == 0) {
        if  (FSCount == 0) {
            FSCount = 1;
            pNewObject->flStyle = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP;
        }
        else {
            pNewObject->flStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP | WS_GROUP;
        }
    }
    if  (strcmp (pNewObject->szType, "UDOBJ1") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP | WS_GROUP;
    }
    if  (strcmp (pNewObject->szType, "CBOBJ1") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP | WS_GROUP | LBS_SORT;
    }
    if  (strcmp (pNewObject->szType, "CBOBJA") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP | WS_GROUP | LBS_SORT;
    }
    if  (strcmp (pNewObject->szType, "RBOBJ1") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_TABSTOP | WS_GROUP;
    }
    if  (strcmp (pNewObject->szType, "LBOBJM") == 0) {
	pNewObject->flStyle = WS_CHILD | WS_VISIBLE | LBS_EXTENDEDSEL | WS_TABSTOP | WS_GROUP | LBS_STANDARD | LBS_SORT | CBS_AUTOHSCROLL;
        if  (FixedFont > 0) {
            pNewObject->flStyle |= LBS_OWNERDRAWFIXED;
        }
    }
    if  (strcmp (pNewObject->szType, "LBOBJ1") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | LBS_STANDARD | WS_BORDER | LBS_SORT | CBS_AUTOHSCROLL;
        if  (FixedFont > 0) {
            //pNewObject->flStyle |= LBS_OWNERDRAWFIXED;
        }
    }
    if  (strcmp (pNewObject->szType, "GBOBJ1") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP;
    }
    if  (strcmp (pNewObject->szType, "STTXT1") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | WS_GROUP;
    }
    if  (strcmp (pNewObject->szType, "STTXTF") == 0) {
	pNewObject->flStyle = WS_CHILD | WS_VISIBLE | SS_BLACKFRAME | WS_GROUP;
    }
    if  (strcmp (pNewObject->szType, "STTXTR") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | SS_BLACKRECT | WS_GROUP;
    }
    if  (strcmp (pNewObject->szType, "COOBJS") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | CBS_SIMPLE | WS_GROUP | WS_TABSTOP | WS_BORDER;
    }
    if  (strcmp (pNewObject->szType, "COOBJD") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_GROUP | WS_TABSTOP | WS_BORDER | CBS_AUTOHSCROLL | CBS_SORT;
    }
    if  (strcmp (pNewObject->szType, "COOBJL") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_GROUP | WS_TABSTOP;
    }
    if  (strcmp (pNewObject->szType, "MLOBJ1") == 0) {
	pNewObject->flStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_GROUP | WS_TABSTOP | WS_BORDER;
    }
    if  (strcmp (pNewObject->szType, "EFOBJL") == 0) {
	pNewObject->flStyle = WS_CHILD | WS_VISIBLE | ES_LEFT | WS_TABSTOP | /*ES_MULTILINE |*/ ES_AUTOHSCROLL;
    }
    if  (strcmp (pNewObject->szType, "EFOBJC") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | ES_CENTER | WS_TABSTOP | ES_MULTILINE | ES_AUTOHSCROLL;
    }
    if  (strcmp (pNewObject->szType, "EFOBJR") == 0) {
        pNewObject->flStyle = WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_TABSTOP | ES_MULTILINE | ES_AUTOHSCROLL;
    }
    if  (pPrevObject) {
        if  ((memcmp (pPrevObject->szType, "RBOBJ", 5) == 0) &&
             (memcmp (pNewObject->szType, "RBOBJ", 5) == 0)) {
            pNewObject->flStyle &= ~WS_GROUP;
            pNewObject->flStyle &= ~WS_TABSTOP;
        }
    }

    if  (strcmp (BEDITargs, "") == 0) {
        strcpy (Error, "Bad WindowFile, Define WINDOW first!");
        z110_fatal ();
        return;
    }
    //*********************************
    //* Enlarge Entry Fields
    //*********************************
    //if  (memcmp (pNewObject->szType, "EFOBJ", 5) == 0) {
    //    pNewObject->y = pNewObject->y - 3;
    //    pNewObject->cy = pNewObject->cy + 5;
    //}
    /* Beditargs used to carry Window Name to child objects */
    if  (strcmp (szWindowName, "") == 0) {
        strcpy (pNewObject->szWindow, BEDITargs);
    }
    else {
        strcpy (pNewObject->szWindow, szWindowName);
    }
    pObject = pNewObject;
    pPrevObject = pNewObject;

    /********************************/
    /* Create the Variable          */
    /********************************/
    test = 'y';
    strcpy (Command, pObject->szName);
    d580_find_variable ();
    test = 'n';
    if  (found != 'y') {
        //z999_open_bug ();
        //sprintf (buff, "Creating Window Object string [%s]\n", Command);
        //fputs (buff, file_debug_out);
        strcpy (input_line, Command);
        index1 = 0;
        More_Commands = 'y';
        d510_string ();
        if  ((memcmp (pObject->szType, "STTXT", 5) == 0) ||
             (memcmp (pObject->szType, "GBOBJ", 5) == 0)) {
            free (pVariable->szString);
            pVariable->szString = (char *) malloc (strlen (pObject->szText) + 5);
            if  (!pVariable->szString) {
                strcpy (Error, "Assign text to obj var");
                z110_fatal ();
                return;
            }
            strcpy (pVariable->szString, pObject->szText);
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a Command                                            */
/********************************************************************/
void w200_proc_cmd () {

    char  cExecute;

    if  (test == 'u') {
        strcpy (Command, "WindowUp");
    }
    if  (test == 'd') {
        strcpy (Command, "WindowDown");
    }
    if  (test == 'l') {
        strcpy (Command, "WindowLeft");
    }
    if  (test == 'r') {
        strcpy (Command, "WindowRight");
    }

    test = 'y';
    if  ((strcmp (Command, "WindowF1") != 0) &&
         (strcmp (Command, "WindowF2") != 0) &&
         (strcmp (Command, "WindowF3") != 0) &&
         (strcmp (Command, "WindowF4") != 0) &&
         (strcmp (Command, "WindowF5") != 0) &&
         (strcmp (Command, "WindowF6") != 0) &&
         (strcmp (Command, "WindowF7") != 0) &&
         (strcmp (Command, "WindowF8") != 0) &&
         (strcmp (Command, "WindowF9") != 0) &&
         (strcmp (Command, "WindowF10") != 0) &&
         (strcmp (Command, "WindowF11") != 0) &&
         (strcmp (Command, "WindowF12") != 0) &&
         (strcmp (Command, "WindowMouse") != 0) &&
         (strcmp (Command, "WindowDown1") != 0) &&
         (strcmp (Command, "WindowDown2") != 0) &&
         (strcmp (Command, "WindowDown3") != 0) &&
         (strcmp (Command, "WindowUp1") != 0) &&
         (strcmp (Command, "WindowUp2") != 0) &&
         (strcmp (Command, "WindowUp3") != 0) &&
         (strcmp (Command, "WindowEscape") != 0) &&
         (strcmp (Command, "WindowUp") != 0) &&
         (strcmp (Command, "WindowDown") != 0) &&
         (strcmp (Command, "WindowLeft") != 0) &&
         (strcmp (Command, "WindowRight") != 0) &&
         (strcmp (Command, "WindowInsert") != 0) &&
         (strcmp (Command, "WindowDelete") != 0)) {
        strcpy (Command, pObject->szName);
    }
    d580_find_variable ();
    strcpy (Command, "");
    test = 'n';
    if  (found != 'y') {
        z999_open_bug ();
        sprintf (buff, "Bad locate command string [%s]!\n", Command);
        MessageBox (HWND_DESKTOP, buff, "Error", MB_OK);
        fputs (buff, file_debug_out);
        MessageBeep (MB_ICONHAND);
        return;
    }

    cExecute = 'y';
    if  (strcmp (pPMCommand->szLine, "") != 0) {
        pPMCommand->szLine [strlen (pPMCommand->szLine) - 13] = 0;
        cExecute = 'n';
    }

    if  (strcmp (pVariable->szAction, "") != 0) {
        if  ((int)(strlen (pPMCommand->szLine) + strlen (pVariable->szAction)) > Max) {
            strcpy (Error, "System Error, PM Action String Too Large!");
            z110_fatal ();
            return;
        }
        strcat (pPMCommand->szLine, pVariable->szAction);
    }
    else {
        if  (strcmp (pVariable->szString, "") == 0) {
            z999_open_bug ();
            sprintf (buff, "No Command Associated with variable [%s]\n", Command);
            fputs (buff, file_debug_out);
            //MessageBeep (MB_ICONHAND);
            //MessageBox (BEDITframe,
            //            "There isn't an Action associated with this Object!",
            //            "No Action Error",
            //            MB_APPLMODAL | MB_ICONHAND | MB_OK);
            strcpy (Command, "DefaultWindowProcess");
            test = 'y';
            d580_find_variable ();
            test = 'n';
            if (found != 'y') {
                return;
            }
        }
        if  ((int)(strlen (pPMCommand->szLine) + strlen (pVariable->szString)) > Max) {
            strcpy (Error, "System Error, PM Action String Too Large!");
            z110_fatal ();
            return;
        }
        strcat (pPMCommand->szLine, pVariable->szString);
    }
    strcat (pPMCommand->szLine, " *PM_CMD_DONE*");
    if  (cExecute == 'n') {
        if  (trace == 'x') {
            sprintf (buff, "     ***** Trace Expanded PM Action to [%s]\n", pPMCommand->szLine);
            fputs (buff, file_debug_out);
        }
        return;
    }
    if  (trace == 'x') {
        fputs ("*****************************************************\n", file_debug_out);
        sprintf (buff, "Trace PM Action [%s]\n", pPMCommand->szLine);
        fputs (buff, file_debug_out);
    }
    strcpy (input_line, pPMCommand->szLine);
    index1 = 0;
    More_Commands = 'y';
    strcpy (BEDITargs, "");
    do  {
        c120_parse_command ();
        if  ((More_Commands == 'y') &&
             (strcmp (Command, "*PM_CMD_DONE*") != 0)) {
            if  ((strcmp (Command, "EXIT") == 0) ||
                 (strcmp (Command, "exit") == 0)) {
                PostMessage (BEDITframe, WM_CLOSE, 0, 0);
                return;
            }
            c200_process_command ();
        }
        if  (More_Commands != 'y') {
            c110_proc_next_cmd ();
            More_Commands = 'y';
            index1 = 0;
            if  (eof_cmd_in == 'y') {
                strcpy (Error, "Premature EOF in Command File (No Return on PM CALL)!");
                z110_fatal ();
                return;
            }
        }
    }
    while (strcmp (Command, "*PM_CMD_DONE*") != 0);
    if  (sInterval != 0) {
        SetTimer (BEDITframe, 1, sInterval, NULL);
    }
    cMouse = 'y';
    More_Commands = 'n';
    index1 = 0;
    strcpy (input_line, "");
    strcpy (pPMCommand->szLine, "");
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Create a New Window                                          */
/********************************************************************/
void w250_createwindow () {
    char szWindowName [100];
    int  Posx;
    int  Posy;
    int  Poscx;
    int  Poscy;
    char szTitle[100];
    char szDataLine [500];

    DATAFILE *pOldDataFile;

    //*********************************
    //* Window Name
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strlen (Command) > 99) {
        sprintf (Error, "Window Name > 99 characters[%s]", Command);
        z110_fatal ();
        return;
    }
    strcpy (szWindowName, Command);
    //*********************************
    //* Posx
    //*********************************
    Posx = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Posx = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Posx = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Posx = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Posx < 1) {
        sprintf (Error, "Posx must be > 0[%i]", Posx);
        z110_fatal ();
        return;
    }
    //*********************************
    //* Posy
    //*********************************
    Posy = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Posy = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Posy = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Posy = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Posy < 1) {
        sprintf (Error, "Posy must be > 0[%i]", Posy);
        z110_fatal ();
        return;
    }
    //*********************************
    //* Poscx
    //*********************************
    Poscx = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Poscx = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Poscx = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Poscx = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Poscx < 1) {
        sprintf (Error, "Poscx must be > 0[%i]", Poscx);
        z110_fatal ();
        return;
    }
    //*********************************
    //* Poscy
    //*********************************
    Poscy = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Poscy = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Poscy = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Poscy = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Poscy < 1) {
        sprintf (Error, "Poscy must be > 0[%i]", Poscy);
        z110_fatal ();
        return;
    }
    //*********************************
    //* Title
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strlen (Command) > 99) {
        sprintf (Error, "Title > 99 characters[%s]", Command);
        z110_fatal ();
        return;
    }
    strcpy (szTitle, Command); 

    //*********************************
    //* Check for Duplicate Window Name
    //*********************************
    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  (strcmp (pObject->szName, szWindowName) == 0) {
                sprintf (Error, "Duplicate Window Name[%s]", szWindowName);
                z110_fatal ();
                return;
            }
        }
    }
    while (pObject->pNext);   

    //*********************************
    //* Create a DataFile Object for the Window
    //*********************************
    pOldDataFile = pDataFile;
    pDataFile = (DATAFILE *) malloc (sizeof (DATAFILE));
    if  (!pDataFile) {
        strcpy (Error, "Bad Create DATAFILE Object");
        z110_fatal ();
        return;
    }
    memset (pDataFile, 0, sizeof (DATAFILE));

    sprintf (szDataLine, "WINDOW,%s,%d,%d,%d,%d,%s", szWindowName, Posx, Posy, Poscx, Poscy, szTitle);
    pDataFile->szLine = (char *) malloc (strlen(szDataLine) + 5);
    if  (!pDataFile->szLine) {
        strcpy (Error, "Bad Create DATAFILE szLine");
        z110_fatal ();
        return;
    }
    strcpy (pDataFile->szLine, szDataLine);
    pDataFile->pNext = NULL;
    pDataFile->pPrev = NULL;
    pDataFile->Begin = 'y';
    w121_window ("");
    free (pDataFile->szLine);
    free (pDataFile);

    pDataFile = pOldDataFile;
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Create a New Control                                         */
/********************************************************************/
void w260_createcontrol () {
    char szWindowName  [100];
    char szControlName [100];
    int  Posx;
    int  Posy;
    int  Poscx;
    int  Poscy;
    char szControlType [10];
    char szControlTitle[100];
    char szDataLine    [500];

    DATAFILE *pOldDataFile;
    OBJECT   *pPrevObject;
    OBJECT   *pWindowObject;

    HWND     hwndFocus;

    //*********************************
    //* Window Name
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strlen (Command) > 99) {
        sprintf (Error, "Window Name > 99 characters[%s]", Command);
        z110_fatal ();
        return;
    }
    strcpy (szWindowName, Command);
    //*********************************
    //* Control Name
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strlen (Command) > 99) {
        sprintf (Error, "Control Name > 99 characters[%s]", Command);
        z110_fatal ();
        return;
    }
    strcpy (szControlName, Command);
    
    //*********************************
    //* Posx
    //*********************************
    Posx = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Posx = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Posx = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Posx = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Posx < 1) {
        sprintf (Error, "Posx must be > 0[%i]", Posx);
        z110_fatal ();
        return;
    }
    //*********************************
    //* Posy
    //*********************************
    Posy = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Posy = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Posy = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Posy = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Posy < 1) {
        sprintf (Error, "Posy must be > 0[%i]", Posy);
        z110_fatal ();
        return;
    }
    //*********************************
    //* Poscx
    //*********************************
    Poscx = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Poscx = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Poscx = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Poscx = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Poscx < 1) {
        sprintf (Error, "Poscx must be > 0[%i]", Poscx);
        z110_fatal ();
        return;
    }
    //*********************************
    //* Poscy
    //*********************************
    Poscy = 0;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (pVariable->Type == 'I') {
           Poscy = pVariable->Integer;
       }
       if  (pVariable->Type == 'S') {
           Poscy = (int) strtol (pVariable->szString, (char**)&p, 10);
       }
    }
    else {
        Poscy = (int) strtol (Command, (char**)&p, 10);
    }
    if  (Poscy < 1) {
        sprintf (Error, "Poscy must be > 0[%i]", Poscy);
        z110_fatal ();
        return;
    }

    //*********************************
    //* Type
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  ((strcmp (Command, "PBOBJ1") == 0) ||
         (strcmp (Command, "UDOBJ1") == 0) ||
         (strcmp (Command, "CBOBJ1") == 0) ||
         (strcmp (Command, "CBOBJA") == 0) ||
         (strcmp (Command, "RBOBJ1") == 0) ||
         (strcmp (Command, "LBOBJM") == 0) ||
         (strcmp (Command, "LBOBJ1") == 0) ||
         (strcmp (Command, "GBOBJ1") == 0) ||
         (strcmp (Command, "STTXT1") == 0) ||
         (strcmp (Command, "STTXTF") == 0) ||
         (strcmp (Command, "STTXTR") == 0) ||
         (strcmp (Command, "COOBJS") == 0) ||
         (strcmp (Command, "COOBJD") == 0) ||
         (strcmp (Command, "COOBJL") == 0) ||
         (strcmp (Command, "MLOBJ1") == 0) ||
         (strcmp (Command, "EFOBJL") == 0) ||
         (strcmp (Command, "EFOBJC") == 0) ||
         (strcmp (Command, "EFOBJR") == 0)) {
     }
     else {
        sprintf (Error, "Incorrect Type[%s]", Command);
        z110_fatal ();
        return;
    }
    strcpy (szControlType, Command);

    //*********************************
    //* Title
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strlen (Command) > 99) {
        sprintf (Error, "Title > 99 characters", Command);
        z110_fatal ();
        return;
    }
    strcpy (szControlTitle, Command);

    //*********************************
    //* Check for Duplicate Control Name
    //*********************************
    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  (strcmp (pObject->szName, szControlName) == 0) {
                sprintf (Error, "Duplicate Control Name[%s]", szControlName);
                z110_fatal ();
                return;
            }
        }
    }
    while (pObject->pNext);   

    //*********************************
    //* Find the Window Name
    //*********************************
    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
        }
    }
    while ((pObject->pNext) && (strcmp (pObject->szName, szWindowName) != 0));   
    if  (strcmp (pObject->szName, szWindowName) != 0) {
        sprintf (Error, "Windowname not found[%s]", Command);
        z110_fatal ();
        return;
    }
    pWindowObject = pObject;

    //*********************************
    //* Create a DataFile Object for the Window
    //*********************************
    pOldDataFile = pDataFile;
    pDataFile = (DATAFILE *) malloc (sizeof (DATAFILE));
    if  (!pDataFile) {
        strcpy (Error, "Bad Create DATAFILE Object");
        z110_fatal ();
        return;
    }
    memset (pDataFile, 0, sizeof (DATAFILE));

    sprintf (szDataLine, "%s,%s,%d,%d,%d,%d,%s", szControlType, szControlName, Posx, Posy, Poscx, Poscy, szControlTitle);
    pDataFile->szLine = (char *) malloc (strlen(szDataLine) + 5);
    if  (!pDataFile->szLine) {
        strcpy (Error, "Bad Create DATAFILE szLine");
        z110_fatal ();
        return;
    }
    strcpy (pDataFile->szLine, szDataLine);
    pDataFile->pNext = NULL;
    pDataFile->pPrev = NULL;
    pDataFile->Begin = 'y';
    w121_window (szWindowName);
    free (pDataFile->szLine);
    free (pDataFile);
    {
        OBJECT * pDumpObject;
        z999_open_bug ();
        //fputs ("------------------ Forward Object Dump --------------------\n", file_debug_out);
        pDumpObject = &StartObject;
        do  {
            if  (pDumpObject->pNext) {
                pDumpObject = pDumpObject->pNext; 
                sprintf (buff, "OBJECT [%s][%s][%s]\n", pDumpObject->szWindow, pDumpObject->szName,pDumpObject->szType);
                //fputs (buff, file_debug_out);
            }
        }
        while (pDumpObject->pNext);

        //fputs ("------------------ Backward Object Dump --------------------\n", file_debug_out);

        do  {
            sprintf (buff, "OBJECT [%s][%s][%s]\n", pDumpObject->szWindow, pDumpObject->szName,pDumpObject->szType);
            //fputs (buff, file_debug_out);
            if  (pDumpObject->pPrev) {
                pDumpObject = pDumpObject->pPrev;
            }
        }
        while (pDumpObject->pPrev);
        //fputs ("-----------------------------------------------------\n", file_debug_out);
    }

    //*********************************
    //* Add the object to the opened window
    //*********************************
    if  (pWindowObject->hwnd) {
        hwndFocus = w301_create_window_control (pWindowObject->hwnd, pObject, hwndFocus);
        if (hwndFocus) {
           SetFocus (hwndFocus);
        }
        else {
           SetFocus (pObject->hwnd);
        }
    }

    pDataFile = pOldDataFile;
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Delete a Window                                              */
/********************************************************************/
void w270_deletewindow () {
    char szWindowName [100];
    //*********************************
    //* Window Name
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strlen (Command) > 99) {
        sprintf (Error, "Window Name > 99 characters[%s]", Command);
        z110_fatal ();
        return;
    }
    strcpy (szWindowName, Command);
    w290_deleteobject (szWindowName);
    SetFocus (BEDITframe);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Delete a Contro                                              */
/********************************************************************/
void w280_deletecontrol () {
    char szControlName [100];

    //z999_open_bug ();
    //sprintf (buff, "w280_deletecontrol\n");
    //fputs (buff, file_debug_out);

    //*********************************
    //* Control Name
    //*********************************
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strlen (Command) > 99) {
        sprintf (Error, "Control Name > 99 characters[%s]", Command);
        z110_fatal ();
        return;
    }
    //z999_open_bug ();
    //sprintf (buff, "Delete Control [%s]\n", Command);
    //fputs (buff, file_debug_out);
    strcpy (szControlName, Command);
    w290_deleteobject (szControlName);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Delete an OBJECT                                             */
/********************************************************************/
void w290_deleteobject (char * szObjectName) {
    OBJECT * pWindowObject;

    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
        }
    }
    while ((pObject->pNext) && 
           (strcmp (pObject->szName, szObjectName) != 0)); 
    if  (strcmp (pObject->szName, szObjectName) != 0) {
        sprintf (Error, "Can't Find Object[%s]", szObjectName);
        z110_fatal ();
        return;
    }
    pWindowObject = pObject;
    //z999_open_bug ();
    //sprintf (buff, "Deleting Object [%s][%s]\n", pObject->szName, pObject->szType);
    //fputs (buff, file_debug_out);

    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        //z999_open_bug ();
        //sprintf (buff, "free single Object [%s][%s]\n", pObject->szName, pObject->szType);
        //fputs (buff, file_debug_out);
        w295_free_object();
    }
    else {
        //******************************
        //* Remove all Window Objects
        //******************************
        sprintf (buff, "free window Object [%s][%s]\n", pObject->szName, pObject->szType);
        fputs (buff, file_debug_out);
        w295_free_object();
        do  {
            pObject = pObject->pNext;
            if  (pObject) {
                if  (strcmp (pWindowObject->szWindow, pObject->szWindow) != 0) {
                    pObject = NULL;
                }
            }
            if  (pObject) {
                w295_free_object();
            }
        }
        while (pObject);
        //******************************
        //* Remove Window Object
        //******************************
        pObject = pWindowObject;
        w295_free_object();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Free One Object                                              */
/********************************************************************/
void w295_free_object() {
    OBJECT * pPrevObj;
    OBJECT * pNextOBJ;

    //z999_open_bug ();
    //sprintf (buff, "Free Object [%s][%s]\n", pObject->szWindow, pObject->szName);
    //fputs (buff, file_debug_out);

    free (pObject->szDraw);

    {
        OBJECT * pDumpObject;
        z999_open_bug ();
        //fputs ("----------------- Object Dump before Delete ----------\n", file_debug_out);
        pDumpObject = &StartObject;
        do  {
            if  (pDumpObject->pNext) {
                pDumpObject = pDumpObject->pNext; 
                sprintf (buff, "OBJECT [%s][%s][%s]\n", pDumpObject->szWindow, pDumpObject->szName,pDumpObject->szType);
                //fputs (buff, file_debug_out);
            }
        }
        while (pDumpObject->pNext);
        //fputs ("------------------ Backward Object Dump --------------------\n", file_debug_out);

        do  {
            sprintf (buff, "OBJECT [%s][%s][%s]\n", pDumpObject->szWindow, pDumpObject->szName,pDumpObject->szType);
            //fputs (buff, file_debug_out);
            if  (pDumpObject->pPrev) {
                pDumpObject = pDumpObject->pPrev;
            }
        }
        while (pDumpObject->pPrev);
        //fputs ("-----------------------------------------------------\n", file_debug_out);
    }

    if  (pObject->pPrev) {
        pPrevObject = pObject->pPrev;
    }
    else {
        pPrevObject = NULL;
    }
    if  (pObject->pNext) {
        pNextObject = pObject->pNext;
    }
    else {
        pNextObject = NULL;
    } 
    //****************************
    //* Set Prev Object Next Ptr
    //****************************
    if  (pNextObject) {
        if  (pPrevObject) {
            pPrevObject->pNext = pNextObject;
        }
    }
    else {
        if  (pPrevObject) {
            pPrevObject->pNext = NULL;
        }
    }
    //****************************
    //* Set Next Object Prev Ptr
    //****************************
    if  (pPrevObject) {
        if  (pNextObject) {
            pNextObject->pPrev = pPrevObject;
        }
    }
    else {
        if  (pNextObject) {
            pNextObject->pPrev = NULL;
        }
    }

    {
        OBJECT * pDumpObject;
        z999_open_bug ();
        //fputs ("----------------- Object Dump after Delete ----------\n", file_debug_out);
        pDumpObject = &StartObject;
        do  {
            if  (pDumpObject->pNext) {
                pDumpObject = pDumpObject->pNext; 
                sprintf (buff, "OBJECT [%s][%s][%s]\n", pDumpObject->szWindow, pDumpObject->szName,pDumpObject->szType);
                //fputs (buff, file_debug_out);
                //MessageBox (HWND_DESKTOP, buff, "debug", MB_OK);
            }
        }
        while (pDumpObject->pNext);
        //fputs ("------------------ Backward Object Dump --------------------\n", file_debug_out);

        do  {
            sprintf (buff, "OBJECT [%s][%s][%s]\n", pDumpObject->szWindow, pDumpObject->szName,pDumpObject->szType);
            //fputs (buff, file_debug_out);
            if  (pDumpObject->pPrev) {
                pDumpObject = pDumpObject->pPrev;
            }
        }
        while (pDumpObject->pPrev);
        //fputs ("-----------------------------------------------------\n", file_debug_out);
    }
    if  (pObject->hwnd) {
        //z999_open_bug ();
        //sprintf (buff, "Destroying Window [%s][%s]\n", pObject->szWindow, pObject->szName);
        //fputs (buff, file_debug_out);
        DestroyWindow (pObject->hwnd);
    }
    free (pObject);
    if  (pPrevObject) {
        pObject = pPrevObject;
    }
    else {
        if  (pNextObject) {
            pObject = pNextObject;
        }
        else {
            pObject = &StartObject;
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Open a Window                                                */
/********************************************************************/
void w300_window_open () {

    HWND hwndWindow;
    HWND hwndClient;
    HWND hwndFocus;
//    POINTL Pointl;
//    short Locx;
//    short Locy;
//    short Sizex;
//    short Sizey;
    OBJECT *pWindowObject;
    char szWindow [100];
    short sResource;
    OBJECT *pCheckListObject;
    char  cFirstPBOBJ;
    OBJECT *pOldWindow;
    OBJECT *pCreateObject;
    DWORD dParms;

    hwndFocus = NULL;
    /*****************************************************/
    /* Find the Window Object                            */
    /*****************************************************/
    WindowCount ++;
    c120_parse_command ();

    z999_open_bug ();
    //fputs ("-------------------- Object Dump --------------------\n", file_debug_out);
    pCreateObject = &StartObject;
    do  {
        if  (pCreateObject->pNext) {
            pCreateObject = pCreateObject->pNext;
            sprintf (buff, "OBJECT [%s][%s][%s]\n", pCreateObject->szWindow, pCreateObject->szName,pCreateObject->szType);
            //fputs (buff, file_debug_out);
        }
    }
    while (pCreateObject->pNext);
    //fputs ("-----------------------------------------------------\n", file_debug_out);
    //sprintf (buff, "Opening Window [%s]\n", Command);
    //fputs (buff, file_debug_out);

    pCreateObject = &StartObject;
    do  {
        if  (pCreateObject->pNext) {
            pCreateObject = pCreateObject->pNext;
        }
    }
    while ((pCreateObject->pNext) && (strcmp (pCreateObject->szName, Command) != 0));
    if  (strcmp (pCreateObject->szName, Command) != 0) {
        sprintf (Error, "Bad locate win obj to open [%s]", Command);
        z110_fatal ();
        return;
    }
    if  (pCreateObject->hwnd) {
        if  (IsWindowVisible (pCreateObject->hwnd)) {
            WindowCount--;
        }
        pOldWindow = pCreateObject;
        SetActiveWindow (pOldWindow->hwnd);
        ShowWindow (pCreateObject->hwnd, SW_SHOWNORMAL);
        SetWindowPos (pOldWindow->hwnd, HWND_TOP, 0, 0, 0, 0,
                         SWP_SHOWWINDOW);
        if  ((memcmp (input_line + index1, "SHADE", 5) == 0) ||
             (memcmp (input_line + index1, "shade", 5) == 0)) {
            c120_parse_command ();
        }
        return;
     }
//    pWindowObject = pCreateObject;
//    sResource = 8000;
//    pCheckListObject = pCreateObject;
//    do  {
//        if  (pCheckListObject->pNext) {
//            pCheckListObject = pCheckListObject->pNext;
//            if  (memcmp (pCheckListObject->szType, "LBOBJ", 5) == 0) {
//                sResource = 8001;
//            }
//        }
//    }
//    while ((pCheckListObject->pNext) &&
//           (strcmp (pCheckListObject->szWindow, pWindowObject->szWindow) == 0) &&
//           (sResource != 8001));

    strcpy (szWindow, Command);

    /*****************************************************/
    /* Create the Window                                 */
    /*****************************************************/
    dParms = (DWORD) /*WS_CHILDWINDOW |*/
                    	WS_OVERLAPPEDWINDOW |
 	            	WS_VISIBLE |
                    	WS_THICKFRAME |
		    	WS_SYSMENU |
		    	WS_CAPTION;
		    	//WS_MINIMIZEBOX |
		    	//WS_MAXIMIZEBOX;

    pCreateObject->hwnd = CreateWindow (
                    (LPCTSTR)szBEDIT2nd,
                    (LPCTSTR)pCreateObject->szName,
		    (DWORD) dParms,
                    (int) pCreateObject->x,
                    (int) pCreateObject->y,
                    (int) pCreateObject->cx,
		    (int) pCreateObject->cy,
                    (HWND) BEDITframe,
                    (HMENU) NULL,
                    (HINSTANCE) hInstance,
                    (LPVOID) NULL);

    if  (!pCreateObject->hwnd) {
        sprintf (Error, "Bad create 2nd Win [%s]", Command);
        z110_fatal ();
        return;
    }
    SetWindowPos (pCreateObject->hwnd, HWND_TOP, pCreateObject->x, pCreateObject->y, pCreateObject->cx, pCreateObject->cy,
                  SWP_SHOWWINDOW);

    SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
    hwndWindow = pCreateObject->hwnd;
    hwndClient = pCreateObject->hwnd;

    //Locx = pCreateObject->x;
    //Locy = pCreateObject->y;
    //Sizex = pCreateObject->cx;
    //Sizey = pCreateObject->cy;

    cFirstPBOBJ = 'y';
    //sprintf (buff, "Opening Window 3 [%s]\n", Command);
    //fputs (buff, file_debug_out);
    /*****************************************************/
    /* Create the Window child objects                   */
    /*****************************************************/

    //z999_open_bug ();
    //sprintf (buff, "Creating Window Objects for [%s]\n", szWindow);
    //fputs (buff, file_debug_out);

    do  {
        if  (pCreateObject->pNext) {
            pCreateObject = pCreateObject->pNext;
            if  (strcmp (pCreateObject->szWindow, szWindow) == 0) {
                //Locx = pCreateObject->x;
                //Locy = pCreateObject->y;
                //Sizex = pCreateObject->cx;
                //Sizey = pCreateObject->cy;

                if  (memcmp (pCreateObject->szType, "PBOBJ", 5) == 0) {
                    if  (cFirstPBOBJ == 'y') {
                        pCreateObject->flStyle |= BS_DEFPUSHBUTTON;
                        cFirstPBOBJ = 'n';
                    }
                    else {
                        pCreateObject->flStyle &= ~BS_DEFPUSHBUTTON;
                    }
                }
                hwndFocus = w301_create_window_control(hwndClient, pCreateObject, hwndFocus);

            }
        }
    }
    while ((pCreateObject->pNext) && (strcmp (pCreateObject->szWindow, szWindow) == 0));

    if  ((memcmp (input_line + index1, "SHADE", 5) == 0) ||
         (memcmp (input_line + index1, "shade", 5) == 0)) {
        c120_parse_command ();
    }
    else {
        if  (hwndWindow) {
            SetWindowPos (hwndWindow,
                          HWND_TOP,
                          pWindowObject->x,
                          pWindowObject->y,
                          pWindowObject->cx,
                          pWindowObject->cy,
                          SWP_SHOWWINDOW);

   	    ShowWindow (hwndWindow, SW_SHOWNORMAL);
	    UpdateWindow (hwndWindow);
        }
    }
    if  (hwndClient) {
        SetFocus (hwndClient);
    }
    if  (hwndFocus) {
	SetFocus (hwndFocus);
    }
    else {
        //z999_open_bug ();
        //strcpy (buff, "Bad set focus to 1st object on window\n");
        //fputs (buff, file_debug_out);
        //MessageBox (HWND_DESKTOP, buff, "Error", MB_OK);
        //MessageBeep (MB_ICONHAND);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Create one Window Control                                    */
/********************************************************************/
HWND w301_create_window_control (HWND hwndClient, OBJECT * pCreateObject, HWND hwndFocus) {
    short Locx = pCreateObject->x;
    short Locy = pCreateObject->y;
    short Sizex = pCreateObject->cx;
    short Sizey = pCreateObject->cy;
    HWND hwndFocus2;

    pCreateObject->hwnd = NULL;

    //z999_open_bug ();
    //sprintf (buff, "Creating Object [%s][%s][%s][%i][%i][%i][%i]\n", pCreateObject->szWindow, pCreateObject->szType, pCreateObject->szName,
    //                                                                 pCreateObject->x,pCreateObject->y,pCreateObject->cx,pCreateObject->cy);
    //fputs (buff, file_debug_out);

    if  (memcmp (pCreateObject->szType, "PBOBJ", 5) == 0) {
        pCreateObject->flStyle |= WS_TABSTOP;
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "BUTTON",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
        EnableWindow (pCreateObject->hwnd, TRUE);
        if (!hwndFocus) {
           hwndFocus = pCreateObject->hwnd;
        }
    }
    if  (memcmp (pCreateObject->szType, "UDOBJ", 5) == 0) {
        pCreateObject->flStyle |= WS_TABSTOP;
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR)"STATIC",
             (LPCTSTR)NULL,
             (DWORD) WS_CHILD | WS_VISIBLE | SS_BLACKFRAME | WS_GROUP/*pCreateObject->flStyle*/,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
             (LPVOID) NULL);
        EnableWindow (pCreateObject->hwnd, TRUE);
        SendMessage (pCreateObject->hwnd, WM_SETREDRAW, TRUE, 0L);
        if (!hwndFocus) {
           hwndFocus = pCreateObject->hwnd;
        }
    }
    if  (memcmp (pCreateObject->szType, "RBOBJ", 5) == 0) {
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "BUTTON",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
        EnableWindow (pCreateObject->hwnd, TRUE);
        strcpy (Command, pCreateObject->szName);
        d580_find_variable ();
        if  (strcmp (pVariable->szString, "Y") == 0) {
            SendMessage (pCreateObject->hwnd, BM_SETCHECK, 1, 0L);
        }
        else {
            SendMessage (pCreateObject->hwnd, BM_SETCHECK, 0, 0L);
        }
    }
    if  (memcmp (pCreateObject->szType, "EFOBJ", 5) == 0) {
        VARIABLE *pKeepVar;

        pKeepVar = pVariable;
        strcpy (Command, pCreateObject->szName);
        d580_find_variable ();
        if  (strcmp (pVariable->szEdit, "UPPER") == 0) {
            pCreateObject->flStyle &= ~ES_LOWERCASE;
            pCreateObject->flStyle |= ES_UPPERCASE;
        }
        if  (strcmp (pVariable->szEdit, "LOWER") == 0) {
            pCreateObject->flStyle &= ~ES_UPPERCASE;
            pCreateObject->flStyle |= ES_LOWERCASE;
        }
        pVariable = pKeepVar;
        pCreateObject->flStyle |= WS_TABSTOP;
        pCreateObject->flStyle |= WS_BORDER;

        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "EDIT",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        if (!hwndFocus) {
           hwndFocus = pCreateObject->hwnd;
        }

        EnableWindow (pCreateObject->hwnd, TRUE);
        strcpy (Command, pCreateObject->szName);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (buff, pVariable->szString);
        }
        else {
            sprintf (buff, "%d", pVariable->Integer);
        }
        SetWindowText (pCreateObject->hwnd, buff);
//        if  (pVariable->sSize != 0) {
//            WinSendMsg (pCreateObject->hwnd, EM_SETTEXTLIMIT,
//            MPFROMshort(pVariable->sSize), 0L);
//        }
    }
    if  (memcmp (pCreateObject->szType, "CBOBJ", 5) == 0) {
        pCreateObject->flStyle |= WS_TABSTOP;
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "BUTTON",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        if (!hwndFocus) {
           hwndFocus = pCreateObject->hwnd;
        }
        SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
        EnableWindow (pCreateObject->hwnd, TRUE);
        strcpy (Command, pCreateObject->szName);
        d580_find_variable ();
        if  (strcmp (pVariable->szString, "Y") == 0) {
            SendMessage (pCreateObject->hwnd, BM_SETCHECK, 1, 0L);
        }
        else {
            SendMessage (pCreateObject->hwnd, BM_SETCHECK, 0, 0L);
        }
    }
    if  (memcmp (pCreateObject->szType, "LBOBJ", 5) == 0) {
        pCreateObject->flStyle |= WS_TABSTOP;
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "LISTBOX",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        if (!hwndFocus) {
           hwndFocus = pCreateObject->hwnd;
        }
	SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
        EnableWindow (pCreateObject->hwnd, TRUE);
        SendMessage (pCreateObject->hwnd, WM_SETREDRAW, TRUE, 0L);
    }
    if  (memcmp (pCreateObject->szType, "GBOBJ", 5) == 0) {
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "BUTTON",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
    }
    if  (memcmp (pCreateObject->szType, "STTXT", 5) == 0) {
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "STATIC",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
    }
    if  (memcmp (pCreateObject->szType, "COOBJ", 5) == 0) {
        pCreateObject->flStyle |= WS_TABSTOP;
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "COMBOBOX",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
        if (!hwndFocus) {
           hwndFocus = pCreateObject->hwnd;
        }
	EnableWindow (pCreateObject->hwnd, TRUE);
        SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
    }
    if  (memcmp (pCreateObject->szType, "MLOBJ", 5) == 0) {
        pCreateObject->flStyle |= WS_TABSTOP;
        pCreateObject->hwnd = CreateWindow (
             (LPCTSTR) "EDIT",
             (LPCTSTR) "",
             (DWORD) pCreateObject->flStyle,
             (int) Locx,
             (int) Locy,
             (int) Sizex,
             (int) Sizey,
             (HWND) hwndClient,
             (HMENU) NULL,
             (HINSTANCE) hInstance,
	     (LPVOID) NULL);
	EnableWindow (pCreateObject->hwnd, TRUE);
        SetWindowText (pCreateObject->hwnd, pCreateObject->szText);
//        WinSendMsg (pCreateObject->hwnd, MLM_SETTEXTLIMIT,
//        MPFROMshort ((short)Max - 5), 0L);
    }
    if  (!pCreateObject->hwnd) {
        sprintf (Error, "Create obj hwnd [%s]", pCreateObject->szName);
        z110_fatal ();
        return (HWND) NULL;
    }
    SetWindowPos(pCreateObject->hwnd,
                 NULL,
                 Locx,             /*  Window Position x coordinate    */
                 Locy,             /*  Window Position y coordinate    */
                 Sizex,            /*  Window cx size      */
                 Sizey,            /*  Window cy size      */
                 SWP_NOZORDER);
    //***************************
    //* Subclass the Window
    //***************************
    pCreateObject->OldProc = (WNDPROC) 0;
    if  (pCreateObject->hwnd) {
        pCreateObject->OldProc = (WNDPROC)SetWindowLongPtr (pCreateObject->hwnd, GWL_WNDPROC, (LONG)w999_subclass_edit);
        if  (!pCreateObject->OldProc) {
            z999_open_bug ();
            sprintf (buff, "SetWindowLong Subclass Error\n", Command);
            fputs (buff, file_debug_out);
            //MessageBox (HWND_DESKTOP, buff, "Error", MB_OK);
        }
    }
    if  ((!hwndFocus) && (pCreateObject->flStyle & WS_TABSTOP)) {
        if  (pCreateObject->hwnd) {
	    hwndFocus = pCreateObject->hwnd;
        }
    }
    pCreateObject->cVisible = 'y';
    pCreateObject->cEnabled = 'y';
    hwndFocus2 = hwndFocus;
    return hwndFocus2;
}

/********************************************************************/
/* This module will :                                               */
/*     Shade a Window                                               */
/********************************************************************/
void w305_shade () {
    c120_parse_command ();
    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        strcpy (Error, "Must use Win obj in SHADE");
        z110_fatal ();
        return;
    }
    ShowWindow (pObject->hwnd, SW_HIDE);
    return;
}


/********************************************************************/
/* This module will :                                               */
/*     Unshade a Window                                             */
/********************************************************************/
void w310_unshade () {
    c120_parse_command ();
    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        strcpy (Error, "Must use Win obj in UNSHADE");
        z110_fatal ();
        return;
    }
    ShowWindow (pObject->hwnd, SW_SHOW);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Close a Window                                               */
/********************************************************************/
void w400_window_close () {
    c120_parse_command ();
    if  (strcmp (Command, "DEBUGHWND") == 0) {
        HWND DebugHWND;
        d580_find_variable ();
        DebugHWND = (HWND)(long int) strtol ((char*)pVariable->szString, (char**)&p, 10);
        PostMessage (DebugHWND, WM_CLOSE, 0L, 0L);
        return;
    }
    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        strcpy (Error, "Must use Win obj in CLOSE");
        z110_fatal ();
        return;
    }

//    if  (WindowCount > 0) {
//        WindowCount--;
//    }
    PostMessage (pObject->hwnd, WM_CLOSE, 0L, 0L);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Check to see if a window exists                              */
/********************************************************************/
void w450_object_exists () {
    char cExists;
    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    pObject = &StartObject;
    cExists = 'n';
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  (strcmp (pObject->szName, Command) == 0) {
                cExists = 'y';
                break;
            }
        }
    }
    while (pObject->pNext);  
    strcpy (Command, "status");
    d580_find_variable ();
    sprintf (pVariable->szString, "%c", cExists);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Modify an object's text (Var to Obj)                         */
/********************************************************************/
void w500_modify () {

    char  szWindow [100];
    int   X1;

    if  ((strcmp (Command, "MODIFY") == 0) ||
         (strcmp (Command, "modify") == 0)) {
        c120_parse_command ();
    }
    d580_find_variable ();
    w580_find_object ();
    if  (pObject->hwnd == NULL) {
        return;
    }

    //z999_open_bug ();
    //sprintf (buff, "Modify Object [%s] to [%s]\n", pVariable->szName, pVariable->szString);
    //fputs (buff, file_debug_out);

    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        if  (pVariable->Type == 'S') {
            strcpy (buff, pVariable->szString);
        }
        else {
            sprintf (buff, "%d", pVariable->Integer);
        }
        if  ((memcmp (pObject->szType, "EFOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "MLOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "COOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "PBOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "MSOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "STTXT", 5) == 0) ||
             (memcmp (pObject->szType, "GBOBJ", 5) == 0)) {
            SetWindowText (pObject->hwnd, buff);
            SendMessage (pObject->hwnd, WM_PAINT, TRUE, 0L);
            if  (memcmp (pObject->szType, "PBOBJ", 5) == 0) {
                strcpy (pObject->szText, buff);
            }
//            if  (memcmp (pObject->szType, "MSOBJ", 5) == 0) {
//                WinSendMsg (pObject->hwnd,
//                            MM_SETITEMTEXT,
//                            MPFROM2SHORT((SHORT) pObject->id, TRUE),
//                            MPFROMP(buff));
//                strcpy (pObject->szText, buff);
//            }
        }
        if  ((memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
            if  (strcmp (pVariable->szString, "Y") == 0) {
                SendMessage (pObject->hwnd, BM_SETCHECK,
                             1, NULL);
            }
            else {
                if  (strcmp (pVariable->szString, "N") == 0) {
                    SendMessage (pObject->hwnd, BM_SETCHECK,
                                0, NULL);
                }
                else {
                    SetWindowText (pObject->hwnd, buff);
                    strcpy (pObject->szText, buff);
                }
            }
        }
        return;
    }
    strcpy (szWindow, Command);
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  (strcmp (pObject->szWindow, szWindow) == 0) {
                strcpy (Command, pObject->szName);
                d580_find_variable ();
                if  (pVariable->Type == 'S') {
                    strcpy (buff, pVariable->szString);
                }
                else {
                    sprintf (buff, "%d", pVariable->Integer);
                }
                if  ((memcmp (pObject->szType, "EFOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "MLOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "COOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "STTXT", 5) == 0) ||
                     (memcmp (pObject->szType, "GBOBJ", 5) == 0)) {
		    SetWindowText (pObject->hwnd, buff);
                }
                if  ((memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
                    if  (strcmp (pVariable->szString, "Y") == 0) {
                        SendMessage (pObject->hwnd, BM_SETCHECK,
                                    1, NULL);
                    }
                    else {
                        if  (strcmp (pVariable->szString, "N") == 0) {
                            SendMessage (pObject->hwnd, BM_SETCHECK,
                                        0, NULL);
                        }
                        else {
			    SetWindowText (pObject->hwnd, buff);
                            strcpy (pObject->szText, buff);
                        }
                    }
                }
            }
        }
    }
    while ((pObject->pNext) &&
           (strcmp (pObject->szWindow, szWindow) == 0));
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Check a Menu Selection                                       */
/********************************************************************/
void w505_check () {

    c120_parse_command ();
    d580_find_variable ();
    w580_find_object ();
    if  (memcmp (pObject->szType, "MSOBJ", 5) != 0) {
        strcpy (Error, "Must use MSOBJ for CHECK");
        z110_fatal ();
    }
    CheckMenuItem (hMenu, pObject->id, MF_CHECKED);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     UN-Check a Menu Selection                                    */
/********************************************************************/
void w506_uncheck () {

    c120_parse_command ();
    d580_find_variable ();
    w580_find_object ();
    if  (memcmp (pObject->szType, "MSOBJ", 5) != 0) {
        strcpy (Error, "Must use MSOBJ for UNCHECK");
        z110_fatal ();
    }
    CheckMenuItem (hMenu, pObject->id, MF_UNCHECKED);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Obtain an object's text (Obj to Var)                         */
/********************************************************************/
void w510_obtain () {

    char  szWindow [100];
    int usChecked;
    int    usX;
    int    X1;
    static int MIndex;
    int    Items;

    if  ((strcmp (Command, "OBTAIN") == 0) ||
         (strcmp (Command, "obtain") == 0)) {
        c120_parse_command ();
    }
    d580_find_variable ();
    w580_find_object ();
    if  (pObject->hwnd == NULL) {
        return;
    }
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
//        memset (buff, 0, Max);
        if  ((memcmp (pObject->szType, "EFOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "MLOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "COOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "STTXT", 5) == 0) ||
             (memcmp (pObject->szType, "GBOBJ", 5) == 0)) {
            GetWindowText (pObject->hwnd, buff, Max);
            if  (pVariable->Type == 'S') {
                free (pVariable->szString);
                pVariable->szString = (char *) malloc (strlen (buff) + 5);
                if  (!pVariable->szString) {
                    sprintf (buff, "Bad store obtained text for [%s]", Command);
                    z110_fatal ();
                    return;
                }
                strcpy (pVariable->szString, buff);
            }
            else {
                Number = 0;
                Number = strtol (buff, (char**)&p, 10);
                pVariable->Integer = (int) Number;
            }
        }
        if  ((memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
             (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
            usChecked = (int) SendMessage (pObject->hwnd, BM_GETCHECK,
                                      0, 0L);
            if  (usChecked != 0) {
                strcpy (pVariable->szString, "Y");
            }
            else {
                strcpy (pVariable->szString, "N");
            }
        }
        if  (memcmp (pObject->szType, "LBOBJ", 5) == 0) {
            Items = (int) SendMessage (pObject->hwnd, LB_GETCOUNT,
                                       0, 0L);
            if  (pVariable->Type != 'S') {
                sprintf (Error, "Must use String for LBOBJ [%s]", pObject->szName);
                z110_fatal ();
                return;
            }
            if  (strcmp (pVariable->szString, "COUNT") == 0) {
                usX = Items;
                sprintf (buff, "%d", (int) usX);
            }
            else {
                if  (strcmp (pVariable->szString, "INDEX") == 0) {
                    usX =  (int) SendMessage (pObject->hwnd, LB_GETCURSEL,
                                                    0, 0L);
                    if  (usX == LB_ERR) {
                        usX = 0;
                    }
                    else {
                        usX++;
                    }
                    sprintf (buff, "%d", (int) usX);
                }
                else {
                    if  (memcmp (pVariable->szString, "INDEX=", 6) == 0) {
                        Number = 0;
                        Number = strtol (pVariable->szString+6, (char**)&p, 10);
                        usX = (short) Number;
                        if  (usX > 0) {
                            usX--;
                        }
                    }
                    else {
                        if  (strcmp (pVariable->szString, "NEXT") == 0) {
                            if  (memcmp (pObject->szType, "LBOBJM", 6) == 0) {
                                char cDone;

                                cDone = 'n';
                                MIndex++;
                                do  {
                                    usX =  (int) SendMessage (pObject->hwnd, LB_GETSEL,
                                                              (DWORD) MIndex, 0L);
                                    if  ((usX > 0) &&
                                         (MIndex < Items)) {
                                        usX = MIndex;
                                        cDone = 'y';
                                    }
                                    else {
                                        usX = LB_ERR;
                                        MIndex++;
                                    }
                                }
                                while ((MIndex < Items) &&
                                       (cDone != 'y'));

                            }
                            else {
                                usX =  (int) SendMessage (pObject->hwnd, LB_GETCURSEL,
                                                        0, 0L);
                            }
                        }
                        else {
                            if  (memcmp (pObject->szType, "LBOBJM", 6) == 0) {
                                char cDone;

                                MIndex = 0;
                                cDone = 'n';
                                do  {
                                    usX =  (int) SendMessage (pObject->hwnd, LB_GETSEL,
                                                              (DWORD) MIndex, 0L);
                                    if  ((usX > 0) &&
                                         (MIndex < Items)) {
                                        usX = MIndex;
                                        cDone = 'y';
                                    }
                                    else {
                                        usX = LB_ERR;
                                        MIndex++;
                                    }
                                }
                                while ((MIndex < Items) &&
                                       (cDone != 'y'));
                            }
                            else {
                                usX =  (int) SendMessage (pObject->hwnd, LB_GETCURSEL,
                                                        0, 0L);
                            }
                        }

                    }
                    if  (usX + 1 > Items) {
                        usX = LB_ERR;
                    }
                    if  (usX == LB_ERR) {
                        strcpy (buff, "");
                        pVariable->Integer = 0;
                    }
                    else {
                        pVariable->Integer = (int) usX;
                        SendMessage (pObject->hwnd, LB_GETTEXT,
                                     usX, (LONG) (LPSTR) buff);
                    }
                }
            }
            free (pVariable->szString);
            pVariable->szString = (char *) malloc (strlen (buff) + 5);
            if  (!pVariable->szString) {
                sprintf (buff, "Bad store obtained text for [%s]", Command);
                z110_fatal ();
                return;
            }
            strcpy (pVariable->szString, buff);
        }

        return;
    }
    strcpy (szWindow, Command);
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  (strcmp (pObject->szWindow, szWindow) == 0) {
                strcpy (Command, pObject->szName);
                d580_find_variable ();
//                memset (buff, 0, Max);
                if  ((memcmp (pObject->szType, "EFOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "MLOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "COOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "STTXT", 5) == 0) ||
                     (memcmp (pObject->szType, "GBOBJ", 5) == 0)) {
                    GetWindowText (pObject->hwnd, buff, Max);
                    if  (pVariable->Type == 'S') {
                        free (pVariable->szString);
                        pVariable->szString = (char *) malloc (strlen (buff) + 5);
                        if  (!pVariable->szString) {
                            sprintf (buff, "Bad store obtained text for [%s]", Command);
                            z110_fatal ();
                            return;
                        }
                        strcpy (pVariable->szString, buff);
                    }
                    else {
                        Number = 0;
                        Number = strtol (buff, (char**)&p, 10);
                        pVariable->Integer = (int) Number;
                    }
                }
                if  ((memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
                    usChecked = SendMessage (pObject->hwnd, BM_GETCHECK,
                                              0, 0L);
                    if  (usChecked != 0) {
                        strcpy (pVariable->szString, "Y");
                    }
                    else {
                        strcpy (pVariable->szString, "N");
                    }
                }
                if  (memcmp (pObject->szType, "LBOBJ", 5) == 0) {
                    Items = (int) SendMessage (pObject->hwnd, LB_GETCOUNT,
                                               0, 0L);
                    if  (pVariable->Type != 'S') {
                        sprintf (Error, "Must use String for LBOBJ [%s]", pObject->szName);
                        z110_fatal ();
                        return;
                    }
                    if  (strcmp (pVariable->szString, "COUNT") == 0) {
                        usX = Items;
                        sprintf (buff, "%d", (int) usX);
                    }
                    else {
                        if  (strcmp (pVariable->szString, "INDEX") == 0) {
                            usX =  (int) SendMessage (pObject->hwnd, LB_GETCURSEL,
                                                            0, 0L);
                            if  (usX == LB_ERR) {
                                usX = 0;
                            }
                            else {
                                usX++;
                            }
                            sprintf (buff, "%d", (int) usX);
                        }
                        else {
                            if  (memcmp (pVariable->szString, "INDEX=", 6) == 0) {
                                Number = 0;
                                Number = strtol (pVariable->szString+6, (char**)&p, 10);
                                usX = (short) Number;
                                if  (usX > 0) {
                                    usX--;
                                }
                            }
                            else {
                                if  (strcmp (pVariable->szString, "NEXT") == 0) {
                                    if  (memcmp (pObject->szType, "LBOBJM", 6) == 0) {
                                        char cDone;

                                        cDone = 'n';
                                        MIndex++;
                                        do  {
                                            usX =  (int) SendMessage (pObject->hwnd, LB_GETSEL,
                                                                      (DWORD) MIndex, 0L);
                                            if  ((usX > 0) &&
                                                 (MIndex < Items)) {
                                                usX = MIndex;
                                                cDone = 'y';
                                            }
                                            else {
                                                usX = LB_ERR;
                                                MIndex++;
                                            }
                                        }
                                        while ((MIndex < Items) &&
                                               (cDone != 'y'));

                                    }
                                    else {
                                        usX =  (int) SendMessage (pObject->hwnd, LB_GETCURSEL,
                                                                0, 0L);
                                    }
                                }
                                else {
                                    if  (memcmp (pObject->szType, "LBOBJM", 6) == 0) {
                                        char cDone;

                                        MIndex = 0;
                                        cDone = 'n';
                                        do  {
                                            usX =  (int) SendMessage (pObject->hwnd, LB_GETSEL,
                                                                      (DWORD) MIndex, 0L);
                                            if  ((usX > 0) &&
                                                 (MIndex < Items)) {
                                                usX = MIndex;
                                                cDone = 'y';
                                            }
                                            else {
                                                usX = LB_ERR;
                                                MIndex++;
                                            }
                                        }
                                        while ((MIndex < Items) &&
                                               (cDone != 'y'));
                                    }
                                    else {
                                        usX =  (int) SendMessage (pObject->hwnd, LB_GETCURSEL,
                                                                0, 0L);
                                    }
                                }

                            }
                            if  (usX + 1 > Items) {
                                usX = LB_ERR;
                            }
                            if  (usX == LB_ERR) {
                                strcpy (buff, "");
                                pVariable->Integer = 0;
                            }
                            else {
                                pVariable->Integer = (int) usX;
                                SendMessage (pObject->hwnd, LB_GETTEXT,
                                             usX, (LONG) (LPSTR) buff);
                            }
                        }
                    }
                    free (pVariable->szString);
                    pVariable->szString = (char *) malloc (strlen (buff) + 5);
                    if  (!pVariable->szString) {
                        sprintf (buff, "Bad store obtained text for [%s]", Command);
                        z110_fatal ();
                        return;
                    }
                    strcpy (pVariable->szString, buff);
                }
            }
        }
    }
    while ((pObject->pNext) &&
           (strcmp (pObject->szWindow, szWindow) == 0));
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     show an object (Visible)                                     */
/********************************************************************/
void w520_show () {
    long ulStyle;

    c120_parse_command ();
    if  (strcmp (Command, "MAIN") == 0) {
        ShowWindow (BEDITframe, SW_SHOW);
        return;
    }
        
    w580_find_object ();

    ShowWindow (pObject->hwnd, SW_SHOW);
    ulStyle = GetWindowLong (pObject->hwnd, GWL_STYLE);
    if  ((memcmp (pObject->szType, "STTXT", 5) != 0) &&
         (memcmp (pObject->szType, "GBOBJ", 5) != 0)) {
        if  (pObject->flStyle & WS_GROUP) {
            ulStyle |= WS_TABSTOP;
        }
        pObject->flStyle = ulStyle;
    }
    SetWindowLong (pObject->hwnd, GWL_STYLE, ulStyle);

    pObject->cVisible = 'y';
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     hide an object (Invisible)                                   */
/********************************************************************/
void w530_hide () {
    long ulStyle;

    //z999_open_bug ();
    //fputs ("w530_hide\n", file_debug_out);
    c120_parse_command ();
    if  (strcmp (Command, "MAIN") == 0) {
        //z999_open_bug ();
        //fputs ("Hiding main window\n", file_debug_out);
        ShowWindow (BEDITframe, SW_HIDE);
        return;
    }

    w580_find_object ();

    ShowWindow (pObject->hwnd, SW_HIDE);
    ulStyle = GetWindowLong (pObject->hwnd, GWL_STYLE);
    ulStyle &= ~WS_TABSTOP;
    pObject->flStyle = ulStyle;
    SetWindowLong (pObject->hwnd, GWL_STYLE, ulStyle);

    pObject->cVisible = 'n';
    if  (memcmp (pObject->szType, "MSOBJ", 5) == 0) {
        DeleteMenu (hMenu, pObject->id, MF_BYPOSITION);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     set focus to an object                                       */
/********************************************************************/
void w540_setfocus () {
    short sId;
    c120_parse_command ();
    if  (strcmp (Command, "DEBUGHWND") == 0) {
        HWND DebugHWND;
        d580_find_variable ();
        DebugHWND = (HWND)(long int) strtol ((char*)pVariable->szString, (char**)&p, 10);
        SetForegroundWindow (DebugHWND);
        return;
    }
    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") == 0) {
        ShowWindow (pObject->hwnd, SW_SHOWNORMAL);
        //SetActiveWindow (pObject->hwnd);
        SetForegroundWindow (pObject->hwnd);
    }
    else {
        sId = (short) pObject->id;
        do  {
            if  (pObject->pPrev) {
                pObject = pObject->pPrev;
            }
        }
        while ((pObject->pPrev) && (strcmp (pObject->szType, "WINDOW") != 0));

        strcpy (BEDITargs, "SETFOCUS");
	PostMessage (pObject->hwnd, WM_FOCUSSET, sId, 0L);
//        WinSetFocus (HWND_DESKTOP, pObject->hwnd);
    }
    return;
}    

/********************************************************************/
/* This module will :                                               */
/*     set the title of a window                                    */
/********************************************************************/
void w550_title () {
    c120_parse_command ();
    if  (strcmp (Command, "MainWindow") == 0) {
        d580_find_variable ();
        SetWindowText (BEDITframe, pVariable->szString);
        return;
    }

    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        sprintf (Error, "Must use Window for TITLE [%s]", Command);
        z110_fatal ();
        return;
    }
    d580_find_variable ();
    SetWindowText (pObject->hwnd, pVariable->szString);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     enable an object                                             */
/********************************************************************/
void w560_enable () {

    long  ulStyle;
    OBJECT *pEnaObject;

    c120_parse_command ();
    w580_find_object ();
    pEnaObject = pObject;
    pEnaObject->cEnabled = 'y';

    EnableWindow (pEnaObject->hwnd, TRUE);

    if  (memcmp (pEnaObject->szType, "LBOBJ", 5) == 0) {
        SendMessage (pEnaObject->hwnd, WM_SETREDRAW, TRUE, 0L);
    }

    ulStyle = GetWindowLong (pEnaObject->hwnd, GWL_STYLE);
    if  ((memcmp (pEnaObject->szType, "STTXT", 5) != 0) &&
         (memcmp (pEnaObject->szType, "GBOBJ", 5) != 0)) {
        if  (pEnaObject->flStyle & WS_GROUP) {
            ulStyle |= WS_TABSTOP;
        }
    }

    if  (memcmp (pEnaObject->szType, "EFOBJ", 5) == 0) {
//        SetWindowPos (pEnaObject->hwnd, NULL, pEnaObject->x,
//                         pEnaObject->y, pEnaObject->cx, pEnaObject->cy, SWP_MOVE);
        ulStyle |= WS_BORDER;
    }
    if  (memcmp (pEnaObject->szType, "MLOBJ", 5) == 0) {
//        WinSendMsg (pEnaObject->hwnd, MLM_SETREADONLY, MPFROMSHORT(FALSE), 0L);
    }
    SetWindowLong (pEnaObject->hwnd, GWL_STYLE, ulStyle);
    pEnaObject->flStyle = ulStyle;
    SendMessage (pEnaObject->hwnd, BM_SETSTYLE,
                 ulStyle, 1L);

    if  (memcmp (pEnaObject->szType, "MSOBJ", 5) == 0) {
        EnableMenuItem (hMenu, pEnaObject->id, MF_ENABLED);
    }
    InvalidateRect (pEnaObject->hwnd, NULL, TRUE);

    if  (memcmp (pEnaObject->szType, "EFOBJ", 5) == 0) {
        short sId;
        sId = (short) pEnaObject->id;
        do  {
            if  (pEnaObject->pPrev) {
                pEnaObject = pEnaObject->pPrev;
            }
        }
        while ((pEnaObject->pPrev) && (strcmp (pEnaObject->szType, "WINDOW") != 0));
        PostMessage (pEnaObject->hwnd, WM_ENABLEENTRY, sId, 0L);
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     disable an object                                            */
/********************************************************************/
void w570_disable () {

    long  ulStyle;
    OBJECT *pEnaObject;

    c120_parse_command ();
    w580_find_object ();
    pEnaObject = pObject;
    pEnaObject->cEnabled = 'n';

    EnableWindow (pEnaObject->hwnd, FALSE);
    if  (memcmp (pEnaObject->szType, "LBOBJ", 5) == 0) {
        SendMessage (pEnaObject->hwnd, WM_SETREDRAW, FALSE, 0L);
    }
    ulStyle = GetWindowLong (pEnaObject->hwnd, GWL_STYLE);
    if  (memcmp (pEnaObject->szType, "EFOBJ", 5) == 0) {
        ulStyle &= ~WS_BORDER;
//        WinSetWindowPos (pEnaObject->hwnd, NULL, pEnaObject->x+3,
//                         pEnaObject->y+4, pEnaObject->cx, pEnaObject->cy, SWP_MOVE);
//        SetWindowPos(pEnaObject->hwnd,
//                     NULL,
//                     Sizex,             /*  Window Position x coordinate    */
//                     Sizey,             /*  Window Position y coordinate    */
//                     Sizecx,            /*  Window cx size                  */
//                     Sizecy,            /*  Window cy size                  */
//                     SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER |
//                                     SWP_NOACTIVE);
    }
    ulStyle &= ~WS_TABSTOP;
    SetWindowLong (pEnaObject->hwnd, GWL_STYLE, ulStyle);
    InvalidateRect (pEnaObject->hwnd, NULL, TRUE);
    pEnaObject->flStyle = ulStyle;
    SendMessage (pEnaObject->hwnd, BM_SETSTYLE,
                 ulStyle, 1L);
    if  (memcmp (pEnaObject->szType, "MLOBJ", 5) == 0) {
//        WinSendMsg (pEnaObject->hwnd, MLM_SETREADONLY, MPFROMSHORT(TRUE), 0L);
    }
    if  (memcmp (pEnaObject->szType, "MSOBJ", 5) == 0) {
        EnableMenuItem (hMenu, pEnaObject->id, MF_GRAYED);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     find an object                                               */
/********************************************************************/
void w580_find_object () {
    //******************************************
    //* Check last used object
    //******************************************
    //if  (pObject) {
    //    if  (strcmp (pObject->szName, Command) == 0) {
    //        return;
    //    }
    //}
    //******************************************
    //* Check last object used on line
    //******************************************
    //if  (pCommand) {
    //    if  (pCommand->pObj1) {
    //        pObject = pCommand->pObj1;
    //        if  (strcmp (pObject->szName, Command) == 0) {
    //            return;
    //        }
    //    }
    //    if  (pCommand->pObj2) {
    //        pObject = pCommand->pObj2;
    //        if  (strcmp (pObject->szName, Command) == 0) {
    //            return;
    //        }
    //    }
    //    if  (pCommand->pObj3) {
    //        pObject = pCommand->pObj3;
    //        if  (strcmp (pObject->szName, Command) == 0) {
    //            return;
    //        }
    //    }
    //}
    //******************************************
    //* Find the Object in the list (Top Down)
    //******************************************
    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  (strcmp (pObject->szName, Command) == 0) {
                //**************************************
                //* Update the pCommand Object Pointer
                //**************************************
                if  (pCommand) {
                    if  ((!pCommand->pObj1) || (pCommand->pObj1 == pObject)) {
                        pCommand->pObj1 = pObject;
                    }
                    else {
                        if  ((!pCommand->pObj2) || (pCommand->pObj2 == pObject)) {
                            pCommand->pObj2 = pObject;
                        }
                        else {
                            if  ((!pCommand->pObj3) || (pCommand->pObj3 == pObject)) {
                                pCommand->pObj3 = pObject;
                            }
                        }
                    }
                }
                return;
            }
        }
    }
    while (pObject->pNext);
    sprintf (Error, "Bad locate object [%s]", Command);
    z110_fatal ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     display a message box                                        */
/********************************************************************/
void w590_message () {

    WORD   flStyle;
    WORD   idWindow;
    int    uRes;
    char   cStyleSet;
    char   cIdSet;
    HWND   hwndObject;
    HWND   hwndWindow;

    idWindow = 0;
    cIdSet = 'n';

    flStyle = 0;
    cStyleSet = 'n';

    c120_parse_command ();
    if  ((strcmp (Command, "INFO") == 0) ||
         (strcmp (Command, "info") == 0)) {
        idWindow = MB_ICONINFORMATION;
        cIdSet = 'y';
    }
    if  ((strcmp (Command, "WARNING") == 0) ||
         (strcmp (Command, "warning") == 0)) {
        idWindow = MB_ICONEXCLAMATION;
        cIdSet = 'y';
        MessageBeep (MB_ICONEXCLAMATION);
    }
    if  ((strcmp (Command, "ERROR") == 0) ||
         (strcmp (Command, "error") == 0)) {
        idWindow = MB_ICONHAND;
        cIdSet = 'y';
        MessageBeep (MB_ICONHAND);
    }
    if  ((strcmp (Command, "QUESTION") == 0) ||
         (strcmp (Command, "question") == 0)) {
        idWindow = MB_ICONQUESTION;
        cIdSet = 'y';
        MessageBeep (MB_ICONQUESTION);
    }
    if  (cIdSet != 'y') {
        z010_syntax_error ();
        return;
    }

    c120_parse_command ();
    if  ((strcmp (Command, "OK") == 0) ||
         (strcmp (Command, "ok") == 0) ||
         (strcmp (Command, "ENTER") == 0) ||
         (strcmp (Command, "enter") == 0)) {
        flStyle = MB_OK;
        cStyleSet = 'y';
    }
    if  ((strcmp (Command, "OKCANCEL") == 0) ||
         (strcmp (Command, "okcancel") == 0) ||
         (strcmp (Command, "ENTERCANCEL") == 0) ||
         (strcmp (Command, "entercancel") == 0)) {
        flStyle = MB_OKCANCEL;
        cStyleSet = 'y';
    }
    if  ((strcmp (Command, "RETRYCANCEL") == 0) ||
         (strcmp (Command, "retrycancel") == 0)) {
        flStyle = MB_RETRYCANCEL;
        cStyleSet = 'y';
    }
    if  ((strcmp (Command, "ABORTRETRYIGNORE") == 0) ||
         (strcmp (Command, "abortretryignore") == 0)) {
        flStyle = MB_ABORTRETRYIGNORE;
        cStyleSet = 'y';
    }
    if  ((strcmp (Command, "YESNO") == 0) ||
         (strcmp (Command, "yesno") == 0)) {
        flStyle = MB_YESNO;
        cStyleSet = 'y';
    }
    if  ((strcmp (Command, "YESNOCANCEL") == 0) ||
         (strcmp (Command, "yesnocancel") == 0)) {
        flStyle = MB_YESNOCANCEL;
        cStyleSet = 'y';
    }
    if  (cStyleSet != 'y') {
        z010_syntax_error ();
        return;
    }

    strcpy (Command, "message");
    d580_find_variable ();

    strcpy (BEDITargs, "MessageBox In Process");
    hwndObject = GetFocus ();
    hwndWindow = GetParent (hwndObject);
    uRes = MessageBox (BEDITframe,
                       pVariable->szString,
                       "Message",
                       MB_APPLMODAL | flStyle | idWindow);
    strcpy (BEDITargs, "");
    memset (buff, 0, 20);
    if  (uRes == IDOK) {
        strcpy (buff, "OK");
    }
    if  (uRes == IDCANCEL) {
        strcpy (buff, "CANCEL");
    }
    if  (uRes == IDABORT) {
        strcpy (buff, "ABORT");
    }
    if  (uRes == IDIGNORE) {
        strcpy (buff, "IGNORE");
    }
    if  (uRes == IDRETRY) {
        strcpy (buff, "RETRY");
    }
    if  (uRes == IDYES) {
        strcpy (buff, "YES");
    }
    if  (uRes == IDNO) {
        strcpy (buff, "NO");
    }
    strcpy (Command, "message");
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (char *) malloc (strlen (buff) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad store response from message box");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, buff);
    SetActiveWindow (hwndWindow);
    SetFocus (hwndObject);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     center a window                                              */
/********************************************************************/
void w600_center () {
    RECT rectDesktop;
    int  ix, iy;

    c120_parse_command ();
    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        sprintf (Error, "Must use WINDOW in CENTER [%s]", Command);
        z110_fatal ();
        return;
    }

    //GetWindowRect (GetDesktopWindow (), &rectDesktop);
    GetWindowRect (BEDITframe, &rectDesktop);

    rectDesktop.bottom += 40;    /* Allow room for Title & Menu Bar */

    if  (rectDesktop.right < pObject->cx) {
        ix = 1;
    }
    else {
        ix = (rectDesktop.right - pObject->cx) / 2;
    }

    if  (rectDesktop.bottom < pObject->cy) {
        iy = 1;
    }
    else {
	iy = (rectDesktop.bottom - pObject->cy) / 2;
    }
    SetWindowPos (pObject->hwnd, HWND_TOP, ix, iy, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    return;
}
/********************************************************************/
/* This module will :                                               */
/*     center a window on the desktop                               */
/********************************************************************/
void w605_center_desktop () {
    RECT rectDesktop;
    int  ix, iy;

    c120_parse_command ();
    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        sprintf (Error, "Must use WINDOW in CENTER [%s]", Command);
        z110_fatal ();
        return;
    }

    GetWindowRect (GetDesktopWindow (), &rectDesktop);
    //GetWindowRect (BEDITframe, &rectDesktop);

    rectDesktop.bottom += 40;    /* Allow room for Title & Menu Bar */

    if  (rectDesktop.right < pObject->cx) {
        ix = 1;
    }
    else {
        ix = (rectDesktop.right - pObject->cx) / 2;
    }

    if  (rectDesktop.bottom < pObject->cy) {
        iy = 1;
    }
    else {
	iy = (rectDesktop.bottom - pObject->cy) / 2;
    }
    SetWindowPos (pObject->hwnd, HWND_TOP, ix, iy, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     clear window variables                                       */
/********************************************************************/
void w610_clear () {

    char      szWindow      [100];
    int usX;
    int x;

    c120_parse_command ();
    w580_find_object ();
    if  (strcmp (pObject->szType, "WINDOW") != 0) {
        sprintf (Error, "Must use WINDOW in CLEAR [%s]", Command);
        z110_fatal ();
        return;
    }
    strcpy (szWindow, Command);
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  (strcmp (pObject->szType, "WINDOW") != 0) {
                strcpy (Command, pObject->szName);
                d580_find_variable ();
                if  ((memcmp (pObject->szType, "EFOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "MLOBJ", 5) == 0)) {
                    strcpy (pVariable->szString, "");
                    pVariable->Integer = 0;
                }
                if  ((memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
                     (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
                    strcpy (pVariable->szString, "N");
                    pVariable->Integer = 0;
                }
                if  (memcmp (pObject->szType, "LBOBJ", 5) == 0) {
                    strcpy (pVariable->szString, "");
                    pVariable->Integer = 0;
                }
            }
        }
    }
    while ((pObject->pNext) && (strcmp (pObject->szWindow, szWindow) == 0));
    strcpy (Command, szWindow);
    w500_modify ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     insert a line into a listbox                                 */
/********************************************************************/
void w700_insert () {
    short     sSort;
    char      cSortSet;
    long      lColor;
    char      cOk;
    short     usId;

    cSortSet = 'n';
    /************************************/
    /* Sort                             */
    /************************************/
    c120_parse_command ();
    if  ((strcmp (Command, "END") == 0) ||
         (strcmp (Command, "end") == 0)) {
        cSortSet = 'y';
//        sSort = LIT_END;
    }
    if  ((strcmp (Command, "ASCEND") == 0) ||
         (strcmp (Command, "ascend") == 0)) {
        cSortSet = 'y';
//        sSort = LIT_SORTASCENDING;
    }
    if  ((strcmp (Command, "DESCEND") == 0) ||
         (strcmp (Command, "descend") == 0)) {
        cSortSet = 'y';
//        sSort = LIT_SORTDESCENDING;
    }
    if  (cSortSet != 'y') {
        z010_syntax_error ();
        return;
    }

    /************************************/
    /* Color                            */
    /************************************/
    c120_parse_command ();
    cOk = 'n';
    if  ((strcmp (Command, "DEFAULT") == 0) ||
         (strcmp (Command, "default") == 0)) {
//        lColor = CLR_DEFAULT;
        cOk = 'y';
    }
    if  ((strcmp (Command, "WHITE") == 0) ||
         (strcmp (Command, "white") == 0)) {
//        lColor = CLR_WHITE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLACK") == 0) ||
         (strcmp (Command, "black") == 0)) {
//        lColor = CLR_BLACK;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BACKGROUND") == 0) ||
         (strcmp (Command, "background") == 0)) {
//        lColor = CLR_BACKGROUND;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLUE") == 0) ||
         (strcmp (Command, "blue") == 0)) {
//        lColor = CLR_BLUE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "RED") == 0) ||
         (strcmp (Command, "red") == 0)) {
//        lColor = CLR_RED;
        cOk = 'y';
    }
    if  ((strcmp (Command, "PINK") == 0) ||
         (strcmp (Command, "pink") == 0)) {
//        lColor = CLR_PINK;
        cOk = 'y';
    }
    if  ((strcmp (Command, "GREEN") == 0) ||
         (strcmp (Command, "green") == 0)) {
//        lColor = CLR_GREEN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "CYAN") == 0) ||
         (strcmp (Command, "cyan") == 0)) {
//        lColor = CLR_CYAN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "YELLOW") == 0) ||
         (strcmp (Command, "yellow") == 0)) {
//        lColor = CLR_YELLOW;
        cOk = 'y';
    }
    if  ((strcmp (Command, "NEUTRAL") == 0) ||
         (strcmp (Command, "neutral") == 0)) {
//        lColor = CLR_NEUTRAL;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGRAY") == 0) ||
         (strcmp (Command, "darkgray") == 0)) {
//        lColor = CLR_DARKGRAY;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKBLUE") == 0) ||
         (strcmp (Command, "darkblue") == 0)) {
//        lColor = CLR_DARKBLUE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKRED") == 0) ||
         (strcmp (Command, "darkred") == 0)) {
//        lColor = CLR_DARKRED;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKPINK") == 0) ||
         (strcmp (Command, "darkpink") == 0)) {
//        lColor = CLR_DARKPINK;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGREEN") == 0) ||
         (strcmp (Command, "darkgreen") == 0)) {
//        lColor = CLR_DARKGREEN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKCYAN") == 0) ||
         (strcmp (Command, "darkcyan") == 0)) {
//        lColor = CLR_DARKCYAN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BROWN") == 0) ||
         (strcmp (Command, "brown") == 0)) {
//        lColor = CLR_BROWN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "PALEGRAY") == 0) ||
         (strcmp (Command, "palegray") == 0)) {
//        lColor = CLR_PALEGRAY;
        cOk = 'y';
    }
    if  ((strcmp (Command, "TRUE") == 0) ||
         (strcmp (Command, "true") == 0)) {
//        lColor = CLR_TRUE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "FALSE") == 0) ||
         (strcmp (Command, "false") == 0)) {
//        lColor = CLR_FALSE;
        cOk = 'y';
    }
    if  (cOk == 'n') {
        z010_syntax_error ();
        return;
    }

    /************************************/
    /* Object                           */
    /************************************/
    c120_parse_command ();
    w580_find_object ();
    if  ((memcmp (pObject->szType, "LBOBJ", 5) != 0) &&
         (memcmp (pObject->szType, "COOBJ", 5) != 0)) {
        sprintf (Error, "Must use LBOBJ or COOBJ in INSERT [%s]", Command);
        z110_fatal ();
        return;
    }
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        sprintf (Error, "Must use String Variable in INSERT [%s]", Command);
        z110_fatal ();
        return;
    }
    /************************************/
    /* Insert the Line                  */
    /************************************/
    if  (strcmp (pVariable->szString, "") != 0) {
	if  (memcmp (pObject->szType, "COOBJ", 5) == 0) {
	    SendMessage (pObject->hwnd, CB_ADDSTRING, 0,
			(LONG) (LPSTR) pVariable->szString);
	}
        else {
            SendMessage (pObject->hwnd, LB_ADDSTRING, 0,
			(LONG) (LPSTR) pVariable->szString);
	}
        UpdateWindow (pObject->hwnd);
    }
    else {
        z999_open_bug ();
        sprintf (buff, "Null String Not Inserted into List [%s]!\n", pVariable->szName);
        fputs (buff, file_debug_out);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     insert a file into a listbox                                 */
/********************************************************************/
void w705_fileinsert () {
    short     sSort;
    char      cSortSet;
    LONG      lColor;
    char      cOk;
    short     usId;

    cSortSet = 'n';
    /************************************/
    /* Sort                             */
    /************************************/
    c120_parse_command ();
    if  ((strcmp (Command, "END") == 0) ||
         (strcmp (Command, "end") == 0)) {
        cSortSet = 'y';
//        sSort = LIT_END;
    }
    if  ((strcmp (Command, "ASCEND") == 0) ||
         (strcmp (Command, "ascend") == 0)) {
        cSortSet = 'y';
//        sSort = LIT_SORTASCENDING;
    }
    if  ((strcmp (Command, "DESCEND") == 0) ||
         (strcmp (Command, "descend") == 0)) {
        cSortSet = 'y';
//        sSort = LIT_SORTDESCENDING;
    }
    if  (cSortSet != 'y') {
        z010_syntax_error ();
        return;
    }

    /************************************/
    /* Color                            */
    /************************************/
    c120_parse_command ();
    cOk = 'n';
    if  ((strcmp (Command, "DEFAULT") == 0) ||
         (strcmp (Command, "default") == 0)) {
//        lColor = CLR_DEFAULT;
        cOk = 'y';
    }
    if  ((strcmp (Command, "WHITE") == 0) ||
         (strcmp (Command, "white") == 0)) {
//        lColor = CLR_WHITE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLACK") == 0) ||
         (strcmp (Command, "black") == 0)) {
//        lColor = CLR_BLACK;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BACKGROUND") == 0) ||
         (strcmp (Command, "background") == 0)) {
//        lColor = CLR_BACKGROUND;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLUE") == 0) ||
         (strcmp (Command, "blue") == 0)) {
//        lColor = CLR_BLUE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "RED") == 0) ||
         (strcmp (Command, "red") == 0)) {
//        lColor = CLR_RED;
        cOk = 'y';
    }
    if  ((strcmp (Command, "PINK") == 0) ||
         (strcmp (Command, "pink") == 0)) {
//        lColor = CLR_PINK;
        cOk = 'y';
    }
    if  ((strcmp (Command, "GREEN") == 0) ||
         (strcmp (Command, "green") == 0)) {
//        lColor = CLR_GREEN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "CYAN") == 0) ||
         (strcmp (Command, "cyan") == 0)) {
//        lColor = CLR_CYAN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "YELLOW") == 0) ||
         (strcmp (Command, "yellow") == 0)) {
//        lColor = CLR_YELLOW;
        cOk = 'y';
    }
    if  ((strcmp (Command, "NEUTRAL") == 0) ||
         (strcmp (Command, "neutral") == 0)) {
//        lColor = CLR_NEUTRAL;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGRAY") == 0) ||
         (strcmp (Command, "darkgray") == 0)) {
//        lColor = CLR_DARKGRAY;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKBLUE") == 0) ||
         (strcmp (Command, "darkblue") == 0)) {
//        lColor = CLR_DARKBLUE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKRED") == 0) ||
         (strcmp (Command, "darkred") == 0)) {
//        lColor = CLR_DARKRED;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKPINK") == 0) ||
         (strcmp (Command, "darkpink") == 0)) {
//        lColor = CLR_DARKPINK;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGREEN") == 0) ||
         (strcmp (Command, "darkgreen") == 0)) {
//        lColor = CLR_DARKGREEN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKCYAN") == 0) ||
         (strcmp (Command, "darkcyan") == 0)) {
//        lColor = CLR_DARKCYAN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BROWN") == 0) ||
         (strcmp (Command, "brown") == 0)) {
//        lColor = CLR_BROWN;
        cOk = 'y';
    }
    if  ((strcmp (Command, "PALEGRAY") == 0) ||
         (strcmp (Command, "palegray") == 0)) {
//        lColor = CLR_PALEGRAY;
        cOk = 'y';
    }
    if  ((strcmp (Command, "TRUE") == 0) ||
         (strcmp (Command, "true") == 0)) {
//        lColor = CLR_TRUE;
        cOk = 'y';
    }
    if  ((strcmp (Command, "FALSE") == 0) ||
         (strcmp (Command, "false") == 0)) {
//        lColor = CLR_FALSE;
        cOk = 'y';
    }
    if  (cOk == 'n') {
        z010_syntax_error ();
        return;
    }

    /************************************/
    /* Object                           */
    /************************************/
    c120_parse_command ();
    w580_find_object ();
    if  ((memcmp (pObject->szType, "LBOBJ", 5) != 0) &&
         (memcmp (pObject->szType, "COOBJ", 5) != 0)) {
        sprintf (Error, "Must use LBOBJ or COOBJ in INSERT [%s]", Command);
        z110_fatal ();
        return;
    }
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        sprintf (Error, "Must use String Variable in INSERT [%s]", Command);
        z110_fatal ();
        return;
    }
    /***************************************/
    /* Move to the top of the file         */
    /***************************************/
    if  (!pDataFile) {
        strcpy (Error, "No Current File");
        z110_fatal ();
        return;
    }
    do  {
        if  (pDataFile->pPrev) {
            pDataFile = pDataFile->pPrev;
        }
    }
    while (pDataFile->pPrev);
    /***************************************/
    /* Insert the Lines                    */
    /***************************************/
    do  {
        if  (pDataFile->pNext) {
            pDataFile = pDataFile->pNext;
            if  (strcmp (pDataFile->szLine, "") != 0) {
                strcpy (buff, pDataFile->szLine);
                if  (buff [strlen (buff) - 1] == retn [0]) {
                    buff [strlen (buff) - 1] = 0;
		}
	        if  (memcmp (pObject->szType, "COOBJ", 5) == 0) {
	            SendMessage (pObject->hwnd, CB_ADDSTRING, 0,
		      	        (LONG) (LPSTR) buff);
	        }
                else {
                    SendMessage (pObject->hwnd, LB_ADDSTRING, 0,
				(LONG) (LPSTR) buff);
	        }
                UpdateWindow (pObject->hwnd);
            }
        }
    }
    while (pDataFile->pNext);
    return;
}      
       
/********************************************************************/
/* This module will :                                               */
/*     delete from a listbox                                        */
/********************************************************************/
void w710_delete () {
    short usX;
    short x;

    c120_parse_command ();
    w580_find_object ();
    if  ((memcmp (pObject->szType, "LBOBJ", 5) != 0) &&
         (memcmp (pObject->szType, "COOBJ", 5) != 0)) {
        sprintf (Error, "Must use LBOBJ or COOBJ in DELETE [%s]", Command);
        z110_fatal ();
        return;
    }
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        sprintf (Error, "Must use String Variable in INSERT [%s]", Command);
        z110_fatal ();
        return;
    }
    if  (strcmp (pVariable->szString, "ALL") == 0) {
	if  (memcmp (pObject->szType, "COOBJ", 5) == 0) {
	    do {
                SendMessage (pObject->hwnd, CB_DELETESTRING, 0, 0L);
            }
	    while (SendMessage (pObject->hwnd, CB_GETCOUNT, 0, 0L) > 0);
        }
        else {
            do {
                SendMessage (pObject->hwnd, LB_DELETESTRING, 0, 0L);
            }
	    while (SendMessage (pObject->hwnd, LB_GETCOUNT, 0, 0L) > 0);
        }
    }
    else {
        if  (memcmp (pVariable->szString, "INDEX=", 6) == 0) {
            Number = 0;
            Number = strtol (pVariable->szString + 6, (char**)&p, 10);
            usX = (short) Number;
            if  (usX > 0) {
                usX--;
            }
        }
	else {
	    if  (memcmp (pObject->szType, "COOBJ", 5) == 0) {
		usX = (short) SendMessage (pObject->hwnd, CB_GETCURSEL, 0, 0L);
	    }
	    else {
		usX = (short) SendMessage (pObject->hwnd, LB_GETCURSEL, 0, 0L);
            }
        }
        if  (usX == LB_ERR) {
            return;
	}
	if  (memcmp (pObject->szType, "COOBJ", 5) == 0) {
	    SendMessage (pObject->hwnd, CB_DELETESTRING, usX, 0L);
	}
        else {
	    SendMessage (pObject->hwnd, LB_DELETESTRING, usX, 0L);
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     select a line in a listbox                                   */
/********************************************************************/
void w720_select () {
    short usX;

    c120_parse_command ();
    w580_find_object ();
    if  (memcmp (pObject->szType, "LBOBJ", 5) != 0) {
        sprintf (Error, "Must use LBOBJ in SELECT [%s]", Command);
        z110_fatal ();
        return;
    }
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        sprintf (Error, "Must use String Variable in SELECT [%s]", Command);
        z110_fatal ();
        return;
    }
    if  (memcmp (pVariable->szString, "INDEX=", 6) != 0) {
        sprintf (Error, "Must use INDEX= in SELECT [%s]", Command);
        z110_fatal ();
        return;
    }
    Number = 0;
    Number = strtol (pVariable->szString + 6, (char**)&p, 10);
    usX = (short) Number;
    if  (usX > 0) {
        usX--;
    }
    SendMessage (pObject->hwnd, LB_SETCURSEL, usX, 0L);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     unselect a line in a listbox                                 */
/********************************************************************/
void w730_unselect () {
    //short usX;
    c120_parse_command ();
    w580_find_object ();
    if  (memcmp (pObject->szType, "LBOBJ", 5) != 0) {
        sprintf (Error, "Must use LBOBJ in UNSELECT [%s]", Command);
        z110_fatal ();
        return;
    }
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        sprintf (Error, "Must use String Variable in UNSELECT [%s]", Command);
        z110_fatal ();
        return;
    }
    if  (memcmp (pVariable->szString, "INDEX=", 6) != 0) {
        sprintf (Error, "Must use INDEX= in UNSELECT [%s]", Command);
        z110_fatal ();
        return;
    }
    Number = 0;
    Number = strtol (pVariable->szString + 6, (char**)&p, 10);
    //usX = (short) Number;
    //if  (usX > 0) {
    //    usX--;
    //}
    SendMessage (pObject->hwnd, LB_SETCURSEL, -1, 0L);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     set the pointer                                              */
/********************************************************************/
void w740_pointer () {

    c120_parse_command ();

    strcpy (szPointer, Command);
    if  ((strcmp (Command, "ARROW") == 0) ||
         (strcmp (Command, "arrow") == 0) ||
         (strcmp (Command, "POINTER") == 0) ||
         (strcmp (Command, "pointer") == 0)) {
        cUpdateCursor = 'y';
        SetCursor (LoadCursor (NULL, IDC_ARROW));
        return;
    }
    cUpdateCursor = 'n';
    if  ((strcmp (Command, "CROSS") == 0) ||
         (strcmp (Command, "cross") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_CROSS));
        return;
    }
    if  ((strcmp (Command, "WAIT") == 0) ||
         (strcmp (Command, "wait") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_WAIT));
        return;
    }
    if  ((strcmp (Command, "APPICON") == 0) ||
         (strcmp (Command, "appicon") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_ICON));
        return;
    }
    if  ((strcmp (Command, "ICONERROR") == 0) ||
         (strcmp (Command, "iconerror") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_ICON));
        return;
    }
    if  ((strcmp (Command, "ICONINFORMATION") == 0) ||
         (strcmp (Command, "iconinformation") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_ICON));
        return;
    }
    if  ((strcmp (Command, "ICONQUESTION") == 0) ||
         (strcmp (Command, "iconquestion") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_ICON));
        return;
    }
    if  ((strcmp (Command, "ICONWARNING") == 0) ||
         (strcmp (Command, "iconwarning") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_ICON));
        return;
    }
    if  ((strcmp (Command, "MOVE") == 0) ||
         (strcmp (Command, "move") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_SIZE));
        return;
    }
    if  ((strcmp (Command, "SIZENESW") == 0) ||
         (strcmp (Command, "sizenesw") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_SIZENESW));
        return;
    }
    if  ((strcmp (Command, "SIZENS") == 0) ||
         (strcmp (Command, "sizens") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_SIZENS));
        return;
    }
    if  ((strcmp (Command, "SIZENWSE") == 0) ||
         (strcmp (Command, "sizenwse") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_SIZENWSE));
        return;
    }
    if  ((strcmp (Command, "SIZEWE") == 0) ||
         (strcmp (Command, "sizewe") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_SIZEWE));
        return;
    }
    if  ((strcmp (Command, "TEXT") == 0) ||
         (strcmp (Command, "text") == 0)) {
        SetCursor (LoadCursor (NULL, IDC_CROSS));
        return;
    }
    strcpy (szPointer, "ARROW");
    z010_syntax_error ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     set an ACTION to a Variable                                  */
/********************************************************************/
void w750_action () {
    c120_parse_command ();
    d580_find_variable ();
    free (pVariable->szAction);
    pVariable->szAction = (char *) malloc (strlen (pVariable->szString) + 5);
    if  (!pVariable->szAction) {
        strcpy (Error, "Bad set up ACTION");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szAction, pVariable->szString);
    strcpy (pVariable->szEdit, szIf1);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     set an EDIT to a Variable                                    */
/********************************************************************/
void w760_edit () {
    c120_parse_command ();
    d580_find_variable ();
    c120_parse_command ();
    strcpy (pVariable->szEdit, "");
    if  ((strcmp (Command, "NUMERIC") == 0) ||
         (strcmp (Command, "numeric") == 0)) {
        strcpy (pVariable->szEdit, "NUMERIC");
    }
    if  ((strcmp (Command, "ALPHA") == 0) ||
         (strcmp (Command, "alpha") == 0)) {
        strcpy (pVariable->szEdit, "ALPHA");
    }
    if  ((strcmp (Command, "TEXT") == 0) ||
         (strcmp (Command, "text") == 0)) {
        strcpy (pVariable->szEdit, "TEXT");
    }
    if  ((strcmp (Command, "UPPER") == 0) ||
         (strcmp (Command, "upper") == 0)) {
        strcpy (pVariable->szEdit, "UPPER");
    }
    if  ((strcmp (Command, "LOWER") == 0) ||
         (strcmp (Command, "lower") == 0)) {
        strcpy (pVariable->szEdit, "LOWER");
    }
    if  ((strcmp (Command, "MIX") == 0) ||
         (strcmp (Command, "mix") == 0)) {
        strcpy (pVariable->szEdit, "MIX");
    }
    if  ((strcmp (Command, "DATE") == 0) ||
         (strcmp (Command, "date") == 0)) {
        strcpy (pVariable->szEdit, "DATE");
    }
    if  ((strcmp (Command, "CURRDATE") == 0) ||
         (strcmp (Command, "currdate") == 0)) {
        strcpy (pVariable->szEdit, "CURRDATE");
    }
    if  ((strcmp (Command, "DOLLAR") == 0) ||
         (strcmp (Command, "dollar") == 0)) {
        strcpy (pVariable->szEdit, "DOLLAR");
    }
    if  ((strcmp (Command, "TELEPHONE") == 0) ||
         (strcmp (Command, "telephone") == 0)) {
        strcpy (pVariable->szEdit, "TELEPHONE");
    }
    if  ((strcmp (Command, "ZERO") == 0) ||
         (strcmp (Command, "zero") == 0)) {
        strcpy (pVariable->szEdit, "ZERO");
    }
    if  (strcmp (pVariable->szEdit, "") == 0) {
        z010_syntax_error ();
    }
    c120_parse_command ();
    Number = 0;
    Number = strtol (Command, (char**)&p, 10);
    if  ((Number < (LONG) 1) || (Number > (LONG) Max - 5)) {
        strcpy (Error, "Invalid Edit Size");
        z110_fatal ();
        return;
    }
    if  ((strcmp (pVariable->szEdit, "DATE") == 0) ||
         (strcmp (pVariable->szEdit, "CURRDATE") == 0)) {
        if  (Number != 10) {
            strcpy (Error, "Size must be 10 for Date fields!");
            z110_fatal ();
            return;
        }
    }
    if  (strcmp (pVariable->szEdit, "TELEPHONE") == 0) {
        if  (Number != 14) {
            strcpy (Error, "Size must be 14 for Telephone fields!");
            z110_fatal ();
            return;
        }
    }
    pVariable->sSize = (short) Number;

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     set a bitmap for a user define object                        */
/********************************************************************/
void w765_bitmap () {
    VARIABLE *pHoldVariable;

    c120_parse_command ();
    d580_find_variable ();
    c120_parse_command ();
    if  (Command [0] == Subs) {
        pHoldVariable = pVariable;
        d580_find_variable ();
        strcpy (Command, pVariable->szString);
        pVariable = pHoldVariable;
    }
    if  (strlen (Command) > 9) {
        strcpy (Error, "Invalid bitmap name (size)!");
        z110_fatal ();
        return;
    }
//    strcpy (pVariable->szEdit, Command);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     push a button                                                */
/********************************************************************/
void w770_push () {
    c120_parse_command ();
    w580_find_object ();
    if  ((memcmp (pObject->szType, "PBOBJ", 5) == 0) ||
         (memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
         (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
        //z999_open_bug ();
        //sprintf (buff, "window push %s[%i]\n", pObject->szName, (int)pObject->hwnd);
        //fputs (buff, file_debug_out);
        PostMessage (GetParent(pObject->hwnd), WM_COMMAND, pObject->id, (long)pObject->hwnd);
        strcpy (BEDITargs, "PUSH");
    }
    else {
        strcpy (Error, "Must use PBOBJ, CBOBJ, or RBOBJ in PUSH");
        z110_fatal ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process timer functions                                      */
/********************************************************************/
void w780_timer () {
    c120_parse_command ();
    if  ((strcmp (Command, "ON") == 0) ||
         (strcmp (Command, "on") == 0)) {
        c120_parse_command ();
        Number = 0;
        Number = strtol (Command, (char**)&p, 10);
        if  (Number == 0) { // if = 0 then make it 1/10th of a second
            Number = 100;
        }
        else {
            if  (Number < 0) { // if < 0 then make it 1/2 second
                Number = 500;
            }
            else {
                if  (Number > 60) {  // Max 60 Seconds
                    Number = 60;
                }
                Number = Number * 1000; // Turn number into Seconds
            }
        }
        sInterval = (short) Number;
        KillTimer(BEDITframe, 1);
        SetTimer (BEDITframe, 1, sInterval, NULL);
        return;
    }
    if  ((strcmp (Command, "OFF") == 0) ||
	 (strcmp (Command, "off") == 0)) {
        KillTimer(BEDITframe, 1);
        sInterval = 0;
        return;
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     retrieve the window focus                                    */
/********************************************************************/
void w790_getfocus () {
    HWND hwndFocus;
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use string variable for GETFOCUS");
        z110_fatal ();
        return;
    }
    hwndFocus = GetFocus ();
    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
        }
    }
    while ((pObject->pNext) && (pObject->hwnd != hwndFocus));
    if  (pObject->hwnd != hwndFocus) {
        strcpy (temp, "");
    }
    else {
        strcpy (temp, pObject->szName);
    }
    free (pVariable->szString);
    pVariable->szString = (char *) malloc (strlen (temp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad assign GETFOCUS name");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, temp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     retrieve the window focus                                    */
/********************************************************************/
void w795_getfocuswin () {
    HWND hwndFocus;
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use string variable for GETFOCUSWIN");
        z110_fatal ();
        return;
    }
    hwndFocus = GetFocus ();
    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
        }
    }
    while ((pObject->pNext) && (pObject->hwnd != hwndFocus));
    if  (pObject->hwnd != hwndFocus) {
        strcpy (temp, "");
    }
    else {
        strcpy (temp, pObject->szWindow);
    }
    free (pVariable->szString);
    pVariable->szString = (char *) malloc (strlen (temp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad assign GETFOCUSWIN name");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, temp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     change the default button                                    */
/********************************************************************/
void w800_default () {
    char szWindow [100];
    char First;
    long ulStyle;

    First = 'n';
    if  (memcmp (Command, "FGB-", 4) == 0) {
        if  (strcmp (Command, "FGB-FIRST") == 0) {
            First = 'y';
        }
        else {
            strcpy (Command, Command+4);
        }
    }
    else {
       c120_parse_command ();
    }
    strcpy (szIf2, Command);
    if  (First != 'y') {
        w580_find_object ();
        if  (memcmp (pObject->szType, "PBOBJ", 5) != 0) {
            strcpy (Error, "Must use Push Button Object for DEFAULT");
            z110_fatal ();
            return;               
        }
    }
    strcpy (szWindow, pObject->szWindow);
    pObject = &StartObject;
    do  {
        if  (pObject->pNext) {
            pObject = pObject->pNext;
            if  ((strcmp (pObject->szWindow, szWindow) == 0) &&
                 (memcmp (pObject->szType, "PBOBJ", 5) == 0)) {
                if  ((strcmp (pObject->szName, szIf2) == 0) ||
                     (First == 'y')) {

                    ulStyle = GetWindowLong (pObject->hwnd, GWL_STYLE);
                    ulStyle |= BS_DEFPUSHBUTTON;
                    pObject->flStyle = ulStyle;
                    SendMessage (pObject->hwnd, BM_SETSTYLE,
                                 ulStyle, 1L);
                    First = 'n';
                }
                else {
                    ulStyle = GetWindowLong (pObject->hwnd, GWL_STYLE);
		    if  (ulStyle & BS_DEFPUSHBUTTON) {
                        ulStyle &= ~BS_DEFPUSHBUTTON;
                        ulStyle |= BS_PUSHBUTTON;
                        pObject->flStyle = ulStyle;
                        SendMessage (pObject->hwnd, BM_SETSTYLE,
                                     ulStyle, 1L);
                    }
                }
            }
        }
    }
    while (pObject->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     call DosBeep                                                 */
/********************************************************************/
void w810_sound () {
    int sNote;
    int sDuration;
    /**************************/
    /* Note                   */
    /**************************/
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            sprintf (Command, "%d", pVariable->Integer);
        }
    }
    Number = 0;
    Number = strtol (Command, (char**)&p, 10);
    //sNote = (int) Number;

    /**************************/
    /* Duration               */
    /**************************/
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            sprintf (Command, "%d", pVariable->Integer);
        }
    }
    Number = 0;
    Number = strtol (Command, (char**)&p, 10);
    //sDuration = (unsigned) Number;

//    DosBeep (sNote, sDuration);
    MessageBeep (MB_ICONHAND);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     set the color of an object                                   */
/********************************************************************/
void w820_color () {

    COLORREF lColor;
    char cOk;
    HDC hdc;

    c120_parse_command ();
    w580_find_object ();
    if  (!pObject->hwnd) {
        strcpy (Error, "Must open the window before setting the COLOR!");
        z110_fatal ();
        return;
    }
    c120_parse_command ();
    cOk = 'n';
    if  ((strcmp (Command, "DEFAULT") == 0) ||
         (strcmp (Command, "default") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "WHITE") == 0) ||
         (strcmp (Command, "white") == 0)) {
        lColor = RGB (255,255,255);
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLACK") == 0) ||
         (strcmp (Command, "black") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "BACKGROUND") == 0) ||
         (strcmp (Command, "background") == 0)) {
        lColor = COLOR_BACKGROUND;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLUE") == 0) ||
         (strcmp (Command, "blue") == 0)) {
        lColor = RGB (0,0,255);
        cOk = 'y';
    }
    if  ((strcmp (Command, "RED") == 0) ||
         (strcmp (Command, "red") == 0)) {
        lColor = RGB (255,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "PINK") == 0) ||
         (strcmp (Command, "pink") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "GREEN") == 0) ||
         (strcmp (Command, "green") == 0)) {
        lColor = RGB (0,255,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "CYAN") == 0) ||
         (strcmp (Command, "cyan") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "YELLOW") == 0) ||
         (strcmp (Command, "yellow") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "NEUTRAL") == 0) ||
         (strcmp (Command, "neutral") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGRAY") == 0) ||
         (strcmp (Command, "darkgray") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKBLUE") == 0) ||
         (strcmp (Command, "darkblue") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKRED") == 0) ||
         (strcmp (Command, "darkred") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKPINK") == 0) ||
         (strcmp (Command, "darkpink") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGREEN") == 0) ||
         (strcmp (Command, "darkgreen") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKCYAN") == 0) ||
         (strcmp (Command, "darkcyan") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "BROWN") == 0) ||
         (strcmp (Command, "brown") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "PALEGRAY") == 0) ||
         (strcmp (Command, "palegray") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "TRUE") == 0) ||
         (strcmp (Command, "true") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "FALSE") == 0) ||
         (strcmp (Command, "false") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  (cOk == 'n') {
        z010_syntax_error ();
        return;
    }
    hdc = GetDC (pObject->hwnd);
    SetTextColor (hdc, lColor);
    ReleaseDC (pObject->hwnd, hdc);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     set the background color of an object                        */
/********************************************************************/
void w825_backcolor () {

    COLORREF lColor;
    char cOk;
    HDC  hdc;

    c120_parse_command ();
    w580_find_object ();
    if  (!pObject->hwnd) {
        strcpy (Error, "Must open the window before setting the BACKCOLOR!");
        z110_fatal ();
        return;
    }
    c120_parse_command ();
    cOk = 'n';
    if  ((strcmp (Command, "DEFAULT") == 0) ||
         (strcmp (Command, "default") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "WHITE") == 0) ||
         (strcmp (Command, "white") == 0)) {
        lColor = RGB (255,255,255);
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLACK") == 0) ||
         (strcmp (Command, "black") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "BACKGROUND") == 0) ||
         (strcmp (Command, "background") == 0)) {
        lColor = COLOR_BACKGROUND;
        cOk = 'y';
    }
    if  ((strcmp (Command, "BLUE") == 0) ||
         (strcmp (Command, "blue") == 0)) {
        lColor = RGB (0,0,255);
        cOk = 'y';
    }
    if  ((strcmp (Command, "RED") == 0) ||
         (strcmp (Command, "red") == 0)) {
        lColor = RGB (255,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "PINK") == 0) ||
         (strcmp (Command, "pink") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "GREEN") == 0) ||
         (strcmp (Command, "green") == 0)) {
        lColor = RGB (0,255,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "CYAN") == 0) ||
         (strcmp (Command, "cyan") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "YELLOW") == 0) ||
         (strcmp (Command, "yellow") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "NEUTRAL") == 0) ||
         (strcmp (Command, "neutral") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGRAY") == 0) ||
         (strcmp (Command, "darkgray") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKBLUE") == 0) ||
         (strcmp (Command, "darkblue") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKRED") == 0) ||
         (strcmp (Command, "darkred") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKPINK") == 0) ||
         (strcmp (Command, "darkpink") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKGREEN") == 0) ||
         (strcmp (Command, "darkgreen") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "DARKCYAN") == 0) ||
         (strcmp (Command, "darkcyan") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "BROWN") == 0) ||
         (strcmp (Command, "brown") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "PALEGRAY") == 0) ||
         (strcmp (Command, "palegray") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "TRUE") == 0) ||
         (strcmp (Command, "true") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  ((strcmp (Command, "FALSE") == 0) ||
         (strcmp (Command, "false") == 0)) {
        lColor = RGB (0,0,0);
        cOk = 'y';
    }
    if  (cOk == 'n') {
        z010_syntax_error ();
        return;
    }
    hdc = GetDC (pObject->hwnd);
    SetBkColor (hdc, lColor);
    ReleaseDC (pObject->hwnd, hdc);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     conceal an entry field (password)                            */
/********************************************************************/
void w830_conceal () {
    c120_parse_command ();
    w580_find_object ();
    if  (memcmp (pObject->szType, "EFOBJ", 5) != 0) {
        strcpy (Error, "Must use Entry Field in CONCEAL");
        z110_fatal ();
        return;
    }
    pObject->flStyle |= ES_PASSWORD;
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     unconceal an entry field (password)                          */
/********************************************************************/
void w840_unconceal () {
    c120_parse_command ();
    w580_find_object ();
    if  (memcmp (pObject->szType, "EFOBJ", 5) != 0) {
        strcpy (Error, "Must use Entry Field in UNCONCEAL");
        z110_fatal ();
        return;
    }
    pObject->flStyle &= ~ES_PASSWORD;
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     HWND to string conversion                                    */
/********************************************************************/
void w870_hwnd () {

    long lHwnd;
    char szTemp [100];

    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    w580_find_object ();
    if  (!pObject->hwnd) {
        strcpy (Error, "Window must be open!");
        z110_fatal ();
        return;
    }

    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use string variable in HWND!");
        z110_fatal ();
        return;
    }
    lHwnd = (long) pObject->hwnd;
    sprintf (szTemp, "%ld", lHwnd);
    free (pVariable->szString);
    pVariable->szString = (char *) malloc (strlen (szTemp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad allocate variable for HWND!");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szTemp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Size a Window                                                */
/********************************************************************/
void w880_size () {
    HWND  hwndSize;
    short Sizex;
    short Sizey;
    short Sizecx;
    short Sizecy;
    RECT  rect;

    //z999_open_bug ();
    //sprintf (buff, "w880_size\n");
    //fputs (buff, file_debug_out);

    c120_parse_command ();
    if (Command[0] == Subs) {
       d580_find_variable ();
       if  (!pVariable) {
           return;
       }
       strcpy (Command, pVariable->szString);
    }
    //pObject = NULL;                                       // 11-22-2013
    if  (strcmp (Command, "MainWindow") == 0) {
        hwndSize = BEDITframe;
    }
    else {
        w580_find_object ();
        if  (!pObject->hwnd) {
            //strcpy (Error, "Must open the window before SIZE!");
            //z110_fatal ();
            return;
        }
        hwndSize = pObject->hwnd;
        //z999_open_bug ();
        //sprintf (buff, "Size pVariable[%s] pObject[%s]\n", pVariable->szName, pObject->szName);
        //fputs (buff, file_debug_out);
    }

    /***************/
    /* Max Size    */
    /***************/
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'I') {
            sprintf (Command, "%d", pVariable->Integer);
        }
        else {
            strcpy (Command, pVariable->szString);
        }
    }
    if  ((strcmp (Command, "MAX") == 0) ||
         (strcmp (Command, "max") == 0)) {
        GetWindowRect (GetParent (hwndSize), &rect);
        SetWindowPos (hwndSize, HWND_TOP, rect.left, rect.top, rect.right,
                      rect.bottom, SWP_NOZORDER);
        return;
    }

    /***************/
    /* x           */
    /***************/
    Number = 0;
    Number = strtol (Command, (char**)&p, 10);
    Sizex = (short) Number;
    /***************/
    /* y           */
    /***************/
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'I') {
            sprintf (Command, "%d", pVariable->Integer);
        }
        else {
            strcpy (Command, pVariable->szString);
        }
    }
    Number = 0;
    Number = strtol (Command, (char**)&p, 10);
    Sizey = (short) Number;
    /***************/
    /* cx          */
    /***************/
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'I') {
            sprintf (Command, "%d", pVariable->Integer);
        }
        else {
            strcpy (Command, pVariable->szString);
        }
    }
    Number = 0;
    Number = strtol (Command, (char**)&p, 10);
    Sizecx = (short) Number;
    /***************/
    /* cy          */
    /***************/
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'I') {
            sprintf (Command, "%d", pVariable->Integer);
        }
        else {
            strcpy (Command, pVariable->szString);
        }
    }
    Number = 0;
    Number = strtol (Command, (char**)&p, 10);
    Sizecy = (short) Number;

    //**************************
    //* Convert to OS/2 coord
    //**************************
    //if  (hwndSize != BEDITframe) {
        //RECT rectParent;
        //int  WY;

        //if  (memcmp (pObject->szType, "WINDOW", 6) == 0) {
        //    GetWindowRect (GetDesktopWindow (), &rectParent);
        //}
        //else {
        //    GetWindowRect (GetParent(hwndSize), &rectParent);
        //}
        //WY = rectParent.bottom - rectParent.top;
        //if  (memcmp (pObject->szType, "WINDOW", 6) != 0) {
        //    WY = WY - 23;
        //}
        //Sizey = WY - Sizey - Sizecy + 5;
    //}

    /***************/
    /* Size Window */
    /***************/
    if  (hwndSize) {
        SetWindowPos(hwndSize,
                 NULL,
                 Sizex,             /*  Window Position x coordinate    */
                 Sizey,             /*  Window Position y coordinate    */
                 Sizecx,            /*  Window cx size                  */
                 Sizecy,            /*  Window cy size                  */
                 SWP_NOZORDER);
        UpdateWindow(hwndSize);
    }
    if  (hwndSize != BEDITframe) {
        if (pObject) {               // 11-22-2013
           pObject->x = Sizex;
           pObject->y = Sizey;
           pObject->cx = Sizecx;
           pObject->cy = Sizecy;
           //z999_open_bug ();
           //sprintf (buff, "Change Size Object[%s][%i][%i][%i][%i]\n", pObject->szName,pObject->x,pObject->y,pObject->cx,pObject->cy);
           //fputs (buff, file_debug_out);
        }
    }

    return;
}

void w900_date () {
    char  szTemp [20];
    int   index1;
    int   index2;
    int   dMonth;
    int   dDay;
    int   dYear;
    int   dMaxDay;
    char  szDate [15];
    char  date_str [15];

    _strdate (date_str);
    if  (strcmp (buff, "") == 0) {
        if  (strcmp (pVariable->szEdit, "CURRDATE") == 0) {
            strcpy (buff, date_str);
            if  (strcmp (szCountry, "EUROPE") == 0) { // dd-mm-ccyy
                memcpy (buff, date_str+3, 2);
                memcpy (buff+3, date_str, 2);
            }
        }
        else {
            strcpy (pVariable->szString, "");
            return;
        }
    }
    if  (strlen (buff) > 10) {
	//MessageBeep (MB_ICONHAND);
        memcpy (pVariable->szString, buff, 10);
        pVariable->szString [10] = 0;
        return;
    }
    strcpy (pVariable->szString, buff);
    strcpy (szDate, buff);
    memset (szTemp, 0, sizeof(szTemp));
    for (index1 = 0; index1 < (int) strlen (szDate); index1++)
        if  ((szDate [index1] == '.') || (szDate [index1] == '/'))
            szDate [index1] = '-';
    index1 = 0;
    index2 = 0;
    if  (strcmp (szCountry, "EUROPE") == 0) {
        /***********/
        /* Day     */
        /***********/
        memset (szTemp, 0, sizeof(szTemp));
        do  {
            if  ((szDate [index1] != '-') &&
                 (szDate [index1] > 32)) {
                szTemp [index2] = szDate [index1];
                index1++;
                index2++;
            }
        }
        while ((szDate [index1] != '-') &&
               (szDate [index1] > 32) &&
               (index2 < 2));
        dDay = 0;
        sscanf (szTemp, "%d", &dDay);
        if  (szDate [index1] != '-') {
            //MessageBeep (MB_ICONHAND);
            return;
        }
        /***********/
        /* Month   */
        /***********/
        memset (szTemp, 0, sizeof(szTemp));
        index1++;
        index2 = 0;
        do  {
            if  ((szDate [index1] != '-') &&
                 (szDate [index1] > 32)) {
                szTemp [index2] = szDate [index1];
                index1++;
                index2++;
            }
        }
        while ((szDate [index1] != '-') &&
               (szDate [index1] > 32) &&
               (index2 < 2));
        dMonth = 0;
        sscanf (szTemp, "%d", &dMonth);
        if  ((dMonth < 1) ||
             (dMonth > 12)) {
            //MessageBeep (MB_ICONHAND);
            return;
        }
    }
    else {
        /***********/
        /* Month   */
        /***********/
        do  {
            if  ((szDate [index1] != '-') &&
                 (szDate [index1] > 32)) {
                szTemp [index2] = szDate [index1];
                index1++;
                index2++;
            }
        }
        while ((szDate [index1] != '-') &&
               (szDate [index1] > 32) &&
               (index2 < 2));
        dMonth = 0;
        sscanf (szTemp, "%d", &dMonth);
        if  ((dMonth < 1) ||
             (dMonth > 12)) {
            //MessageBeep (MB_ICONHAND);
            return;
        }
        if  (szDate [index1] != '-') {
            //MessageBeep (MB_ICONHAND);
            return;
        }
        /***********/
        /* Day     */
        /***********/
        index1++;
        index2 = 0;
        memset (szTemp, 0, sizeof(szTemp));
        do  {
            if  ((szDate [index1] != '-') &&
                 (szDate [index1] > 32)) {
                szTemp [index2] = szDate [index1];
                index1++;
                index2++;
            }
        }
        while ((szDate [index1] != '-') &&
               (szDate [index1] > 32) &&
               (index2 < 2));
        dDay = 0;
        sscanf (szTemp, "%d", &dDay);
    }
    /******************/
    /* Check Month    */
    /******************/
    dMaxDay = 31;
    if  ((dMonth == 4) ||
         (dMonth == 6) ||
         (dMonth == 9) ||
         (dMonth == 11))
        dMaxDay = 30;
    if  (dMonth == 2)
        dMaxDay = 29;
    if  ((dDay < 1) ||
         (dDay > dMaxDay)) {
        //MessageBeep (MB_ICONHAND);
        return;
    }
    /***********/
    /* Year    */
    /***********/
    if  (szDate [index1] > 32) {
        index1++;
        index2 = 0;
        memset (szTemp, 0, sizeof(szTemp));
        do  {
            if  (szDate [index1] > 32) {
                szTemp [index2] = szDate [index1];
                index1++;
                index2++;
            }
        }
        while ((szDate [index1] > 32) &&
               (index2 < 4));
    }
    else {
        index2 = 4;
        szTemp [4] = 0;
        szTemp [2] = date_str [6];
        szTemp [3] = date_str [7];
        if  (date_str [6] > 53) {
            szTemp [0] = '1';
            szTemp [1] = '9';
        }
        else {
            szTemp [0] = '2';
            szTemp [1] = '0';
        }
    }
    if  ((index2 != 2) &&
         (index2 != 4)) {
        //MessageBeep (MB_ICONHAND);
        return;
    }
    if  (index2 == 2) {
        szTemp [2] = szTemp [0];
        szTemp [3] = szTemp [1];
        if  (szTemp [2] > 53) {
            szTemp [0] = '1';
            szTemp [1] = '9';
        }
        else {
            szTemp [0] = '2';
            szTemp [1] = '0';
        }
    }
    dYear = 0;
    sscanf (szTemp, "%d", &dYear);
    if  ((dYear < 1900) ||
         (dYear > 2050)) {
        //MessageBeep (MB_ICONHAND);
        return;
    }
    if  (strcmp (szCountry, "EUROPE") == 0) {
        sprintf (pVariable->szString, "%02d/%02d/%04d", dDay, dMonth, dYear);
    }
    else {
        sprintf (pVariable->szString, "%02d-%02d-%04d", dMonth, dDay, dYear);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Convert a Dollar Field                                       */
/********************************************************************/
void w910_dollar () {
    char  *szTemp;
    double dDollar;

    if  (strlen (buff) > 25) {
        //MessageBeep (MB_ICONHAND);
        memcpy (pVariable->szString, buff, 25);
        pVariable->szString [25] = 0;
        return;
    }
    strcpy (pVariable->szString, buff);
    dDollar = strtod (buff, &szTemp);
    sprintf (pVariable->szString, "%.02f", dDollar);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Convert a Telephone Field                                    */
/********************************************************************/
void w920_telephone () {
    char  *szTemp;
    int    Area;
    int    Number3;
    int    Number4;
    char   szTemp1  [15];
    if  (strlen (buff) == 0) {
        strcpy (pVariable->szString, "");
        return;
    }
    if  (strlen (buff) < 7) {
        strcpy (pVariable->szString, buff);
        //MessageBeep (MB_ICONHAND);
        return;
    }
    if  (strlen (buff) > 14) {
        memcpy (pVariable->szString, buff, 14);
        pVariable->szString [14] = 0;
        //MessageBeep (MB_ICONHAND);
        return;
    }
    strcpy (pVariable->szString, buff);
    if  (strlen (buff) == 7) {           // 7248010  -> 724-8010
        memcpy (szTemp1, buff, 3);
        szTemp1 [3] = 0;
        Number3 = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+3, 4);
        szTemp1 [4] = 0;
        Number4 = (int) strtod (szTemp1, &szTemp);

        sprintf (pVariable->szString, "%03d-%04d", Number3, Number4);
        return;
    }
    if  (strlen (buff) == 8) {           // 724-8010 -> 724-8010
        memcpy (szTemp1, buff, 3);
        szTemp1 [3] = 0;
        Number3 = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+4, 4);
        szTemp1 [4] = 0;
        Number4 = (int) strtod (szTemp1, &szTemp);

        sprintf (pVariable->szString, "%03d-%04d", Number3, Number4);
        return;
    }
    if  (strlen (buff) == 10) {          // 6127248010 -> (612) 724-8010
        memcpy (szTemp1, buff, 3);
        szTemp1 [3] = 0;
        Area = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+3, 3);
        szTemp1 [3] = 0;
        Number3 = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+6, 4);
        szTemp1 [4] = 0;
        Number4 = (int) strtod (szTemp1, &szTemp);

        sprintf (pVariable->szString, "(%03d) %03d-%04d", Area, Number3, Number4);
        return;
    }
    if  (strlen (buff) == 12) {          // (612)7248010 -> (612) 724-8010
        memcpy (szTemp1, buff+1, 3);
        szTemp1 [3] = 0;
        Area = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+5, 3);
        szTemp1 [3] = 0;
        Number3 = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+8, 4);
        szTemp1 [4] = 0;
        Number4 = (int) strtod (szTemp1, &szTemp);

        sprintf (pVariable->szString, "(%03d) %03d-%04d", Area, Number3, Number4);
        return;
    }
    if  ((strlen (buff) == 13) &&
         (buff [5] != ' ')) {          // (612)724-8010 -> (612) 724-8010
        memcpy (szTemp1, buff+1, 3);
        szTemp1 [3] = 0;
        Area = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+5, 3);
        szTemp1 [3] = 0;
        Number3 = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+9, 4);
        szTemp1 [4] = 0;
        Number4 = (int) strtod (szTemp1, &szTemp);

        sprintf (pVariable->szString, "(%03d) %03d-%04d", Area, Number3, Number4);
        return;
    }
    if  ((strlen (buff) == 13) &&
         (buff [5] == ' ')) {          // (612) 7248010 -> (612) 724-8010
        memcpy (szTemp1, buff+1, 3);
        szTemp1 [3] = 0;
        Area = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+6, 3);
        szTemp1 [3] = 0;
        Number3 = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+9, 4);
        szTemp1 [4] = 0;
        Number4 = (int) strtod (szTemp1, &szTemp);

        sprintf (pVariable->szString, "(%03d) %03d-%04d", Area, Number3, Number4);
        return;
    }
    if  (strlen (buff) == 14) {          // (612) 724-8010 -> (612) 724-8010
        memcpy (szTemp1, buff + 1, 3);
        szTemp1 [3] = 0;
        Area = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+6, 3);
        szTemp1 [3] = 0;
        Number3 = (int) strtod (szTemp1, &szTemp);

        memcpy (szTemp1, buff+10, 4);
        szTemp1 [4] = 0;
        Number4 = (int) strtod (szTemp1, &szTemp);

        sprintf (pVariable->szString, "(%03d) %03d-%04d", Area, Number3, Number4);
        return;
    }
    //MessageBeep (MB_ICONHAND);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Set up an Entry Field Editing Mask                           */
/********************************************************************/
void w960_mask () {
    /**************************/
    /* Object Name            */
    /**************************/
    c120_parse_command ();
    w580_find_object ();
    if  (memcmp (pObject->szType, "EFOBJ", 5) != 0) {
        strcpy (Error, "Must use EFOBJ");
        z110_fatal ();
        return;
    }

    /**************************/
    /* Variable Name          */
    /**************************/
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }

    /**************************/
    /* Assign the Mask        */
    /**************************/
    if  (pObject->szDraw) {
        free (pObject->szDraw);
    }
    pObject->szDraw = (char *) malloc (strlen (pVariable->szString) + 5);
    if  (!pObject->szDraw) {
        strcpy (Error, "Bad Obj Cr Mask");
        z110_fatal ();
        return;
    }
    strcpy (pObject->szDraw, pVariable->szString);
    strcpy (pObject->szEdit, "MASK");
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Get the Current Mouse Position                               */
/********************************************************************/
void w970_mousepos () {
    VARIABLE *pXvar;
    VARIABLE *pYvar;

    /**************************/
    /* X Variable             */
    /**************************/
    c120_parse_command ();
    d580_find_variable ();
    pXvar = pVariable;

    /**************************/
    /* Y Variable             */
    /**************************/
    c120_parse_command ();
    d580_find_variable ();
    pYvar = pVariable;

    /**************************/
    /* Assign X Coords        */
    /**************************/
    if  (pXvar->Type == 'I') {
        pXvar->Integer = (int) XMouse;
    }
    else {
        free (pXvar->szString);
        pXvar->szString = (char *) malloc (20);
        sprintf (pXvar->szString, "%d", (int) XMouse);
    }
    /**************************/
    /* Assign Y Coords        */
    /**************************/
    if  (pYvar->Type == 'I') {
        pYvar->Integer = (int) YMouse;
    }
    else {
        free (pYvar->szString);
        pYvar->szString = (char *) malloc (20);
        sprintf (pYvar->szString, "%d", (int) YMouse);
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Get a window position/size                                   */
/********************************************************************/
void w980_windowpos () {
    VARIABLE *pXvar;
    VARIABLE *pYvar;
    VARIABLE *pCxvar;
    VARIABLE *pCyvar;
    RECT     rectParent;
    RECT     rect;
    int      WY;

    /**************************/
    /* Object                 */
    /**************************/
    c120_parse_command ();

    if (Command[0] == Subs) {
       d580_find_variable ();
       strcpy (Command, pVariable->szString);
    }
    if  (strcmp (Command, "MAIN") == 0) {
	strcpy (Command, "WindowTimer");

        //z999_open_bug ();
        //sprintf (buff, "MAIN WindowTimer windowpos [%s][%i][%i][%i][%i]\n", pObject->szName,pObject->x,pObject->y,pObject->cx,pObject->cy);
        //fputs (buff, file_debug_out);
    }
    w580_find_object ();
    if  (!pObject->hwnd) {
        z999_open_bug ();
        sprintf (buff, "No hwnd for object Var[%s] Obj[%s]\n", pVariable->szName, pObject->szName);
        //fputs (buff, file_debug_out);
        //MessageBeep (MB_ICONHAND);
        //MessageBox (HWND_DESKTOP, buff, "Error", MB_OK);
        c120_parse_command ();
        c120_parse_command ();
        c120_parse_command ();
        c120_parse_command ();
        return;
    }
    //sprintf (buff, "window pos for Var[%s] obj[%s] hwndParent[%i]", pVariable->szName, pObject->szName, (int) GetParent(pObject->hwnd));
    //MessageBox (HWND_DESKTOP, buff, "debug", MB_OK);


    //z999_open_bug ();
    //sprintf (buff, "w980_windowpos [%s][%i][%i][%i][%i]\n", pObject->szName,pObject->x,pObject->y,pObject->cx,pObject->cy);
    //fputs (buff, file_debug_out);

    /**************************/
    /* X Variable             */
    /**************************/
    c120_parse_command ();
    d580_find_variable ();
    pXvar = pVariable;

    /**************************/
    /* Y Variable             */
    /**************************/
    c120_parse_command ();
    d580_find_variable ();
    pYvar = pVariable;

    /**************************/
    /* CX Variable            */
    /**************************/
    c120_parse_command ();
    d580_find_variable ();
    pCxvar = pVariable;

    /**************************/
    /* CY Variable            */
    /**************************/
    c120_parse_command ();
    d580_find_variable ();
    pCyvar = pVariable;

    /**************************/
    /* Get Window Coords      */
    /**************************/
    if  (memcmp (pObject->szType, "WINDOW", 6) == 0) {
        GetWindowRect (GetDesktopWindow (), &rectParent);
    }
    else {
        GetWindowRect (GetParent(pObject->hwnd), &rectParent);
    }

    if  (strcmp (pObject->szName, "WindowTimer") == 0) {
        GetWindowRect (GetDesktopWindow (), &rect);
        //z999_open_bug ();
        //sprintf (buff, "WindowTimer used GetDesktopWindow [%s][%i][%i][%i][%i]\n", pObject->szName,rect.left,rect.top,rect.right,rect.bottom);
        //fputs (buff, file_debug_out);
    }
    else {
        GetWindowRect (pObject->hwnd, &rect);
    }

    rect.bottom = rect.bottom - rect.top;
    rect.right = rect.right - rect.left;

    if  (strcmp (pObject->szName, "WindowTimer") != 0) {
        if  (memcmp (pObject->szType, "WINDOW", 6) != 0) {   // 23,4
            rect.top = rect.top - rectParent.top - 27;
            rect.left = rect.left - rectParent.left - 8;
        }

        if  (pObject) {                            // 11-22-2013
            rect.left = pObject->x;
            rect.top = pObject->y;
            rect.right = pObject->cx;
            rect.bottom = pObject->cy;
        }
    }


    //z999_open_bug ();
    //sprintf (buff, "Get Object Size [%s][%i][%i][%i][%i]\n", pObject->szName,pObject->x,pObject->y,pObject->cx,pObject->cy);
    //fputs (buff, file_debug_out);

    /**************************/
    /* Assign X Coords        */
    /**************************/
    if  (pXvar->Type == 'I') {
        pXvar->Integer = (int) rect.left;
    }
    else {
        free (pXvar->szString);
        pXvar->szString = (char *) malloc (20);
        sprintf (pXvar->szString, "%d", (int) rect.left);
    }
    /**************************/
    /* Assign Y Coords        */
    /**************************/
    if  (pYvar->Type == 'I') {
        pYvar->Integer = (int) rect.top;
    }
    else {
        free (pYvar->szString);
        pYvar->szString = (char *) malloc (20);
        sprintf (pYvar->szString, "%d", (int) rect.top);
    }

    /**************************/
    /* Assign CX Coords       */
    /**************************/
    if  (pCxvar->Type == 'I') {
        pCxvar->Integer = (int) rect.right;
    }
    else {
        free (pCxvar->szString);
        pCxvar->szString = (char *) malloc (20);
        sprintf (pCxvar->szString, "%d", (int) rect.right);
    }
    /**************************/
    /* Assign CY Coords       */
    /**************************/
    if  (pCyvar->Type == 'I') {
        pCyvar->Integer = (int) rect.bottom;
    }
    else {
        free (pCyvar->szString);
        pCyvar->szString = (char *) malloc (20);
        sprintf (pCyvar->szString, "%d", (int) rect.bottom);
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Subclass Procedure                                           */
/********************************************************************/
LRESULT APIENTRY w999_subclass_edit (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    OBJECT *pEditObject;
    OBJECT *pFocusObject;
    OBJECT *pParentObject;
    OBJECT *pTestObject;
    HWND   hwndParent;
    short usID;
    char  cTestChar;
    static char cKeepChar;
    char cBackTabTwice;

    //usID = GetWindowWord (hwnd, GWW_ID);
//    SetFocus (hwnd);

    pEditObject = &StartObject;
    do  {
        if  (pEditObject->pNext) {
            pEditObject = pEditObject->pNext;
            if  (memcmp (pEditObject->szType, "WINDOW", 6) == 0) {
                hwndParent = pEditObject->hwnd;
                pParentObject = pEditObject;
            }
        }
        else {
            strcpy (Error, "Unable to locate subclassed window");
            z110_fatal ();
        }
    }
    while ((pEditObject->pNext) &&
           (pEditObject->hwnd != hwnd));
    //if  (pEditObject) {
    //    char szBuff[500];
    //    z999_open_bug ();
    //    sprintf (szBuff, "Call/Return Original Process[%s][%f]\n", pEditObject->szName,pEditObject->OldProc);
    //    fputs (szBuff, file_debug_out);
    //    if  (!pEditObject->OldProc) {
    //        z999_open_bug ();
    //        sprintf (szBuff, "Bad Original Process[%s][%f]\n", pEditObject->szName,pEditObject->OldProc);
    //        fputs (szBuff, file_debug_out);
    //    }
    //    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
    //}
    
    strcpy (Command, pEditObject->szName);
    d580_find_variable ();
    pFocusObject = pEditObject;
    pTestObject = pEditObject;
    cTestChar = (char) wParam;
    switch (message) {

    case WM_SYSCOMMAND:
        if  (wParam == SC_KEYMENU)
        {
            //char szDebug [100];
            //cTestChar = cOldTestChar;
            //sprintf (szDebug, "Testchar=%c", cTestChar);
            //MessageBox (HWND_DESKTOP, szDebug, "Debug WM_SYSCOMMAND", MB_OK);
            pObject = pParentObject;
            do  {
                if  (pObject->pNext) {
                    pObject = pObject->pNext;

                    if  ((memcmp (pObject->szType, "PBOBJ", 5) == 0) ||
                         (memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
                         (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
                        /******************************************************/
                        /* If TABSTOP is off don't allow accelerator          */
                        /* Allow all Radio Buttons since they don't all have  */
                        /* TABSTOP set on.                                    */
                        /******************************************************/
                        if   ((pObject->flStyle & WS_TABSTOP) ||
                              (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {  

                            int  Tx1 = 0;

                            do  {
                                if  ((pObject->szText [Tx1] == 38) &&   /* & */
                                     (toupper(pObject->szText [Tx1+1]) == toupper(cTestChar))) {
                                    //MessageBox (HWND_DESKTOP, "Found!", "Debug", MB_OK);
                                    return 0L;
                                }
                                Tx1++;
                            }
                            while ((pObject->szText [Tx1] > 30) &&
                                   (Tx1 < 100));
                        }
                    }
                }
            }
            while ((pObject->pNext) &&
                   (strcmp (pObject->szWindow, pParentObject->szName) == 0));
        }
        break;

    case WM_MENUCHAR:
         return 1L;

    case WM_LBUTTONDBLCLK:
        /***********************************************/
        /* Double Click in List Box                    */
        /***********************************************/
        if  (memcmp (pFocusObject->szType, "LBOBJ", 5) == 0) {
            strcpy (Command, pFocusObject->szName);
            d580_find_variable ();
            if  ((strcmp (pVariable->szAction, "") == 0) ||
                 (strcmp (pVariable->szEdit, "SELECT") == 0)) {
                MessageBeep (MB_ICONHAND);
                z999_open_bug ();
                sprintf (buff, "No Double Click Action associated with [%s]\n", Command);
                fputs (buff, file_debug_out);
                //MessageBox (HWND_DESKTOP, buff, "Error", MB_OK);
                return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
            }
            pObject = pFocusObject;
            w200_proc_cmd ();
        }
        return CallWindowProc (pFocusObject->OldProc, hwnd, message, wParam, lParam);

    case WM_KILLFOCUS:
        if  (memcmp (pFocusObject->szType, "EFOBJ", 5) == 0) {
             PostMessage (GetParent (hwnd), WM_FOCUSCHG, pFocusObject->id, 0L);
        }
        return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);

    case WM_SYSKEYDOWN:
        //MessageBox (HWND_DESKTOP, "WM_SYSKEYDOWN", "Debugit", MB_OK);
        //*******************************************
        //* Control Push Button Accelerators
        //*******************************************
        cOldTestChar = cTestChar;
//        if  (lParam & 0x20000000) {
            pObject = pParentObject;
            do  {
                if  (pObject->pNext) {
                    pObject = pObject->pNext;
                    if  ((memcmp (pObject->szType, "PBOBJ", 5) == 0) ||
                         (memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
                         (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {

                        /******************************************************/
                        /* If TABSTOP is off don't allow accelerator          */
                        /* Allow all Radio Buttons since they don't all have  */
                        /* TABSTOP set on.                                    */
                        /******************************************************/
                        if   ((pObject->flStyle & WS_TABSTOP) ||
                              (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {  

                            int  Tx1 = 0;

                            do  {
                                if  ((pObject->szText [Tx1] == 38) &&   /* & */
                                     (toupper(pObject->szText [Tx1+1]) == toupper(cTestChar))) {
                                    if  (memcmp (pObject->szType, "PBOBJ", 5) == 0) {
                                        /*********************/
                                        /* PBOBJ             */
                                        /*********************/
                                        OBJECT *pKeepObject = pObject;

                                        SetFocus (pObject->hwnd);
                                        strcpy (Command, "FGB-");
                                        strcat (Command, pKeepObject->szName);
                                        PostMessage (GetParent(pKeepObject->hwnd), WM_COMMAND, pKeepObject->id, (long)pKeepObject->hwnd);
                                        w800_default ();
                                    }
                                    else {
                                        if  (memcmp (pObject->szType, "CBOBJ", 5) == 0) {

                                            OBJECT *pKeepObject = pObject;

                                            /*********************/
                                            /* CBOBJ             */
                                            /*********************/
                                            int usChecked;

                                            usChecked = (int) SendMessage (pKeepObject->hwnd, BM_GETCHECK,
                                                         0, 0L);
                                            SetFocus (pKeepObject->hwnd);
                                            PostMessage (GetParent(pKeepObject->hwnd), WM_COMMAND, pKeepObject->id, (long)pKeepObject->hwnd);
                                            if  (usChecked == 0) {
                                                SendMessage (pKeepObject->hwnd, BM_SETCHECK,
                                                             1, NULL);
                                            }
                                            else {
                                                SendMessage (pKeepObject->hwnd, BM_SETCHECK,
                                                             0, NULL);
                                            }
                                        }
                                        else {
                                            /*********************/
                                            /* RBOBJ             */
                                            /*********************/
                                            OBJECT *pRobject;
                                            OBJECT *pKeepObject;

                                            pKeepObject = pObject;
                                            SetFocus (pKeepObject->hwnd);
                                            SendMessage (pKeepObject->hwnd, BM_SETCHECK,
                                                         1, NULL);
                                            PostMessage (GetParent(pObject->hwnd), WM_COMMAND, pObject->id, (long)pKeepObject->hwnd);
                                            //******************************
                                            //* Backup to first RBOBJ
                                            //******************************
                                            pRobject = pKeepObject;
                                            do  {
                                                if  (pRobject->pPrev) {
                                                    pRobject = pRobject->pPrev;
                                                }
                                            }
                                            while ((pRobject->pPrev) &&
                                                   (memcmp (pRobject->szType, "RBOBJ", 5) == 0));
                                            //******************************
                                            //* Walk all RBOBJ & turn off
                                            //******************************
                                            do  {
                                                if  (pRobject->pNext) {
                                                    pRobject = pRobject->pNext;
                                                }
                                                if  ((memcmp (pRobject->szType, "RBOBJ", 5) == 0) &&
                                                     (pRobject != pKeepObject)) {
 
                                                    SendMessage (pRobject->hwnd, BM_SETCHECK,
                                                                 0, NULL);
                                                }
                                            }
                                            while ((pRobject->pNext) &&
                                                   (memcmp (pRobject->szType, "RBOBJ", 5) == 0));
                                        }
                                    }
//                                    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
                                    cKeepChar = 'n';
                                    return 1L;
                                }
                                Tx1++;
                            }
                            while ((pObject->szText [Tx1] > 30) &&
                                   (Tx1 < 100));
                        }
                    }
                }
            }
            while ((pObject->pNext) &&
                   (strcmp (pObject->szWindow, pParentObject->szName) == 0));
            return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
            //break;

	case WM_KEYDOWN:
            //MessageBox (HWND_DESKTOP, "WM_KEYDOWN", "Debugit", MB_OK);

	    switch (wParam) {

		case VK_RETURN:
                    //MessageBox (HWND_DESKTOP, "VK_RETURN", "Debug", MB_OK);
		    //************************************
		    //* Push first default push button
		    //************************************
		    pTestObject = &StartObject;
		    do  {
			if  (pTestObject->pNext) {
                            pTestObject = pTestObject->pNext;
			    if  (strcmp (pTestObject->szWindow, pParentObject->szWindow) == 0) {
				if  ((pTestObject->flStyle & BS_DEFPUSHBUTTON) &&
				     (memcmp (pTestObject->szType, "PBOBJ", 5) == 0)) {
                                    PostMessage (hwndParent, WM_COMMAND, pTestObject->id, (long)pTestObject->hwnd);
				    PostMessage (hwndParent, WM_FOCUSSET, pTestObject->id, (long)pTestObject->hwnd);
				    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
                                }
                            }
                        }
		    }
		    while (pTestObject->pNext);
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);

        case VK_ESCAPE:
            //MessageBox (HWND_DESKTOP, "VK_ESCAPE", "Debugit", MB_OK);
            strcpy (Command, "WindowEscape");
            test = 'y';
            d580_find_variable ();
            test = 'n';
            if  (found == 'y') {
                w200_proc_cmd ();
            }
            else {
                PostMessage (hwndParent, WM_CLOSE, 0, 0L);
	    }
            return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);

        case VK_INSERT:
            //MessageBox (HWND_DESKTOP, "WM_INSERT", "Debugit", MB_OK);
            strcpy (Command, "WindowInsert");
            test = 'y';
            d580_find_variable ();
            test = 'n';
            if  (found == 'y') {
                w200_proc_cmd ();
            }
            return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);

        case VK_DELETE:
            //MessageBox (HWND_DESKTOP, "WM_DELETE", "Debugit", MB_OK);
            strcpy (Command, "WindowDelete");
            test = 'y';
            d580_find_variable ();
            test = 'n';
            if  (found == 'y') {
                w200_proc_cmd ();
            }
            return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);

		case VK_F1:
		    strcpy (Command, "WindowF1");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F2:
		    strcpy (Command, "WindowF2");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F3:
		    strcpy (Command, "WindowF3");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F4:
		    strcpy (Command, "WindowF4");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F5:
		    strcpy (Command, "WindowF5");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F6:
		    strcpy (Command, "WindowF6");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F7:
		    strcpy (Command, "WindowF7");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F8:
		    strcpy (Command, "WindowF8");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F9:
		    strcpy (Command, "WindowF9");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F10:
		    strcpy (Command, "WindowF10");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F11:
		    strcpy (Command, "WindowF11");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		case VK_F12:
		    strcpy (Command, "WindowF12");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    if  (found == 'y') {
			w200_proc_cmd ();
		    }
		    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);

        case VK_UP:
            //MessageBox (HWND_DESKTOP, "WM_UP", "Debugit", MB_OK);
            {
                OBJECT *pKeepObject = pEditObject;
		strcpy (Command, "WindowUp");
		test = 'y';
		d580_find_variable ();
		test = 'n';
		if  (found == 'y') {
		    strcpy (Command,"WindowUp");
		    test = 'u';
		    w200_proc_cmd ();
		    strcpy (Command, "KeyMoveOK");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    strcpy (Command,"");
		    if  (found == 'y') {
			found = (char)pVariable->szString[0];
			if  (found == 'y') {
			    return 0L;
			}
		    }
		    //return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
		}

                if  (memcmp (pEditObject->szType, "RBOBJ", 5) == 0) {
                    OBJECT *pRobject = pEditObject->pPrev;

                    if  (memcmp (pRobject->szType, "RBOBJ", 5) == 0) {
                        SetFocus (pRobject->hwnd);
                        SendMessage (pRobject->hwnd, BM_SETCHECK,
                                     1, NULL);
                        SendMessage (pEditObject->hwnd, BM_SETCHECK,
                                     0, NULL);
                        PostMessage (GetParent(pRobject->hwnd), WM_COMMAND, pRobject->id, 0L);
                    }
                    else {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 123456);
                    }
                }
                else {
                    if  ((memcmp (pEditObject->szType, "LBOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "MLOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "COOBJ", 5) != 0)) {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 123456);
                    }
                }
                return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
            }

        case VK_LEFT:
            //MessageBox (HWND_DESKTOP, "WM_LEFT", "Debugit", MB_OK);
            {
                OBJECT *pKeepObject = pEditObject;

		strcpy (Command, "WindowLeft");
		test = 'y';
		d580_find_variable ();
		test = 'n';
		if  (found == 'y') {
		    strcpy (Command,"WindowLeft");
		    test = 'l';
		    w200_proc_cmd ();
		    strcpy (Command, "KeyMoveOK");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    strcpy (Command,"");
		    if  (found == 'y') {
			found = (char)pVariable->szString[0];
			if  (found == 'y') {
			    return 0L;
			}
		    }
		    //return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
		}

                //MessageBox (HWND_DESKTOP, "VK_UP", "Debug", MB_OK);
                if  (memcmp (pEditObject->szType, "RBOBJ", 5) == 0) {
                    OBJECT *pRobject = pEditObject->pPrev;

                    if  (memcmp (pRobject->szType, "RBOBJ", 5) == 0) {
                        SetFocus (pRobject->hwnd);
                        SendMessage (pRobject->hwnd, BM_SETCHECK,
                                     1, NULL);
                        SendMessage (pEditObject->hwnd, BM_SETCHECK,
                                     0, NULL);
                        PostMessage (GetParent(pRobject->hwnd), WM_COMMAND, pRobject->id, 0L);
                    }
                    else {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 123456);
                    }
                }
                else {
                    if  ((memcmp (pEditObject->szType, "LBOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "MLOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "COOBJ", 5) != 0)) {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 123456);
                    }
                }
                return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
            }

        case VK_DOWN:

            //MessageBox (HWND_DESKTOP, "WM_DOWN", "Debugit", MB_OK);
            {
                OBJECT *pKeepObject = pEditObject;

		strcpy (Command, "WindowDown");
		test = 'y';
		d580_find_variable ();
		test = 'n';
		if  (found == 'y') {
		    strcpy (Command,"WindowDown");
		    test = 'd';
		    w200_proc_cmd ();
		    strcpy (Command, "KeyMoveOK");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    strcpy (Command,"");
		    if  (found == 'y') {
			found = (char)pVariable->szString[0];
			if  (found == 'y') {
			    return 0L;
			}
		    }
		    //return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
		}

                //MessageBox (HWND_DESKTOP, "VK_DOWN", "Debug", MB_OK);
                if  (memcmp (pEditObject->szType, "RBOBJ", 5) == 0) {
                    OBJECT *pRobject = pEditObject->pNext;

                    if  (pRobject) {
                        if  (memcmp (pRobject->szType, "RBOBJ", 5) == 0) {
                            SetFocus (pRobject->hwnd);
                            SendMessage (pRobject->hwnd, BM_SETCHECK,
                                         1, NULL);
                            SendMessage (pEditObject->hwnd, BM_SETCHECK,
                                         0, NULL);
                            PostMessage (GetParent(pRobject->hwnd), WM_COMMAND, pRobject->id, 0L);
                        }
                        else {
                            PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                        }
                    }
                    else {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                    }
                }
                else {
                    if  ((memcmp (pEditObject->szType, "LBOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "MLOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "COOBJ", 5) != 0)) {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                    }
                }
                return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
            }
        case VK_RIGHT:

            //MessageBox (HWND_DESKTOP, "WM_RIGHT", "Debugit", MB_OK);
            {
                OBJECT *pKeepObject = pEditObject;

		strcpy (Command, "WindowRight");
		test = 'y';
		d580_find_variable ();
		test = 'n';
		if  (found == 'y') {
		    strcpy (Command,"WindowRight");
		    test = 'r';
		    w200_proc_cmd ();
		    strcpy (Command, "KeyMoveOK");
		    test = 'y';
		    d580_find_variable ();
		    test = 'n';
		    strcpy (Command,"");
		    if  (found == 'y') {
			found = (char)pVariable->szString[0];
			if  (found == 'y') {
			    return 0L;
			}
		    }
		    //return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
		}

                //MessageBox (HWND_DESKTOP, "VK_DOWN", "Debug", MB_OK);
                if  (memcmp (pEditObject->szType, "RBOBJ", 5) == 0) {
                    OBJECT *pRobject = pEditObject->pNext;

                    if  (pRobject) {
                        if  (memcmp (pRobject->szType, "RBOBJ", 5) == 0) {
                            SetFocus (pRobject->hwnd);
                            SendMessage (pRobject->hwnd, BM_SETCHECK,
                                         1, NULL);
                            SendMessage (pEditObject->hwnd, BM_SETCHECK,
                                         0, NULL);
                            PostMessage (GetParent(pRobject->hwnd), WM_COMMAND, pRobject->id, 0L);
                        }
                        else {
                            PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                        }
                    }
                    else {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                    }
                }
                else {
                    if  ((memcmp (pEditObject->szType, "LBOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "MLOBJ", 5) != 0) &&
                         (memcmp (pEditObject->szType, "COOBJ", 5) != 0)) {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L); 
                    }
                }
                return CallWindowProc (pKeepObject->OldProc, hwnd, message, wParam, lParam);
            }

        //*******************************************
        //* Special Characters
        //*******************************************
        case VK_LBUTTON:
        case VK_RBUTTON:
        case VK_CANCEL:
        case VK_MBUTTON:
        case VK_BACK:
        case VK_TAB:
        case VK_CLEAR:
        case VK_SHIFT:
        case VK_CONTROL:
        case VK_MENU:
        case VK_PAUSE:
        case VK_CAPITAL:
        case VK_PRIOR:
        case VK_NEXT:
        case VK_END:
        case VK_HOME:
        case VK_SELECT:
        case VK_PRINT:
        case VK_EXECUTE:
        case VK_SNAPSHOT:
        case VK_HELP:
        case VK_MULTIPLY:
        case VK_ADD:
        case VK_SEPARATOR:
        case VK_DIVIDE:
        case VK_NUMLOCK:
            //MessageBox (HWND_DESKTOP, "Special Chars", "Debugit", MB_OK);
            return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
	    }
        //****************************************
        //* Space = Press the Button
        //****************************************
        if  (cTestChar == ' ') {
            if  (memcmp (pFocusObject->szType, "PBOBJ", 5) == 0) {
                PostMessage (GetParent(pObject->hwnd), WM_COMMAND, pFocusObject->id, 0L);
                return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
            }
            if  (memcmp (pFocusObject->szType, "CBOBJ", 5) == 0) {
                PostMessage (GetParent(pObject->hwnd), WM_COMMAND, pFocusObject->id, 0L);
                return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
            }
        }

        //*******************************************
        //* Control Push Button Accelerators
        //*******************************************

        if  (GetKeyState (VK_CONTROL) < 0) {
            //MessageBox (HWND_DESKTOP, "GetKeyState(VK_CONTROL)", "debug", MB_OK);
            pObject = pParentObject;
            do  {
                if  (pObject->pNext) {
                    pObject = pObject->pNext;
                    if  ((memcmp (pObject->szType, "PBOBJ", 5) == 0) ||
                         (memcmp (pObject->szType, "CBOBJ", 5) == 0) ||
                         (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {
                        /******************************************************/
                        /* If TABSTOP is off don't allow accelerator          */
                        /* Allow all Radio Buttons since they don't all have  */
                        /* TABSTOP set on.                                    */
                        /******************************************************/
                        if   ((pObject->flStyle & WS_TABSTOP) ||
                              (memcmp (pObject->szType, "RBOBJ", 5) == 0)) {  

                            int  Tx1 = 0;

                            do  {
                                if  ((pObject->szText [Tx1] == 38) &&   /* & */
                                     (toupper(pObject->szText [Tx1+1]) == toupper(cTestChar))) {
                                    if  (memcmp (pObject->szType, "PBOBJ", 5) == 0) {
                                        /*********************/
                                        /* PBOBJ             */
                                        /*********************/
                                        OBJECT *pKeepObject = pObject;

                                        SetFocus (pObject->hwnd);
                                        strcpy (Command, "FGB-");
                                        strcat (Command, pKeepObject->szName);
                                        PostMessage (GetParent(pKeepObject->hwnd), WM_COMMAND, pKeepObject->id, (long)pKeepObject->hwnd);
                                        w800_default ();
                                    }
                                    else {
                                        if  (memcmp (pObject->szType, "CBOBJ", 5) == 0) {

                                            OBJECT *pKeepObject = pObject;

                                            /*********************/
                                            /* CBOBJ             */
                                            /*********************/
                                            int usChecked;

                                            usChecked = (int) SendMessage (pKeepObject->hwnd, BM_GETCHECK,
                                                         0, 0L);
                                            SetFocus (pKeepObject->hwnd);
                                            PostMessage (GetParent(pKeepObject->hwnd), WM_COMMAND, pKeepObject->id, 0L);
                                            if  (usChecked == 0) {
                                                SendMessage (pKeepObject->hwnd, BM_SETCHECK,
                                                             1, NULL);
                                            }
                                            else {
                                                SendMessage (pKeepObject->hwnd, BM_SETCHECK,
                                                             0, NULL);
                                            }
                                        }
                                        else {
                                            /*********************/
                                            /* RBOBJ             */
                                            /*********************/
                                            OBJECT *pRobject;
                                            OBJECT *pKeepObject;

                                            pKeepObject = pObject;
                                            SetFocus (pKeepObject->hwnd);
                                            SendMessage (pKeepObject->hwnd, BM_SETCHECK,
                                                         1, NULL);
                                            PostMessage (GetParent(pObject->hwnd), WM_COMMAND, pObject->id, 0L);
                                            //******************************
                                            //* Backup to first RBOBJ
                                            //******************************
                                            pRobject = pKeepObject;
                                            do  {
                                                if  (pRobject->pPrev) {
                                                    pRobject = pRobject->pPrev;
                                                }
                                            }
                                            while ((pRobject->pPrev) &&
                                                   (memcmp (pRobject->szType, "RBOBJ", 5) == 0));
                                            //******************************
                                            //* Walk all RBOBJ & turn off
                                            //******************************
                                            do  {
                                                if  (pRobject->pNext) {
                                                    pRobject = pRobject->pNext;
                                                }
                                                if  ((memcmp (pRobject->szType, "RBOBJ", 5) == 0) &&
                                                     (pRobject != pKeepObject)) {
 
                                                    SendMessage (pRobject->hwnd, BM_SETCHECK,
                                                                 0, NULL);
                                                }
                                            }
                                            while ((pRobject->pNext) &&
                                                   (memcmp (pRobject->szType, "RBOBJ", 5) == 0));
                                        }
                                    }
                                    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
                                }
                                Tx1++;
                            }
                            while ((pObject->szText [Tx1] > 30) &&
                                   (Tx1 < 100));
                        }
                    }
                }
            }
            while ((pObject->pNext) &&
                   (strcmp (pObject->szWindow, pParentObject->szName) == 0));
        }

        //*******************************************
        //* Correct Characters
        //*******************************************
        if  (GetKeyState (VK_SHIFT) < 0) {
            if  (cTestChar == '9') {
                cTestChar = '(';
            }
            if  (cTestChar == '0') {
                cTestChar = ')';
            }
        }
        if  (cTestChar == -67) {
            cTestChar = '-';
        }
        if  (cTestChar == -66) {
            cTestChar = '.';
        }
        if  (cTestChar == -65) {
            cTestChar = 47;
        }
        if  (cTestChar == -69) {
            cTestChar = '+';
        }
        if  (cTestChar == -68) {
            cTestChar = ',';
        }
        //MessageBox (HWND_DESKTOP, "Types", "Debug", MB_OK);
        /*************************/
        /* Numeric               */
        /*************************/
        if  ((strcmp (pVariable->szEdit, "NUMERIC") == 0) ||
             (strcmp (pVariable->szEdit, "ZERO") == 0)) {
            if  ( (!(isdigit (cTestChar))) || (cTestChar < 0) ) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0;
            }
        }
        /*************************/
        /* ALPHA                 */
        /*************************/
        if  (strcmp (pVariable->szEdit, "ALPHA") == 0) {
            if  (isdigit (cTestChar)) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
        }
        /*************************/
        /* DATE/CURRDATE         */
        /*************************/
        if  ((strcmp (pVariable->szEdit, "DATE") == 0) ||
             (strcmp (pVariable->szEdit, "CURRDATE") == 0)) {
            if  ((!(isdigit (cTestChar))) &&
                 (cTestChar != '-') &&
                 (cTestChar != '.') &&
                 (cTestChar != 47)) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
            if  (cTestChar < 0) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
        }
        /*************************/
        /* DOLLAR                */
        /*************************/
        if  (strcmp (pVariable->szEdit, "DOLLAR") == 0) {
            if  ((!(isdigit (cTestChar))) &&
                 (cTestChar != '.') &&
                 (cTestChar != '+') &&
                 (cTestChar != '-') &&
                 (cTestChar != ',')) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
            if  (cTestChar < 0) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
        }
        /*************************/
        /* TELEPHONE             */
        /*************************/
        if  (strcmp (pVariable->szEdit, "TELEPHONE") == 0) {
            if  ((!(isdigit (cTestChar))) &&
                 (cTestChar != '(') &&
                 (cTestChar != ')') &&
                 (cTestChar != '-')) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
            if  (cTestChar < 0) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
        }

        /*************************/
        /* MASK                  */
        /*************************/
        if  (strcmp (pEditObject->szEdit, "MASK") == 0) {
            PostMessage (GetParent (pEditObject->hwnd), WM_MASKENTRY, (WORD) pEditObject->id, 0L);
        }

        /*************************/
        /* Max Size              */
        /*************************/
        if  (pVariable->sSize > 0) {
            GetWindowText (pFocusObject->hwnd, buff, Max);
            if  ((int)strlen (buff) >= pVariable->sSize) {
                MessageBeep (MB_ICONHAND);
                cKeepChar = 'n';
                return 0L;
            }
        }

        /*************************/
        /* Autotab               */
        /*************************/
        if  (pVariable->sSize > 0) {
            if  ((cAutotab == 'y') &&
                 (strcmp (pFocusObject->szEdit, "MASK") != 0)) {
                GetWindowText (pFocusObject->hwnd, buff, Max);
                if  (pVariable->sSize == 1) {
                    if  ((int) strlen (buff) == 1) {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                    }
                }
                else {
                    if  ((int) strlen (buff) + 1 >= (int) pVariable->sSize) {
                        PostMessage (hwnd, WM_CHAR, (WORD)'\t', 0L);
                    }
                }
            }
        }
        return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);


	case WM_CHAR:
        if  (cKeepChar == 'n') {
            cKeepChar = 'y';
            return 0;
        }
        //MessageBox (HWND_DESKTOP, "WM_CHAR keep", "Debug", MB_OK);
        //*******************************************************
        //* If on non first RBOBJ backtab to object before the
        //* RBOBJ group
        //*******************************************************
        if  (pTestObject->flStyle & WS_TABSTOP) {
            cBackTabTwice = 'n';
        }
        else {
            cBackTabTwice = 'y';
        }

            if  (wParam != '\t') {
                int iC = (int) wParam;
                //char szMsg [100];
                //sprintf(szMsg, "No Tab [%i]", iC);
                //MessageBox (HWND_DESKTOP, szMsg, "Debug", MB_OK);
                if (iC == 13) {
                   //****************************
                   //* Return in Field
                   //****************************
                   return 0;
                }
                else {
                   //****************************
                   //* Non-Tab Char in Field
                   //****************************
                    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
                }
            }
	    switch (wParam) {

		case '\t':
                    //MessageBox (HWND_DESKTOP, "Tab", "Debug", MB_OK);
		    //************************************
		    //* Backtab
		    //************************************
		    if  ((GetKeyState (VK_SHIFT) < 0) ||
                         (lParam == 123456)) {
			//********************************
			//* Backtab to prev object on win
			//********************************
			do  {
			    if  (pTestObject->pPrev) {
			        pTestObject = pTestObject->pPrev;
                        	//*******************************************************
	                        //* If on non first RBOBJ backtab to object before the
        	                //* RBOBJ group
                	        //*******************************************************
			        if  ((pTestObject->flStyle & WS_TABSTOP) &&
                                     (cBackTabTwice == 'y')) {
                                     cBackTabTwice = 'n';
                                     if  (pTestObject->pPrev) {
                                         pTestObject = pTestObject->pPrev;
                                     }
                                 }
			         if  (pTestObject->flStyle & WS_TABSTOP) {
                          	     if  (memcmp (pTestObject->szType, "RBOBJ", 5) == 0) {
	                                 OBJECT *pRobject = pTestObject;
        	                         int usChecked;
                	                 //OBJECT *pPrevRobject = pTestObject->pPrev;
	
        	                         do  {
                	                    usChecked = (int) SendMessage (pRobject->hwnd, BM_GETCHECK,
                        	                         0, 0L);
                                	    if  (usChecked == 0) {
                                        	if  (pRobject->pNext) {
                                            	    pRobject = pRobject->pNext;
                                       	        }
                                    	     }
                                   	     else {
                                      	        pTestObject = pRobject;
                                   	     }
                                          }
                                          while ((usChecked == 0) &&
                                                 (pRobject->pNext) &&
                                                 (memcmp (pRobject->szType, "RBOBJ", 5) == 0));
                            	      }
                           	      PostMessage (hwndParent, WM_FOCUSSET, pTestObject->id, 0L);
                            	      strcpy (Command, "FGB-");
                            	      if  (memcmp (pTestObject->szType, "PBOBJ", 5) != 0) {
                                          strcat (Command, "FIRST");
                                      }
                            	      else {
                                          strcat (Command, pTestObject->szName);
                            	      }
                            	      w800_default ();
                                      return 0;
//                                    return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
			          }
                              }
		          }
		          while (memcmp (pTestObject->szType, "WINDOW", 6) != 0);
			  //********************************
			  //* Backtab to last object on win
			  //********************************
			  do  {
			      if  (pTestObject->pNext) {
                                  pTestObject = pTestObject->pNext;
                               }
			  }
			  while ((pTestObject->pNext) &&
			         (strcmp (pTestObject->szWindow, pParentObject->szWindow) == 0));
			  do  {
			      if  (pTestObject->pPrev) {  
			          pTestObject = pTestObject->pPrev;
			          if  (pTestObject->flStyle & WS_TABSTOP) {
                                      if  (memcmp (pTestObject->szType, "RBOBJ", 5) == 0) {
                                          OBJECT *pRobject = pTestObject;
                                          int usChecked;

                                           do  {
                                               usChecked = (int) SendMessage (pRobject->hwnd, BM_GETCHECK, 0, 0L);
                                               if  (usChecked == 0) {
                                                   if  (pRobject->pNext) {
                                                       pRobject = pRobject->pNext;
                                                   }
                                               }
                                               else {
                                                   pTestObject = pRobject;
                                               }
                                           }
                                           while ((usChecked == 0) &&
                                                  (pRobject->pNext) &&
                                                  (memcmp (pRobject->szType, "RBOBJ", 5) == 0));

                                       }
                                       PostMessage (hwndParent, WM_FOCUSSET, pTestObject->id, 0L);
                                       strcpy (Command, "FGB-");
                                       if  (memcmp (pTestObject->szType, "PBOBJ", 5) != 0) {
                                           strcat (Command, "FIRST");
                                       }
                                       else {
                                           strcat (Command, pTestObject->szName);
                                       }
                                       w800_default ();
                                       return 0;
//                                     return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
				   }
                                }
			     }
			     while (memcmp (pTestObject->szType, "WINDOW", 6) != 0);
			     SetFocus (hwnd);
                             return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
		         }

		         //************************************
		         //* Tab to next object on window
		         //************************************
		         do  {
			     if  (pTestObject->pNext) {
				 pTestObject = pTestObject->pNext;
                                 if  ((strcmp (pTestObject->szWindow, pParentObject->szWindow) == 0) &&
                                      (pTestObject->flStyle & WS_TABSTOP)) {
                                     if  (memcmp (pTestObject->szType, "RBOBJ", 5) == 0) {
                                         OBJECT *pRobject = pTestObject;
                                         int usChecked;

                                         do  {
                                             usChecked = (int) SendMessage (pRobject->hwnd, BM_GETCHECK,
                                                                             0, 0L);
                                             if  (usChecked == 0) {
                                                 if  (pRobject->pNext) {
                                                     pRobject = pRobject->pNext;
                                                 }
                                             }
                                             else {
                                                 pTestObject = pRobject;
                                             }
                                         }
                                         while ((usChecked == 0) &&
                                                (pRobject->pNext) &&
                                                (memcmp (pRobject->szType, "RBOBJ", 5) == 0));

                                     }
                                     PostMessage (hwndParent, WM_FOCUSSET, pTestObject->id, 0L);
                                     strcpy (Command, "FGB-");
                                     if  (memcmp (pTestObject->szType, "PBOBJ", 5) != 0) {
                                         strcat (Command, "FIRST");
                                     }
                                     else {
                                         strcat (Command, pTestObject->szName);
                                     }
                                     w800_default ();
                                     return 0;
//                                   return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
                                 }
                             }
		         }
		         while ((pTestObject->pNext) &&
                                (strcmp (pTestObject->szWindow, pParentObject->szWindow) == 0));

		         //************************************
		         //* Tab to first object on window
		         //************************************
		         pTestObject = &StartObject;
		         do  {
			     if  (pTestObject->pNext) {
			         pTestObject = pTestObject->pNext;
			     }
			     if  ((strcmp (pTestObject->szWindow, pParentObject->szWindow) == 0) &&
			          (pTestObject->flStyle & WS_TABSTOP)) {
                                 if  (memcmp (pTestObject->szType, "RBOBJ", 5) == 0) {
                                     OBJECT *pRobject = pTestObject;
                                     int usChecked;

                                     do  {
                                         usChecked = (int) SendMessage (pRobject->hwnd, BM_GETCHECK,
                                                                        0, 0L);
                                         if  (usChecked == 0) {
                                             if  (pRobject->pNext) {
                                                 pRobject = pRobject->pNext;
                                             }
                                          }
                                          else {
                                              pTestObject = pRobject;
                                          }
                                      }
                                      while ((usChecked == 0) &&
                                             (pRobject->pNext) &&
                                             (memcmp (pRobject->szType, "RBOBJ", 5) == 0));

                                  }
			          PostMessage (hwndParent, WM_FOCUSSET, pTestObject->id, 0L);
                                  strcpy (Command, "FGB-");
                                  if  (memcmp (pTestObject->szType, "PBOBJ", 5) != 0) {
                                      strcat (Command, "FIRST");
                                  }
                                  else {
                                      strcat (Command, pTestObject->szName);
                                  }
                                  w800_default ();
                   //****************************
                   //* Return in Field
                   //****************************
                                  return 0;
//                                return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
			      }
		          }
		          while (pTestObject->pNext);
		          SetFocus (hwnd);
		          return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
                      }
                  }
                  return CallWindowProc (pEditObject->OldProc, hwnd, message, wParam, lParam);
}

/********************************************************************/
/* This module will :                                               */
/*     display an abort message box                                 */
/********************************************************************/
void z111_fatal_message () {

    MessageBeep (MB_ICONHAND);

    if  (!BEDITframe) {
        return;
    }

    memset (temp, 0, Max);
    if  (pCommand) {
        if (strcmp (Error, "Label Not Found") == 0) {
            sprintf (temp, "%s:\n%s\nline number %d\nSee BEDIT.LOG for more details", Error, pCommand->szLine, pCommand->LineNo);
        }
        else {
            sprintf (temp, "%s\n%s\nline number %d\nSee Logfile for more details", Error, buff, pCommand->LineNo);
        }
    }
    else {
        sprintf (temp, "%s\n%s\nSee Logfile for more details", Error, buff);
    }

    MessageBox (BEDITframe,
                temp,
                "BEDITWIN Abort",
                MB_APPLMODAL | MB_OK | MB_ICONHAND);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Close a window by name                                       */
/********************************************************************/
BOOL CALLBACK EnumWindowCloseProc (HWND hwnd, LONG lParam) {
    TCHAR szWindowText[512];
    if (lParam == 1) {
    }
    SendMessage (hwnd, WM_GETTEXT, sizeof(szWindowText), (LPARAM)(void*)szWindowText);

    if  (strlen (szWindowText) >= strlen (pEnumVar->szString)) {
        if  (memcmp (szWindowText+ (strlen (szWindowText) - strlen(pEnumVar->szString)), pEnumVar->szString, strlen (pEnumVar->szString)) == 0) {
            PostMessage (hwnd, WM_CLOSE, 0, 0L);
            //MessageBox (BEDITframe,
            //    szWindowText,
            //    "Debug Closing",
            //    MB_APPLMODAL | MB_OK | MB_ICONHAND);
            //*********************
            //*  Stop Looking
            //*********************
            return FALSE;
        }
    }
    //*********************
    //*  Keep Looking
    //*********************
    return TRUE;    
}

/********************************************************************/
/* This module will :                                               */
/*     Set Focus to a Window by Name                                */
/********************************************************************/
BOOL CALLBACK EnumWindowFocusProc (HWND hwnd, LONG lParam) {
    TCHAR szWindowText[512];
    if (lParam == 1) {
    }
    SendMessage (hwnd, WM_GETTEXT, sizeof(szWindowText), (LPARAM)(void*)szWindowText);

    if  (strlen (szWindowText) >= strlen (pEnumVar->szString)) {
        if  (memcmp (szWindowText+ (strlen (szWindowText) - strlen(pEnumVar->szString)), pEnumVar->szString, strlen (pEnumVar->szString)) == 0) {
            SetForegroundWindow (hwnd);
            return FALSE;
        }
    }
    //*********************
    //*  Keep Looking
    //*********************
    return TRUE;    
}


/************************************************************************/
/*                                                                      */
/*                         End of program BEDIT4PM.c                     */
/*                                                                      */
/************************************************************************/
/* Formatted on 09/21/90 at 14:24:23 by Forrest Bentley's C-Format v1.4 */
