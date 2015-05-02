/**********************************************/
/* BEDITVAR.H                                 */
/**********************************************/

extern unsigned char     cFast;
extern unsigned char     cBeditDebug;
extern unsigned char     cEncryptCommand;
extern int               iEncryptKeyPtr;
extern unsigned char     name_cmd_in     [100];            /* Command File Name             */
extern unsigned char     name_debug_out   [80];            /* Command File Name             */

extern unsigned char     date_str [9];                     /* system date                   */
extern unsigned char     time_str [9];                     /* system time                   */

extern unsigned char     retn [2];                         /* return extern unsigned character string       */
extern unsigned char     quote [2];                        /* " character string            */
extern unsigned char     slash [2];                        /* backslash character           */
extern unsigned char     splitter [2];
extern unsigned char     *input_line;                      /* input line                    */
extern unsigned char     *temp;                            /* temporary string work         */
extern unsigned char     *Command;                         /* parsed command                */
extern unsigned char     *Error;                           /* Error                         */
extern unsigned char     *szFileLabel;
extern unsigned char     *szPartialVar;
extern int      LineNumber;

extern unsigned char     eof_file;                         /* eof for input file            */
extern unsigned char     eof_cmd_in;                       /* eof for input file            */
extern unsigned char     eof_debug_out;                    /* eof for input file            */

extern unsigned char     *file_status;                     /* file status for input file    */
extern unsigned char     bell;                             /* bell sound for errors         */
extern unsigned char     *buff;                            /* string for outtext fncts      */
extern unsigned char     *Debugbuff;                       /* string for outtext fncts      */
extern unsigned char     *Tvariable;                       /* string for outtext fncts      */
extern unsigned char     *szHoldBuff;                      /* string for outtext fncts      */
extern unsigned char     response;                         /* misc response character       */
extern unsigned char     first;                            /* first time switch             */
extern unsigned char     args_supplied;                    /* file name arguments supplied  */
extern unsigned char     done;                             /* done flag                     */
extern unsigned char     done2;                            /* second done flag              */
extern unsigned char     done3;                            /* done flag                     */
extern unsigned char     found;                            /* found flag                    */
extern unsigned char     found1;
extern unsigned char     found2;
extern unsigned char     More_Commands;
extern unsigned char     Debug;
extern unsigned char     *ErrorLine;
extern int      ErrorPtr;                   
extern unsigned char     Delimit;
extern unsigned char     Delimiter;
extern int      DelimitCount;            
extern unsigned char     *szFSString;
extern unsigned char     *szFSString2;
extern unsigned char     Compound;
extern int      FSCount;
extern unsigned char     Verify;
extern unsigned char     Veri;
extern unsigned char     InsertBefore;
extern unsigned char     Before;
extern unsigned char     *szFileString;
extern unsigned char     *szFileName;
extern unsigned char     *szChangeFrom;
extern unsigned char     *szChangeTo;
extern unsigned char     *szConvString;
extern unsigned char     *szNewString;
extern int      x1;
extern int      x2;
extern int      x3;
extern unsigned char     Pattany;
extern unsigned char     Pattnum;
extern unsigned char     Pattalp;
extern unsigned char     Pattspc;
extern unsigned char     Pattend;
extern unsigned char     Patton;
extern long int ChangeNumber;           
extern long int InputNumber;            
extern int      ReadCount;              
extern unsigned int
         MemCount;                 
extern int      pcount;                       
extern unsigned char     cut;
extern unsigned char     cutcopy;
extern unsigned char     ClearBuff;
extern unsigned char     *Label;
extern unsigned char     EOF_Flag;
extern unsigned char     Printx;
extern unsigned char     Call;
extern unsigned char     *szVariable;
extern unsigned char     test;
extern unsigned char     *Var1;
extern unsigned char     *Var2;
extern int      Num1;
extern int      Num2;
extern unsigned char     Math           [20];
extern unsigned char     *szIf1;
extern unsigned char     *szIf2;
extern unsigned char     *szVar2;
extern unsigned char     szFunctionkey [20];
extern unsigned char     len;
extern unsigned char     Word;
extern unsigned char     interactive;
extern unsigned char     BEDITargs     [100];
extern unsigned char     BEDITMore_Commands;
extern unsigned char     Abort;
extern unsigned char     Subs;
extern unsigned char     Recursion;
extern unsigned char     Cols;
extern int      ColStart;                   
extern int      ColEnd;                       
extern int      ExtrStart;                      
extern int      ExtrLen;                       
extern unsigned char     trace;
extern unsigned char     SortType       [10];
extern unsigned char     SortPos        [10];
extern int      SortPosNum;                
extern unsigned char     BeginEnd;
extern int      BEDITlevel;                   
extern unsigned char     FoundVar;
extern unsigned char     FileError;
extern unsigned char     FirstInteractive;
extern unsigned char     CallVar;

extern int      index1;                           /* input line index              */
extern int      holdindex1;                
extern int      BEDITindex1;                  
extern int      index2;                           /* output line index             */
extern int      index3;                           /* token index                   */
extern int      index4;                           /* misc work index               */
extern int      index7;                           /* misc work index               */
extern int      index8;                           /* misc work index               */
extern int      GotoIndex1;                      
extern unsigned char     Or_Cond;
extern unsigned char     Ok_If;

extern int      count;                        
extern int      LineNo;                                           

extern int      count_file;                       /* input line count              */
extern int      count_cmd_in;                     /* input line count              */
extern int      count_debug_out;                  /* input line count              */

extern long int backcolor;                        /* backround color for outtext   */
extern int      nrmcolor;                         /* normal color for outtext      */
extern int      highcolor;                        /* high color for outtext        */
extern int      warncolor;                        /* warning color for outtext     */
extern int      warn2color;                       /* warning 2 color for outtext   */
extern int      row;                              /* row for outtext               */
extern int      col;                              /* column for outtext            */
extern int      hrow;                             /* hold row for display          */
extern int      hcol;                             /* hold col for display          */

extern long int Number;
extern long int Number2;
extern long int Tnum1;
extern long int Tnum2;
extern unsigned char     *p;
extern int      mode;
extern int      RC;
extern unsigned long   ulSleep;

extern unsigned char     *szScanStr;
extern unsigned char     *szScanLeft;
extern unsigned char     *szScanRight;
extern unsigned char     *szScanTest;
extern unsigned char     *szScanMid;
extern unsigned char     *szLEFT;
extern unsigned char     *szScanKeep;
extern unsigned char     *szLword;
extern unsigned char     *szRword;
extern int      Lwordindex;                 
extern unsigned char     cScanFound;
extern unsigned char     cScanDone;
extern unsigned char     cScanFromRight;
extern unsigned char     Scanr;
extern unsigned char     cScanChar1;
extern unsigned char     cScanChar2;
extern unsigned char     cCase;
extern int      ScanIndex1;
extern int      ScanIndex2;
extern unsigned char     szCursorCmd   [10];
extern unsigned char     Direction;
extern int      Max;
extern int      iScramble;
extern unsigned char     szEncryptKey  [100];
extern unsigned char     szDefaultKey [];

extern unsigned char     *envptr;
extern unsigned char     *envstr;
extern unsigned char     Break;
extern unsigned char     BreakStatus;
extern unsigned char     If_Proc;
extern unsigned char     Dountil;
extern unsigned char     szIfName [100];
extern int      dCallFileLevel;
extern unsigned char     Callfile;
extern unsigned char     cLocal;
extern unsigned char     cArg;
extern unsigned char     cLocalVars;
extern int      CallLevel;
extern unsigned char     szCurrentFile [100];

extern unsigned char     szMainTitle   [100];

extern unsigned char     Beditwin;
extern short    sInterval;

//extern HAB       hab;
//extern HMQ       hmq;
//extern QMSG      qmsg;
extern HWND      BEDITframe;
extern HWND      BEDITclient;
//extern ULONG     frameflags;
//extern SWP       swp;
//extern HSEM      hsemApp;
//extern USHORT    usRc;
extern HWND      hwndMain;
//extern HMODULE   hmod;
//extern LONG      FontsAvail;
//extern FATTRS    vfat;
//extern LONG      FontSize;
extern LONG      FixedFont;
extern unsigned char      szCountry [30];
extern unsigned char      cAutotab;
extern int       XMouse;
extern int       YMouse;
extern unsigned char      cMouse;
extern char     szPointer [20];
extern int ExecFullScreen;

extern HANDLE              hInstance;
extern HANDLE              hPrevInstance;
extern char szAppName [];
extern WNDCLASS            wndclass;
extern WNDCLASS            wndclass2;
extern RECT                rect;
extern HMENU               hMenu;
extern HMENU               hMenuPopup;
extern MSG                 msg;
extern int                 nCmdShow;
extern char                cUpdateCursor;
extern FARPROC             lpfnSubClass;

extern char szBeditSecondary;

extern FILE     *file_cmd_in;                     /* command file                  */
extern FILE     *file_debug_out;                  /* command file                  */
extern FILE     *file_ot;                         /* write file                    */

extern INTERACTIVE StartInteractive;
extern INTERACTIVE *pInteractive;
extern INTERACTIVE *pNewInteractive;
extern INTERACTIVE *pCurrInteractive;

extern CALL StartCall;
extern CALL *pCall;
extern CALL *pNewCall;
extern CALL *pPrevCall;
extern CALL *pNextCall;
extern CALL *pHoldCall;
extern CALL *pCountCall;

extern VARIABLE StartVariable;
extern VARIABLE *pVariable;
extern VARIABLE *pEnumVar;
extern VARIABLE *pVariableRecursive;
extern VARIABLE *pHoldVariable;
extern VARIABLE *pNewVariable;
extern VARIABLE *pPrevVariable;
extern VARIABLE *pNextVariable;
extern VARIABLE *pAssignVar;
extern VARIABLE *pVar1;
extern VARIABLE *pVar2;
extern VARIABLE *pCountVariable;

extern COMMAND StartCommand;
extern COMMAND *pCommand;
extern COMMAND *pNewCommand;
extern COMMAND *pHoldCommand;
extern COMMAND *pStartCommand;
extern COMMAND *pStartCommandBEDIT;
extern COMMAND *pCommandBEDIT;
extern COMMAND *pLabel;
extern COMMAND *pPMCommand;
extern COMMAND *pLastCallGoto;
extern COMMAND *pDountilcommand;

extern PERMENANT StartPermenant;
extern PERMENANT *pPermenant;
extern PERMENANT *pNewPermenant;

extern BUFFER StartBuffer;
extern BUFFER *pBuffer;
extern BUFFER *pNewBuffer;
extern BUFFER *pCountBuffer;

extern DATAFILE StartDataFile;
extern DATAFILE *pDataFile;
extern DATAFILE *pNewDataFile;
extern DATAFILE *pCurrData;
extern DATAFILE *pPrevData;
extern DATAFILE *pNextData;
extern DATAFILE *pCountDataFile;

extern OPENFILE StartOpenFile;
extern OPENFILE *pOpenFile;
extern OPENFILE *pNewOpenFile;
extern OPENFILE *pNextFile;
extern OPENFILE *pPrevFile;
extern OPENFILE *pCountFiles;
extern OPENFILE *pOpenCurr;

extern OBJECT StartObject;
extern OBJECT *pObject;
extern OBJECT *pNewObject;
extern OBJECT *pNextObject;
extern OBJECT *pPrevObject;
extern OBJECT *pCountObject;

extern ARGS   StartArgs;
extern ARGS   *pArgs;
extern ARGS   *pNextArgs;
extern ARGS   *pPrevArgs;

extern int WindowCount;

/**********************************************/
/*             End of BEDITVAR.H              */
/**********************************************/

