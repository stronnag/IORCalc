#include <stdlib.h>
#include "ior.h"

ior_rec_t *allocate_ior_rec(void) {
  ior_rec_t *nr = calloc(1,sizeof(ior_rec_t));
  return nr;
}

calc_rec_t *allocate_calc_rec(void) {
  calc_rec_t *nr = calloc(1,sizeof(calc_rec_t));
  return nr;
}
