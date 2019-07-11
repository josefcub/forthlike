/*
 * PRELOAD.C - This is the file preloader.
 *
 * Functions:
 *    preload
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "strutil.h"
#include "list.h"
#include "debug.h"
#include "parser.h"
#include "preload.h"
#include "interp.h"


struct inst wordlist[MAXSTACK];   /* For preload, interp, do_word */
int lastword = 0;                 /* For the same, it keeps track */
                                  /* of the last word in the list */
int line = 1;                     /* For debug.c, interp.c and   
                                     parser.c                     */

/*
 * preload - Load the file into linked lists in preparation 
 *           for execution.
 */
int preload(FILE *filename)
{
  struct inst *prim = NULL;
  char *word = NULL;
  FILE *includefile = NULL;
  int count = 0;
  char input = 0;
  
  /* Loading Loop */
  do
  {
    /* Start the getting */
    word = parse(filename);
    
    if (word == NULL) break;
    
    /* "$def" is ignored as a matter of necessity. */
    if (!strcmp(word, "$DEF"))
    { 
      input = 0;
      while (input != 10) input = fgetc(filename);
      line++;
      
      continue;
    }

    /* Attempt re-entrance for $include */
    if (!strcmp(word, "$INCLUDE"))
    {
    
      /* Get the name of the source file to include. */
      word = parse(filename);
    
      /* Basic sanity checks */
      if (word == NULL) return(ERR_PEOF);
      if (word[strlen(word) - 1] == 34) return(ERR_SYNTAX);
      
      /* Convert to lowercase and attempt to open our source file. */
      word = strtolower(word);
      includefile = fopen(word, "r");
      if (includefile == NULL) return(ERR_FNF);
      
      /* Load the program into memory */
      fprintf(stdout, "Including %s\n", word);
      error("INCLUDE", preload(includefile));
      
      continue;
    }

   /* Is it a procedure definition? */
    if (!strcmp(word, ":"))
    {
      /* Get the name of the defined word */
      word = parse(filename);
      
      /* Allowable name checks */
      if (word == NULL) return(ERR_PEOF);
      if (word[strlen(word) - 1] == 34) return(ERR_SYNTAX);
      if (is_integer(word)) return(ERR_SYNTAX);
      
      /* Check for duplicates */
      for (count = 1; count <= lastword; count++)
        if (!strcmp(word, wordlist[count].inst)) return(ERR_DUPDEF);

      /* Overflow check */
      lastword++;
      if (lastword == MAXSTACK) return(ERR_TOOPROC);

      /* Set up the list */
      wordlist[lastword].inst = word;
      wordlist[lastword].line = line;
      
      /* Allocate the first item */
      wordlist[lastword].next = (struct inst *)malloc(sizeof(struct inst));
      if (wordlist[lastword].next == NULL) return(ERR_MEM);
      prim = wordlist[lastword].next;

      /* Retrieve our primitives */
      while (strcmp(word, ";"))
      {
      
        /* Actual Retrieval */
        word = parse(filename);

        /* Syntax checks */
        if (word == NULL) return(ERR_PEOF);
        if (!strcmp(word, ":")) return(ERR_SYNTAX);
        
        /* Store the primitive */
        prim->inst = word;
        prim->line = line;
        prim->next = (struct inst *)malloc(sizeof(struct inst));
	      if (prim->next == NULL) return(ERR_MEM);
        prim->top = &wordlist[lastword];
      
        /* Initialize next item */      
        prim = prim->next;
        prim->inst = NULL;
        prim->line = 0;
        prim->next = NULL;
      }

    } else return(ERR_SYNTAX); /* No primitives outside of */
                               /* procedure definitions.   */
  } 
  while (word != NULL);

/*  if (lastword == 0) return(ERR_NOPROC); */ /* Need at least one */ 

  return(0);
}

