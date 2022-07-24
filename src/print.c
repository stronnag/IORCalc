#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "iorext.h"

static char *cptrs[66][2];

char * show_rtg(ior_rec_t *u, calc_rec_t *c) {
     char *buf = NULL;
     int n = asprintf(&buf,
		      "        FF    FA    FMD    BWL      RMC    CGF      EPF    DSPL\n"
		      "    %7.3f%6.3f%6.3f%7.3f%10.1f%8.4f%8.4f%8.1f\n"
		      "         I       JC        S      MR      R        RA    TMF\n"
		      "     %9.4f%8.4f%8.4f%10.4f%8.4f%8.4f%7.4f\n",
		      u->ffm, u->fam, c->fmd, u->bwl, c->rmc, c->cgf, c->epf, c->dspl,
		      c->i, c->jc, c->s, c->mr, c->r, c->ra, c->tmf);
     if(n == -1) {
	  free(buf);
	  buf = NULL;
     }
     return buf;
}

static void write_file(char *fn, int opt) {
     int page, loop;
     FILE *fp;

     if (fn == NULL || *fn == 0 || (*fn == '-' && *(fn+1) == 0)) {
	  fp = fdopen(1, "w");
     } else {
	  fp = fopen(fn, "w");
	  if (fp == NULL) {
	       fprintf(stderr, " Error on %s\n", fn);
	       return;
	  }
     }

     if (opt == 2) {
	  for (page = 0; page < 2; page++) {
	       loop = 0;
	       while (cptrs[loop][page] != NULL) {
		    fputs(cptrs[loop++][page], fp);
	       }
	       if(page == 0) {
		    fputc('\f', fp);
	       }
	  }
     } else {
	  char *s1 = NULL;
	  char *s2 = NULL;
	  loop = 0;
	  while(true) {
	       s1 = cptrs[loop][0];
	       s2 = cptrs[loop][1];
	       if (s1 == NULL && s2 == NULL) {
		    break;
	       }
	       if (s1 != NULL) {
		    char* np = strchr(s1,'\n');
		    if(np != NULL)
			 *np = 0;
		    fprintf(fp,"%-75s", s1);
	       } else {
		    fprintf(fp,"%-75c", ' ');
	       }
	       if (s2 != NULL) {
		    fputs(s2, fp);
	       } else {
		    fputc('\n', fp);
	       }
	       loop++;
	  }
     }
     fclose(fp);
}

void pcert(ior_rec_t* u, calc_rec_t* c, char *fn, int opt) {
  int size[2] ;
  char * cert;

  if ((cert = malloc(16*1024)) == NULL) {
       fputs(" Malloc Failure ", stderr);
       exit(3);
  }
  ior_cert(cert, u, c, cptrs, size);
  write_file(fn, opt);
  free(cert);
}
