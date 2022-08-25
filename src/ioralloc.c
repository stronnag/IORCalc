#include <stdlib.h>
#include <stdbool.h>

#include "ior.h"

ior_rec_t *allocate_ior_rec(void) {
  ior_rec_t *nr = calloc(1,sizeof(ior_rec_t));
  return nr;
}

calc_rec_t *allocate_calc_rec(void) {
  calc_rec_t *nr = calloc(1,sizeof(calc_rec_t));
  return nr;
}

bool is_empty(ior_rec_t * u) {
  bool ret=true;
  for(int i = 0; i < sizeof(ior_rec_t); i++) {
    if (*(char*)((char *)u+i) != 0) {
      return false;
    }
  }
  return ret;
}
