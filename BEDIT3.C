/*************************************************************************/
/* Program          : BEDIT3.c                   v1.4                    */
/* Author           : Forrest Bentley                                    */
/* Date Written     : January 15, 1992                                   */
/*                                                                       */
/* Description      :                                                    */
/*     This program will process all commands in an input command file.  */
/*                                                                       */
/*  Forrest Bentley   05-11-1992 Split BEDIT2.c into BEDIT2.c & BEDIT3.c */
/*  Forrest Bentley   05-16-1992 Added string name = '...'               */
/*                               Added integer name = '...'              */
/*                               Added INSERT, DELETE, HOME, END, PAGEUP */
/*                                     PAGEDOWN, ESC to functionkey      */
/*  Forrest Bentley   09/25/2013 Added DUMP Processing:                  */
/*                                     DUMP SYSTEM                       */
/*                                     DUMP CUSTOM                       */
/*                                     Line up Dump output               */
//* Forrest Bentley   09/26/2013 Added logic to line up dump field output*/ 
//* Forrest Bentley   10/02/2013 Added Scramble on/off logic             */ 
//* Forrest Bentley   10/03/2013 Added ENCRYPTKEY, ENCRYPT, DECRYPT      */ 
//* Forrest Bentley   10/08/2013 Corrected @^ Recursive Field Locator    */
//*                              in d580_find_variable ()                */
//* Forrest Bentley   02/03/2014 Added trace=x logic to d580             */ 
//* Forrest Bentley   02/14/2014 Added code to STOP debugging if abort   */
//* Forrest Bentley   09/02/2014 Added SIN,COS,TAN                       */
//* Forrest Bentley   11/25/2014 Corrected ASIN,ACOS,ATAN                */
//* Forrest Bentley   12/10/2014 Fixed single char 'mix'                 */
/*************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
//#include <graph.h>
#include <time.h>
#include <string.h>
#include <MALLOC.h>
#include <conio.h>
#include <io.h>
#include <math.h>
#include <errno.h>
#include <direct.h>
#include <signal.h>
#include <dos.h>
#include <dir.h>
#include <windows.h>

#include "BEDIT.h"
#include "BEDITvar.h"

static int Batcall = 0;

#define PI 3.14159265

/********************************************************************/
/* This module will :                                               */
/*     process CLEAR                                                */
/********************************************************************/
void d300_clear_buffer () {
//  printf ("%s", "d300_clear_buffer\n");
    pBuffer = &StartBuffer;
    do  {
        if  (pBuffer->pNext) {
            pBuffer = pBuffer->pNext;
        }
    }
    while (pBuffer->pNext);
    do  {
        if  (pBuffer->pPrev) {
            pNewBuffer = pBuffer->pPrev;
            free (pBuffer);
            pBuffer = pNewBuffer;
        }
    }
    while (pBuffer->pPrev);
    pBuffer->pNext = NULL;
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process CUT CUT;n CUT;* COPY COPY;n COPY;*                   */
/********************************************************************/
void d310_copy_buffer () {
//  printf ("%s", "d310_copy_buffer\n");
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    Number = 1;
    if  (cut == 'y') {
        index4 = 3;
    }
    else {
        index4 = 4;
    }
    if  (Command [index4] != 0) {
        if  (Command [index4] == ':') {
            strcpy (buff, "fs");
            strcat (buff, Command + index4);
            strcpy (Command, buff);
            cutcopy = 'y';
            d150_process_FS ();
            cutcopy = 'n';
            return;
        }
        if  (Command [index4] != ';') {
            strcpy (Error, "Bad CUT/COPY");
            z110_fatal ();
            return;
        }
        if  (Command [index4 + 1] == '*') {
            Number = 999999;
        }
        else {
            if  (Command [index4 + 1] == Subs) {
                strcpy (buff, Command + index4 + 1);
                strcpy (temp, Command);
                strcpy (Command, buff);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = (long) pVariable->Integer;
                }
                else {
                    Number = (long int) strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                strcpy (buff, Command + index4 + 1);
                Number = (long int) strtol((char*)buff, (char**)&p, 10);
            }
        }
        if  (Number < 1) {
            strcpy (Error, "Bad CUT/COPY Cnt");
            z110_fatal ();
            return;
        }
    }

    if  (ClearBuff == 'y') {
        d300_clear_buffer ();
    }

    if  (Number > 0) {
        do  {
            Number--;
            pcount++;
            pNewBuffer = (BUFFER *) malloc (sizeof (BUFFER));
            if  (!pNewBuffer) {
                strcpy (Error, "Unable to CUT/COPY");
                z110_fatal ();
                return;
            }
            memset (pNewBuffer, 0, sizeof (BUFFER));
            pBuffer->pNext = pNewBuffer;
            pNewBuffer->pPrev = pBuffer;
            pNewBuffer->pNext = NULL;
            strcpy (pNewBuffer->szLine, pDataFile->szLine);
            if  (Veri == 'y') {
                printf ("%s", pDataFile->szLine);
            }
            pBuffer = pNewBuffer;
            index4 = (int) Number;            // Hold the Number to allow Next
            Number = 1;
            if  (!pDataFile->pNext) {
                index4 = 0;
            }
            if  (cut == 'y') {
                strcpy (Command, "D");
                d170_process_D ();
                pcount--;
            }
            else {
                if  (index4 > 0) {
                    f100_next ();
                    if  (EOF_Flag != 'y') {
                        pcount--;
                    }
                }
            }
            Number = index4;
        }
        while (Number > 0);
    }
    ClearBuff = 'n';
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process PASTE PASTEB                                         */
/********************************************************************/
void d320_paste_buffer () {
//  printf ("%s", "d320_paste_buffer\n");
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    pBuffer = &StartBuffer;
    do  {
        if  (pBuffer->pNext) {
            pBuffer = pBuffer->pNext;
            if  (pBuffer->szLine [0] != 0) {
                strcpy (Command, "I:");
                strcat (Command, splitter);
                strcat (Command, pBuffer->szLine);
                Command [strlen (Command) - 1] = 0;
                strcat (Command, splitter);
                d160_insert_line ();
                InsertBefore = 'n';
            }
        }
    }
    while (pBuffer->pNext);
    ClearBuff = 'y';
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process BUFFER                                               */
/********************************************************************/
void d330_dump_buffer () {
    pBuffer = &StartBuffer;
    do  {
        if  (pBuffer->pNext) {
            pBuffer = pBuffer->pNext;
            printf ("%s", pBuffer->szLine);
            if  ((Debug == 'y') || (Beditwin == 'y')) {
                z999_open_bug ();
                fputs (pBuffer->szLine, file_debug_out);
            }
        }
    }
    while (pBuffer->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process LIST                                                 */
/********************************************************************/
void d350_list () {
    c120_parse_command ();
    if  (Command [0] == 0) {
        strcpy (input_line, "b p;*");
    }
    else {
        strcpy (szScanStr, Command);
        strcpy (szScanTest, "-");
        z900_scan ();
        if  (cScanFound == 'y') {
            Num1 = 0;
            Num1 = (long int) strtol ((char*)szScanLeft, (char**)&p, 10);
            if  ((strcmp (szScanRight, "PAGE") == 0) ||
                 (strcmp (szScanRight, "page") == 0)) {
                sprintf (szScanRight, "%d", Num1 + 20);
            }
            Num2 = 0;
            Num2 = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
            if  (Num1 == 0) {
                Num1 = 1;
            }
            if  (Num2 < Num1) {
                Num2 = 30000;
            }
            if  (Num1 == 1) {
                sprintf (input_line, "b p;%d", (Num2 - Num1) + 1);
            }
            else {
                sprintf (input_line, "b f;%d p;%d", Num1 - 1, (Num2 - Num1) + 1);
            }
        }
        else {
            if  ((strcmp (Command, "PAGE") == 0) ||
                 (strcmp (Command, "page") == 0) ||
                 (strcmp (Command, "NEXT") == 0) ||
                 (strcmp (Command, "next") == 0)) {
                strcpy (input_line, "p;20");
            }
            else {
                if  ((strcmp (Command, "PREV") == 0) ||
                     (strcmp (Command, "prev") == 0)) {
                    strcpy (input_line, "b;38 p;20");
                }
                else {
                    Num1 = 0;
                    Num1 = (long int) strtol ((char*)Command, (char**)&p, 10);
                    if  (Num1 < 2) {
                        strcpy (input_line, "b p");
                    }
                    else {
                        sprintf (input_line, "b f;%d p", Num1 - 1);
                    }
                }
            }
        }
    }
    index1 = 0;
    More_Commands = 'y';
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the BEGINEND command                                 */
/********************************************************************/
void d360_beginend () {
    c120_parse_command ();
    if  ((strcmp (Command, "OFF") == 0) ||
         (strcmp (Command, "off") == 0)) {
        BeginEnd = 0;
        return;
    }
    if  (Cols == 'y') {
        strcpy (Error, "No BEGINEND with COLS");
        z110_fatal ();
        return;
    }
    if  ((strcmp (Command, "BEGIN") == 0) ||
         (strcmp (Command, "begin") == 0)) {
        BeginEnd = 'B';
        return;
    }
    if  ((strcmp (Command, "END") == 0) ||
         (strcmp (Command, "end") == 0)) {
        BeginEnd = 'E';
        return;
    }
    z010_syntax_error ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the STATUS command                                   */
/********************************************************************/
void d370_status () {
    printf ("\n********************** BEDIT Status *****************************\n");
    /*************************/
    /* EOF                   */
    /*************************/
    sprintf (buff, "EOF             = [%c]\n", EOF_Flag);
    printf ("%s", buff);
    /*************************/
    /* Max Chars             */
    /*************************/
    sprintf (buff, "Max Chars       = [%d]\n", Max);
    printf ("%s", buff);
    /*************************/
    /* Subs                  */
    /*************************/
    sprintf (buff, "Subs            = [%c]\n", Subs);
    printf ("%s", buff);
    /*************************/
    /* Verify                */
    /*************************/
    if  (Veri == 'y') {
        strcpy (buff, "Verify          = [ON]\n");
    }
    else {
        strcpy (buff, "Verify          = [OFF]\n");
    }
    printf ("%s", buff);
    /*************************/
    /* Debug                 */
    /*************************/
    if  (Debug == 'y') {
        strcpy (buff, "Debug           = [ON]\n");
    }
    else {
        strcpy (buff, "Debug           = [OFF]\n");
    }
    printf ("%s", buff);
    /*************************/
    /* Trace                 */
    /*************************/
    if  (trace == 'y') {
        strcpy (buff, "Trace           = [ON]\n");
    }
    else {
        strcpy (buff, "Trace           = [OFF]\n");
    }
    printf ("%s", buff);
    /*************************/
    /* Break                 */
    /*************************/
    if  (BreakStatus == 'y') {
        strcpy (buff, "BREAK           = [ON]\n");
    }
    else {
        strcpy (buff, "BREAK           = [OFF]\n");
    }
    printf ("%s", buff);
    /*************************/
    /* CASE                  */
    /*************************/
    if  (cCase == 'y') {
        strcpy (buff, "Case Sensitive  = [ON]\n");
    }
    else {
        strcpy (buff, "Case Sensitive  = [OFF]\n");
    }
    printf ("%s", buff);
    /*************************/
    /* Cols                  */
    /*************************/
    if  (Cols == 'y') {
        sprintf (buff, "Cols            = [%d-%d]\n", ColStart + 1, ColEnd + 1);
    }
    else {
        sprintf (buff, "Cols            = [OFF]\n");
    }
    printf ("%s", buff);
    /*************************/
    /* BEGINEND              */
    /*************************/
    if  (BeginEnd == 'B') {
        sprintf (buff, "BEGINEND        = [BEGIN]\n");
    }
    else {
        if  (BeginEnd == 'E') {
            sprintf (buff, "BEGINEND        = [END]\n");
        }
        else {
            sprintf (buff, "BEGINEND        = [OFF]\n");
        }
    }
    printf ("%s", buff);
    /*************************/
    /* PATTERN               */
    /*************************/
    if  (Patton == 'y') {
        sprintf (buff, "PATTERN         = [%c%c%c%c%c] ON\n",
                 Pattany, Pattnum, Pattalp, Pattspc, Pattend);
    }
    else {
        sprintf (buff, "PATTERN         = [%c%c%c%c%c] OFF\n",
                Pattany, Pattnum, Pattalp, Pattspc, Pattend);
    }
    printf ("%s", buff);
    /*************************/
    /* Variable Count        */
    /*************************/
    count = 0;
    pCountVariable = &StartVariable;
    do  {
        if  (pCountVariable->pNext) {
            pCountVariable = pCountVariable->pNext;
            count++;
        }
    }
    while (pCountVariable->pNext);
    sprintf (buff, "Count Variables = [%d]\n", count);
    printf ("%s", buff);
    /*************************/
    /* Buffer Count          */
    /*************************/
    count = 0;
    pCountBuffer = &StartBuffer;
    do  {
        if  (pCountBuffer->pNext) {
            pCountBuffer = pCountBuffer->pNext;
            count++;
        }
    }
    while (pCountBuffer->pNext);
    sprintf (buff, "Count Buffer    = [%d]\n", count);
    printf ("%s", buff);
    /*************************/
    /* Files Count           */
    /*************************/
    count = 0;
    pCountFiles = &StartOpenFile;
    do  {
        if  (pCountFiles->pNext) {
            pCountFiles = pCountFiles->pNext;
            count++;
        }
    }
    while (pCountFiles->pNext);
    sprintf (buff, "Count Files     = [%d]\n", count);
    printf ("%s", buff);
    /*************************/
    /* Calls Count           */
    /*************************/
    count = 0;
    pCountCall = &StartCall;
    do  {
        if  (pCountCall->pNext) {
            pCountCall = pCountCall->pNext;
            count++;
        }
    }
    while (pCountCall->pNext);
    sprintf (buff, "Call Level      = [%d]\n", count);
    printf ("%s", buff);
    /*************************/
    /* BEDIT Levels         */
    /*************************/
    count = 1;
    if  (BEDITlevel > 0) {
        count = BEDITlevel;
    }
    sprintf (buff, "BEDIT Level     = [%d]\n", count);
    printf ("%s", buff);

    printf ("******************************************************************\n");
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the CALL command                                     */
/********************************************************************/
void d400_call () {

    unsigned char     cPerm;
    unsigned char     cLoaded;

    iEncryptKeyPtr = 0;
    /*******************************/
    /* Erase existing ARGS         */
    /*******************************/
    pArgs = &StartArgs;
    do  {
        if  (pArgs->pNext) {
            pPrevArgs = pArgs;
            pArgs = pArgs->pNext;
            if  (pPrevArgs->szArg) {
                free (pPrevArgs->szArg);
            }
            free (pPrevArgs);
        }
    }
    while (pArgs->pNext);
    if  (pArgs != &StartArgs) {
        if  (pPrevArgs->szArg) {
            free (pPrevArgs->szArg);
        }
        free (pPrevArgs);
    }
    pArgs = &StartArgs;
    pArgs->pNext = NULL;

    pNewCall = (CALL *) malloc (sizeof (CALL));
    if  (!pNewCall) {
        strcpy (Error, "Call Stack Exceeded");
        z110_fatal ();
        return;
    }
    memset (pNewCall, 0, sizeof (CALL));
    pNewCall->input_line = (unsigned char *) malloc (strlen (input_line) + 5);
    if  (!pNewCall->input_line) {
        strcpy (Error, "CALL Input Memory Error");
        z110_fatal ();
        return;
    }
    strcpy (pNewCall->input_line, input_line);
    do  {
        if  (input_line [index1] == ' ') {
            index1++;
        }
    }
    while (input_line [index1] == ' ');
    pNewCall->cPerm = 'n';
    index2 = 0;
    do  {
        if  (input_line [index1] != ' ') {
            Command [index2] = input_line [index1];
            index2++;
        }
        index1++;
    }
    while ((input_line [index1] != ' ') && (input_line [index1] != 0));
    do  {
        if  (input_line [index1] == ' ') {
            index1++;
        }
    }
    while (input_line [index1] == ' ');
    Command [index2] = 0;
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (Command, pVariable->szString);
    }
    /*******************/
    /* CALL *          */
    /*******************/
    if  (strcmp (Command, "*") == 0) {
        if  (!pOpenFile) {
            strcpy (Error, "No Curr File");
            z110_fatal ();
            return;
        }
        strcpy (szIf1, input_line);  /* Use szIf1 as temporary storage */
        sprintf (input_line, "%s %s", pOpenFile->szFileName, "BEDIT.CUR");
        index4 = index1;
        index1 = 0;
        d110_process_write ();
        index1 = index4;
        strcpy (input_line, szIf1);
        strcpy (Command, "BEDIT.CUR");
    }
    d405_call_args ();
    /*************************/
    /* Set up for RETURN     */
    /*************************/
    pCall->pNext = pNewCall;
    pNewCall->pPrev = pCall;
    pNewCall->pNext = NULL;
    pNewCall->index1 = index1;
    pNewCall->pCommand = pCommand;
    pNewCall->More_Commands = More_Commands;
    pNewCall->If_Proc = If_Proc;
    pNewCall->EOF_Flag = EOF_Flag;
    pNewCall->pStartCommand = NULL;
    pNewCall->pPrevStartCommand = NULL;
    pNewCall->pVariable = NULL;
    /*****************************************/
    /* Set up for Var = CALL ...             */
    /*****************************************/
    if  (CallVar == 'y') {
        CallVar = 'n';
        pNewCall->pVariable = pCountVariable;
    }
    memset (pNewCall->szFilename, 0, sizeof (pNewCall->szFilename));
    strcpy (pNewCall->szFilename, szCurrentFile);
    CallLevel++;

    pCall = pNewCall;
    if  (Command [0] == '!') {
        pLastCallGoto = pCommand;
    }
    if  (Command [0] != '!') {
        /****************************/
        /* Check for File!Label     */
        /****************************/
        strcpy (szFileLabel, "");
        strcpy (szScanStr, Command);
        strcpy (szScanTest, "!");
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Command, szScanLeft);
            strcpy (szFileLabel, "!");
            strcat (szFileLabel, szScanRight);
        }

        pCall->cPerm = 'n';
        cPerm = 'n';
        pPermenant = &StartPermenant;
        do  {
            if  (pPermenant->pNext) {
                pPermenant = pPermenant->pNext;
                if  (strcmp (pPermenant->szName, Command) == 0) {
                    cPerm = 'y';
                    pCall->cPerm = 'y';
                }
            }
        }
        while ((pPermenant->pNext) && (cPerm == 'n'));
        if  (cPerm == 'y') {
            cLoaded = pPermenant->cLoaded;
        }

        /******************************************/
        /* Find the Command File in the DPATH     */
        /******************************************/
        if  ((cPerm == 'n') || (cLoaded != 'y')) {
            strcat (szScanRight, envstr);
            d401_find_file ();
            file_cmd_in = fopen (Command, "r");
            if  (!file_cmd_in) {
                strcpy (Error, "Can't Find Cmd File");
                z110_fatal ();
                return;
            }
        }
        strcpy (pCall->szFilename, Command);
        strcpy (szCurrentFile, Command);

        pCall->pPrevStartCommand = pStartCommand;
        if  ((cPerm == 'n') || (cLoaded != 'y')) {
            pStartCommand = (COMMAND *) malloc (sizeof (COMMAND));
            if  (!pStartCommand) {
                strcpy (Error, "Bad Load Cmd File");
                z110_fatal ();
                return;
            }
            memset (pStartCommand, 0, sizeof (COMMAND));
            pCommand = pStartCommand;
            pCommand->pNextLabel = NULL;
            pLabel = pStartCommand;

            pCommand->pNext = NULL;
            pCommand->pPrev = NULL;
            pCommand->szLine = (unsigned char *) malloc (5);
            if  (!pCommand->szLine) {
                strcpy (Error, "Unable to Load Start\n");
                z110_fatal ();
                return;
            }
            memset (pCommand->szLine, 0, 5);
            strcpy (pCommand->szLine, "");
        }
        else {
            pStartCommand = pPermenant->pStartCommand;
        }

        LineNo = 0;

        if  ((cPerm == 'n') || (cLoaded != 'y')) {
            do  {
                c100_read_cmd_in ();
                if  (eof_cmd_in == 'y') {
                    strcpy (input_line, "RETURN");
                }
                if  (cEncryptCommand != 'y') {
                    if  (input_line [0] != 0) {
                        /******************************************/
                        /* Remove trailing spaces                 */
                        /******************************************/
                        index8 = strlen (input_line) - 1;
                        do  {
                            if  ((input_line [index8] == 32) ||
                                 (input_line [index8] == 13)) {
                                input_line [index8] = 0;
                                index8--;
                            }
                            else {
                                index8 = 0;
                            }
                        }
                        while (index8 > 0);
                    }
                }

                pNewCommand = (COMMAND *) malloc (sizeof (COMMAND));
                if  (!pNewCommand) {
                    strcpy (Error, "Unable to load Cmd File");
                    z110_fatal ();
                    return;
                }
                memset (pNewCommand, 0, sizeof (COMMAND));
                LineNo++;
                pNewCommand->LineNo = LineNo;
                pCommand->pNext = pNewCommand;
                pNewCommand->pPrev = pCommand;
                pNewCommand->pNext = NULL;
                pNewCommand->pNextLabel = NULL;
                pNewCommand->szLine = (unsigned char *) malloc (strlen (input_line) + 5);
                if  (!pNewCommand->szLine) {
                    strcpy (Error, "Unable to Load");
                    z110_fatal ();
                    return;
                }
                memset (pNewCommand->szLine, 0, strlen (input_line) + 5);
                strcpy (pNewCommand->szLine, input_line);
                pCommand = pNewCommand;
                if  (input_line [0] == '!') {
                    pLabel->pNextLabel = pNewCommand;
                    pLabel = pNewCommand;
                }
            }
            while (eof_cmd_in != 'y');
        }
        eof_cmd_in = 'n';
        if  ((cPerm == 'n') || (cLoaded != 'y')) {
            fclose (file_cmd_in);
        }

        dCallFileLevel++;
        Callfile = 'y';

        pNewCall->pStartCommand = pStartCommand;
        pCommand = pStartCommand;
        index1 = 0;
        More_Commands = 'n';
        interactive = 'n';
        if  (cPerm == 'y') {
            pPermenant->cLoaded = 'y';
            pPermenant->pStartCommand = pStartCommand;
        }
        if  (strcmp (szFileLabel, "") == 0) {
            Callfile = 'n';
            return;
        }
        strcpy (Command, szFileLabel);
    }
    Call = 'y';
    interactive = 'n';
    d114_goto ();
    Callfile = 'n';
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Find the CALLed Command File                                 */
/********************************************************************/
void d401_find_file () {
    cScanFound = 'y';
    do  {
        strcpy (szScanStr, szScanRight);
        strcpy (szScanTest, ";");
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (buff, szScanLeft);
            if  (strcmp (buff, "") != 0) {
                strcat (buff, "\\");
            }
            strcat (buff, Command);
            file_cmd_in = fopen (buff, "r");
            if  (!file_cmd_in) {
                strcat (buff, ".BED");
                file_cmd_in = fopen (buff, "r");
            }
            if  (file_cmd_in) {
                fclose (file_cmd_in);
                strcpy (Command, buff);
                return;
            }
        }
    }
    while (cScanFound == 'y');
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the RETURN command                                   */
/********************************************************************/
void d405_call_args () {
    unsigned char ArgsDone;
    int      ArgCount;

    ArgsDone = 'n';
    if  (input_line [index1] != '[') {
        return;
    }
    strcpy (szIf1, Command);
    pArgs = &StartArgs;
    ArgCount = 0;
    do  {
        /************************/
        /* Get Next Arg         */
        /************************/
        found2 = '~';
        c120_parse_command ();
        found2 = 'n';
        /************************/
        /* Check if finished    */
        /************************/
        if  (strcmp (Command, "") == 0) {
            c110_proc_next_cmd ();
            if  (eof_cmd_in == 'y') {
                strcpy (Error, "Premature EOF Call Args");
                z110_fatal ();
                return;
            }
            index1 = 0;
            More_Commands = 'y';
            c120_parse_command ();
        }
        if  (Command [strlen (Command) - 1] == ']') {
            ArgsDone = 'y';
            Command [strlen (Command) - 1] = 0;
        }
        /************************/
        /* Check Count          */
        /************************/
        ArgCount++;
        if  (ArgCount > 30) {
            strcpy (Error, "Too Many Args > 30");
            z110_fatal ();
            return;
        }
        /************************/
        /* Remove 1st [         */
        /************************/
        if  (Command [0] == '[') {
            strcpy (buff, Command + 1);
            strcpy (Command, buff);
        }
        if  (Command [0] == Subs) {
            d580_find_variable ();
            if  (pVariable->Type == 'I') {
                sprintf (Command, "%d", pVariable->Integer);
            }
            else {
                strcpy (Command, pVariable->szString);
            }
        }
        /************************/
        /* Allocate the ARG     */
        /************************/
        pNextArgs = (ARGS *) malloc (sizeof (ARGS));
        if  (!pNextArgs) {
            strcpy (Error, "Bad Arg Create");
            z110_fatal ();
            return;
        }
        pNextArgs->szArg = (unsigned char *) malloc (strlen (Command) + 5);
        if  (!pNextArgs->szArg) {
            strcpy (Error, "Bad Arg Str Create");
            z110_fatal ();
            return;
        }
        strcpy (pNextArgs->szArg, Command);
        pNextArgs->pPrev = pArgs;
        pNextArgs->pNext = NULL;
        pArgs->pNext = pNextArgs;
        pArgs = pNextArgs;
    }
    while (ArgsDone != 'y');
    pArgs = &StartArgs;
    strcpy (Command, szIf1);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the RETURN command                                   */
/********************************************************************/
void d410_return () {
//    printf ("%s", "d410_return\n");
    if  (!pCall->pPrev) {
        strcpy (Error, "RETURN without CALL");
        z110_fatal ();
        return;
    }
    if  (CallLevel > 0) {
        CallLevel--;
    }

    /************************************/
    /* Release the current command file */
    /************************************/
    if  (pCall->pStartCommand) {
        if  (pCall->cPerm == 'n') {
            pHoldCommand = pCall->pStartCommand;
            do  {
                if  (pHoldCommand->pNext) {
                    pNewCommand = pHoldCommand->pNext;
                }
                else {
                    pNewCommand = NULL;
                }
                free (pHoldCommand);
                if  (pNewCommand) {
                    pHoldCommand = pNewCommand;
                }
            }
            while (pNewCommand);
        }
        pStartCommand = pCall->pPrevStartCommand;
        if  (dCallFileLevel != 0) {
            dCallFileLevel--;
        }
    }
    /***********************************************/
    /* Assign the return value to the variable     */
    /***********************************************/
    if  (pCall->pVariable) {
        pAssignVar = pCall->pVariable;
        c120_parse_command ();
        if  (strcmp (Command, "") == 0) {
            strcpy (Error, "Must return a value!");
            z110_fatal ();
            return;
        }
        if  (Command [0] == Subs) {
            d580_find_variable ();
            if  (pVariable->Type == 'I') {
                sprintf (Command, "%d", pVariable->Integer);
            }
            else {
                strcpy (Command, pVariable->szString);
            }
        }
        if  (pAssignVar->Type == 'I') {
            pAssignVar->Integer = (long int) strtol ((char*)Command, (char**)&p, 10);
        }
        else {
            free (pAssignVar->szString);
            pAssignVar->szString = (unsigned char *) malloc (strlen (Command) + 5);
            if  (!pAssignVar->szString) {
                strcpy (Error, "Bad assign return val");
                z110_fatal ();
                return;
            }
            strcpy (pAssignVar->szString, Command);
        }
    }
    /**********************************/
    /* Release Local Variables        */
    /**********************************/
    if  (cLocalVars == 'y') {
        cLocalVars = 'n';
        pVariable = &StartVariable;
        do  {
            if  (pVariable->pNext) {
                pVariable = pVariable->pNext;
                pPrevVariable = pVariable->pPrev;
                pNextVariable = pVariable->pNext;
                if  (pVariable->LocalCallLevel > CallLevel) {
                    pPrevVariable->pNext = pNextVariable;
                    if  (pNextVariable) {
                        pNextVariable->pPrev = pPrevVariable;
                    }
                    if  (pVariable->szString) {
                        free (pVariable->szString);
                    }
                    if  (pVariable->szSaveString) {
                        free (pVariable->szSaveString);
                    }
                    if  (pVariable->szAction) {
                        free (pVariable->szAction);
                    }
                    free (pVariable);
                    pVariable = pPrevVariable;
                }
                if  (pVariable->LocalCallLevel > 0) {
                    cLocalVars = 'y';
                }
            }
        }
        while (pVariable->pNext);
    }
    pPrevCall = pCall->pPrev;
    pPrevCall->pNext = NULL;
    pCommand = pCall->pCommand;
    index1 = pCall->index1;
    More_Commands = pCall->More_Commands;
    If_Proc = pCall->If_Proc;
//    strcpy (input_line, pCommand->szLine);
    strcpy (input_line, pCall->input_line);
    if  (index1 > (int) strlen (input_line)) {
        More_Commands = 'n';
    }
//    EOF_Flag = pCall->EOF_Flag;
    free (pCall->input_line);
    free (pCall);
    pCall = pPrevCall;
    if  (!pCall->pPrev) {
        interactive = 'y';
    }
    strcpy (szCurrentFile, pCall->szFilename);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the INTEGER command                                  */
/********************************************************************/
void d500_integer () {
//    printf ("%s", "d500_integer\n");
    c120_parse_command ();
    g100_convert_variable ();
    FoundVar = 'n';
    d590_find_last_variable ();
    if  (FoundVar == 'y') {
        return;
    }
    pNewVariable = (VARIABLE *) malloc (sizeof (VARIABLE));
    if  (!pNewVariable) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    memset (pNewVariable, 0, sizeof (VARIABLE));
    strcpy (pNewVariable->szName, Command);

    pNewVariable->Integer = 0;
    pNewVariable->Float = 0.0;
    pNewVariable->Saved = 'n';
    pNewVariable->sSize = 0;
    pNewVariable->szString = (unsigned char *) malloc (5);
    if  (!pNewVariable->szString) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    strcpy (pNewVariable->szString, "");
    pNewVariable->szAction = (unsigned char *) malloc (5);
    if  (!pNewVariable->szAction) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    strcpy (pNewVariable->szAction, "");
    strcpy (pNewVariable->szEdit, "");
    pNewVariable->Type = 'I';
    pNewVariable->pNext = NULL;
    pNewVariable->pPrev = pVariable;
    pVariable->pNext = pNewVariable;
    /*************************/
    /* Set up Local Variable */
    /*************************/
    if  (cLocal == 'y') {
        pNewVariable->LocalCallLevel = CallLevel;
        cLocalVars = 'y';
        if  (cArg == 'y') {
            d525_local_args ();
        }
    }
    cLocal = 'n';
    cArg = 'n';
    pVariable = pNewVariable;
    if  (input_line [index1] == '=') {
        d600_variable ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the STRING command                                   */
/********************************************************************/
void d510_string () {
//    printf ("%s", "d510_string\n");
    c120_parse_command ();
    g100_convert_variable ();
    FoundVar = 'n';
    d590_find_last_variable ();
    if  (FoundVar == 'y') {
        return;
    }
    pNewVariable = (VARIABLE *) malloc (sizeof (VARIABLE));
    if  (!pNewVariable) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    memset (pNewVariable, 0, sizeof (VARIABLE));
    strcpy (pNewVariable->szName, Command);

    pNewVariable->Integer = 0;
    pNewVariable->Float = 0.0;
    pNewVariable->Saved = 'n';
    pNewVariable->sSize = 0;
    pNewVariable->szString = (unsigned char *) malloc (5);
    if  (!pNewVariable->szString) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    pNewVariable->szAction = (unsigned char *) malloc (5);
    if  (!pNewVariable->szAction) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    strcpy (pNewVariable->szString, "");
    strcpy (pNewVariable->szAction, "");
    strcpy (pNewVariable->szEdit, "");
    pNewVariable->Type = 'S';
    pNewVariable->pNext = NULL;
    pNewVariable->pPrev = pVariable;
    pVariable->pNext = pNewVariable;
    /*************************/
    /* Set up Local Variable */
    /*************************/
    if  (cLocal == 'y') {
        pNewVariable->LocalCallLevel = CallLevel;
        cLocalVars = 'y';
        if  (cArg == 'y') {
            d525_local_args ();
        }
    }
    cLocal = 'n';
    cArg = 'n';
    pVariable = pNewVariable;
    if  (input_line [index1] == '=') {
        d600_variable ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the DOLLAR command                                   */
/********************************************************************/
void d520_float () {
    unsigned char  szEditType [20];

    if  ((strcmp (Command, "DOLLAR") == 0) ||
         (strcmp (Command, "dollar") == 0)) {
        strcpy (szEditType, "DOLLAR");
    }
    else {
        strcpy (szEditType, "FLOAT");
    }

    c120_parse_command ();
    g100_convert_variable ();
    FoundVar = 'n';
    d590_find_last_variable ();
    if  (FoundVar == 'y') {
        return;
    }
    pNewVariable = (VARIABLE *) malloc (sizeof (VARIABLE));
    if  (!pNewVariable) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    memset (pNewVariable, 0, sizeof (VARIABLE));
    strcpy (pNewVariable->szName, Command);

    pNewVariable->Integer = 0;
    pNewVariable->Float = 0.0;
    pNewVariable->Saved = 'n';
    pNewVariable->sSize = 0;
    pNewVariable->szString = (unsigned char *) malloc (5);
    if  (!pNewVariable->szString) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    pNewVariable->szAction = (unsigned char *) malloc (5);
    if  (!pNewVariable->szAction) {
        strcpy (Error, "Bad create var");
        z110_fatal ();
        return;
    }
    strcpy (pNewVariable->szString, "");
    strcpy (pNewVariable->szAction, "");
    strcpy (pNewVariable->szEdit, szEditType);
    pNewVariable->Type = 'S';
    pNewVariable->pNext = NULL;
    pNewVariable->pPrev = pVariable;
    pVariable->pNext = pNewVariable;
    /*************************/
    /* Set up Local Variable */
    /*************************/
    if  (cLocal == 'y') {
        pNewVariable->LocalCallLevel = CallLevel;
        cLocalVars = 'y';
        if  (cArg == 'y') {
            d525_local_args ();
        }
    }
    cLocal = 'n';
    cArg = 'n';
    pVariable = pNewVariable;
    if  (input_line [index1] == '=') {
        d600_variable ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Assign the Arguments to the Local Variable                   */
/********************************************************************/
void d525_local_args () {
    if  (pArgs->pNext) {
        pArgs = pArgs->pNext;
        if  (pNewVariable->Type == 'I') {
            pNewVariable->Integer = (long int) strtol ((char*)pArgs->szArg, (char**)&p, 10);
        }
        else {
            if  (pNewVariable->szString) {
                free (pNewVariable->szString);
            }
            pNewVariable->szString = (unsigned char *) malloc (strlen (pArgs->szArg) + 5);
            if  (!pNewVariable->szString) {
                strcpy (Error, "Bad Arg Assign");
                z110_fatal ();
                return;
            }
            strcpy (pNewVariable->szString, pArgs->szArg);
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Erase Variables                                              */
/********************************************************************/
void d530_erase () {

    unsigned char  cUnsaved;

    strcpy (Command, "time");
    d580_find_variable ();
    c120_parse_command ();
    /******************************/
    /* Erase Interactive Commands */
    /******************************/
    if  ((strcmp (Command, "CMDS") == 0) ||
         (strcmp (Command, "cmds") == 0)) {
        pInteractive = &StartInteractive;
        if  (!pInteractive->pNext) {
            return;
        }
        pNewInteractive = pInteractive->pNext;
        pInteractive->pNext = NULL;
        pInteractive = pNewInteractive;
        do  {
            pNewInteractive = pInteractive->pNext;
            free (pInteractive->szCmd);
            free (pInteractive);
            pInteractive = pNewInteractive;
        }
        while (pInteractive->pNext);
        pInteractive = &StartInteractive;
        return;
    }
    if  (Command [0] == 0) {
        strcpy (Command, "*");
    }
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (Command, pVariable->szString);
    }
    cUnsaved = 'n';
    if  ((strcmp (Command, "UNSAVED") == 0) ||
         (strcmp (Command, "unsaved") == 0)) {
        cUnsaved = 'y';
    }
    do  {
        if  (pVariable->pNext) {
            pVariable = pVariable->pNext;
            pPrevVariable = pVariable->pPrev;
            pNextVariable = pVariable->pNext;
            if  ((strcmp (Command, "*") == 0) ||
                 (strcmp (Command, pVariable->szName) == 0) ||
                 (cUnsaved == 'y' && pVariable->Saved != 'y')) {
                pPrevVariable->pNext = pNextVariable;
                if  (pNextVariable) {
                    pNextVariable->pPrev = pPrevVariable;
                }
                if  (pVariable->szString) {
                    free (pVariable->szString);
                }
                if  (pVariable->szSaveString) {
                    free (pVariable->szSaveString);
                }
                if  (pVariable->szAction) {
                    free (pVariable->szAction);
                }
                free (pVariable);
                pVariable = pPrevVariable;
            }
        }
    }
    while (pVariable->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Initialize Variables                                         */
/********************************************************************/
void d540_initialize () {
    strcpy (Command, "time");
    d580_find_variable ();
    c120_parse_command ();
    if  (Command [0] == 0) {
        strcpy (Command, "*");
    }
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (Command, pVariable->szString);
    }
    do  {
        if  (pVariable->pNext) {
            pVariable = pVariable->pNext;
            pPrevVariable = pVariable->pPrev;
            pNextVariable = pVariable->pNext;
            if  ((strcmp (Command, "*") == 0) ||
                 (strcmp (Command, pVariable->szName) == 0)) {
                free (pVariable->szString);
                pVariable->szString = (unsigned char *) malloc (5);
                if  (!pVariable->szString) {
                    strcpy (Error, "Bad init Var");
                    z110_fatal ();
                    return;
                }
                strcpy (pVariable->szString, "");
                pVariable->Integer = 0;
            }
        }
    }
    while (pVariable->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     find a variable by name                                      */
/********************************************************************/
void d580_find_variable () {
    //unsigned char szDump [500];
    char cCurrent;

    //sprintf (szDump, "d580_find_variable: Command[%s]Recursion[%c]\n", Command, Recursion); printf(szDump);
    if  (Debug == 'y') {
        z999_open_bug ();
        fputs ("    Debug find variable [", file_debug_out);
        fputs (Command, file_debug_out);
        fputs ("]\n", file_debug_out);
    }
    /*********************************************/
    /* Recursion For @@Var                       */
    /*********************************************/
    if  ((Command [0] == Subs) &&
         (Command [1] == Subs)) {
        strcpy (Tvariable, Command + 1);
        strcpy (Command, Tvariable);
        Recursion = 'y';
    }
    /*********************************************/
    /* Recursion For @^Var                       */
    /*********************************************/
    if  ((Command [0] == Subs) &&
         (Command [1] == '^')) {
        strcpy (Tvariable, Command + 1);
        strcpy (Command, Tvariable);
        pVariableRecursive = NULL;
        Recursion = 'Y';
    }
    /*********************************************/
    /* Recursion For ^var                        */
    /*********************************************/
    if  (Command [0] == '^') {
        Command [0] = Subs;
        Recursion = 'Y';
        if  (Command [strlen (Command) - 1] == '^') {
            if  (strlen (Command) > 0) {
                Command [strlen (Command) - 1] = 0;
            }
        }
    }
    /*********************************************/
    /* Recursion using @var                      */
    /*********************************************/
    if  (Command [0] == Subs) {
        VARIABLE      *pLastVariable;
        if  ((Recursion == 'Y') && (pVariable)) {
            pVariableRecursive = pVariable;
        }
        if  (Command [0] == Subs) {
            strcpy (Tvariable, Command + 1); 
            strcpy (Command, Tvariable);
            if  (Command [strlen (Command) - 1] == Subs) {
                if  (strlen (Command) > 0) {
                    Command [strlen (Command) - 1] = 0;
                }
            }
        }
        d580_find_variable ();
        if ((found == 'y') && ((Recursion == 'y') || (Recursion == 'Y'))) {
            if  ((pVariable->szString [0] == Subs) &&
                 (pVariable->Type != 'S')) {
                strcpy (Error, "Must use str");
                z110_fatal ();
                strcpy (Command, "");
                Recursion = 'n';
                return;
            }
            if  (pVariable->szString [0] == Subs) {
                //printf ("Following chain\n");
                pLastVariable = pVariable;
                strcpy (Command, pVariable->szString);
                d580_find_variable ();
                if (!found) {
                   pVariable = pLastVariable;
                }
            }
            /*********************************************/
            /* Final Recursion For ^var                  */
            /*********************************************/
            //if  (Recursion == 'y') {       //FGB10072013
            //    d580_find_variable ();     //FGB10072013
            //    if (found != 'y') {  
            //       pVariable = pLastVariable;
            //    }
            //    //Recursion = 'n';           //FGB10072013
            //}                              //FGB10072013
            //return;
        }
        else {
            Recursion = 'n';
            return;
        }
        Recursion = 'n';
    }
    else {
        if  ((Recursion == 'Y') && (pVariableRecursive)) {
            pVariable = pVariableRecursive;
        }
    }

    found = 'n';
    cCurrent = 'n';
    /*********************************************/
    /* Check last used variable                  */
    /*********************************************/
    if  (cFast == 'y') {
        if  (pVariable) {
            if  (strcmp (pVariable->szName, Command) == 0) {
                cCurrent = 'y';
            }
        }
        if  (cCurrent != 'y') {
            //****************************************
            //* Check last var used on line
            //****************************************
            if  (pCommand) {
                if  (pCommand->pVar1) {
                    pVariable = pCommand->pVar1;
                    if  (strcmp (pVariable->szName, Command) == 0) {
                        cCurrent = 'y';
                    }
                }
                if  ((pCommand->pVar2) && (cCurrent != 'y')) {
                    pVariable = pCommand->pVar2;
                    if  (strcmp (pVariable->szName, Command) == 0) {
                        cCurrent = 'y';
                    }
                }
                if  ((pCommand->pVar3) && (cCurrent != 'y')) {
                    pVariable = pCommand->pVar3;
                    if  (strcmp (pVariable->szName, Command) == 0) {
                        cCurrent = 'y';
                    }
                }
            }
        }
    }
    /*********************************************/
    /* Find the variable (Top Down if not curr)  */
    /*********************************************/
    if  (cCurrent != 'y') {
        pVariable = &StartVariable;
    }
    do  {
        if  ((pVariable->pNext) || (cCurrent == 'y')) {
            if  (cCurrent != 'y') {
                pVariable = pVariable->pNext;
            }
            if  ((cCurrent == 'y') || (strcmp (pVariable->szName, Command) == 0)) {
                found = 'y';
                //*****************************************************
                //* Update the pCommand variable pointer for next time
                //*****************************************************
                if  (pCommand) {
                    if  ((!pCommand->pVar1) || (pCommand->pVar1 == pVariable)) {
                        pCommand->pVar1 = pVariable;
                    }
                    else {
                        if  ((!pCommand->pVar2) || (pCommand->pVar2 == pVariable)) {
                            pCommand->pVar2 = pVariable;
                        }
                        else {
                            if  ((!pCommand->pVar3) || (pCommand->pVar3 == pVariable)) {
                                pCommand->pVar3 = pVariable;
                            }
                        }
                    }
                }
                if  (Debug == 'y') {
                    if  (pVariable->Type == 'S') {
                        sprintf (Debugbuff, "     string = [%s]\n", pVariable->szString);
                    }
                    else {
                        sprintf (Debugbuff, "     integer= [%d]\n", pVariable->Integer);
                    }
                    z999_open_bug ();
                    fputs (Debugbuff, file_debug_out);
                }
                if  (strcmp (Command, "date") == 0) {
                    free (pVariable->szString);
                    pVariable->szString = (char *) malloc (strlen (date_str) + 5);
                    if  (!pVariable->szString) {
                        strcpy (Error, "Bad update date");
                        z110_fatal ();
                        return;
                    }
                    _strdate (date_str);
                    strcpy (pVariable->szString, date_str);
                }
                if  (strcmp (Command, "time") == 0) {
                    free (pVariable->szString);
                    pVariable->szString = (char *) malloc (strlen (time_str) + 5);
                    if  (!pVariable->szString) {
                        strcpy (Error, "Bad update time");
                        z110_fatal ();
                        return;
                    }
                    _strtime (time_str);
                    strcpy (pVariable->szString, time_str);
                }
                if  (strcmp (Command, "lineno") == 0) {
                    LineNumber = 0;
                    if  (pDataFile) {
                        pCountDataFile = pDataFile;
                        do  {
                            if  (pCountDataFile->pPrev) {
                                LineNumber++;
                                pCountDataFile = pCountDataFile->pPrev;
                            }
                        }
                        while (pCountDataFile->pPrev);
                    }
                    pVariable->Integer = LineNumber;
                }

                if  (strcmp (Command, "line") == 0) {
                    if  (pDataFile) {
                        free (pVariable->szString);
                        pVariable->szString = (char *) malloc (strlen (pDataFile->szLine) + 5);
                        if  (!pVariable->szString) {
                            strcpy (Error, "Bad update time");
                            z110_fatal ();
                            return;
                        }
                        strcpy (pVariable->szString, pDataFile->szLine);
                        if  (strlen (pVariable->szString) > 0) {
                            pVariable->szString [strlen (pVariable->szString) - 1] = 0;
                        }
                    }
                    else {
                        strcpy (Error, "No Curr File");
                        z110_fatal ();
                        return;
                    }
                }
                if  (strcmp (Command, "lline") == 0) {
                    if  (pDataFile) {
                        pHoldVariable = pVariable;
                        strcpy (Command, "line");
                        d580_find_variable ();
                        pHoldVariable->Integer = (int) strlen (pVariable->szString);
                        pVariable = pHoldVariable;
                    }
                    else {
                        strcpy (Error, "No Curr File");
                        z110_fatal ();
                        return;
                    }
                }
                if  (strcmp (Command, "lleft") == 0) {
                    pHoldVariable = pVariable;
                    strcpy (Command, "left");
                    d580_find_variable ();
                    pHoldVariable->Integer = (int) strlen (pVariable->szString);
                    pVariable = pHoldVariable;
                }
                if  (strcmp (Command, "lright") == 0) {
                    pHoldVariable = pVariable;
                    strcpy (Command, "right");
                    d580_find_variable ();
                    pHoldVariable->Integer = (int) strlen (pVariable->szString);
                    pVariable = pHoldVariable;
                }
                if  (strcmp (Command, "lmid") == 0) {
                    pHoldVariable = pVariable;
                    strcpy (Command, "mid");
                    d580_find_variable ();
                    pHoldVariable->Integer = (int) strlen (pVariable->szString);
                    pVariable = pHoldVariable;
                }
                if  (strcmp (Command, "lmidright") == 0) {
                    pHoldVariable = pVariable;
                    strcpy (Command, "midright");
                    d580_find_variable ();
                    pHoldVariable->Integer = (int) strlen (pVariable->szString);
                    pVariable = pHoldVariable;
                }
                if  (strcmp (Command, "llword") == 0) {
                    pHoldVariable = pVariable;
                    strcpy (Command, "lword");
                    d580_find_variable ();
                    pHoldVariable->Integer = (int) strlen (pVariable->szString);
                    pVariable = pHoldVariable;
                }
                if  (strcmp (Command, "lrword") == 0) {
                    pHoldVariable = pVariable;
                    strcpy (Command, "rword");
                    d580_find_variable ();
                    pHoldVariable->Integer = (int) strlen (pVariable->szString);
                    pVariable = pHoldVariable;
                }
                if  (strcmp (Command, "pcount") == 0) {
                    pVariable->Integer = pcount;
                }
                if  (strcmp (Command, "calllevel") == 0) {
                    pVariable->Integer = dCallFileLevel;
                }
                if  (trace == 'x') {
                    z999_open_bug ();
                    sprintf (buff, "                                  %s=[%s]\n", Command, pVariable->szString);
                    fputs (buff, file_debug_out);
                }
                return;
            }
        }
    }
    while (pVariable->pNext);
    if  (test == 'y') {
        test = 'n';
        return;
    }
    //while (pVariable->pNext);
    if  ((Recursion != 'y') && (Recursion != 'Y')) {
        strcpy (Error, "Variable Not Defined or Type Conflict");
        z110_fatal ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     find the last variable in the list                           */
/********************************************************************/
void d590_find_last_variable () {
//    printf ("%s", "d590_find_last_variable\n");
    pVariable = &StartVariable;
    FoundVar = 'n';
    do  {
        if  (pVariable->pNext) {
            pVariable = pVariable->pNext;
            if  (strcmp (pVariable->szName, Command) == 0) {
                strcpy (Error, "Dup Var Def");
                FoundVar = 'y';
                z110_fatal ();
//                return;
            }
        }
    }
    while (pVariable->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Dump all Open Files                                          */
/********************************************************************/
void d598_files () {
    printf ("%s", "*** OPEN FILES ***\n");
    if  ((Debug == 'y') || (Beditwin == 'y')) {
        z999_open_bug ();
        fputs ("*** OPEN FILES ***\n", file_debug_out);
    }
    pNewOpenFile = &StartOpenFile;
    do  {
        if  (pNewOpenFile->pNext) {
            pNewOpenFile = pNewOpenFile->pNext;
            if  (pNewOpenFile == pOpenFile) {
                sprintf (buff, "Curr-> File %s [Filestring=%s] EOF=[%c]\n",
                                               pNewOpenFile->szFileName,
                                               pNewOpenFile->szFile,
                                               EOF_Flag);
            }
            else {
                sprintf (buff, "       File %s [Filestring=%s]\n",
                                               pNewOpenFile->szFileName,
                                               pNewOpenFile->szFile);
            }
            if  ((Debug == 'y') || (Beditwin == 'y')) {
                z999_open_bug ();
                fputs (buff, file_debug_out);
            }
            printf ("%s", buff);
        }
    }
    while (pNewOpenFile->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Dump all variable and contents to the debug file             */
/********************************************************************/
void d599_dump () {
    unsigned char szVarName [500];
    int iCustomVars;
    int iMaxNameSize = 0;
//    printf ("%s", "d599_dump\n");

    //*******************************
    //* Find longest field name
    //*******************************
    pVariable = &StartVariable;
    do  {
        if  (pVariable->pNext) {
            pVariable = pVariable->pNext;
            if ((int)strlen(pVariable->szName) > iMaxNameSize) {
               iMaxNameSize = (int)strlen(pVariable->szName);
            }
        }
    }
    while (pVariable->pNext);
    //sprintf (buff, "%s%d%s", "Max Field Name Size=[", iMaxNameSize, "]\n");
    //printf ("%s", buff);

    strcpy (szVarName, "                                                                ");
    szVarName [9+iMaxNameSize] = 0;
    sprintf (buff, "%s%s", szVarName, "         1         2         3         4         5\n");
    printf ("%s", buff);
    sprintf (buff, "%s%s", szVarName, "....5....0....5....0....5....0....5....0....5....0\n");
    printf ("%s", buff);

    c120_parse_command ();
    if  (Command [0] == 0) {
        strcpy (Command, "*");
    }
    /****************************/
    /* Refresh Variables        */
    /****************************/
    strcpy (szIf1, Command); /* Temporary Storage */
    strcpy (Command, "date");
    d580_find_variable ();
    strcpy (Command, "time");
    d580_find_variable ();
    strcpy (Command, "lleft");
    d580_find_variable ();
    strcpy (Command, "lmid");
    d580_find_variable ();
    strcpy (Command, "lright");
    d580_find_variable ();
    strcpy (Command, "lmidright");
    d580_find_variable ();
    strcpy (Command, "llword");
    d580_find_variable ();
    strcpy (Command, "lrword");
    d580_find_variable ();
    strcpy (Command, "pcount");
    d580_find_variable ();
    if  (pDataFile) {
        strcpy (Command, "lline");
        d580_find_variable ();
        strcpy (Command, "lineno");
        d580_find_variable ();
        strcpy (Command, "line");
        d580_find_variable ();
    }
    strcpy (Command, szIf1);
    pVariable = &StartVariable;
    iCustomVars = 0;
    if  ((strcmp (Command, "*") == 0) ||
         (strcmp (Command, "SYSTEM") == 0) ||
         (strcmp (Command, "system") == 0)) {
        printf ("========================== System Variables ==========================\n");
    }
    do  {
        if  (pVariable->pNext) {
            pVariable = pVariable->pNext;
            if  ((strcmp (Command, "*") == 0) ||
                 ((strcmp (Command, "SYSTEM") == 0) && (iCustomVars == 0)) ||
                 ((strcmp (Command, "system") == 0) && (iCustomVars == 0)) ||
                 ((strcmp (Command, "CUSTOM") == 0) && (iCustomVars != 0)) ||
                 ((strcmp (Command, "custom") == 0) && (iCustomVars != 0)) ||
                 (strcmp (pVariable->szName, Command) == 0)) {
                sprintf(szVarName, "%s%s", pVariable->szName, "................................................................................");
                szVarName[iMaxNameSize] = 0;
                if  (iCustomVars == 1) {
                    iCustomVars = 2;
                    printf ("========================== Custom Variables ==========================\n");
                }
                if  (pVariable->Type == 'S') {
                    if  (strcmp (pVariable->szName, "encryptkey") == 0) {
                         sprintf (buff, "STRING  %s[%s]\n", szVarName, szEncryptKey);
                    }
                    else {
                        if (strcmp (pVariable->szEdit, "") == 0) {
                            sprintf (buff, "STRING  %s[%s]\n", szVarName, pVariable->szString);
                        }
                        else {
                            if  (strcmp (pVariable->szEdit, "FLOAT") == 0) {
                                sprintf (buff, "%s   %s[%s]\n", pVariable->szEdit, szVarName, pVariable->szString);
                            }
                            else { 
                                sprintf (buff, "%s  %s[%s]\n", pVariable->szEdit, szVarName, pVariable->szString);
                            }
                        }
                    }
                }
                else {
                    sprintf (buff, "INTEGER %s[%d]\n", szVarName, pVariable->Integer);
                }                
                if  ((Debug == 'y') || (Beditwin == 'y')) {
                    if  (strcmp (pVariable->szName, "encryptkey") != 0) {
                        z999_open_bug ();
                        fputs (buff, file_debug_out);
                    }
                }
                if  (strcmp (pVariable->szName, "encryptkey") != 0) {
                    printf ("%s", buff);
                }
            }
            if  (strcmp(pVariable->szName, "time") == 0) {
                iCustomVars = 1;
                if  ((strcmp (Command, "SYSTEM") == 0) ||
                     (strcmp (Command, "system") == 0)) {
                    break;
                }
            }
        }
    }
    while (pVariable->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     A variable assignment line                                   */
/********************************************************************/
void d600_variable () {
//    printf ("%s", "d600_variable\n");
    /********************************/
    /* Format of assignment:        */
    /*                              */
    /* Var = Var + Var              */
    /* Var = "   " + Var            */
    /* Var = Var + "   "            */
    /* Var = Var                    */
    /* Var = "   "                  */
    /********************************/
    pAssignVar = pVariable;
    c120_parse_command ();
    //z999_open_bug ();
    //sprintf (buff, "Variable Assignment [%s]\n", pVariable->szName);
    //fputs (buff, file_debug_out);
    /**********************/
    /* Var ++             */
    /**********************/
    if  (pVariable->Type == 'I') {
        if  (strcmp (Command, "++") == 0) {
            pVariable->Integer++;
            return;
        }
    }
    if  ((strcmp (pVariable->szEdit, "DOLLAR") == 0) ||   //fgb 11-8-2013
         (strcmp (pVariable->szEdit, "FLOAT") == 0)) {
        if  (strcmp (Command, "++") == 0) {
            char szDollar [50];
            double dDollar;
            unsigned char   *cpDollar;

            dDollar = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
            dDollar++;
            if  (strcmp (pVariable->szEdit, "DOLLAR") == 0) {
                sprintf (szDollar, "%.02f", dDollar);
            }
            else {
                sprintf (szDollar, "%f", dDollar);
            }
            free (pVariable->szString);
            pVariable->szString = (unsigned char *) malloc (strlen (szDollar) + 5);
            if  (!pVariable->szString) {
                strcpy (Error, "Bad assign");
                z110_fatal ();
                return;
            }
            strcpy (pVariable->szString, szDollar);
            return;
        }
    }
    /**********************/
    /* Var --             */
    /**********************/
    if  (pVariable->Type == 'I') {
        if  (strcmp (Command, "--") == 0) {
            pVariable->Integer--;
            return;
        }
    }
    if  ((strcmp (pVariable->szEdit, "DOLLAR") == 0) ||   //fgb 11-8-2013
         (strcmp (pVariable->szEdit, "FLOAT") == 0)) {
        if  (strcmp (Command, "--") == 0) {
            char szDollar [50];
            double dDollar;
            unsigned char   *cpDollar;

            dDollar = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
            dDollar--;
            if  (strcmp (pVariable->szEdit, "DOLLAR") == 0) {
                sprintf (szDollar, "%.02f", dDollar);
            }
            else {
                sprintf (szDollar, "%f", dDollar);
            }
            free (pVariable->szString);
            pVariable->szString = (unsigned char *) malloc (strlen (szDollar) + 5);
            if  (!pVariable->szString) {
                strcpy (Error, "Bad assign");
                z110_fatal ();
                return;
            }
            strcpy (pVariable->szString, szDollar);
            return;
        }
    }

    //*********************/
    //* VarA += VarB      */
    //*********************/
    if  ((strcmp (Command, "+=") == 0) ||
         (strcmp (Command, "-=") == 0) ||
         (strcmp (Command, "*=") == 0) ||
         (strcmp (Command, "/=") == 0)) {
         char szMath [3];
         strcpy (szMath, Command);
         c120_parse_command ();
         if  (pAssignVar->Type == 'I') {
             int Number;
             if  (Command [0] < 64) {
                 Number = (long int)strtol ((char*)Command, (char**)&p, 10);             
             }
             else {
                 d580_find_variable ();
                 if  (pVariable->Type == 'I') {
                     Number = pVariable->Integer;
                 }
                 else {
                     if  ((strcmp (pVariable->szEdit, "DOLLAR") == 0) || 
                          (strcmp (pVariable->szEdit, "FLOAT") == 0)) {
                          double dDollar;
                          unsigned char   *cpDollar;

                          dDollar = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
                          Number = dDollar;
                     }
                     else {
                          double dDollar;
                          unsigned char   *cpDollar;

                          dDollar = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
                          Number = dDollar;                          
                     }
                 }
             }
             if  (strcmp(szMath, "+=") == 0) {
                 pAssignVar->Integer += Number;
             }
             if  (strcmp(szMath, "-=") == 0) {
                 pAssignVar->Integer -= Number;
             }
             if  (strcmp(szMath, "*=") == 0) {
                 pAssignVar->Integer *= Number;
             }
             if  (strcmp(szMath, "/=") == 0) {
                 if  (Number != 0) {
                     pAssignVar->Integer /= Number;
                 }
                 else {
                    strcpy (Error, "Div by 0");
                    z110_fatal ();
                    return;
                 }
             }
             return;
         }
         else {
             if  ((strcmp (pAssignVar->szEdit, "DOLLAR") == 0) || 
                  (strcmp (pAssignVar->szEdit, "FLOAT") == 0)) {
                 double dAssignDollar;
                 double dSecondDollar;
                 char szDollar [50];
                 unsigned char   *cpDollar;

                 dAssignDollar = (double)strtod ((char*)pAssignVar->szString, (char**)&cpDollar);
                 if  (Command [0] < 64) {
                     Number = (long int)strtol ((char*)Command, (char**)&p, 10);
                     dSecondDollar = Number;             
                 }
                 else {
                     d580_find_variable ();
                     if  (pVariable->Type == 'I') {
                         Number = pVariable->Integer;
                         dSecondDollar = Number;
                     }
                     else {
                         if  ((strcmp (pVariable->szEdit, "DOLLAR") == 0) || 
                              (strcmp (pVariable->szEdit, "FLOAT") == 0)) {
                              double dDollar;
                              unsigned char   *cpDollar;
    
                              dDollar = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
                              dSecondDollar = dDollar;
                         }
                         else {
                              double dDollar;
                              unsigned char   *cpDollar;

                              dDollar = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
                              dSecondDollar = dDollar;                          
                         }
                     }
                 }    
                 if  (strcmp(szMath, "+=") == 0) {
                     dAssignDollar += dSecondDollar;
                 }
                 if  (strcmp(szMath, "-=") == 0) {
                     dAssignDollar -= dSecondDollar;
                 }
                 if  (strcmp(szMath, "*=") == 0) {
                     dAssignDollar *= dSecondDollar;
                 }
                 if  (strcmp(szMath, "/=") == 0) {
                     if  (dSecondDollar != 0) {
                         dAssignDollar /= dSecondDollar;
                     }
                     else {
                        strcpy (Error, "Div by 0");
                        z110_fatal ();
                        return;
                     }
                 }
                 if  (strcmp (pAssignVar->szEdit, "DOLLAR") == 0) {
                     sprintf (szDollar, "%.02f", dAssignDollar);
                 }
                 else {
                     sprintf (szDollar, "%f", dAssignDollar);
                 }
                 free (pAssignVar->szString);
                 pAssignVar->szString = (unsigned char *) malloc (strlen (szDollar) + 5);
                 if  (!pVariable->szString) {
                     strcpy (Error, "Bad assign");
                     z110_fatal ();
                     return;
                 }
                 strcpy (pAssignVar->szString, szDollar);
                 return;
             }
         }
    }

    /**********************/
    /* Var = ...          */
    /**********************/
    if  (strcmp (Command, "=") != 0) {
        z010_syntax_error ();
        return;
    }
    /**********************/
    /* Var = CALL ...     */
    /**********************/
    CallVar = 'n';
    if  ((memcmp (input_line + index1, "CALL ", 5) == 0) ||
         (memcmp (input_line + index1, "call ", 5) == 0)) {
        CallVar = 'y';
        pCountVariable = pVariable;
        return;
    }
    if  (pVariable->Type == 'I') {
        d610_variable_integer ();
        return;
    }
    if  ((strcmp (pVariable->szEdit, "DOLLAR") == 0) ||
         (strcmp (pVariable->szEdit, "FLOAT") == 0)) {
        d620_variable_dollar ();
        return;
    }
    /******************************************/
    /* String addition                        */
    /******************************************/
    /************************/
    /* Var1                 */
    /************************/
    found1 = 'y';
    if  ((input_line [index1] < 48) ||
         ((input_line [index1] > 57) && (input_line [index1] < 64)) ||
         ((input_line [index1] > 90) && (input_line [index1] < 97)) ||
         (input_line [index1] > 122)) {
        if  (input_line [index1] != Subs) {
            Delimit = 'y';
            found1 = 'n';
        }
    }
    c120_parse_command ();
    if  (found1 == 'y') {
        d580_find_variable ();
        pVar1 = pVariable;
    }
    else {
        strcpy (Var1, Command + 1);
        Var1 [strlen (Var1) - 1] = 0;
        strcpy (szConvString, Var1);
        g200_convert_string ();
        strcpy (Var1, szConvString);
    }
    /************************/
    /* Var2                 */
    /************************/
    if  (input_line [index1] == '+') {
        c120_parse_command ();
        if  (strcmp (Command, "+") != 0) {
            z010_syntax_error ();
            return;
        }
        found2 = 'y';
        if  ((input_line [index1] < 48) ||
             ((input_line [index1] > 57) && (input_line [index1] < 64)) ||
             ((input_line [index1] > 90) && (input_line [index1] < 97)) ||
             (input_line [index1] > 122)) {
            if  (input_line [index1] != Subs) {
                Delimit = 'y';
                found2 = 'n';
            }
        }
        c120_parse_command ();
        if  (found2 == 'y') {
            d580_find_variable ();
            pVar2 = pVariable;
        }
        else {
            strcpy (Var2, Command + 1);
            Var2 [strlen (Var2) - 1] = 0;
            strcpy (szConvString, Var2);
            g200_convert_string ();
            strcpy (Var2, szConvString);
        }
    }
    else {
        found2 = 'x';
    }

    /********************************/
    /* Build the New Var Data       */
    /********************************/
    buff [0] = 0;
    if  (found1 == 'y') {
        if  (pVar1->Type == 'S') {
            strcpy (buff, pVar1->szString);
        }
        if  (pVar1->Type == 'I') {
            sprintf (buff, "%d", pVar1->Integer);
        }
    }
    else {
        strcpy (buff, Var1);
    }
    /********************************/
    /* Add the Second Part          */
    /********************************/
    if  (found2 != 'x') {
        if  (found2 == 'y') {
            if  (pVar2->Type == 'S') {
                strcat (buff, pVar2->szString);
            }
            if  (pVar2->Type == 'I') {
                sprintf (temp, "%d", pVar2->Integer);
                strcat (buff, temp);
            }
        }
        else {
            strcat (buff, Var2);
        }
    }
    /********************************/
    /* Move the Data to the Var     */
    /********************************/
    strcpy (Command, pAssignVar->szName);
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (buff) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad assign");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, buff);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     An integer variable assignment                               */
/********************************************************************/
void d610_variable_integer () {
//    printf ("%s", "d6100_variable_integer\n");
    /********************************/
    /* Format of assignment:        */
    /*                              */
    /*    (+, -, *, /)              */
    /*                              */
    /* Var = Var + Var              */
    /* Var = # + Var                */
    /* Var = Var + #                */
    /* Var = Var                    */
    /* Var = #                      */
    /********************************/
    /******************************************/
    /* String addition                        */
    /******************************************/
    /************************/
    /* Var1                 */
    /************************/
    c120_parse_command ();
    g100_convert_variable ();
    len = 'n';
    strcpy (buff, Command);
    buff [4] = 0;
    if  ((strcmp (buff, "LEN(") == 0) ||
         (strcmp (buff, "len(") == 0)) {
        strcpy (buff, Command + 4);
        strcpy (Command, buff);
        Command [strlen (Command) - 1] = 0;
        len = 'y';
    }
    if  (Command [0] < 64) {
        Number = 0;
        Number = (long int)strtol ((char*)Command, (char**)&p, 10);
    }
    else {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            if  (len == 'y') {
                Number = strlen (pVariable->szString);
            }
            else {
                Number = 0;
                Number = (long int) strtol ((char*)pVariable->szString, (char**)&p, 10);
            }
        }
        else {
            if  (len == 'y') {
                sprintf (buff, "%d", pVariable->Integer);
                Number = strlen (buff);
            }
            else {
                Number = pVariable->Integer;
            }
        }
    }
    Num1 = (int) Number;
    /************************/
    /* Var2                 */
    /************************/
    if  ((input_line [index1] == '+') ||
         (input_line [index1] == '-') ||
         (input_line [index1] == '*') ||
         (input_line [index1] == '/')) {
        c120_parse_command ();
        strcpy (Math, Command);
        c120_parse_command ();
        g100_convert_variable ();
        if  (Command [0] < 64) {
            Number = 0;
            Number = (long int)strtol ((char*)Command, (char**)&p, 10);
        }
        else {
            d580_find_variable ();
            if  (pVariable->Type == 'S') {
                Number = 0;
                Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
            }
            else {
                Number = pVariable->Integer;
            }
        }
        Num2 = (int) Number;
    }
    else {
        Num2 = 0;
        strcpy (Math, "+");
    }

    /********************************/
    /* Move the Data to the Var     */
    /********************************/
    strcpy (Command, pAssignVar->szName);
    d580_find_variable ();
    if  (strcmp (Math, "+") == 0) {
        pVariable->Integer = Num1 + Num2;
    }
    if  (strcmp (Math, "-") == 0) {
        pVariable->Integer = Num1 - Num2;
    }
    if  (strcmp (Math, "*") == 0) {
        pVariable->Integer = Num1 * Num2;
    }
    if  (strcmp (Math, "/") == 0) {
        if  (Num2 != 0) {
            pVariable->Integer = Num1 / Num2;
        }
        else {
            strcpy (Error, "Div by 0");
            z110_fatal ();
            return;
        }
    }
    /********************************/
    /* More Data to Add?            */
    /********************************/
    if  ((input_line [index1] == '+') ||
         (input_line [index1] == '-') ||
         (input_line [index1] == '*') ||
         (input_line [index1] == '/')) {
        do  {
            c120_parse_command ();
            strcpy (Math, Command);
            c120_parse_command ();
            g100_convert_variable ();
            if  (Command [0] < 64) {
                Number = 0;
                Number = (long int)strtol ((char*)Command, (char**)&p, 10);
            }
            else {
                d580_find_variable ();
                if  (pVariable->Type == 'S') {
                    Number = 0;
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
                else {
                    Number = pVariable->Integer;
                }
            }
            Num2 = (int) Number;
            /********************************/
            /* Add the Data to the Var      */
            /********************************/
            strcpy (Command, pAssignVar->szName);
            d580_find_variable ();
            if  (strcmp (Math, "+") == 0) {
                pVariable->Integer = pVariable->Integer + Num2;
            }
            if  (strcmp (Math, "-") == 0) {
                pVariable->Integer = pVariable->Integer - Num2;
            }
            if  (strcmp (Math, "*") == 0) {
                pVariable->Integer = pVariable->Integer * Num2;
            }
            if  (strcmp (Math, "/") == 0) {
                if  (Num2 != 0) {
                    pVariable->Integer = pVariable->Integer / Num2;
                }
                else {
                    strcpy (Error, "Div by 0");
                    z110_fatal ();
                    return;
                }
            }
        }
        while ((input_line [index1] == '+') ||
               (input_line [index1] == '-') ||
               (input_line [index1] == '*') ||
               (input_line [index1] == '/'));
    }
    
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     A DOLLAR variable assignment                                 */
/********************************************************************/
void d620_variable_dollar () {

    double Dollar1;
    double Dollar2;
    unsigned char   *cpDollar;
    unsigned char   szDollar     [100];

    int Trig;
    int Shift;

    /******************************************/
    /* Dollar addition                        */
    /******************************************/
    /************************/
    /* Var1                 */
    /************************/
    c120_parse_command ();
    g100_convert_variable ();

    strcpy (buff, Command);

    buff [5] = 0;
    Trig = 0;

    if  ((strcmp (buff, "SQRT(") == 0) ||
         (strcmp (buff, "sqrt(") == 0)) {
        Trig = 7;
    }
    else {
        if  ((strcmp (buff, "ASIN(") == 0) ||
             (strcmp (buff, "asin(") == 0)) {
            Trig = 4;
        }
        else {
            if  ((strcmp (buff, "ACOS(") == 0) ||
                 (strcmp (buff, "acos(") == 0)) {
                Trig = 5;
            }
            else {
                if  ((strcmp (buff, "ATAN(") == 0) ||
                     (strcmp (buff, "atan(") == 0)) {
                    Trig = 6;
                }
            }
        }
    }
    if  (Trig == 0) {
        buff [4] = 0;
        if  ((strcmp (buff, "SIN(") == 0) ||
             (strcmp (buff, "sin(") == 0)) {
            Trig = 1;
       }
        else {
           if  ((strcmp (buff, "COS(") == 0) ||
                 (strcmp (buff, "cos(") == 0)) {
                Trig = 2;
            }
           else {
                if  ((strcmp (buff, "TAN(") == 0) ||
                     (strcmp (buff, "tan(") == 0)) {
                    Trig = 3;
                }
            }
        }
    }

    if  (Trig) {
        if  (Trig < 4) {
            Shift = 4;
        }
        else {
            Shift = 5;
        }
        strcpy (buff, Command + Shift);
        strcpy (Command, buff);
        Command [strlen (Command) - 1] = 0;
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            //Dollar1 = 0;
            Dollar1 = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
        }
        else {
            Dollar1 = (double) pVariable->Integer;
        }
        if  (Trig == 1) {
            Dollar1 = sin (Dollar1 * PI / 180);
        }
        if  (Trig == 2) {
            Dollar1 = cos (Dollar1 * PI / 180);
        }
        if  (Trig == 3) {
            Dollar1 = tan (Dollar1 * PI / 180);
        }
        if  (Trig == 4) {
            Dollar1 = asin (Dollar1) * 180 / PI;
        }
        if  (Trig == 5) {
            Dollar1 = acos (Dollar1) * 180 / PI;
        }
        if  (Trig == 6) {
            Dollar1 = atan (Dollar1) * 180 / PI;
        }
        if  (Trig == 7) {
            Dollar1 = sqrt (Dollar1);
        }
    }
    else {
        if  (Command [0] < 64) {
            //Dollar1 = 0;
            Dollar1 = (double)strtod ((char*)Command, (char**)&cpDollar);
        }
        else {
            d580_find_variable ();
            if  (pVariable->Type == 'S') {
                //Dollar1 = 0;
                Dollar1 = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
            }
            else {
                Dollar1 = (double) pVariable->Integer;
            }
        }
    }
    /************************/
    /* Var2                 */
    /************************/
    if  ((input_line [index1] == '+') ||
         (input_line [index1] == '-') ||
         (input_line [index1] == '*') ||
         (input_line [index1] == '/')) {
        c120_parse_command ();
        strcpy (Math, Command);
        c120_parse_command ();
        g100_convert_variable ();
        if  (Command [0] < 64) {
            //Dollar2 = 0;
            Dollar2 = (double) strtod ((char*)Command, (char**)&cpDollar);
        }
        else {
            d580_find_variable ();
            if  (pVariable->Type == 'S') {
                //Dollar2 = 0;
                Dollar2 = (double)strtod ((char*)pVariable->szString, (char**)&cpDollar);
            }
            else {
                Dollar2 = (double) pVariable->Integer;
            }
        }
    }
    else {
        Dollar2 = 0;
        strcpy (Math, "+");
    }

    /********************************/
    /* Move the Data to the Var     */
    /********************************/
    strcpy (Command, pAssignVar->szName);
    d580_find_variable ();
    if  (strcmp (pVariable->szEdit, "DOLLAR") == 0) {
        if  (strcmp (Math, "+") == 0) {
            sprintf (szDollar, "%.02f", Dollar1 + Dollar2);
        }
        if  (strcmp (Math, "-") == 0) {
            sprintf (szDollar, "%.02f", Dollar1 - Dollar2);
        }
        if  (strcmp (Math, "*") == 0) {
            sprintf (szDollar, "%.02f", Dollar1 * Dollar2);
        }
        if  (strcmp (Math, "/") == 0) {
            if  (Dollar2 != 0) {
                sprintf (szDollar, "%.02f", Dollar1 / Dollar2);
            }
            else {
                strcpy (Error, "Div by 0");
                z110_fatal ();
                return;
            }
        }
    }
    else {
        if  (strcmp (Math, "+") == 0) {
            sprintf (szDollar, "%f", Dollar1 + Dollar2);
        }
        if  (strcmp (Math, "-") == 0) {
            sprintf (szDollar, "%f", Dollar1 - Dollar2);
        }
        if  (strcmp (Math, "*") == 0) {
            sprintf (szDollar, "%f", Dollar1 * Dollar2);
        }
        if  (strcmp (Math, "/") == 0) {
            if  (Dollar2 != 0) {
                sprintf (szDollar, "%f", Dollar1 / Dollar2);
            }
            else {
                strcpy (Error, "Div by 0");
                z110_fatal ();
                return;
            }
        }
    }
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (szDollar) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad assign");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szDollar);
    /********************************/
    /* More Data to Add?            */
    /********************************/
    if  ((input_line [index1] == '+') ||
         (input_line [index1] == '-') ||
         (input_line [index1] == '*') ||
         (input_line [index1] == '/')) {
        do  {
            c120_parse_command ();
            strcpy (Math, Command);
            c120_parse_command ();
            g100_convert_variable ();
            if  (Command [0] < 64) {
                //Dollar2 = 0;
                Dollar2 = (double)strtod ((char*)Command, (char**)&cpDollar);
            }
            else {
                d580_find_variable ();
                if  (pVariable->Type == 'S') {
                    //Dollar2 = 0;
                    Dollar2 = (double) strtod ((char*)pVariable->szString, (char**)&cpDollar);
                }
                else {
                    Dollar2 = (double) pVariable->Integer;
                }
            }
            /********************************/
            /* Add the Data to the Var      */
            /********************************/
            strcpy (Command, pAssignVar->szName);
            d580_find_variable ();
            //Dollar1 = 0;
            Dollar1 = (double)strtod ((char*)pAssignVar->szString, (char**)&cpDollar);
            if  (strcmp (Math, "+") == 0) {
                sprintf (szDollar, "%.02f", Dollar1 + Dollar2);
            }
            if  (strcmp (Math, "-") == 0) {
                sprintf (szDollar, "%.02f", Dollar1 + Dollar2);
            }
            if  (strcmp (Math, "*") == 0) {
                sprintf (szDollar, "%.02f", Dollar1 + Dollar2);
            }
            if  (strcmp (Math, "/") == 0) {
                if  (Dollar2 != 0) {
                    sprintf (szDollar, "%.02f", Dollar1 + Dollar2);
                }
                else {
                    strcpy (Error, "Div by 0");
                    z110_fatal ();
                    return;
                }
            }
            free (pVariable->szString);
            pVariable->szString = (unsigned char *) malloc (strlen (szDollar) + 5);
            if  (!pVariable->szString) {
                strcpy (Error, "Bad assign");
                z110_fatal ();
                return;
            }
            strcpy (pVariable->szString, szDollar);
        }
        while ((input_line [index1] == '+') ||
               (input_line [index1] == '-') ||
               (input_line [index1] == '*') ||
               (input_line [index1] == '/'));
    }
    
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process the INPUT command.                                   */
/********************************************************************/
void d700_input (char cSingleChar) {
    c120_parse_command ();
    InputNumber = 999;
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound == 'y') {
        if  (szScanRight [0] == Subs) {
            strcpy (Command, szScanRight);
            d580_find_variable ();
            if  (pVariable->Type == 'I') {
                InputNumber = (long) pVariable->Integer;
            }
            else {
                InputNumber = (long int) strtol ((char*)pVariable->szString, (char**)&p, 10);
            }
        }
        else {
            InputNumber = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
        }
        strcpy (Command, szScanLeft);
    }
    g100_convert_variable ();
    d580_find_variable ();
    if  (cSingleChar != 'c') {
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
    }
    y500_input_string (cSingleChar);
    if  (pVariable->Type == 'S') {
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (strlen (temp) + 5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad input var");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, temp);
    }
    else {
        Number = (long int)strtol ((char*)temp, (char**)&p, 10);
        pVariable->Integer = (int) Number;
    }
    /*******************************/
    /* Update functionkey          */
    /*******************************/
    strcpy (Command, "functionkey");
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (szFunctionkey) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad store functionkey");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szFunctionkey);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process the SLEEP command.                                   */
/********************************************************************/
void d710_sleep () {
      c120_parse_command ();
      if  (Command [0] == Subs) {
          d580_find_variable ();
          if  (pVariable->Type != 'I') {
              strcpy (Error, "Must use Int");
              z110_fatal ();
              return;
          }
          Number = pVariable->Integer;
      }
      else {
          Number = (long int) strtol ((char*)Command, (char**)&p, 10);
      }
      if  (Number < 1) {
          strcpy (Error, "Bad Cnt");
          z110_fatal ();
          return;
      }
      //ulSleep = (unsigned long) (Number * 1000);
      ulSleep = (unsigned long) (Number);
      Sleep ((DWORD)ulSleep);
      return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process the WAITBELL command.                                */
/********************************************************************/
void d715_waitbell () {
    if  (Beditwin == 'y') {
        strcpy (Error, "Unavail in Beditwin");
        z110_fatal ();
        return;
    }

    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type != 'I') {
            strcpy (Error, "Must use Int");
            z110_fatal ();
            return;
        }
        Number = pVariable->Integer;
    }
    else {
        Number = (long int) strtol ((char*)Command, (char**)&p, 10);
    }
    if  (Number < 1) {
        strcpy (Error, "Bad Cnt");
        z110_fatal ();
        return;
    }

    do  {
        printf ("%s", "Press Any Key To Continue...\n");
        printf ("%c", bell);
        ulSleep = (unsigned long) (Number * 1000);
        Sleep ((DWORD)ulSleep);
    }
    while (!kbhit ());
    response = (unsigned char) getch ();
    printf ("\n");
}

/********************************************************************/
/* This module will :                                               */
/*     Process the CHAIN command.                                   */
/********************************************************************/
void d720_chain () {
    c120_parse_command ();
    sprintf (buff, "---- Start of Chain ----\n");
    printf ("%s", buff);
    if  ((Debug == 'y') || (Beditwin == 'y')) {
        z999_open_bug ();
        fputs (buff, file_debug_out);
    }
    do  {
        d580_find_variable ();
        sprintf (buff, "Name        = [%s]\n", pVariable->szName);
        printf ("%s", buff);
        if  ((Debug == 'y') || (Beditwin == 'y')) {
            z999_open_bug ();
            fputs (buff, file_debug_out);
        }
        if  (pVariable->Type == 'S') {
            sprintf (buff, "    String  = [%s]\n", pVariable->szString);
        }
        else {
            sprintf (buff, "    Integer = [%d]\n", pVariable->Integer);
        }
        printf ("%s", buff);
        if  ((Debug == 'y') || (Beditwin == 'y')) {
            z999_open_bug ();
            fputs (buff, file_debug_out);
        }
        if  (pVariable->szString [0] == Subs) {
            strcpy (Command, pVariable->szString + 1);
            Command [strlen (Command) - 1] = 0;
        }
        else {
            Command [0] = 0;
        }
    }
    while (Command [0] != 0);
    sprintf (buff, "----- End of Chain -----\n");
    printf ("%s", buff);
    if  ((Debug == 'y') || (Beditwin == 'y')) {
        z999_open_bug ();
        fputs (buff, file_debug_out);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process the SCAN command.                                    */
/********************************************************************/
void d800_scan () {

    unsigned char   CharScanr;

    CharScanr = Scanr;

    strcpy (szScanTest, ":");
    strcpy (szScanStr, Command);
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    szScanTest [0] = szScanRight [0];
    Delimiter = szScanTest [0];
    szScanTest [1] = 0;
    strcpy (szScanStr, szScanRight + 1);
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }

    if  (szScanLeft [0] == Subs) {
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szScanLeft, pVariable->szString);
        }
        else {
            sprintf (szScanLeft, "%d", pVariable->Integer);
        }
    }

    strcpy (szChangeFrom, szScanLeft);
    if  ((szScanRight [0] != ':') || (strlen (szScanRight) < 2)) {
        z010_syntax_error ();
        return;
    }

    Word = 'n';
    if  ((strcmp (szScanRight, ":WORD") == 0) ||
         (strcmp (szScanRight, ":word") == 0)) {
        Word = 'y';
        index3 = 0;
        if  (CharScanr == 'y') {
            strrev (szChangeFrom);
        }
        do  {
            if  (szChangeFrom [index3] == ' ') {
                index3++;
            }
        }
        while (szChangeFrom [index3] == ' ');
        Lwordindex = index3;
        strcpy (szScanLeft, szChangeFrom + index3);
        if  (CharScanr == 'y') {
            strrev (szScanLeft);
        }
        strcpy (szChangeFrom, szScanLeft);
        strcpy (szScanLeft, " ");
    }
    else {
        szScanTest [0] = szScanRight [1];
        Delimiter = szScanTest [0];
        szScanTest [1] = 0;
        strcpy (szScanStr, szScanRight + 2);
        z900_scan ();
        if  (cScanFound != 'y') {
            z010_syntax_error ();
            return;
        }

        if  (szScanLeft [0] == Subs) {
            strcpy (Command, szScanLeft);
            d580_find_variable ();
            if  (pVariable->Type == 'S') {
                strcpy (szScanLeft, pVariable->szString);
            }
            else {
                sprintf (szScanLeft, "%d", pVariable->Integer);
            }
        }
    }

    strcpy (szChangeTo, szScanLeft);

    if  (Scanr == 'y') {
        cScanFromRight = 'y';
    }

    /*********************************/
    /* Scan for the string           */
    /*********************************/
    strcpy (szScanStr, szChangeFrom);
    strcpy (szScanTest, szChangeTo);
    z900_scan ();

    if  ((Scanr == 'y') && (cScanFound == 'y')) {
        strcpy (temp, szScanLeft);
        strcpy (szScanLeft, szScanRight);
        strcpy (szScanRight, temp);
    }

    cScanFromRight = 'n';
    Scanr = 'n';

    if  ((cScanFound != 'y') &&
         (Word == 'y') &&
         (szScanStr [0] > 0)) {
        cScanFound = 'y';
        strcpy (szScanLeft, szScanStr);
        szScanRight [0] = 0;
    }

    if  (cScanFound == 'y') {
        /***************/
        /* Left        */
        /***************/
        strcpy (Command, "left");
        d580_find_variable ();
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (strlen (szScanLeft) + 5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad store LEFT");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, szScanLeft);
        /***************/
        /* RIGHT       */
        /***************/
        strcpy (Command, "right");
        d580_find_variable ();
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (strlen (szScanRight) + 5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad store RIGHT");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, szScanRight);
        /***************/
        /* MID         */
        /***************/
        strcpy (Command, "mid");
        d580_find_variable ();
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (strlen (szScanMid) + 5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad store MID");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, szScanMid);

        /***************/
        /* MIDRIGHT    */
        /***************/
        strcpy (Command, "midright");
        d580_find_variable ();
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (strlen (szScanMid) + strlen (szScanRight) + 5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad store MIDRIGHT");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, szScanMid);
        strcat (pVariable->szString, szScanRight);

        /***************/
        /* LWORD       */
        /***************/
        if  (Word == 'y') {
            memset (szLword, ' ', Max);
            szLword [Lwordindex] = 0;
            strcpy (Command, "lword");
            d580_find_variable ();
            free (pVariable->szString);
            pVariable->szString = (unsigned char *) malloc (strlen (szLword) + 5);
            if  (!pVariable->szString) {
                strcpy (Error, "Bad store LWORD");
                z110_fatal ();
                return;
            }
            strcpy (pVariable->szString, szLword);
        }

        /***************/
        /* RWORD       */
        /***************/
        if  (Word == 'y') {
            strcpy (Command, "rword");
            d580_find_variable ();
            free (pVariable->szString);
            pVariable->szString = (unsigned char *) malloc (strlen (szChangeFrom) + 5);
            if  (!pVariable->szString) {
                strcpy (Error, "Bad store RWORD");
                z110_fatal ();
                return;
            }
            strcpy (pVariable->szString, szChangeFrom);
        }

        /***************/
        /* FOUND = 'Y' */
        /***************/
        strcpy (Command, "found");
        d580_find_variable ();
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad store found");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, "Y");
    }
    else {
        /***************/
        /* FOUND = 'N' */
        /***************/
        strcpy (Command, "found");
        d580_find_variable ();
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad store found");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, "N");
        /***************/
        /* Left/right  */
        /***************/
        if  (CharScanr == 'y') {
            strcpy (Command, "right");
        }
        else {
            strcpy (Command, "left");
        }
        d580_find_variable ();
        free (pVariable->szString);
        pVariable->szString = (unsigned char *) malloc (strlen (szScanStr) + 5);
        if  (!pVariable->szString) {
            strcpy (Error, "Bad store LEFT");
            z110_fatal ();
            return;
        }
        strcpy (pVariable->szString, szScanStr);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process the SPLIT command.                                   */
/********************************************************************/
void d900_split () {

    //printf ("d900_split\n");

    strcpy (szScanTest, ":");
    strcpy (szScanStr, Command);
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    szScanTest [0] = szScanRight [0];
    Delimiter = szScanTest [0];
    szScanTest [1] = 0;
    strcpy (szScanStr, szScanRight + 1);
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }

    if  (szScanLeft [0] == Subs) {
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szScanLeft, pVariable->szString);
        }
        else {
            sprintf (szScanLeft, "%d", pVariable->Integer);
        }
    }

    strcpy (szChangeFrom, szScanLeft);

    if  ((szScanRight [0] != ';') || (strlen (szScanRight) < 2)) {
        z010_syntax_error ();
        return;
    }

    if  ((szScanRight [1] > 47) && (szScanRight [1] < 58)) {
        Number = 0;
        Number = (long int) strtol ((char*)szScanRight + 1, (char**)&p, 10);
    }
    else {
        if  (szScanRight [1] == Subs) {
            strcpy (Command, szScanRight + 1);
            d580_find_variable ();
            if  (pVariable->Type == 'S') {
                Number = 0;
                Number = (long int) strtol ((char*)pVariable->szString, (char**)&p, 10);
            }
            else {
                Number = pVariable->Integer;
            }
        }
        else {
            z010_syntax_error ();
            return;
        }
    }
    if  (Number < 1) {
        Number = 0;
    }

    /***************/
    /* Find Left   */
    /***************/
    if  (Scanr == 'y') {
        strrev (szChangeFrom);
    }
    strcpy (szScanLeft, szChangeFrom);
    if  (!(Number > (long) strlen (szChangeFrom))) {
        szScanLeft [Number] = 0;
    }

    /***************/
    /* Find Right  */
    /***************/
    //sprintf (buff, "Number %d\n", (int) Number);
    //printf (buff);
    if  (!(Number > (long) strlen (szChangeFrom))) {
        strcpy (szScanRight, szChangeFrom + Number);
    }
    else {
        szScanRight [0] = 0;
    }

    if  (Scanr == 'y') {
        strrev (szChangeFrom);
        strrev (szScanLeft);
        strrev (szScanRight);
        strcpy (temp, szScanLeft);
        strcpy (szScanLeft, szScanRight);
        strcpy (szScanRight, temp);
        Scanr = 'n';
    }


    /***************/
    /* Store Left  */
    /***************/
    strcpy (Command, "left");
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (szScanLeft) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad store LEFT");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szScanLeft);

    /***************/
    /* Store Right */
    /***************/
    strcpy (Command, "right");
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (szScanRight) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad store RIGHT");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szScanRight);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process EXTRACT                                              */
/********************************************************************/
void d950_extract () {
    /*******************/
    /* Variable        */
    /*******************/
    c120_parse_command ();
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    if  (szScanLeft [0] == Subs) {
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (szScanLeft, pVariable->szString);
    }
    strcpy (Command, szScanLeft);
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    pVar1 = pVariable;
    /*******************/
    /* Start           */
    /*******************/
    strcpy (szScanStr, szScanRight);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    if  (szScanLeft [0] == Subs) {
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szScanLeft, pVariable->szString);
        }
        else {
            sprintf (szScanLeft, "%d", pVariable->Integer);
        }
    }
    ExtrStart = 0;
    ExtrStart = (long int) strtol ((char*) szScanLeft, (char**)&p, 10);
    /*******************/
    /* Length          */
    /*******************/
    if  (szScanRight [0] == Subs) {
        strcpy (Command, szScanRight);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szScanRight, pVariable->szString);
        }
        else {
            sprintf (szScanRight, "%d", pVariable->Integer);
        }
    }
    ExtrLen = 0;
    ExtrLen = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
    /*******************/
    /* Test Numbers    */
    /*******************/
    if  (ExtrStart > Max) {
        strcpy (Error, "Bad Start, Too Big");
        z110_fatal ();
        return;
    }
    if  (ExtrStart < 1) {
        strcpy (Error, "Bad Start, < 1");
        z110_fatal ();
        return;
    }
        if  (ExtrLen < 1) {
        strcpy (Error, "Bad Length, < 1");
        z110_fatal ();
        return;
    }
    if  (ExtrLen > Max) {
        strcpy (Error, "Bad Length, Too Large");
        z110_fatal ();
        return;
    }
    /*******************/
    /* Find Strings    */
    /*******************/
    strcpy (szScanLeft, "");
    strcpy (szScanTest, "");
    strcpy (szScanRight, "");
    /******************/
    /* LEFT           */
    /******************/
    strcpy (szScanLeft, pVar1->szString);
    szScanLeft [ExtrStart - 1] = 0;
    /******************/
    /* MID            */
    /******************/
    if  ((int) strlen (pVar1->szString) > (ExtrStart - 1)) {
        strcpy (szScanTest, pVar1->szString + (ExtrStart - 1));
        szScanTest [ExtrLen] = 0;
    }
    /******************/
    /* RIGHT          */
    /******************/
    if  ((int) strlen (pVar1->szString) > (ExtrStart + ExtrLen - 1)) {
        strcpy (szScanRight, pVar1->szString + (ExtrStart + ExtrLen - 1));
    }
    /*******************/
    /* Save Strings    */
    /*******************/
    /***************/
    /* Left        */
    /***************/
    strcpy (Command, "left");
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (szScanLeft) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad store LEFT");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szScanLeft);
    /***************/
    /* RIGHT       */
    /***************/
    strcpy (Command, "right");
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (szScanRight) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad store RIGHT");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szScanRight);
    /***************/
    /* MID         */
    /***************/
    strcpy (Command, "mid");
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (szScanTest) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad store MID");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, szScanTest);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Size a string variable                                       */
/********************************************************************/
void e100_size () {
    c120_parse_command ();
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    if  (szScanRight [0] == Subs) {
        strcpy (Command, szScanRight);
        d580_find_variable ();
        Number = pVariable->Integer;
    }
    else {
        Number = 0;
        Number = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
    }
    if  (Number < 1) {
        strcpy (Error, "Bad Size");
        z110_fatal ();
        return;
    }
    strcpy (Command, szScanLeft);
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    memset (temp, ' ', Max);
    strcpy (temp, pVariable->szString);
    if  (Number < (long) strlen (pVariable->szString)) {
        temp [Number] = 0;
    }
    else {
        temp [strlen (temp)] = ' ';
        temp [Number] = 0;
    }

    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (temp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Can't SIZE field");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, temp);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Store a random number in the integer variable                */
/********************************************************************/
void e102_random () {
    int iMin;
    int iMax;
    int iDiff;
    VARIABLE *pRandomVar;
    int Multiplier;

    c120_parse_command ();
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    //*************************************
    //* Find Variable
    //*************************************
    strcpy (Command, szScanLeft);
    d580_find_variable ();
    if  (!pVariable) {
        return;
    }
    if  (pVariable->Type != 'I') {
        strcpy (Error, "Must use INTEGER");
        z110_fatal ();
        return;
    }
    pRandomVar = pVariable;

    strcpy (szScanStr, szScanRight);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    //*************************************
    //* Find Min
    //*************************************
    if  (szScanLeft [0] == Subs) {
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (!pVariable) {  
            return;
        }
        if  (pVariable->Type != 'I') {
            strcpy (Error, "Min must be integer");
            z110_fatal();
            return;
        }
        iMin = pVariable->Integer;
    }
    else {
        iMin = (long int) strtol ((char*)szScanLeft, (char**)&p, 10);
    }
    if  (iMin < 1) {
        strcpy (Error, "Bad Size");
        z110_fatal ();
        return;
    }
    //*************************************
    //* Find Max
    //*************************************
    if  (szScanRight [0] == Subs) {
        strcpy (Command, szScanRight);
        d580_find_variable ();
        if  (!pVariable) {  
            return;
        }
        if  (pVariable->Type != 'I') {
            strcpy (Error, "Max must be integer");
            z110_fatal();
            return;
        }
        iMax = pVariable->Integer;
    }
    else {
        iMax = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
    }
    if  (iMin > iMax) {
        strcpy (Error, "Bad Range");
        z110_fatal ();
        return;
    }
    
    //*************************************
    //* Find Multiplier
    //*************************************
    strcpy (Command, "time");
    d580_find_variable ();
    Multiplier = 1 + (long int) strtol ((char*)pVariable->szString + 6, (char**)&p, 10);

    //*************************************
    //* Calculate Random Number
    //*************************************
    iDiff = iMax - iMin;
    pRandomVar->Integer = rand() * Multiplier;
    if  (pRandomVar->Integer < 0) {
        pRandomVar->Integer = pRandomVar->Integer * -1;
    }
    if  (iMin == iMax) {
        pRandomVar->Integer = iMin;
    }
    else {
        pRandomVar->Integer = iMin + pRandomVar->Integer%iDiff;
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     ENCRYPT/DECRYPT a custom string variable                     */
/********************************************************************/
void e105_encrypt (int iEncrypt, char cType) {
    int i1;
    int i2;
    int iStrLen;
    int iKeyLen;
    int iLoop;
    unsigned char char1;
    unsigned char char2;
    unsigned char cResult;
    int iResult;
    int iChar1;
    int iChar2;
    unsigned char szTemp [5000];
    if  ((cType != 'c') && (iEncrypt == 0)) {
        if  (strcmp (szEncryptKey, szDefaultKey) == 0) {
            strcpy (Error, "Can't decrypt using Default Encryption Key");
            z110_fatal ();
            return;
        }
    }
    if  (cType == 'c') {
        strcpy (szTemp, input_line);
    }
    else {
        if  (cType == 'f') {
            strcpy (szTemp, pDataFile->szLine);
        }
        else {
            c120_parse_command ();
            d580_find_variable ();
            if  (found != 'y') {
                return;
            }
            if  (pVariable->Type != 'S') {
                strcpy (Error, "Must use Str");
                z110_fatal ();
                return;
            }
            strcpy (szTemp, pVariable->szString);
        }
    }

    iStrLen = strlen (szTemp);
    iKeyLen = strlen (szEncryptKey);
    if  (iStrLen == 0) {
        return;
    }
    i1 = 0;
    if  ((cType == 'f') || (cType == 'c')) {
        i2 = iEncryptKeyPtr;
    }
    else {
        i2 = 0;
    }

    do  {
        char1 = szEncryptKey[i2];
        i2++;
        if  (i2 < iKeyLen) {
            char2 = szEncryptKey[i2];
        }
        else {
            i2 = 0;
            char2 = szEncryptKey[i2];
        }    
        iChar1 = char1;
        iChar2 = char2;
 
        if  (szTemp[i1] > 31) {
            if (iEncrypt == 1) {
                if  ((szTemp[i1] < 1) || (szTemp[i1] > 126)) {
                    sprintf (Error, "Character out of range [%c]Value[%d] Position[%d][%s]\n", szTemp[i1], (int)szTemp[i1], i1, pVariable->szString);
                    z110_fatal ();
                    strcpy (pVariable->szString, szTemp);
                    return;
                }
                cResult = szTemp[i1] + 65;
                iResult = cResult + iChar1;
                if  (iResult > 255) {
                    cResult = 65 + ((cResult + iChar1) - 255);
                }
                else {
                    cResult = cResult + iChar1;
                }
                iResult = cResult - iChar2;
                if  (iResult < 65) {
                    cResult = 255 - (65 - (cResult - iChar2));
                }
                else {
                    cResult = cResult - iChar2;
                }
                szTemp[i1] = cResult;
                //iEncrypt = 0;
            }
            else {
                cResult = szTemp[i1];
                iResult = cResult + iChar2;
                if  (iResult > 255) {
                    cResult = 65 + ((cResult + iChar2) - 255);
                }
                else {
                    cResult = cResult + iChar2;
                }
                iResult = cResult - iChar1;
                if  (iResult < 65) {
                    cResult = 255 - (65 - (cResult - iChar1));
                }
                else {
                    cResult = cResult - iChar1;
                }
                cResult = cResult - 65;
                szTemp[i1] = cResult;
            }
        }
        i1++;        
    }
    while (i1 < iStrLen);
    if  (cType == 'c') {
        strcpy (input_line, szTemp);
    }
    else {
        if  (cType == 'f') {
            strcpy (pDataFile->szLine, szTemp);
        }
        else {
            strcpy (pVariable->szString, szTemp);
        }
    }
    iEncryptKeyPtr = i2;
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Encrypt/Decrypt an entire file                               */
/********************************************************************/
void e106_encrypt_file (int iEncrypt) {

    if  (iEncrypt == 0) {
        if  (strcmp (szEncryptKey, szDefaultKey) == 0) {
            strcpy (Error, "Can't decrypt using Default Encryption Key");
            z110_fatal ();
            return;
        }
    }

    pcount = 0;
    iEncryptKeyPtr = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    //***************************
    //* Move to Top of File
    //***************************
    do {
        if  (pDataFile->pPrev) {
            pDataFile = pDataFile->pPrev;
        }
    }
    while (pDataFile->pPrev);
    //***************************
    //* Encrypt each line
    //*************************** 
    e105_encrypt (iEncrypt, 'f');   
    do  {
        if  (pDataFile->pNext) {
            pDataFile = pDataFile->pNext;
            e105_encrypt (iEncrypt, 'f');
        }
        pcount++;
    }
    while (pDataFile->pNext);
    EOF_Flag = 'y';   
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     PACK a string variable                                       */
/********************************************************************/
void e110_pack () {
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    index4 = strlen (pVariable->szString);
    if  (index4 == 0) {
        return;
    }
    do  {
        if  (index4 > 0) {
            index4--;
            if  (pVariable->szString [index4] == ' ') {
                pVariable->szString [index4] = 0;
            }
            else {
                index4 = 0;
            }
        }
    }
    while (index4 > 0);
    strcpy (temp, pVariable->szString);
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (temp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad PACK");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, temp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     CRUSH a string variable                                      */
/********************************************************************/
void e120_crush () {
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    done3 = 'n';
    index3 = 0;
    index4 = 0;
    do  {
        if  (pVariable->szString [index3] < 32) {
            done3 = 'y';
        }
        else {
            if  (pVariable->szString [index3] != ' ') {
                temp [index4] = pVariable->szString [index3];
                index4++;
            }
            index3++;
        }
    }
    while (done3 != 'y');
    temp [index4] = 0;

    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (temp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad PACK");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, temp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Right Justify a string variable                              */
/********************************************************************/
void e130_right () {
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    if  (pVariable->szString [0] == 0) {
        return;
    }
    memset (temp, 0, Max);
    memset (temp, ' ', strlen (pVariable->szString));
    index3 = strlen (pVariable->szString);
    index4 = index3;
    done3 = 'n';
    do  {
        if  (index3 > 0) {
            index3--;
        }
        if  (index3 == 0) {
            done3 = 'y';
        }
        if  (pVariable->szString [index3] > 32) {
            done3 = 'y';
        }
    }
    while (done3 != 'y');
    index3++;
    do  {
        index3--;
        index4--;
        temp [index4] = pVariable->szString [index3];
    }
    while (index3 > 0);

    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (temp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad PACK");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, temp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Left Justify a string variable                               */
/********************************************************************/
void e140_left () {
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    if  (pVariable->szString [0] == 0) {
        return;
    }
    memset (temp, 0, Max);
    memset (temp, ' ', strlen (pVariable->szString));
    index3 = 0;
    index4 = 0;
    done3 = 'n';
    do  {
        if  (index3 == (int) strlen (pVariable->szString)) {
            done3 = 'y';
        }
        else {
            if  (pVariable->szString [index3] > 32) {
                done3 = 'y';
            }
            else {
                index3++;
            }
        }
    }
    while (done3 != 'y');
    do  {
        temp [index4] = pVariable->szString [index3];
        index3++;
        index4++;
    }
    while (index3 < (int) strlen (pVariable->szString));

    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (temp) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad PACK");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, temp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Mix Case a string variable                                   */
/********************************************************************/
void e150_mix () {
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must ust Str");
        z110_fatal ();
        return;
    }
    if  (pVariable->szString [0] == 0) {
        return;
    }

    for (index3 = 0; index3 < (int) strlen (pVariable->szString); index3++) {
        if  (pVariable->szString [index3] > 32) {
            if  (((index3 == 0) && (strlen (pVariable->szString) > 1)) ||
                 ((index3 > 0) && (pVariable->szString [index3 - 1] == 32))) {
                pVariable->szString [index3] = (unsigned char) toupper (pVariable->szString [index3]);
            }
            else {
                pVariable->szString [index3] = (unsigned char) tolower (pVariable->szString [index3]);
            }
        }
    }
    pVariable->szString [0] = (unsigned char) toupper (pVariable->szString [0]);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Upper Case a string variable                                 */
/********************************************************************/
void e160_upper () {
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    if  (pVariable->szString [0] == 0) {
        return;
    }
    for (index3 = 0; index3 < (int) strlen (pVariable->szString); index3++) {
        pVariable->szString [index3] = (unsigned char) toupper (pVariable->szString [index3]);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Lower Case a string variable                                 */
/********************************************************************/
void e170_lower () {
    c120_parse_command ();
    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    if  (pVariable->szString [0] == 0) {
        return;
    }
    for (index3 = 0; index3 < (int) strlen (pVariable->szString); index3++) {
        pVariable->szString [index3] = (unsigned char) tolower (pVariable->szString [index3]);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Save Variables                                               */
/********************************************************************/
void e200_save () {
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (Command, pVariable->szString);
    }
    /*********************************/
    /* SAVE *                        */
    /*********************************/
    if  (strcmp (Command, "*") == 0) {
        pVariable = &StartVariable;
        do  {
            if  (pVariable->pNext) {
                pVariable = pVariable->pNext;
                e220_save_variable ();
            }
        }
        while (pVariable->pNext);
        return;
    }

    /*********************************/
    /* SAVE SYSVARS                  */
    /*********************************/
    if  ((strcmp (Command, "SYSVARS") == 0) ||
         (strcmp (Command, "sysvars") == 0)) {
        pVariable = &StartVariable;
        do  {
            if  (pVariable->pNext) {
                pVariable = pVariable->pNext;
                e220_save_variable ();
                if  (strcmp (pVariable->szName, "time") == 0) {
                    return;
                }
            }
        }
        while (pVariable->pNext);
        return;
    }

    /*********************************/
    /* SAVE USERVARS                 */
    /*********************************/
    if  ((strcmp (Command, "USERVARS") == 0) ||
         (strcmp (Command, "uservars") == 0)) {
        strcpy (Command, "time");
        d580_find_variable ();
        do  {
            if  (pVariable->pNext) {
                pVariable = pVariable->pNext;
                e220_save_variable ();
            }
        }
        while (pVariable->pNext);
        return;
    }

    /*********************************/
    /* SAVE name                     */
    /*********************************/
    d580_find_variable ();
    e220_save_variable ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Restore Variables                                            */
/********************************************************************/
void e210_restore () {
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (Command, pVariable->szString);
    }
    /*********************************/
    /* RESTORE *                     */
    /*********************************/
    if  (strcmp (Command, "*") == 0) {
        pVariable = &StartVariable;
        do  {
            if  (pVariable->pNext) {
                pVariable = pVariable->pNext;
                if  (pVariable->Saved == 'y') {
                    e230_restore_variable ();
                }
            }
        }
        while (pVariable->pNext);
        return;
    }
    /*********************************/
    /* RESTORE SYSVARS               */
    /*********************************/
    if  ((strcmp (Command, "SYSVARS") == 0) ||
         (strcmp (Command, "sysvars") == 0)) {
        pVariable = &StartVariable;
        do  {
            if  (pVariable->pNext) {
                pVariable = pVariable->pNext;
                if  (pVariable->Saved == 'y') {
                    e230_restore_variable ();
                }
                if  (strcmp (pVariable->szName, "time") == 0) {
                    return;
                }
            }
        }
        while (pVariable->pNext);
        return;
    }

    /*********************************/
    /* RESTORE USERVARS              */
    /*********************************/
    if  ((strcmp (Command, "USERVARS") == 0) ||
         (strcmp (Command, "uservars") == 0)) {
        strcpy (Command, "time");
        d580_find_variable ();
        do  {
            if  (pVariable->pNext) {
                pVariable = pVariable->pNext;
                if  (pVariable->Saved == 'y') {
                    e230_restore_variable ();
                }
            }
        }
        while (pVariable->pNext);
        return;
    }

    /*********************************/
    /* RESTORE name                  */
    /*********************************/
    d580_find_variable ();
    e230_restore_variable ();

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Save Variables                                               */
/********************************************************************/
void e220_save_variable () {
    if  (pVariable->Saved == 'y') {
        free (pVariable->szSaveString);
    }
    pVariable->Saved = 'y';
    pVariable->szSaveString = (unsigned char *) malloc (strlen (pVariable->szString) + 5);
    if  (!pVariable->szSaveString) {
        strcpy (Error, "Bad Save Var");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szSaveString, pVariable->szString);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Restore Variables                                            */
/********************************************************************/
void e230_restore_variable () {
    if  (pVariable->Saved != 'y') {
        strcpy (Error, "Bad restore Non-Saved Var");
        z110_fatal ();
        return;
    }
    free (pVariable->szString);
    pVariable->szString = (unsigned char *) malloc (strlen (pVariable->szSaveString) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad Restore Variable");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, pVariable->szSaveString);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Move one line down in the file.                              */
/********************************************************************/
void f100_next () {
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    if  (Direction == 'U') {
        Direction = 'D';
        f110_prev ();
        Direction = 'U';
        return;
    }
    EOF_Flag = 'n';
    if  (Number > 0) {
        do  {
            Number--;
            if  (pDataFile->pNext) {
                pDataFile = pDataFile->pNext;
                pcount++;
            }
            else {
                Number = 0;
                if  (interactive == 'y') {
                    printf ("%s", "EOF\n");
                }
                if  (Debug == 'y') {
                    z999_open_bug ();
                    fputs ("EOF\n", file_debug_out);
                }
                EOF_Flag = 'y';
            }
        }
        while (Number > 0);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Move one line up in the file.                                */
/********************************************************************/
void f110_prev () {
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    if  (Direction == 'U') {
        Direction = 'D';
        f100_next ();
        Direction = 'U';
        return;
    }
    EOF_Flag = 'n';
    if  (Number > 0) {
        do  {
            Number--;
            if  (pDataFile->pPrev) {
                pDataFile = pDataFile->pPrev;
                pcount++;
            }
            else {
                Number = 0;
            }
        }
        while (Number > 0);
    }
    if  (pDataFile->Begin == 'y') {
        if  (Direction == 'U') {
            if  (interactive == 'y') {
                printf ("%s", "TOF\n");
            }
            if  (Debug == 'y') {
                z999_open_bug ();
                fputs ("TOF\n", file_debug_out);
            }
            EOF_Flag = 'y';
        }
        pcount--;
        if  (pDataFile->pNext) {
            pDataFile = pDataFile->pNext;
        }
        else {
            if  (interactive == 'y') {
                printf ("%s", "EOF\n");
            }
            if  (Debug == 'y') {
                z999_open_bug ();
                fputs ("EOF\n", file_debug_out);
            }
            EOF_Flag = 'y';
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Test the variable to see if it is @name@, if so convert it   */
/********************************************************************/
void g100_convert_variable () {
    if  (Command [0] == Subs) {
        strcpy (Tvariable, Command + 1);
        strcpy (Command, Tvariable);
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use str");
            z110_fatal ();
            strcpy (Command, "");
            return;
        }
        strcpy (Command, pVariable->szString);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Convert a string with imbedded variables.                    */
/********************************************************************/
void g200_convert_string () {
    x1 = 0;
    x2 = 0;
    do  {
        if  (szConvString [x1] != 0) {
            if  (szConvString [x1] == Subs) {
                x3 = 0;
                do  {
                    if  (szConvString [x1] == 0) {
//                        strcpy (Error, "Syntax Error");
//                        z110_fatal ();
                        return;
                    }
                    Command [x3] = szConvString [x1];
                    x3++;
                    x1++;
                }
                while (szConvString [x1] != Subs);
                Command [x3] = 0;

                strcpy (szScanStr, Command);
                strcpy (szScanTest, "[");
                z900_scan ();
                if  (cScanFound == 'y') {
                    strcpy (szPartialVar, "partial1");
                    d117_partial_var ();
                }
                else {
                    d580_find_variable ();
                }

                if  (pVariable->Type == 'S') {
                    strcpy (Command, pVariable->szString);
                }
                else {
                    sprintf (Command, "%d", pVariable->Integer);
                }
                x3 = 0;
                do  {
                    if  (Command [x3] != 0) {
                        szNewString [x2] = Command [x3];
                        x2++;
                        x3++;
                    }
                }
                while (Command [x3] != 0);
            }
            else {
                szNewString [x2] = szConvString [x1];
                x2++;
            }
            x1++;
        }
    }
    while (szConvString [x1] != 0);
    szNewString [x2] = 0;
    strcpy (szConvString, szNewString);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Print out the heading line.                                  */
/********************************************************************/
void w100_heading () {
    /* printf ("%s", "w100_heading () {\n"); */
    printf ("%s",
    "************************************************************************\n"
        );
    printf ("%s",
    "*     BEDIT v1.4 : Batch Editing Language System                      *\n"
        );
    printf ("%s",
    "************************************************************************\n"
        );
    printf ("%s", "\n");
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Handle Control-Break processing.                             */
/********************************************************************/
void x100_break (int sig) {
    z100_error ();
    strcpy (input_line, "goto !BREAK");
    Break = 'y';
    b200_proc ();
    exit (sig);
}

/********************************************************************/
/* This module will :                                               */
/*     Input a string into temp.                                    */
/********************************************************************/
void y100_input_string () {
    /* printf ("%s", "y100_input_string () {\n"); */
    gets (temp);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Input an interactive command.                                */
/********************************************************************/
void y200_input_interactive () {
    gets (temp);
    return;
/*
    index8 = 0;
    buff [1] = 0;
    FirstInteractive = 'y';
    pCurrInteractive = pInteractive;
    //*************************
    //* Get the Cmd           *
    //*************************
    do  {
        temp [index8] = (unsigned char) getch ();
        if  ((index8 == 0) &&
             (temp [0] != 13)) {
            printf ("   ");
            index8 = 3;
            y210_backup ();
            y210_backup ();
            y210_backup ();
        }
        if  (temp [index8] < 1) {
            response = (unsigned char) getch ();
            if  (response == 75) {
                y210_backup ();
            }
            else {
                y250_interactive_cmds ();
            }
        }
        else {
            if  (temp [index8] == 8) {
                y210_backup ();
            }
            else {
                if  (temp [index8] == 27) {
                    printf ("\033[80D");           // Left 80 characters
                    printf ("\033[5C");            // Right 7 characters
                    printf ("\033[K");             // Erase EOL
                    index8 = 0;
                    memset (temp, 0, Max);
                }
                else {
                    printf ("%c", temp [index8]);
                    index8++;
                }
            }
        }
    }
    while (temp [index8 - 1] != 13);
    temp [index8 - 1] = 0;
    printf ("\n");
    //*************************
    //* Should It Be Stored   *
    //*************************
    if  (temp [0] == 0) {
        return;
    }
    if  ((pInteractive->pNext) ||
         (pInteractive->pPrev)) {
        if  (strcmp (pInteractive->szCmd, temp) == 0) {
            return;
        }
    }
    //*************************
    //* Move to the end       *
    //*************************
    do  {
        if  (pInteractive->pNext) {
            pInteractive = pInteractive->pNext;
        }
    }
    while (pInteractive->pNext);
    //*************************
    //* Store the new cmd     *
    //*************************
    pNewInteractive = (INTERACTIVE *) malloc (sizeof (INTERACTIVE));
    if  (!pNewInteractive) {
        strcpy (Error, "Bad Create Cmd");
        z110_fatal ();
        return;
    }
    memset (pNewInteractive, 0, sizeof (INTERACTIVE));
    pNewInteractive->szCmd = (unsigned char *) malloc (strlen (temp) + 5);
    if  (!pNewInteractive->szCmd) {
        strcpy (Error, "Bad Store Cmd");
        z110_fatal ();
        return;
    }
    pInteractive->pNext = pNewInteractive;
    pNewInteractive->pPrev = pInteractive;
    pNewInteractive->pNext = NULL;
    pInteractive = pNewInteractive;
    strcpy (pInteractive->szCmd, temp);
    return;
*/
}

/********************************************************************/
/* This module will :                                               */
/*     Backup in the interactive input.                             */
/********************************************************************/
void y210_backup () {
    int X_Loc;
    int Y_Loc;

    if  (index8 == 0) {
        return;
    }
    index8 = index8 - 1;
//    printf ("\033[1D");           // Backup Left
//    printf (" ");                 // Destroy last char
//    printf ("\033[1D");           // Backup Left

    X_Loc = wherex ();
    Y_Loc = wherey ();
    X_Loc--;                      
    gotoxy (X_Loc, Y_Loc);        // Backup Left
    printf (" ");                 // Destroy last char
    gotoxy (X_Loc, Y_Loc);        // Backup Left
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Check for Cursor up or down.                                 */
/********************************************************************/
void y250_interactive_cmds () {
    if  ((response != 72 /* UP */) &&
         (response != 80 /* DOWN */)) {
        z100_error ();
        return;
    }
    printf ("\033[80D");           // Left 80 characters
    printf ("\033[5C");            // Right 7 characters
    printf ("\033[K");             // Erase EOL
    index8 = 0;
    memset (temp, 0, Max);
    /*********************/
    /* Cursor UP         */
    /*********************/
    if  (response == 72) {         // UP
        if  (pInteractive->pPrev) {
            pInteractive = pInteractive->pPrev;
            if  (!pInteractive->pPrev) {
                do  {
                    if  (pInteractive->pNext) {
                        pInteractive = pInteractive->pNext;
                    }
                }
                while (pInteractive->pNext);
                if  (!pInteractive->pPrev) {
                    return;
                }
            }
        }
        else {
            return;
        }
    }
    /*********************/
    /* Cursor DOWN       */
    /*********************/
    if  (response == 80) {         // DOWN
        if  (pInteractive->pNext) {
            pInteractive = pInteractive->pNext;
        }
        else {
            pInteractive = &StartInteractive;
            if  (pInteractive->pNext) {
                pInteractive = pInteractive->pNext;
            }
            else {
                return;
            }
        }
    }
    if  (FirstInteractive == 'y') {
        FirstInteractive = 'n';
        pInteractive = pCurrInteractive;
    }
    printf ("%s", pInteractive->szCmd);
    index8 = strlen (pInteractive->szCmd);
    strcpy (temp, pInteractive->szCmd);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Input a string                                               */
/********************************************************************/
void y500_input_string (char cSingleChar) {
    index8 = 0;
    buff [1] = 0;
    strcpy (szFunctionkey, "");
    do  {
        temp [index8] = (unsigned char) getch ();
        if  (temp [index8] < 1) {
            response = (unsigned char) getch ();
            if  (response == 75) {
                if  (cSingleChar == 'c') {
                    strcpy (szFunctionkey, "LEFT");
                    temp [index8] = 13;
                    index8++;
                }
                else {
                    y210_backup ();
                }
            }
            else {
                //sprintf (buff, "Char[%c][%d]\n", response, (int)response);
                //printf ("%s", buff);
                if  (response == 59) {
                    strcpy (szFunctionkey, "F1");
                }
                if  (response == 60) {
                    strcpy (szFunctionkey, "F2");
                }
                if  (response == 61) {
                    strcpy (szFunctionkey, "F3");
                }
                if  (response == 62) {
                    strcpy (szFunctionkey, "F4");
                }
                if  (response == 63) {
                    strcpy (szFunctionkey, "F5");
                }
                if  (response == 64) {
                    strcpy (szFunctionkey, "F6");
                }
                if  (response == 65) {
                    strcpy (szFunctionkey, "F7");
                }
                if  (response == 66) {
                    strcpy (szFunctionkey, "F8");
                }
                if  (response == 67) {
                    strcpy (szFunctionkey, "F9");
                }
                if  (response == 68) {
                    strcpy (szFunctionkey, "F10");
                }
                //if  (response == -123) {
                //    strcpy (szFunctionkey, "F11");
                //}
                //if  (response == -122) {
                //    strcpy (szFunctionkey, "F12");
                //}
                if  (response == 84) {
                    strcpy (szFunctionkey, "SHFT-F1");
                }
                if  (response == 85) {
                    strcpy (szFunctionkey, "SHFT-F2");
                }
                if  (response == 86) {
                    strcpy (szFunctionkey, "SHFT-F3");
                }
                if  (response == 87) {
                    strcpy (szFunctionkey, "SHFT-F4");
                }
                if  (response == 88) {
                    strcpy (szFunctionkey, "SHFT-F5");
                }
                if  (response == 89) {
                    strcpy (szFunctionkey, "SHFT-F6");
                }
                if  (response == 90) {
                    strcpy (szFunctionkey, "SHFT-F7");
                }
                if  (response == 91) {
                    strcpy (szFunctionkey, "SHFT-F8");
                }
                if  (response == 92) {
                    strcpy (szFunctionkey, "SHFT-F9");
                }
                if  (response == 93) {
                    strcpy (szFunctionkey, "SHFT-F10");
                }
                //if  (response == -121) {
                //    strcpy (szFunctionkey, "SHFT-F11");
                //}
                //if  (response == -120) {
                //    strcpy (szFunctionkey, "SHFT-F12");
                //}
                if  (response == 94) {
                    strcpy (szFunctionkey, "CTRL-F1");
                }
                if  (response == 95) {
                    strcpy (szFunctionkey, "CTRL-F2");
                }
                if  (response == 96) {
                    strcpy (szFunctionkey, "CTRL-F3");
                }
                if  (response == 97) {
                    strcpy (szFunctionkey, "CTRL-F4");
                }
                if  (response == 98) {
                    strcpy (szFunctionkey, "CTRL-F5");
                }
                if  (response == 99) {
                    strcpy (szFunctionkey, "CTRL-F6");
                }
                if  (response == 100) {
                    strcpy (szFunctionkey, "CTRL-F7");
                }
                if  (response == 101) {
                    strcpy (szFunctionkey, "CTRL-F8");
                }
                if  (response == 102) {
                    strcpy (szFunctionkey, "CTRL-F9");
                }
                if  (response == 103) {
                    strcpy (szFunctionkey, "CTRL-F10");
                }
                //if  (response == -119) {
                //    strcpy (szFunctionkey, "CTRL-F11");
                //}
                //if  (response == -118) {
                //    strcpy (szFunctionkey, "CTRL-F12");
                //}
                if  (response == 80) {            
                    strcpy (szFunctionkey, "DOWN");
                }
                if  (response == 72) {
                    strcpy (szFunctionkey, "UP");
                }
                if  (response == 77) {
                    strcpy (szFunctionkey, "RIGHT");
                }
                if  (response == 15) {
                    strcpy (szFunctionkey, "BACKTAB");
                }
                if  (response == 82) {
                    strcpy (szFunctionkey, "INSERT");
                }
                if  (response == 83) {
                    strcpy (szFunctionkey, "DELETE");
                }
                if  (response == 71) {
                    strcpy (szFunctionkey, "HOME");
                }
                if  (response == 79) {
                    strcpy (szFunctionkey, "END");
                }
                if  (response == 73) {
                    strcpy (szFunctionkey, "PAGEUP");
                }
                if  (response == 81) {
                    strcpy (szFunctionkey, "PAGEDOWN");
                }
                temp [index8] = 13;
                index8++;
            }
        }
        else {
            if  (temp [index8] == 8) {
                y210_backup ();
            }
            else {
                if  (temp [index8] < 32) {
                    if  (temp [index8] == 9) {
                        strcpy (szFunctionkey, "TAB");
                    }
                    if  (temp [index8] == 27) {
                        strcpy (szFunctionkey, "ESC");
                    }
                    temp [index8] = 13;
                }
                if  (cSingleChar == 'p') {
                    printf ("*");
                }
                else {
                    if (cSingleChar != 'c') {
                        printf ("%c", temp [index8]);
                    }
                }
                index8++;
                if  (cSingleChar == 'c') {
                    temp[index8] = 13;
                    index8++;
                }
            }
        }
    }
    while ((temp [index8 - 1] != 13) && (index8 < (int) InputNumber));
    temp [index8] = 0;
    if  (temp [index8 - 1] == 13) {
        temp [index8 - 1] = 0;
    }
    printf ("\n");
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     display the syntax error message                             */
/********************************************************************/
void z010_syntax_error () {
    strcpy (Error, "Syntax Error");
    z110_fatal ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Sound the Bell if an error has occurred.                     */
/********************************************************************/
void z100_error () {
    /* printf ("%s", "z100_error () {\n"); */
    printf ("%c", bell);
    return;
}


/********************************************************************/
/* This module will :                                               */
/*     Fatal Error                                                  */
/********************************************************************/
void z110_fatal () {
    if  (Beditwin == 'y') {
        interactive = 'n';
    }
    Num1 = ErrorPtr;
    if  (More_Commands == 'y') {
        do  {
            c120_parse_command ();
            if  ((strcmp (Command, "ERROR") == 0) ||
                 (strcmp (Command, "error") == 0)) {
                return;
            }
        }
        while (More_Commands == 'y');
    }
    ErrorPtr = Num1;
    printf ("%s", Error);
    if  ((Debug == 'y') || (interactive != 'y')) {
        z999_open_bug ();
        fputs (Error, file_debug_out);
    }
    if  (Abort == 'y') {
//        if  (BEDITframe) {
//            WinDestroyWindow(BEDITframe);
//        }
//        if  (hmq) {
//            WinDestroyMsgQueue(hmq);
//        }
//        if  (hab) {
//            WinTerminate(hab);
//        }
        if  (file_debug_out) {
            fclose (file_debug_out);
        }
        //*******************************
        //* If deugging, write STOP to
        //* beditsem.stp to stop the
        //* Builder.bed Debugger
        //*******************************
        if  (cBeditDebug == 'y') {
            FILE *BeditBugFile;

            BeditBugFile = fopen ("beditsem.stp", "w");
            fputs("ABORT\n", BeditBugFile);
            fclose (BeditBugFile);
        }
        exit (1);
    }

    if  (interactive == 'y') {
        printf ("%s", "\n");
        return;
    }
    Debug = 'y';
    sprintf (buff, " on line %d of %s\n", pCommand->LineNo, szCurrentFile);
    printf ("%s", buff);
    z999_open_bug ();
    fputs (buff, file_debug_out);
    if  (Beditwin == 'y') {
        sprintf (buff, "Command = [%s]\n", Command);
        fputs (buff, file_debug_out);
        sprintf (buff, "%s\n", pCommand->szLine);
        fputs (buff, file_debug_out);
        if  (strcmp (pCommand->szLine, input_line) != 0) {
            sprintf (buff, "%s\n", input_line);
            fputs (buff, file_debug_out);
        }
        memset (ErrorLine, ' ', Max);
        ErrorLine [ErrorPtr] = '^';
        ErrorLine [ErrorPtr + 1] = 0;
        fputs (ErrorLine, file_debug_out);
        fclose (file_debug_out);
    }
    z111_fatal_message ();
    if  (Beditwin == 'y') {
//        if  (BEDITframe) {
//            WinDestroyWindow(BEDITframe);
//        }
//        if  (hmq) {
//            WinDestroyMsgQueue(hmq);
//        }
//        if  (hab) {
//            WinTerminate(hab);
//        }
        //*******************************
        //* If deugging, write STOP to
        //* beditsem.stp to stop the
        //* Builder.bed Debugger
        //*******************************
        if  (cBeditDebug == 'y') {
            FILE *BeditBugFile;

            BeditBugFile = fopen ("beditsem.stp", "w");
            fputs("ABORT\n", BeditBugFile);
            fclose (BeditBugFile);
        }
        exit (1);
    }

    sprintf (buff, "Command = [%s]\n", Command);
    printf ("%s", buff);
    fputs (buff, file_debug_out);
    sprintf (buff, "%s\n", pCommand->szLine);
    printf ("%s", buff);
    fputs (buff, file_debug_out);
    if  (strcmp (pCommand->szLine, input_line) != 0) {
        sprintf (buff, "%s\n", input_line);
        printf ("%s", buff);
        fputs (buff, file_debug_out);
    }
    memset (ErrorLine, ' ', Max);
    ErrorLine [ErrorPtr] = '^';
    ErrorLine [ErrorPtr + 1] = 0;
    printf ("%s", ErrorLine);
    fputs (ErrorLine, file_debug_out);
    printf ("%s", "\n");
    fputs ("\n", file_debug_out);
    printf ("%c", bell);

    /********************************/
    /* Dump all variables and files */
    /********************************/
    printf ("%s", "Press Any Key To Continue...\n");
    if  (Beditwin != 'y') {
        getch ();
    }
    printf ("%s", "***************** Variable Dump *********************\n");
    Abort = 'y';
    strcpy (input_line, "*");
    index1 = 0;
    d599_dump ();

    printf ("%s", "Press Any Key To Continue...\n");
    if  (Beditwin != 'y') {
        getch ();
    }
    printf ("%s", "******************* Files Dump **********************\n");
    d598_files ();

//    if  (BEDITframe) {
//        WinDestroyWindow(BEDITframe);
//    }
//    if  (hmq) {
//        WinDestroyMsgQueue(hmq);
//    }
//    if  (hab) {
//        WinTerminate(hab);
//    }
    //*******************************
    //* If deugging, write STOP to
    //* beditsem.stp to stop the
    //* Builder.bed Debugger
    //*******************************
    if  (cBeditDebug == 'y') {
        FILE *BeditBugFile;

        BeditBugFile = fopen ("beditsem.stp", "w");
        fputs("ABORT\n", BeditBugFile);
        fclose (BeditBugFile);
    }
    exit (1);
}

/********************************************************************/
/* This module will :                                               */
/*     Reset the current File pointers                              */
/********************************************************************/
void z200_reset_file_position () {
    if  (pOpenFile) {
        z300_store_file_position ();
    }
    pOpenFile = &StartOpenFile;
    do  {
        if  (!pOpenFile->pNext) {
            pOpenFile = NULL;
            strcpy (Error, "Bad locate file");
            FileError = 'y';
            z110_fatal ();
            pOpenFile = pOpenCurr;
            return;
        }
        pOpenFile = pOpenFile->pNext;
    }
    while (strcmp (pOpenFile->szFileName, szFileName) != 0);
    pDataFile = pOpenFile->pData;
    if  ((pDataFile->pNext) || (pDataFile->pPrev)) {
        EOF_Flag = 'n';
    }
    else {
        EOF_Flag = 'y';
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Store the current file pointers                              */
/********************************************************************/
void z300_store_file_position () {
    if  (!pOpenFile) {
        return;
    }
    pOpenCurr = pOpenFile;
    if  (pDataFile) {
        pOpenFile->pData = pDataFile;
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Scan for a string and set up left and right strings.         */
/********************************************************************/
void z900_scan () {

    unsigned char      Add2;
    int       Firstx1;
    int       Lastx1;
    unsigned char      Endpatt;

    ScanIndex1 = 0;
    ScanIndex2 = 0;
    Firstx1 = 0;
    //Lastx1 = 0;
    Endpatt = 'n';
    cScanFound = 'n';
    cScanDone = 'n';

    Add2 = 'y';
    strcpy (szScanKeep, szScanTest);

    if  (strlen (szScanTest) > strlen (szScanStr)) {
        return;
    }
    if  (szScanTest [0] == 0) {
        return;
    }
    if  (cScanFromRight == 'y') {
        z910_scan_reverse ();
    }

//    memset (szScanLeft, 0, Max);
//    memset (szScanRight, 0, Max);
//    memset (szScanMid, 0, Max);
    strcpy (szScanLeft, "");
    strcpy (szScanRight, "");
    strcpy (szScanMid, "");
    if  (iScramble == 2) {
        z905_scramble();
    }
    else {
        do  {
            ScanIndex2 = 0;
            cScanDone = 'n';
            do  {
                cScanChar1 = szScanStr [ScanIndex1+ScanIndex2];
                cScanChar2 = szScanTest [ScanIndex2];
    
                if  (cCase == 'n') {
                    cScanChar1 = (unsigned char) toupper (cScanChar1);
                    cScanChar2 = (unsigned char) toupper (cScanChar2); 
                }

                if  (Patton == 'y') {
                    /*********************************/
                    /* Any Character                 */
                    /*********************************/
                    if  ((cScanChar2 == Pattany) && (cScanChar1 > 10)) {
                        cScanChar2 = cScanChar1;
                    }
                    /*********************************/
                    /* Numeric Character             */
                    /*********************************/
                    if  ((cScanChar2 == Pattnum) && (cScanChar1 > 10)) {
                        if  (isdigit (cScanChar1)) {
                            cScanChar2 = cScanChar1;
                        }
                        else {
                            cScanChar2 = 255;
                        }
                    }
                    /*********************************/
                    /* Alpha Character               */
                    /*********************************/
                    if  ((cScanChar2 == Pattalp) && (cScanChar1 > 10)) {
                        if  ((isalpha (cScanChar1)) ||
                             (cScanChar1 == ' ')) {
                            cScanChar2 = cScanChar1;
                        }
                        else {
                            cScanChar2 = 255;
                        }
                    }
                    /*********************************/
                    /* Special Character             */
                    /*********************************/
                    if  ((cScanChar2 == Pattspc) && (cScanChar1 > 10)) {
                        if  ((!isdigit (cScanChar1)) &&
                             (!isalpha (cScanChar1)) &&
                             (cScanChar1 != ' ')) {
                            cScanChar2 = cScanChar1;
                        }
                        else {
                            cScanChar2 = 255;
                        }
                    }
                    /*********************************/
                    /* End Character                 */
                    /*********************************/
                    if  (cScanChar2 == Pattend) {
                        cScanChar2 = cScanChar1;
                        if  (szScanTest [ScanIndex2 + 1] == 0) {
                            cScanFound = 'y';
                            cScanDone = 'y';
                        }
                        else {
                            strcpy (szScanTest, szScanTest + ScanIndex2 + 1);
                            Add2 = 'n';
                            Endpatt = 'y';
                            if  (Firstx1 == 0) {
                                Firstx1 = ScanIndex1;
                            }
                            ScanIndex1 = ScanIndex1 + ScanIndex2;
                            ScanIndex2 = 0;
                        }
                    }
                }
                /*********************************/
                /* Check the Character           */
                /*********************************/
                if  (cScanChar1 != cScanChar2) {
                    cScanDone = 'y';
                }
                else {
                    if  (Add2 == 'n') {
                        Add2 = 'y';
                    }
                    else {
                        ScanIndex2++;
                    }
                    if  (ScanIndex2 > (int) (strlen (szScanTest) - 1)) {
                        cScanFound = 'y';
                        cScanDone = 'y';
                    }
                }
            }
            while (cScanDone != 'y');
            /**********/
            /* Found  */
            /**********/
            if (cScanFound == 'y') {
                cScanDone = 'y';
                if  (Endpatt == 'y') {
                    Lastx1 = ScanIndex1 + ScanIndex2;
                    /****************/
                    /* szScanLeft   */
                    /****************/
                    ScanIndex1 = 0;
                    ScanIndex2 = 0;
                    do  {
                        if  (ScanIndex1 < Firstx1) {
                            szScanLeft [ScanIndex2] = szScanStr [ScanIndex1];
                            ScanIndex1++;
                            ScanIndex2++;
                        }    
                    }    
                    while (ScanIndex1 < Firstx1);
                    szScanLeft [ScanIndex2] = 0;
                    /****************/
                    /* szScanMid    */
                    /****************/
                    ScanIndex2 = 0;
                    do  {
                        if  (ScanIndex1 < Lastx1) {
                            szScanMid [ScanIndex2] = szScanStr [ScanIndex1];
                            ScanIndex1++;
                            ScanIndex2++;
                        }
                    }
                    while (ScanIndex1 < Lastx1);
                    szScanMid [ScanIndex2] = 0;
                    /****************/
                    /* szScanRight  */
                    /****************/
                    ScanIndex2 = 0;
                    do  {
                        if  (ScanIndex1 < (int) strlen (szScanStr)) {
                            szScanRight [ScanIndex2] = szScanStr [ScanIndex1];
                            ScanIndex1++;
                            ScanIndex2++;
                        }
                    }
                    while (ScanIndex1 < (int) strlen (szScanStr));
                    szScanRight [ScanIndex2] = 0;
                }
                else {
                    /****************/
                    /* szScanLeft   */
                    /****************/
                    for (ScanIndex2 = 0; ScanIndex2 < ScanIndex1; ScanIndex2++) {
                        szScanLeft [ScanIndex2] = szScanStr [ScanIndex2];
                    }
                    szScanLeft [ScanIndex2] = 0;
                    /****************/
                    /* szScanMid    */
                    /****************/
                    for (ScanIndex2 = 0; ScanIndex2 < (int) strlen (szScanTest); ScanIndex2++) {
                        szScanMid [ScanIndex2] = szScanStr [ScanIndex1 + ScanIndex2];
                    }
                    szScanMid [ScanIndex2] = 0;
                    /****************/
                    /* szScanRight  */
                    /****************/
                    ScanIndex2 = 0;
                    ScanIndex1 = ScanIndex1 + strlen (szScanTest);
                    do  {
                        szScanRight [ScanIndex2] = szScanStr [ScanIndex1];
                        ScanIndex2++;
                        if  (szScanStr [ScanIndex1] != 0) {
                            ScanIndex1++;
                        }
                    }
                    while (szScanStr [ScanIndex1] != 0);
                    szScanRight [ScanIndex2] = 0;
                }
            }
            else {
                if  ((strlen(szScanStr) - ScanIndex1) > (strlen (szScanTest)-1)) {
                    ScanIndex1++;
                    cScanDone = 'n';
                }
                else {
                    cScanDone = 'y';
                }
            }
        }
        while (cScanDone != 'y');
    }
    if  (cScanFromRight == 'y') {
        z910_scan_reverse ();
    }

    strcpy (szScanTest, szScanKeep);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Scan for a string and set up left and right strings.         */
/********************************************************************/
void z905_scramble () {
    
    int i1;
    int i2;
    int i3;
    int iEndLoc;
    int iLenTest = (int)strlen(szScanTest);
    int iLenStr  = (int)strlen(szScanStr);
    int iOK;
    i1 = 0;
    //printf ("z905_scramble()\n");
    do {
        strcpy (szScanTest, szScanKeep);
        iEndLoc = i1 + iLenTest - 1;
        i2 = i1;
        do {
            i3 = 0;
            iOK = 0;
            do  {
                cScanChar1 = szScanStr[i2];
                cScanChar2 = szScanTest[i3];
                if  (cCase == 'n') {
                    cScanChar1 = (unsigned char) toupper (cScanChar1);
                    if  (szScanTest[i3] != 0) {
                        cScanChar2 = (unsigned char) toupper (cScanChar2);
                    }
                }
                if  (cScanChar1 == cScanChar2) {
                    szScanTest[i3] = 0;
                    iOK = 1;
                    break;
                }
                i3++;
            }
            while (i3 < iLenTest);
            i2++;
            if (!iOK) {
               break;
            }
        }
        while (i2 <= iEndLoc);
        if  (iOK) {
            cScanDone = 'y';
            cScanFound = 'y';
            printf ("Found\n");
            break;
        }
        i1++;
        if  ((i1 + iLenTest) > iLenStr) {
            cScanDone = 'y';
            cScanFound = 'n';
            break;
        }
    }
    while (cScanDone != 'y');
    strcpy (szScanTest, szScanKeep);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Reverse the Scan Strings to allow scanning from the right.   */
/********************************************************************/
void z910_scan_reverse () {
    strrev (szScanStr);
    strrev (szScanTest);
    strrev (szScanLeft);
    strrev (szScanMid);
    strrev (szScanRight);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Execute a WinExec and wait for it to complete                */
/********************************************************************/
void z998_winexec (char *szCmd) {
    FILE *BEDITbat;
    FILE *BEDITSem;
    HFILE hfFile;
    struct find_t ffblk;
    int FileFound;
    MSG lpMsg;

    /***************************************************/
    /* Create the .bat file                            */
    /***************************************************/
    for (;;) {
        BEDITbat = fopen ("BEDITexec.bat", "w");
        if  (BEDITbat) {
            break;
        }
    }
    fprintf (BEDITbat, "ECHO OFF\n");
    fprintf (BEDITbat, "%s\n", szCmd);
    fprintf (BEDITbat, "%s\n", "echo DONE > BEDITexec.sem");
    fprintf (BEDITbat, "exit\n");
    fclose (BEDITbat);
    remove ("BEDITexec.sem");
    if  (ExecFullScreen) {
        WinExec ("BEDITexec.bat", SW_HIDE);
    }
    else {
        WinExec ("BEDITexec.bat", SW_HIDE);
    }
    for (;;) {
        //printf ("waiting for BEDITexec.sem\n");
        Yield();

        BEDITSem = fopen ("BEDITexec.sem", "r"); 
        if  (BEDITSem) {
            //printf ("BEDITexec.sem done\n");
	    fclose (BEDITSem);
            //printf ("Removing BEDITexec.bat\n");
            //remove ("BEDITexec.bat");
            //printf ("Removing BEDITexec.sem\n");
            //remove ("BEDITexec.sem");
            //printf ("Done with System command\n");
	    break;
	}       
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Open the BEDIT Debug File                                     */
/********************************************************************/
void z999_open_bug () {
    if  ((strcmp (name_debug_out, "NULL") == 0) ||
         (strcmp (name_debug_out, "null") == 0)) {
        return;
    }
    if  (!file_debug_out) {
        file_debug_out = fopen (name_debug_out, "w");
        if  (!file_debug_out) {
            printf ("%s", "Bad Open Debug LOG File %s!\n", name_debug_out);
            if  (Beditwin == 'y') {
//                USHORT flStyle;
//                USHORT idWindow;
//
//                idWindow = MB_ICONHAND;
//                WinAlarm (HWND_DESKTOP, WA_NOTE);
//                flStyle = MB_OK;
//
//                sprintf (temp, "Bad open Log File %s", name_debug_out);
//
//                WinMessageBox (HWND_DESKTOP,
//                               hwndMain,
//                               temp,
//                               "BEDIT(PM) Abort",
//                               0,
//                               flStyle | idWindow);
//                if  (BEDITframe) {
//                    WinDestroyWindow(BEDITframe);
//                }
//                if  (hmq) {
//                    WinDestroyMsgQueue(hmq);
//                }
//                if  (hab) {
//                    WinTerminate(hab);
//                }
//                exit (1);
            }
        }
        fputs ("*******************************************************************\n", file_debug_out);
        fputs ("*                            BEDIT v09112013                      *\n", file_debug_out);
        fputs ("*                            Forrest Bentley                      *\n", file_debug_out);
        fputs ("*******************************************************************\n", file_debug_out);
    }
    return;
}

/************************************************************************/
/*                                                                      */
/*                         End of program BEDIT3.c                      */
/*                                                                      */
/************************************************************************/
/* Formatted on 09/21/90 at 14:24:23 by Forrest Bentley's C-Format v1.4 */
