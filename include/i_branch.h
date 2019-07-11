/* Available structures and variables */

/* Available functions */
extern struct inst *do_if(struct inst *prim);
extern int do_begin(struct inst *prim);
extern struct inst *do_break(struct inst *prim);
extern struct inst *do_continue(struct inst *prim);
extern struct inst *do_while(struct inst *prim);
extern struct inst *do_repeat(struct inst *prim);
extern struct inst *do_until(struct inst *prim);
extern int do_abort();

