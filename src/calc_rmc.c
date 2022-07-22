#include "ior.h"
#include <math.h>
#include <stdio.h>
#include <sys/param.h>

void calc_rmc(ior_rec_t *u, calc_rec_t *c)
{
     double sum1[2], sum2, sum3[2], sum4[2], sum5, wnum, top, bot;
     int i, j;

  if (u->ifyr > 1987) {
       wnum = sum2 = sum5 = 0.0;
    for (i = 0; i < 5; i++) {
      if (u->wts[i] == 0.0)
        break;
      wnum++;
      sum2 += u->wts[i];
      sum5 += u->wts[i] * u->wts[i];
    }

    for (i = 0; i < 2; i++) {
      sum1[i] = sum3[i] = sum4[i] = 0.0;
      for (j = 0; j < wnum; j++) {
        sum1[i] += u->pdef[i][j];
        sum3[i] += u->pdef[i][j] * u->wts[j];
        sum4[i] += u->pdef[i][j] * u->pdef[i][j];
      }

      top = (wnum * sum3[i] - sum1[i] * sum2);
      bot = (wnum * sum4[i] - sum1[i] * sum1[i]);

      c->slp[i] = top / bot;
      c->cor[i] = top / sqrt(bot * (wnum * sum5 - sum2 * sum2));
    }

    c->rm = 0.004375 * (c->slp[0] + c->slp[1]) * (u->swd + u->pwd) * u->pl;

  } else {
       c->arm = u->aw * u->awd * 0.0175 * u->pl / u->apd;
       c->brm = u->bw * u->bwd * 0.0175 * u->pl / u->bpd;
       c->crm = u->cw * u->cwd * 0.0175 * u->pl / u->cpd;
       c->drm = u->dw * u->dwd * 0.0175 * u->pl / u->dpd;
       c->rm = 0.25 * (c->arm + c->brm + c->crm + c->drm);
  }

  c->rmc = c->rm + 0.0175 * (u->wcba * u->cbda + u->wcbb * u->cbdb);
  c->tr = MAX(5.15, CONSTANTS[8][u->munit] * 0.97 * c->l * pow(u->bwl, 3.0) / c->rmc);
  c->cgf = (c->tr > 35.0)
            ? 0.0064 * c->tr + 0.744
            : 2.2 / (c->tr - 5.1) + 0.00023 * c->l * CONSTANTS[6][u->munit] + 0.8693;

  if (c->cgf > 0.968 || c->ihy > 1984 || (c->ihy == 1984 && c->imo > 10))
    c->cgfm = 0.968;
  else {
    c->cgfm = 0.000115 * c->l * CONSTANTS[6][u->munit] + 0.9588;
    c->cgf = 0.5 * (c->cgf + 0.968);
  }

  c->bwl1 = MIN(3.0 + (u->omd - c->fmd) / (u->md - u->omd) * u->b / 4.0, 0.8 * u->b);

  if (c->bwl1 < u->bwl)
    c->bwl1 = u->bwl;

  c->sv =
      ((0.03 * c->l * CONSTANTS[12][u->munit] * pow(c->bwl1, 3.0) - 54.0 * c->rm) / c->dspl +
       0.6 * c->cmdi - 0.54 * u->cmd) *
          CONSTANTS[6][u->munit] +
      0.25;
}
