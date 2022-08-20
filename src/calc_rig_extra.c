#include <math.h>
#include <stdio.h>
#include <sys/param.h>

#include "iorext.h"

// Heron's formula
double triangle(double a, double b, double c) {
  double sp = (a + b + c) / 2.0;
  return sqrt(sp * (sp - a) * (sp - b) * (sp - c));
}

void gaff_main(ior_rec_t *u, calc_rec_t *c) {
  c->p3 = c->p10 = c->p2 = c->xmgu = c->xmgm = c->xblp = c->xhb = c->satm = 0.0;
  c->rgf = 1.0;
  c->p4 = fmax(0.0, u->bl1 - c->xbl1) / 6.0;
  c->p9 = fmax(0, c->bad + 0.05 * (u->h + 0.6 * u->g) - get_constant(u->munit, 11));
  c->bpen = c->p4 + c->p5 + c->p6 + c->p7 + c->p8;
  c->hc = u->h + c->p1 + c->bpen + c->p9;
  if (u->mule == 2) {
    c->satm = triangle(u->s1, u->s2, u->s3);
  }
  c->rsam = 0.35 * (c->hc * c->ec + sqrt(c->hc * c->hc + c->ec * c->ec)) + fmax(0.0, 0.2 * c->ec * (c->hc - 2.0 * u->e)) +
            0.6 * c->satm;
}

void two_mast_rig(ior_rec_t *u, calc_rec_t *c) {
  if (u->is > 0) { // schooner,
    c->ris = c->rhc;
    c->rsag = 0;
    c->ebc = u->eb + 0.5 * u->ysf;                                          // ysf aka of
    c->rsab = fmax(0.094 * c->ic * c->ic, 0.35 * c->ebc * (u->py + u->is)); // py, aka psf
    c->rsac = c->rsam + c->rsab * c->rsab / (c->rsam + c->rsab);
  } else { // foresail schooner
    if (u->iy > c->i) {
      c->rsab = 0.0;
      c->ris = u->iy;
      c->efc = u->ey + fmax(u->baly - get_constant(u->munit, 10), 0.0);
      c->fp1 = fmax(0.0, u->bdy - 0.05 * u->ey);
      c->fp2 = fmax(0.0, u->hby - fmax(get_constant(u->munit, 12), 0.04 * u->ey));
      c->fp3 = fmax(0.0, 0.2 * u->py - u->blpy);
      c->fp4 = fmax(0.0, u->by1 - fmax(0.1 * u->ey + get_constant(u->munit, 5), 0.16 * u->ey) + c->fp3);
      c->fp5 = fmax(0.0, u->by2 - fmax(0.1 * u->ey + get_constant(u->munit, 5), 0.16 * u->ey)) / 6;
      c->fp6 = fmax(0.0, u->by3 - fmax(0.12 * u->ey + get_constant(u->munit, 5), 0.18 * u->ey)) / 6;
      c->fp7 = fmax(0.0, u->by4 - fmax(0.12 * u->ey + get_constant(u->munit, 5), 0.18 * u->ey)) / 6;
      c->fp8 = fmax(0.0, u->by5);
      if (u->gy == 0.0) {
        c->fp10 = fmax(0.0, 0.96 * u->ig - u->py - u->bads);
        c->fp9 = fmax(0.0, u->bads - 0.05 * u->py - get_constant(u->munit, 11));
        c->bfpen =
            (c->fp2 + c->fp4 + c->fp5 + c->fp6 + c->fp7 + c->fp8) * u->py / fmax(u->ey, 0.01); // ey aka ef, 0.01 safety net
        c->psfc = u->py + c->bfpen + c->fp1 + c->fp9 + c->fp10;
        c->rsag = 0.35 * c->efc * c->psfc + fmax(0.0, 0.2 * c->efc * (c->psfc - 2 * u->ey));
      } else {
        c->fp4 = fmax(0.0, u->by1 - 0.1 * u->ey - get_constant(u->munit, 5)) / 6;
        c->fp9 = fmax(0.0, u->bads - 0.05 * (u->hy + 0.6 * u->gy) - get_constant(u->munit, 11));
        c->fp10 = 0;
        c->fp2 = 0;
        c->fp3 = 0;
        c->bfpen = c->fp4 + c->fp5 + c->fp6 + c->fp7 + c->fp8;
        c->hfc = u->hy + c->bfpen + c->fp1 + c->fp9;
        if (u->ysd * u->ysmg * u->s6 > 0) {
          c->satf = triangle(u->ysd, u->ysmg, u->s6);
        } else {
          c->satf = 0;
        }
        c->rsag = 0.35 * (c->hfc * c->efc + u->gy * sqrt(c->hfc * c->hfc + c->efc * c->efc)) +
                  fmax(0.0, 0.2 * c->efc * (c->hfc - 2 * u->ey)) + 0.6 * c->satf;
      }
      c->rsag = fmax(c->rsag, 0.094 * c->ic * c->ic);
      c->rsac = c->rsam + c->rsag * c->rsag / (c->rsam + c->rsag);
    } else { // Yawl or Ketch
      c->eyc = u->ey;
      if (u->py > 0) {
        c->eyc = fmax(u->ey, 0.85 * u->e * u->py / u->p);
        c->eyc = c->eyc + fmax(0.0, u->baly - get_constant(u->munit, 10));
        c->y1 = fmax(0.0, u->bdy - 0.05 * u->ey);
        c->y2 = fmax(0.0, u->hby - fmax(get_constant(u->munit, 12), 0.04 * u->ey));
        c->y3 = fmax(0.0, 0.2 * u->py - u->blpy);
        c->y4 = fmax(0.0, u->by1 - fmax(0.1 * u->ey + get_constant(u->munit, 5), 0.16 * u->ey) + c->y3);
        c->y5 = fmax(0.0, u->by2 - fmax(0.1 * u->ey + get_constant(u->munit, 5), 0.16 * u->ey)) / 6;
        c->y6 = fmax(0.0, u->by3 - fmax(0.12 * u->ey + get_constant(u->munit, 5), 0.18 * u->ey)) / 6;
        c->y7 = fmax(0.0, u->by4 - fmax(0.12 * u->ey + get_constant(u->munit, 5), 0.18 * u->ey)) / 6;
        c->y8 = fmax(0.0, u->by5);
        if (u->hy * u->gy == 0) {
          c->bypen = (c->y2 + c->y4 + c->y5 + c->y6 + c->y7 + c->y8) * u->py / u->ey;
          c->y9 = fmax(0.0, u->bads - 0.05 * u->py - get_constant(u->munit, 11));
          c->y10 = fmax(0.0, 0.96 * u->iy - u->py - u->bads);
          c->pyc = u->py + c->bypen + c->y1 + c->y9 + c->y10;
          c->rsay = 0.5 * c->eyc * c->pyc;
        } else {
          c->y2 = 0;
          c->y10 = 0;
          c->y4 = fmax(0.0, u->by1 - 0.1 * u->ey - get_constant(u->munit, 5)) / 6;
          c->bypen = c->y4 + c->y5 + c->y6 + c->y7 + c->y8;
          c->y9 = fmax(0.0, u->bads - 0.05 * (u->hy + 0.6 * u->gy) - get_constant(u->munit, 11));
          c->hyc = u->hy + c->bypen + c->y1 + c->y9;
          c->rsay = 0.5 * ((c->hyc * c->eyc) + u->gy * sqrt(c->hyc * c->hyc + c->eyc * c->eyc));
        }
        c->rsak = u->ysd * (u->ysd + u->ysmg) * 0.1 * u->eb / u->e;
        c->ysac = fmax(c->rsay, c->rsak);
        c->rsac = c->rsam + c->ysac * c->ysac / (c->rsam + c->ysac);
      }
    }
    c->satc = 0.1 * (c->rsaf - 1.43 * c->rsac);
    c->satca = fmax(0.3 * (c->rsaf - 2.2 * c->rsac), -0.25 * c->rsaf);
    c->rsat = c->rsac + c->rsaf + c->satc;
    c->rsata = fmin(c->rsat, c->rsaf + c->rsac + c->satca);
  }

  if (c->ysac > 0) {
    c->rsal = 0.8 * (c->rsay + u->ysd * (u->ysmg + u->ysf) / 3);
  } else if (u->ysd > 0) { // aka of
    c->rsal = c->rsam + 0.4 * c->ris * (u->eb + u->ysd);
  } else {
    c->rsal = 0;
  }
}
