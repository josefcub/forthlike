/*
 * FLL.C - Primary file for compilation, a Forth-like interp-
 *         reted language that shares the syntax and a subset
 *         of primitives with the MUCKer Forth language.
 *
 * Functions:
 *    main
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"           /* For VERSION */
#include "list.h"
#include "debug.h"
#include "preload.h"
#include "interp.h"

int main(int argc, char *argv[])
{
  FILE *filename;  /* The file to execute */

  /* Display the banner */
  fprintf(stdout, "Forth-like Language Interpreter %s\n", VERSION);

  /* Argument check. */
  if (argc < 2)
  {
    fprintf(stdout, "ERROR: Filename required.\n\n");
    fprintf(stdout, "Usage:\n  %s <filename>\n", argv[0]);
    return(0);
  }

  /* Attempt to open our source file. */
  filename = fopen(argv[1], "r");
  if (filename == NULL)
  {
    fprintf(stdout, "ERROR: File not found.\n\n");
    return(0);
  }

  /* Load the program into memory */
  error("PRELOAD", preload(filename));

  /* Close the file, as it is no longer needed */
  fclose(filename);

  /* Do we have anything to execute? */
  if (lastword == 0) return(ERR_NOPROC); /* Need at least one */
    

  /* Initialize random number generator for the interpreter */
  srand((unsigned) time(NULL));

  /* Finally, run the program */
  error("FLL", do_word(&wordlist[lastword]));

  /* Bye! */
  return(0);
}
