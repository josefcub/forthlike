/*
 * I_MANIP.C - Interpreter routines for stack manipulation primitives.
 *
 * Functions:
 *    do_push do_pop do_put do_pick do_dup do_over do_rotate 
 *    do_rot do_swap do_depth
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "strutil.h"
#include "list.h"

/*
 * do_push() - Push an item onto the stack.
 */
int do_push(char *data, int type)
{
  struct item *temp = itemalloc();

  /* Obvious Checks */
  if (temp == NULL) return(ERR_MEM);
  if (depth == MAXSTACK) return(ERR_SOVERF);

  /* Assemble the stack item */
  temp->data = (char *)malloc(strlen(data) + 1);
  if (temp->data == NULL) return(ERR_MEM);
  strcpy(temp->data, data);
  temp->type = type;
  
  /* Add the item to the stack */
  temp = listgraft(stack, temp);

  stack = listhead(temp);

  return(0);
}

/*
 * int do_pop() - Pop top item off of the stack.  Item is lost.
 */
int do_pop()
{
  struct item *temp = stack;

  /* Range check is essential */
  if (depth > 1) 
  {
    stack = stack->prev;
    temp = listprune(temp);
    itemfree(temp);
  }
  else
  {
    if (depth == 0) return(ERR_SUNDERF);
    itemfree(stack);
    stack = NULL;
  }

  return(0);
}

/*
 * int do_put() - put item n steps into the stack.
 */
int do_put()
{
  struct item *temp; /* The item to move */
  int location;

  /* Argument checking and retrieval */
  if (depth < 2) return(ERR_SUNDERF);

  if (stack->type != NUM) return(ERR_ARGT);
  location = atoi(stack->data); 
  do_pop();

  /* Operand range checking */
  if (location < 1) return(ERR_POSINT);
  if (location > (depth - 1)) return(ERR_SUNDERF);
    
  /* Invert the operand for listseek */
  location = -(location);

  /* Ugly, brute-force method that works */
  temp = listseek(stack, location);
  
  free(temp->data);
  
  temp->data = (char *)malloc(strlen(stack->data) + 1);
  if (temp->data == NULL) return(ERR_MEM);
  strcpy(temp->data, stack->data);
  temp->type = stack->type;

  /* All done, don't need this anymore */
  do_pop();
  
  return(0);
}

/*
 * int do_pick() - copy n'th item inside the stack and place it up top.
 *     do_dup() - 1 pick
 *     do_over() - 2 pick
 *
 */
int do_pick()
{
  struct item *temp;
  int location;

  /* Argument checking and retrieval */
  if (depth < 2) return(ERR_SUNDERF);

  if (stack->type != NUM) return(ERR_ARGT);
  location = atoi(stack->data);
  do_pop();

  /* Operand range checking */
  if (location < 1) return(ERR_POSINT);
  if (location > depth) return(ERR_SUNDERF);

  /* Juggle the operand for listseek */
  location = -(location);
  location++;  

  /* Push the result */
  temp = listseek(stack, location);
  return(do_push(temp->data, temp->type));
}

int do_dup()
{
  if (do_push("1", NUM) == ERR_MEM) return(ERR_MEM);
  return(do_pick());
}

int do_over()
{
  if (do_push("2", NUM) == ERR_MEM) return(ERR_MEM);
  return(do_pick());
}

/*
 * int do_rotate() - rotates the nth item to the top of the stack.
 * int do_rot()    - 3 rotate
 * int do_swap()   - 2 rotate
 */
int do_rotate()
{
  struct item *temp;
  int location;
  
  /* Argument checking and retrieval */
  if (depth < 2) return(ERR_SUNDERF);

  if (stack->type != NUM) return(ERR_ARGT);
  location = atoi(stack->data); 
  do_pop();

  /* Operand range checking */
  if (location < 1) return(ERR_POSINT);
  if (location > (depth)) return(ERR_SUNDERF);
    
  /* Invert the operand for listseek */
  location = -(location);
  location++;

  
  /* Prune and graft our rotation */
  temp = listprune(listseek(stack, location));
  temp = listgraft(stack, temp);

  /* Make stack point in the right direction */
  stack = listhead(stack);

  return(0);
}

int do_rot()
{
  if (do_push("3", NUM) == ERR_MEM) return(ERR_MEM);
  return(do_rotate());
}

int do_swap()
{
  if (do_push("2", NUM) == ERR_MEM) return(ERR_MEM);
  return(do_rotate());
}

/*
 * do_depth() - Pushes the depth of the stack onto the stack.
 */
int do_depth()
{
  return(do_push(intostr(depth), NUM));
}
