/*
 * GLOBALS.H - Include globally-used #defines.
 *
 * Functions:
 *    none
 */

/* Global Defines */

/* Interpreter version */
#define VERSION       "Alpha 0.4"

/*
 * Uncomment this if you want the 'dump' primitive in the language, and 
 * if you want to see a stack dump when an error occurs.
 */
#define DEBUG

/* Maximum buffer size */
#define MAXBUF        1024

/* Maximum stack depth */
#define MAXSTACK      512

/* Maximum nesting depth */
#define MAXNEST       128

/*
 * Don't change anything below this comment line.
 */

/* Constants for identifying stack item types */
#define NOTHING       0
#define STRING        1
#define NUM           2
#define VARIABLE      3 

/* Error codes */
#define ERR_ARGT     -1   /* Argument type          */
#define ERR_BUFOVER  -2   /* Buffer Overflow        */
#define ERR_DBZ      -3   /* Divide by zero         */
#define ERR_DUPDEF   -4   /* Duplicate definition   */
#define ERR_MEM      -5   /* Out of Memory          */
#define ERR_NESTING  -6   /* Nested too deep        */
#define ERR_NOPROC   -7   /* No procedures defined  */
#define ERR_NOTHEN   -8   /* IF without THEN        */
#define ERR_PEOF     -9   /* Premature End-Of-File  */
#define ERR_POSINT   -10  /* Not a Positive Integer */
#define ERR_SOVERF   -11  /* Stack Overflow         */
#define ERR_SUNDERF  -12  /* Stack Underflow        */
#define ERR_SYNTAX   -13  /* Syntax Error           */
#define ERR_TOOPROC  -14  /* Too many procedures    */
#define ERR_UEOD     -15  /* Unexpected End of Def. */
#define ERR_FNF      -16  /* File Not Found         */
#define ERR_ESA      -17  /* Empty String Argument  */

/* End of Global Defines */
