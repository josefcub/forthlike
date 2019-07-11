/* Available structures and variables */
struct item
{
  char *data;
  int type;  
  struct item *prev, *next;
};

extern struct item *stack;

/* Available functions */
extern struct item *itemalloc();
extern int itemfree(struct item *src);
extern struct item *itemcpu(struct item *src);
extern struct item *listgraft(struct item *list, struct item *item);
extern struct item *listprune(struct item *item);
extern struct item *listhead(struct item *list);
extern struct item *listtail(struct item *list);
extern struct item *listseek(struct item *list, int length);
extern int listdepth(struct item *list);

/* This is easier than a systemwide global */
#define depth listdepth(stack)
