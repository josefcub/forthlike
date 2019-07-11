/*
 * STRUTIL.C - String manipulation functions.
 *
 * Functions:
 *    intostr strtoupper strtolower is_integer
 *
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * char *intostr() - Changes an integer into a string.
 *        
 */
char *intostr(int i)
{
  static char num[16];
  int j, k;
  char *ptr2;

  k = i;
  ptr2 = num+14;
  num[15] = '\0';
  if (i < 0) i = -i;
  while (i)
  {
    j = i % 10;
    *ptr2-- = '0' + j;
    i /= 10;
  }

  if (!k) *ptr2-- = '0';
  if (k < 0) *ptr2-- = '-';
  return (++ptr2);
}

/*
 * char strtoupper - Change string to all uppercase.
 */
char *strtoupper(char *incoming)
{
  char *ptr = incoming;

  while (*ptr != '\0')
  {
    *ptr = toupper(*ptr);
    ptr++;
  }

  return(incoming);
}

/*
 * char strtolower - Change string to all lowercase.
 */
char *strtolower(char *incoming)
{
  char *ptr = incoming;

  while (*ptr != '\0')
  {
    *ptr = tolower(*ptr);
    ptr++;
  }

  return(incoming);
}

/* 
 * is_integer - Tests the string to see if it contains
 *              an integer or not.  Returns zero if it
 *              is NOT a string.
 */
int is_integer(char *src)
{
  char *ptr = src;

  /* Easy checks */
  if ((*ptr != 45) && ((*ptr < 48) || (*ptr > 57))) return(0);
  if ((*ptr == 45) && strlen(src) == 1) return(0);

  /* A time-consuming check */
  
  while (*++ptr != '\0')
  {
    if ((*ptr < 48) || (*ptr > 57)) return(0);
  }

  /* Must be good */
  return(-1);
}
