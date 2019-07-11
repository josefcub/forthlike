/*
 * INTERP.C - Contains the actual interpretation routines.
 *
 * Functions:
 *    interp trypush tryword endofword do_word do_word_until
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "globals.h"
#include "strutil.h"
#include "list.h"
#include "debug.h"
#include "preload.h"
#include "interp.h"
#include "i_arith.h"
#include "i_branch.h"
#include "i_manip.h"
#include "i_misc.h"
#include "i_string.h"

/* For the EXIT primitive: Available this file only. */
static jmp_buf jmpstack[MAXNEST];
int word_depth = 1;  /* Always in one word at least */

/*
 * interp - Interpret the string prim->inst.  This only inter-
 *          prets the primitives that are NOT flow-control
 *          related.
 */
int interp(char *incoming)
{
  #ifdef DEBUG
  /* DEBUG.C - Debugging primitives            */
  if (!strcmp(incoming, "DUMP"))   return(dump(stack));
  #endif

  /* I_MANIP.C - Stack manipulation primitives */
  if (!strcmp(incoming, "DEPTH"))     return(do_depth());
  if (!strcmp(incoming, "DUP"))       return(do_dup());
  if (!strcmp(incoming, "OVER"))      return(do_over());
  if (!strcmp(incoming, "PICK"))      return(do_pick());
  if (!strcmp(incoming, "POP"))       return(do_pop());
  if (!strcmp(incoming, "PUT"))       return(do_put());
  if (!strcmp(incoming, "ROT"))       return(do_rot());
  if (!strcmp(incoming, "ROTATE"))    return(do_rotate());
  if (!strcmp(incoming, "SWAP"))      return(do_swap());

  /* I_STRING.C - String related primitives    */
  if (!strcmp(incoming, "EXPLODE"))   return(do_explode());
  if (!strcmp(incoming, "INSTR"))     return(do_instr());
  if (!strcmp(incoming, "INSTRING"))  return(do_instring());
  if (!strcmp(incoming, "INTOSTR"))   return(do_intostr());
  if (!strcmp(incoming, "RINSTR"))    return(do_rinstr());
  if (!strcmp(incoming, "RINSTRING"))  return(do_rinstring());
  if (!strcmp(incoming, "STRCAT"))    return(do_strcat());
  if (!strcmp(incoming, "STRCMP"))    return(do_strcmp());
  if (!strcmp(incoming, "STRCUT"))    return(do_strcut());
  if (!strcmp(incoming, "STRING?"))   return(do_stringq());
  if (!strcmp(incoming, "STRINGCMP")) return(do_stringcmp());
  if (!strcmp(incoming, "STRINGPFX")) return(do_stringpfx());
  if (!strcmp(incoming, "STRIP"))     return(do_strip());
  if (!strcmp(incoming, "STRIPLEAD")) return(do_striplead());
  if (!strcmp(incoming, "STRIPTAIL")) return(do_striptail());
  if (!strcmp(incoming, "STRLEN"))    return(do_strlen());
  if (!strcmp(incoming, "STRNCMP"))   return(do_strncmp());
  if (!strcmp(incoming, "TOLOWER"))   return(do_tolower());
  if (!strcmp(incoming, "TOUPPER"))   return(do_toupper());

  /* I_ARITH.C - Arithmetic related primitives */
  if (!strcmp(incoming, "+"))         return(do_add());
  if (!strcmp(incoming, "-"))         return(do_sub());
  if (!strcmp(incoming, "*"))         return(do_mul());
  if (!strcmp(incoming, "/"))         return(do_div());
  if (!strcmp(incoming, "%"))         return(do_mod());
  if (!strcmp(incoming, ">"))         return(do_gthan());
  if (!strcmp(incoming, ">="))        return(do_gteq());
  if (!strcmp(incoming, "<"))         return(do_lthan());
  if (!strcmp(incoming, "<="))        return(do_lteq());
  if (!strcmp(incoming, "="))         return(do_iseq());
  if (!strcmp(incoming, "AND"))       return(do_and());
  if (!strcmp(incoming, "ATOI"))      return(do_atoi());
  if (!strcmp(incoming, "INT"))      return(do_atoi());
  if (!strcmp(incoming, "BITAND"))    return(do_bitand());
  if (!strcmp(incoming, "BITOR"))     return(do_bitor());
  if (!strcmp(incoming, "BITSHIFT"))   return(do_bitshift());
  if (!strcmp(incoming, "BITXOR"))    return(do_bitxor());
  if (!strcmp(incoming, "INT?"))      return(do_intq());
  if (!strcmp(incoming, "NOT"))       return(do_not());
  if (!strcmp(incoming, "NUMBER?"))   return(do_numberq());
  if (!strcmp(incoming, "OR"))        return(do_or());
  if (!strcmp(incoming, "RANDOM"))    return(do_random());
  
  /* I_MISC.C - Miscellaneous primitives       */
  if (!strcmp(incoming, "DATE"))      return(do_date());
  if (!strcmp(incoming, "NOTIFY"))    return(do_notify());
  if (!strcmp(incoming, "READ"))      return(do_read());
  if (!strcmp(incoming, "SLEEP"))     return(do_sleep());
  if (!strcmp(incoming, "TIME"))      return(do_time());
  if (!strcmp(incoming, "VERSION"))   return(do_version());
 
  /* If all else failed... */
  return(ERR_SYNTAX);
}

/*
 * trypush - Try to push incoming onto the stack, if 
 *           possible.
 */
int trypush(char *incoming)
{
  /* Attempt a string push */
  if (incoming[strlen(incoming) - 1] == 34) /* dquote */
  {
    incoming[strlen(incoming) - 1] = '\0';  /* remove it */
  
    error("FLL", do_push(incoming, STRING));
    
    incoming[strlen(incoming)] = 34;        /* put it back */
    
    return(1);
  }

  /* Attempt an integer push */
  if (is_integer(incoming))
  {
    error("FLL", do_push(incoming, NUM));
    return(1);
  }

  /* We couldn't do it */
  return(0);
}

/*
 * endofword - Returns the end-of-word pointer in a
 *             procedure list.
 */
struct inst *endofword(struct inst *prim)
{
  while (strcmp(prim->inst, ";")) prim = prim->next;
  return(prim);
}

/*
 * tryword - Check to see if incoming is a procedure we
 *           can run.
 */
int tryword(struct inst *prim)
{
  int nest_level = 0;
  int count = 0;

  for (count = lastword; count > 0; count--)
  {
    if (!strcmp(prim->inst, strtoupper(wordlist[count].inst)))
    {
      /* Syntax Check - Above it's own definition */
      if (prim->line < wordlist[count].line) return(ERR_SYNTAX);
    
      /* Recurse Check - In it's own definition */
      if (!strcmp(prim->inst, prim->top->inst)) return(ERR_SYNTAX);
     
      /* Nesting check */
      if (++nest_level >= MAXNEST) return(ERR_NESTING);
      
      /* Interpret the word */
      do_word(&wordlist[count]);

      /* Release this nesting level */
      nest_level--;
      
      return(count);
    }
  }

  /* It wasn't a word */
  return(0);
}

/*
 * do_word - Executes the procedure passed until the end
 *           is reached.  Wrapper for do_word_until to
 *           facilitate the EXIT primitive.
 */
int do_word(struct inst *word)
{
   word_depth++;
   if (word_depth > MAXNEST) error(word->inst, ERR_NESTING);
   
   if (!setjmp(jmpstack[word_depth]))     /* EXIT wasn't called. */
     do_word_until(word, endofword(word));
      
   word_depth--;
   return(0);
}

/*
 * do_word_until - Executes the procedure passed until endword 
 *                 is reached.
 */
int do_word_until(struct inst *word, struct inst *endword)
{
  struct inst *prim = word->next;
  char *incoming = NULL;
  int errn = 0;

  /* Execute the requested word */
  while (prim != endword)
  {
    /* Assign our variables */
    line = prim->line;
    incoming = prim->inst;

    /* 
     * Check one - data push.
     */
    if (trypush(incoming))
    {
      prim = prim->next;
      continue;
    }
        
    /* 
     * Check two - user-defined word, or nonforceable primitive.
     */
    if (incoming[0] != '\\')    /* Only if it's not a forced prim */
    {
      errn = tryword(prim);
      if (errn < 0) error(incoming, errn);
      else 
      {
        if (errn > 0)
        {
          prim = prim->next;
          continue;
        }
      }

    } else incoming++; /* Get rid of \ from forced primitive */
    
    /*
     * Check three - flow control primitives.
     */

    /* ABORT and EXIT technically never return. */
    if (!strcmp(incoming, "ABORT")) error(prim->inst, do_abort());

    /* EXIT is the only primitive in the interpreter that is * 
     * handled directly by these interpreter functions.      */
    if (!strcmp(incoming, "EXIT")) longjmp(jmpstack[word_depth], 1);

    if (!strcmp(incoming, "IF")) 
    {
      prim = do_if(prim);
      prim = prim->next;
      continue;
    }
    if (!strcmp(incoming, "BEGIN"))
    {
      error(prim->inst, do_begin(prim));
      prim = prim->next;
      continue;
    }
    if (!strcmp(incoming, "CONTINUE")) 
    {
      
      prim = do_continue(prim);
      prim = prim->next;
      continue;
    }
    if (!strcmp(incoming, "BREAK"))
    {
      
      prim = do_break(prim);
      prim = prim->next;
      continue;
    }
    if (!strcmp(incoming, "WHILE"))
    {
      
      prim = do_while(prim);
      prim = prim->next;
      continue;
    }
    if (!strcmp(incoming, "REPEAT"))
    {
      
      prim = do_repeat(prim);
      prim = prim->next;
      continue;
    }
    if (!strcmp(incoming, "UNTIL"))
    {
      
      prim = do_until(prim);
      prim = prim->next;
      continue;
    }
        
    /*
     * Check four - Data manipulation primitive.
     */
    errn = interp(incoming);
    if (errn)
    {
      error(prim->inst, errn);
    }

    /* Increment and go */
    prim = prim->next;
  }

  /* Done with the word */
  return(0);
}

