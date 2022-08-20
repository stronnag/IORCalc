#include "iorext.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ior_rec_t *allocate_ior_rec(void) {
  ior_rec_t *nr = malloc(sizeof(ior_rec_t));
  if (nr != NULL)
    memset(nr, 0, sizeof(ior_rec_t));
  return nr;
}

calc_rec_t *allocate_calc_rec(void) {
  calc_rec_t *nr = malloc(sizeof(calc_rec_t));
  return nr;
}

char *calc_ior(ior_rec_t *u, calc_rec_t *c) {
  memset(c, 0, sizeof(calc_rec_t));
  calc_hull(u, c);
  calc_rmc(u, c);
  calc_prop(u, c);
  calc_rig(u, c);
  calc_rtg(u, c);
  return show_rtg(u, c);
}

double trunc_to(double value, int decp) {
  double expn;
  int temp;

  expn = pow(10.0, (double)decp);
  temp = (int)(value * expn + 0.5);
  return ((double)temp / expn);
}

cert_pages_t *new_cert_pages_t() {
  cert_pages_t *cp = malloc(sizeof(cert_pages_t));
  cp->page1 = calloc(CERT_LINES, sizeof(char *));
  cp->page2 = calloc(CERT_LINES, sizeof(char *));
  return cp;
}

void free_cert_pages_t(cert_pages_t *cp) {
  for (int i = 0; i < CERT_LINES; i++) {
    free(cp->page1[i]);
    free(cp->page2[i]);
  }
  free(cp);
}
