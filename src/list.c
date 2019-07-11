/*
 * LIST.C - Linked list functions.
 *
 * Functions:
 *    itemalloc itemfree itemcpy listgraft listprune
 *    listseek listhead listtail listdepth
 */

/* Necessary includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "list.h"

struct item *stack = NULL;  /* For everything from interp.c down */

/*
 * itemalloc - Allocates the memory for a stack item.
 */
struct item *itemalloc()
{
  struct item *temp;   

  temp = (struct item *)malloc(sizeof(struct item));
 
  /* Memory Check */
  if (temp == NULL) return(NULL);

  /* Initialize our elements */   
  temp->type = NOTHING;
  temp->prev = NULL;
  temp->next = NULL;
  return(temp);        
}

/*
 * itemfree - Frees the memory taken up by a stack item.
 */
int itemfree(struct item *src)
{
  if (src != NULL)
  {
    if (src->data != NULL) free(src->data);
    free(src);                
  }
  return(0);
}

/*
 * itemcpy - Copies an item.  Returns the copy of the first item.
 */
struct item *itemcpy(struct item *src)
{
  struct item *temp = itemalloc();
  if (temp == NULL) return(NULL);
  
  temp->data = (char *)malloc(strlen(src->data) + 1);
  if (temp->data == NULL) return(NULL);
  
  strcpy(temp->data, src->data);
  temp->type = src->type;
  temp->prev = src->prev;
  temp->next = src->next;

  return(temp);
}

/*
 * listgraft - Graft an item onto the list.  This grafts it immediately
 *             BEFORE the item pointed to by *list.  Returns pointer to
 *             new list item.
 */
struct item *listgraft(struct item *list, struct item *item)
{
  /* Does it indeed exist? */
  if (item == NULL) return(NULL);

  /* Initial assignment */
  item->prev = list;
  item->next = NULL;

  /* Juggle Pointers */
  if (list != NULL)
  {
    item->next = list->next;
    list->next = item;
    if (item->next != NULL) item->next->prev = item;
  }

  return(item);
}

/*
 * listprune - Prunes the passed item out its list.  
 * Returns the item if successful.
 *
 */
struct item *listprune(struct item *item)
{
  /* Does it indeed exist? */
  if (item == NULL) return(NULL);

  /* Juggle pointers */
  if (item->prev != NULL) item->prev->next = item->next;
  if (item->next != NULL) item->next->prev = item->prev;

  return(item);
}

/*
 * listhead - Returns a pointer to the top of the list.
 */
struct item *listhead(struct item *list)
{
  /* Does it even exist? */
  if (list == NULL) return(NULL);
  
  while (list->next != NULL)
  {
    if (list->next == NULL) return(list);
    list = list->next;
  }
  return(list);
}

/*
 * listtail - Returns a pointer to the top of the list.
 */
struct item *listtail(struct item *list)
{
  /* Does it even exist? */
  if (list == NULL) return(NULL);

  while (list->prev != NULL)
  {
    if (list->prev == NULL) return(list);
    list = list->prev;
  }
  return(list);
}

/*
 * listseek - Seek backwards or forwards in a list.
 *            length is the amount to move.  Positive
 *            moves forward, negative moves back.
 *            Returns NULL if the end or the beginning
 *            of the list is reached before length
 *            is.
 */
struct item *listseek(struct item *list, int length)
{
  int counter = 0;

  if (list == NULL) return(NULL);
  if (length > 0)
    /* Positive winds the list forward */
    for (; counter < length; counter++ )
    {
      if (list == NULL) return(NULL);
      list = list->next;
    }
  else
    /* Negative winds the list backward */
    for (; counter < abs(length); counter++ )
    {
      if (list == NULL) return(NULL);
      list = list->prev;
    }

  return(list);
}

/*
 * listdepth - Returns the depth of the list.  
 */
int listdepth(struct item *list)
{
  int count = 0;                

  list = listhead(list);
  while (list != NULL)
  {
    count++;
    list = list->prev;
  }

  return(count);
}
