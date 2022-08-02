#include "ior.h"
#include <math.h>
#include <stdio.h>

#define INAPT 1
#define INAPTS 2
#define OTHER 3
#define NONE 4
#define OBOARD 5
#define EXPSFT 6
#define STRUT 7

void calc_prop(ior_rec_t *u, calc_rec_t *c) {
    double pfc, z, factor, zpf;
    int ipt;

    c->prdc = u->prd;
    c->pdc = c->pd;
    c->epf = c->pf = c->emf = c->prdc = c->df = c->ps = 0;

    while (1) {
        if (u->ew == 0.0) {
            u->ipt = c->ecod = NONE;
            break;
        }

        c->em = u->ew * u->ewd;
        c->emf = CONSTANTS[7][u->munit] * c->em / (c->l * c->l * u->b * c->d);

        if (u->prd == 0.0) {
            c->ecod = OBOARD;
            u->ipt = NONE;
            c->emf = fmax(c->emf, 0.002);
            break;
        }

        if (c->ihy > 1974)
            c->pdc = fmin(c->pdc, c->cmdi + 0.75 * u->prd);

        c->ps = fmin(u->prd, 4.0 * u->pbw);

        if ((u->ipt == 2 || u->ipt == 3) && u->phd != 0.0)
            c->ps = fmin(u->prd, 10.0 * u->phd);

        if (u->aph != 0.0) {
            c->ecod =
                (u->aph < u->prd * 1.125 || fmin(u->apt, u->apb) < 0.4 * u->prd)
                    ? INAPTS
                    : INAPT;
            break;
        }

        if (u->apt != 0.0) {
            c->ecod = STRUT;
            c->prdc = fmin(
                c->ps, fmin(u->st1 / 0.08, fmin(u->st2 / 0.3, u->apt / 1.5)));
            if (u->st3 > 0.75 * c->prdc || c->prdc == 0.0)
                c->ecod = OTHER;
            c->pdc = c->pdc - fmax(0.0, u->apb - 0.75 * c->prdc);
            break;
        }

        c->ecod = EXPSFT;
        c->prdc =
            fmin(c->ps, fmin(u->st1 / 0.05, fmin(u->st2 / 0.2, u->esl / 1.5)));
        if (c->ihy > 1987)
            c->prdc = fmin(c->prdc, 16.0 * u->psd);
        if (u->st3 > 0.75 * c->prdc || c->prdc == 0.0)
            c->ecod = OTHER;
        c->pdc =
            c->pdc -
            fmax((u->esc - 0.75 * c->prdc) * u->esl / (1.5 * c->prdc), 0.0);
        break;
    }

    if (u->prd > 0.0) {
        if (c->ecod == OTHER)
            c->prdc = u->prd;

        ipt = u->ipt;
        if (u->phd > 0.0 && u->ipt == 2 && c->ecod > 2)
            ipt = 3;
        z = (ipt == 1) ? 1.0 : 0.5;
        pfc = 0.0;

        if (c->ihy > 1978) {
            if (c->ihy < 1986) {
                zpf = c->cmdi + 0.375 * u->prd;
                factor = 0.375;
            } else {
                zpf = 1.3 * c->cmdi - 0.37 * c->mdi + 0.07 * c->omdi;
                factor = 0.3;
            }

            if (c->esd > zpf)
                pfc = z * fmin(1.0, (c->esd - zpf) / (factor * u->prd));
        }

        c->pf = PFACTOR[ipt - 1][c->ecod - 1] - pfc;

        c->df =
            (c->ps < 0.03 * c->l)
                ? c->pf * sqrt(c->pdc / c->db) *
                      (fmax(c->prdc * c->prdc * 60.0 / (c->l * c->l), 0.024) -
                       0.024)
                : 1.25 * c->pf * sqrt(c->pdc / c->db) *
                      fmin(c->prdc / c->l, 0.04);
    }
    c->epf = fmax(0.96, 1.0 - c->emf - c->df);
}
