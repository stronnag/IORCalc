#include <math.h>
#include <stdio.h>
#include <math.h>
#include <sys/param.h>

#include "iorext.h"

void calc_rtg(ior_rec_t *u, calc_rec_t *c) {
     int idiv;
     double z, af, age, pars, parl, fbx;

  if (c->ihy < 1973 && (u->irgy < 1975 || (u->irgy == 1975 && u->irgm < 11)))
    idiv = 0;
  else if (c->ihy < 1973)
    idiv = 1;
  else if (c->ihy < 1976)
    idiv = 2;
  else
    idiv = 3;

  if (idiv > 0) {
    c->sa = c->s;
    c->satca = 0;
  } else
    c->sa = sqrt(MAX(c->rsata, MAX(c->rsala, c->spin)));

  if (idiv < 2) {
       c->dlfa = 0.98;
    c->cgfa = (c->cgf + c->cgfm - 0.005) * 0.5;
    c->scfa = (c->shr > 15.5) ? c->scf : 1.0 + 0.024 * (c->shr - 15.5);
  } else {
       c->cgfc = MIN(1.0, MAX(0.5, 0.375 * u->b / (c->cmdi + c->mdi + c->omdi) - 0.5));
       c->cgfa = c->cgfc * (c->cgf - c->cgfm) + c->cgfm;
       c->dlfa = 0.98 + MAX(0.0, 5.54 * pow(c->bdr - 0.94, 1.92));
    if (idiv == 3) {
      c->dlfa = MIN(c->dlfa, 1.1);
      c->cbfa = c->cbf;
      c->msar = 0.5 * (c->pc * c->ec + c->ic * c->lp);
      c->shra = sqrt(c->msar) * (8.66 / c->l + 1.0 / sqrt(u->b * c->mdia)) -
             c->l / (10.0 *CONSTANTS[0][u->munit]);
      c->scfa = 1.0 + MAX(0.0, 0.04 * (c->shra - 15.5));
    } else {
      c->dlfa = MIN(c->dlfa, 0.5 * (1.0 + c->dlf));
      c->scfa = c->scf;
    }
  }
  if (idiv < 3)
    c->cbfa = (c->cbf < 1.0) ? 0.99 : 1.0;
  c->sca = c->sa * c->scfa;
  c->tpf = 1.0 + 1.146 * c->atrim;
  c->mr = (0.13 * c->l * c->sc / sqrt(u->b * c->d) + 0.25 * c->l + 0.2 * c->sc + c->dc + c->fc) * c->dlf;
  fbx = (u->fbm > 0.0) ? u->fbm : c->fmd;

  if (c->ihy > 1986) {
       c->csf = pow(((1.98 * (0.2 * (u->bmax + 1.5 * u->bai) * u->bmax / u->b -
                              0.085 * (fbx - c->fai))) /
                     (1.41 / CONSTANTS[17][u->munit] + 0.16 * (c->mr / c->dlf))),
                    0.05);
       c->csf = MAX(1.0, c->csf);
  } else
       c->csf = 1.0;

  c->cgf = c->csf * MAX(c->cgf, c->cgfm);
  c->cgf = MAX(c->cgfm, c->cgf);

  c->lrp = (u->mpen & 1) ? 1.01 : 1.0;

  c->smf = (u->mpen & 2) ? 1.03 : 1.0;

  c->mra1 =
      (0.13 * c->l * c->sca / sqrt(u->b * c->d) + 0.25 * c->l + 0.2 * c->sca + c->fc + c->dc) * c->dlfa;
  pars = c->sca * (0.0777 * c->sca / sqrt(u->b * c->d) + 0.2216);
  parl = c->l * (0.0659 * c->l / pow((c->l * u->b * c->mdia), 1.0 / 3.0) + 0.1738);
  c->mra2 = (pars + parl + c->dc + c->fc) * c->dlfa;
  c->mra = MIN(c->mra1, c->mra2);
  c->r = c->mr * c->epf * c->cgf * c->maf * c->cbf * c->smf * c->lrp * c->tpf;
  c->rior = MAX(15.95 / CONSTANTS[6][u->munit], c->r);
  c->ra = MAX(c->rior * 0.85, c->mra * c->epf * c->cgfa * c->maf * c->cbfa * c->smf * c->lrp * c->tpf);
  if (c->ihy > 1981)
    c->ra = MAX(c->ra, c->rior * 0.968);
  c->rtf = trunc_to(c->rior *CONSTANTS[6][u->munit], 2);
  c->ra = MIN(c->ra, c->rior);
  c->rtfa = MIN(c->rtf, trunc_to(c->ra *CONSTANTS[6][u->munit], 2));
  c->w = 1.6 *
      (CONSTANTS[6][u->munit] *CONSTANTS[6][u->munit] * c->l * u->b * u->b +
       (u->p - c->d / 2.0) * (5.0 *CONSTANTS[6][u->munit] * u->p - 110.0) + 35.0 * c->r) /
      (CONSTANTS[14][u->munit] * (c->i + 0.67 * c->d));
  c->crewno = (int)(0.4 * c->r *CONSTANTS[6][u->munit] + 0.02) - 2;
  c->crewwt = (int)((74.0 * c->r *CONSTANTS[6][u->munit] - 370.0) / CONSTANTS[14][u->munit]);

  z = 1.0 / sqrt(c->rtfa);

  c->tmf = (c->rtfa > 30.5)   ? 0.2738 / (z + 0.0875)
        : (c->rtfa > 22.9) ? 0.2424 / (z + 0.0567)
                        : 0.4039 / (z + 0.2337);

  age = u->yr - 1900;

  if (u->yr > 1972)
    af = 1.0 - MAX(0.0, (0.0015 * (84 - age) * (age + 5) / 88));
  else
    af = MIN(1.0, 0.997 - ((0.015 * (72 - age) * age / 72)));

  c->tmf = trunc_to(c->tmf * af, 4);
}
