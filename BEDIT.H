/***********************************************/
/* Windows System Global Headers               */
/***********************************************/
#include <windows.h>

/***********************************************/
/* Define Maximum String Characters to Process */
/***********************************************/
#define MAX 1000

/****************************************************************************/
/* Interactive Command Stack Structure                                      */
/*     This structure contains all of the interactive commands entered.     */
/*     This allows the use of the cursor up and down keys to scroll through */
/*     previous commands.                                                   */
/*                                                                          */
/****************************************************************************/
typedef struct _INTERACTIVE
{
    struct _INTERACTIVE *pNext;
    struct _INTERACTIVE *pPrev;
    unsigned char   *szCmd;
} INTERACTIVE;

/****************************************************************************/
/* Call Stack Structure                                                     */
/*     All information about the current call location is placed here to    */
/*     allow RETURNing. The Start Command points to the beginning of the    */
/*     current Command structure. The Prev Start Command points to the      */
/*     beginning of the Command structure that called the current Command   */
/*     structure.                                                           */
/*                                                                          */
/****************************************************************************/
typedef struct _CALL
{
    struct _COMMAND *pCommand;
    struct _COMMAND *pStartCommand;
    struct _COMMAND *pPrevStartCommand;
    struct _VARIABLE *pVariable;
    unsigned char   *input_line;
    unsigned char   szFilename            [100];
    int    index1;
    unsigned char   More_Commands;
    unsigned char   If_Proc;
    unsigned char   EOF_Flag;
    unsigned char   cPerm;
    struct _CALL    *pNext;
    struct _CALL    *pPrev;
} CALL;

/****************************************************************************/
/* Variable's Structure                                                     */
/*     All variables reside here including the standard system variables.   */
/*                                                                          */
/****************************************************************************/
typedef struct _VARIABLE
{
    unsigned char   szName   [100];
    int    Integer;
    double Float;
    unsigned char   *szString;
    unsigned char   Saved;
    unsigned char   *szSaveString;
    unsigned char   Type;
    unsigned char   *szAction;
    unsigned char   szEdit   [10];
    short  sSize;
    int    LocalCallLevel;
    struct _VARIABLE *pNext;
    struct _VARIABLE *pPrev;
} VARIABLE;

/****************************************************************************/
/* Object Structure                                                         */
/*     This structure contains a list of all objects for PM Applications.   */
/*                                                                          */
/****************************************************************************/
typedef struct _OBJECT
{
    unsigned char    szName     [100];
    unsigned char    szType     [10];
    unsigned char    szEdit     [10];
    unsigned char    szWindow   [100];
    unsigned char    szText     [100];
    unsigned char    *szDraw;

    HWND    hwnd;
    HWND    hwndClient;
    SHORT   x;
    SHORT   y;
    SHORT   cx;
    SHORT   cy;
    USHORT  id;
    ULONG   flFrameFlags;
    ULONG   flStyle;
    unsigned char    cVisible;
    unsigned char    cEnabled;
    WNDPROC   OldProc;
//    LONG    lFontSize;
//    LONG    lFont;
//    HBITMAP hbm;

    struct  _OBJECT    *pNext;
    struct  _OBJECT    *pPrev;
    struct  _OBJECT    *pNextWin;
    struct  _OBJECT    *pNextWinObj;
} OBJECT;

/****************************************************************************/
/* Command File Structure                                                   */
/*     This is where the BEDIT batch command file is stored. Also, any      */
/*     interactive command is placed here and executed.                     */
/*                                                                          */
/****************************************************************************/
typedef struct _COMMAND
{
    unsigned char   *szLine;
    int    LineNo;
    struct _COMMAND *pNext;
    struct _COMMAND *pPrev;
    struct _COMMAND *pNextLabel;
    struct _COMMAND *pLastCallGoto;
    VARIABLE *pVar1;
    VARIABLE *pVar2;
    VARIABLE *pVar3;
    OBJECT   *pObj1;
    OBJECT   *pObj2;
    OBJECT   *pObj3;
} COMMAND;

/****************************************************************************/
/* Permanent CALLed functions                                               */
/*     This is where the Permenant CALLed functions pointers reside.        */
/*	   If a CALLed routine is defined as PERManent, the routine will not*/
/*	   be released from memory after the corresponding RETURN. Any CALLs*/
/*	   made to a PERManent routine after it has been loaded will not read*/
/*	   the routine from disk.					    */
/*                                                                          */
/****************************************************************************/
typedef struct _PERMANENT
{
    unsigned char   szName                [100];
    unsigned char   cLoaded;
	struct _COMMAND *pStartCommand;
	struct _PERMANENT *pNext;
	struct _PERMANENT *pPrev;
} PERMENANT;

/****************************************************************************/
/* Cut/Copy Buffer Structure                                                */
/*     This is where all Cut/Copy data is written or read.                  */
/*                                                                          */
/****************************************************************************/
typedef struct _BUFFER
{
    unsigned char   szLine    [MAX];
    struct _BUFFER *pNext;
    struct _BUFFER *pPrev;
} BUFFER;

/****************************************************************************/
/* Data File Structure                                                      */
/*     When a file is OPENed a new Data File structure is created and the   */
/*     file is read into the structure. All files open are read completely  */
/*     into memory.                                                         */
/*     A pointer to the current line is held in the Open File structure.    */
/*                                                                          */
/****************************************************************************/
typedef struct _DATAFILE
{
    unsigned char   *szLine;
    unsigned char   *szBookmark;
    unsigned char   Begin;
    struct _DATAFILE *pNext;
    struct _DATAFILE *pPrev;
} DATAFILE;

/****************************************************************************/
/* Open File Structure                                                      */
/*     This structure contains a list of all open files. pOpenFile points   */
/*     to the current open file. pData points to the current line of the    */
/*     file.                                                                */
/*                                                                          */
/****************************************************************************/
/**************************************/
/* Open File Structure                */
/**************************************/
typedef struct _OPENFILE
{
    unsigned char   szFile      [100];
    unsigned char   szFileName  [100];
    FILE   *File;
    struct _DATAFILE *pData;
    struct _OPENFILE *pNext;
    struct _OPENFILE *pPrev;
} OPENFILE;

/****************************************************************************/
/* Arguments Structure                                                      */
/*     Contains all Arguments passed in a CALL statement                    */
/*                                                                          */
/****************************************************************************/
typedef struct _ARGS
{
    struct _ARGS *pNext;
    struct _ARGS *pPrev;
    unsigned char   *szArg;
} ARGS;

/**************************************/
/* Prototypes                         */
/**************************************/
long FAR PASCAL BeditwinMainWindProc(HWND, UINT, WPARAM, LPARAM);
long FAR PASCAL BeditwinSecondaryWindProc(HWND, UINT, WPARAM, LPARAM);
void PaintObjects(HWND, UINT);
void b100_init (void);
void b200_proc (void);
void b300_wrap (void);
void c100_read_cmd_in (void);
void c110_proc_next_cmd (void);
int  c115_check_cond (char *);
void c120_parse_command (void);
void c200_process_command (void);
void d100_process_read (void);
void d101_find_file (void);
void d105_filesize (void);
void d106_filestring (void);
void d110_process_write (void);
void d112_process_file (void);
void d113_close_file (void);
void d114_goto (void);
void d115_process_print (void);
void d116_if (void);
void d117_partial_var (void);
void d118_cursor (void);
void d119_interactive (void);
void d120_process_B (void);
void d121_color (void);
void d122_backcolor (void);
void d123_bell (void);
void d124_bookmark (void);
void d125_case (void);
void d126_char (void);
void d127_eol (void);
void d128_data (void);
void d129_scramble (void);
void d130_process_P (void);
void d135_encryptkey (void);
void d140_process_F (void);
void d150_process_FS (void);
void d160_insert_line (void);
void d165_insert_mode (void);
void d170_process_D (void);
void d175_subs (void);
void d176_pattern (void);
void d180_process_system (void);
void d181_process_systemcurr (void);
void d182_sort (void);
void d185_cols (void);
void d190_process_RV (void);
void d200_process_R (void);
void d210_fixfile (void);
void d250_front (void);
void d260_back (void);
void d270_perm (void);
void d300_clear_buffer (void);
void d310_copy_buffer (void);
void d320_paste_buffer (void);
void d330_dump_buffer (void);
void d350_list (void);
void d360_beginend (void);
void d370_status (void);
void d400_call (void);
void d401_find_file (void);
void d405_call_args (void);
void d410_return (void);
void d500_integer (void);
void d510_string (void);
void d520_float (void);
void d525_local_args (void);
void d530_erase (void);
void d540_initialize (void);
void d580_find_variable (void);
void d590_find_last_variable (void);
void d598_files (void);
void d599_dump (void);
void d600_variable (void);
void d610_variable_integer (void);
void d620_variable_dollar (void);
void d700_input (char);
void d710_sleep (void);
void d715_waitbell (void);
void d720_chain (void);
void d800_scan (void);
void d900_split (void);
void d950_extract (void);
void e100_size (void);
void e102_random (void);
void e105_encrypt (int, char);
void e106_encrypt_file (int);
void e110_pack (void);
void e120_crush (void);
void e130_right (void);
void e140_left (void);
void e150_mix (void);
void e160_upper (void);
void e170_lower (void);
void e200_save (void);
void e210_restore (void);
void e220_save_variable (void);
void e230_restore_variable (void);
void f100_next (void);
void f110_prev (void);
void g100_convert_variable (void);
void g200_convert_string (void);
void h100_help (char *);
void u100_user (char *, char *, char *);
void w100_heading (void);
void w100_test_Beditwin (void);
void w100_window (void);
void w110_window_start (void);
void w120_window_stop (void);
void x100_break (int);
void y100_input_string (void);
void y200_input_interactive (void);
void y210_backup (void);
void y250_interactive_cmds (void);
void y500_input_string (char);
void z010_syntax_error (void);
void z100_error (void);
void z110_fatal (void);
void z111_fatal_message (void);
void z200_reset_file_position (void);
void z300_store_file_position (void);
void z900_scan (void);
void z905_scramble(void);
void z910_scan_reverse (void);
void z998_winexec (char *);
void z999_open_bug (void);

