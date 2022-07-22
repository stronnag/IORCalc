#include <stdint.h>
#include <stdbool.h>
#include "ior.h"

typedef enum  {
     EDIT_TYPE_ED_C = 0,
     EDIT_TYPE_ED_F = 1,
     EDIT_TYPE_ED_I = 2,
     EDIT_TYPE_ED_T = 3,
} EditType_e;

typedef struct edit_field {
     char *prompt;
     char *ename;
     size_t offset;
     uint8_t len;
     uint8_t col;
     uint8_t row;
     uint8_t flag;
} edit_field_t;

extern int  get_field(int j, edit_field_t *ef);
extern void update_rec(char *dt, void *u, int count);
extern char * get_as_string(void *u, int count);
extern void *read_file(char * f);
extern int get_int(void *u, int count);
extern double get_double(void *u, int count);
extern char *get_string (void *u, int count);

extern void set_int(int v, void *u, int count);
extern void set_double(double d, void *u, int count);
extern void set_string (char *s, void *u, int count);

extern ior_rec_t* allocate_ior_rec(void);
extern calc_rec_t* allocate_calc_rec(void);

extern bool is_data_valid(void *u);
extern edit_field_t *find_field_by_name(char *name, void *u);
extern char * calc_ior(ior_rec_t *u, calc_rec_t *c);

/* Legacy C functions */
extern void calc_hull(ior_rec_t *u, calc_rec_t *c);
extern double trunc_to(double value, int decp);
extern void calc_rmc(ior_rec_t *u, calc_rec_t *c) ;
extern void calc_prop(ior_rec_t *u, calc_rec_t *c);
extern void calc_prop(ior_rec_t *u, calc_rec_t *c);
extern void calc_rig(ior_rec_t *u, calc_rec_t *c);
extern void calc_rtg(ior_rec_t *u, calc_rec_t *c);
extern char *show_rtg(ior_rec_t *u, calc_rec_t *c);
extern void ior_cert(char *fp, ior_rec_t*, calc_rec_t*,  char *[][2], int *);
extern void pcert(ior_rec_t *u, calc_rec_t *c, char *, int);
extern char* report(ior_rec_t *u, calc_rec_t *c);
