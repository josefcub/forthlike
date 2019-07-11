/*
 * I_STRING.C - String manipulation primitives.
 *
 * Functions:
 *    do_intostr do_strcat do_strlen do_toupper do_tolower do_strcmp 
 *    do_stringcmp do_strncmp do_strcut do_stringpfx
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "list.h"
#include "debug.h"
#include "strutil.h"
#include "i_manip.h"

/*
 * do_intostr - Converts an integer into a string.
 */
int do_intostr()
{
  /* Range/Type Check */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);

  /* Relying on support coding to do the
     validity check on the supposed integer */

  /* Do the conversion */
  stack->type = STRING;

  return(0);
}

/*
 * do_strcat - Concatenate the top two stack items.
 */
int do_strcat()
{
  char *temp;

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if ((stack->type != STRING) || (stack->prev->type != STRING))
    return(ERR_ARGT);
  
  /* Do the strcat */
  temp = (char *)malloc(strlen(stack->data) + strlen(stack->prev->data) + 1);
  if (temp == NULL) return(ERR_MEM);
  temp = strcpy(temp, stack->prev->data);
  temp = strcat(temp, stack->data);

  /* Clean up and push the result */
  do_pop();
  do_pop();

  if (do_push(temp, STRING) == ERR_MEM) return(ERR_MEM);
  free(temp);

  return(0);
}

/*
 * do_strlen - Push the length of the topmost string item onto the stack.
 */
int do_strlen()
{
  int len = strlen(stack->data);

  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Clean up and push the result */
  do_pop();
  
  return(do_push(intostr(len), NUM));
}

/*
 * do_toupper - Change the topmost string item to all uppercase.
 */
int do_toupper()
{
  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Do the conversion */
  stack->data = strtoupper(stack->data);
  
  return(0);
}

/*
 * do_tolower - Change the topmost string item to all uppercase.
 */
int do_tolower()
{
  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Do the conversion */
  stack->data = strtolower(stack->data);
  
  return(0);
}

/*
 * do_stringq - Is the topmost item a string?
 */
int do_stringq()
{
  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING)
  {
    do_pop();
    return(do_push("0", NUM));
  }
  else
  {
    do_pop();
    return(do_push("1", NUM));
  }
  
}

/*
 * do_strcmp - Are the strings identical?
 * do_stringcmp - tolower swap tolower swap strcmp
 * do_strncmp - Compare i characters of s1 and s2.
 */
 int do_strcmp()
 {
   /* Easy comparison pointers */
   char *s1 = NULL;
   char *s2 = NULL;
   int diff = 0;       
   
   /* Range/Type Check */
   if (depth < 2) return(ERR_SUNDERF);
   if ((stack->type != STRING) || (stack->prev->type != STRING)) 
     return(ERR_ARGT);

   s1 = stack->prev->data;
   s2 = stack->data;

   while ((*s1 != '\0') || (*s2 != '\0'))
   {

     /* Compare strings */
     if (*s1 != *s2)
     {
       /* Compute the difference */
       diff = *s1 - *s2;
       
       /* Clean up and return the difference */
       do_pop();
       do_pop();
       
       return(do_push(intostr(diff), NUM));
     }
     /* Next character */
     s1++;
     s2++;
   }
   
   /* Clean up and return */
   do_pop();
   do_pop();      
   
   return(do_push("0", NUM));
 }
 
 int do_stringcmp()
 {
   /* Range/Type Check */
   if (depth < 2) return(ERR_SUNDERF);
   if ((stack->type != STRING) || (stack->prev->type != STRING)) 
     return(ERR_ARGT);

   /* Do exactly what it says */
   do_tolower();
   do_swap();
   do_tolower();
   do_swap();
   return(do_strcmp());
 }
 
 /*
  * do_strncmp - As seem above, the first strcmp was done the
  *              hard way: manually.  This time, fuck it.
  *              Why do I leave the hand-made strcmp in place?
  *              Because it proves I could do it.
  */
 int do_strncmp()
 {
   int limit = 0;
   int diff = 0;
   
   /* Range/Type Check */   
   if (depth < 3) return(ERR_SUNDERF);
   if (stack->type != NUM) return(ERR_ARGT);
   if (stack->prev->type != STRING) return(ERR_ARGT);
   if (stack->prev->prev->type != STRING) return(ERR_ARGT);

   /* Retrieve our strncmp argument */
   limit = atoi(stack->data);
   do_pop();
   
   /* Get the difference */
   diff = strncmp(stack->prev->data, stack->data, limit);
   
   /* Clean up and return the result */
   do_pop();
   do_pop();
      
   return(do_push(intostr(diff), NUM));
 }
 
/*
 * do_striplead - Strip leading spaces from top stack item.
 */
int do_striplead()
{
  char *ptr = NULL;
  char *result = NULL;

  /* Range/type check */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Prepare to copy */
  result = (char *)malloc(strlen(stack->data) + 1);
  if (result == NULL) return(ERR_MEM);
  ptr = stack->data;

  /* Copy what isn't leading whitespace */
  while (*ptr == 32) ptr++;
  strcpy(result, ptr);    

  /* Clean up and push the result */
  do_pop();
  return(do_push(result, STRING));
}

/*
 * do_striptail - Strip leading spaces from top stack item.
 */
int do_striptail()
{
  char *ptr = NULL;
  char *result = NULL;

  /* Range/type check */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Prepare to copy */
  result = (char *)malloc(strlen(stack->data) + 1);
  if (result == NULL) return(ERR_MEM);

  ptr = stack->data; 

  /* Seek to the end of the string 
     and skip ending whitespace    */
  while (*ptr != '\0') ptr++;
  while (*(--ptr) == 32) *ptr = '\0';
  
  /* Copy the result */
  strcpy(result, stack->data);    

  /* Clean up and push the result */
  do_pop();
  return(do_push(result, STRING));
}

/*
 * do_strip - striplead striptail
 */
int do_strip()
{
  /* Range/type check */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  do_striplead();
  do_striptail();
 
  return(0);
}

/* 
 * do_strcut - ( s i -- s s ) Cuts a string at position i.
 */
int do_strcut()
{
  int x = 0;                  /* X is the amount to cut */
  int count = 0;              /* Generic counter */
  char *src = NULL;           /* Original String */
  char *dest = NULL;          /* Result String */

  /* Range/type checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != STRING) return(ERR_ARGT);

  /* Get our argument and remove it */
  x = atoi(stack->data);
  if (x < 0) return(ERR_POSINT);
  do_pop();
  
  /* Assign and allocate our strings */
  src = stack->data;
  dest = (char *)malloc(strlen(stack->data) + 1);
  if (dest == NULL) return(ERR_MEM);
  
  /* Get the second string */
  while ((*src != '\0') && (++count <= x)) src++;
  dest = strcpy(dest, src);

  /* Trim the first string */
  *src = '\0';

  return(do_push(dest, STRING));
}

/*
 * do_stringpfx - ( s1 s2 -- i ) Is s2 a prefix of s1?
 */
int do_stringpfx()
{
  int result = 0;
  char *s1 = NULL; /* For the compare */
  char *s2 = NULL;

  /* Range/Type Check */
  if (depth < 2) return(ERR_SUNDERF);
  if ((stack->type != STRING) || (stack->prev->type != STRING)) 
    return(ERR_ARGT);

  /* Assign our pointers */
  s1 = stack->prev->data;
  s2 = stack->data;

  /* Do the compare */
  result = 1;               /* Start out true */
  while (*s2 != '\0')
  {
    if (*s2 != *s1) result = 0;  /* False */
    if (*s1 == '\0') break;    
    s1++;
    s2++;
  }

  /* Clean up and return */
  do_pop();
  do_pop();

  return(do_push(intostr(result), NUM));
}

/*
 * do_explode - Take a string, and cut it up by the delimiter, 
 *              pushing each segment onto the stack and throw
 *              the count on the end.
 */
int do_explode()
{

  int count = 0;
  int total = 1;
  
  char *delimiter = NULL;
  char *source = NULL;
  char *segment = NULL;
  
  /* Range/Type/Size Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if ((stack->type != STRING) || (stack->prev->type != STRING)) 
    return(ERR_ARGT);
  if (strlen(stack->data) == 0) return(ERR_ESA);
  
  /* If the source is a null string, return '1' and */
  /* the empty string.  This is what MUF does.      */
  if (strlen(stack->prev->data) == 0)
  {
    do_pop();
    return(do_push(intostr(total), NUM));
  }
  
  /* Copy the stack items */  
  source = (char *)malloc(strlen(stack->prev->data) + 1);
  if (source == NULL) return(ERR_MEM);
  delimiter = (char *)malloc(strlen(stack->data) + 1);
  if (delimiter == NULL) return(ERR_MEM);
  
  source = strcpy(source, stack->prev->data);
  delimiter = strcpy(delimiter, stack->data);
  
  /* Clean up the stack for the mess yet to come. */
  do_pop();
  do_pop();
  
  /* Compare and push from source */  
  
  for (count = strlen(source) ; count >= 0 ; count--)
  {
    if (!strncmp(source + count, delimiter, strlen(delimiter)))
    {
      source[count] = '\0';
      
      /* Allocate and copy string for stack item */
      segment = (char *)malloc(strlen(source + count + strlen(delimiter)) + 1);
      if (segment == NULL) return(ERR_MEM);
      segment = strcpy(segment, source + count + strlen(delimiter));
                    
      /* Push the segment onto the stack. */
      error("EXPLODE", do_push(segment, STRING));
      total++;

    }
  }  

  /* Push the first segment */
  segment = (char *)malloc(strlen(source) + 1);
  if (segment == NULL) return(ERR_MEM);
  segment = strcpy(segment, source);

  error("EXPLODE", do_push(segment, STRING));
  
  /* Push the total last of all */
  error("EXPLODE", do_push(intostr(total), NUM));
  
  /* Clean up after ourselves */
  free(source);
  free(delimiter);
  
  return(0);
} 

/*
 * do_instr - Return the first occurrence of the search 
 *            string in the target.  ( s s1 -- i )
 */
int do_instr()
{
  int count = 0;
  char *source = stack->prev->data;
  char *search = stack->data;
  
  /* Range/Type/Size Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if ((stack->type != STRING) || (stack->prev->type != STRING)) 
    return(ERR_ARGT);
  if (strlen(stack->data) == 0) return(ERR_ESA);

  /* If the source is a null string, return '0'. */
  if (strlen(stack->prev->data) == 0)
  {
    do_pop();
    do_pop();
    return(do_push(intostr(count), NUM));
  }
  
  /* Now perform the search. */
  for (count = 0 ; count <= strlen(source) ; count++)
  {
    if (!strncmp(source + count, search, strlen(search)))
    {
      do_pop();
      do_pop();
      return(do_push(intostr(count + 1), NUM));
    }
  }
  
  /* If we've made it this far, there were no matches. */
  count = 0;
  do_pop();
  do_pop();
  return(do_push(intostr(count), NUM));
}

/*
 * do_rinstr - Return the last occurrence of the search 
 *             string in the target.  ( s s1 -- i )
 */
int do_rinstr()
{
  int count = 0;
  char *source = stack->prev->data;
  char *search = stack->data;
  
  /* Range/Type/Size Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if ((stack->type != STRING) || (stack->prev->type != STRING)) 
    return(ERR_ARGT);
  if (strlen(stack->data) == 0) return(ERR_ESA);

  /* If the source is a null string, return '0'. */
  if (strlen(stack->prev->data) == 0)
  {
    do_pop();
    do_pop();
    return(do_push(intostr(count), NUM));
  }
  
  /* Now perform the search. */
  for (count = strlen(source) ; count >= 0 ; count--)
  {
    if (!strncmp(source + count, search, strlen(search)))
    {
      do_pop();
      do_pop();
      return(do_push(intostr(count + 1), NUM));
    }
  }
  
  /* If we've made it this far, there were no matches. */
  count = 0;
  do_pop();
  do_pop();
  return(do_push(intostr(count), NUM));
}

/*
 * do_instring  - tolower swap tolower swap instr
 * do_rinstring - tolower swap tolower swap rinstr
 */
int do_instring()
{
  /* Range/Type Check */
  if (depth < 2) return(ERR_SUNDERF);
  if ((stack->type != STRING) || (stack->prev->type != STRING)) 
      return(ERR_ARGT);

  /* Do exactly what it says */
  do_tolower();
  do_swap();
  do_tolower();
  do_swap();
  return(do_instr());
}

int do_rinstring()
{
  /* Range/Type Check */
  if (depth < 2) return(ERR_SUNDERF);
  if ((stack->type != STRING) || (stack->prev->type != STRING)) 
      return(ERR_ARGT);

  /* Do exactly what it says */
  do_tolower();
  do_swap();
  do_tolower();
  do_swap();
  return(do_rinstr());
}
