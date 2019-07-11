/*
 * PARSER.C - Contains the parsing function.
 * 
 * Functions:
 *    parse
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "list.h"
#include "debug.h"
#include "strutil.h"
#include "preload.h"

/*
 * parse() - Parses the input file and returns an
 *           item, or NULL for EOF.
 */
char *parse(FILE *filename)
{
  char buf[MAXBUF];
  char *retval;
  int count  = 0;
  char input = 0;

 /* Start a loop to get a word or phrase */
  while (input != EOF)
  {
     input = fgetc(filename);

    /* Decide what to do based on the character. */
     switch (input)
     {

      /*
       * LF - Send the result in.
       */
       case 10:
       {
         line++;
      	 if (count > 0)
	       {
           buf[count] = '\0';

           retval = (char *)malloc(strlen(buf) + 1);
           if (retval == NULL) error("PARSE", ERR_MEM);
           strcpy(retval, buf);

           /* Not-strings in uppercase */
           retval = strtoupper(retval);
           
           return(retval);
         } else break;
       }
      
      /*
       * CR - Ignored for DOS/nix compatibility.
       */
       case 13:
       {
         break;
       }

      /*
       * SPACE - Send the result in.
       */
       case 32:
       {
         if (count > 0)
         {
           buf[count] = '\0';

           retval = (char *)malloc(strlen(buf) + 1);
           if (retval == NULL) error("PARSE", ERR_MEM);
           
           strcpy(retval, buf);
           
           /* Not-strings in uppercase */
           retval = strtoupper(retval);

           return(retval);
         } else break;
       }

      /*
       * DQUOTE - Parse out a string.
       */
       case 34:
       {
         /* Clear input */
         input = 0;

         /* Syntax check */
         if (count > 0) error("PARSE", ERR_SYNTAX);
         
         /* Get the rest of the string, until we hit the other 34 */
         while (input != 34)
         {
           input = fgetc(filename);
           
           /* There's enough syntax checks to justify a switch */
           switch (input)
           {
             case 10: error("PARSE", ERR_SYNTAX);
             case 13: error("PARSE", ERR_SYNTAX);
             case EOF: error("PARSE", ERR_PEOF);
             default: buf[count++] = input;
           }
         }

         /* Return the resulting string */
         buf[count] = '\0';

         retval = (char *)malloc(strlen(buf) + 1);
         if (retval == NULL) error("PARSE", ERR_MEM);
         strcpy(retval, buf);
         return(retval);
       }

      /*
       * LPARENTH - Ignore everything in parentheses.
       */
       case 40:
       {
         while (input != 41)
         {
	         input = fgetc(filename);
           if (input == 40) error("PARSE", ERR_SYNTAX);
           if (input == EOF) error("PARSE", ERR_PEOF);
	         if (input == 10)  line++;
	       }
	       break;
       }

      /*
       * RPARENTH - This is an error if we get it.
       */
       case 41:
       {
         error("PARSE", ERR_SYNTAX);
       }

      /*
       * DEFAULT - add it to the buffer.
       */
       default:
       {
         if (input != EOF)
	       {
           if (count < (MAXBUF - 1))
	           buf[count++] = input;
	         else
	           error("PARSE", ERR_BUFOVER);
	        break;
	       }
       }
     }
   }

  /* If we fall out due to EOF, make sure the
     buffer's empty before we send the NULL.  */
  if (count > 0)
  {
    buf[count] = '\0';
    
    retval = (char *)malloc(strlen(buf) + 1);
    if (retval == NULL) error("PARSE", ERR_MEM);
    strcpy(retval, buf);
    
    /* Not-strings in uppercase */
    retval = strtoupper(retval);
    
    return(retval);
  } else return(NULL);
}
