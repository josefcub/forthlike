/*
 * DEBUG.C - Debugging and error handling routines.
 *
 * Functions:
 *   abort_fll dump (dump) error
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "list.h"
#include "preload.h"


#ifdef DEBUG
/*
 * dump() - dumps the (formatted) contents of the
 *          stack to stdout.  This version runs if
 *          DEBUG is uncommented in GLOBALS.H.
 */
int dump(struct item *stack)
{
  struct item *temp = listtail(stack);
  
  fprintf(stdout, "Stack Dump: %d items.\n", depth);
  
  if (stack != NULL)
  {
    /* Print out the stack item, according to the type */
    while (temp != NULL)
    {
      switch (temp->type)
      {
        case NOTHING:
        {
          fprintf(stdout, "  *** Internal Error - Nothing here! ***\n");
          break;
        }
        case STRING:
        {
  	      fprintf(stdout, "  \"%s\"\n", temp->data);
          break;
        }
        case NUM:
        {
          fprintf(stdout, "  %s\n", temp->data);
          break;
        }
        case VARIABLE:
        {
          fprintf(stdout, "  V%s\n", temp->data);
          break;
        }
      }
      temp = temp->next;
    }
  }

  return(0);
}
#endif

#ifndef DEBUG
/*
 * dump - This version does nothing, because DEBUG is not defined.
 */
int dump(struct item *stack)
{
  return(0);
}
#endif

/*
 * abort_fll - Aborts the interpreter with the given message.
 */
void abort_fll(char *location, char *errm)
{
    fprintf(stdout, "Program Error. Your program just got the following error:\n");
    fprintf(stdout, "Line %d; %s: %s.\n", line, location, errm);
    dump(stack);
    exit(1);
}

/*
 * error - Display an error message for the user, dump
 *         the stack to stdout, and exit.  If errn is
 *         zero, just return to where we're called from.
 */
void error(char *location, int errn)
{
  char *errm = NULL;  /* The error message */

  /* Which error was it? See GLOBALS.H */
  switch (errn)
  {
    case ERR_ARGT:    errm = "Argument type mismatch"; break;
    case ERR_BUFOVER: errm = "Buffer overflow"; break;
    case ERR_DBZ:     errm = "Divide by zero"; break;
    case ERR_DUPDEF:  errm = "Duplicate definition"; break; 
    case ERR_MEM:     errm = "Out of memory"; break;
    case ERR_NESTING: errm = "Nesting level exceeded"; break;
    case ERR_NOPROC:  errm = "No initial procedure found"; break;
    case ERR_NOTHEN:  errm = "IF without THEN"; break;
    case ERR_PEOF:    errm = "Premature end of file"; break;
    case ERR_POSINT:  errm = "Operand not a positive integer"; break;
    case ERR_SOVERF:  errm = "Stack overflow"; break;
    case ERR_SUNDERF: errm = "Stack underflow"; break;
    case ERR_SYNTAX:  errm = "Syntax error"; break;
    case ERR_TOOPROC: errm = "Too many words defined"; break;
    case ERR_UEOD:    errm = "Unexpected end of definition"; break;
    case ERR_FNF:     errm = "File not found"; break;
    case ERR_ESA:     errm = "Empty string argument"; break;
  }
  
  /* Print out our error message and exit, if applicable */
  if (errn < 0) abort_fll(location, errm);
}
