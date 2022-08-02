#include "iorext.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
