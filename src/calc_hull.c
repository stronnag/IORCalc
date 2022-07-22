#include <math.h>
#include <stdio.h>
#include <sys/param.h>
#include "iorext.h"

void calc_hull(ior_rec_t *u, calc_rec_t *c) {
  int iz, n;
  double ftop, fbot, z;

  c->ihy = u->yr;
  c->imo = u->mon;

  if (u->syr) {
    c->ihy = MIN(u->syr, u->yr);
    c->imo = MIN(u->smo, c->imo);
  }

  if (u->hyr) {
       c->ihy = u->hyr;
       c->ihm = u->hmo;
  }

  c->lbg = u->loa - u->fgo - u->ago;

  if ((u->ffm + u->fam) == 0.0)
    c->sink = c->trim = 0;
  else {
       c->sink = u->ffs - u->ffm;
       c->trim = u->fam - u->fas + c->sink;
  }

  c->atrim = fabs(c->trim) / c->lbg;

  if (c->atrim < 0.175 || c->ihy < 1984)
       c->atrim = 0;

  c->fmd = u->fmds - c->sink + c->trim * 0.5;
  if (u->fbms != 0.0)
    u->fbm = u->fbms - c->sink + c->trim * (u->sbmax - u->fgo) / c->lbg;

  if (u->wlh1 > 0.0) {
       iz = ((u->fbm - u->wlh1) / CONSTANTS[18][u->munit]);

    u->bwl = u->twl[iz] - (u->twl[iz] - u->twl[iz + 1]) *
                            (u->fbm - u->wlh1 - CONSTANTS[18][u->munit] * iz) /
                            CONSTANTS[18][u->munit];
  }

  for (n = 0; n < 5; n++)
       c->ccor[n] = 0.005 * u->b * u->idc[n];

  c->mdi = u->md - c->fmd - c->ccor[2];
  c->omdi = u->omd - c->fmd - c->ccor[1];
  c->cmdi = u->cmd - c->fmd - c->ccor[3];

  c->mdia = (u->mdias > 0.0) ? u->mdias + (c->omdi / u->b) * (u->bwl + 0.75 * u->b) / 2.0
       : 0.125 * (3.0 * c->cmdi + 2.0 * (c->mdi - c->omdi)) +
       (0.5 * c->omdi * u->bwl) / u->b;

  z = (u->sg == 0.0) ? 1.015 : u->sg;

  c->sgfi = 0.92 * u->b * c->mdia * (1.0 / z - 1.0) / u->bwl;

  c->sink -= 1.1 * c->sgfi;
  c->trim += 0.9 * c->sgfi;

  c->ff = u->ffs - c->sink;
  c->ffi = u->ffis - c->sink + c->trim * u->gsdf / c->lbg;
  c->ffd = u->ffds - c->sink + c->trim * 0.25;
  c->fmd = u->fmds - c->sink + c->trim * 0.5;

  if (u->fbms == 0.0 && u->fbm != 0.0)
    u->fbms = trunc_to(u->fbm + c->sink - c->trim * (u->sbmax - u->fgo) / c->lbg, u->munit + 2);

  if (u->fbms != 0.0)
    u->fbm = u->fbms - c->sink + c->trim * (u->sbmax - u->fgo) / c->lbg;

  c->fai = u->fais - c->sink + c->trim * (c->lbg - u->gsda) / c->lbg;
  c->fa = u->fas - c->sink + c->trim;

  c->pd = (u->pds != 0.0) ? u->pds + c->sink - c->trim * (u->spd - u->fgo) / c->lbg : 0.0;

  c->dm = u->dms + c->sink - c->trim * (u->sdm - u->fgo) / c->lbg;

  c->esd = (u->esds != 0.0) ? u->esds + c->sink - c->trim * (u->spd - u->esl - u->fgo) / c->lbg
                        : 0.0;

  if (u->sfs != 0.0 && u->fss == 0.0 && u->fs != 0.0) {
    u->fss = u->fs + c->sink - c->trim * (u->sfs - u->fgo) / c->lbg;
    u->fbis = u->fbi + c->sink - c->trim * (u->sfs + u->j - u->fgo) / c->lbg;
    u->fss = trunc_to(u->fss, u->munit + 2);
    u->fbis = trunc_to(u->fbis, u->munit + 2);
  }

  if (u->fss != 0.0) {
    u->fs = u->fss - c->sink + (u->sfs - u->fgo) * c->trim / c->lbg;
    u->fbi = u->fbis - c->sink + (u->sfs + u->j - u->fgo) * c->trim / c->lbg;
  }

  if (c->ihy > 1979)
    u->fs = MIN(u->fs, c->ff + (c->ff - c->ffi) * (u->fgo - u->sfs) / u->gsdf);

  if (u->bf > u->bfi)
    u->bf = u->bfi;

  if (c->ihy > 1986) {

    c->cffi = c->ff - c->ffi + (4 * c->fmd - c->fa - 3 * c->ff) * u->gsdf / c->lbg +
           (2 * c->ff - 4 * c->fmd + 2 * c->fa) * pow(u->gsdf / c->lbg, 2.0);

    c->cffi = MAX(c->cffi, 0.0);

  } else
    c->cffi = 0;

  ftop = c->ff - c->cffi - .3 * u->b + .15 * u->bf;

  fbot = 0.125 * u->b + c->ff - c->cffi - c->ffi - 0.15 * (u->bfi - u->bf) + 0.5 * u->gdfi;

  c->foc = u->gsdf * ftop / fbot;

  if (ftop < 0.0 && fbot < 0.0)
    c->foc = -u->fgo;

  c->foc = MIN(c->foc, 1.5 * u->gsdf * 0.25 * u->b / (0.25 * u->b + u->gdfi));

  c->fdi = u->fd - c->ffd - c->ccor[0] * 2;
  c->mdi = u->md - c->fmd - c->ccor[2];
  c->omdi = u->omd - c->fmd - c->ccor[1];
  c->cmdi = u->cmd - c->fmd - c->ccor[3];

  z = c->mdi + c->cmdi;

  c->fdic = (c->fdi > 0.435 * z) ? MIN(0.475 * z, 0.2175 * z + .5 * c->fdi) : c->fdi;

  c->mdia = (u->mdias > 0.0) ? u->mdias + (c->omdi / u->b) * (u->bwl + 0.75 * u->b) / 2.0
                         : 0.125 * (3.0 * c->cmdi + 2.0 * (c->mdi - c->omdi)) +
                               (0.5 * c->omdi * u->bwl) / u->b;

  if (u->gsda == 0.0)
       c->aocc = c->fai = c->aocg = c->aocp = c->aoc = c->apsl = c->bapsl
            = c->apslc = c->acg1 = c->acg2 = c->agsl = 0.0;

  else {
    if (u->ba > u->bai)
      u->ba = u->bai;

    c->hgla = 0.5 * (0.75 * u->b + u->gd);
    c->hgli = 0.5 * MAX(0.875 * u->b + u->gd, u->glai);

    c->agsl = u->gsda / (c->fa - c->fai - 0.2 * (u->bai - u->ba) + c->hgli - c->hgla);

    c->acg1 = c->agsl * (c->fa - c->hgla + 0.2 * u->ba);
    if (u->bhai > u->bha)
      u->vhai = u->bhai - u->bha + u->vha;
    c->lbgc = c->lbg + u->gd * u->gsda / (2.0 * (c->hgli - c->hgla));

    c->bapsl = (u->gd * u->bha == 0.0)
                ? 0.0
         : 0.4 * (0.9 * c->lbg + u->gd + u->y) /
                      (c->cmdi + c->mdi + 2.0 * (c->fa + u->vha - 2.0 * u->bha));

    c->apsl = MIN(u->gsda / (u->vhai - u->vha + c->fa - c->fai), 7.0);
    if (c->apsl < 0.0)
      c->apsl = 6.0;
    c->apslc = MAX(c->apsl, c->bapsl);
    c->apslc = MIN(c->apslc, 7.0);
    if (c->agsl - 0.8 * c->apslc < 0.0) {
      c->acg2 = 0.8 * c->apslc *
                 (0.5 * (c->fa + c->fai - c->hgla - c->hgli) + 0.1 * (u->ba + u->bai)) +
             0.5 * u->gsda;
      c->aocg = MIN(c->acg1, c->acg2);
    } else {
      c->acg2 = 0.0;
      c->aocg = c->acg1;
    }

    c->ccor[4] = (c->ihy < 1984) ? 0.0 : c->ccor[4] * 4.0;

    c->apslc = (c->fa - u->vha - c->ccor[4] - 0.018 * c->lbgc < 0.0) ? MIN(c->apslc, 6.0)
                                                        : MIN(c->apslc, 4.0);

    c->aocp = c->apslc * (c->fa - u->vha - c->ccor[4] - 0.018 * c->lbgc);

    c->ycor = (c->ihy > 1980) ? MAX(1.7 * u->vha - 0.5 * u->gd - u->y, 0.15 * c->lbg - u->y)
                        : MAX(2.0 * u->vha - 0.5 * u->gd - u->y, 0.2 * c->lbg - u->y);

    if (u->gd == 0)
      c->ycor = 0.0;

    c->ycor = MIN(c->ycor, 0.0);

    c->aoc = MIN(0.5 * (c->aocp + c->aocg), 0.05 * c->lbg + 0.95 * c->aocp);
    c->aoc = c->ycor + MIN(c->aoc, 0.05 * c->lbg + 0.95 * c->aocg);

    c->aocc = (c->aoc + u->y < 0.0) ? MIN(.6 * c->aoc, -u->y) : MIN(c->aoc, 1.25 * u->gsda);
  }

  c->l = c->lbg - c->foc - c->aocc;

  c->d = 1.3 * c->mdia + 0.9 * c->fdic + 0.055 * (3.0 * c->foc - c->aocc) +
       (c->l + CONSTANTS[0][u->munit]) / 30.0;

  c->bdr = pow((2.165 * pow(c->l *CONSTANTS[6][u->munit], 0.525) - CONSTANTS[19][u->munit]),
            0.375) /
       pow(c->l * u->b * c->mdia *CONSTANTS[6][u->munit], 0.125);

  c->dlf = MIN(1.1, 1 + 5.7 * pow((MAX(c->bdr, 1.0) - 1.0), 1.75));

  c->fb = 0.057 * c->l + CONSTANTS[1][u->munit];
  c->fm = 0.6 * c->ff + 0.4 * c->fa;
  c->fc = 0.25 * (c->fb - c->fm);

  c->db = 0.146 * c->l + CONSTANTS[2][u->munit];
  c->dspl = 0.5 * c->l * u->b * c->mdia *CONSTANTS[12][u->munit];

  if (u->cd == 0.0) {
    c->cbf = 1.0;
    c->rd = c->dm;
  } else {
    c->rd =
        pow(pow(u->cd, 1.6) + pow(MAX(c->dm - u->dmc - 0.3 * c->db, 0.0), 1.6), 0.625) +
        MIN(0.3 * c->db, c->dm);
    c->cbf = 0.95 + (c->l / (150.0 * (c->dm - u->dmc + c->cmdi)));
  }

  c->cbfa = c->cbf;
  c->cbf = MAX(c->cbf, 1.0);
  c->maf = 0.0075 * u->macl + 0.0125 * u->maco + 1.0;
  c->dd = c->rd - c->db;
  c->dc = 0.07 * c->l * (c->rd / c->db - 1.0);
}
