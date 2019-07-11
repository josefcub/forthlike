/* Available structures and variables */
struct inst
{
  char *inst;            /* The text instruction  */
  int line;              /* The line number of it */
  struct inst *next;     /* Next instruction      */
  struct inst *top;      /* The beginning of the list */
};

extern struct inst wordlist[MAXSTACK];
extern int lastword;
extern int line;

/* Available functions */
extern int preload(FILE *filename);
