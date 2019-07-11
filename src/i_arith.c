/*
 * I_ARITH.C - Arithmetic, relational, integer manipulation, 
 *             and logic primitives.
 *
 * Functions:
 *     do_add do_sub do_mul do_div do_mod do_numberq do_intq 
 *     do_atoi
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "list.h"
#include "strutil.h"
#include "i_manip.h"

/*
 * do_atoi - Convert the top stack item into an integer.
 */
int do_atoi()
{

  /* Range/Type Check */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Validity Check */
  if (!is_integer(stack->data))
  {
    stack->data = strcpy(stack->data, "0");
    stack->type = NUM;
  }

  /* Do the conversion */
  stack->type = NUM;
  
  return(0);
}

/*
 * do_add - Add the top two integers on the stack together.
 */
int do_add()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);

  /* Retrieve our arguments */

  x = atoi(stack->prev->data) + atoi(stack->data);

  do_pop();
  do_pop();

  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_sub - Subtract the top two integers on the stack.
 */
int do_sub()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);

  /* Retrieve our arguments */
  x = atoi(stack->prev->data) - atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */

  return(do_push(intostr(x), NUM));
}

/*
 * do_mul - Multiply the top two integers on the stack together.
 */
int do_mul()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);

  /* Retrieve our arguments */
  x = atoi(stack->prev->data) * atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */

  return(do_push(intostr(x), NUM));

}

/*
 * do_div - divide the top two integers on the stack.
 */
int do_div()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  if (atoi(stack->data) == 0) return(ERR_DBZ);

  /* Retrieve our arguments */
  x = atoi(stack->prev->data) / atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */

  return(do_push(intostr(x), NUM));
}

/*
 * do_mod - Compute the modulus (remainder) of two numbers.
 */
int do_mod()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  if (atoi(stack->data) == 0) return(ERR_DBZ);

  /* Retrieve our arguments */
  x = atoi(stack->prev->data) % atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */

  return(do_push(intostr(x), NUM));
}

/*
 * do_numberq - Push 1 onto the stack if its a number string 
 *              on top.
 */
int do_numberq()
{
  int result = 0;  /* What we return */

  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != STRING) return(ERR_ARGT);

  /* Is it a number? */
  result = is_integer(stack->data);
  if (result < 0) result = -(result);  /* Invert */

  /* Clean up and push the result */
  do_pop();

  return(do_push(intostr(result), NUM));
}

/*
 * do_intq - If it's an integer on top, return 1;
 */
int do_intq()
{
  int result = 0;  /* What we return */

  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);

  result = stack->type;
  
  /* Clean up and push the result */
  do_pop();

  if (result == NUM)
    result = 1;
  else
    result = 0;

  return(do_push(intostr(result), NUM));
}

/*
 * do_random - Push a random number onto the stack.
 */
 int do_random()
 {
   return(do_push(intostr(rand() % 32767), NUM));
 }

/*
 * do_gthan() - Is i1 greater than i2?
 */
int do_gthan()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) > atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_gteq() - Is i1 greater than or equal to i2?
 */
int do_gteq()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) >= atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_lthan() - Is i1 less than i2?
 */
int do_lthan()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) < atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_lteq() - Is i1 less than or equal to i2?
 */
int do_lteq()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) <= atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_iseq() - Is i1 equal to i2?
 */
int do_iseq()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) == atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_and() - Logical AND operation.
 */
int do_and()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) && atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_or() - Logical OR operation.
 */
int do_or()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) || atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_not() - Logical NOT operation.
 */
int do_not()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 1) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = !atoi(stack->data);
  do_pop();
    
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_bitand() - Bitwise AND operation.
 */
int do_bitand()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) & atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_bitor() - Bitwise OR operation.
 */
int do_bitor()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) | atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_bitxor() - Bitwise OR operation.
 */
int do_bitxor()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the compare */
  x = atoi(stack->prev->data) ^ atoi(stack->data);
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}

/*
 * do_bitshift() - Bitwise OR operation.
 */
int do_bitshift()
{
  int x;      /* Our result integer */

  /* Range/Type Checks */
  if (depth < 2) return(ERR_SUNDERF);
  if (stack->type != NUM) return(ERR_ARGT);
  if (stack->prev->type != NUM) return(ERR_ARGT);
  
  /* Do the shift */
  if (atoi(stack->data) > 0)
  {
    x = atoi(stack->prev->data) << atoi(stack->data);
  } else 
  {
    x = atoi(stack->prev->data) >> -(atoi(stack->data));
  }
  do_pop();
  do_pop();
  
  /* Push the result */
  return(do_push(intostr(x), NUM));
}
