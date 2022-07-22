#include <math.h>
#include <string.h>
#include <stdio.h>
#include "ior.h"


double trunc_to(double value, int decp) {
     double expn;
     int temp;

     expn = pow(10.0, (double)decp);
     temp = (int)(value * expn + 0.5);
     return ((double)temp / expn);
}
