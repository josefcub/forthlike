/* Available structures and variables */

/* Available Functions */
extern int interp(char *incoming);
extern int trypush(char *incoming);
extern struct inst *endofword(struct inst *prim);
extern int tryword(struct inst *prim);
extern int do_word(struct inst *word);
extern int do_word_until(struct inst *word, struct inst *endword);
