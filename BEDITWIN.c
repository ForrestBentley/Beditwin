/*************************************************************************/
/* Program          : BEDIT.c                   v09112013                */
/* Author           : Forrest Bentley                                    */
/* Date Written     : January 15, 1992                                   */
/*                                                                       */
/* Description      :                                                    *
/*     This program will process all commands in an input command file.  */
/*                                                                       */
/*************************************************************************/
/*                     P R O G R A M    N O T E S                        */
/*************************************************************************/
/*   When started BEDIT initializes the Command File structure to        */
/*                      Input Arguments                                  */
/*                      ERASE *                                          */
/*                      BEDIT                                            */
/*                      EXIT                                             */
/*   This allows BEDIT to call a file & when complete go to interactive. */
/*   If no arguments are passed when BEDIT starts, BEDIT will start in   */
/*   interactive.                                                        */
/*                                                                       */
/*   When a file is called the current state is preserved in the Call    */
/*   Stack structure, and the new file is loaded into a new Command      */
/*   structure.                                                          */
/*                                                                       */
/*   Each Command line is parsed (c120_parse_command) and then the       */
/*   parsed command is processed (c200_process_command). When a command  */
/*   is known processing transfers to the module that handles that       */
/*   command. Some command processing modules will parse more of the     */
/*   command line to retrieve the parameters for the command.            */
/*                                                                       */
/*   (index1 is an integer variable that points to the current position  */
/*   on the command line.                                                */
/*                                                                       */
/*   Multiple commands may be entered on one line.                       */
/*                                                                       */
/*   When running interactive each line keyed in at the BEDIT> prompt    */
/*   is moved into a Command structure and control is passed into        */
/*   the command line process routine to process the Command structure.  */
/*                                                                       */
/*   d580_find_variable finds the variable name stored in Command. This  */
/*   is a completely recursive function if the Command starts with '@'.  */
/*                                                                       */
/*   time is a special variable in that when searching for a user        */
/*   defined variable BEDIT will find time and search from there. If     */
/*   any system variable is added it should be added before time.        */
/*                                                                       */
/*************************************************************************/
/*         M  A  I  N  T  E  N  A  N  C  E         L  O  G               */
/*************************************************************************/
/*  Name              Date       Desc                                    */
/*  ----------------- ---------- --------------------------------------- */
/*  Forrest Bentley   02-13-1992 Added Ruler to DUMP name                */
/*                               Added IF (KEY), IF (NOKEY)              */
/*                               Added IF (NOTEXISTS)                    */
/*                               Added Dump all variables/files on abort */
/*                               Added WAITBELL n                        */
/*                               Added CASE                              */
/*                               Added +,- to FS                         */
/*                               Remove EOF displays if running Batch    */
/*                               Added CHAIN                             */
/*                               Added Sleep                             */
/*                               @ following a name is optional          */
/*                                                                       */
/*  Forrest Bentley   02-16-1992 Added SUBS                              */
/*                               Added IF (NOTEOF)                       */
/*                               Added COLS                              */
/*                               Added NOCO                              */
/*                               Added WRITE *                           */
/*                               Added EXTRACT                           */
/*                               Added DIR                               */
/*                               Added LIST                              */
/*                               Added HELP                              */
/*                               Added FILESIZE                          */
/*                               Added SYSTEMCURR                        */
/*                               Added TRACE                             */
/*                               Added PATTERN                           */
/*                               Added Imbedded Variables in Strings     */
/*                               Added SORT                              */
/*                               Added cut:/xxx/ and copy:/xxx/          */
/*                               Added BEGINEND                          */
/*                               Added STATUS                            */
/*                               No CLS upon entry                       */
/*                               Added CD, MD, and RD                    */
/*  Forrest Bentley   03-03-1992 Corrected EOF_Flag after FILES command  */
/*  Forrest Bentley   03-14-1992 Changed z110 to set Debug = 'Y' to      */
/*                               allow printing vars/files to BEDIT.log  */
/*  Forrest Bentley   03-18-1992 Corrected D to set EOF if D'd last line */
/*  Forrest Bentley   04-03-1992 Version 1.3                             */
/*                               Added if (name)                         */
/*                               Added if (!name)                        */
/*                               Added more interactive quit commands    */
/*                               Added assumed .GLD on CALL commands     */
/*                               Added name ++                           */
/*                               Added name --                           */
/*                               Added ERROR                             */
/*                               Added if (numeric name)                 */
/*                               Added if (!numeric name)                */
/*                               Corrected COLOR, BACKCOLOR, CURSOR      */
/*                               Added CHAR                              */
/*                               Added EOL                               */
/*                               Added Cursor Keys to Interactive Cmds   */
/*                               Added ERASE CMDS                        */
/*                               Added INPUT NAME,#                      */
/*  Forrest Bentley   04-28-1992 Speeded up Label GOTO's (up to 40%)     */
/*                               Added lineno variable                   */
/*                               Added CALL File!Label                   */
/*                               Added lleft variable                    */
/*                               Added lright variable                   */
/*                               Added lmid variable                     */
/*                               Added lline variable                    */
/*                               Added pcount variable                   */
/* Forrest Bentley    05-04-1992 Added FRONT                             */
/*                               Added BACK                              */
/*                               Added IF Var[x-y] logic                 */
/*                               Added AND/OR to IF logic                */
/*                               Added midright variable                 */
/*                               Added lmidright variable                */
/*                               Added lword variable                    */
/*                               Added llword variable                   */
/*                               Added rword variable                    */
/*                               Added lrword variable                   */
/*                               Added logic to scan not found str->left */
/*                               Added allowance of multiple $Var cmds   */
/*                               Corrected Stack Exceeded Error, Not     */
/*                                   finding a label.                    */
/*  Forrest Bentley   05-06-1992 Added funcionkey                        */
/*                               Corrected FRONT/BACK stay on curr line  */
/*                               Corrected Interactive up/down 1st time  */
/*                               Split the code into BEDIT.C & BEDIT2.C  */
/*  Forrest Bentley   05-21-1992 Added BEDITDATA                         */
/*                               Added SAVE SYSVARS                      */
/*                               Added RESTORE SYSVARS                   */
/*                               Added SAVE *                            */
/*                               Added RESTORE *                         */
/*                               Added SAVE name                         */
/*                               Added RESTORE name                      */
/*                               Added SAVE USERVARS                     */
/*                               Added RESTORE USERVARS                  */
/*  Forrest Bentley   09/11/2013 Recompiled to BCC55                     */
/*                               Changed version to 09112013             */
//* Forrest Bentley   09/12/2013 Renamed back to BEDIT                   */
/*  Forrest Bentley   09/25/2013 Added DUMP Processing:                  */
/*                                     DUMP SYSTEM                       */
/*                                     DUMP CUSTOM                       */
/*                                     Line up Dump output               */
//* Forrest Bentley   09/26/2013 Added logic to line up dump field output*/ 
//* Forrest Bentley   10/02/2013 Added Scramble on/off logic             */ 
//* Forrest Bentley   10/03/2013 Added ENCRYPTKEY, ENCRYPT, DECRYPT      */
//*                                    ENCRYPTFILE, DECRYPTFILE          */
//*                                    -E:Key call param to decrypt      */
//* Forrest Bentley   10/22/2013 Added INPUTC function                   */
//* Forrest Bentley   10/23/2013 Added RANDOM function                   */
//* Forrest Bentley   10/29/2013 Changed Sleep from seconds to millesec  */
//* Forrest Bentley   10/30/2013 Added "<-" to INPUTC function           */
//* Forrest Bentley   11/07/2013 Added INPUTP for Password entry         */
//* Forrest Bentley   11/08/2013 Updated ++,-- to include FLOAT,DOLLAR   */
//*                              Added +=, -=, *=, /=                    */
//* Forrest Bentley   12/31/2013 Added STARTSWITH/ENDSWITH               */
//* Forrest Bentley   01/01/2014 Better Error Messages if label not found*/
//* Forrest Bentley   01/03/2014 Added IF-A, ELSE-A more complex IFs     */
//* Forrest Bentley   01/04/2014 corrected List Box mouse usage          */
//* Forrest Bentley   01/08/2014 Added Debug pause from bedit.c          */
//*                              Added window hide/show MAIN             */
//*                              SETFOCUS brings window to foreground    */
//* Forrest Bentley   02/15/2014 Added FIXFILE (correct long line files) */
//* Forrest Bentley   09/18/2014 If -n > 25000, max = 25000              */
/*************************************************************************/

#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
//#include <graph.h>
#include <time.h>
#include <string.h>
#include <MALLOC.h>
#include <conio.h>
#include <io.h>
#include <errno.h>
#include <direct.h>
#include <signal.h>

#include "BEDIT.h"

unsigned char     cFast = 'n';
unsigned char     cBeditDebug;
unsigned char     cEncryptCommand = 'n';
int               iEncryptKeyPtr;
unsigned char     name_cmd_in     [100];            /* Command File Name             */
unsigned char     name_debug_out   [80];            /* Command File Name             */

unsigned char     date_str [9];                     /* system date                   */
unsigned char     time_str [9];                     /* system time                   */

unsigned char     retn [2];                         /* return character string       */
unsigned char     quote [2];                        /* " character string            */
unsigned char     slash [2];                        /* backslash character           */
unsigned char     splitter [2];
unsigned char     *input_line;                      /* input line                    */
unsigned char     *temp;                            /* temporary string work         */
unsigned char     *Command;                         /* parsed command                */
unsigned char     *Error;                           /* Error                         */
unsigned char     *szFileLabel;
unsigned char     *szPartialVar;
int      LineNumber;

unsigned char     eof_file            = 'n';        /* eof for input file            */
unsigned char     eof_cmd_in          = 'n';        /* eof for input file            */
unsigned char     eof_debug_out       = 'n';        /* eof for input file            */

unsigned char     *file_status;                     /* file status for input file    */
unsigned char     bell                = 7;          /* bell sound for errors         */
unsigned char     *buff;                            /* string for outtext fncts      */
unsigned char     *Debugbuff;                       /* string for outtext fncts      */
unsigned char     *Tvariable;                       /* string for outtext fncts      */
unsigned char     *szHoldBuff;                      /* string for outtext fncts      */
unsigned char     response            = ' ';        /* misc response character       */
unsigned char     first               = 'y';        /* first time switch             */
unsigned char     args_supplied       = 'n';        /* file name arguments supplied  */
unsigned char     done                = 'n';        /* done flag                     */
unsigned char     done2               = 'n';        /* second done flag              */
unsigned char     done3               = 'n';        /* done flag                     */
unsigned char     found               = 'n';        /* found flag                    */
unsigned char     found1              = 'n';
unsigned char     found2              = 'n';
unsigned char     More_Commands       = 'n';
unsigned char     Debug               = 'n';
unsigned char     *ErrorLine;
int      ErrorPtr            = 0;
unsigned char     Delimit             = 'n';
unsigned char     Delimiter;
int      DelimitCount        = 0;
unsigned char     *szFSString;
unsigned char     *szFSString2;
unsigned char     Compound;
int      FSCount;
unsigned char     Verify              = 'n';
unsigned char     Veri                = 'n';
unsigned char     InsertBefore        = 'n';
unsigned char     Before              = 'n';
unsigned char     *szFileString;
unsigned char     *szFileName;
unsigned char     *szChangeFrom;
unsigned char     *szChangeTo;
unsigned char     *szConvString;
unsigned char     *szNewString;
int      x1;
int      x2;
int      x3;
unsigned char     Pattany             = '?';
unsigned char     Pattnum             = '#';
unsigned char     Pattalp             = '&';
unsigned char     Pattspc             = '~';
unsigned char     Pattend             = '*';
unsigned char     Patton              = 'n';
long int ChangeNumber        = 0;
long int InputNumber         = 0;
int      ReadCount           = 0;
unsigned int
         MemCount            = 0;
int      pcount              = 0;
unsigned char     cut                 = 'n';
unsigned char     cutcopy             = 'n';
unsigned char     ClearBuff           = 'n';
unsigned char     *Label;
unsigned char     EOF_Flag            = 'n';
unsigned char     Printx              = 'n';
unsigned char     Call                = 'n';
unsigned char     *szVariable;
unsigned char     test                = 'n';
unsigned char     *Var1;
unsigned char     *Var2;
int      Num1;
int      Num2;
unsigned char     Math           [20];
unsigned char     *szIf1;
unsigned char     *szIf2;
unsigned char     *szVar2;
unsigned char     szFunctionkey [20];
unsigned char     len                 = 'n';
unsigned char     Word                = 'n';
unsigned char     interactive         = 'n';
unsigned char     BEDITargs     [100];
unsigned char     BEDITMore_Commands  = 'n';
unsigned char     Abort               = 'n';
unsigned char     Subs                = '@';
unsigned char     Recursion           = 'n';
unsigned char     Cols                = 'n';
int      ColStart            = 0;
int      ColEnd              = 0;
int      ExtrStart           = 0;
int      ExtrLen             = 0;
unsigned char     trace               = 'n';
unsigned char     SortType       [10];
unsigned char     SortPos        [10];
int      SortPosNum          = 0;
unsigned char     BeginEnd            = 0;
int      BEDITlevel          = 0;
unsigned char     FoundVar            = 'n';
unsigned char     FileError           = 'n';
unsigned char     FirstInteractive    = 'y';
unsigned char     CallVar             = 'n';

int      index1              = 0;          /* input line index              */
int      holdindex1          = 0;
int      BEDITindex1         = 0;
int      index2              = 0;          /* output line index             */
int      index3              = 0;          /* token index                   */
int      index4              = 0;          /* misc work index               */
int      index7              = 0;          /* misc work index               */
int      index8              = 0;          /* misc work index               */
int      GotoIndex1          = 0;
unsigned char     Or_Cond             = 'n';
unsigned char     Ok_If               = 'n';

int      count               = 0;
int      LineNo              = 0;

int      count_file          = 0;          /* input line count              */
int      count_cmd_in        = 0;          /* input line count              */
int      count_debug_out     = 0;          /* input line count              */

long int backcolor           = 0;          /* backround color for outtext   */
int      nrmcolor            = 3;          /* normal color for outtext      */
int      highcolor           = 11;         /* high color for outtext        */
int      warncolor           = 12;         /* warning color for outtext     */
int      warn2color          = 14;         /* warning 2 color for outtext   */
int      row                 = 0;          /* row for outtext               */
int      col                 = 0;          /* column for outtext            */
int      hrow                = 0;          /* hold row for display          */
int      hcol                = 0;          /* hold col for display          */

long int Number;
long int Number2;
long int Tnum1;
long int Tnum2;
unsigned char     *p;
int      mode;
int      RC;
unsigned long ulSleep;

unsigned char     *szScanStr;
unsigned char     *szScanLeft;
unsigned char     *szScanRight;
unsigned char     *szScanTest;
unsigned char     *szScanMid;
unsigned char     *szLEFT;
unsigned char     *szScanKeep;
unsigned char     *szLword;
unsigned char     *szRword;
int      Lwordindex          = 0;
unsigned char     cScanFound          = 'n';
unsigned char     cScanDone           = 'n';
unsigned char     cScanFromRight      = 'n';
unsigned char     Scanr               = 'n';
unsigned char     cScanChar1;
unsigned char     cScanChar2;
unsigned char     cCase               = 'y';
int      ScanIndex1;
int      ScanIndex2;
unsigned char     szCursorCmd   [10];
unsigned char     Direction;
int      Max                           = 3000;
int      iScramble;
unsigned char     szEncryptKey  [100];
unsigned char     szDefaultKey [] = "BEditEncryptionKey09112013";

unsigned char          *envptr;
unsigned char          *envstr;
unsigned char          Break                    = 'n';
unsigned char          BreakStatus              = 'y';
unsigned char          If_Proc                  = 'n';
unsigned char          Dountil                  = 'n';
unsigned char          szIfName [100];
int           dCallFileLevel           = 0;
unsigned char          Callfile                 = 'n';
unsigned char          cLocal                   = 'n';
unsigned char          cArg                     = 'n';
unsigned char          cLocalVars               = 'n';
int           CallLevel;
unsigned char          szCurrentFile            [100];

unsigned char          szMainTitle              [100];

unsigned char          Beditwin;
short         sInterval;

HANDLE              hInstance;
HANDLE              hPrevInstance;
char szAppName [] = "Bedit Windows";
WNDCLASS            wndclass;
WNDCLASS            wndclass2;
RECT                rect;
HMENU               hMenu;
HMENU               hMenuPopup;
MSG                 msg;
int                 nCmdShow;
char                cUpdateCursor = 'y';
FARPROC             lpfnSubClass;

char szBeditSecondary [] = "Beditwin2";

//HAB           hab;
//HMQ           hmq;
//QMSG          qmsg;
HWND          BEDITframe;
HWND          BEDITclient;
//ULONG         frameflags  = FCF_TITLEBAR      | FCF_SYSMENU  |
//                            FCF_SIZEBORDER    | FCF_MINMAX   |
//                            FCF_SHELLPOSITION | FCF_TASKLIST |
//                            FCF_MENU | FCF_ICON;
//SWP           swp;
//HSEM          hsemApp;
//USHORT        usRc;
HWND          hwndMain;
//HMODULE       hmod        = 0;
//LONG          FontsAvail  = 5;
//FATTRS        vfat;
LONG          FontSize;
LONG          FixedFont   = 0;
unsigned char          szCountry [30];
unsigned char          cAutotab    = 'n';
int           XMouse;
int           YMouse;
unsigned char          cMouse      = 'y';
char     szPointer [20] = "ARROW";
int ExecFullScreen = 0;

FILE     *file_cmd_in;                     /* command file                  */
FILE     *file_debug_out;                  /* command file                  */
FILE     *file_ot;                         /* write file                    */

INTERACTIVE   StartInteractive;
INTERACTIVE   *pInteractive;
INTERACTIVE   *pNewInteractive;
INTERACTIVE   *pCurrInteractive;

CALL          StartCall;
CALL          *pCall;
CALL          *pNewCall;
CALL          *pPrevCall;
CALL          *pNextCall;
CALL          *pHoldCall;
CALL          *pCountCall;

VARIABLE      StartVariable;
VARIABLE      *pVariable;
VARIABLE      *pEnumVar;
VARIABLE      *pVariableRecursive;
VARIABLE      *pHoldVariable;
VARIABLE      *pNewVariable;
VARIABLE      *pPrevVariable;
VARIABLE      *pNextVariable;
VARIABLE      *pAssignVar;
VARIABLE      *pVar1;
VARIABLE      *pVar2;
VARIABLE      *pCountVariable;

COMMAND       StartCommand;
COMMAND       *pCommand;
COMMAND       *pNewCommand;
COMMAND       *pHoldCommand;
COMMAND       *pStartCommand;
COMMAND       *pStartCommandBEDIT;
COMMAND       *pCommandBEDIT;
COMMAND       *pLabel;
COMMAND       *pPMCommand;
COMMAND       *pLastCallGoto;
COMMAND       *pDountilcommand;

PERMENANT     StartPermenant;
PERMENANT     *pPermenant;
PERMENANT     *pNewPermenant;

BUFFER        StartBuffer;
BUFFER        *pBuffer;
BUFFER        *pNewBuffer;
BUFFER        *pCountBuffer;

DATAFILE      StartDataFile;
DATAFILE      *pDataFile;
DATAFILE      *pNewDataFile;
DATAFILE      *pCurrData;
DATAFILE      *pPrevData;
DATAFILE      *pNextData;
DATAFILE      *pCountDataFile;

OPENFILE      StartOpenFile;
OPENFILE      *pOpenFile;
OPENFILE      *pNewOpenFile;
OPENFILE      *pNextFile;
OPENFILE      *pPrevFile;
OPENFILE      *pCountFiles;
OPENFILE      *pOpenCurr;

OBJECT        StartObject;
OBJECT        *pObject;
OBJECT        *pNewObject;
OBJECT        *pNextObject;
OBJECT        *pPrevObject;
OBJECT        *pCountObject;

ARGS          StartArgs;
ARGS          *pArgs;
ARGS          *pNextArgs;
ARGS          *pPrevArgs;

int WindowCount;

/********************************************************************/
/* This module will :                                               */
/*     Process the major process modules.                           */
/********************************************************************/
int PASCAL WinMain (HINSTANCE hMainInstance, HINSTANCE hMainPrevInstance,
                    LPSTR lpszCmdParam, int nMainCmdShow)
    {

    hInstance = hMainInstance;
    hPrevInstance = hMainPrevInstance;
    nCmdShow = nMainCmdShow;

    /********************************************/
    /* set up the file names if supplied as     */
    /* parameters.                              */
    /********************************************/
    args_supplied = 'n';
    memset (BEDITargs, 0, sizeof (BEDITargs));
    strcpy (BEDITargs, lpszCmdParam);

    if  (strcmp (BEDITargs, "") != 0) {
        args_supplied = 'y';
        if  (BEDITargs [0] == '-') {         // Max Characters
            if  (BEDITargs [1] == 'F') {
                cFast = 'y';
                strcpy (szMainTitle, BEDITargs + 3);
                strcpy (BEDITargs, szMainTitle);
            }
            if  (BEDITargs [1] == 'D') {
                cBeditDebug = 'y';
            }
        }
        if  ((BEDITargs [0] == '-') && (BEDITargs [1] == 'E') && (BEDITargs [2] == ':') && (strlen(BEDITargs) > 3)) {
            int ix1 = 2;
            char cDone = 'n';
            do {
                ix1++;
                if  ((BEDITargs [ix1] == ' ') || (BEDITargs [ix1] == 0)) {
                    cDone = 'y';
                }
            } while (cDone != 'y'); 
            BEDITargs [ix1] = 0;
            cEncryptCommand = 'y';
            strcpy (szEncryptKey, BEDITargs + 3);
            strcpy (szMainTitle, BEDITargs + ix1 + 1);
            strcpy (BEDITargs, szMainTitle);
            if  (strcmp (szEncryptKey, "DEFAULT") == 0) {
                strcpy (szEncryptKey, szDefaultKey);
            }
         }
        if  (BEDITargs [0] == '-') {
            Number = 0;
            Number = (long) strtol ((char *)BEDITargs + 1, (char **) &p, 10);
            if  (Number > 25000) {
                Number = 25000;
            }
            if  ((Number > 99) && (Number <= 25000)) {
                Max = (int) Number;
            }
            strcpy (szMainTitle, p+1);
            strcpy (BEDITargs, szMainTitle);
        }
        if  (cBeditDebug == 'y') {
            Max = 25000;
        }
    }

    /* printf ("%s", "main () {\n"); */

    /*------------------------------------------------------*/
    /*       perform main processing routines               */
    /*------------------------------------------------------*/
    b100_init ();
    b200_proc ();
    b300_wrap ();
    return (0);
}

/********************************************************************/
/* This module will :                                               */
/*     Perform all initialize functions.                            */
/********************************************************************/
void b100_init () {
//    printf ("%s", "b100_init () {\n");

    printf ("************************************************************\n");
    printf ("*  BEDIT v09112013 for DOS Windows 7 (Forrest Bentley)     *\n");
    printf ("************************************************************\n");
    remove ("BEDITexec.bat");
    remove ("BEDITexec.sem");

    iScramble = 0;
    strcpy (szCountry, "US");
    /***************************/
    /* Create Variable Storage */
    /***************************/
    /***************************/
    /* Error */
    /***************************/
    Error = (unsigned char *) malloc (Max + 100);
    if  (!Error) {
        strcpy (Error, "CrError");
        z110_fatal ();
        return;
    }
    memset (Error, 0, Max);
    /***************************/
    /* input_line */
    /***************************/
    input_line = (unsigned char *) malloc (Max);
    if  (!input_line) {
        strcpy (Error, "Crinput_line");
        z110_fatal ();
        return;
    }
    memset (input_line, 0, Max);
    /***************************/
    /* temp */
    /***************************/
    temp = (unsigned char *) malloc (Max);
    if  (!temp) {
        strcpy (Error, "Crtemp");
        z110_fatal ();
        return;
    }
    memset (temp, 0, Max);
    /***************************/
    /* Command */
    /***************************/
    Command = (unsigned char *) malloc (Max);
    if  (!Command) {
        strcpy (Error, "CrCommand");
        z110_fatal ();
        return;
    }
    memset (Command, 0, Max);

    /***************************/
    /* szFileLabel */
    /***************************/
    szFileLabel = (unsigned char *) malloc (Max);
    if  (!szFileLabel) {
        strcpy (Error, "CrszFileLabel");
        z110_fatal ();
        return;
    }
    memset (szFileLabel, 0, Max);
    /***************************/
    /* szPartialVar */
    /***************************/
    szPartialVar = (unsigned char *) malloc (Max);
    if  (!szPartialVar) {
        strcpy (Error, "CrszPartialVar");
        z110_fatal ();
        return;
    }
    memset (szPartialVar, 0, Max);
    /***************************/
    /* buff */
    /***************************/
    buff = (unsigned char *) malloc (Max);
    if  (!buff) {
        strcpy (Error, "Crbuff");
        z110_fatal ();
        return;
    }
    memset (buff, 0, Max);
    /***************************/
    /* Debugbuff */
    /***************************/
    Debugbuff = (unsigned char *) malloc (Max);
    if  (!Debugbuff) {
        strcpy (Error, "CrDebugbuff");
        z110_fatal ();
        return;
    }
    memset (Debugbuff, 0, Max);
    /***************************/
    /* Tvariable */
    /***************************/
    Tvariable = (unsigned char *) malloc (Max);
    if  (!Tvariable) {
        strcpy (Error, "CrTvariable");
        z110_fatal ();
        return;
    }
    memset (Tvariable, 0, Max);
    /***************************/
    /* szHoldBuff */
    /***************************/
    szHoldBuff = (unsigned char *) malloc (Max);
    if  (!szHoldBuff) {
        strcpy (Error, "CrszHoldBuff");
        z110_fatal ();
        return;
    }
    memset (szHoldBuff, 0, Max);
    /***************************/
    /* ErrorLine */
    /***************************/
    ErrorLine = (unsigned char *) malloc (Max + 100);
    if  (!ErrorLine) {
        strcpy (Error, "CrErrorLine");
        z110_fatal ();
        return;
    }
    memset (ErrorLine, 0, Max);
    /***************************/
    /* szFSString */
    /***************************/
    szFSString = (unsigned char *) malloc (Max);
    if  (!szFSString) {
        strcpy (Error, "CrszFSString");
        z110_fatal ();
        return;
    }
    memset (szFSString, 0, Max);
    /***************************/
    /* szFSString2 */
    /***************************/
    szFSString2 = (unsigned char *) malloc (Max);
    if  (!szFSString2) {
        strcpy (Error, "CrszFSString2");
        z110_fatal ();
        return;
    }
    memset (szFSString2, 0, Max);
    /***************************/
    /* szFileString */
    /***************************/
    szFileString = (unsigned char *) malloc (Max);
    if  (!szFileString) {
        strcpy (Error, "CrszFileString");
        z110_fatal ();
        return;
    }
    memset (szFileString, 0, Max);
    /***************************/
    /* szFileName */
    /***************************/
    szFileName = (unsigned char *) malloc (Max);
    if  (!szFileName) {
        strcpy (Error, "CrszFileName");
        z110_fatal ();
        return;
    }
    memset (szFileName, 0, Max);
    /***************************/
    /* szChangeFrom */
    /***************************/
    szChangeFrom = (unsigned char *) malloc (Max);
    if  (!szChangeFrom) {
        strcpy (Error, "CrszChangeFrom");
        z110_fatal ();
        return;
    }
    memset (szChangeFrom, 0, Max);
    /***************************/
    /* szChangeTo */
    /***************************/
    szChangeTo = (unsigned char *) malloc (Max);
    if  (!szChangeTo) {
        strcpy (Error, "CrszChangeTo");
        z110_fatal ();
        return;
    }
    memset (szChangeTo, 0, Max);
    /***************************/
    /* szConvString */
    /***************************/
    szConvString = (unsigned char *) malloc (Max);
    if  (!szConvString) {
        strcpy (Error, "CrszConvString");
        z110_fatal ();
        return;
    }
    memset (szConvString, 0, Max);
    /***************************/
    /* szNewString */
    /***************************/
    szNewString = (unsigned char *) malloc (Max);
    if  (!szNewString) {
        strcpy (Error, "CrszNewString");
        z110_fatal ();
        return;
    }
    memset (szNewString, 0, Max);
    /***************************/
    /* Label */
    /***************************/
    Label = (unsigned char *) malloc (Max);
    if  (!Label) {
        strcpy (Error, "CrLabel");
        z110_fatal ();
        return;
    }
    memset (Label, 0, Max);
    /***************************/
    /* szVariable */
    /***************************/
    szVariable = (unsigned char *) malloc (Max);
    if  (!szVariable) {
        strcpy (Error, "CrszVariable");
        z110_fatal ();
        return;
    }
    memset (szVariable, 0, Max);
    /***************************/
    /* Var1 */
    /***************************/
    Var1 = (unsigned char *) malloc (Max);
    if  (!Var1) {
        strcpy (Error, "CrVar1");
        z110_fatal ();
        return;
    }
    memset (Var1, 0, Max);
    /***************************/
    /* Var2 */
    /***************************/
    Var2 = (unsigned char *) malloc (Max);
    if  (!Var2) {
        strcpy (Error, "CrVar2");
        z110_fatal ();
        return;
    }
    memset (Var2, 0, Max);
    /***************************/
    /* szIf1 */
    /***************************/
    szIf1 = (unsigned char *) malloc (Max);
    if  (!szIf1) {
        strcpy (Error, "CrszIf1");
        z110_fatal ();
        return;
    }
    memset (szIf1, 0, Max);
    /***************************/
    /* szIf2 */
    /***************************/
    szIf2 = (unsigned char *) malloc (Max);
    if  (!szIf2) {
        strcpy (Error, "CrszIf2");
        z110_fatal ();
        return;
    }
    memset (szIf2, 0, Max);
    /***************************/
    /* szVar2 */
    /***************************/
    szVar2 = (unsigned char *) malloc (Max);
    if  (!szVar2) {
        strcpy (Error, "CrszVar2");
        z110_fatal ();
        return;
    }
    memset (szVar2, 0, Max);
    /***************************/
    /* szScanStr */
    /***************************/
    szScanStr = (unsigned char *) malloc (Max);
    if  (!szScanStr) {
        strcpy (Error, "CrszScanStr");
        z110_fatal ();
        return;
    }
    memset (szScanStr, 0, Max);
    /***************************/
    /* szScanLeft */
    /***************************/
    szScanLeft = (unsigned char *) malloc (Max);
    if  (!szScanLeft) {
        strcpy (Error, "CrszScanLeft");
        z110_fatal ();
        return;
    }
    memset (szScanLeft, 0, Max);
    /***************************/
    /* szScanRight */
    /***************************/
    szScanRight = (unsigned char *) malloc (Max);
    if  (!szScanRight) {
        strcpy (Error, "CrszScanRight");
        z110_fatal ();
        return;
    }
    memset (szScanRight, 0, Max);
    /***************************/
    /* szScanTest */
    /***************************/
    szScanTest = (unsigned char *) malloc (Max);
    if  (!szScanTest) {
        strcpy (Error, "CrszScanTest");
        z110_fatal ();
        return;
    }
    memset (szScanTest, 0, Max);
    /***************************/
    /* szScanMid */
    /***************************/
    szScanMid = (unsigned char *) malloc (Max);
    if  (!szScanMid) {
        strcpy (Error, "CrszScanMid");
        z110_fatal ();
        return;
    }
    memset (szScanMid, 0, Max);
    /***************************/
    /* szLEFT */
    /***************************/
    szLEFT = (unsigned char *) malloc (Max);
    if  (!szLEFT) {
        strcpy (Error, "CrszLEFT");
        z110_fatal ();
        return;
    }
    memset (szLEFT, 0, Max);
    /***************************/
    /* szScanKeep */
    /***************************/
    szScanKeep = (unsigned char *) malloc (Max);
    if  (!szScanKeep) {
        strcpy (Error, "CrszScanKeep");
        z110_fatal ();
        return;
    }
    memset (szScanKeep, 0, Max);
    /***************************/
    /* szLword */
    /***************************/
    szLword = (unsigned char *) malloc (Max);
    if  (!szLword) {
        strcpy (Error, "CrszLword");
        z110_fatal ();
        return;
    }
    memset (szLword, 0, Max);
    /***************************/
    /* szRword */
    /***************************/
    szRword = (unsigned char *) malloc (Max);
    if  (!szRword) {
        strcpy (Error, "CrszRword");
        z110_fatal ();
        return;
    }
    memset (szRword, 0, Max);
    /***************************/
    /* envstr */
    /***************************/
    envstr = (unsigned char *) malloc (Max);
    if  (!envstr) {
        strcpy (Error, "Crenvstr");
        z110_fatal ();
        return;
    }
    memset (envstr, 0, Max);

    /***************************/
    /* Control-Break Process   */
    /***************************/
    Break = 'n';
    BreakStatus = 'y';
    signal (SIGINT, x100_break);

    /***************************/
    /* Initialize Strings      */
    /***************************/
    Direction = 'D';
    strcpy (name_debug_out, "BEDIT.LOG");
    retn [0] = 10;
    retn [1] = 0;
    quote [0] = 34;
    quote [1] = 0;
    slash [0] = 92;
    slash [1] = 0;
    splitter [0] = 219;
    splitter [1] = 0;
    _strdate (date_str);
    _strtime (time_str);
    memset (szScanStr, 0, Max);
    memset (szScanTest, 0, Max);
    memset (szScanRight, 0, Max);
    memset (szScanLeft, 0, Max);
    memset (szCurrentFile, 0, sizeof (szCurrentFile));

//    BEDITframe = (HWND) NULL;
//    hab = (HAB) NULL;
//    hmq = (HMQ) NULL;
    Beditwin = 'n';
    sInterval = 0;
    w100_test_Beditwin ();

    /***************************/
    /* Init DPATH for CALL's   */
    /***************************/
    memset (envstr, 0, Max);
    strcpy (envstr, ";");
    envptr = getenv ("BEDIT");
    if  (envptr) {
        strcat (envstr, envptr);
        strcat (envstr, ";");
    }
    envptr = getenv ("DPATH");
    if  (envptr) {
        strcat (envstr, envptr);
        strcat (envstr, ";");
    }

    /***************************/
    /* Initialize Structures   */
    /***************************/
    pOpenFile = &StartOpenFile;
    pOpenFile->pNext = NULL;
    pOpenFile = NULL;

    pCall = &StartCall;
    pCall->pNext = NULL;
    pCall->pPrev = NULL;
    pCall->index1 = 0;
    pCall->pCommand = NULL;

    pPermenant = &StartPermenant;
    pPermenant->pNext = NULL;
    pPermenant->pPrev = NULL;

    pVariable = &StartVariable;
    pVariable->pNext = NULL;
    pVariable->pPrev = NULL;

    pBuffer = &StartBuffer;
    strcpy (pBuffer->szLine, "");
    pBuffer->pNext = NULL;
    pBuffer->pPrev = NULL;

    pInteractive = &StartInteractive;
    pInteractive->pNext = NULL;
    pInteractive->pPrev = NULL;

    pPMCommand = NULL;
    pObject = &StartObject;
    pObject->pNext = NULL;
    pObject->pPrev = NULL;
    pObject->pNextWin = NULL;
    pObject->pNextWinObj = NULL;

    /**************************/
    /* Create Standard Vars   */
    /**************************/
    strcpy (input_line, "line");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "left");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "mid");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "right");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "midright");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "lword");
    index1 = 0;
    d510_string ();
    strcpy (input_line, "rword");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "null");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "found");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "partial1");
    index1 = 0;
    d510_string ();
    strcpy (input_line, "partial2");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "functionkey");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "pcount");
    index1 = 0;
    d500_integer ();

    strcpy (input_line, "lineno");
    index1 = 0;
    d500_integer ();

    strcpy (input_line, "lline");
    index1 = 0;
    d500_integer ();
    strcpy (input_line, "lleft");
    index1 = 0;
    d500_integer ();
    strcpy (input_line, "lmid");
    index1 = 0;
    d500_integer ();
    strcpy (input_line, "lright");
    index1 = 0;
    d500_integer ();
    strcpy (input_line, "lmidright");
    index1 = 0;
    d500_integer ();
    strcpy (input_line, "llword");
    index1 = 0;
    d500_integer ();
    strcpy (input_line, "lrword");
    index1 = 0;
    d500_integer ();
    strcpy (input_line, "calllevel");
    index1 = 0;
    d500_integer ();

    strcpy (input_line, "message");
    index1 = 0;
    d510_string ();
    strcpy (input_line, "status");
    index1 = 0;
    d510_string ();
    strcpy (input_line, "tab");
    index1 = 0;
    d510_string ();
    
    strcpy (input_line, "encryptkey");
    index1 = 0;
    d510_string ();

    strcpy (input_line, "date");
    index1 = 0;
    d510_string ();
    strcpy (input_line, "time");
    index1 = 0;
    d510_string ();

    /********************************/
    /* Initialize Variables         */
    /********************************/
    strcpy (Command, "date");
    d580_find_variable ();
    strcpy (Command, "time");
    d580_find_variable ();
    strcpy (Command, "tab");
    d580_find_variable ();
    sprintf (pVariable->szString, "%c", '\t');

//    w100_heading ();
//    printf ("%s", "This program will read a Command File and process the\n");
//    printf ("%s", "editing commands.\n");
//    printf ("%s", "\n");
//    sprintf (buff, "Processing started on %s at %s.\n", date_str, time_str);
//    printf ("%s", buff);

      printf ("%s", "\n");

    /************************************/
    /* Set up Initial BEDIT interactive */
    /*         Arguments                */
    /*         ERASE *                  */
    /*         BEDIT                    */
    /*         EXIT                     */
    /************************************/
    pStartCommand = &StartCommand;
    pCommand = pStartCommand;
    pCommand->pNext = NULL;
    pCommand->pPrev = NULL;
    pCommand->szLine = (unsigned char *) malloc (Max);
    if  (!pCommand->szLine) {
        printf ("%s", "Unable to Initialize Beginning Line!\n");
        exit (1);
    }
    strcpy (pCommand->szLine, "");
    LineNo = 0;

    /************************/
    /* Arguments            */
    /************************/
    if  (BEDITargs [0] != 0) {
        pNewCommand = (COMMAND *) malloc (sizeof (COMMAND));
        if  (!pNewCommand) {
            printf ("%s", "Unable to Initialize Args!\n");
            exit (1);
        }
        memset (pNewCommand, 0, sizeof (COMMAND));
        LineNo++;
        pNewCommand->LineNo = LineNo;
        pCommand->pNext = pNewCommand;
        pNewCommand->pPrev = pCommand;
        pNewCommand->pNext = NULL;
        pNewCommand->szLine = (unsigned char *) malloc (Max);
        if  (!pNewCommand->szLine) {
            printf ("%s", "Unable to Initialize Args Line!\n");
            exit (1);
        }
        memset (pNewCommand->szLine, 0, Max);
        strcpy (pNewCommand->szLine, BEDITargs);
        pCommand = pNewCommand;
    }

    /************************/
    /* ERASE *              */
    /************************/
    if  (cFast != 'y') {
        pNewCommand = (COMMAND *) malloc (sizeof (COMMAND));
        if  (!pNewCommand) {
            printf ("%s", "Unable to Initialize Erase!\n");
            exit (1);
        }
        memset (pNewCommand, 0, sizeof (COMMAND));
        LineNo++;
        pNewCommand->LineNo = LineNo;
        pCommand->pNext = pNewCommand;
        pNewCommand->pPrev = pCommand;
        pNewCommand->pNext = NULL;
        pNewCommand->szLine = (unsigned char *) malloc (Max);
        if  (!pNewCommand->szLine) {
            printf ("%s", "Unable to Initialize Erase Line!\n");
            exit (1);
        }
        memset (pNewCommand->szLine, 0, Max);
        strcpy (pNewCommand->szLine, "ERASE *");
        pCommand = pNewCommand;

        pBuffer = &StartBuffer;
        strcpy (pBuffer->szLine, "");
        pBuffer->pNext = NULL;
        pBuffer->pPrev = NULL;
    }

    /************************/
    /* BEDIT                */
    /************************/
    pNewCommand = (COMMAND *) malloc (sizeof (COMMAND));
    if  (!pNewCommand) {
        printf ("%s", "Unable to Initialize BEDIT!\n");
        exit (1);
    }
    memset (pNewCommand, 0, sizeof (COMMAND));
    LineNo++;
    pNewCommand->LineNo = LineNo;
    pCommand->pNext = pNewCommand;
    pNewCommand->pPrev = pCommand;
    pNewCommand->pNext = NULL;
        pNewCommand->szLine = (unsigned char *) malloc (Max);
        if  (!pNewCommand->szLine) {
            printf ("%s", "Unable to Initialize BEDIT Line!\n");
            exit (1);
        }
    memset (pNewCommand->szLine, 0, Max);
    strcpy (pNewCommand->szLine, "BEDIT");
    pCommand = pNewCommand;

    /************************/
    /* EXIT                 */
    /************************/
    pNewCommand = (COMMAND *) malloc (sizeof (COMMAND));
    if  (!pNewCommand) {
        printf ("%s", "Unable to Initialize EXIT!\n");
        exit (1);
    }
    memset (pNewCommand, 0, sizeof (COMMAND));
    LineNo++;
    pNewCommand->LineNo = LineNo;
    pCommand->pNext = pNewCommand;
    pNewCommand->pPrev = pCommand;
    pNewCommand->pNext = NULL;
        pNewCommand->szLine = (unsigned char *) malloc (Max);
        if  (!pNewCommand->szLine) {
            printf ("%s", "Unable to Initialize Exit Line!\n");
            exit (1);
        }
    memset (pNewCommand->szLine, 0, Max);
    strcpy (pNewCommand->szLine, "EXIT");
    pCommand = pNewCommand;

//  system ("cls");

//  printf ("%s", "\n\n              Processing Commands\n");
//  printf ("%s",
//  "----------------------------------------------------------------------\n\n"
//      );
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Perform modules to read lines and process them.              */
/********************************************************************/
void b200_proc () {
    /* printf ("%s", "b200_proc () {\n"); */
    eof_cmd_in = 'n';
    if  (Break != 'y') {
        pCommand = pStartCommand;
    }
    do  {
        if  (Break == 'y') {
            Break = 'X';
        }
        else {
            c110_proc_next_cmd ();
            Yield ();
        }
        if  (eof_cmd_in != 'y') {
            index1 = 0;
            More_Commands = 'y';
            do  {
                c120_parse_command ();
                if  (More_Commands == 'y') {
                    c200_process_command ();
                }
            }
            while (More_Commands == 'y');
        }
    }
    while (eof_cmd_in != 'y');
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Perform all wrapup functions.                                */
/********************************************************************/
void b300_wrap () {
    /* printf ("%s", "b300_wrap () {\n"); */

//    system ("cls");
//    w100_heading ();

//    if  (BEDITframe) {
//        WinDestroyWindow(BEDITframe);
//    }
//    if  (hmq) {
//        WinDestroyMsgQueue(hmq);
//    }
//    if  (hab) {
//        WinTerminate(hab);
//    }

//    printf ("\033[0m"); /* restore original Attributes */

    remove ("BEDITexec.bat");
    remove ("BEDITexec.sem");

//    printf ("%s", "\n");
//    printf ("%s", "\nBEDIT v09112013 Processing Completed (Forrest Bentley)\n");

//    DestroyWindow (GetActiveWindow ());
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Read one line of the input command file                      */
/********************************************************************/
void c100_read_cmd_in () {
    file_status = fgets (input_line, Max, file_cmd_in);
    if  (file_status == NULL) {
        eof_cmd_in = 'y';
    }
    else {
        count_cmd_in++;
        if  (cEncryptCommand == 'y') {
            e105_encrypt (0, 'c');
            //printf (szEncryptKey);
            //printf (input_line); printf ("\n");
        }
        index1 = strlen (input_line);
        do  {
            index1--;
            if  (input_line [index1] < 32) {
                input_line [index1] = 0;
            }
            else {
                index1 = 0;
            }
        }
        while (index1 != 0);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Read one line of the input command file                      */
/********************************************************************/
void c110_proc_next_cmd () {
    /*************************************/
    /* IF continuation line + Processing */
    /*************************************/
    do  {
        if  (!pCommand->pNext) {
            eof_cmd_in = 'y';
            return;
        }
        pCommand = pCommand->pNext;
        strcpy (input_line, pCommand->szLine);
        if  (input_line [0] != '+') {
            If_Proc = 'n';
            Dountil = 'n';
        }
        if  ((input_line [0] == '+') &&
             (If_Proc == 'y')) {
            input_line [0] = ' ';
        }
        if  ((input_line [0] == '+') &&
             (If_Proc != 'y')) {
            /*************************************************/
            /* See if the first command on the line is ELSE  */
            /*************************************************/
            index1 = 0;
            do  {
                index1++;
                if  ((input_line [index1] != ' ') &&
                     (memcmp (input_line + index1, "ELSE", 4) != 0) &&
                     (memcmp (input_line + index1, "else", 4) != 0)) {
                    input_line [index1] = 0;
                }
            }
            while ((memcmp (input_line + index1, "ELSE", 4) != 0) &&
                   (memcmp (input_line + index1, "else", 4) != 0) &&
                   (input_line [index1] > 30));
            if  ((memcmp (input_line + index1, "ELSE", 4) == 0) ||
                 (memcmp (input_line + index1, "else", 4) == 0)) {
                input_line [0] = ' ';
            }
        }
    }
    while (input_line [0] == '+');

    //************************************
    //* B-Edit Debugging
    //************************************
    if  (cBeditDebug == 'y') {
        char DebugProceed;
        char cFileList;
        int  DebugLastBreakLine;
        char cBreakType = 's';

        //printf("Debug Main\n");
        
        do {
            DebugProceed = 'y';
            //*******************************
            //* Write the Beditsem.stp file
            //* Indicating which line
            //* will be processed
            //*******************************
            RC = access("beditsem.go", 0);
            if  ((RC != 0) && (cFileList != 'y')) {
                FILE *BeditBugFile;
                char szBugOutput[100];
                VARIABLE *pDebugVar;

                BeditBugFile = fopen ("beditsem.stp", "w");
                //*************************************
                //* Write the Line number and Filename
                //*************************************
                //z999_open_bug ();
                //if (BEDITframe) {
                //    sprintf(buff, "Beditwin[%c] BEDITframe[%ld]\n", Beditwin, (long)BEDITframe);
                //}
                //else {
                //      sprintf(buff, "Beditwin[%c] BEDITframe[NONE]\n", Beditwin);
                //}
                //fputs (buff, file_debug_out);
                if  (Beditwin == 'y') {
                    sprintf(szBugOutput, "%i:%c:%ld:%s\n", pCommand->LineNo, cBreakType, (long) BEDITframe, pCall->szFilename);
                }
                else {
                   sprintf(szBugOutput, "%i:%c::%s\n", pCommand->LineNo, cBreakType, pCall->szFilename);
                }

                fputs (szBugOutput, BeditBugFile);

                //*************************************
                //* Write the Variables names and contents
                //*************************************
                pDebugVar = &StartVariable;
                do  {
                    if  (pDebugVar->pNext) {
                        pDebugVar = pDebugVar->pNext;
                        if  (pDebugVar->Type == 'S') {
                            if  (strcmp (pDebugVar->szName, "encryptkey") == 0) {
                                 //sprintf (buff, "%s[%s] - string\n", pDebugVar->szName, szEncryptKey);
                                 sprintf (buff, "%s[%s] - string\n", pDebugVar->szName, "");
                            }
                            else {
                                if (strcmp (pDebugVar->szEdit, "") == 0) {
                                    sprintf (buff, "%s[%s] - string\n", pDebugVar->szName, pDebugVar->szString);
                                }
                                else {
                                    if  (strcmp (pDebugVar->szEdit, "FLOAT") == 0) {
                                        sprintf (buff, "%s[%s] - float\n", pDebugVar->szName, pDebugVar->szString, pDebugVar->szEdit);
                                    }
                                    else { 
                                        if  (strcmp (pDebugVar->szEdit, "DOLLAR") == 0) {
                                            sprintf (buff, "%s[%s] - dollar\n", pDebugVar->szName, pDebugVar->szString);
                                        }
                                        else {
                                            sprintf (buff, "%s[%s] - %s\n", pDebugVar->szName, pDebugVar->szString, pDebugVar->szEdit);
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            sprintf (buff, "%s[%d] - integer\n", pDebugVar->szName, pDebugVar->Integer);
                        }  
                        fputs (buff, BeditBugFile);
                    }
                }
                while (pDebugVar->pNext);
                //printf("Variable List Written to beditsem.stp\n");


                fclose (BeditBugFile);  
                //Sleep ((DWORD)100);          
            }
            cFileList = 'n';
            //*******************************
            //* Wait for the indication to
            //* process the line
            //*******************************
            RC = 1;
            do {
                RC = access("beditsem.go", 0);
                if  (RC != 0) {
                    //***********************
                    //* Still waiting
                    //***********************
                    Sleep ((DWORD)200);
                }
            } while (RC != 0);
            {
                FILE *BeditGoFile;
                char szGoData [50];
                //***********************
                //* See if there's a break
                //***********************
                //printf("See if there's a break\n");
                BeditGoFile = fopen ("beditsem.go", "r");
                //z999_open_bug ();
                //fputs ("opened beditsem.go\n", file_debug_out);
                if (BeditGoFile) {
                    file_status = fgets (szGoData, sizeof(szGoData), BeditGoFile);
                    if  (file_status != NULL) {
                        //sprintf (buff, "beditsem [%s]\n", szGoData);
                        //printf(buff);
                        if  (memcmp (szGoData, "BREAKS", 6) == 0) {
                            //*****************************
                            //* BREAKS
                            //*****************************
                            //printf("BreakProcessing\n");
                            do  {
                                file_status = fgets (szGoData, sizeof(szGoData), BeditGoFile);
                                if  (file_status != NULL) {
                                    //printf(szGoData);
                                    if  (memcmp (szGoData, "COND:", 5) == 0) {
                                        //*****************************
                                        //* Conditional Break
                                        //*****************************
                                        //printf("Condition Processing\n");
                                        if  (c115_check_cond(szGoData)) {
                                            fclose (BeditGoFile);
                                            BeditGoFile = NULL;
                                            remove ("beditsem.go");
                                            DebugProceed = 'n';
                                            //printf("Break found\n");
                                            Sleep ((DWORD)400);
                                            file_status = NULL;
                                            DebugLastBreakLine = pCommand->LineNo;
                                            cBreakType = 'c';
                                        }
                                    }
                                    else {
                                        //*****************************
                                        //* Line Number Break
                                        //*****************************
                                        char szLineFilename [100];
                                        int ix = 0;
                                        //printf(szGoData);
                                        do {
                                            if  ((szGoData[ix] != ':') && (szGoData[ix] != NULL)) {
                                                ix++;
                                            }
                                        }
                                        while ((szGoData[ix] != ':') && (szGoData[ix] != NULL));
                                        if  (szGoData[ix] == ':') {
                                            ix++;
                                            strcpy (szLineFilename, szGoData+ix);
                                        }
                                        else {
                                            strcpy (szLineFilename, "");
                                        }
                                        //sprintf(buff, "Comparing Passed[%s] to pCall[%s]\n", szLineFilename, pCall->szFilename);
                                        //printf(buff);
                                        //getch();
                                        if  (memcmp (szLineFilename, pCall->szFilename, strlen(pCall->szFilename)) == 0) {
                                            //printf("*****************OK*****************\n");
                                            //getch();
                                            Number = (long int) strtol ((char*)szGoData, (char**)&p, 10);
                                            if  (((int)Number == pCommand->LineNo) && (pCommand->LineNo != DebugLastBreakLine)) {
                                                fclose (BeditGoFile);
                                                BeditGoFile = NULL;
                                                remove ("beditsem.go");
                                                DebugProceed = 'n';
                                                //printf("Break found\n");
                                                Sleep ((DWORD)400);
                                                file_status = NULL;
                                                DebugLastBreakLine = pCommand->LineNo;
                                            }
                                        }
                                    }
                                }
                            } while (file_status != NULL);
                        }
                        else {
                            if  (memcmp (szGoData, "FILELIST", 8) == 0) {
                                //*****************************
                                //* FILELIST
                                //*****************************
                                FILE *BeditBugFile;
                                OPENFILE *pFileList;

                                BeditBugFile = fopen ("beditsem.stp", "w");
                                fputs ("FILELIST\n", BeditBugFile);
                                pFileList = &StartOpenFile;
                                do  {
                                    if  (pFileList->pNext) {
                                        pFileList = pFileList->pNext;
                                        if  (pFileList == pOpenFile) {
                                            sprintf (buff, "* %s [File Loc=%s] EOF=[%c]\n",
                                                                           pFileList->szFileName,
                                                                           pFileList->szFile,
                                                                           EOF_Flag);
                                        }
                                        else {
                                            sprintf (buff, "%s [File Loc=%s]\n",
                                                                           pFileList->szFileName,
                                                                           pFileList->szFile);
                                        }
                                        fputs (buff, BeditBugFile);
                                    }
                                }
                                while (pFileList->pNext);
                                fclose (BeditBugFile);
                                fclose (BeditGoFile);
                                BeditGoFile = NULL;
                                remove ("beditsem.go");
                                DebugProceed = 'n';
                                //printf("FILELIST Written to beditsem.stp\n");
                                Sleep ((DWORD)400);
                                file_status = NULL;
                                DebugLastBreakLine = pCommand->LineNo;
                                cFileList = 'y';
                            }
                            else {
                                if  (memcmp (szGoData, "FILEDATA:", 9) == 0) {
                                    //*****************************
                                    //* FILEDATA
                                    //*****************************
                                    FILE     *BeditBugFile;
                                    OPENFILE *pFileList;
                                    char     cDataDone;
                                    int      LinePos;

                                    //printf("FILEDATA Processing\n");
                                    //printf(szGoData);
                                    //printf("\n");

                                    BeditBugFile = fopen ("beditsem.stp", "w");
                                    fputs ("FILEDATA\n", BeditBugFile);
                                    pFileList = &StartOpenFile;

                                    cDataDone = 'n';
                                    do  {
                                        //*****************************
                                        //* Find the File
                                        //*****************************
                                        if  (pFileList->pNext) {
                                            pFileList = pFileList->pNext;
                                            //sprintf(buff, "Check %s - %i:%i\n", pFileList->szFileName, strlen (pFileList->szFileName), strlen (szGoData+9));
                                            //printf(buff);
                                            if  (((strlen (pFileList->szFileName) + 1) == strlen(szGoData+9)) && 
                                                (memcmp (pFileList->szFileName, szGoData+9, strlen (pFileList->szFileName)) == 0)) {
                                                //*****************************
                                                //* File Found
                                                //*****************************
                                                DATAFILE *pDebugDataFile;

                                                fputs (pFileList->szFileName, BeditBugFile);
                                                fputs ("\n", BeditBugFile);
                                                if  (pFileList == pOpenFile) {
                                                    pDebugDataFile = pDataFile;
                                                }
                                                else {
                                                    pDebugDataFile = pFileList->pData;
                                                }
                                                //*****************************
                                                //* Move to the top of the file
                                                //* and determine line pos
                                                //*****************************
                                                LinePos = 0;
                                                do  {
                                                    //if  (pDebugDataFile->pPrev) {
                                                    //    sprintf(buff, "curr line[%s]\n");
                                                    //    printf(buff);
                                                    //}
                                                    //else {
                                                    //    printf("TOP\n");
                                                    //}
                                                    if  (pDebugDataFile->pPrev) {
                                                        pDebugDataFile = pDebugDataFile->pPrev;
                                                        LinePos++;
                                                        //printf("Moving up\n");
                                                    }
                                                }
                                                while (pDebugDataFile->pPrev);
                                                if  (LinePos == 0) {
                                                    LinePos = 1;
                                                }
                                                sprintf (buff, "%d\n", LinePos);
                                                fputs (buff, BeditBugFile);
                                                //printf("writing data\n");
                                                //*****************************
                                                //* Output the data
                                                //*****************************
                                                do {
                                                    fputs (pDebugDataFile->szLine, BeditBugFile);
                                                    //printf(pDebugDataFile->szLine);
                                                    if (pDebugDataFile->pNext) {
                                                        pDebugDataFile = pDebugDataFile->pNext;
                                                    }
                                                    else {
                                                        cDataDone = 'y';
                                                    }
                                                }
                                                while (cDataDone != 'y');
                                            }
                                        }
                                    }
                                    while ((pFileList->pNext) && (cDataDone != 'y'));
                                    fclose (BeditBugFile);
                                    fclose (BeditGoFile);
                                    BeditGoFile = NULL;
                                    remove ("beditsem.go");
                                    DebugProceed = 'n';
                                    //printf("FILELIST Written to beditsem.stp\n");
                                    Sleep ((DWORD)400);
                                    file_status = NULL;
                                    DebugLastBreakLine = pCommand->LineNo;
                                    cFileList = 'y';
                            }
                                else {
                                    //*****************************
                                    //* STEP
                                    //*****************************
                                    //printf("Step\n");  
                                    fclose (BeditGoFile);
                                    BeditGoFile = NULL;
                                    remove ("beditsem.go");
                                }
                            }
                        }
                    }
                    else {
                        //printf("No first line\n");
                        fclose (BeditGoFile);
                        BeditGoFile = NULL;
                        remove ("beditsem.go");
                    }
                }
                else {
                    //printf("Bad open beditsem.go\n");
                    fclose (BeditGoFile);
                    BeditGoFile = NULL;
                    remove ("beditsem.go");
                }
                if  (BeditGoFile) {
                    fclose (BeditGoFile);
                }
            }
        } while (DebugProceed != 'y');
    }

    if  (Debug == 'y') {
        z999_open_bug ();
        fputs ("*************************************************************\n",
               file_debug_out);
        sprintf (buff, "$$$Debug Line %d [%s]\n", pCommand->LineNo, input_line);
        fputs (buff, file_debug_out);
    }

    if  (trace == 'y') {
        printf ("*************************************************************\n");
        sprintf (buff, "\nTrace Line %d [%s]\n", pCommand->LineNo, input_line);
        printf (buff);
    }
    if  (trace == 'x') {
        z999_open_bug ();
        sprintf (buff, "Trace Line %d [%s]\n", pCommand->LineNo, input_line);
        fputs (buff, file_debug_out);
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Check to see if a conditional breakpoint should break        */
/********************************************************************/
int c115_check_cond (char *szGoData) {
    //printf(szGoData);
    char szCond [20];
    char szName [200];
    int  iDone;
    VARIABLE *pCondVar;

    int  Ptr1;
    int  Ptr2;

    Ptr1 = 4;
    
    //***************************************
    //* Variable Name
    //***************************************
    iDone = 0;
    Ptr2 = 0;
    do  {
        Ptr1++;
        if  (szGoData[Ptr1] == 0) {
            return 0;
        }
        if  (szGoData[Ptr1] == ',') {
            szName[Ptr2] = NULL;
            iDone = 1;
        }
        else {
            szName[Ptr2] = szGoData[Ptr1];
            Ptr2++;
            if  (Ptr2 > 190) {
                return 0;
            }
        }
    }
    while (iDone != 1);

    //***************************************
    //* Condition
    //***************************************
    iDone = 0;
    Ptr2 = 0;
    do  {
        Ptr1++;
        if  (szGoData[Ptr1] == 0) {
            return 0;
        }
        if  (szGoData[Ptr1] == ',') {
            szCond[Ptr2] = NULL;
            iDone = 1;
        }
        else {
            szCond[Ptr2] = szGoData[Ptr1];
            Ptr2++;
            if  (Ptr2 > 15) {
                return 0;
            }
        }
    }
    while (iDone != 1);  

    //***************************************
    //* Value
    //***************************************
    Ptr1++;
    strcpy (buff, szGoData+Ptr1);
    if  (strlen (buff) > 0) {
        buff[strlen(buff)-1] = 0;
    }

    //printf(szName);
    //printf("\n");
    //printf("|");
    //printf(szCond);
    //printf("|");
    //printf("\n");
    //printf(buff);


    //***************************************
    //* Check Variables for Conditions
    //***************************************
    pCondVar = &StartVariable;
    do  {
        if  (pCondVar->pNext) {
            pCondVar = pCondVar->pNext;
            if  (strcmp (pCondVar->szName, szName) == 0) {
                //printf("Found Variable\n");
                if  (pCondVar->Type == 'S') {
                    if  (strcmp (pCondVar->szEdit, "") == 0) {
                        //****************
                        //* String
                        //****************
                        //printf("string\n");
                        //printf(pCondVar->szString);
                        //printf(buff);
                        //printf("===================\n");
                        if  (strcmp (szCond, "=") == 0) {
                            if  (strcmp (pCondVar->szString, buff) == 0) {
                                return 1;
                            }
                        }
                        if  (strcmp (szCond, "!=") == 0) {
                            if  (strcmp (pCondVar->szString, buff) != 0) {
                                return 1;
                            }
                        }
                        if  (strcmp (szCond, "TRUE") == 0) {
                            if  ((strcmp (pCondVar->szString, "YES") == 0) ||
                                 (strcmp (pCondVar->szString, "yes") == 0) || 
                                 (strcmp (pCondVar->szString, "Y") == 0) || 
                                 (strcmp (pCondVar->szString, "y") == 0) || 
                                 (strcmp (pCondVar->szString, "T") == 0) || 
                                 (strcmp (pCondVar->szString, "t") == 0) || 
                                 (strcmp (pCondVar->szString, "TRUE") == 0) ||
                                 (strcmp (pCondVar->szString, "true") == 0) ||
                                 (strcmp (pCondVar->szString, "OK") == 0) ||
                                 (strcmp (pCondVar->szString, "ok") == 0)) {
                                return 1;
                            }
                        }
                        if  (strcmp (szCond, "FALSE") == 0) {
                            if  ((strcmp (pCondVar->szString, "YES") != 0) &&
                                 (strcmp (pCondVar->szString, "yes") != 0) &&
                                 (strcmp (pCondVar->szString, "Y") != 0) &&
                                 (strcmp (pCondVar->szString, "y") != 0) &&
                                 (strcmp (pCondVar->szString, "T") != 0) &&
                                 (strcmp (pCondVar->szString, "t") != 0) &&
                                 (strcmp (pCondVar->szString, "TRUE") != 0) &&
                                 (strcmp (pCondVar->szString, "true") != 0) &&
                                 (strcmp (pCondVar->szString, "OK") != 0) &&
                                 (strcmp (pCondVar->szString, "ok") != 0)) {
                                return 1;
                            }
                        }
                        if  (strcmp (szCond, "Contains") == 0) {
                            int xPtr = 0;
                            int iContDone = 0;
                            do  {
                                if  (memcmp ((pCondVar->szString + xPtr), buff, strlen(buff)) == 0) {
                                    return  1;
                                }
                                xPtr++;
                                if  ((xPtr + strlen(buff)) > strlen(pCondVar->szString)) {
                                    iContDone = 1;
                                }
                            }
                            while (iContDone == 0);
                        }
                    }
                    else {
                        double dFloat1;
                        double dFloat2;
                        unsigned char   *cpFloat1;
                        unsigned char   *cpFloat2;

                        dFloat1 = (double)strtod ((char*)pCondVar->szString, (char**)&cpFloat2);
                        dFloat2 = (double)strtod ((char*)buff, (char**)&cpFloat1);

                        //****************
                        //* Float/Dollar
                        //****************
                        //printf("float\n");

                        if (strcmp (szCond, "=") == 0) {
                            printf("equal\n");
                            if  (dFloat1 == dFloat2) {
                                return 1;
                            }
                        }
                        if  (strcmp (szCond, "!=") == 0) {
                            if  (dFloat1 != dFloat2) {
                                return 1;
                            }
                        }
                        if (strcmp (szCond, ">") == 0) {
                            if  (dFloat1 > dFloat2) {
                                return 1;
                            }
                        }
                        if (strcmp (szCond, ">=") == 0) {
                            //char szCompare [100];  
                            //printf("Check >=/n");
                            //sprintf(szCompare, "\ncomparing %f >= %f\n", dFloat1, dFloat2);
                            //printf(szCompare);
                            if  (dFloat1 >= dFloat2) {
                                //printf("Greaterthan-equal\n");
                                return 1;
                            }
                        }
                        if (strcmp (szCond, "<") == 0) {
                            if  (dFloat1 <= dFloat2) {
                                return 1;
                            }
                        }
                        if (strcmp (szCond, "<=") == 0) {
                            if  (dFloat1 <= dFloat2) {
                                return 1;
                            }
                        }

                        if (strcmp (szCond, "TRUE") == 0) {
                            if  (dFloat1 != 0) {
                                return 1;
                            }
                        }
                        if (strcmp (szCond, "FALSE") == 0) {
                            if  (dFloat1 == 0) {
                                return 1;
                            }
                        }
                    }
                }
                else {
                    //****************
                    //* Integer
                    //****************
                    //printf("integer\n");
                    Number = (long int) strtol ((char*)buff, (char**)&p, 10);
                    if (strcmp (szCond, "=") == 0) {
                        if  (pCondVar->Integer == Number) {
                            return 1;
                        }
                    }
                    if (strcmp (szCond, "!=") == 0) {
                        if  (pCondVar->Integer != Number) {
                            return 1;
                        }
                    }
                    if (strcmp (szCond, ">") == 0) {
                        if  (pCondVar->Integer > Number) {
                            return 1;
                        }
                    }
                    if (strcmp (szCond, ">=") == 0) {
                        if  (pCondVar->Integer >= Number) {
                            return 1;
                        }
                    }
                    if (strcmp (szCond, "<") == 0) {
                        if  (pCondVar->Integer < Number) {
                            return 1;
                        }
                    }
                    if (strcmp (szCond, "<=") == 0) {
                        if  (pCondVar->Integer <= Number) {
                            return 1;
                        }
                    }
                    if (strcmp (szCond, "TRUE") == 0) {
                        if  (pCondVar->Integer != 0) {
                            return 1;
                        }
                    }
                    if (strcmp (szCond, "FALSE") == 0) {
                        if  (pCondVar->Integer == 0) {
                            return 1;
                        }
                    }
                } 
                return 0;
            }
        }
    }
    while (pCondVar->pNext);

    return 0;
}

/********************************************************************/
/* This module will :                                               */
/*     Read one line of the input command file                      */
/********************************************************************/
void c120_parse_command () {
//    printf ("%s", "c120_parse_command\n");
//    memset (Command, 0, Max);
    memset (Command, 0, 10);
    index2 = 0;
    done2 = 'n';
    ErrorPtr = 0;
    Delimiter = 0;
    do  {
        if  (input_line [index1] == 0) {
            done2 = 'y';
        }
        else {
            if  ((Delimit != 'y') &&
                 (input_line [index1] == ':') &&
                 (input_line [index1 + 1] > 32) &&
                 (found2 != '~') &&     // OPEN A C:\...
                 (memcmp (input_line + index1, ":WORD", 5) != 0) &&
                 (memcmp (input_line + index1, ":word", 5) != 0)) {
                Delimit = 'y';
                Command [index2] = input_line [index1];
                index2++;
                if  (ErrorPtr == 0) {
                    ErrorPtr = index1;
                }
                index1++;
            }
            if  (input_line [index1] < 33) {
                if  (Delimit == 'y') {
                    if  (Delimiter != 0) {
                        Command [index2] = input_line [index1];
                        index2++;
                        if  (ErrorPtr == 0) {
                            ErrorPtr = index1;
                        }
                    }
                }
                else {
                    if  (Command [0] > 0) {
                        done2 = 'y';
                    }
                }
            }
            else {
                if  ((Delimiter == 0) && (Delimit == 'y')) {
                    Delimiter = input_line [index1];
                    if  (Delimiter == '(') {
                        Delimiter = ')';
                    }
                    DelimitCount = 1;
                    Command [index2] = input_line [index1];
                    index2++;
                    if  (ErrorPtr == 0) {
                        ErrorPtr = index1;
                    }
                }
                else {
                    if  ((input_line [index1] == Delimiter) &&
                         (Delimit == 'y')) {
                        DelimitCount++;
                        if  (DelimitCount == 4) {
                            Delimit = 'n';
                        }
                        if  ((DelimitCount == 2) &&
                             (input_line [index1 + 1] == ';')) {
                            Delimit = 'n';
                        }
                        if  ((DelimitCount == 2) &&
                             (input_line [index1 + 1] == ':')) {
                            Delimiter = input_line [index1 + 2];
                            if  ((memcmp (input_line + index1 + 1, ":WORD", 5) == 0) ||
                                 (memcmp (input_line + index1 + 1, ":word", 5) == 0)) {
                                Delimiter = 0;
                                Delimit = 'n';
                            }
                        }
                        if  ((DelimitCount == 2) &&
                             (input_line [index1 + 1] == '+')) {
                            Delimiter = input_line [index1 + 2];
                        }
                        if  ((DelimitCount == 2) &&
                             (input_line [index1 + 1] == '-')) {
                            Delimiter = input_line [index1 + 2];
                        }
                    }
                    if  ((Delimit == 'y') &&
                         (Delimiter == input_line [index1]) &&
                         (input_line [index1 + 1] < 33) &&
                         (DelimitCount == 2)) {
                        Command [index2] = input_line [index1];
                        index2++;
                        done2 = 'y';
                        if  (ErrorPtr == 0) {
                            ErrorPtr = index1;
                        }
                    }
                    else {
                        Command [index2] = input_line [index1];
                        index2++;
                        if  (ErrorPtr == 0) {
                            ErrorPtr = index1;
                        }
                    }
                }
            }
            index1++;
            if  (index2 > Max - 1) {
                strcpy (Error, "Input Large");
                z110_fatal ();
                return;
            }
        }
    }
    while (done2 != 'y');
    Delimit = 'n';
    if  (Command [0] == 0) {
        More_Commands = 'n';
    }
    else {

        int Nullout1;
        int Nullout2;

        Nullout1 = index2;
        Nullout2 = 0;
        Command [Max] = 0;
        do  {
            if  (Nullout1 < Max) {
                Command [Nullout1] = 0;
                Nullout1++;
            }
            Nullout2++;
        }
        while ((Nullout1 < Max - 1) && (Nullout2 < 10));

        if  (Debug == 'y') {
            z999_open_bug ();
            sprintf (buff, "  $Debug Command =[%s]\n", Command);
            fputs (buff, file_debug_out);
//            printf ("%s", buff);
        }
        if  (trace == 'y') {
            sprintf (buff, "\n   Trace Command =[%s]\n", Command);
            printf (buff);
            printf ("   Trace Wait...");
            getch ();
        }
//        if  (trace == 'x') {
//            z999_open_bug ();
//            sprintf (buff, "     Trace Command = [%s]\n", Command);
//            fputs (buff, file_debug_out);
//        }
    }
    do  {
        if  (input_line [index1] == ' ') {
            index1++;
        }
    }
    while (input_line [index1] == ' ');
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process the Command Line                                     */
/********************************************************************/
void c200_process_command () {
    static unsigned char szDoident [100];

    /***********************/
    /* $Macro              */
    /***********************/
    if  (Command [0] == '$') {
        strcpy (buff, Command + 1);
        strcpy (Command, buff);
        d580_find_variable ();
        if  ((!pVariable) || (pVariable->Type != 'S')) {
            strcpy (Error, "Must use str");
            z110_fatal ();
            return;
        }
        strcpy (szIf2, "");
        if  (More_Commands == 'y') {
            strcpy (szIf2, input_line + index1);
        }
        strcpy (szConvString, pVariable->szString);
        g200_convert_string ();                         // Expand the Macro
        strcpy (input_line, szConvString);
        strcat (input_line, " ");
        strcat (input_line, szIf2);
        index1 = 0;
        More_Commands = 'y';
        return;
    }
    /***********************/
    /* !Label              */
    /***********************/
    if  (Command [0] == '!') {
        More_Commands = 'n';
        return;
    }
    /***********************/
    /* ERROR               */
    /***********************/
    if  ((strcmp (Command, "ERROR") == 0) ||
         (strcmp (Command, "error") == 0)) {
        More_Commands = 'n';
        return;
    }

    /***********************/
    /* LOCAL               */
    /***********************/
    if  ((strcmp (Command, "LOCAL") == 0) ||
         (strcmp (Command, "local") == 0)) {
        if  (cFast == 'y') {
            strcpy (Error, "Can't use with FAST");
            z110_fatal ();
            return;
        }
        cLocal = 'y';
        return;
    }

    /***********************/
    /* ARG                 */
    /***********************/
    if  ((strcmp (Command, "ARG") == 0) ||
         (strcmp (Command, "arg") == 0)) {
        int count = 0;

        pCountCall = &StartCall;
        do  {
            if  (pCountCall->pNext) {
                pCountCall = pCountCall->pNext;
                count++;
            }
        }
        while (pCountCall->pNext);
        if  (count > 1) {
            if  (cFast == 'y') {
                strcpy (Error, "Can't use with FAST");
                z110_fatal ();
                return;
            }
        }
        cLocal = 'y';
        cArg = 'y';
        return;
    }

    /***********************/
    /* *PM_CMD_DONE*       */
    /***********************/
    if  (strcmp (Command, "*PM_CMD_DONE*") == 0) {
        More_Commands = 'n';
        return;
    }

    /***********************/
    /* CLS                 */
    /***********************/
    if  ((strcmp (Command, "CLS") == 0) ||
         (strcmp (Command, "cls") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavailable in Beditwin");
            z110_fatal ();
            return;
        }
//        system ("cls");
        clrscr ();
        return;
    }

    /***********************/
    /* DIRECTION           */
    /***********************/
    if  ((strcmp (Command, "DIRECTION") == 0) ||
         (strcmp (Command, "direction") == 0)) {
        c120_parse_command ();
        if  ((strcmp (Command, "UP") == 0) ||
             (strcmp (Command, "up") == 0)) {
            Direction = 'U';
        }
        else {
            if  ((strcmp (Command, "DOWN") == 0) ||
                 (strcmp (Command, "down") == 0)) {
                Direction = 'D';
            }
            else {
                z010_syntax_error ();
                return;
            }
        }
        return;
    }


    /***********************/
    /* CLEANKEYS           */
    /***********************/
    if  ((strcmp (Command, "CLEANKEYS") == 0) ||
         (strcmp (Command, "cleankeys") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        RC = 0;
        do  {
            if  (kbhit()) {
                getch ();
            }
            else {
                RC = 1;
            }
        }
        while (RC == 0);
        return;
    }

    /***********************/
    /* LOGFILE             */
    /***********************/
    if  ((strcmp (Command, "LOGFILE") == 0) ||
         (strcmp (Command, "logfile") == 0)) {
        c120_parse_command ();
        strcpy (name_debug_out, Command);
        if  ((strcmp (name_debug_out, "null") == 0) ||
             (strcmp (name_debug_out, "NULL") == 0)) {
            strcpy (name_debug_out, "BEDIT.LOG");
        }
        return;
    }

    /***********************/
    /* DIR                 */
    /***********************/
    if  ((strcmp (Command, "DIR") == 0) ||
         (strcmp (Command, "dir") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        c120_parse_command ();
        sprintf (temp, "dir %s", Command);
//        system (temp);
        z998_winexec (temp);
        return;
    }

    /***********************/
    /* CD                  */
    /***********************/
    if  ((strcmp (Command, "CD") == 0) ||
         (strcmp (Command, "cd") == 0)) {
        c120_parse_command ();
        if  (Command [0] == Subs) {
            d580_find_variable ();
            if  ((!pVariable) || (pVariable->Type != 'S')) {
                strcpy (Error, "Must use str!");
                z110_fatal ();
                return;
            }
            strcpy (Command, pVariable->szString);
        }
        RC = chdir (Command);
        if  (RC != 0) {
            strcpy (Error, "Bad CD!");
            z110_fatal ();
            return;
        }
        return;
    }

    /***********************/
    /* MD                  */
    /***********************/
    if  ((strcmp (Command, "MD") == 0) ||
         (strcmp (Command, "md") == 0)) {
        c120_parse_command ();
        if  (Command [0] == Subs) {
            d580_find_variable ();
            if  (!pVariable || pVariable->Type != 'S') {
                strcpy (Error, "Must use Str!");
                z110_fatal ();
                return;
            }
            strcpy (Command, pVariable->szString);
        }
        RC = mkdir (Command);
        if  (RC != 0) {
            strcpy (Error, "Bad MD!");
            z110_fatal ();
            return;
        }
        return;
    }

    /***********************/
    /* RD                  */
    /***********************/
    if  ((strcmp (Command, "RD") == 0) ||
         (strcmp (Command, "rd") == 0)) {
        c120_parse_command ();
        if  (Command [0] == Subs) {
            d580_find_variable ();
            if  (!pVariable || pVariable->Type != 'S') {
                strcpy (Error, "Must use Str!");
                z110_fatal ();
                return;
            }
            strcpy (Command, pVariable->szString);
        }
        RC = rmdir (Command);
        if  (RC != 0) {
            strcpy (Error, "Bad RD!");
            z110_fatal ();
            return;
        }
        return;
    }

    /***********************/
    /* LIST                */
    /***********************/
    if  ((strcmp (Command, "LIST") == 0) ||
         (strcmp (Command, "list") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d350_list ();
        return;
    }

    /***********************/
    /* PERM                */
    /***********************/
    if  ((strcmp (Command, "PERM") == 0) ||
         (strcmp (Command, "perm") == 0)) {
        d270_perm ();
        return;
    }

    /***********************/
    /* BREAK=ON            */
    /***********************/
    if  ((strcmp (Command, "BREAK=ON") == 0) ||
         (strcmp (Command, "break=on") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        BreakStatus = 'y';
        signal (SIGINT, x100_break);
        return;
    }

    /***********************/
    /* BREAK=OFF           */
    /***********************/
    if  ((strcmp (Command, "BREAK=OFF") == 0) ||
         (strcmp (Command, "break=off") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        BreakStatus = 'n';
        signal (SIGINT, SIG_IGN);
        return;
    }

//    /***********************/
//    /* HELP                */
//    /***********************/
//    if  ((strcmp (Command, "HELP") == 0) ||
//         (strcmp (Command, "help") == 0)) {
//        if  (Beditwin == 'y') {
//            strcpy (Error, "Unavail in Beditwin");
//            z110_fatal ();
//            return;
//        }
//        c120_parse_command ();
//        h100_help (Command);
//        return;
//    }

    /***********************/
    /* NOTE:               */
    /***********************/
    memcpy (buff, Command, 5);
    buff [5] = 0;
    if  ((strcmp (buff, "NOTE:") == 0) ||
         (strcmp (buff, "note:") == 0)) {
        More_Commands = 'n';
        return;
    }
    memcpy (buff, Command, 2);
    buff [2] = 0;
    if  (strcmp (buff, "//") == 0) {
        More_Commands = 'n';
        return;
    }
    if  (strcmp (Command, "_") == 0) {
        More_Commands = 'n';
        return;
    }

    /***********************/
    /* SYSTEMCURR          */
    /***********************/
    memcpy (buff, Command, 11);
    buff [11] = 0;
    if  ((strcmp (buff, "SYSTEMCURR:") == 0) ||
         (strcmp (buff, "systemcurr:") == 0)) {
        d181_process_systemcurr ();
        return;
    }

    /***********************/
    /* SYSTEM:"xxx"        */
    /***********************/
    memcpy (buff, Command, 7);
    buff [7] = 0;
    if  ((strcmp (buff, "SYSTEM:") == 0) ||
         (strcmp (buff, "system:") == 0)) {
        d180_process_system ();
        return;
    }

    /***********************/
    /* SYSTEMF:"xxx"       */
    /***********************/
    memcpy (buff, Command, 8);
    buff [8] = 0;
    if  ((strcmp (buff, "SYSTEMF:") == 0) ||
         (strcmp (buff, "systemf:") == 0)) {
        d180_process_system ();
        return;
    }

    /***********************/
    /* WAIT                */
    /***********************/
    if  ((strcmp (Command, "WAIT") == 0) ||
         (strcmp (Command, "wait") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        printf ("%s", "Press Any Key To Continue...\n");
        if  (Debug == 'y') {
            z999_open_bug ();
            fputs ("Press Any Key To Continue...\n", file_debug_out);
        }
        RC = 0;
        do  {
            if  (kbhit()) {
                getch ();
            }
            else {
                RC = 1;
            }
        }
        while (RC == 0);
        response = (unsigned char) getch ();
        return;
    }

    /***********************/
    /* SLEEP               */
    /***********************/
    if  ((strcmp (Command, "SLEEP") == 0) ||
         (strcmp (Command, "sleep") == 0)) {
        d710_sleep ();
        return;
    }

    /***********************/
    /* WINDOW              */
    /***********************/
    if  ((strcmp (Command, "WINDOW") == 0) ||
         (strcmp (Command, "window") == 0)) {
        if  (Beditwin != 'y') {
            strcpy (Error, "Must use Str Beditwin only!");
            z110_fatal ();
            return;
        }
        w100_window ();
        return;
    }

    /***********************/
    /* BEDITDATA           */
    /***********************/
    if  ((strcmp (Command, "BEDITDATA") == 0) ||
         (strcmp (Command, "beditdata") == 0)) {
        d128_data ();
        return;
    }

    /***********************/
    /* WAITBELL            */
    /***********************/
    if  ((strcmp (Command, "WAITBELL") == 0) ||
         (strcmp (Command, "waitbell") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d715_waitbell ();
        return;
    }

    /***********************/
    /* CHAIN               */
    /***********************/
    if  ((strcmp (Command, "CHAIN") == 0) ||
         (strcmp (Command, "chain") == 0)) {
        d720_chain ();
        return;
    }

    /***********************/
    /* USER                */
    /***********************/
    if  ((strcmp (Command, "USER") == 0) ||
         (strcmp (Command, "user") == 0)) {

        //VARIABLE *pUserParam1;
        //VARIABLE *pUserParam2;
        //VARIABLE *pUserParam3;

        /**********************/
        /* Find User Param 1  */
        /**********************/
        c120_parse_command ();
        d580_find_variable ();
        if  (!pVariable || pVariable->Type != 'S') {
            strcpy (Error, "Must use Str!");
            z110_fatal ();
            return;
        }
        strcpy (temp, pVariable->szString);
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (Max);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad resize var");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, temp);
        //pUserParam1 = pVariable;
        /**********************/
        /* Find User Param 2  */
        /**********************/
        c120_parse_command ();
        d580_find_variable ();
        if  (!pVariable || pVariable->Type != 'S') {
            strcpy (Error, "Must use Str!");
            z110_fatal ();
            return;
        }
        strcpy (temp, pVariable->szString);
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (Max);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad resize var");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, temp);
        //pUserParam2 = pVariable;
        /**********************/
        /* Find User Param 3  */
        /**********************/
        c120_parse_command ();
        d580_find_variable ();
        if  (!pVariable || pVariable->Type != 'S') {
            strcpy (Error, "Must use Str!");
            z110_fatal ();
            return;
        }
        strcpy (temp, pVariable->szString);
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (Max);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad resize var");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, temp);
        //pUserParam3 = pVariable;

        /**********************/
        /* Process function   */
        /**********************/
        //u100_user (pUserParam1->szString, //09/11/2013
        //           pUserParam2->szString,
        //           pUserParam3->szString);
        return;
    }

    /***********************/
    /* CURRDIR             */
    /***********************/
    if  ((strcmp (Command, "CURRDIR") == 0) ||
         (strcmp (Command, "currdir") == 0)) {
        c120_parse_command ();
        d580_find_variable ();
        if  (!pVariable || pVariable->Type != 'S') {
            strcpy (Error, "Must use Str!");
            z110_fatal ();
            return;
        }
        getcwd (buff, Max);
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (strlen (buff) + 5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad to resize var");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, buff);
        return;
    }

    /***********************/
    /* OPEN xxx            */
    /***********************/
    if  ((strcmp (Command, "OPEN") == 0) ||
         (strcmp (Command, "open") == 0) ||
         (strcmp (Command, "READ") == 0) ||
         (strcmp (Command, "read") == 0)) {
        d100_process_read ();
        return;
    }

    /***********************/
    /* FILESIZE            */
    /***********************/
    if  ((strcmp (Command, "FILESIZE") == 0) ||
         (strcmp (Command, "filesize") == 0)) {
        d105_filesize ();
        return;
    }
    /***********************/
    /* FILESTRING          */
    /***********************/
    if  ((strcmp (Command, "FILESTRING") == 0) ||
         (strcmp (Command, "filestring") == 0)) {
        d106_filestring ();
        return;
    }

    /***********************/
    /* WRITE xxx           */
    /***********************/
    if  ((strcmp (Command, "WRITE") == 0) ||
         (strcmp (Command, "write") == 0)) {
        d110_process_write ();
        return;
    }

    /***********************/
    /* SAVE                */
    /***********************/
    if  ((strcmp (Command, "SAVE") == 0) ||
         (strcmp (Command, "save") == 0)) {
        e200_save ();
        return;
    }

    /***********************/
    /* RESTORE             */
    /***********************/
    if  ((strcmp (Command, "RESTORE") == 0) ||
         (strcmp (Command, "restore") == 0)) {
        e210_restore ();
        return;
    }

    /***********************/
    /* SORT                */
    /***********************/
    if  ((strcmp (Command, "SORT") == 0) ||
         (strcmp (Command, "sort") == 0)) {
        d182_sort ();
        return;
    }
    /***********************/
    /* FILE xxx            */
    /***********************/
    if  ((strcmp (Command, "FILE") == 0) ||
         (strcmp (Command, "file") == 0)) {
        d112_process_file ();
        return;
    }
    /***********************/
    /* FIXFILE From To     */
    /***********************/
    if  ((strcmp (Command, "FIXFILE") == 0) ||
         (strcmp (Command, "fixfile") == 0)) {
        d210_fixfile ();
        return;
    }

    /***********************/
    /* FULLSCREEN          */
    /***********************/
    if  ((strcmp (Command, "FULLSCREEN") == 0) ||
	 (strcmp (Command, "fullscreen") == 0)) {
	RECT rect;

	GetWindowRect (GetDesktopWindow (), &rect);
	SetWindowPos (GetActiveWindow (), HWND_TOP,
		      rect.left, rect.top, rect.right, rect.bottom,
		      SWP_SHOWWINDOW);
	ShowWindow (GetActiveWindow (), SW_SHOWMAXIMIZED);
        UpdateWindow (GetActiveWindow ());
        return;
    }

    /***********************/
    /* CLOSE xxx           */
    /***********************/
    if  ((strcmp (Command, "CLOSE") == 0) ||
         (strcmp (Command, "close") == 0)) {
        d113_close_file ();
        return;
    }

    /***********************/
    /* INPUT xxx           */
    /***********************/
    if  ((strcmp (Command, "INPUT") == 0) ||
         (strcmp (Command, "input") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d700_input ('s');
        return;
    }
    /***********************/
    /* INPUTP xxx          */
    /***********************/
    if  ((strcmp (Command, "INPUTP") == 0) ||
         (strcmp (Command, "inputp") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d700_input ('p');
        return;
    }
    /***********************/
    /* INPUTC xxx          */
    /***********************/
    if  ((strcmp (Command, "INPUTC") == 0) ||
         (strcmp (Command, "inputc") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d700_input ('c');
        return;
    }

    /***********************/
    /* BEDIT               */
    /***********************/
    if  ((strcmp (Command, "BEDIT") == 0) ||
         (strcmp (Command, "bedit") == 0)) {
        d119_interactive ();
        return;
    }

    /***********************/
    /* COLOR xxx           */
    /***********************/
    if  ((strcmp (Command, "COLOR") == 0) ||
         (strcmp (Command, "color") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d121_color ();
        return;
    }

    /***********************/
    /* BACKCOLOR xxx       */
    /***********************/
    if  ((strcmp (Command, "BACKCOLOR") == 0) ||
         (strcmp (Command, "backcolor") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d122_backcolor ();
        return;
    }

    /***********************/
    /* CHAR                */
    /***********************/
//    if  ((strcmp (Command, "CHAR") == 0) ||
//         (strcmp (Command, "char") == 0)) {
//        if  (Beditwin == 'y') {
//            strcpy (Error, "Unavail in Beditwin");
//            z110_fatal ();
//           return;
//        }
//        d126_char ();
//        return;
//    }

    /***********************/
    /* EOL                 */
    /***********************/
    if  ((strcmp (Command, "EOL") == 0) ||
         (strcmp (Command, "eol") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d127_eol ();
        return;
    }

    /***********************/
    /* BELL                */
    /***********************/
    if  ((strcmp (Command, "BELL") == 0) ||
         (strcmp (Command, "bell") == 0)) {
        d123_bell ();
        return;
    }

    /***********************/
    /* GOTO xxx            */
    /***********************/
    if  ((strcmp (Command, "GOTO") == 0) ||
         (strcmp (Command, "goto") == 0)) {
//        pCommand->pLastCallGoto = NULL;
        pLastCallGoto = pCommand;
        d114_goto ();
        return;
    }

    /***********************/
    /* CASE                */
    /***********************/
    if  ((strcmp (Command, "CASE") == 0) ||
         (strcmp (Command, "case") == 0)) {
        d125_case ();
        return;
    }
    /***********************/
    /* SCRAMBLE            */
    /***********************/
    if  ((strcmp (Command, "SCRAMBLE") == 0) ||
         (strcmp (Command, "scramble") == 0)) {
        d129_scramble ();
        return;
    }

    /***********************/
    /* ENCRYPTKEY          */
    /***********************/
    if  ((strcmp (Command, "ENCRYPTKEY") == 0) ||
         (strcmp (Command, "encryptkey") == 0)) {
        d135_encryptkey ();
        return;
    }

    /***********************/
    /* BOOKMARK            */
    /***********************/
    if  ((strcmp (Command, "BOOKMARK") == 0) ||
         (strcmp (Command, "bookmark") == 0)) {
        d124_bookmark ();
        return;
    }

    /***********************/
    /* CURSOR xxx,yyy      */
    /***********************/
    if  ((strcmp (Command, "CURSOR") == 0) ||
         (strcmp (Command, "cursor") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        d118_cursor ();
        return;
    }

    /***********************/
    /* SUBS                */
    /***********************/
    if  ((strcmp (Command, "SUBS") == 0) ||
         (strcmp (Command, "subs") == 0)) {
        d175_subs ();
        return;
    }

    /***********************/
    /* PATTERN             */
    /***********************/
    if  ((strcmp (Command, "PATTERN") == 0) ||
         (strcmp (Command, "pattern") == 0)) {
        d176_pattern ();
        return;
    }

    /***********************/
    /* BEGINEND            */
    /***********************/
    if  ((strcmp (Command, "BEGINEND") == 0) ||
         (strcmp (Command, "beginend") == 0)) {
        d360_beginend ();
        return;
    }

    /***********************/
    /* STATUS              */
    /***********************/
    if  ((strcmp (Command, "STATUS") == 0) ||
         (strcmp (Command, "status") == 0)) {
        d370_status ();
        return;
    }

    /***********************/
    /* COLS                */
    /***********************/
    if  ((strcmp (Command, "COLS") == 0) ||
         (strcmp (Command, "cols") == 0)) {
        d185_cols ();
        return;
    }

    /***********************/
    /* NOCO                */
    /***********************/
    if  ((strcmp (Command, "NOCO") == 0) ||
         (strcmp (Command, "noco") == 0)) {
        Cols = 'n';
        return;
    }

    /***********************/
    /* PRINT:"xxxx"        */
    /***********************/
    memcpy (buff, Command, 6);
    buff [6] = 0;
    if  ((strcmp (buff, "PRINT:") == 0) ||
         (strcmp (buff, "print:") == 0)) {
        Printx = 'n';
        d115_process_print ();
        return;
    }

    /***********************/
    /* PRINTX "xxxx"       */
    /***********************/
    memcpy (buff, Command, 7);
    buff [7] = 0;
    if  ((strcmp (buff, "PRINTX:") == 0) ||
         (strcmp (buff, "printx:") == 0)) {
        Printx = 'y';
        d115_process_print ();
        return;
    }

    /***********************/
    /* SCAN                */
    /***********************/
    memcpy (buff, Command, 5);
    buff [5] = 0;
    if  ((strcmp (buff, "SCAN:") == 0) ||
         (strcmp (buff, "scan:") == 0)) {
        d800_scan ();
        return;
    }

    /***********************/
    /* SCANR               */
    /***********************/
    memcpy (buff, Command, 6);
    buff [6] = 0;
    if  ((strcmp (buff, "SCANR:") == 0) ||
         (strcmp (buff, "scanr:") == 0)) {
        Scanr = 'y';
        d800_scan ();
        Scanr = 'n';
        return;
    }

    /***********************/
    /* SPLIT               */
    /***********************/
    memcpy (buff, Command, 6);
    buff [6] = 0;
    if  ((strcmp (buff, "SPLIT:") == 0) ||
         (strcmp (buff, "split:") == 0)) {
        d900_split ();
        return;
    }

    /***********************/
    /* SPLITR              */
    /***********************/
    memcpy (buff, Command, 7);
    buff [7] = 0;
    if  ((strcmp (buff, "SPLITR:") == 0) ||
         (strcmp (buff, "splitr:") == 0)) {
        Scanr = 'y';
        d900_split ();
        Scanr = 'n';
        return;
    }

    /***********************/
    /* EXTRACT             */
    /***********************/
    if  ((strcmp (Command, "EXTRACT") == 0) ||
         (strcmp (Command, "extract") == 0)) {
        d950_extract ();
        return;
    }

    /***********************/
    /* IF (...)            */
    /***********************/
    if  ((memcmp (Command, "IF-", 3) == 0) ||
         (memcmp (Command, "if-", 3) == 0)) {
        if  (strlen (Command) > 3) {
            strcpy (szIfName, Command + 3);
            Command [2] = 0;
        }
    }
    else {
        if  ((strcmp (Command, "IF") == 0) ||
             (strcmp (Command, "if") == 0)) {
            strcpy (szIfName, "");
        }
    }
    if  ((strcmp (Command, "IF") == 0) ||
         (strcmp (Command, "if") == 0) ||
         (strcmp (Command, "AND") == 0) ||
         (strcmp (Command, "and") == 0) ||
         (memcmp (Command, "UNTIL", 5) == 0) ||
         (memcmp (Command, "until", 5) == 0)) {

        if  ((strcmp (Command, "IF") == 0) ||
             (strcmp (Command, "if") == 0)) {
              Dountil = 'n';
        }
        if  ((memcmp (Command, "UNTIL", 5) == 0) ||
             (memcmp (Command, "until", 5) == 0)) {
            Dountil = 'y';
            pDountilcommand = pCommand;
            strcpy (szDoident, Command+5);
        }
        If_Proc = 'y';
        Ok_If = 'n';
        d116_if ();
        /*****************************/
        /* Conitinue on OR condition */
        /*****************************/
        if  (Or_Cond == 'y') {
            if  (More_Commands == 'y') {
                Ok_If = 'y';
            }
            else {
                More_Commands = 'y';
            }
        }
        if  ((More_Commands != 'y') && (Dountil != 'y')) {
            /*******************************************/
            /* Bypass the rest of the line except ELSE */
            /*******************************************/
            If_Proc = 'n';
            Dountil = 'n';
            More_Commands = 'y';
            do  {
                c120_parse_command ();
                if  (More_Commands == 'y') {
                    if  ((memcmp (Command, "ELSE-", 5) == 0) ||
                         (memcmp (Command, "else-", 5) == 0)) {
                        if  (strcmp (szIfName, "") != 0) {
                            if  (strcmp (szIfName, Command + 5) == 0) {
                                strcpy (szIfName, "");
                                Command [4] = 0;
                            }
                        }
                    }
                }
            }
            while ((More_Commands == 'y') &&
                   (strcmp (Command, "ELSE") != 0) &&
                   (strcmp (Command, "else") != 0));
            if  ((strcmp (Command, "ELSE") == 0) ||
                 (strcmp (Command, "else") == 0)) {
                If_Proc = 'y';
            }
        }
        /**************************************************/
        /* Find Matching DO for UNTIL                     */
        /**************************************************/
        if  ((More_Commands != 'y') && (Dountil == 'y')) {
            if  (pCommand->pLastCallGoto) {
                pCommand = pCommand->pLastCallGoto;
                strcpy (input_line, pCommand->szLine);
                index1 = 0;
                More_Commands = 'y';
                if  (input_line [0] == '+') {
                    input_line [0] = ' ';
                }
            }
            else {
                do  {
                    index1 = 0;
                    More_Commands = 'y';
                    strcpy (input_line, pCommand->szLine);
                    c120_parse_command ();
                    if  (strcmp (Command, "+") == 0) {
                        c120_parse_command ();
                    }
                    if  (strlen (Command) < 2) {
                        strcpy (Command, "xx");
                    }
                    if  (((memcmp (Command, "DO", 2) != 0) &&
                          (memcmp (Command, "do", 2) != 0)) ||
                         (strcmp (Command+2, szDoident) != 0)){
                        if  (pCommand->pPrev) {
                            pCommand = pCommand->pPrev;
                            More_Commands = 'y';
                        }
                        else {
                            pCommand = pDountilcommand;
                            strcpy (Error, "UNTIL without DO!");
                            z110_fatal ();
                            return;
                        }
                    }
                    else {
                        More_Commands = 'n';
                        pDountilcommand->pLastCallGoto = pCommand;
                    }
                }
                while (More_Commands == 'y');
            }
            More_Commands = 'y';
        }
        return;
    }

    /***********************/
    /* IF (...) OR (...)   */
    /***********************/
    if  ((strcmp (Command, "OR") == 0) ||
         (strcmp (Command, "or") == 0)) {
        d116_if ();
        if  (Or_Cond == 'y') {
            if  (More_Commands == 'y') {
                Ok_If = 'y';
            }
            else {
                More_Commands = 'y';
            }
        }
        else {
            if  (Ok_If == 'y') {
                More_Commands = 'y';
            }
        }
        if  ((More_Commands != 'y') && (Dountil != 'y')) {
            /*******************************************/
            /* Bypass the rest of the line except ELSE */
            /*******************************************/
            If_Proc = 'n';
            Dountil = 'n';
            More_Commands = 'y';
            do  {
                c120_parse_command ();
                if  (More_Commands == 'y') {
                    if  ((memcmp (Command, "ELSE-", 5) == 0) ||
                         (memcmp (Command, "else-", 5) == 0)) {
                        if  (strcmp (szIfName, "") != 0) {
                            if  (strcmp (szIfName, Command + 5) == 0) {
                                strcpy (szIfName, "");
                                Command [4] = 0;
                            }
                        }
                    }
                }
            }
            while ((More_Commands == 'y') &&
                   (strcmp (Command, "ELSE") != 0) &&
                   (strcmp (Command, "else") != 0));
            if  ((strcmp (Command, "ELSE") == 0) ||
                 (strcmp (Command, "else") == 0)) {
                If_Proc = 'y';
            }
        }
        /**************************************************/
        /* Find Matching DO for UNTIL                     */
        /**************************************************/
        if  ((More_Commands != 'y') && (Dountil == 'y')) {
            if  (pCommand->pLastCallGoto) {
                pCommand = pCommand->pLastCallGoto;
                strcpy (input_line, pCommand->szLine);
                index1 = 0;
                More_Commands = 'y';
                if  (input_line [0] == '+') {
                    input_line [0] = ' ';
                }
            }
            else {
                do  {
                    index1 = 0;
                    More_Commands = 'y';
                    strcpy (input_line, pCommand->szLine);
                    c120_parse_command ();
                    if  (strcmp (Command, "+") == 0) {
                        c120_parse_command ();
                    }
                    if  (strlen (Command) < 2) {
                        strcpy (Command, "xx");
                    }
                    if  (((memcmp (Command, "DO", 2) != 0) &&
                          (memcmp (Command, "do", 2) != 0)) ||
                         (strcmp (Command+2, szDoident) != 0)){
                        if  (pCommand->pPrev) {
                            pCommand = pCommand->pPrev;
                            More_Commands = 'y';
                        }
                        else {
                            pCommand = pDountilcommand;
                            strcpy (Error, "UNTIL without DO!");
                            z110_fatal ();
                            return;
                        }
                    }
                    else {
                        More_Commands = 'n';
                        pDountilcommand->pLastCallGoto = pCommand;
                    }
                }
                while (More_Commands == 'y');
            }
            More_Commands = 'y';
        }
        return;
    }

    /***********************/
    /* ELSE                */
    /***********************/
    if  ((memcmp (Command, "ELSE-", 5) == 0) ||
         (memcmp (Command, "else-", 5) == 0)) {
        if  (If_Proc == 'y') {
            Command [4] = 0;
        }
        else {
            if  (strcmp (szIfName, "") != 0) {
                if  (strcmp (szIfName, Command + 5)  == 0) {
                    Command[4] = 0;
                }
                else {
                    //*******************************
                    //* Find correct else, or end of IF
                    //*******************************
                    do  {
                        if  (!pCommand->pNext) {
                                If_Proc = 'n';
                                More_Commands = 'n';
                                return;
                        }
                        pCommand = pCommand->pNext;    
                        index1 = 1;
                        More_Commands = 'y';
                        strcpy (input_line, pCommand->szLine);
                        if  (input_line[0] != '+') {
                            If_Proc = 'n';
                            More_Commands = 'y';
                            break;
                        }
                        c120_parse_command ();   
                        if  (More_Commands == 'y') {
                            if  ((memcmp (Command, "ELSE-", 5) == 0) ||
                                 (memcmp (Command, "else-", 5) == 0)) {
                                if  (strcmp (szIfName, "") != 0) {
                                    if  (strcmp (szIfName, Command + 5) == 0) {
                                        strcpy (szIfName, "");
                                        Command [4] = 0;
                                    }
                                }
                            }
                        }
                    }
                    while ((strcmp (Command, "ELSE") != 0) &&
                           (strcmp (Command, "else") != 0));
                }
            }
           
        }
    }

    if  ((strcmp (Command, "ELSE") == 0) ||
         (strcmp (Command, "else") == 0)) {
        if  (If_Proc == 'y') {
            If_Proc = 'n';
            if (interactive == 'y') {
               More_Commands = 'n';
               return;
            }
            //*********************************
            //* Skip the else command
            //*********************************
            do  {
                if  (pCommand->pNext) {
                    pCommand = pCommand->pNext;
                }
            }
            while ((pCommand->pNext) &&
                   (pCommand->szLine [0] == '+'));
            if  (pCommand->szLine [0] == '+') {
                More_Commands = 'n';
            }
            else {
                More_Commands = 'y';
                index1 = 0;
                strcpy (input_line, pCommand->szLine);
            }
        }
        else {
            If_Proc = 'y';
        }
        return;
    }

    /***********************/
    /* ENDIF               */
    /***********************/
    if  ((strcmp (Command, "ENDIF") == 0) ||
         (strcmp (Command, "endif") == 0)) {
        return;
    }

    /***********************/
    /* COUNTRY             */
    /***********************/
    if  ((strcmp (Command, "COUNTRY") == 0) ||
         (strcmp (Command, "country") == 0)) {
        c120_parse_command ();
        strcpy ((unsigned char *)szCountry, Command);
        return;
    }

    /***********************/
    /* CLEAR               */
    /***********************/
    memcpy (buff, Command, 5);
    buff [5] = 0;
    if  ((strcmp (buff, "CLEAR") == 0) ||
         (strcmp (buff, "clear") == 0)) {
        d300_clear_buffer ();
        return;
    }

    /***********************/
    /* BUFFER              */
    /***********************/
    if  ((strcmp (Command, "BUFFER") == 0) ||
         (strcmp (Command, "buffer") == 0)) {
        d330_dump_buffer ();
        return;
    }
    /***********************/
    /* CALL Label          */
    /***********************/
    if  ((strcmp (Command, "CALL") == 0) ||
         (strcmp (Command, "call") == 0)) {
//        pCommand->pLastCallGoto = NULL;
        pLastCallGoto = NULL;
        d400_call ();
        return;
    }
    /***********************/
    /* Return              */
    /***********************/
    if  ((strcmp (Command, "RETURN") == 0) ||
         (strcmp (Command, "return") == 0)) {
        d410_return ();
        return;
    }

    /***********************/
    /* PASTEB              */
    /***********************/
    if  ((strcmp (Command, "PASTEB") == 0) ||
         (strcmp (Command, "pasteb") == 0)) {
        InsertBefore = 'y';
        d320_paste_buffer ();
        InsertBefore = 'n';
        return;
    }
    /***********************/
    /* PASTE               */
    /***********************/
    if  ((strcmp (Command, "PASTE") == 0) ||
         (strcmp (Command, "paste") == 0)) {
        InsertBefore = 'n';
        d320_paste_buffer ();
        return;
    }

    /***********************/
    /* INTEGER             */
    /***********************/
    if  ((strcmp (Command, "INTEGER") == 0) ||
         (strcmp (Command, "integer") == 0)) {
        d500_integer ();
        return;
    }
    /***********************/
    /* INT                 */
    /***********************/
    if  ((strcmp (Command, "INT") == 0) ||
         (strcmp (Command, "int") == 0)) {
        d500_integer ();
        return;
    }

    /***********************/
    /* STRING              */
    /***********************/
    if  ((strcmp (Command, "STRING") == 0) ||
         (strcmp (Command, "string") == 0)) {
        d510_string ();
        return;
    }

    /***********************/
    /* CHAR                */
    /***********************/
    if  ((strcmp (Command, "CHAR") == 0) ||
         (strcmp (Command, "char") == 0)) {
        d510_string ();
        return;
    }

    /***********************/
    /* DOLLAR              */
    /***********************/
    if  ((strcmp (Command, "DOLLAR") == 0) ||
         (strcmp (Command, "dollar") == 0)) {
        d520_float ();
        return;
    }

    /***********************/
    /* FLOAT               */
    /***********************/
    if  ((strcmp (Command, "FLOAT") == 0) ||
         (strcmp (Command, "float") == 0)) {
        d520_float ();
        return;
    }

    /***********************/
    /* ERASE               */
    /***********************/
    if  ((strcmp (Command, "ERASE") == 0) ||
         (strcmp (Command, "erase") == 0)) {
        if  (cFast == 'y') {
            strcpy (Error, "Can't use with FAST");
            z110_fatal ();
            return;
        }
        d530_erase ();
        return;
    }

    /***********************/
    /* INITIALIZE          */
    /***********************/
    if  ((strcmp (Command, "INITIALIZE") == 0) ||
         (strcmp (Command, "initialize") == 0)) {
        d540_initialize ();
        return;
    }

    /***********************/
    /* FILES               */
    /***********************/
    if  ((strcmp (Command, "FILES") == 0) ||
         (strcmp (Command, "files") == 0)) {
        d598_files ();
        return;
    }

    /***********************/
    /* I                   */
    /***********************/
    if  ((strcmp (Command, "I") == 0) ||
         (strcmp (Command, "i") == 0)) {
        Before = 'n';
        d165_insert_mode ();
        Before = 'n';
        return;
    }

    /***********************/
    /* IB                  */
    /***********************/
    if  ((strcmp (Command, "IB") == 0) ||
         (strcmp (Command, "ib") == 0)) {
        Before = 'y';
        d165_insert_mode ();
        Before = 'n';
        return;
    }

    /***********************/
    /* DUMP name           */
    /***********************/
    if  ((strcmp (Command, "DUMP") == 0) ||
         (strcmp (Command, "dump") == 0)) {
        d599_dump ();
        return;
    }

    /***********************/
    /* SIZE                */
    /***********************/
    if  ((strcmp (Command, "SIZE") == 0) ||
         (strcmp (Command, "size") == 0)) {
        e100_size ();
        return;
    }
    /***********************/
    /* RANDOM              */
    /***********************/
    if  ((strcmp (Command, "RANDOM") == 0) ||
         (strcmp (Command, "random") == 0)) {
        e102_random ();
        return;
    }
    /***********************/
    /* ENCRYPT             */
    /***********************/
    if  ((strcmp (Command, "ENCRYPT") == 0) ||
         (strcmp (Command, "encrypt") == 0)) {
        e105_encrypt (1, 's');
        return;
    }
    /***********************/
    /* DECRYPT             */
    /***********************/
    if  ((strcmp (Command, "DECRYPT") == 0) ||
         (strcmp (Command, "decrypt") == 0)) {
        e105_encrypt (0, 's');
        return;
    }

    /***********************/
    /* ENCRYPTFILE         */
    /***********************/
    if  ((strcmp (Command, "ENCRYPTFILE") == 0) ||
         (strcmp (Command, "encryptfile") == 0)) {
        e106_encrypt_file (1);
        return;
    }
    /***********************/
    /* DECRYPTFILE         */
    /***********************/
    if  ((strcmp (Command, "DECRYPTFILE") == 0) ||
         (strcmp (Command, "decryptfile") == 0)) {
        e106_encrypt_file (0);
        return;
    }
        
    /***********************/
    /* PACK                */
    /***********************/
    if  ((strcmp (Command, "PACK") == 0) ||
         (strcmp (Command, "pack") == 0)) {
        e110_pack ();
        return;
    }
    /***********************/
    /* CRUSH               */
    /***********************/
    if  ((strcmp (Command, "CRUSH") == 0) ||
         (strcmp (Command, "crush") == 0)) {
        e120_crush ();
        return;
    }
    /***********************/
    /* RIGHT               */
    /***********************/
    if  ((strcmp (Command, "RIGHTJUST") == 0) ||
         (strcmp (Command, "rightjust") == 0)) {
        e130_right ();
        return;
    }
    /***********************/
    /* LEFT                */
    /***********************/
    if  ((strcmp (Command, "LEFTJUST") == 0) ||
         (strcmp (Command, "leftjust") == 0)) {
        e140_left ();
        return;
    }
    /***********************/
    /* MIX                 */
    /***********************/
    if  ((strcmp (Command, "MIX") == 0) ||
         (strcmp (Command, "mix") == 0)) {
        e150_mix ();
        return;
    }
    /***********************/
    /* UPPER               */
    /***********************/
    if  ((strcmp (Command, "UPPER") == 0) ||
         (strcmp (Command, "upper") == 0)) {
        e160_upper ();
        return;
    }
    /***********************/
    /* LOWER               */
    /***********************/
    if  ((strcmp (Command, "LOWER") == 0) ||
         (strcmp (Command, "lower") == 0)) {
        e170_lower ();
        return;
    }

    /***********************/
    /* DEBUG               */
    /***********************/
    if  ((strcmp (Command, "DEBUG=ON") == 0) ||
         (strcmp (Command, "debug=on") == 0)) {
        Debug = 'y';
        return;
    }
    if  ((strcmp (Command, "DEBUG=OFF") == 0) ||
         (strcmp (Command, "debug=off") == 0)) {
        Debug = 'n';
        return;
    }

    /***********************/
    /* TRACE               */
    /***********************/
    if  ((strcmp (Command, "TRACE=ON") == 0) ||
         (strcmp (Command, "trace=on") == 0)) {
        if  (Beditwin == 'y') {
            strcpy (Error, "Unavail in Beditwin");
            z110_fatal ();
            return;
        }
        trace = 'y';
        return;
    }
    if  ((strcmp (Command, "TRACE=OFF") == 0) ||
         (strcmp (Command, "trace=off") == 0)) {
        trace = 'n';
        return;
    }
    if  ((strcmp (Command, "TRACE=LOG") == 0) ||
         (strcmp (Command, "trace=log") == 0)) {
        trace = 'x';
        return;
    }

    /***********************/
    /* VERI                */
    /***********************/
    if  ((strcmp (Command, "VERI") == 0) ||
         (strcmp (Command, "veri") == 0)) {
        Veri = 'y';
        return;
    }

    /***********************/
    /* NOVE                */
    /***********************/
    if  ((strcmp (Command, "NOVE") == 0) ||
         (strcmp (Command, "nove") == 0)) {
        Veri = 'n';
        return;
    }

    /***********************/
    /* FRONT               */
    /***********************/
    memcpy (buff, Command, 6);
    buff [6] = 0;
    if  ((stricmp (buff, "FRONT:") == 0) ||
         (stricmp (buff, "front:") == 0)) {
        d250_front ();
        return;
    }

    /***********************/
    /* BACK                */
    /***********************/
    memcpy (buff, Command, 5);
    buff [5] = 0;
    if  ((stricmp (buff, "BACK:") == 0) ||
         (stricmp (buff, "back:") == 0)) {
        d260_back ();
        return;
    }
    /*************************************************************************************/
    /* Variable Assignment : Varibable = ... (a = b, a = b + c, a ++, a --, etc...)      */
    /*************************************************************************************/
    test = 'y';
    g100_convert_variable ();
    d580_find_variable ();
    test = 'n';
    if  (found == 'y') {
        d600_variable ();
        return;
    }
    /***********************/
    /* DO                  */
    /***********************/
    /* Must be checked below DOLLAR due to the first 2 chars of DOllar */
    if  ((memcmp (Command, "DO", 2) == 0) ||
         (memcmp (Command, "do", 2) == 0)) {
        Dountil = 'n';
        return;
    }
    /***********************/
    /* CUT;n               */
    /***********************/
    memcpy (buff, Command, 3);
    buff [3] = 0;
    if  ((strcmp (buff, "CUT") == 0) ||
         (strcmp (buff, "cut") == 0)) {
        cut = 'y';
        pcount = 0;
        d310_copy_buffer ();
        cut = 'n';
        return;
    }
    /***********************/
    /* COPY;n              */
    /***********************/
    memcpy (buff, Command, 4);
    buff [4] = 0;
    if  ((strcmp (buff, "COPY") == 0) ||
         (strcmp (buff, "copy") == 0)) {
        cut = 'n';
        pcount = 0;
        d310_copy_buffer ();
        return;
    }
    /***********************/
    /* EXIT                */
    /***********************/
    memcpy (buff, Command, 4);
    buff [4] = 0;
    if  ((strcmp (buff, "EXIT") == 0) ||
         (strcmp (buff, "exit") == 0)) {
        eof_cmd_in = 'y';
        More_Commands = 'n';
        return;
    }

    /***********************/
    /* B B;n               */
    /***********************/
    memcpy (buff, Command, 1);
    buff [1] = 0;
    if  ((stricmp (buff, "B") == 0) ||
         (stricmp (buff, "b") == 0)) {
        d120_process_B ();
        return;
    }

    /***********************/
    /* PS:/xxx/ PS:/xxx/;n */
    /***********************/
    memcpy (buff, Command, 3);
    buff [3] = 0;
    if  ((stricmp (buff, "PS:") == 0) ||
         (stricmp (buff, "ps:") == 0)) {
        Verify = 'y';
        d150_process_FS ();
        Verify = 'n';
        return;
    }

    /***********************/
    /* P P;n P;*           */
    /***********************/
    memcpy (buff, Command, 1);
    buff [1] = 0;
    if  ((stricmp (buff, "P") == 0) ||
         (stricmp (buff, "p") == 0)) {
        d130_process_P ();
        return;
    }

    /***********************/
    /* FS:/xxx/ FS:/xxx/;n */
    /***********************/
    memcpy (buff, Command, 3);
    buff [3] = 0;
    if  ((stricmp (buff, "FS:") == 0) ||
         (stricmp (buff, "fs:") == 0)) {
        Verify = 'n';
        d150_process_FS ();
        return;
    }

    /***********************/
    /* F F;n F;*           */
    /***********************/
    memcpy (buff, Command, 1);
    buff [1] = 0;
    if  ((stricmp (buff, "F") == 0) ||
         (stricmp (buff, "f") == 0)) {
        d140_process_F ();
        return;
    }

    /***********************/
    /* IB:/xxx/            */
    /***********************/
    memcpy (buff, Command, 2);
    buff [2] = 0;
    if  ((stricmp (buff, "IB") == 0) ||
         (stricmp (buff, "ib") == 0)) {
        InsertBefore = 'y';
        d160_insert_line ();
        InsertBefore = 'n';
        return;
    }

    /***********************/
    /* I:/xxx/             */
    /***********************/
    memcpy (buff, Command, 1);
    buff [1] = 0;
    if  ((stricmp (buff, "I") == 0) ||
         (stricmp (buff, "i") == 0)) {
        InsertBefore = 'n';
        d160_insert_line ();
        return;
    }

    /***********************/
    /* D D;n D;*           */
    /***********************/
    memcpy (buff, Command, 1);
    buff [1] = 0;
    if  ((stricmp (buff, "D") == 0) ||
         (stricmp (buff, "d") == 0)) {
        pcount = 0;
        d170_process_D ();
        return;
    }

    /***********************/
    /* RVS:/xxx/:/yyy/;n   */
    /***********************/
    memcpy (buff, Command, 4);
    buff [4] = 0;
    if  ((stricmp (buff, "RVS:") == 0) ||
         (stricmp (buff, "rvs:") == 0)) {
        Verify = 'y';
        d190_process_RV ();
        Verify = 'n';
        return;
    }

    /***********************/
    /* RS:/xxx/:/yyy/;n    */
    /***********************/
    memcpy (buff, Command, 3);
    buff [3] = 0;
    if  ((stricmp (buff, "RS:") == 0) ||
         (stricmp (buff, "rs:") == 0)) {
        Verify = 'n';
        d190_process_RV ();
        return;
    }

    /***********************/
    /* R:/xxx/             */
    /***********************/
    memcpy (buff, Command, 2);
    buff [2] = 0;
    if  ((stricmp (buff, "R:") == 0) ||
         (stricmp (buff, "r:") == 0)) {
        Verify = 'n';
        d200_process_R ();
        return;
    }

    /***********************/
    /* RV:/xxx/            */
    /***********************/
    memcpy (buff, Command, 3);
    buff [3] = 0;
    if  ((stricmp (buff, "RV:") == 0) ||
         (stricmp (buff, "rv:") == 0)) {
        Verify = 'y';
        d200_process_R ();
        Verify = 'n';
        return;
    }

    sprintf (Error, "Syntax Error / Variable not Defined [%s]", Command);
    z110_fatal ();

    return;
}

/************************************************************************/
/*                                                                      */
/*                         End of program BEDIT.C                       */
/*                                                                      */
/************************************************************************/
/* Formatted on 09/21/90 at 14:24:23 by Forrest Bentley's C-Format v2.0 */
