#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "iorext.h"

static void show_error(char** buf, char *fmt, double var1, double var2) {
     char lbuf[256];

     if (var1 > var2) {
          int n = snprintf(lbuf, 256, fmt, var1, var2);
	  lbuf[n++] = '\n';
	  lbuf[n] = 0;
	  strcpy(*buf, lbuf);
	  *buf += n;
     }
}

char * report(ior_rec_t *u, calc_rec_t *c) {
  double t1, t2;
  char *cbuf = malloc(4096);
  char *buf = cbuf;
  *buf = 0;

  show_error(&buf, "Mid Depths Inflected", u->cmd - u->md, u->md - u->omd);
  show_error(&buf, "Mid Depths Inflected", 0.0, u->cmd - u->md);
  show_error(&buf, "MD %8.3g > CMD %8.3g", u->md, u->cmd);
  show_error(&buf, "Buttock heights reversed", u->bha, u->bhai);
  show_error(&buf, "BHA %8.3g > BHAI %8.3g", u->bha, u->bhai);
  show_error(&buf, "VHA %8.3g > VHAI %8.3g", u->vha, u->vhai);
  show_error(&buf, "B %8.3g > BMAX %8.3g", u->b, u->bmax);
  show_error(&buf, "BF %8.3g > BFI %8.3g", u->bf, u->bfi);
  show_error(&buf, "BA %8.3g > BAI %8.3g", u->ba, u->bai);

  t1 = fmax(c->fdi, c->cmdi);
  t1 = fmax(t1, u->vhai - c->fai);
  t1 = fmax(t1, u->vha - c->fa);
  show_error(&buf, "Hull depth exceeds DM ", t1, c->dm);

  if (u->bha)
    t1 = sqrt(pow(0.5 * u->ba - 0.15 * u->b, 2.0) + u->bha * u->bha) +
         sqrt(0.0225 * u->ba * u->ba + (u->vha - u->bha) * (u->vha - u->bha));
  else
    t1 = sqrt(u->vha * u->vha + 0.25 * u->ba * u->ba);

  show_error(&buf, "Error in aft girth, gla %8.3g, calc %8.3g", 2 * t1, 0.75 * u->b + u->gd);

  t2 = 0.049 *CONSTANTS[4][u->munit];

  if (u->dmt) {
       t1 = u->fmds +
	    (u->fais - u->fmds) * (u->sdm - u->fgo - 0.5 * c->lbg) / (0.5 * c->lbg - u->gsda);
       show_error(&buf, "DMT approximation error %8.4g", fabs(u->dmt - u->dms - t1), t2);
  }

  if (u->pdt) {
       t1 = u->fmds +
	    (u->fais - u->fmds) * (u->spd - u->fgo - 0.5 * c->lbg) / (0.5 * c->lbg - u->gsda);
       show_error(&buf, "PDT approximation error %8.4g", fabs(u->pdt - u->pds - t1), t2);
  }

  if (u->glai) {
       t1 = 0.875 * u->b + u->gd;
       show_error(&buf, "GLAI required, girth %8.3g, GLAI %8.3g", t1, u->glai);
  }

  show_error(&buf, "YCOR correction ", 0.0, c->ycor);

  show_error(&buf, "Large absolute FC %8.3g", fabs(c->fc), 0.25);

  if (c->pd)
    show_error(&buf, "ESD %8.3g exceeds PD %8.3g", c->esd, c->pd);

  show_error(&buf, "Depth correction to PF", c->pf, PFACTOR[u->ipt - 1][c->ecod - 1]);

  if (u->st1) {
    if (u->apt) {
      show_error(&buf, "Strut Fail 1.5*prdc > edl", 1.5 * c->prdc, u->apt);
      show_error(&buf, " Strut Fail edc > 0.75*prdc", u->apb, 0.75 * c->prdc);
      show_error(&buf, " Strut Fail 0.08*prdc > st1", 0.08 * c->prdc, u->st1);
      show_error(&buf, " Strut Fail 0.3*prdc > st2", 0.3 * c->prdc, u->st2);
      show_error(&buf, " Strut Fail st3 > 0.75*prdc", 0.75 * c->prdc, u->st3);
    } else {
      show_error(&buf, " Exp Shaft Fail 1.5*prdc > esl", 1.5 * c->prdc, u->esl);
      show_error(&buf, " Exp Shaft Fail esc > 0.75*prdc", u->esc, 0.75 * c->prdc);
      show_error(&buf, " Exp Shaft Fail 0.05*prdg > st1", 0.05 * c->prdc, u->st1);
      show_error(&buf, " Exp Shaft Fail 0.2*prdc > st2", 0.2 * c->prdc, u->st2);
      show_error(&buf, " Exp Shaft Fail st3 > 0.5*prdc", u->st3, 0.5 * c->prdc);
    }
  }

  show_error(&buf, "LPG+FSP %8.3g exceeds 1.5*JC %8.3g", u->lpg + u->fsp, 1.5 * c->jc);

  if (c->ic > c->i) {
    show_error(&buf, "SPS %8.3g exceeds 0.27*ISP %8.3g", u->sps, 0.27 * u->isp);
    show_error(&buf, "HBS %8.3g exceeds 0.05*JC %8.3g", u->hbs, 0.05 * c->jc);
  }

  show_error(&buf, "BAL %8.3g exceeds limit %8.3g", u->bal, CONSTANTS[9][u->munit]);

  if (c->pc > u->p) {
       show_error(&buf, "BAS %8.3g exceeds limit %8.3g", c->bad,
		  0.05 * u->p + c->tci + CONSTANTS[10][u->munit]);
       show_error(&buf, "I/P penalty %8.3g %8.3g", 0.96 * c->i, u->p + c->bad);
       show_error(&buf, "HB %8.3g exceeds limit %8.3g", u->hb, c->xhb);
       show_error(&buf, "BD %8.3g exceeds limit %8.3g", u->bd, 0.05 * u->e);
       show_error(&buf, "Limit %8.3g exceeds BLP %8.3g", c->xblp, u->blp);
       show_error(&buf, "BL1 %8.3g exceeds limit %8.3g", u->bl1, c->xbl1);
       show_error(&buf, "BL2 %8.3g exceeds limit %8.3g", u->bl2, c->xbl2);
       show_error(&buf, "BL3 %8.3g exceeds limit %8.3g", u->bl3, c->xbl3);
       show_error(&buf, "BL4 %8.3g exceeds limit %8.3g", u->bl4, c->xbl4);
       show_error(&buf, "BL5 %8.3g exceeds limit %8.3g", u->bl5, c->xbl5);
       show_error(&buf, "MGU %8.3g exceeds limit %8.3g", u->mgu, c->xmgu);
       show_error(&buf, "MGM %8.3g exceeds limit %8.3g", u->mgm, c->xmgm);
  }

  if (c->jc > u->j) {
       show_error(&buf, "SPL %8.3g exceeds J %8.3g", u->spl, u->j);
       t1 = fmax(u->j, u->spl);
       show_error(&buf, "SMW exceeds limit %8.3g", u->smw, 1.8 * t1);
       show_error(&buf, "MW %8.3g exceeds GO %8.3g", u->mw, u->go);
  }
  show_error(&buf, "SV %8.4g is positive", c->sv, 0.0);
  return cbuf;
}
