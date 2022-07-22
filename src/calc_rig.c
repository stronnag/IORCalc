#include <math.h>
#include <stdio.h>
#include <math.h>
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
       c->i = fmax(u->isp, u->ig + u->ig * (u->go - u->mw) / (u->j - u->go + u->mw)) -
        0.04 * u->b + c->tci;
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

  c->rsaf = 0.5 * c->ic * c->jc * (1.0 + (1.1 * (c->lp - c->jc) / c->lp)) +
         fmax(0.0, 0.125 * c->jc * (c->ic - 2 * c->jc));

  c->ec = u->e + fmax(u->bal - CONSTANTS[9][u->munit], 0.0);

  if (u->is == 0.0) {
    c->rh = u->ig;
    c->rhc = c->ic;
  } else
    c->rhc = c->rh = u->is + u->badx;

  c->xbl1 = c->xbl2 = fmax(0.21 * u->e, CONSTANTS[4][u->munit] + 0.1 * u->e);
  c->xbl3 = c->xbl4 = fmax(0.34 * u->e, CONSTANTS[4][u->munit] + 0.12 * u->e);

  c->xblp = 0.2 * u->p;
  c->xhb = fmax(CONSTANTS[11][u->munit], 0.04 * u->e);

  c->p1 = fmax(0.0, u->bd - 0.05 * u->e);
  c->p2 = fmax(0.0, u->hb - c->xhb) * u->p / u->e;
  c->p3 = fmax(0.0, c->xblp - u->blp);
  c->p4 = fmax(0.0, u->bl1 + c->p3 - c->xbl1);
  c->p5 = fmax(0.0, u->bl2 - c->xbl2) / 6.0;
  c->p6 = fmax(0.0, u->bl3 - c->xbl3) / 6.0;
  c->p7 = fmax(0.0, u->bl4 - c->xbl4) / 6.0;

  c->p8 = (u->e > 40.0 / CONSTANTS[6][u->munit]) ? fmax(0.0, u->bl5 - c->xbl2) / 6.0 : 0.0;

  c->xbl1 = fmax(u->bl1, c->xbl1 - c->p3);
  c->xbl2 = fmax(u->bl2, c->xbl2);
  c->xbl3 = fmax(u->bl3, c->xbl3);
  c->xbl4 = fmax(u->bl4, c->xbl4);

  c->xhb = fmax(u->hb, c->xhb);

  /*   if (u->g == 0.0)
        {
  */
  c->bpen = (c->p4 + c->p5 + c->p6 + c->p7 + c->p8) * u->p / u->e;
  c->p9 = fmax(0.0, c->bad - 0.05 * u->p - CONSTANTS[10][u->munit] - c->tci);
  c->p10 = fmax(0.0, 0.96 * c->rh - u->p - u->bas);

  c->xmgu = fmax(0.28 * u->e + 0.016 * u->p + CONSTANTS[15][u->munit], 0.38 * u->e);
  c->xmgm = fmax(0.5 * u->e + 0.022 * u->p + CONSTANTS[16][u->munit], 0.65 * u->e);

  if (u->mgu * u->mgm > 0.0) {
    c->gpen = u->p / u->e * (fmax(0.0, u->mgu - c->xmgu) + fmax(0.0, u->mgm - c->xmgm));
    c->rgf = (u->hb + 2.0 * u->mgu + 3.0 * u->mgm + 2.0 * u->e) /
          (u->hb + 2 * c->xmgu + 3 * c->xmgm + 2 * u->e);

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
  /*
        }
     else
        {
        p3=p10=p2=xmgu=xmgm=xblp=xhb=satm=0.0;
        rgf=1.0;
     }
     p4=max(0.0,bl1-xbl1)/6.0;
     p9=max(0,bad+5e-2*(h+.6*g)-CONSTANTS(m,11))
     bpen=p4+p5+p6+p7+p8
     hc=h+p1+bpen+p9
     if mule=2 then satm=triangle(s1,s2,s3)
     rsam=.35*(hc*ec+sqrt(hc^2+ec^2))+max(0,.2*ec*(hc-2*e))+.6*satm
     }
     if mule = 1 then rsam=rsam+.5*triangle(s1,s2,s3)

  */

  c->smin = c->rgf * 0.094 * c->rhc * c->rhc;
  c->rsama = c->rsam;
  c->rsam = fmax(c->smin, c->rsam * c->rgf);

  /*
     if (eb == 0.0)
        {
  */
  c->satc = 0.1 * (c->rsaf - 1.43 * c->rsam);
  c->satca = fmax(0.3 * (c->rsaf - 2.2 * c->rsam), -0.25 * c->rsaf);
  c->rsat =c-> rsam + c->rsaf + c->satc;
  c->rsata = fmin(c->rsaf + c->rsam + c->satca, c->rsat);
  /*
        }
     else
   if is > 0 then
   rsag=0
   ebc=eb+ysf*.5
   rsab=max(9.4e-2*ic^2,.35*ebc*(py+is))
   rsac=rsam+rsab^2/(rsam+rsab)
   ris=is
   else
   if iy > i then
   rsab=0
   ris=iy
   efc=ey+max(baly-CONSTANTS(m,10),0)
   fp1=max(0,bdy-5e-2*ey)
   fp2=max(0,hby-max(CONSTANTS(m,12),4e-2*ey))
   fp3=max(0,.2*py-blpy)
   fp4=max(0,by1-max(.1*ey+CONSTANTS(m,5),.16*ey)+fp3)
   fp5=max(0,by2-max(.1*ey+CONSTANTS(m,5),.16*ey))/6
   fp6=max(0,by3-max(.12*ey+CONSTANTS(m,5),.18*ey))/6
   fp7=max(0,by4-max(.12*ey+CONSTANTS(m,5),.18*ey))/6
   fp8=max(0,by5)
   if gf = 0 then
   fp10=max(0,.96*i-py-bads)
   fp9=max(0,bads-5e-2*py-CONSTANTS(m,11))
   bfpen=(fp2+fp4+fp5+fp6+fp7+fp8)*py/max(ey,1e-2)
   psfc=py+bfpen+fp1+fp9+fp10
   rsag=.35*efc*psfc+max(0,.2*efc*(psfc-2*ey))
   else
   fp4=max(0,by1-.1*ey-CONSTANTS(m,5))/6
   fp9=max(0,bads-5e-2*(hf+.6*gf)-CONSTANTS(m,11))
   fp10=0:fp2=0:fp3=0
   bfpen=fp4+fp5+fp6+fp7+fp8
   hfc=hy+bfpen+fp1+fp9
   if ysd*ysmg*s6 > 0 then
   satf=triangle(ysd,ysmg,s6)
   else
   satf=0
   end if
   rsag=.35*(hfc*efc+gy*sqrt(hfc^2+efc^2))+max(0,.2*efc*(hfc-2*ey))+satf
   end if
   rsag=max(rsag,9.4e-2*ic^2)
   rsac=rsam+rsag^2/(rsam+rsag)
   else
   eyc=ey
   if py > 0 then eyc=max(ey,.85*e*py/p)
   eyc=eyc+max(0,baly-CONSTANTS(m,10))
   y1=max(0,bdy-5e-2*ey)
   y2=max(0,hby-max(CONSTANTS(m,12),4e-2*ey))
   y3=max(0,.2*py-blpy)
   y4=max(0,by1-max(.1*ey+CONSTANTS(m,5),.16*ey)+y3)
   y5=max(0,by2-max(.1*ey+CONSTANTS(m,5),.16*ey))/6
   y6=max(0,by3-max(.12*ey+CONSTANTS(m,5),.18*ey))/6
   y7=max(0,by4-max(.12*ey+CONSTANTS(m,5),.18*ey))/6
   y8=max(0,by5)
   if hy*gy =0 then
   bypen=(y2+y4+y5+y6+y7+y8)*py/ey
   y9=max(0,bady-5e-2*py-CONSTANTS(m,11))
   y10=max(0,.96*iy-py-bads)
   pyc=py+bypen+y1+y9+y10
   rsay=.5*eyc*pyc
   else
   y2=0:y10=0
   y4=max(0,by1-.1*ey-CONSTANTS(m,5))/6
   bypen=y4+y5+y6+y7+y8
   y9=max(0,bady-5e-2*(hy+.6*gy)-CONSTANTS(m,11))
   hyc=hy+bypen+y1+y9
   rsay=.5*((hyc*eyc)+gy*sqrt(hyc^2+eyc^2))
   end if
   rsak=ysd*(ysd+ysmg)*.1*eb/e
   ysac=max(rsay,rsak)
   rsac=rsam+ysac^2/(rsam+ysac)
   end if
   end if
   satc=.1*(rsaf-1.43*rsac)
   satca=max(.3*(rsaf-2.2*rsac),-.25*rsaf)
   rsat=rsac+rsaf+satc
   rsata=min(rsat,rsaf+rsac+satca)
   end if
   if eb > 0 then
   rsal=rsam+.4*ris*(eb+ysf)
   else
   if ysac>0 then
   rsal=.8*(rsay+ysd*(ysmg+ysf)/3)
   else
   rsal=0
   end if
   end if
  */
  c->rsal = fmax(1.3 * c->rsam, c->rsal);
  c->spin = 1.01 * c->jc * fmax(0.95 * sqrt(c->i * c->i + c->jc * c->jc), u->sl);
  c->rsala = c->rsal;
  c->s = sqrt(fmax(c->rsat, fmax(c->rsal, c->spin)));
  c->shr = c->s * (8.66 / c->l + 1.0 / sqrt(u->b * c->mdia)) - c->l / (10 *CONSTANTS[0][u->munit]);
  c->scf = fmax(1.0, 1 + 0.04 * (c->shr - 16.1));
  c->sc = c->s * c->scf;
}
