/*************************************************************************/
/* Program          : BEDIT2.c                   v1.4                    */
/* Author           : Forrest Bentley                                    */
/* Date Written     : January 15, 1992                                   */
/*                                                                       */
/* Description      :                                                    */
/*     This program will process all commands in an input command file.  */
/*                                                                       */
/*  Forrest Bentley   05-06-1992 Split BEDIT.C into BEDIT.C & BEDIT2.C   */
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
#include <errno.h>
#include <direct.h>
#include <signal.h>

#include "BEDIT.h"
#include "BEDITvar.h"

/********************************************************************/
/* This module will :                                               */
/*     process the READ command                                     */
/********************************************************************/
void d100_process_read () {
//    printf ("%s", "d100_process_read\n");

    char cAlreadyOpen;

    ReadCount = 0;
    MemCount = 0;
    z300_store_file_position ();
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
    }
    strcpy (szFileName, Command);
    found2 = '~';
    c120_parse_command ();
    found2 = 'n';
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
    }
    /*****************************************/
    /* See if the file is already open       */
    /*****************************************/
    strcpy (szFileString, Command);
    cAlreadyOpen = 'n';
    pOpenFile = &StartOpenFile;
    do  {
        if  (pOpenFile->pNext) {
            pOpenFile = pOpenFile->pNext;
            if  (strcmp (pOpenFile->szFileName, szFileName) == 0) {
                cAlreadyOpen = 'y';
                pNewOpenFile = pOpenFile;
            }
        }
    }
    while ((pOpenFile->pNext) && (cAlreadyOpen != 'y'));
    /*****************************************/
    /* Allocate a new file                   */
    /*****************************************/
    if  (cAlreadyOpen != 'y') {
        pNewOpenFile = (OPENFILE *) malloc (sizeof (OPENFILE));
        if  (!pNewOpenFile) {
            strcpy (Error, "Bad Open");
            z110_fatal ();
            return;
        }
        memset (pNewOpenFile, 0, sizeof (OPENFILE));
    }
    /*****************************************/
    /* open the file                         */
    /*****************************************/
    strcpy (pNewOpenFile->szFile, szFileString);
    strcpy (pNewOpenFile->szFileName, szFileName);
    if  ((strcmp (szFileString, "NULL") != 0) &&
         (strcmp (szFileString, "null") != 0)) {
        pNewOpenFile->File = fopen (pNewOpenFile->szFile, "r");
        if  (!pNewOpenFile->File) {
            d101_find_file ();
        }

        if  (!pNewOpenFile->File) {
            /**************************/
            /* Retry                  */
            /**************************/
            Sleep ((DWORD)200);
            pNewOpenFile->File = fopen (pNewOpenFile->szFile, "r");
        }

        if  (!pNewOpenFile->File) {
            free (pNewOpenFile);
            pOpenFile = NULL;
            pDataFile = NULL;
            strcpy (Error, "Bad Open");
            z110_fatal ();
            pOpenFile = pOpenCurr;
            return;
        }
    }
    /*****************************************/
    /* prepare the data structure for data   */
    /*****************************************/
    if  (cAlreadyOpen != 'y') {
        pNewOpenFile->pNext = NULL;
        pNewOpenFile->pPrev = pOpenFile;
        pOpenFile->pNext = pNewOpenFile;

        pNewOpenFile->pData = (DATAFILE *) malloc (sizeof (DATAFILE));
        if  (!pNewOpenFile->pData) {
            strcpy (Error, "Bad Read");
            z110_fatal ();
            return;
        }
        memset (pNewOpenFile->pData, 0, sizeof (DATAFILE));

        pDataFile = pNewOpenFile->pData;
        pDataFile->szLine = (char *) malloc (20);
        if  (!pDataFile->szLine) {
            strcpy (Error, "Bad Read 1");
            z110_fatal ();
            return;
        }
        strcpy (pDataFile->szLine, "");
        pDataFile->pNext = NULL;
        pDataFile->pPrev = NULL;
        pDataFile->Begin = 'y';
        if  ((strcmp (szFileString, "NULL") == 0) ||
             (strcmp (szFileString, "null") == 0)) {
            pOpenFile = pNewOpenFile;
            return;
        }
    }
    else {
        pDataFile = pNewOpenFile->pData;
        do  {
            if  (pDataFile->pNext) {
                pDataFile = pDataFile->pNext;
            }
        }
        while (pDataFile->pNext);
        if  ((strcmp (szFileString, "NULL") == 0) ||
             (strcmp (szFileString, "null") == 0)) {
            pOpenFile = pNewOpenFile;
            return;
        }
    }

    /*****************************************/
    /* read the file into the data structure */
    /*****************************************/
    eof_file = 'n';
    do  {
        file_status = fgets (szHoldBuff, Max-3, pNewOpenFile->File);
        if  (file_status == NULL) {
            eof_file = 'y';
        }
        else {
            szHoldBuff [Max - 1] = 0;
            if  ((int) strlen (szHoldBuff) > Max - 5) {
                sprintf (Error, "Max=%d Size=%d\n", Max, strlen(szHoldBuff));
                printf (Error);
                strcpy (Error, "Bad Read, Large Rec");
                z110_fatal ();
                eof_file = 'y';
                //fclose (pNewOpenFile->File);
                //return;
            }
            else {
                ReadCount++;
                MemCount+=strlen (szHoldBuff);
//                sprintf (temp, "Line %d Mem %d\n", ReadCount, MemCount);
//                printf ("%s", temp);
//               printf ("%s", szHoldBuff);
                pNewDataFile = (DATAFILE *) malloc (sizeof (DATAFILE));
                if  (!pNewDataFile) {
                        printf ("%s", "Not Enough Memory to read File\n");
                    sprintf (buff, "Line Number %d\n", ReadCount);
                    printf ("%s", buff);
                    strcpy (Error, "Bad Read");
                    z110_fatal ();
                    return;
                }
                memset (pNewDataFile, 0, sizeof (DATAFILE));
                pNewDataFile->szLine = (char *) malloc (strlen (szHoldBuff) + 5);
                if  (!pNewDataFile->szLine) {
                    printf ("%s", "Not Enough Memory to read File Line\n");
                    sprintf (buff, "Line Number %d\n", ReadCount);
                    printf ("%s", buff);
                    strcpy (Error, "Bad Read");
                    z110_fatal ();
                    return;
                }
                //*************************************
                //* Add CR to any line missing CR
                //*************************************
                if  (strlen(szHoldBuff) > 0) {
                    if  ((szHoldBuff[strlen(szHoldBuff) - 1] != 10) && (szHoldBuff[strlen(szHoldBuff) - 1] != 13))  {
                        strcat (szHoldBuff, "\n");
                    }
                }
                strcpy (pNewDataFile->szLine, szHoldBuff);
                pDataFile->pNext = pNewDataFile;
                pNewDataFile->pNext = NULL;
                pNewDataFile->pPrev = pDataFile;
                pNewDataFile->Begin = 'n';
                pDataFile = pNewDataFile;
            }
        }
    }
    while (eof_file != 'y');
    pDataFile = pNewOpenFile->pData;
    if  (pDataFile->pNext) {
        pDataFile = pDataFile->pNext;
        pNewOpenFile->pData = pDataFile;
    }

    pOpenFile = pNewOpenFile;
    fclose (pOpenFile->File);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the FILESIZE Command                                 */
/********************************************************************/
void d101_find_file () {
    int tx1;
    tx1 = 0;
    do  {
        if  (pNewOpenFile->szFile [tx1] == '\\') {
            return;
        }
        tx1++;
    }
    while (pNewOpenFile->szFile [tx1] > 0);

    strcpy (szScanRight, envstr);
    cScanFound = 'y';
    do  {
        strcpy (szScanStr, szScanRight);
        strcpy (szScanTest, ";");
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (buff, szScanLeft);
            strcat (buff, "\\");
            strcat (buff, pNewOpenFile->szFile);
            pNewOpenFile->File = fopen (buff, "r");
            if  (pNewOpenFile->File) {
                if  (strlen (buff) < 100) {
                    strcpy (pNewOpenFile->szFile, buff);
                }
                return;
            }
        }    
    }
    while (cScanFound == 'y');
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the FILESIZE Command                                 */
/********************************************************************/
void d105_filesize () {
    c120_parse_command ();
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ";");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    /*******************************/
    /* Get the File Name           */
    /*******************************/
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
    if  (strcmp (szScanLeft, "*") == 0) {
        if  (!pOpenFile) {
            strcpy (Error, "No Curr File");
            z110_fatal ();
            return;
        }
        strcpy (szScanLeft, pOpenFile->szFileName);
    }
    strcpy (szIf1, szScanLeft); /* Temporary Storage to Hold File Name */

    strcpy (szScanStr, szScanRight);
    strcpy (szScanTest, ";");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }

    /*******************************/
    /* Get the File Start Pos      */
    /*******************************/
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
    Number = 0;
    if  ((strcmp (szScanLeft, "VAR") == 0) ||
         (strcmp (szScanLeft, "var") == 0)) {
        Number2 = 0;
    }
    else {
        Number2 = (long int) strtol ((char*)szScanLeft, (char**)&p, 10);
        if  ((Number2 < 1) || ((int)Number2 > Max)) {
            strcpy (Error, "Invalid Begin");
            z110_fatal ();
            return;
        }
    }

    /*******************************/
    /* Get the File End Pos        */
    /*******************************/
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
    Number = 0;
    if  ((strcmp (szScanRight, "VAR") == 0) ||
         (strcmp (szScanRight, "var") == 0)) {
        Number = 0;
    }
    else {
        Number = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
        if  ((Number < 1) || ((int)Number > Max) ||
             ((Number != 0) && (Number < Number2))) {
            strcpy (Error, "Invalid End");
            z110_fatal ();
            return;
        }
    }

    /*******************************/
    /* Find the File               */
    /*******************************/
    z300_store_file_position ();
    strcpy (szFileName, szIf1); /* Temporary Storage to Hold File Name */
    z200_reset_file_position ();
    if  (!pOpenFile) {
        strcpy (Error, "File Not Found");
        z110_fatal ();
        pOpenFile = pOpenCurr;
        return;
    }
    if  (!pDataFile) {
        strcpy (Error, "File Not Found");
        z110_fatal ();
        pOpenFile = pOpenCurr;
        return;
    }
    /*******************************/
    /* Move to the top of the File */
    /*******************************/
    do  {
        if  (pDataFile->pPrev) {
            pDataFile = pDataFile->pPrev;
        }
    }
    while (pDataFile->pPrev);
    /*******************************/
    /* Size Each Line in the File  */
    /*******************************/
    do  {
        if  (pDataFile->pNext) {
            pDataFile = pDataFile->pNext;
            memset (temp, ' ', (int) Number);
            strcpy (temp, pDataFile->szLine);
            free (pDataFile->szLine);
            if  (Number == 0) {
                if  ((long) strlen (temp) < Number2) {
                    strcpy (temp, " ");
                }
                else {
                    strcpy (buff, temp + (Number2 - 1));
                    strcpy (temp, buff);
                }
                temp [strlen (temp) - 1] = 0;
                index4 = strlen (temp);
                if  (index4 > 0) {
                    do  {
                        index4--;
                        if  (temp [index4] == ' ') {
                            temp [index4] = 0;
                        }
                        else {
                            index4 = 0;
                        }
                    }
                    while (index4 != 0);
                }
                strcat (temp, retn);
            }
            else {
                temp [strlen (temp) - 1] = 0;
                temp [strlen (temp) + 1] = ' ';
                if  (Number > (long) strlen (temp)) {
                    temp [strlen (temp)] = ' ';
                }
                temp [Number] = 0;
                strcpy (buff, temp + (Number2 - 1));
                strcpy (temp, buff);
                strcat (temp, retn);
            }
            pDataFile->szLine = (char *) malloc (strlen (temp) + 5);
            if  (!pDataFile->szLine) {
                strcpy (Error, "Bad Resize");
                z110_fatal ();
                return;
            }
            strcpy (pDataFile->szLine, temp);
        }
    }
    while (pDataFile->pNext);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the filestring command                               */
/********************************************************************/
void d106_filestring () {

    OPENFILE *pTestFile;

    /********************************************/
    /* Find the Open File                       */
    /********************************************/
    c120_parse_command ();
    pTestFile = &StartOpenFile;
    do  {
        if  (pTestFile->pNext) {
            pTestFile = pTestFile->pNext;
        }
    }
    while ((pTestFile->pNext) && (strcmp (pTestFile->szFileName, Command) != 0));
    if  (strcmp (pTestFile->szFileName, Command) == 0) {
        strcpy (buff, pTestFile->szFile);
    }
    else {
        strcpy (buff, "");
    }

    /********************************************/
    /* Find the Variable                        */
    /********************************************/
    c120_parse_command ();
    d580_find_variable ();

    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use str");
        z110_fatal ();
        return;
    }
    free (pVariable->szString);
    pVariable->szString = (char *) malloc (strlen (buff) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad Update!");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, buff);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the WRITE command                                    */
/********************************************************************/
void d110_process_write () {
//    printf ("%s", "d110_process_write\n");
    z300_store_file_position ();
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
    }
    if  (strcmp (Command, "*") == 0) {
        if  (!pOpenFile) {
            strcpy (Error, "No Curr File");
            z110_fatal ();
            return;
        }
        strcpy (Command, pOpenFile->szFileName);
    }
    strcpy (szFileName, Command);
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
    }
    if  (strcmp (Command, "*") == 0) {
        if  (!pOpenFile) {
            strcpy (Error, "No Curr File");
            z110_fatal ();
            return;
        }
        strcpy (Command, pOpenFile->szFile);
    }
    strcpy (szFileString, Command);
    file_ot = fopen (szFileString, "r");
    if  (file_ot) {
        fclose (file_ot);
        strcpy (szScanStr, szFileString);
        strcpy (szScanTest, ".");
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (temp, szScanLeft);
        }
        else {
            strcpy (temp, szFileString);
        }
        strcat (temp, ".BAK");
        strcpy (buff, "copy ");
        strcat (buff, szFileString);
        strcat (buff, " ");
        strcat (buff, temp);
        strcat (buff, " >NULL");
//        system (buff);
//        system ("del null");
//        z998_winexec (buff);
//        z998_winexec ("del null");
    }

    z200_reset_file_position ();
    Number = 999999;
    f110_prev ();
    file_ot = fopen (szFileString, "w");
    if  (!file_ot) {
        strcpy (Error, "Bad Write");
        z110_fatal ();
        return;
    }
    done = 'n';
    do  {
        fputs (pDataFile->szLine, file_ot);
        if  (pDataFile->pNext) {
            pDataFile = pDataFile->pNext;
        }
        else {
            done = 'y';
        }
    }
    while (done != 'y');
    done = 'n';
    Number = 999999;
    f110_prev ();
    if  (file_ot)
        fclose (file_ot);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the FILE command                                     */
/********************************************************************/
void d112_process_file () {
//    printf ("%s", "d112_process_file\n");
    z300_store_file_position ();
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            FileError = 'y';
            z110_fatal ();
            return;
        }
    }
    strcpy (szFileName, Command);
    z200_reset_file_position ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the CLOSE command                                    */
/********************************************************************/
void d113_close_file () {
//    printf ("%s", "d113_close_file\n");

    FileError = 'n';
    d112_process_file ();
    if  (FileError == 'y') {
        return;
    }
    pPrevFile = pOpenFile->pPrev;
    pNextFile = pOpenFile->pNext;
    /*******************************/
    /* Move to the top of the file */
    /*******************************/
    pDataFile = pOpenFile->pData;
    if  (pDataFile->pPrev) {
        Number = 999999;
        f110_prev ();
    }
    /*******************************/
    /* Remove the first line       */
    /*******************************/
    pPrevData = pDataFile->pPrev;
    if  (pPrevData) {
        free (pPrevData->szLine);
        free (pPrevData);
    }
    /*******************************/
    /* Remove all other lines      */
	 /*******************************/

    do  {
        pNextData = pDataFile->pNext;
		  free (pDataFile->szLine);
        free (pDataFile);
        pDataFile = pNextData;
    }
    while (pDataFile);
    /*******************************/
    /* Remove the file             */
    /*******************************/
    free (pOpenFile);
    if  (pNextFile) {
        pPrevFile->pNext = pNextFile;
        pNextFile->pPrev = pPrevFile;
    }
    else {
        pPrevFile->pNext = NULL;
    }
    pOpenFile = &StartOpenFile;
    if  (pOpenFile->pNext) {
        pOpenFile = pOpenFile->pNext;
        pDataFile = pOpenFile->pData;
    }
    else {
        pDataFile = NULL;
        pOpenFile = NULL;
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the GOTO command                                     */
/********************************************************************/
void d114_goto () {
//    printf ("%s", "d114_goto\n");

    if  (Call != 'y') {
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
    }
    Call = 'n';
    pHoldCommand = pCommand;
    GotoIndex1 = index1;
    if  (Command [0] != '!') {
        strcpy (Error, "Invalid Label");
        z110_fatal ();
        return;
    }
    strcpy (Label, Command);
    index8 = 0;
    do  {
        if  (Label [index8] < 33) {
            Label [index8] = 0;
        }
        else {
            index8++;
        }
    }
    while (Label [index8] != 0);

    /**************************************************/
    /* Find Last Used Call/Goto for this command line */
    /**************************************************/
    if  (pCommand->pLastCallGoto) {
        pCommand = pCommand->pLastCallGoto;
        strcpy (input_line, pCommand->szLine);
        if  (strcmp (Label, input_line) == 0) {
            if  (Debug == 'y') {
                z999_open_bug ();
                sprintf (buff, "$$$Debug Line %d [%s]\n", pCommand->LineNo, input_line);
                fputs (buff, file_debug_out);
            }
            More_Commands = 'n';
            if  (pLastCallGoto) {
                pLastCallGoto->pLastCallGoto = pCommand;
            }
            return;
        }
    }

    /**************************************************/
    /* Find the first matching label in command file  */
    /**************************************************/
    pCommand = pStartCommand;
    do  {
        if  (pCommand->pNextLabel) {
            pCommand = pCommand->pNextLabel;
            strcpy (input_line, pCommand->szLine);
//            printf ("Label = [%s]  Check = [%s]\n", Label, input_line);
            if  (strcmp (Label, input_line) == 0) {
                if  (Debug == 'y') {
                    z999_open_bug ();
                    sprintf (buff, "$$$Debug Line %d [%s]\n", pCommand->LineNo, input_line);
                    fputs (buff, file_debug_out);
                }
                More_Commands = 'n';
                if  (pLastCallGoto) {
                    pLastCallGoto->pLastCallGoto = pCommand;
                }
                return;
            }
        }
    }
    while (pCommand->pNextLabel);
    if  (strcmp (Label, "!BREAK") == 0) {
        exit (1);
    }
    strcpy (Error, "Label Not Found");
    index1 = GotoIndex1;
    pCommand = pHoldCommand;
    if  (Callfile == 'y') {
        d410_return ();
    }
    z110_fatal ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the PRINT command                                    */
/********************************************************************/
void d115_process_print () {
//    printf ("%s", "d115_process_print\n");
    if  (Printx == 'y') {
        strcpy (szConvString, Command + 8);
        szConvString [strlen (szConvString) - 1] = 0;
        g200_convert_string ();
        cprintf ("%s", szConvString);
        if  ((Debug == 'y') || (Beditwin == 'y')) {
            z999_open_bug ();
            fputs (szConvString, file_debug_out);
            fputs ("\n", file_debug_out);
        }
    }
    else {
        strcpy (szConvString, Command + 7);
        szConvString [strlen (szConvString) - 1] = 0;
        g200_convert_string ();
        cprintf ("%s", szConvString);
        printf ("%s", "\n");
        if  ((Debug == 'y') || (Beditwin == 'y')) {
            z999_open_bug ();
            fputs (szConvString, file_debug_out);
            fputs ("\n", file_debug_out);
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the IF command                                       */
/********************************************************************/
void d116_if () {
//    printf ("%s", "d116_if\n");
    double dNum1;
    double dNum2;
    char   *cp;

    Delimit = 'y';
    c120_parse_command ();
    Command [strlen (Command) - 1] = 0;
    strcpy (buff, Command + 1);
    /***********************************/
    /* Check for OR condition          */
    /***********************************/
    Or_Cond = 'n';
    if  (((input_line [index1] == 'O') || (input_line [index1] == 'o')) &&
         ((input_line [index1 + 1] == 'R') || (input_line [index1 + 1] == 'r'))) {
        Or_Cond = 'y';
    }

    /***********************************/
    /* if (name)                       */
    /***********************************/
    test = 'y';
    strcpy (Command, buff);
    d580_find_variable ();
    test = 'n';
    if  (found == 'y') {
        if  (pVariable->Type == 'S') {
            if  ((strcmp (pVariable->szString, "Y") == 0) ||
                 (strcmp (pVariable->szString, "y") == 0) ||
                 (strcmp (pVariable->szString, "YES") == 0) ||
                 (strcmp (pVariable->szString, "yes") == 0) ||
                 (strcmp (pVariable->szString, "OK") == 0) ||
                 (strcmp (pVariable->szString, "ok") == 0) ||
                 (strcmp (pVariable->szString, "T") == 0) ||
                 (strcmp (pVariable->szString, "t") == 0) ||
                 (strcmp (pVariable->szString, "TRUE") == 0) ||
                 (strcmp (pVariable->szString, "true") == 0)) {
                return;
            }
            else {
                More_Commands = 'n';
                return;
            }
        }
        else {
            if  (pVariable->Integer == 0) {
                More_Commands = 'n';
                return;
            }
            else {
                return;
            }
        }
    }

    /***********************************/
    /* if (!name)                      */
    /***********************************/
    if  (buff [0] == '!') {
        test = 'y';
        strcpy (Command, buff + 1);
        d580_find_variable ();
        test = 'n';
        if  (found == 'y') {
            if  (pVariable->Type == 'S') {
                if  ((strcmp (pVariable->szString, "Y") == 0) ||
                     (strcmp (pVariable->szString, "y") == 0) ||
                     (strcmp (pVariable->szString, "YES") == 0) ||
                     (strcmp (pVariable->szString, "yes") == 0) ||
                     (strcmp (pVariable->szString, "T") == 0) ||
                     (strcmp (pVariable->szString, "t") == 0) ||
                     (strcmp (pVariable->szString, "TRUE") == 0) ||
                     (strcmp (pVariable->szString, "true") == 0)) {
                    More_Commands = 'n';
                    return;
                }
                else {
                    return;
                }
            }
            else {
                if  (pVariable->Integer == 0) {
                    return;
                }
                else {
                    More_Commands = 'n';
                    return;
                }
            }
        }
    }

    /***********************************/
    /* if (EOF)                        */
    /***********************************/
    if  ((strcmp (buff, "EOF") == 0) ||
         (strcmp (buff, "eof") == 0)) {
        if  (EOF_Flag != 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (!EOF)                       */
    /***********************************/
    if  ((strcmp (buff, "NOTEOF") == 0) ||
         (strcmp (buff, "noteof") == 0) ||
         (strcmp (buff, "!EOF") == 0) ||
         (strcmp (buff, "!eof") == 0)) {
        if  (EOF_Flag == 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (PM)                         */
    /***********************************/
    if  ((strcmp (buff, "PM") == 0) ||
         (strcmp (buff, "pm") == 0)) {
        if  (Beditwin != 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (!PM)                        */
    /***********************************/
    if  ((strcmp (buff, "!PM") == 0) ||
         (strcmp (buff, "!pm") == 0)) {
        if  (Beditwin == 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (WIN)                        */
    /***********************************/
    if  ((strcmp (buff, "WIN") == 0) ||
         (strcmp (buff, "win") == 0)) {
        return;
    }
 
    /***********************************/
    /* if (!WIN)                       */
    /***********************************/
    if  ((strcmp (buff, "!WIN") == 0) ||
         (strcmp (buff, "!win") == 0)) {
        More_Commands = 'n';
        return;
    }

    /***********************************/
    /* if (KEY)                        */
    /***********************************/
    if  ((strcmp (buff, "KEY") == 0) ||
         (strcmp (buff, "key") == 0)) {
        if  (!kbhit()) {
            More_Commands = 'n';
            return;
        }
        else {
//            getch ();
            return;
        }
    }

    /***********************************/
    /* if (!KEY)                       */
    /***********************************/
    if  ((strcmp (buff, "NOKEY") == 0) ||
         (strcmp (buff, "nokey") == 0) ||
         (strcmp (buff, "!KEY") == 0) ||
         (strcmp (buff, "!key") == 0)) {
        if  (kbhit()) {
            //getch ();
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (MASK Var Maskvar)           */
    /* if (!MASK Var Maskvar)          */
    /***********************************/
    if  ((memcmp (buff, "MASK", 4) == 0) ||
         (memcmp (buff, "mask", 4) == 0) ||
         (memcmp (buff, "!MASK", 5) == 0) ||
         (memcmp (buff, "!mask", 5) == 0)) {

        int     Mask1;
        char    szStatus [20];
        char    cNot;

        cNot = 'n';
        if  (buff [0] == '!') {
            cNot = 'y';
        }

        if  (strlen (buff) < 8) {
            z010_syntax_error ();
            return;
        }
        strcpy (szScanStr, buff);
        strcpy (szScanTest, " ");
        z900_scan ();
        if  (cScanFound != 'y') {
            z010_syntax_error ();
            return;
        }
        strcpy (szScanStr, szScanRight);
        strcpy (szScanTest, " ");
        z900_scan ();
        if  (cScanFound != 'y') {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (buff, pVariable->szString);
        strcpy (Command, szScanRight);
        d580_find_variable ();
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
        strcpy (szStatus, "OK");
        if  (strlen (buff) > strlen (pVariable->szString)) {
            strcpy (buff, "");
            strcpy (szStatus, "BAD");
        }
        Mask1 = 0;
        do  {
            if  (buff [Mask1] != 0) {
                if  (pVariable->szString [Mask1] == Pattany) {
                    buff [Mask1] = pVariable->szString [Mask1];
                }
                if  (pVariable->szString [Mask1] == Pattnum) {
                    if  (isdigit (buff [Mask1])) {
                        buff [Mask1] = pVariable->szString [Mask1];
                    }
                    else {
                        buff [Mask1] = 255;
                    }
                }
                if  (pVariable->szString [Mask1] == Pattalp) {
                    if  ((isalpha (buff [Mask1])) ||
                         (buff [Mask1] == ' ')) {
                        buff [Mask1] = pVariable->szString [Mask1];
                    }
                    else {
                        buff [Mask1] = 255;
                    }
                }
                if  (pVariable->szString [Mask1] == Pattspc) {
                    if  ((!isdigit (buff [Mask1])) &&
                         (!isalpha (buff [Mask1]))) {
                        buff [Mask1] = pVariable->szString [Mask1];
                    }
                    else {
                        buff [Mask1] = 255;
                    }
                }
                if  (pVariable->szString [Mask1] != buff [Mask1]) {
                    strcpy (szStatus, "BAD");
                }
                Mask1++;
            }
        }
        while (buff [Mask1] != 0);
        if  (strcmp (szStatus, "BAD") == 0) {
            if  (cNot == 'n') {
                More_Commands = 'n';
            }
        }
        else {
            if  (cNot == 'y') {
                More_Commands = 'n';
            }
        }

        return;
    }

    /***********************************/
    /* if (NUMERIC name)               */
    /***********************************/
    strcpy (temp, buff);
    temp [7] = 0;
    if  ((strcmp (temp, "NUMERIC") == 0) ||
         (strcmp (temp, "numeric") == 0)) {
        if  (strlen (buff) < 9) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 8);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  ((!isdigit (pVariable->szString [x1])) &&
                 (pVariable->szString [x1] != '-') &&
                 (pVariable->szString [x1] != '+') &&
                 (pVariable->szString [x1] != '.')) {
                x2 = 1;
            }
        }
        if  (x2 != 0) {
            More_Commands = 'n';
        }
        return;
    }

    /***********************************/
    /* if (!NUMERIC name)              */
    /***********************************/
    strcpy (temp, buff);
    temp [8] = 0;
    if  ((strcmp (temp, "!NUMERIC") == 0) ||
         (strcmp (temp, "!numeric") == 0)) {
        if  (strlen (buff) < 10) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 9);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  ((!isdigit (pVariable->szString [x1])) &&
                 (pVariable->szString [x1] != '-') &&
                 (pVariable->szString [x1] != '+') &&
                 (pVariable->szString [x1] != '.')) {
                x2 = 1;
            }
        }
        if  (x2 == 0) {
            More_Commands = 'n';
        }
        return;
    }

    /***********************************/
    /* if (DIGIT name)                 */
    /***********************************/
    strcpy (temp, buff);
    temp [5] = 0;
    if  ((strcmp (temp, "DIGIT") == 0) ||
         (strcmp (temp, "digit") == 0)) {
        if  (strlen (buff) < 7) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 6);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  (!isdigit (pVariable->szString [x1])) {
                x2 = 1;
            }
        }
        if  (x2 != 0) {
            More_Commands = 'n';
        }
        return;
    }

    /***********************************/
    /* if (!DIGIT name)                */
    /***********************************/
    strcpy (temp, buff);
    temp [6] = 0;
    if  ((strcmp (temp, "!DIGIT") == 0) ||
         (strcmp (temp, "!digit") == 0)) {
        if  (strlen (buff) < 8) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 7);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  (!isdigit (pVariable->szString [x1])) {
                x2 = 1;
            }
        }
        if  (x2 == 0) {
            More_Commands = 'n';
        }
        return;
    }

    /***********************************/
    /* if (ALPHA name)                 */
    /***********************************/
    strcpy (temp, buff);
    temp [5] = 0;
    if  ((strcmp (temp, "ALPHA") == 0) ||
         (strcmp (temp, "alpha") == 0)) {
        if  (strlen (buff) < 7) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 6);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  (!isalpha (pVariable->szString [x1])) {
                x2 = 1;
            }
        }
        if  (x2 != 0) {
            More_Commands = 'n';
        }
        return;
    }

    /***********************************/
    /* if (!ALPHA name)                */
    /***********************************/
    strcpy (temp, buff);
    temp [6] = 0;
    if  ((strcmp (temp, "!ALPHA") == 0) ||
         (strcmp (temp, "!alpha") == 0)) {
        if  (strlen (buff) < 8) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 7);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  (!isalpha (pVariable->szString [x1])) {
                x2 = 1;
            }
        }
        if  (x2 == 0) {
            More_Commands = 'n';
        }
        return;
    }

    /***********************************/
    /* if (SPECIAL name)               */
    /***********************************/
    strcpy (temp, buff);
    temp [7] = 0;
    if  ((strcmp (temp, "SPECIAL") == 0) ||
         (strcmp (temp, "special") == 0)) {
        if  (strlen (buff) < 9) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 8);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  ((isdigit (pVariable->szString [x1])) ||
                 (isalpha (pVariable->szString [x1]))) {
                x2 = 1;
            }
        }
        if  (x2 != 0) {
            More_Commands = 'n';
        }
        return;
    }

    /***********************************/
    /* if (!SPECIAL name)              */
    /***********************************/
    strcpy (temp, buff);
    temp [8] = 0;
    if  ((strcmp (temp, "!SPECIAL") == 0) ||
         (strcmp (temp, "!special") == 0)) {
        if  (strlen (buff) < 10) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 9);
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
        if  (pVariable->Type != 'S') {
            strcpy (Error, "Must Use Str");
            z110_fatal ();
            return;
        }
        x2 = 0;
        for (x1 = 0; x1 < (int) strlen (pVariable->szString); x1++) {
            if  ((isdigit (pVariable->szString [x1])) ||
                 (isalpha (pVariable->szString [x1]))) {
                x2 = 1;
            }
        }
        if  (x2 == 0) {
            More_Commands = 'n';
        }
        return;
    }


    /***********************************/
    /* if (VAREXISTS varname)          */
    /***********************************/
    strcpy (temp, buff);
    temp [9] = 0;
    if  ((strcmp (temp, "VAREXISTS") == 0) ||
         (strcmp (temp, "varexists") == 0)) {
        if  (strlen (buff) < 11) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 10);
        if  (Command [0] == Subs) {
            d580_find_variable ();
        }
        test = 'y';
        d580_find_variable ();
        test = 'n';
        if  (found != 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (!VAREXISTS varname)          */
    /***********************************/
    strcpy (temp, buff);
    temp [10] = 0;
    if  ((strcmp (temp, "!VAREXISTS") == 0) ||
         (strcmp (temp, "!varexists") == 0)) {
        if  (strlen (buff) < 12) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 11);
        if  (Command [0] == Subs) {
            d580_find_variable ();
        }
        test = 'y';
        d580_find_variable ();
        test = 'n';
        if  (found == 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (FILEOPEN filename)          */
    /***********************************/
    strcpy (temp, buff);
    temp [8] = 0;
    if  ((strcmp (temp, "FILEOPEN") == 0) ||
         (strcmp (temp, "fileopen") == 0)) {
        if  (strlen (buff) < 10) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 9);
        if  (Command [0] == Subs) {
            d580_find_variable ();
        }
        pCountFiles = &StartOpenFile;
        found = 'n';
        do  {
            if  (pCountFiles->pNext) {
                pCountFiles = pCountFiles->pNext;
                if  (strcmp (pCountFiles->szFileName, Command) == 0) {
                    found = 'y';
                }
            }
        }
        while ((pCountFiles->pNext) && (found != 'y'));
        if  (found != 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }

    }

    /***********************************/
    /* if (!FILEOPEN filename)          */
    /***********************************/
    strcpy (temp, buff);
    temp [9] = 0;
    if  ((strcmp (temp, "!FILEOPEN") == 0) ||
         (strcmp (temp, "!fileopen") == 0)) {
        if  (strlen (buff) < 11) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 10);
        if  (Command [0] == Subs) {
            d580_find_variable ();
        }
        pCountFiles = &StartOpenFile;
        found = 'n';
        do  {
            if  (pCountFiles->pNext) {
                pCountFiles = pCountFiles->pNext;
                if  (strcmp (pCountFiles->szFileName, Command) == 0) {
                    found = 'y';
                }
            }
        }
        while ((pCountFiles->pNext) && (found != 'y'));
        if  (found == 'y') {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }

    }

    /***********************************/
    /* if (EXISTS Filename)            */
    /***********************************/
    strcpy (temp, buff);
    temp [6] = 0;
    if  ((strcmp (temp, "EXISTS") == 0) ||
         (strcmp (temp, "exists") == 0)) {
        if  (strlen (buff) < 8) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 7);
        if  (Command [0] == Subs) {
            d580_find_variable ();
        }
        mode = 0;
        RC = access (Command, mode);
        if  (RC != 0) {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }

    /***********************************/
    /* if (NOTEXISTS Filename)         */
    /***********************************/
    strcpy (temp, buff);
    temp [9] = 0;
    if  ((strcmp (temp, "NOTEXISTS") == 0) ||
         (strcmp (temp, "notexists") == 0)) {
        if  (strlen (buff) < 11) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 10);
        if  (Command [0] == Subs) {
            d580_find_variable ();
        }
        mode = 0;
        RC = access (Command, mode);
        if  (RC == 0) {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }
    /***********************************/
    /* if (!EXISTS Filename)           */
    /***********************************/
    strcpy (temp, buff);
    temp [7] = 0;
    if  ((strcmp (temp, "!EXISTS") == 0) ||
         (strcmp (temp, "!exists") == 0)) {
        if  (strlen (buff) < 9) {
            z010_syntax_error ();
            return;
        }
        strcpy (Command, buff + 8);
        if  (Command [0] == Subs) {
            d580_find_variable ();
        }
        mode = 0;
        RC = access (Command, mode);
        if  (RC == 0) {
            More_Commands = 'n';
            return;
        }
        else {
            return;
        }
    }
    /***********************************/
    /* if (Var1 = Var2)                */
    /* if (Var1 != Var2)               */
    /* if (Var1 > Var2)                */
    /* if (Var1 < Var2)                */
    /* if (Var1 <= Var2)               */
    /* if (Var1 >= Var2)               */
    /***********************************/
    /*****************/
    /* =, <, or >    */
    /*****************/
    strcpy (szScanTest, " = ");
    strcpy (szScanStr, buff);
    z900_scan ();
    if  (cScanFound == 'y') {
        strcpy (Math, "=");
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " > ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, ">");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " < ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, "<");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " <= ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, "<=");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " >= ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, ">=");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " != ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, "!=");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " BEGINSWITH ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, "beginswith");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " beginswith ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, "beginswith");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " ENDSWITH ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, "endswith");
        }
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, " endswith ");
        strcpy (szScanStr, buff);
        z900_scan ();
        if  (cScanFound == 'y') {
            strcpy (Math, "endswith");
        }
    }
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    /*****************/
    /* Var1          */
    /*****************/
    strcpy (szVar2, szScanRight);
    strcpy (Command, szScanLeft);
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
        strcpy (szIf1, pVariable->szString);
    }
    else {
        sprintf (szIf1, "%d", pVariable->Integer);
    }
    /*****************/
    /* Var2          */
    /*****************/
    if  ((szVar2 [0] == 45) ||
         ((szVar2 [0] > 47) && (szVar2 [0] < 58))) {
        /********************/
        /* Number           */
        /********************/
        strcpy (szIf2, szVar2);
    }
    else {
        if  (((szVar2 [0] > 64) && (szVar2 [0] < 91)) ||
             ((szVar2 [0] > 96) && (szVar2 [0] < 123)) ||
             (szVar2 [0] == Subs)) {
            /**************************/
            /* Variable               */
            /**************************/
            strcpy (Command, szVar2);
            strcpy (szScanStr, Command);
            strcpy (szScanTest, "[");
            z900_scan ();
            if  (cScanFound == 'y') {
                strcpy (szPartialVar, "partial2");
                d117_partial_var ();
            }
            else {
                d580_find_variable ();
            }
            if  (pVariable->Type == 'S') {
                strcpy (szIf2, pVariable->szString);
            }
            else {
                sprintf (szIf2, "%d", pVariable->Integer);
            }
        }
        else {
            /**************************/
            /* String                 */
            /**************************/
            strcpy (szIf2, szVar2 + 1);
            szIf2 [strlen (szIf2) - 1] = 0;
        }
    }
    /**********************************/
    /* Compare Var1 to Var2           */
    /**********************************/
    //Tnum1 = 0;
    Tnum1 = (long int) strtol ((char*)szIf1, (char**)&p, 10);
    //dNum1 = 0;
    dNum1 = strtod (szIf1, &cp);
    //Tnum2 = 0;
    Tnum2 = (long int) strtol ((char*)szIf2, (char**)&p, 10);
    //dNum2 = 0;
    dNum2 = strtod (szIf2, &cp);
    x2 = 0;
    for (x1 = 0; x1 < (int) strlen (szIf1); x1++) {
        if  ((!isdigit (szIf1 [x1])) &&
             (szIf1 [x1] != '-') &&
             (szIf1 [x1] != '+') &&
             (szIf1 [x1] != '.')) {
            x2 = 1;
        }
    }
    for (x1 = 0; x1 < (int) strlen (szIf2); x1++) {
        if  ((!isdigit (szIf2 [x1])) &&
             (szIf2 [x1] != '-') &&
             (szIf2 [x1] != '+') &&
             (szIf2 [x1] != '.')) {
            x2 = 1;
        }
    }
    if  ((dNum1 != 0) && (dNum2 != 0) && (x2 == 0)) {
        if  (dNum1 == dNum2) {
            Num1 = 0;
        }
        if  (dNum1 < dNum2) {
            Num1 = -1;
        }
        if  (dNum1 > dNum2) {
            Num1 = 1;
        }
    }
    else {
        Num1 = strcmp (szIf1, szIf2);
    }
    /******************/
    /* if (Var1=Var2) */
    /******************/
    if  (strcmp (Math, "=") == 0) {
        if  (Num1 == 0) {
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }
    /******************/
    /* if (Var1!=Var2)*/
    /******************/
    if  (strcmp (Math, "!=") == 0) {
        if  (Num1 != 0) {
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }

    /******************/
    /* if (Var1<Var2) */
    /******************/
    if  (strcmp (Math, "<") == 0) {
        if  (Num1 < 0) {
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }

    /******************/
    /* if (Var1>Var2) */
    /******************/
    if  (strcmp (Math, ">") == 0) {
        if  (Num1 > 0) {
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }
    /******************/
    /* if (Var1>=Var2)*/
    /******************/
    if  (strcmp (Math, ">=") == 0) {
        if  ((Num1 > 0) || (Num1 == 0)) {
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }
    /******************/
    /* if (Var1<=Var2)*/
    /******************/
    if  (strcmp (Math, "<=") == 0) {
        if  ((Num1 < 0) || (Num1 == 0)) {
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }
    /****************************/
    /* if (Var1 beginswith Var2)*/
    /****************************/
    if  (strcmp (Math, "beginswith") == 0) {
        if  (strlen(szIf1) < strlen(szIf2)) {
            More_Commands = 'n';
            return;
        }
        if  ((szIf1[0] == ' ') || (szIf1[0] == '\t')) {
            int x = 0;
            strcpy (buff, szIf1);
            do {
               x++;
            }
            while ((buff[x] == ' ') || (buff[x] == '\t'));
            strcpy (szIf1, buff+x);
        }
        if  (memcmp (szIf1, szIf2, strlen(szIf2)) == 0) {
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }
    /****************************/
    /* if (Var1 endswith Var2)*/
    /****************************/
    if  (strcmp (Math, "endswith") == 0) {
        int x;
        if  (strlen(szIf1) < strlen(szIf2)) {
            More_Commands = 'n';
            return;
        }
        x = strlen(szIf1) - 1;
        if (x == 0) {
            More_Commands = 'n';
            return;
        }
        do {
           if  ((szIf1[x] == ' ') || (szIf1[x] == '\t')) {
               szIf1[x] = 0;
               x--;
           }
        }
        while ((x > 0) && ((szIf1[x] == ' ') || (szIf1[x] == '\t')));
        if  (strlen(szIf1) < strlen(szIf2)) {
            More_Commands = 'n';
            return;
        }
        if  (memcmp (szIf1 + strlen(szIf1) - strlen(szIf2), szIf2, strlen(szIf2)) == 0) { 
            return;
        }
        else {
            More_Commands = 'n';
            return;
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Find a Partial Variable Var[x-y]                             */
/********************************************************************/
void d117_partial_var () {
//    printf ("%s", "d117_partial_var\n");
    test = 'n';
    strcpy (Command, szScanLeft);
    d580_find_variable ();
    pVar1 = pVariable;
    strcpy (szScanStr, szScanRight);
    szScanStr [strlen (szScanStr) - 1] = 0;
    /************************************************/
    /* Look for "," then "|" then "-" for separator */
    /************************************************/
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        strcpy (szScanTest, "|");
        z900_scan ();
    }
    if  (cScanFound != 'y') {
        strcpy (szScanTest, "-");
        z900_scan ();
    }
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    /************************************/
    /* Find Variable FROM               */
    /************************************/
    if  ((strcmp (szScanLeft, "END") != 0) &&
         (strcmp (szScanLeft, "end") != 0) &&
         (szScanLeft [0] > 64)) {
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szScanLeft, pVariable->szString);
        }
        else {
            sprintf (szScanLeft, "%d", pVariable->Integer);
        }
    }
    Tnum1 = (long int) strtol ((char*)szScanLeft, (char**)&p, 10);
    if  ((Tnum1 < 0) || ((int)Tnum1 > Max)) {
        strcpy (Error, "Invalid Begin");
        z110_fatal ();
        return;
    }
    /************************************/
    /* Find Variable TO                 */
    /************************************/
    if  (szScanRight [0] > 64) {
        strcpy (Command, szScanRight);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szScanRight, pVariable->szString);
        }
        else {
            sprintf (szScanRight, "%d", pVariable->Integer);
        }
    }
    Tnum2 = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
    if  ((Tnum2 < 1) || ((int)Tnum2 > Max) || (Tnum2 < Tnum1)) {
        strcpy (Error, "Invalid End");
        z110_fatal ();
        return;
    }
    /*********************************/
    /* Find the Partial Value        */
    /*********************************/
    pVariable = pVar1;
    strcpy (buff, "");
    if  (Tnum1 == 0) {                                               // Right
        if  ((long) strlen (pVariable->szString) < (Tnum2)) {
            strcpy (buff, pVariable->szString);
        }
        else {
            strcpy (buff, pVariable->szString +
                          strlen (pVariable->szString) - Tnum2);
        }
    }
    else {
        if  ((long) strlen (pVariable->szString) > (Tnum1 - 1)) {    // Left
            strcpy (temp, pVariable->szString);
            temp [Tnum2] = 0;
            strcpy (buff, temp + (Tnum1 - 1));
        }
    }
    /*********************************/
    /* Assign the Partial Value      */
    /*********************************/
    strcpy (Command, szPartialVar);
    d580_find_variable ();
    free (pVariable->szString);
    pVariable->szString = (char *) malloc (strlen (buff) + 5);
    if  (!pVariable->szString) {
        strcpy (Error, "Bad Partial");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szString, buff);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process the CURSOR command                                   */
/********************************************************************/
void d118_cursor () {
    static int Savex;
    static int Savey;
    int Xloc;
    int Yloc;

    Xloc = wherex ();
    Yloc = wherey ();
//    printf ("%s", "d118_cursor\n");
    strcpy (szCursorCmd, "");
    c120_parse_command ();
    if  ((strcmp (Command, "SAVE") == 0) ||
         (strcmp (Command, "save") == 0)) {
//        printf ("\033[s");
        Savex = Xloc;
        Savey = Yloc;
        return;
    }
    if  ((strcmp (Command, "RESTORE") == 0) ||
         (strcmp (Command, "restore") == 0)) {
//        printf ("\033[u");
        gotoxy (Savex, Savey);
        return;
    }
    strcpy (szScanTest, ",");
    strcpy (szScanStr, Command);
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    /**************/
    /* ROW        */
    /**************/
    if  (szScanLeft [0] == Subs) {
        strcpy (Command, szScanLeft);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            Number = 0;
            Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
        }
        else {
            Number = pVariable->Integer;
        }
    }
    else {
        if  ((strcmp (szScanLeft, "UP") == 0) ||
             (strcmp (szScanLeft, "up") == 0) ||
             (strcmp (szScanLeft, "DOWN") == 0) ||
             (strcmp (szScanLeft, "down") == 0) ||
             (strcmp (szScanLeft, "RIGHT") == 0) ||
             (strcmp (szScanLeft, "right") == 0) ||
             (strcmp (szScanLeft, "LEFT") == 0) ||
             (strcmp (szScanLeft, "left") == 0)) {
            strcpy (szCursorCmd, szScanLeft);
        }
        else {
            strcpy (szCursorCmd, "");
            Number = 0;
            Number = (long int)strtol ((char*)szScanLeft, (char**)&p, 10);
        }
    }
    if  (Number < 0) {
        Number = 0;
    }
    row = (int) Number;
    /**************/
    /* COL        */
    /**************/
    if  (szScanRight [0] == Subs) {
        strcpy (Command, szScanRight);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            Number = 0;
            Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
        }
        else {
            Number = pVariable->Integer;
        }
    }
    else {
        Number = 0;
        Number = (long int)strtol ((char*)szScanRight, (char**)&p, 10);
    }
    if  (Number < 0) {
        Number = 0;
    }
    col = (int) Number;
    /********************/
    /* Set Position     */
    /********************/
    if  ((strcmp (szCursorCmd, "UP") == 0) ||
         (strcmp (szCursorCmd, "up") == 0)) {
//        printf ("\033[%dA", col);
        gotoxy (Xloc, Yloc - 1);
        return;
    }
    if  ((strcmp (szCursorCmd, "DOWN") == 0) ||
         (strcmp (szCursorCmd, "down") == 0)) {
//        printf ("\033[%dB", col);
        gotoxy (Xloc, Yloc + 1);
        return;
    }
    if  ((strcmp (szCursorCmd, "RIGHT") == 0) ||
         (strcmp (szCursorCmd, "right") == 0)) {
//        printf ("\033[%dC", col);
        gotoxy (Xloc + 1, Yloc);

        return;
    }
    if  ((strcmp (szCursorCmd, "LEFT") == 0) ||
         (strcmp (szCursorCmd, "left") == 0)) {
//        printf ("\033[%dD", col);
        gotoxy (Xloc - 1, Yloc);
        return;
    }
//    printf ("\033[%d;%dH", row, col);
    gotoxy (col, row);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process INTERACTIVE                                          */
/********************************************************************/
void d119_interactive () {
//    printf ("%s", "d119_interactive\n");
    interactive = 'y';
    pStartCommandBEDIT = pStartCommand;
    pCommandBEDIT = pCommand;
    BEDITindex1 = index1;
    BEDITMore_Commands = More_Commands;
    BEDITlevel++;
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
        exit (1);
    }
    do  {
        printf ("%s", "BEDIT>");
        y200_input_interactive ();
        index1 = 0;
        More_Commands = 'y';
        strcpy (input_line, temp);
        c120_parse_command ();
        if  ((strcmp (Command, "EXIT") == 0) ||
             (strcmp (Command, "exit") == 0) ||
             (strcmp (Command, "DONE") == 0) ||
             (strcmp (Command, "done") == 0) ||
             (strcmp (Command, "END") == 0) ||
             (strcmp (Command, "end") == 0) ||
             (strcmp (Command, "QUIT") == 0) ||
             (strcmp (Command, "quit") == 0) ||
             (strcmp (Command, "STOP") == 0) ||
             (strcmp (Command, "stop") == 0)) {
            /*********************************/
            /* Restore the Original          */
            /*********************************/
            pStartCommand = pStartCommandBEDIT;
            pCommand = pCommandBEDIT;
            More_Commands = BEDITMore_Commands;
            eof_cmd_in = 'n';
            interactive = 'n';
            index1 = BEDITindex1;
            strcpy (input_line, pCommand->szLine);
            if  (BEDITlevel > 0) {
                BEDITlevel--;
            }
        }
        else {
            /*********************************/
            /* Set up New CMD and Process    */
            /*********************************/
            pStartCommand = (COMMAND *) malloc (sizeof (COMMAND));
            if  (!pStartCommand) {
                printf ("%s", "Unable to Process BEDIT Command, MEMORY!\n");
                exit (1);
            }
            memset (pStartCommand, 0, sizeof (COMMAND));
            pCommand = pStartCommand;
            pCommand->pNext = NULL;
            pCommand->pPrev = NULL;
            pCommand->szLine = (char *) malloc (Max);
            if  (!pCommand->szLine) {
                strcpy (Error, "Bad interactive");
                z110_fatal ();
                return;
            }
            memset (pCommand->szLine, 0, Max);
            strcpy (pCommand->szLine, "");
            LineNo = 0;

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
            pNewCommand->szLine = (char *) malloc (Max);
            if  (!pNewCommand->szLine) {
                strcpy (Error, "Bad Interactive");
                z110_fatal ();
                return;
            }
            memset (pNewCommand->szLine, 0, Max);
            strcpy (pNewCommand->szLine, temp);
            pCommand = pNewCommand;
            b200_proc ();
//            /***********************/
//            /* Erase all Variables */
//            /***********************/
//            strcpy (Command, "*");
//            index1 = 0;
//            input_line [0] = 0;
//            d530_erase ();
            interactive = 'y';
        }
    }
    while (interactive == 'y');
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process B B;n                                                */
/********************************************************************/
void d120_process_B () {
//    printf ("%s", "d120_process_B\n");
    pcount = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    Number = 999999;
    if  (Command [1] != 0) {
        if  (Command [1] != ';') {
            strcpy (Error, "Invalid B");
            z110_fatal ();
            return;
        }
        if  (Command [2] == '*') {
            Number = 999999;
        }
        else {
            if  (Command [2] == Subs) {
                strcpy (buff, Command + 2);
                strcpy (temp, Command);
                strcpy (Command, buff);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = pVariable->Integer;
                }
                else {
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                strcpy (buff, Command + 2);
                Number = (long int)strtol((char*)buff, (char**)&p, 10);
            }
        }
        if  (Number < 1) {
            strcpy (Error, "Invalid B Cnt");
            z110_fatal ();
            return;
        }
    }

    f110_prev ();
    if  (Veri == 'y') {
        strcpy (Command, "P");
        d130_process_P ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process COLOR                                                */
/********************************************************************/
void d121_color () {
//    printf ("%s", "d121_color\n");
    c120_parse_command ();
    if  (Command [0] < 64) {
         textcolor ((long int)strtol ((char*)Command, (char**)&p, 10));
         return;             
    }
    if  ((strcmp (Command, "BLACK") == 0) ||
         (strcmp (Command, "black") == 0)) {
         textcolor (BLACK);
         return;
    }
    if  ((strcmp (Command, "DARKBLUE") == 0) ||
         (strcmp (Command, "darkblue") == 0)) {
         textcolor (BLUE);
         return;
    }
    if  ((strcmp (Command, "DARKGREEN") == 0) ||
         (strcmp (Command, "darkgreen") == 0)) {
         textcolor (GREEN);
         return;
    }
    if  ((strcmp (Command, "DARKCYAN") == 0) ||
         (strcmp (Command, "darkcyan") == 0)) {
         textcolor (CYAN);
         return;
    }
    if  ((strcmp (Command, "DARKRED") == 0) ||
         (strcmp (Command, "darkred") == 0)) {
         textcolor (RED);
         return;
    }
    if  ((strcmp (Command, "DARKMAGENTA") == 0) ||
         (strcmp (Command, "darkmagenta") == 0)) {
         textcolor (MAGENTA);
         return;
    }
    if  ((strcmp (Command, "BROWN") == 0) ||
         (strcmp (Command, "brown") == 0)) {
         textcolor (BROWN);
         return;
    }
    if  ((strcmp (Command, "GRAY") == 0) ||
         (strcmp (Command, "gray") == 0)) {
         textcolor (LIGHTGRAY);
         return;
    }
    if  ((strcmp (Command, "BLUE") == 0) ||
         (strcmp (Command, "blue") == 0)) {
         textcolor (LIGHTBLUE);
         return;
    }
    if  ((strcmp (Command, "GREEN") == 0) ||
         (strcmp (Command, "green") == 0)) {
         textcolor (LIGHTGREEN);
         return;
    }
    if  ((strcmp (Command, "CYAN") == 0) ||
         (strcmp (Command, "cyan") == 0)) {
         textcolor (LIGHTCYAN);
         return;
    }
    if  ((strcmp (Command, "RED") == 0) ||
         (strcmp (Command, "red") == 0)) {
         textcolor (LIGHTRED);
         return;
    }
    if  ((strcmp (Command, "MAGENTA") == 0) ||
         (strcmp (Command, "magenta") == 0)) {
         textcolor (LIGHTMAGENTA);
         return;
    }
    if  ((strcmp (Command, "YELLOW") == 0) ||
         (strcmp (Command, "yellow") == 0)) {
         textcolor (YELLOW);
         return;
    }
    if  ((strcmp (Command, "WHITE") == 0) ||
         (strcmp (Command, "white") == 0)) {
         textcolor (WHITE);
         return;
    }
    if  ((strcmp (Command, "INVERSE") == 0) ||
         (strcmp (Command, "inverse") == 0)) {
         textcolor (BLINK);
         return;
    }
    if  ((strcmp (Command, "DEFAULT") == 0) ||
         (strcmp (Command, "default") == 0)) {
         textcolor (7);
         return;
    }
    d580_find_variable ();
    if  (pVariable->Type == 'I') {
        textcolor(pVariable->Integer);
    }
    else {
        if  ((strcmp (pVariable->szString, "BLACK") == 0) ||
             (strcmp (pVariable->szString, "black") == 0)) {
             textcolor (BLACK);
        }
        if  ((strcmp (pVariable->szString, "DARKBLUE") == 0) ||
             (strcmp (pVariable->szString, "darkblue") == 0)) {
             textcolor (BLUE);
        }
        if  ((strcmp (pVariable->szString, "DARKGREEN") == 0) ||
             (strcmp (pVariable->szString, "darkgreen") == 0)) {
             textcolor (GREEN);
        }
        if  ((strcmp (pVariable->szString, "DARKCYAN") == 0) ||
             (strcmp (pVariable->szString, "darkcyan") == 0)) {
             textcolor (CYAN);
        }
        if  ((strcmp (pVariable->szString, "DARKRED") == 0) ||
             (strcmp (pVariable->szString, "darkred") == 0)) {
             textcolor (RED);
        }
        if  ((strcmp (pVariable->szString, "DARKMAGENTA") == 0) ||
             (strcmp (pVariable->szString, "darkmagenta") == 0)) {
             textcolor (MAGENTA);
        }
        if  ((strcmp (pVariable->szString, "BROWN") == 0) ||
             (strcmp (pVariable->szString, "brown") == 0)) {
             textcolor (BROWN);
        }
        if  ((strcmp (pVariable->szString, "GRAY") == 0) ||
             (strcmp (pVariable->szString, "gray") == 0)) {
             textcolor (LIGHTGRAY);
        }
        if  ((strcmp (pVariable->szString, "BLUE") == 0) ||
             (strcmp (pVariable->szString, "blue") == 0)) {
             textcolor (LIGHTBLUE);
        }
        if  ((strcmp (pVariable->szString, "GREEN") == 0) ||
             (strcmp (pVariable->szString, "green") == 0)) {
             textcolor (LIGHTGREEN);
        }
        if  ((strcmp (pVariable->szString, "CYAN") == 0) ||
             (strcmp (pVariable->szString, "cyan") == 0)) {
             textcolor (LIGHTCYAN);
        }
        if  ((strcmp (pVariable->szString, "RED") == 0) ||
             (strcmp (pVariable->szString, "red") == 0)) {
             textcolor (LIGHTRED);
        }
        if  ((strcmp (pVariable->szString, "MAGENTA") == 0) ||
             (strcmp (pVariable->szString, "magenta") == 0)) {
             textcolor (LIGHTMAGENTA);
        }
        if  ((strcmp (pVariable->szString, "YELLOW") == 0) ||
             (strcmp (pVariable->szString, "yellow") == 0)) {
             textcolor (YELLOW);
        }
        if  ((strcmp (pVariable->szString, "WHITE") == 0) ||
             (strcmp (pVariable->szString, "white") == 0)) {
             textcolor (WHITE);
        }
        if  ((strcmp (pVariable->szString, "INVERSE") == 0) ||
             (strcmp (pVariable->szString, "inverse") == 0)) {
             textcolor (BLINK);
        }
        if  ((strcmp (pVariable->szString, "DEFAULT") == 0) ||
             (strcmp (pVariable->szString, "default") == 0)) {
             textcolor (7);
        }
    }

//    if  (Command [0] == Subs) {
//        d580_find_variable ();
//        if  (pVariable->Type == 'S') {
//            Number = 0;
//            Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
//        }
//        else {
//            Number = pVariable->Integer;
//        }
//    }
//    else {
//        Number = 0;
//        Number = (long int)strtol ((char*)Command, (char**)&p, 10);
//    }
//    if  ((Number < 1) || (Number > 7)) {
//        Number = 0;
//    }
//    printf ("\033[%dm", Number + 30);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process BACKCOLOR                                            */
/********************************************************************/
void d122_backcolor () {
//    printf ("%s", "d122_backcolor\n");
    c120_parse_command ();
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            Number = 0;
            Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
        }
        else {
            Number = pVariable->Integer;
        }
    }
    else {
        Number = 0;
        Number = (long int)strtol ((char*)Command, (char**)&p, 10);
    }
    if  ((Number < 1) || (Number > 7)) {
        Number = 0;
    }
//    printf ("\033[%dm", Number + 40);
//    system ("cls");
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process BACKCOLOR                                            */
/********************************************************************/
void d123_bell () {
//    printf ("%s", "d123_bell\n");
    if  (Beditwin == 'n') {
        printf ("%c", bell);
    }
    else {
        MessageBeep (MB_ICONINFORMATION);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process BOOKMARK                                             */
/********************************************************************/
void d124_bookmark () {
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    c120_parse_command ();
    if  ((strcmp (Command, "ERASE") == 0) ||
         (strcmp (Command, "erase") == 0)) {
        if  (pDataFile->szBookmark) {
            free (pDataFile->szBookmark);
            pDataFile->szBookmark = NULL;
        }
        return;
    }

    if  ((strcmp (Command, "ERASEALL") == 0) ||
         (strcmp (Command, "eraseall") == 0)) {
        do  {
            if  (pDataFile->pPrev) {
                pDataFile = pDataFile->pPrev;
            }
        }
        while (pDataFile->pPrev);
        do  {
            if  (pDataFile->pNext) {
                pDataFile = pDataFile->pNext;
                if  (pDataFile->szBookmark) {
                    free (pDataFile->szBookmark);
                    pDataFile->szBookmark = NULL;
                }
            }
        }
        while (pDataFile->pNext);
        return;
    }

    d580_find_variable ();
    if  (pVariable->Type != 'S') {
        strcpy (Error, "Must use Str");
        z110_fatal ();
        return;
    }
    strcpy (pVariable->szEdit, "BOOKMARK");
    if  (pDataFile->szBookmark) {
        free (pDataFile->szBookmark);
        pDataFile->szBookmark = NULL;
    }
    if  (strcmp (pVariable->szString, "") == 0) {
        strcpy (pVariable->szEdit, "");
        return;
    }
    pDataFile->szBookmark = (char *) malloc (strlen (pVariable->szString) + 5);
    if  (!pDataFile->szBookmark) {
        strcpy (Error, "Memory Error");
        z110_fatal ();
        return;
    }
    strcpy (pDataFile->szBookmark, pVariable->szString);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process CASE                                                 */
/********************************************************************/
void d125_case () {
    c120_parse_command ();
    if  ((strcmp (Command, "ON") == 0) ||
         (strcmp (Command, "on") == 0)) {
        cCase = 'y';
        return;
    }
    if  ((strcmp (Command, "OFF") == 0) ||
         (strcmp (Command, "off") == 0)) {
        cCase = 'n';
        return;
    }
    z010_syntax_error ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process CHAR                                                 */
/********************************************************************/
void d126_char () {
//    printf ("%s", "d126_char\n");
    c120_parse_command ();
    if  ((strcmp (Command, "NORMAL") == 0) ||
         (strcmp (Command, "normal") == 0)) {
//        printf ("\033[0m");
        return;
    }
    if  ((strcmp (Command, "BOLD") == 0) ||
         (strcmp (Command, "bold") == 0)) {
//        printf ("\033[1m");
        return;
    }
    if  ((strcmp (Command, "BLINK") == 0) ||
         (strcmp (Command, "blink") == 0)) {
//        printf ("\033[5m");
        return;
    }
    if  ((strcmp (Command, "REVERSE") == 0) ||
         (strcmp (Command, "reverse") == 0)) {
//        printf ("\033[7m");
        return;
    }
    if  ((strcmp (Command, "HIDE") == 0) ||
         (strcmp (Command, "hide") == 0)) {
//        printf ("\033[8m");
        return;
    }
    z010_syntax_error ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process EOL                                                  */
/********************************************************************/
void d127_eol () {
//    printf ("%s", "d127_eol\n");
//    printf ("\033[K");
    clreol ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process BEDITDATA                                            */
/********************************************************************/
void d128_data () {
//    printf ("%s", "d128_data\n");
    if  (!pOpenFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    do  {
        if  (pCommand->pNext) {
            pCommand = pCommand->pNext;
            if  ((memcmp (pCommand->szLine, "*ENDDATA", 8) == 0) ||
                 (memcmp (pCommand->szLine, "*enddata", 8) == 0)) {
                More_Commands = 'n';
                return;
            }
            else {
                strcpy (Command, "I:");
                strcat (Command, splitter);
                strcat (Command, pCommand->szLine);
                strcat (Command, splitter);
                InsertBefore = 'n';
                d160_insert_line ();
            }
        }
    }
    while (pCommand->pNext);

    strcpy (Error, "EOF for BEDITDATA");
    z110_fatal ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Process SCRAMBLE                                             */
/********************************************************************/
void d129_scramble () {
    c120_parse_command ();
    if  ((strcmp (Command, "ON") == 0) ||
         (strcmp (Command, "on") == 0)) {
        iScramble = 1;
        return;
    }
    if  ((strcmp (Command, "OFF") == 0) ||
         (strcmp (Command, "off") == 0)) {
        iScramble = 0;
        return;
    }
    z010_syntax_error ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process P P;n P;*                                            */
/********************************************************************/
void d130_process_P () {
//  printf ("%s", "d130_process_P\n");
    pcount = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    EOF_Flag = 'n';
    Number = 1;
    if  (Command [1] != 0) {
        if  (Command [1] != ';') {
            strcpy (Error, "Invalid P");
            z110_fatal ();
            return;
        }
        if  (Command [2] == '*') {
            Number = 999999;
        }
        else {
            if  (Command [2] == Subs) {
                strcpy (buff, Command + 2);
                strcpy (temp, Command);
                strcpy (Command, buff);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = pVariable->Integer;
                }
                else {
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                strcpy (buff, Command + 2);
                Number = (long int)strtol((char*)buff, (char**)&p, 10);
            }
        }
        if  (Number < 1) {
            strcpy (Error, "Bad Cnt");
            z110_fatal ();
            return;
        }
    }

    if  (pDataFile->szLine [0] != 0) {
        pcount++;
        printf ("%s", pDataFile->szLine);
        //printf ("\n");
        if  ((Debug == 'y') || (Beditwin == 'y')) {
            z999_open_bug ();
            fputs (pDataFile->szLine, file_debug_out);
        }
    }
    if  (Number > 1) {
        Number--;
        do  {
            Number--;
            if  (Direction == 'D') {
                /**********/
                /* Down   */
                /**********/
                if  (pDataFile->pNext) {
                    pDataFile = pDataFile->pNext;
                    if  (pDataFile->szLine [0] != 0) {
                        pcount++;
                        printf ("%s", pDataFile->szLine);
                        //printf ("\n");
                        if  ((Debug == 'y') || (Beditwin == 'y')) {
                            z999_open_bug ();
                            fputs (pDataFile->szLine, file_debug_out);
                        }
                    }
                }
                else {
                    Number = 0;
                    EOF_Flag = 'y';
                    if  (interactive == 'y') {
                        printf ("%s", "EOF\n");
                    }
                    if  ((Debug == 'y') || (Beditwin == 'y')) {
                        z999_open_bug ();
                        fputs ("EOF\n", file_debug_out);
                    }
                }
            }
            else {
                /**********/
                /* Up     */
                /**********/
                pPrevData = NULL;
                if  (pDataFile->pPrev) {
                    pPrevData = pDataFile->pPrev;
                    if  (pPrevData->Begin == 'y') {
                        pPrevData = NULL;
                    }
                }
                if  (pPrevData) {
                    pDataFile = pDataFile->pPrev;
                    if  (pDataFile->szLine [0] != 0) {
                        pcount++;
                        printf ("%s", pDataFile->szLine);
                        //printf ("\n");
                        if  ((Debug == 'y') || (Beditwin == 'y')) {
                            z999_open_bug ();
                            fputs (pDataFile->szLine, file_debug_out);
                        }
                    }
                }
                else {
                    Number = 0;
                    EOF_Flag = 'y';
                    if  (interactive == 'y') {
                        printf ("%s", "TOF\n");
                    }
                    if  ((Debug == 'y') || (Beditwin == 'y')) {
                        z999_open_bug ();
                        fputs ("TOF\n", file_debug_out);
                    }
                }
            }
        }
        while (Number > 0);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process F F;n F;*                                            */
/********************************************************************/
void d135_encryptkey () {
    c120_parse_command ();

    /*********************************************/
    /* Find the Variable                         */
    /*********************************************/
    if  ((Command [0] == Subs) && (strlen (Command) > 1)) {
        strcpy (Tvariable, Command + 1);
        strcpy (Command, Tvariable);
        if  (Command [strlen (Command) - 1] == Subs) {
            if  (strlen (Command) > 0) {
                Command [strlen (Command) - 1] = 0;
            }
        }
        d580_find_variable ();
        if  (found == 'y') {
            strcpy (Command, pVariable->szString);
        }
        else {
            return;
        }
    }
    if  (strcmp (Command, "") != 0) {
        strcpy (szEncryptKey, Command);
        iEncryptKeyPtr = 0;
        return;
    }
    z010_syntax_error ();
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process F F;n F;*                                            */
/********************************************************************/
void d140_process_F () {
//  printf ("%s", "d140_process_F\n");
    pcount = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    Number = 1;
    if  (Command [1] != 0) {
        if  (Command [1] != ';') {
            strcpy (Error, "Bad F");
            z110_fatal ();
            return;
        }
        if  (Command [2] == '*') {
            Number = 999999;
        }
        else {
            if  (Command [2] == Subs) {
                strcpy (buff, Command + 2);
                strcpy (temp, Command);
                strcpy (Command, buff);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = pVariable->Integer;
                }
                else {
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                strcpy (buff, Command + 2);
                Number = (long int)strtol((char*)buff, (char**)&p, 10);
            }
        }
        if  (Number < 1) {
            strcpy (Error, "Bad F");
            z110_fatal ();
            return;
        }
    }

    f100_next ();

    if  (Veri == 'y') {
        strcpy (Command, "P");
        d130_process_P ();
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process FS:/xxx/ FS:/xxx/;n                                  */
/********************************************************************/
void d150_process_FS () {
//  printf ("%s", "d150_process_FS\n");
    char cBookmark;

    cBookmark = 'n';
    pcount = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    /********************************/
    /* Find test string             */
    /********************************/
    szScanTest [0] = Command [3];
    Delimiter = szScanTest [0];
    szScanTest [1] = 0;
    strcpy (szScanStr, Command + 4);
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
        if  (strcmp (pVariable->szEdit, "BOOKMARK") == 0) {
            cBookmark = 'y';
            strcpy (szIf2, pVariable->szString);
        }
    }
    strcpy (szFSString, szScanLeft);
    /********************************/
    /* Check for compound find +,-  */
    /********************************/
    Compound = 0;
    if  ((szScanRight [0] == '+') ||
         (szScanRight [0] == '-')) {
        Compound = szScanRight [0];
        szScanTest [0] = szScanRight [1];
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
        strcpy (szFSString2, szScanLeft);
    }
    /********************************/
    /* Find Count                   */
    /********************************/
    strcpy (szScanTest, ";");
    strcpy (szScanStr, szScanRight);
    z900_scan ();
    Number = 1;
    if  (cScanFound == 'y') {
        if  (strcmp (szScanRight, "*") == 0) {
            Number = 999999;
        }
        else {
            if  (szScanRight [0] == Subs) {
                strcpy (temp, Command);
                strcpy (Command, szScanRight);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = pVariable->Integer;
                }
                else {
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                Number = (long int)strtol((char*)szScanRight, (char**)&p, 10);
            }
            if  (Number < 1) {
                strcpy (Error, "Bad Cnt");
                z110_fatal ();
                return;
            }
        }
    }
    FSCount = (int) Number;
    /********************************/
    /* Find the String              */
    /********************************/
    strcpy (szScanStr, pDataFile->szLine);
    strcpy (szIf1, szScanStr);  /* Temp Hold for - Compound */
    if  (BeginEnd == 'B') {
        szScanStr [strlen (szFSString)] = 0;
    }
    if  (BeginEnd == 'E') {
        if  ((strlen (szScanStr) - 1) < strlen (szFSString)) {
            strcpy (szScanStr, "");
        }
        else {
            strcpy (buff, szScanStr + (strlen (szScanStr) - strlen (szFSString)) - 1);
            strcpy (szScanStr, buff);
        }
    }
    if  (Cols == 'y') {
        if  (ColStart > (int) strlen (szScanStr)) {
            szScanStr [0] = 0;
        }
        else {
            szScanStr [ColEnd + 1] = 0;
            strcpy (temp, szScanStr + ColStart);
            strcpy (szScanStr, temp);
        }
    }
    do  {
        strcpy (szScanTest, szFSString);
        if  (cBookmark == 'y') {
            cScanFound = 'n';
            if  (pDataFile->szBookmark) {
                if  (strcmp (pDataFile->szBookmark, szIf2) == 0) {
                    cScanFound = 'y';
                }
            }
        }
        else {
            if  (iScramble) {
                iScramble = 2;
            }
            z900_scan ();
            if  (iScramble) {
                iScramble = 1;
            }
        }
        /***********************/
        /*     Compound +      */
        /***********************/
        if  ((cScanFound == 'y') && (Compound == '+')) {
            strcpy (temp, szScanRight);
            strcpy (szScanTest, szFSString2);
            z900_scan ();
            if  (cScanFound == 'y') {
                if  (strlen (temp) < strlen (szScanRight)) {
                    strcpy (szScanRight, temp);
                }
            }
        }
        /***********************/
        /*     Compound -      */
        /***********************/
        if  ((cScanFound != 'y') && (Compound == '-')) {
            strcpy (szScanTest, szFSString2);
            strcpy (szScanStr, szIf1);
            if  (BeginEnd == 'B') {
                szScanStr [strlen (szFSString2)] = 0;
            }
            if  (BeginEnd == 'E') {
                if  ((strlen (szScanStr) - 1) < strlen (szFSString2)) {
                    strcpy (szScanStr, "");
                }
                else {
                    strcpy (buff, szScanStr + (strlen (szScanStr) - strlen (szFSString2)) - 1);
                    strcpy (szScanStr, buff);
                }
            }
            z900_scan ();
        }
        /***********************/
        /* Print it            */
        /***********************/
        if  (cScanFound == 'y') {
            strcpy (szScanStr, szScanRight);
            strcpy (szIf1, szScanStr);
            FSCount--;
            pcount++;
            if  (cutcopy == 'y') {
                if  (cut == 'y') {
                    strcpy (Command, "cut;1");
                }
                else {
                    strcpy (Command, "copy;1");
                }
                d310_copy_buffer ();
                pcount--;
                cScanFound = 'n';
                if  ((cut == 'y') && (pDataFile->pPrev)) {
                    pDataFile = pDataFile->pPrev;
                }
            }
            else {
                if  ((Verify == 'y') || (Veri == 'y')) {
                    printf ("%s", pDataFile->szLine);
                    if  ((Debug == 'y') || (Beditwin == 'y')) {
                        z999_open_bug ();
                        fputs (pDataFile->szLine, file_debug_out);
                    }
                }
            }
            if  (cBookmark == 'y') {
                cScanFound = 'n';
            }
        }
        /***********************/
        /* Check the Count     */
        /***********************/
        if  (FSCount > 0) {
            if  (cScanFound != 'y') {
                Number = 1;
                if  (Direction == 'D') {
                    /***********/
                    /* Down    */
                    /***********/
                    if  (!pDataFile->pNext) {
                        FSCount = 0;
                        if  (interactive == 'y') {
                            printf ("%s", "EOF\n");
                        }
                        if  (Debug == 'y') {
                            z999_open_bug ();
                            fputs ("EOF\n", file_debug_out);
                        }
                        EOF_Flag = 'y';
                    }
                    else {
                        f100_next ();
                        if  (EOF_Flag != 'y') {
                            pcount--;
                        }
                        strcpy (szScanStr, pDataFile->szLine);
                        strcpy (szIf1, szScanStr);
                        if  (BeginEnd == 'B') {
                            szScanStr [strlen (szFSString)] = 0;
                        }
                        if  (BeginEnd == 'E') {
                            if  ((strlen (szScanStr) - 1) < strlen (szFSString)) {
                                strcpy (szScanStr, "");
                            }
                            else {
                                strcpy (buff, szScanStr + (strlen (szScanStr) - strlen (szFSString)) - 1);
                                strcpy (szScanStr, buff);
                            }
                        }
                        if  (Cols == 'y') {
                            if  (ColStart > (int) strlen (szScanStr)) {
                                szScanStr [0] = 0;
                            }
                            else {
                                szScanStr [ColEnd + 1] = 0;
                                strcpy (temp, szScanStr + ColStart);
                                strcpy (szScanStr, temp);
                            }
                        }
                    }
                }
                else {
                    /***********/
                    /* Up      */
                    /***********/
                    pPrevData = NULL;
                    if  (pDataFile->pPrev) {
                        pPrevData = pDataFile->pPrev;
                        if  (pPrevData->Begin == 'y') {
                            pPrevData = NULL;
                        }
                    }
                    if  (!pPrevData) {
                        FSCount = 0;
                        if  (interactive == 'y') {
                            printf ("%s", "TOF\n");
                        }
                        if  (Debug == 'y') {
                            z999_open_bug ();
                            fputs ("TOF\n", file_debug_out);
                        }
                        EOF_Flag = 'y';
                    }
                    else {
                        f100_next ();
                        if  (EOF_Flag != 'y') {
                            pcount--;
                        }
                        strcpy (szScanStr, pDataFile->szLine);
                        strcpy (szIf1, szScanStr);
                        if  (BeginEnd == 'B') {
                            szScanStr [strlen (szFSString)] = 0;
                        }
                        if  (BeginEnd == 'E') {
                            if  ((strlen (szScanStr) - 1) < strlen (szFSString)) {
                                strcpy (szScanStr, "");
                            }
                            else {
                                strcpy (buff, szScanStr + (strlen (szScanStr) - strlen (szFSString)) - 1);
                                strcpy (szScanStr, buff);
                            }
                        }
                        if  (Cols == 'y') {
                            if  (ColStart > (int) strlen (szScanStr)) {
                                szScanStr [0] = 0;
                            }
                            else {
                                szScanStr [ColEnd + 1] = 0;
                                strcpy (temp, szScanStr + ColStart);
                                strcpy (szScanStr, temp);
                            }
                        }
                    }
                }
            }
        }
    }
    while (FSCount > 0);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process I:/xxx/ IB:/xxx/                                     */
/********************************************************************/
void d160_insert_line () {
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ":");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    szScanTest [0] = szScanRight [0];
    Delimiter = szScanRight [0];
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
    pNewDataFile = (DATAFILE *) malloc (sizeof (DATAFILE));
    if  (!pNewDataFile) {
        strcpy (Error, "Bad Insert!");
        z110_fatal ();
        return;
    }
    memset (pNewDataFile, 0, sizeof (DATAFILE));
    pCurrData = pDataFile;
    pPrevData = pDataFile->pPrev;
    pNextData = pDataFile->pNext;
    if  ((InsertBefore != 'y') || (!pCurrData->pPrev)) {
        pCurrData->pNext = pNewDataFile;
        pNewDataFile->pPrev = pCurrData;
        if  (pNextData) {
            pNextData->pPrev = pNewDataFile;
            pNewDataFile->pNext = pNextData;
        }
        else {
            pNewDataFile->pNext = NULL;
        }
    }
    else {
        pPrevData->pNext = pNewDataFile;
        pNewDataFile->pPrev = pPrevData;
        pNewDataFile->pNext = pCurrData;
        pCurrData->pPrev = pNewDataFile;
    }
    pDataFile = pNewDataFile;
    strcat (szScanLeft, retn);
    pDataFile->szLine = (char *) malloc (strlen (szScanLeft) + 5);
    if  (!pDataFile->szLine) {
        strcpy (Error, "Bad Insert!");
        z110_fatal ();
        return;
    }
    strcpy (pDataFile->szLine, szScanLeft);
    if  (Veri == 'y') {
        printf ("%s", pDataFile->szLine);
    }

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Insert lines until finished                                  */
/********************************************************************/
void d165_insert_mode () {
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    done3 = 'n';
    do  {
        printf ("%s", "Insert>");
        y100_input_string ();
        if  (temp [0] == 0) {
            done3 = 'y';
        }
        else {
            strcpy (Command, "I:");
            strcat (Command, splitter);
            strcat (Command, temp);
            strcat (Command, splitter);
            InsertBefore = Before;
            d160_insert_line ();
            InsertBefore = 'n';
        }
    }
    while (done3 != 'y');
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process D D;n D;*                                            */
/********************************************************************/
void d170_process_D () {
//  printf ("%s", "d170_process_D\n");
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    Number = 1;
    if  (Command [1] != 0) {
        if  (Command [1] != ';') {
            strcpy (Error, "Bad D");
            z110_fatal ();
            return;
        }
        if  (Command [2] == '*') {
            Number = 999999;
        }
        else {
            if  (Command [2] == Subs) {
                strcpy (buff, Command + 2);
                strcpy (temp, Command);
                strcpy (Command, buff);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = pVariable->Integer;
                }
                else {
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                strcpy (buff, Command + 2);
                Number = (long int)strtol((char*)buff, (char**)&p, 10);
            }
        }
        if  (Number < 1) {
            strcpy (Error, "Bad D");
            z110_fatal ();
            return;
        }
    }

    if  (Number > 0) {
        do  {
            Number--;
            pCurrData = pDataFile;
            pPrevData = pDataFile->pPrev;
            pNextData = pDataFile->pNext;
            if  (Direction == 'D') {
                /*******************/
                /* Down            */
                /*******************/
                if  (pCurrData->pNext) {
                    pPrevData->pNext = pCurrData->pNext;
                    pNextData->pPrev = pPrevData;
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
                    if  (!pPrevData) {
                        return;
                    }
                    pPrevData->pNext = NULL;
                    pDataFile = pDataFile->pPrev;
                    Number = 0;
                }
            }
            else {
                /*******************/
                /* Up              */
                /*******************/
                pPrevData->pNext = pCurrData->pNext;
                if  (pCurrData->pNext) {
                    pNextData->pPrev = pPrevData;
                }
                pDataFile = pPrevData;
                if  (pDataFile->Begin == 'y') {
                    if  (interactive == 'y') {
                        printf ("%s", "TOF\n");
                    }
                    if  (Debug == 'y') {
                        z999_open_bug ();
                        fputs ("TOF\n", file_debug_out);
                    }
                    EOF_Flag = 'y';
                    if  (pNextData) {
                        pDataFile = pDataFile->pNext;
                    }
                    Number = 0;
                }
            }
            free (pCurrData->szLine);
            free (pCurrData);
            pcount++;
        }
        while (Number > 0);
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a SUBS command                                       */
/********************************************************************/
void d175_subs () {
    c120_parse_command ();
    if  (strlen (Command) != 1) {
        strcpy (Error, "Bad Subs");
        z110_fatal ();
        return;
    }
    Subs = Command [0];
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a PATTERN command                                    */
/********************************************************************/
void d176_pattern () {
    c120_parse_command ();
    if  ((strcmp (Command, "ON") == 0) ||
         (strcmp (Command, "on") == 0)) {
        Patton = 'y';
        return;
    }
    if  ((strcmp (Command, "OFF") == 0) ||
         (strcmp (Command, "off") == 0)) {
        Patton = 'n';
        return;
    }
    if  (strlen (Command) != 5) {
        z010_syntax_error ();
        return;
    }
    Pattany = Command [0];
    Pattnum = Command [1];
    Pattalp = Command [2];
    Pattspc = Command [3];
    Pattend = Command [4];
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a SYSTEM command                                     */
/********************************************************************/
void d180_process_system () {

    HFILE pDefault;
    HFILE pFullscr;
    int cbRead;
    PBYTE pbBuf;

    //char cFull;
    char cDone;
    char cReadChar;

    //cFull = 'n';
    ExecFullScreen = 0;
//    printf ("%s", "d180_process_system\n");
    if  ((strcmp (buff, "SYSTEMF:") == 0) ||
         (strcmp (buff, "systemf:") == 0)) {
        //cFull = 'y';
        ExecFullScreen = 1;
        strcpy (buff, Command + 9);
    }
    else {
        strcpy (buff, Command + 8);
    }
    buff [strlen (buff) - 1] = 0;
    if  (buff [0] == Subs) {
        strcpy (Command, buff);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (buff, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
    }
    z998_winexec (buff);
    ExecFullScreen = 0;
    //******************************
    //* Reset to fullscreen
    //******************************
    //pbBuf = (PBYTE) LocalAlloc (LMEM_FIXED, 2048);
    //pFullscr = _lopen ("c:\\windows\\fullscr.pif", READ);
    //if  (pFullscr == HFILE_ERROR) {
    //    strcpy (Error, "No fullscr.pif");
    //    z110_fatal ();
    //    return;
    //}
    //pDefault = _lopen ("c:\\windows\\_default.pif", WRITE);
    //do  {
    //    cbRead = _lread (pFullscr, pbBuf, 2048);
    //    _lwrite (pDefault, pbBuf, cbRead);
    //}
    //while (cbRead != 0);
    //LocalFree ((HLOCAL) pbBuf);
    //_lclose (pDefault);
    //_lclose (pFullscr);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a SYSTEMCURR command                                 */
/********************************************************************/
void d181_process_systemcurr () {
    if  (!pOpenFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    /*****************************/
    /* Close the Current File    */
    /*****************************/
    strcpy (szFileName, pOpenFile->szFileName);
    holdindex1 = index1;
    strcpy (szIf1, input_line);
    strcpy (szIf2, Command);
    strcpy (input_line, szFileName);
    strcpy (szFSString, szFileName);
    index1 = 0;
    d113_close_file ();

    index1 = holdindex1;
    strcpy (input_line, szIf1);

    /*****************************/
    /* Find the System Command   */
    /*****************************/
    strcpy (Command, szIf2);
    strcpy (buff, Command + 12);
    buff [strlen (buff) - 1] = 0;
    if  (buff [0] == Subs) {
        strcpy (Command, buff);
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (buff, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
    }
    /*****************************/
    /* Process the System Command*/
    /*****************************/
    strcat (buff, " >BEDIT.CUR");
//    system (buff);
    z998_winexec (buff);

    /*****************************/
    /* Read the output into curr */
    /*****************************/
    holdindex1 = index1;
    strcpy (szIf1, input_line);
    index1 = 0;
    sprintf (input_line, "%s BEDIT.CUR", szFSString);
    d100_process_read ();
    index1 = holdindex1;
    strcpy (input_line, szIf1);
//    system ("del BEDIT.cur");
    z998_winexec ("del BEDIT.cur");

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a SORT command                                       */
/********************************************************************/
void d182_sort () {
    char szHoldFile [100];
    z300_store_file_position ();
    c120_parse_command ();
    /**************************/
    /* Keep Current Line Vals */
    /**************************/
    holdindex1 = index1;
    strcpy (szIf1, input_line);
    strcpy (szIf2, Command);
    /**************************/
    /* File Name              */
    /**************************/
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    strcpy (szFileName, szScanLeft);
    /**************************/
    /* Ascending-Descending   */
    /**************************/
    strcpy (szScanStr, szScanRight);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    if  ((strcmp (szScanLeft, "A") == 0) ||
         (strcmp (szScanLeft, "a") == 0)) {
        strcpy (SortType, "");
    }
    else {
        if  ((strcmp (szScanLeft, "D") == 0) ||
             (strcmp (szScanLeft, "d") == 0)) {
            strcpy (SortType, "/R");
        }
        else {
            z010_syntax_error ();
            return;
        }
    }
    /**************************/
    /* Starting Column        */
    /**************************/
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
    SortPosNum = 0;
    SortPosNum = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
    sprintf (SortPos, "/+%d", SortPosNum);

    /*****************************/
    /* Convert Filename          */
    /*****************************/

    strcpy (Command, szFileName);
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (Command, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use Str");
            z110_fatal ();
            return;
        }
    }
    if  (strcmp (Command, "*") == 0) {
        if  (!pOpenFile) {
            strcpy (Error, "No Curr File");
            z110_fatal ();
            return;
        }
        strcpy (Command, pOpenFile->szFileName);
    }
    strcpy (szFileName, Command);
    z200_reset_file_position ();

    if  (!pOpenFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    /*****************************/
    /* Write the Current File    */
    /*****************************/
    strcpy (szFileName, pOpenFile->szFileName);
    strcpy (szHoldFile, pOpenFile->szFile);
    strcpy (input_line, szFileName);
    strcat (input_line, " BEDIT.sr1");
    strcpy (szFSString, szFileName);
    index1 = 0;
    d110_process_write ();

    /*****************************/
    /* Close the Current File    */
    /*****************************/
    strcpy (szFileName, pOpenFile->szFileName);

    strcpy (input_line, szFileName);
    strcpy (szFSString, szFileName);
    index1 = 0;
    d113_close_file ();

    /*****************************/
    /* Sort the file             */
    /*****************************/
    strcpy (input_line, "sort ");
    strcat (input_line, SortType);
    strcat (input_line, " ");
    strcat (input_line, SortPos);
    strcat (input_line, " <BEDIT.sr1 >BEDIT.sr2");
//    system (input_line);
    z998_winexec (input_line);

    /*****************************/
    /* Read the output into curr */
    /*****************************/
    index1 = 0;
    sprintf (input_line, "%s BEDIT.sr2", szFSString);
    d100_process_read ();
    /**************************/
    /* Change the filestring  */
    /**************************/
    strcpy (pOpenFile->szFile, szHoldFile);

    /**************************/
    /* Restore Curr Line Vals */
    /**************************/
    index1 = holdindex1;
    strcpy (input_line, szIf1);

    /**************************/
    /* Delete Sort Work Files */
    /**************************/
//    system ("del BEDIT.sr*");
    z998_winexec ("del BEDIT.sr*");

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process COLS                                                 */
/********************************************************************/
void d185_cols () {
    c120_parse_command ();
    if  ((BeginEnd == 'B') ||
         (BeginEnd == 'E')) {
        strcpy (Error, "No COLS with BEGINEND!");
        z110_fatal ();
        return;
    }
    strcpy (szScanStr, Command);
    strcpy (szScanTest, ",");
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    /************/
    /* Start    */
    /************/
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
    ColStart = 0;
    ColStart = (long int) strtol ((char*)szScanLeft, (char**)&p, 10);
    if  (ColStart < 1) {
        strcpy (Error, "Bad Start Pos");
        z110_fatal ();
        return;
    }
    ColStart--;
    /************/
    /* End      */
    /************/
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
    ColEnd = 0;
    ColEnd = (long int) strtol ((char*)szScanRight, (char**)&p, 10);
    if  (ColEnd < 1) {
        strcpy (Error, "Bad End Pos");
        z110_fatal ();
        return;
    }
    ColEnd--;
    if  (ColEnd < ColStart) {
        strcpy (Error, "Bad Range");
        z110_fatal ();
        return;
    }
    Cols = 'y';
    return;
}


/********************************************************************/
/* This module will :                                               */
/*     process a RV:/xxx/:/yyy/;n                                   */
/********************************************************************/
void d190_process_RV () {
//    printf ("%s", "d190_process_RV\n");
    pcount = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    strcpy (szScanTest, ":");
    strcpy (szScanStr, Command);
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    /*********************************/
    /* Find the FROM string          */
    /*********************************/
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

    /*********************************/
    /* Find the TO string            */
    /*********************************/
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

    strcpy (szChangeTo, szScanLeft);
    /*********************************/
    /* Find the NUMBER to change     */
    /*********************************/
    ChangeNumber = 1;
    if  (szScanRight [0] == ';') {
        if  (szScanRight [1] == '*') {
            ChangeNumber = 999999;
        }
        else {
            if  (szScanRight [1] == Subs) {
                strcpy (Command, szScanRight + 1);
                d580_find_variable ();
                if  (pVariable->Type == 'I') {
                    ChangeNumber = (long) pVariable->Integer;
                }
                else {
                    ChangeNumber = (long int) strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                ChangeNumber = (long int) strtol ((char*)szScanRight + 1, (char**)&p, 10);
            }
            if  (ChangeNumber < 1) {
                strcpy (Error, "Bad Cnt");
                z110_fatal ();
                return;
            }
        }
    }
    /*********************************/
    /* Perform the RVS               */
    /*********************************/
    strcpy (szScanStr, pDataFile->szLine);
    strcpy (szLEFT, "");
    if  (Cols == 'y') {
        if  (ColStart > (int) strlen (szScanStr)) {
            szScanStr [0] = 0;
        }
        else {
            strcpy (szLEFT, szScanStr);
            strcpy (szScanStr, szLEFT + ColStart);
            szLEFT [ColStart] = 0;
        }
    }
    do  {

        strcpy (szScanTest, szChangeFrom);
        z900_scan ();
        if  (Cols == 'y') {
            if  (( (int) strlen (szLEFT) +
                   (int) strlen (szScanLeft) +
                   (int) strlen (szScanTest) - 1) > ColEnd) {
                cScanFound = 'n';
            }
        }
        if  (cScanFound == 'y') {
            if  (strlen (szChangeFrom) > 0) {
                if  (Patton == 'y') {
                    if  (szChangeFrom [strlen (szChangeFrom) - 1] == Pattend) {
                        strcpy (szScanRight, retn);
                    }
                }
            }
            ChangeNumber--;
            pcount++;
            sprintf (buff, "%s%s", szLEFT, szScanLeft);
            strcpy (szScanLeft, buff);
            sprintf (szLEFT, "%s%s", szScanLeft, szChangeTo);
            sprintf (buff, "%s%s%s",
                     szScanLeft, szChangeTo, szScanRight);
            free (pDataFile->szLine);
            pDataFile->szLine = (char *) malloc (strlen (buff) + 5);
            if  (!pDataFile->szLine) {
                strcpy (Error, "Bad Insert");
                z110_fatal ();
                return;
            }
            strcpy (pDataFile->szLine, buff);
            if  ((Verify == 'y') || (Veri == 'y')) {
                printf ("%s", pDataFile->szLine);
                if  ((Debug == 'y') || (Beditwin == 'y')) {
                    z999_open_bug ();
                    fputs (pDataFile->szLine, file_debug_out);
                }
            }
            strcpy (szScanStr, szScanRight);
        }
        if  (ChangeNumber > 0) {
            if  (cScanFound != 'y') {
                Number = 1;
                if  (!pDataFile->pNext) {
                    ChangeNumber = 0;
                    if  (interactive == 'y') {
                        printf ("%s", "EOF\n");
                    }
                    if  (Debug == 'y') {
                        z999_open_bug ();
                        fputs ("EOF\n", file_debug_out);
                    }
                    EOF_Flag = 'y';
                }
                else {
                    f100_next ();
                    if  (EOF_Flag != 'y') {
                        pcount--;
                    }
                    strcpy (szScanStr, pDataFile->szLine);
                    strcpy (szLEFT, "");
                    if  (Cols == 'y') {
                        if  (ColStart > (int) strlen (szScanStr)) {
                            szScanStr [0] = 0;
                        }
                        else {
                            strcpy (szLEFT, szScanStr);
                            strcpy (szScanStr, szLEFT + ColStart);
                            szLEFT [ColStart] = 0;
                        }
                    }
                }
            }
        }
    }
    while (ChangeNumber > 0);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process a R:/xxx/                                            */
/********************************************************************/
void d200_process_R () {
//    printf ("%s", "d200_process_R\n");
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    if  (!pDataFile->pPrev) {
        strcpy (Error, "No Current Line");
        z110_fatal ();
        return;
    }
    strcpy (szScanTest, ":");
    strcpy (szScanStr, Command);
    z900_scan ();
    if  (cScanFound != 'y') {
        z010_syntax_error ();
        return;
    }
    szScanTest [0] = szScanRight [0];
    Delimiter = szScanRight [0];
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

    strcpy (buff, szScanLeft);
    strcat (buff, retn);

    free (pDataFile->szLine);
    pDataFile->szLine = (char *) malloc (strlen (buff) + 5);
    if  (!pDataFile->szLine) {
        strcpy (Error, "Bad Insert!");
        z110_fatal ();
        return;
    }
    strcpy (pDataFile->szLine, buff);
    if  ((Verify == 'y') || (Veri == 'y')) {
        printf ("%s", pDataFile->szLine);
        if  ((Debug == 'y') || (Beditwin == 'y')) {
            z999_open_bug ();
            fputs (pDataFile->szLine, file_debug_out);
        }
    }
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process FIXFILE                                              */
/********************************************************************/
void d210_fixfile () {
    char szFromFile [500];
    char szToFile [500];

    FILE       *FromFile;
    FILE       *ToFile;

    char       *cFileStatus;
    int        iChar;

    //*******************************
    //* From Filename
    //*******************************
    c120_parse_command ();
    if  (strlen (Command) > 190) {
        strcpy (Error, "From Filename Too Long");
        z110_fatal ();
        return;
    }

    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szFromFile, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use string variable for From Filename");
            z110_fatal ();
            return;
        }
    }
    else {
        strcpy (szFromFile, Command);
    }

    //*******************************
    //* To Filename
    //*******************************
    c120_parse_command ();
    if  (strlen (Command) > 190) {
        strcpy (Error, "To Filename Too Long");
        z110_fatal ();
        return;
    }
    if  (Command [0] == Subs) {
        d580_find_variable ();
        if  (pVariable->Type == 'S') {
            strcpy (szToFile, pVariable->szString);
        }
        else {
            strcpy (Error, "Must use string variable for To Filename");
            z110_fatal ();
            return;
        }
    }
    else {
        strcpy (szToFile, Command);
    }

    //**************************************************
    //* Open files
    //**************************************************
    FromFile = fopen (szFromFile, "r");
    if  (!FromFile) {
        sprintf(Error, "Unable to open %s", szFromFile);
        z110_fatal ();
        return;
    }
    ToFile = fopen (szToFile, "w");
    if  (!ToFile) {
        sprintf(Error, "Unable to open %s", szToFile);
        z110_fatal ();
        return;
    }

    //**************************************************
    //* Fix the file
    //**************************************************
    do {
        iChar = fgetc(FromFile);
        if  (iChar != EOF) {
            if (iChar == 10) {
               fputs("\n", ToFile);
            }
            else {
               fputc(iChar, ToFile);
            }
        }
    }
    while (iChar != EOF);

    //**************************************************
    //* Close the Files
    //**************************************************
    fclose (FromFile);
    fclose (ToFile);

    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process FRONT                                                */
/********************************************************************/
void d250_front () {
//    printf ("%s", "d250_front\n");
    pcount = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    if  ((!pDataFile->pPrev) && (!pDataFile->pNext)) {
        return;
    }
    /********************************/
    /* Find test string             */
    /********************************/
    szScanTest [0] = Command [6];
    Delimiter = szScanTest [0];
    szScanTest [1] = 0;
    strcpy (szScanStr, Command + 7);
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
    strcpy (szFSString, szScanLeft);
    /********************************/
    /* Find Count                   */
    /********************************/
    strcpy (szScanTest, ";");
    strcpy (szScanStr, szScanRight);
    z900_scan ();
    Number = 1;
    if  (cScanFound == 'y') {
        if  (strcmp (szScanRight, "*") == 0) {
            Number = 999999;
        }
        else {
            if  (szScanRight [0] == Subs) {
                strcpy (temp, Command);
                strcpy (Command, szScanRight);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = pVariable->Integer;
                }
                else {
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                Number = (long int)strtol((char*)szScanRight, (char**)&p, 10);
            }
            if  (Number < 1) {
                strcpy (Error, "Bad Cnt");
                z110_fatal ();
                return;
            }
        }
    }
    FSCount = (int) Number;
    if  (FSCount < 1) {
        FSCount = 1;
    }
    do  {
        /***********************/
        /* Change the line     */
        /***********************/
        FSCount--;
        pcount++;
        strcpy (temp, szFSString);
        strcat (temp, pDataFile->szLine);
        free (pDataFile->szLine);
        pDataFile->szLine = (char *) malloc (strlen (temp) + 5);
        if  (!pDataFile->szLine) {
            strcpy (Error, "Bad resize");
            z110_fatal ();
            return;
        }
        strcpy (pDataFile->szLine, temp);
        if  (FSCount > 0) {
            if  (pDataFile->pNext) {
                pDataFile = pDataFile->pNext;
            }
            else {
                FSCount = 0;
            }
        }
    }
    while (FSCount > 0);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     process BACK                                                 */
/********************************************************************/
void d260_back () {
//    printf ("%s", "d260_back\n");
    pcount = 0;
    if  (!pDataFile) {
        strcpy (Error, "No Curr File");
        z110_fatal ();
        return;
    }
    if  ((!pDataFile->pPrev) && (!pDataFile->pNext)) {
        return;
    }
    /********************************/
    /* Find test string             */
    /********************************/
    szScanTest [0] = Command [5];
    Delimiter = szScanTest [0];
    szScanTest [1] = 0;
    strcpy (szScanStr, Command + 6);
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
    strcpy (szFSString, szScanLeft);
    /********************************/
    /* Find Count                   */
    /********************************/
    strcpy (szScanTest, ";");
    strcpy (szScanStr, szScanRight);
    z900_scan ();
    Number = 1;
    if  (cScanFound == 'y') {
        if  (strcmp (szScanRight, "*") == 0) {
            Number = 999999;
        }
        else {
            if  (szScanRight [0] == Subs) {
                strcpy (temp, Command);
                strcpy (Command, szScanRight);
                d580_find_variable ();
                strcpy (Command, temp);
                if  (pVariable->Type == 'I') {
                    Number = pVariable->Integer;
                }
                else {
                    Number = (long int)strtol ((char*)pVariable->szString, (char**)&p, 10);
                }
            }
            else {
                Number = (long int)strtol((char*)szScanRight, (char**)&p, 10);
            }
            if  (Number < 1) {
                strcpy (Error, "Bad Cnt");
                z110_fatal ();
                return;
            }
        }
    }
    FSCount = (int) Number;
    if  (FSCount < 1) {
        FSCount = 1;
    }
    do  {
        /***********************/
        /* Change the line     */
        /***********************/
        FSCount--;
        pcount++;
        strcpy (temp, pDataFile->szLine);
        temp [strlen (temp) - 1] = 0;
        strcat (temp, szFSString);
        strcat (temp, retn);
        free (pDataFile->szLine);
        pDataFile->szLine = (char *) malloc (strlen (temp) + 5);
        if  (!pDataFile->szLine) {
            strcpy (Error, "Bad resize");
            z110_fatal ();
            return;
        }
        strcpy (pDataFile->szLine, temp);
        if  (FSCount > 0) {
            if  (pDataFile->pNext) {
                pDataFile = pDataFile->pNext;
            }
            else {
                FSCount = 0;
            }
        }
    }
    while (FSCount > 0);
    return;
}

/********************************************************************/
/* This module will :                                               */
/*     Set up a PERMenant CALLed routine structure                  */
/********************************************************************/
void d270_perm () {
    c120_parse_command ();
    pPermenant = &StartPermenant;
    do  {
        if  (pPermenant->pNext) {
            pPermenant = pPermenant->pNext;
            /************************************/
            /* See if it's already set up       */
            /************************************/
            if  (strcmp (pPermenant->szName, Command) == 0) {
                return;
            }
        }
    }
    while (pPermenant->pNext);
    pNewPermenant = (PERMENANT *) malloc (sizeof (PERMENANT));
    if  (!pNewPermenant) {
        strcpy (Error, "Bad Perm");
        z110_fatal ();
        return;
    }
    memset (pNewPermenant, 0, sizeof (PERMENANT));
    pPermenant->pNext = pNewPermenant;
    pNewPermenant->pPrev = pPermenant;
    pNewPermenant->pNext = NULL;\
    pNewPermenant->cLoaded = 'n';
    pNewPermenant->pStartCommand = NULL;
    strcpy (pNewPermenant->szName, Command);
    return;
}

/************************************************************************/
/*                                                                      */
/*                         End of program BEDIT2.c                      */
/*                                                                      */
/************************************************************************/
/* Formatted on 09/21/90 at 14:24:23 by Forrest Bentley's C-Format v1.4 */
