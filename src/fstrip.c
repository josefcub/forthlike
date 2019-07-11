/*
 * FSTRIP.C - Strips comments and whitespace from a file.
 *
 * Functions:
 *    main
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main(int argc, char *argv[])
{
  FILE *source = NULL;  /* The file to read in */
  FILE *dest = NULL;    /* The file to write out */
  char *word = NULL;    /* The parser's output */
  char *s1 = NULL;      /* Temporaries for parser */
  
  fprintf(stdout, "Forth-like Language Interpreter File Stripper\n");

  /* Argument check. */
  if (argc < 3)
  {
    fprintf(stdout, "ERROR: Two filenames required.\n\n");
    fprintf(stdout, "Usage:\n  %s <input file> <output file>\n", argv[0]);
    return(0);
  }

  /* Attempt to open our source file. */
  source = fopen(argv[1], "r");
  if (source == NULL)
  {
    fprintf(stdout, "ERROR: File not found.\n\n");
    return(0);
  }

  /* Attempt to open our destination file */
  dest = fopen(argv[2], "w");
  if (dest == NULL)
  {
    fprintf(stdout, "ERROR: File creation error.\n\n");
    return(0);
  }

  /* Finish the banner */
  fprintf(stdout, "Copying code from file %s into file %s...\n", argv[1], argv[2]);

  /* Do the strip */
  do
  {
    word = parse(source);
    if (word == NULL) break;

    /* Check for a parsed string, to fix it. */
    if (word[strlen(word) - 1] == 34) 
    {
      /* Add the double-quote */
      s1 = (char *)malloc(strlen(word) + 2); /* For extra dquote */   
      
      /* Add that dquote */
      s1[0] = 34;
      
      /* Increment and copy */
      s1++;
      strcpy(s1, word);
      s1--;
      
      /* Free and reassign char *word */
      free(word);
      word = s1;
    }

    fprintf(dest, "%s ", word);
    free(word);
  }
  while (word != NULL);
  
  /* Close the files, as they are no longer needed */
  fclose(source);
  fclose(dest);
 
  /* Bye! */
  fprintf(stdout, "Done.\n");
  return(0);
}
