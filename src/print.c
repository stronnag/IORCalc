#include "iorext.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *show_rtg(ior_rec_t *u, calc_rec_t *c) {
    char *buf = NULL;
    int n = asprintf(
        &buf,
        "        FF    FA    FMD    BWL      RMC    CGF      EPF    DSPL\n"
        "    %7.3f%6.3f%6.3f%7.3f%10.1f%8.4f%8.4f%8.1f\n"
        "         I       JC        S      MR      R        RA    TMF\n"
        "     %9.4f%8.4f%8.4f%10.4f%8.4f%8.4f%7.4f\n",
        u->ffm, u->fam, c->fmd, u->bwl, c->rmc, c->cgf, c->epf, c->dspl, c->i,
        c->jc, c->s, c->mr, c->r, c->ra, c->tmf);
    if (n == -1) {
        free(buf);
        buf = NULL;
    }
    return buf;
}

static void write_file(char *fn, cert_pages_t *cp, int opt) {
    FILE *fp;

    if (fn == NULL || *fn == 0 || (*fn == '-' && *(fn + 1) == 0)) {
        fp = fdopen(1, "w");
    } else {
        fp = fopen(fn, "w");
        if (fp == NULL) {
            fprintf(stderr, " Error on %s\n", fn);
            return;
        }
    }

    if (opt == 2) {
        char **ptr;
        for (ptr = cp->page1; *ptr != NULL; ptr++) {
            fputs(*ptr, fp);
            fputc('\n', fp);
        }
        fputc('\f', fp);
        for (ptr = cp->page2; *ptr != NULL; ptr++) {
            fputs(*ptr, fp);
            fputc('\n', fp);
        }
    } else {
        char **ps1 = cp->page1;
        char **ps2 = cp->page2;
        while (true) {
            char *s1 = *ps1;
            char *s2 = *ps2;
            if (s1 == NULL && s2 == NULL) {
                break;
            }
            if (s1 != NULL) {
                fprintf(fp, "%-70s", s1);
            } else {
                fprintf(fp, "%-70c", ' ');
            }
            ps1++;
            if (s2 != NULL) {
                fputs(s2, fp);
                ps2++;
            }
            fputc('\n', fp);
        }
    }
    fclose(fp);
}

void pcert(ior_rec_t *u, calc_rec_t *c, char *fn, int opt) {
    cert_pages_t *pages = new_cert_pages_t();
    ior_cert(u, c, pages);
    write_file(fn, pages, opt);
    free_cert_pages_t(pages);
}
