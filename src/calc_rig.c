#include <math.h>
#include <stdio.h>
#include <sys/param.h>

#include "iorext.h"

void calc_rig(ior_rec_t *u, calc_rec_t *c) {
  c->tci = (u->fs > 0.0) ? fmax(0.0, u->fbi - u->fs + 0.04 * u->b) : 0.0;

  if (u->go + u->mw == 0.0) {
    c->i = u->ig + c->tci;
    c->sph = u->sps + c->tci;
    c->psh = fmax(0.0, c->sph - 0.25 * c->i);
    c->bad = u->bas + c->tci;
  } else {
    c->bad = u->bas - 0.04 * u->b + c->tci;
    c->i = fmax(u->isp, u->ig + u->ig * (u->go - u->mw) / (u->j - u->go + u->mw)) - 0.04 * u->b + c->tci;
    c->psh = fmax(0.0, u->sps - 0.27 * u->isp);
  }

  c->jc = fmax(u->j, fmax(u->spl, u->smw / 1.8));
  c->shbs = fmax(0.0, u->hbs - 0.05 * c->jc);
  c->plf = fmax(0.0, 2.0 * (u->sl - 0.95 * sqrt(c->i * c->i + c->jc * c->jc)));
  c->tti = c->plf + c->psh + c->shbs * 1.8 * u->sl / fmax(1.0, u->smw);
  c->ic = c->i + c->tti;
  c->lp = fmax(1.5 * c->jc, fmax(u->lpg + u->fsp, u->lpis));

  c->smwm = trunc_to(fmax(u->smw, 1.8 * c->jc), u->munit + 1);
  c->slm = trunc_to(fmax(u->sl, 0.95 * sqrt(c->i * c->i + c->jc * c->jc)), u->munit + 1);
  c->ajlmx = sqrt(u->j * u->j + u->ig * u->ig);

  c->rsaf = 0.5 * c->ic * c->jc * (1.0 + (1.1 * (c->lp - c->jc) / c->lp)) + fmax(0.0, 0.125 * c->jc * (c->ic - 2 * c->jc));

  c->ec = u->e + fmax(u->bal - get_constant(u->munit, 9), 0.0);

  if (u->is == 0.0) {
    c->rh = u->ig;
    c->rhc = c->ic;
  } else {
    c->rhc = c->rh = u->is + u->badx;
  }

  c->xbl1 = c->xbl2 = fmax(0.21 * u->e, get_constant(u->munit, 4) + 0.1 * u->e);
  c->xbl3 = c->xbl4 = fmax(0.34 * u->e, get_constant(u->munit, 4) + 0.12 * u->e);

  c->xblp = 0.2 * u->p;
  c->xhb = fmax(get_constant(u->munit, 11), 0.04 * u->e);

  c->p1 = fmax(0.0, u->bd - 0.05 * u->e);
  c->p2 = fmax(0.0, u->hb - c->xhb) * u->p / u->e;
  c->p3 = fmax(0.0, c->xblp - u->blp);
  c->p4 = fmax(0.0, u->bl1 + c->p3 - c->xbl1);
  c->p5 = fmax(0.0, u->bl2 - c->xbl2) / 6.0;
  c->p6 = fmax(0.0, u->bl3 - c->xbl3) / 6.0;
  c->p7 = fmax(0.0, u->bl4 - c->xbl4) / 6.0;

  c->p8 = (u->e > 40.0 / get_constant(u->munit, 6)) ? fmax(0.0, u->bl5 - c->xbl2) / 6.0 : 0.0;

  c->xbl1 = fmax(u->bl1, c->xbl1 - c->p3);
  c->xbl2 = fmax(u->bl2, c->xbl2);
  c->xbl3 = fmax(u->bl3, c->xbl3);
  c->xbl4 = fmax(u->bl4, c->xbl4);

  c->xhb = fmax(u->hb, c->xhb);

  if (u->g == 0.0) {
    c->bpen = (c->p4 + c->p5 + c->p6 + c->p7 + c->p8) * u->p / u->e;
    c->p9 = fmax(0.0, c->bad - 0.05 * u->p - get_constant(u->munit, 10) - c->tci);
    c->p10 = fmax(0.0, 0.96 * c->rh - u->p - u->bas);

    c->xmgu = fmax(0.28 * u->e + 0.016 * u->p + get_constant(u->munit, 15), 0.38 * u->e);
    c->xmgm = fmax(0.5 * u->e + 0.022 * u->p + get_constant(u->munit, 16), 0.65 * u->e);

    if (u->mgu * u->mgm > 0.0) {
      c->gpen = u->p / u->e * (fmax(0.0, u->mgu - c->xmgu) + fmax(0.0, u->mgm - c->xmgm));
      c->rgf = (u->hb + 2.0 * u->mgu + 3.0 * u->mgm + 2.0 * u->e) / (u->hb + 2 * c->xmgu + 3 * c->xmgm + 2 * u->e);

      c->rgf = fmax(c->rgf, 0.83);
      c->rgf = fmin(1.0, c->rgf);

      c->xmgu = u->mgu;
      c->xmgm = u->mgm;
    } else {
      c->gpen = 0.0;
      c->rgf = 1.0;
    }
    c->ttp = c->bpen + c->p2 + c->p9 + c->p10 + c->p1 + c->gpen;
    c->pc = u->p + c->ttp;
    c->rsam = 0.35 * c->pc * c->ec + fmax(0.0, 0.2 * c->ec * (c->pc - 2 * u->e));
  } else {
    fprintf(stderr, "Gaff mainsail!\n");
    gaff_main(u, c);
  }

  // anyone can have a mule .....
  if (u->mule == 1) {
    c->rsam = c->rsam + 0.5 * triangle(u->s1, u->s2, u->s3);
  }
  c->smin = c->rgf * 0.094 * c->rhc * c->rhc;
  c->rsama = c->rsam;
  c->rsam = fmax(c->smin, c->rsam * c->rgf);

  if (u->eb == 0.0) {
    c->rsal = 0.0;
    c->satc = 0.1 * (c->rsaf - 1.43 * c->rsam);
    c->satca = fmax(0.3 * (c->rsaf - 2.2 * c->rsam), -0.25 * c->rsaf);
    c->rsat = c->rsam + c->rsaf + c->satc;
    c->rsata = fmin(c->rsaf + c->rsam + c->satca, c->rsat);
  } else {
    fprintf(stderr, "Two mast rig\n");
    two_mast_rig(u, c);
  }
  c->rsal = fmax(1.3 * c->rsam, c->rsal);
  c->spin = 1.01 * c->jc * fmax(0.95 * sqrt(c->i * c->i + c->jc * c->jc), u->sl);
  c->rsala = c->rsal;
  c->s = sqrt(fmax(c->rsat, fmax(c->rsal, c->spin)));
  c->shr = c->s * (8.66 / c->l + 1.0 / sqrt(u->b * c->mdia)) - c->l / (10 * get_constant(u->munit, 0));
  c->scf = fmax(1.0, 1 + 0.04 * (c->shr - 16.1));
  c->sc = c->s * c->scf;
}
