#pragma once

#include "ior.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#define CERT_LINES 66

typedef enum {
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

typedef struct cert_pages {
  char **page1;
  char **page2;
} cert_pages_t;

extern int get_field(int j, edit_field_t *ef);
extern void update_rec(const char *dt, ior_rec_t *u, int count);
extern char *get_as_string(ior_rec_t *u, int count);
extern ior_rec_t* read_file(char *f);
extern int get_int(ior_rec_t *u, int count);
extern double get_double(ior_rec_t *u, int count);
extern char *get_string(ior_rec_t *u, int count);

extern void set_int(int v, ior_rec_t *u, int count);
extern void set_double(double d, ior_rec_t *u, int count);
extern void set_string(const char *s, ior_rec_t *u, int count);

extern ior_rec_t *allocate_ior_rec(void);
extern calc_rec_t *allocate_calc_rec(void);

extern bool is_data_valid(ior_rec_t *u);
extern edit_field_t *find_field_by_name(char *name, ior_rec_t *u);
extern char *calc_ior(ior_rec_t *u, calc_rec_t *c);

/* Legacy C functions */
extern void calc_hull(ior_rec_t *u, calc_rec_t *c);
extern double trunc_to(double value, int decp);
extern void calc_rmc(ior_rec_t *u, calc_rec_t *c);
extern void calc_prop(ior_rec_t *u, calc_rec_t *c);
extern void calc_prop(ior_rec_t *u, calc_rec_t *c);
extern void calc_rig(ior_rec_t *u, calc_rec_t *c);
extern void calc_rtg(ior_rec_t *u, calc_rec_t *c);
extern char *show_rtg(ior_rec_t *u, calc_rec_t *c);
extern void ior_cert(ior_rec_t *, calc_rec_t *, cert_pages_t *);
extern void pcert(ior_rec_t *u, calc_rec_t *c, const char *, int);
extern char *report(ior_rec_t *u, calc_rec_t *c);

extern cert_pages_t *new_cert_pages_t();
extern void free_cert_pages_t(cert_pages_t *);

double get_constant(int vcode, int unit);
double get_pfactor(int ptype, int itype);

extern void two_mast_rig(ior_rec_t *, calc_rec_t *);
extern void gaff_main(ior_rec_t *, calc_rec_t *);
extern double triangle(double, double, double);

extern void get_plot_profile (ior_rec_t*, calc_rec_t* , double **, int*, double **, int*);
extern void get_plot_plan (ior_rec_t*, calc_rec_t* , double **, int*, double **, int*);
extern void get_plot_section (ior_rec_t*, calc_rec_t* , double **, int*, double **, int*);
