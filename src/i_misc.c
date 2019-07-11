/*
 * I_MISC.C - Primitives that don't fit in elsewhere.
 *
 * Functions:
 *    do_sleep do_version do_date do_time do_notify do_read
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "globals.h"
#include "list.h"
#include "i_manip.h"
#include "strutil.h"

/*
 * do_notify - Throw the topmost string item onto the screen.
 */
int do_notify()
{
  /* Validity Check */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Print the message */
  fprintf(stdout, "%s\n", stack->data);
  do_pop();

  return(0);
}


/*
 * do_read - Read a STRING from stdin.
 */
int do_read()
{
  char buf[MAXBUF];
  char inp = 0;
  int count = 0;

  /* Read the string */
  while ((inp != 10) || (count == 0))
  {
    inp = fgetc(stdin);
    if (count == (MAXBUF - 1)) break;
    if (inp == EOF) return(ERR_SYNTAX);
    if (inp != 10) buf[count++] = inp;
  }
  
  buf[count] = '\0';

  return(do_push(buf, STRING));
}

/*
 * do_sleep - Sleep for n seconds.  Due to vagarities in OSes,
 *            processors, and execution time, this is NOT an exact
 *            function.
 */
int do_sleep()
{
  time_t count;    /* For difftime */
  double interval; /* User's desired sleep */
  char *endptr;    /* For strtod */ 

  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  
  /* Get our data */
  interval = strtod(stack->data, &endptr);
  do_pop();
  count = time(NULL);
  
  while (difftime(time(NULL), count) < interval);
    
  return(0);
}

/*
 * do_date - Returns the month, day, and year on the stack.
 */
int do_date()
{
  time_t moment;
  struct tm *date;
  
  /* Retrieve the current date */
  moment = time(NULL);
  date = localtime(&moment);
    
  do_push(intostr(date->tm_mday), NUM);
  do_push(intostr(date->tm_mon + 1), NUM);
  return((do_push(intostr(date->tm_year + 1900), NUM)));
  
}

/*
 * do_time - Returns the hours, minutes, and seconds on the stack.
 */
int do_time()
{
  time_t moment;
  struct tm *date;
  
  /* Retrieve the current date */
  moment = time(NULL);
  date = localtime(&moment);
    
  do_push(intostr(date->tm_sec), NUM);
  do_push(intostr(date->tm_min), NUM);
  return((do_push(intostr(date->tm_hour), NUM)));
  
}

/*
 * do_version - Push the version string onto the stack.
 */
int do_version()
{
  return(do_push(VERSION, STRING));
}
