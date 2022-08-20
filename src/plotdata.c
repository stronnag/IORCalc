#include <math.h>
#include <stdlib.h>
#include "ior.h"

void get_plot_profile (ior_rec_t* u, calc_rec_t* c,
                       double **xp, int*xp_count, double **yp, int*yp_count) {
  double *x = calloc(16, sizeof(double));
  double *y = calloc(16, sizeof(double));
  *xp  = x;
  *yp = y;

  double sp1,sp2,vh1;

  sp1=-(c->cmdi+(c->fa-u->vha))/c->lbg/2.0;
  sp2=((c->fa-u->vha)-(c->fai-u->vhai))/u->gsda;
  vh1=c->fa-u->vha+u->ago*(sp1+sp2)/2.0;

  // 0 Stem
  *x++ = 0.0;
  *y++ = u->fs;
  // 1 FG
  *x++ = u->fgo;
  *y++ = c->ff;
  // 2 FIGS
  *x++ = u->fgo+u->gsdf;
  *y++ = c->ffi;
  // 3 FD
  *x++ = u->fgo + c->lbg / 4;
  *y++ = c->ffd;
  // 4 MD
  *x++ = u->fgo + c->lbg/2;
  *y++ = c->fmd;
  // 5 AIGS
  *x++ = u->fgo+c->lbg-u->gsda;
  *y++ = c->fai;
  // 6 AGS
  *x++ = u->fgo+c->lbg;
  *y++ = c->fa;
  // 7 LOA
  *x++ = u->loa;
  *y++  = vh1;
  // AGS
  *x++ = *(*xp+6);
  *y++ = c->fa-u->vha;
  // AIGS
  *x++ = *(*xp+5);
  *y++ = c->fai-u->vhai;
  // MD
  *x++ = *(*xp+4);
  *y++ = -c->cmdi;
  // FD
  *x++ = *(*xp+3);
  *y++ =-c->fdi;
  //  FIGS
  *x++ = *(*xp+2);
  *y++ =c->ffi-sqrt(0.140625*u->b*u->b-0.25*u->bf*u->bf);
  // FGS
  *x++ = *(*xp+1);
  *y++ = c->ff-sqrt(0.0625*u->b*u->b-0.25*u->bf*u->bf);
  // Stem
  *x++ = 0.0;
  *y++ = u->fs;
  *xp_count = *yp_count = (x-*xp);
}

void get_plot_plan(ior_rec_t* u, calc_rec_t* c,
                       double **xp, int*xp_count, double **yp, int*yp_count) {
  double *x = calloc(16, sizeof(double));
  double *y = calloc(16, sizeof(double));

  *xp  = x;
  *yp = y;

  // Stem
  *x++ = 0.0;
  *y++ = 0.0;
  // FGS
  *x++ = u->fgo;
  *y++ = u->bf/2;
  // FIGS
  *x++ = u->fgo+u->gsdf;
  *y++  = u->bfi/2;
  // BMAX
  *x++ = u->sbmax;
  *y++ = u->bmax/2;
  // AIGS
  *x++ = u->fgo+c->lbg-u->gsda;
  *y++ = u->bai/2;
  // AGS
  *x++  = u->fgo+c->lbg;
  *y++  = u->ba/2;
  // LOA
  *x++  = u->loa;
  *y++ = 0.0;
  *xp_count = *yp_count = (x-*xp);
}

void get_plot_section (ior_rec_t* u, calc_rec_t* c, double **xp, int*xp_count, double **yp, int*yp_count) {
  double *x = calloc(16, sizeof(double));
  double *y = calloc(16, sizeof(double));
  *xp  = x;
  *yp = y;

  // C/L
  *x++ = 0.0;
  *y++ = (u->fbm==0.0) ? c->fmd : u->fbm;
  // BMAX
  *x++ = u->bmax/2;
  *y++  = **yp;
  // B
  *x++ = u->b/2;
  *y++ = **yp - ((u->bbs==0.0) ? u->bmax/6.0 : u->bbs);
  // BWL
  *x++ = u->bwl/2;
  *y++ = 0.0;
  // OMD
  *x++ = 0.375*u->b;
  *y++  = -c->omdi;
  // MD
  *x++ = 0.25*u->b;
  *y++ = -c->mdi;
  // CMD
  *x++ = 0.125*u->b;
  *y++ = -c->cmdi;
  // C/L
  *x++  = 0.0;
  *y  = -(1.3*c->cmdi - 0.37*c->mdi+0.07*c->omdi);
  if (*y  > -c->cmdi) {
    *y = -c->cmdi;
  }
  *xp_count = *yp_count = (x-*xp);
}
