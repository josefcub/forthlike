/*
 * I_BRANCH.C - Branching and other flow-control primitives. 
 *
 * Functions:
 *    do_if do_begin do_break do_continue do_while do_repeat 
 *    do_until do_abort
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "strutil.h"
#include "list.h"
#include "debug.h"
#include "preload.h"
#include "interp.h"
#include "i_manip.h"

/* For the looping primitives: We don't need these outside of *
 *                             this file.                     */                                                
struct loopinfo 
{
  struct inst *bol;   /* Beginning of loop */
  struct inst *eol;   /* End of the loop  */
} loopstack[MAXNEST]; /* Our list of loops running. */

static int currentloop = 0;

/*
 * do_if - 'IF' flow-control statement. do_if assumes word
 *         passed is "IF".  This also handles the primitives
 *         "THEN" and "ELSE"
 */
struct inst *do_if(struct inst *prim)
{
  static int iflevel = 0;
  struct inst *endif = NULL;
  struct inst *ifelse = NULL;
  
  /* For nesting conditions */
  struct inst *counter = prim->next;
  int count = 1;

  /* Range/type check */
  if (depth < 1) error("IF", ERR_SUNDERF);
  if (stack->type != NUM) error("IF", ERR_ARGT);
  if (iflevel == MAXNEST) error("IF", ERR_NESTING);

  /* Set variables for later use */
  iflevel++;
  line = prim->line;

  /* Setup - Track THENs from the current IF to the last one   */
  /*         in this block, so we return in the correct place. */
  while (count != 0)
  {
    if (!strcmp(counter->inst, "IF"))
    {
      count++;
    }
    if (!strcmp(counter->inst, "ELSE"))
    {
      if (count == 1) ifelse = counter;  /* Only this IF's else */
    }
    if (!strcmp(counter->inst, "THEN"))
    {
      endif = counter;
      count--;
    }
    if (!strcmp(counter->inst, ";")) error("IF", ERR_UEOD);
    
    counter = counter->next;
  }
  
  /* Make the comparison */
  count = atoi(stack->data);
  do_pop();

  if (count)
  {
    if (ifelse != NULL)
      do_word_until(prim, ifelse);
    else
      do_word_until(prim, endif);
  }
  else
  {
    if (ifelse != NULL)
    {
      do_word_until(ifelse, endif);
    }
  } 
  
  iflevel--;
  
  return(endif);
}

/*
 * do_begin - Handles starting the loop.  Just assign the loop
 *            stack values appropriately, and go.  This is the
 *            only loop function that doesn't handle its own
 *            errors.
 * 
 *      NOTE: Internally, the loop support primitives could be
 *            thought of like this:
 *              $def repeat continue
 *              $def while not if break then
 *              $def until if break else continue then 
 *  
 *      NOTE: Must fix that in the future.
 */
int do_begin(struct inst *prim)
{
  /* Temporary variables */
  int count = 1;
  struct inst *counter = prim->next;

  /* Author's note:  This IS accumulative across procedures. */
  if (currentloop == MAXNEST) return(ERR_NESTING);
  currentloop++;

  loopstack[currentloop].bol = prim;

  /* Setup - Track the loop from the current begin to the end of */
  /*         the whole loop.                                     */
  while (count != 0)
  {
    if (!strcmp(counter->inst, "BEGIN"))
    {
      count++;
    }
    if (!strcmp(counter->inst, "UNTIL"))
    {
      if (count == 1) loopstack[currentloop].eol = counter;
      count--;
    }
    if (!strcmp(counter->inst, "REPEAT"))
    {
      if (count == 1) loopstack[currentloop].eol = counter;  
      count--;
    }
    if (!strcmp(counter->inst, ";")) return(ERR_UEOD);
    
    counter = counter->next;
  }
  
  return(0);
}

/* 
 * do_break - Break out the loop immediately, return to after
 *            the EOL statement.
 */
struct inst *do_break(struct inst *prim)
{
  struct inst *eol = NULL;

  /* Range/Type/Syntax Checks */
  if (currentloop == 0) error(prim->inst, ERR_SYNTAX);
  if (prim->top != loopstack[currentloop].bol->top)
    error(prim->inst, ERR_SYNTAX); 

  eol = loopstack[currentloop].eol;
  
  currentloop--;

  return(eol);
}

/* 
 * do_continue - Advance to Go, Collect $200.  Jump to BOL.
 */
struct inst *do_continue(struct inst *prim)
{
  struct inst *bol = NULL;

  /* Range/Type/Syntax Checks */
  if (currentloop == 0) error(prim->inst, ERR_SYNTAX);
  if (prim->top != loopstack[currentloop].bol->top)
    error(prim->inst, ERR_SYNTAX); 

  bol = loopstack[currentloop].bol;

  return(bol);
}

/*
 * do_while - if false, end the loop.
 */
struct inst *do_while(struct inst *prim)
{
  int result = 0;
  struct inst *eol = NULL;
      
  /* Range/Type/Syntax Checks */
  if (currentloop == 0) error(prim->inst, ERR_SYNTAX);
  if (prim->top != loopstack[currentloop].bol->top)
    error(prim->inst, ERR_SYNTAX); 

  if (!depth) error(prim->inst, ERR_SUNDERF);
  if (stack->type != NUM) error(prim->inst, ERR_ARGT);

  result = atoi(stack->data);
  do_pop();

  /* If it's false, end the loop */
  if (!result)
  {
    eol = loopstack[currentloop].eol;
    currentloop--;  /* Decrement the current loop level */
    return(eol);
  }

  return(prim);
}

/* 
 * do_repeat - Repeat from the top.
 */
struct inst *do_repeat(struct inst *prim)
{
  /* Range/Syntax Checks */
  if (currentloop == 0) error(prim->inst, ERR_SYNTAX);
  if (prim->top != loopstack[currentloop].bol->top)
    error(prim->inst, ERR_SYNTAX);

  /* If we survive this far, return the true BOL. */
  return(loopstack[currentloop].bol);
}

/*
 * do_until - If value is false, repeat the loop.
 */
struct inst *do_until(struct inst *prim)
{
  int result = 0; /* Result on the stack */
  struct inst *bol = NULL;  /* BOL pointer */

  /* Range/Type/Syntax Checks */
  if (currentloop == 0) error(prim->inst, ERR_SYNTAX);
  if (prim->top != loopstack[currentloop].bol->top)
    error(prim->inst, ERR_SYNTAX);
  
  if (!depth) error(prim->inst, ERR_SUNDERF);
  if (stack->type != NUM) error(prim->inst, ERR_ARGT);

  result = atoi(stack->data);
  do_pop();

  /* If it's false, keep going in the loop */
  if (!result)
  {
    bol = loopstack[currentloop].bol;
    return(bol);
  }
  
  /* Decrement the current loop level */
  currentloop--;

  return(prim);
}

/*
 * do_abort - Aborts the interpreter with the user-defined message.
 */
int do_abort()
{
  char *msg = (char *)malloc(strlen(stack->data) + 1);

  /* Range/Type Checks */
  if (msg == NULL) return(ERR_MEM);
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);
  
  msg = strcpy(msg, stack->data);
  do_pop();

  abort_fll("ABORT", msg);

  /* Just to avoid compiler warnings */
  return(0);
}
