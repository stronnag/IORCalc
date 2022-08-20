#include "ior.h"
#include <assert.h>

static double CONSTANTS[20][2] = {{10.0, 3.048}, {1.2, 0.366}, {2.0, 0.61},     {3.0, 0.915},     {1.0, 0.305},
                                  {0.15, 0.046}, {1.0, 3.281}, {0.1, 0.006243}, {1.0, 16.08},     {0.5, 0.152},
                                  {4.0, 1.219},  {0.5, 0.152}, {64.0, 1025.8},  {100.0, 1000.0},  {1.0, 2.205},
                                  {0.85, 0.26},  {1.2, 0.37},  {0.3048, 1.0},   {0.164042, 0.05}, {5.85, 1.783}};

static double PFACTOR[3][7] = {{1.05, 0.525, 1.05, 0.0, 0.0, 2.05, 2.05},
                               {0.95, 0.475, 0.40, 0.0, 0.0, 0.90, 0.90},
                               {0.95, 0.475, 0.45, 0.0, 0.0, 0.95, 0.95}};

double get_constant(int unit, int vcode) {
  assert((unit == 0 || unit == 1) && (vcode >= 0 && vcode < 20));
  return CONSTANTS[vcode][unit];
}

double get_pfactor(int ptype, int itype) {
  assert((ptype >= 0 || ptype < 4) && (itype >= 0 && itype < 7));
  return PFACTOR[ptype][itype];
}
