#include <math.h>
#include <stdio.h>
#include <time.h>

#include "ior.h"

static char *ptype[] = {"FIXED", "FOLDING", "FEATHERING"};
static char *units[] = {"FEET & POUNDS", "METRES & KILOGRAMS"};
static char *instype[] = {
    "OUT OF APT (FULL SIZE)", "OUT OF APT (SMALL)", "OTHER", "NONE", "OUTBOARD",
    "EXPOSED SHAFT",          "STRUT DRIVE"};

void ior_cert(char *cptr, ior_rec_t *u, calc_rec_t *c, char *lines[][2], int *pages) {
  int loop, jibs, spinnakers, line, n, nx;
  struct tm *tm;
  char tbuf[64];

  time_t tnow = time(NULL);

  tm = localtime(&tnow);

  if (c->rior < 23.0) {
    jibs = 3.0;
    spinnakers = 3.0;
  } else if (c->rior < 33.0) {
    jibs = 3.0;
    spinnakers = 4.0;
  } else if (c->rior < 40.0) {
    jibs = 4.0;
    spinnakers = 5.0;
  } else {
    jibs = 5.0;
    spinnakers = 6.0;
  }

  if (u->isp > u->p + u->bas)
    jibs = jibs + 1;

  line = 0;
  lines[line++][0] = cptr;

  n = sprintf(
       cptr, " IOR AMENDED TO JANUARY 1988       CERTIFICATE NO. %-15dPAGE 1\n",
       u->icert);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " ***************************       MEASUREMENTS: %s\n",
              units[u->munit]);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " * %-24s*       CLASS    : %-18sSTD: %d\n", u->yacht, u->class,
              u->istd);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " * %-24s*       DESIGNER : %s\n", u->sail, u->designer);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " * RATING %7.2f  FEET    *       BUILDER  : %s\n", c->ra,
              u->builder);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " ***************************       RIG      : %s\n", u->rig);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " I CERTIFY THAT I UNDERSTAND       KEEL     : %s\n", u->keel);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " MY RESPONSIBILITIES AS            PROPELLER: %s\n",
              ptype[u->ipt - 1]);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " COVERED IN THE IOR                INSTALLED: %s\n",
              instype[c->ecod - 1]);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " SIGNED :%62s\n", "------------------------------------");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "%52s\n", "REVAL AUTHORITY:-");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "%68s\n", "NOT VALID AFTER: 31 DECEMBER 1988");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "%51s\n", "ISSUED:");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "      %-42sBY:-\n", u->owner);
  cptr += ++n;
  lines[line++][0] = cptr;

  for (loop = 0; loop < 3; loop++) {
       n = sprintf(cptr, "      %-28s\n", u->address[loop]);
    cptr += ++n;
    lines[line++][0] = cptr;
  }

  if (!u->ifyr) {
    n = sprintf(cptr, " --------WEIGHTS/LOCATIONS FROM STEM------\n");
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, "    ANCHORS       BALLAST       BATTERY\n");
    cptr += ++n;
    lines[line++][0] = cptr;

    for (loop = 0; loop < 3; loop++) {
      n = sprintf(cptr, "    %-12s  %-12s  %-12s\n", u->anchor[loop],
                  u->ballast[loop], u->battery[loop]);
      cptr += ++n;
      lines[line++][0] = cptr;
    }
  } else {
    n = sprintf(cptr, " ------------------------------------------\n");
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, " MEASUREMENT INVENTORY TO FORM PAGE THREE\n");
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, " FLOATATION DATE %3d/%2d/%4d\n", u->ifda, u->ifmo, u->ifyr);
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, " SPECIFIC GRAVITY %5.3f\n", u->sg);
    cptr += ++n;
    lines[line++][0] = cptr;
  }

  n = sprintf(cptr, " ---COMMENTS------------------------------------------------"
                  "DATES-----\n");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " %61s%3d/%4d\n", "MOD HULL", u->hmo, u->hyr);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " %-36s%17cSERIES%5d/%4d\n", u->comment[0], ' ', u->smo, u->syr);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " %-36s%17cRIG%8d/%4d\n", u->comment[1], ' ', u->irgm, u->irgy);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, " %-36s%17cAGE%8d/%4d\n", u->comment[2], ' ', u->mon, u->yr);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(
      cptr,
      "  --HULL AND APPENDAGES ----  SGFI %7.3f  --------------------------\n",
      c->sgfi);

  if (u->ifyr > 1987)
    sprintf(cptr + 54, "W----SPD----PPD-\n");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  LOA %8.3f  BMAX %7.3f  FF %9.3f  AW %9.3f  APD %8.3f\n",
              u->loa, u->bmax, c->ff, u->aw, u->apd);

  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (1) %8.3f%7.3f%7.3f\n", u->wts[0], u->pdef[0][0],
            u->pdef[1][0]);

  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  FGO %8.3f  B %10.3f  FFI %8.3f  BW %9.3f  BPD %8.3f\n",
              u->fgo, u->b, c->ffi, u->bw, u->bpd);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (2) %8.3f%7.3f%7.3f\n", u->wts[1], u->pdef[0][1],
            u->pdef[1][1]);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  AGO %8.3f  BWL %8.3f  FFD %8.3f  CW %9.3f  CPD %8.3f\n",
              u->ago, u->bwl, c->ffd, u->cw, u->cpd);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (3) %8.3f%7.3f%7.3f\n", u->wts[2], u->pdef[0][2],
            u->pdef[1][2]);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  LBG %8.3f  BF %9.3f  FMD %8.3f  DW %9.3f  DPD %8.3f\n",
              c->lbg, u->bf, c->fmd, u->dw, u->dpd);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (4) %8.3f%7.3f%7.3f\n", u->wts[3], u->pdef[0][3],
            u->pdef[1][3]);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  GSDA %7.3f  BFI %8.3f  FBM %8.3f  AWD %8.3f  PL %9.3f\n",
              u->gsda, u->bfi, u->fbm, u->awd, u->pl);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (5) %8.3f%7.3f%7.3f\n", u->wts[4], u->pdef[0][4],
            u->pdef[1][4]);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr,
              "  GLAI %7.3f  BAI %8.3f  FAI %8.3f  BWD %8.3f  SBMAX %6.3f\n",
              u->glai, u->bai, c->fai, u->bwd, u->sbmax);
  if (u->ifyr > 1987) {
    nx = sprintf(cptr + 42, "  SWD %8.3f", u->swd);
    *(cptr + 42 + nx) = ' ';
  }
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  GSDF %7.3f  BA %9.3f  FA %9.3f  CWD %8.3f  SPD %8.3f\n",
              u->gsdf, u->ba, c->fa, u->cwd, u->spd);
  if (u->ifyr > 1987) {
    nx = sprintf(cptr + 42, "  PWD %8.3f", u->pwd);
    *(cptr + 42 + nx) = ' ';
  }
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  FD %9.3f  GD %9.3f  VHAI %7.3f  DWD %8.3f  SDM %8.3f\n",
              u->fd, u->gd, u->vhai, u->dwd, u->sdm);
  if (u->ifyr > 1987) {
    nx = sprintf(cptr + 42, "  PL %9.0f", u->pl);
    *(cptr + 42 + nx) = ' ';
  }
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  CMD %8.3f  Y %10.3f  VHA %8.3f  MAW1 %7.3f  MAW2 %7.3f\n",
              u->cmd, u->y, u->vha, u->raw1, u->raw2);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr,
              "  MD %9.3f  GDFI %7.3f  BHAI %7.3f  MACG1 %6.3f  MACG2 %6.3f\n",
              u->md, u->gdfi, u->bhai, u->racg1, u->racg2);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  OMD %8.3f  DM %9.3f  BHA %8.3f  MACL %7.3f  MACO %7.3f\n",
              u->omd, c->dm, u->bha, u->macl, u->maco);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  WLH1 %7.3f  DMT %8.3f  PDT %8.3f  PD %9.3f  ESD %8.3f\n",
              u->wlh1, u->dmt, u->pdt, c->pd, c->esd);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr,
              "  TWL1 %7.3f  TWL2 %7.3f  TWL3 %7.3f  TWL4 %7.3f  TWL5 %7.3f\n",
              u->twl[0], u->twl[1], u->twl[2], u->twl[3], u->twl[4]);
  cptr += ++n;
  lines[line++][0] = cptr;

  switch (c->ecod)

  {
  case 1:
  case 2:
    n = sprintf(cptr, "  EW %9.3f  EWD %8.3f %27c  PRD %8.3f\n", u->ew, u->ewd, ' ',
                u->prd);
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, "  APH %8.3f  APB %8.3f  APT %8.3f %13c  PBW %8.3f\n",
                u->aph, u->apb, u->apt, ' ', u->pbw);
    cptr += ++n;
    lines[line++][0] = cptr;

    break;

  case 3:
  case 4:
  case 6:
    n = sprintf(cptr, "  EW %9.3f  EWD %8.3f  ESL %8.3f  ESC %8.3f  PRD %8.3f\n",
                u->ew, u->ewd, u->esl, u->esc, u->prd);
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, "  PSD %8.3f  ST1 %8.3f  ST2 %8.3f  ST3 %8.3f  PBW %8.3f\n",
                u->psd, u->st1, u->st2, u->st3, u->pbw);
    cptr += ++n;
    lines[line++][0] = cptr;

    break;

  case 5:

    n = sprintf(cptr, "  EW %9.3f  EWD %8.3f\n", u->ew, u->ewd);
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, "  OUTBOARD MOTOR\n");
    cptr += ++n;
    lines[line++][0] = cptr;

    break;

  case 7:

    n = sprintf(cptr, "  EW %9.3f  EWD %8.3f  EDL %8.3f  EDC %8.3f  PRD %8.3f\n",
                u->ew, u->ewd, u->apt, u->apb, u->prd);
    cptr += ++n;
    lines[line++][0] = cptr;

    n = sprintf(cptr, "%14c  ST1 %8.3f  ST2 %8.3f  ST3 %8.3f  PBW %8.3f\n", ' ',
                u->st1, u->st2, u->st3, u->pbw);
    cptr += ++n;
    lines[line++][0] = cptr;
    break;
  }

  n = sprintf(cptr, "  CD %9.3f  WCBA%8.3f  CBBA%8.3f  CBLDA%7.3f  PHD %8.3f\n",
              u->cd, u->wcba, u->cbda, u->cblda, u->phd);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  DMC %8.3f  WCBB%8.3f  CBDB%8.3f  CBLDB%7.3f\n", u->dmc,
              u->wcbb, u->cbdb, u->cbldb);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(
      cptr,
      "  "
      "------FORESAIL-----------------MAINSAIL-----------------------------\n");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  I %10.3f  SPL %8.3f  P %10.3f  BL1 %8.3f  PC %9.3f\n", c->i,
              u->spl, u->p, u->bl1, c->pc);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  J %10.3f  SL %9.3f  E %10.3f  BL2 %8.3f  IC %9.3f\n", u->j,
              u->sl, u->e, u->bl2, c->ic);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  LPG %8.3f  SMW %8.3f  BAL %8.3f  BL3 %8.3f  JC %9.3f\n",
              u->lpg, u->smw, u->bal, u->bl3, c->jc);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  LPIS %7.3f  HBS %8.3f  BD %9.3f  BL4 %8.3f  EC %9.3f\n",
              u->lpis, u->hbs, u->bd, u->bl4, c->ec);
  cptr += ++n;
  lines[line++][0] = cptr;

  /*
    if (u->isp > 0.0)
     {
  */
  n = sprintf(cptr, "  FSP %8.3f  SPS %8.3f  BAS %8.3f  BL5 %8.3f  MXSL %7.3f\n",
              u->fsp, u->sps, u->bas, u->bl5, c->slm);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  FJ %9.3f  SFJ %8.3f  HB %9.3f  BLP %8.3f  MXSMW%7.3f\n",
              u->fs, u->sfs, u->hb, u->blp, c->smwm);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  FBI %8.3f  ISP %8.3f  MW %9.3f  MGU %8.3f  MXLPG%7.3f\n",
              u->fbi, u->isp, u->mw, u->mgu, c->lp - u->fsp);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  IG %9.3f  TCI %8.3f  GO %9.3f  MGM %8.3f  MXJL %7.3f\n",
              u->ig, c->tci, u->go, u->mgm, c->ajlmx);
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(
      cptr,
      "  "
      "--------------------------------------------------------------------\n");
  cptr += ++n;
  lines[line++][0] = cptr;

  n = sprintf(cptr, "  MEASURED ON:- %2d/%2d/%4d BY :- %s\n", u->idaa, u->idbb,
              u->idcc, u->measurer);
  cptr += ++n;

  lines[line][0] = NULL;
  *pages = line;

  line = 0;

  lines[line++][1] = cptr;
  n = sprintf(cptr, "%8cCERTIFICATE NO. %-12d %s\n", ' ', u->icert, u->yacht);
  cptr += ++n;
  lines[line++][1] = cptr;
  n = sprintf(cptr, "%8cPAGE 2%23c%s\n", ' ', ' ', u->sail);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8c----FREEBOARDS----------------------RM------PROP & CB-\n",
              ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFJS %8.3f  DMS %8.3f  ARM %8.3f  PF %9.3f\n", ' ', u->fss,
              u->dms, c->arm, c->pf);

  if (u->ifyr > 1987) {
    nx = sprintf(cptr + 34, "  SSLP%8.4f", c->slp[0]);
    *(cptr + 34 + nx) = ' ';
  }
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFFS %8.3f  PDS %8.3f  BRM %8.3f  PS %9.3f\n", ' ', u->ffs,
              u->pds, c->brm, c->ps);
  if (u->ifyr > 1987) {
    nx = sprintf(cptr + 34, "  PSLP%8.4f", c->slp[1]);
    *(cptr + 34 + nx) = ' ';
  }
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFFIS %7.3f  ESDS %7.3f  CRM %8.3f  DF %9.3f\n", ' ',
              u->ffis, u->esds, c->crm, c->df);
  if (u->ifyr > 1987) {
    nx = sprintf(cptr + 34, "  SCOR%8.4f", c->cor[0]);
    *(cptr + 34 + nx) = ' ';
  }
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFFDS %7.3f  MDIAS%7.3f  DRM %8.3f  EMF %8.3f\n", ' ',
              u->ffds, u->mdias, c->drm, c->emf);
  if (u->ifyr > 1987) {
    nx = sprintf(cptr + 34, "  PCOR%8.4f", c->cor[1]);
    *(cptr + 34 + nx) = ' ';
  }
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFBIS %7.3f  BBS %8.3f  RM %9.3f  PRDC %7.3f\n", ' ',
              u->fbis, u->bbs, c->rm, c->prdc);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFBMS %7.3f  BCS %8.3f  RMC %8.3f  PDC %8.3f\n", ' ',
              u->fbms, u->bsc, c->rmc, c->pdc);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFMDS %7.3f  CFFI %7.3f  TR %9.3f  RD %9.3f\n", ' ',
              u->fmds, c->cffi, c->tr, c->rd);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFAIS %7.3f  FFM %8.3f  SV %9.3f\n", ' ', u->fais, u->ffm,
              c->sv);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFAS %8.3f  FAM %8.3f  W %10.3f\n", ' ', u->fas, u->fam, c->w);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8c---L/FC-----------------------D/DC---------LIMITS-----\n",
              ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFOC %8.3f  AGSL %7.3f  FDI %8.3f  BL1 %8.3f\n", ' ', c->foc,
              c->agsl, c->fdi, c->xbl1);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cAOC %8.3f  APSL %7.3f  FDIC %7.3f  BL2 %8.3f\n", ' ', c->aoc,
              c->apsl, c->fdic, c->xbl2);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cAOCC %7.3f  BAPSL%7.3f  CMDI %7.3f  BL3 %8.3f\n", ' ',
              c->aocc, c->bapsl, c->cmdi, c->xbl3);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cAOCP %7.3f  APSLC%7.3f  MDI %8.3f  BL4 %8.3f\n", ' ',
              c->aocp, c->apslc, c->mdi, c->xbl4);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cAOCG %7.3f  ACG1 %7.3f  OMDI %7.3f  BLP %8.3f\n", ' ',
              c->aocg, c->acg1, c->omdi, c->xblp);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cYCOR %7.3f  ACG2 %7.3f  MDIA %7.3f  HB %9.3f\n", ' ',
              c->ycor, c->acg2, c->mdia, c->xhb);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFB %9.3f  HGLA %7.3f  DB %9.3f  MGU %8.3f\n", ' ', c->fb,
              c->hgla, c->db, c->xmgu);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cFM %9.3f  HGLI %7.3f  DD %9.3f  MGM %8.3f\n", ' ', c->fm,
              c->hgli, c->dd, c->xmgm);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cCCAI %7.3f  BDR %8.3f  DSPL %7.0f\n", ' ', c->ccor[4], c->bdr,
              c->dspl);
  cptr += ++n;
  lines[line++][1] = cptr;

  if (u->idc[0] || u->idc[1] || u->idc[2] || u->idc[3]) {
    n = sprintf(cptr, "%8cCCF %8.3f  CCO %8.3f  CCM %8.3f  CCC %8.3f\n", ' ',
                c->ccor[0], c->ccor[1], c->ccor[2], c->ccor[3]);
    cptr += ++n;
    lines[line++][1] = cptr;
  } else {
    n = sprintf(cptr, "\n");
    cptr += ++n;
    lines[line++][1] = cptr;
  }

  n = sprintf(cptr, "%8c--FORE/MAIN------PENALTY----MIZZEN & PEN----SCH & PEN-\n",
              ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cLP %9.2f  SL %9.3f  RSAY %7.2f  RSAB %7.2f\n", ' ', c->lp,
              c->plf, c->rsay, c->rsab);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cRSAF%8.2f  SPS %8.3f  RSAK %7.2f  RSAG %7.2f\n", ' ',
              c->rsaf, c->psh, c->rsak, c->rsag);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cSAM %8.2f  HBS %8.3f  YSAC %7.2f\n", ' ', c->rsama, c->shbs,
              c->ysac);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cMSAM%8.2f  HB %9.3f  RSAC %7.2f  HBF %8.2f\n", ' ', c->smin,
              c->p2, c->rsac, c->fp2);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cRSAM%8.2f  BD %9.3f%14c  BDF %8.2f\n", ' ', c->rsam, c->p1, ' ',
              c->fp1);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cRGF %8.4f  BAS %8.3f  HBY %8.3f  BADS %7.2f\n", ' ', c->rgf,
              c->p9, c->y2, c->fp9);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cSATC%8.2f  I/P %8.3f  BDY %8.3f  I/PSF%7.2f\n", ' ', c->satc,
              c->p10, c->y1, c->fp10);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cRSAT%8.2f  BLP %8.3f  BADY %7.3f  BLPS %7.2f\n", ' ',
              c->rsat, c->p3, c->y9, c->fp3);
  cptr += ++n;
  lines[line++][1] = cptr;

  // FIXME last 2 params?
  n = sprintf(cptr, "%8cRSAL%8.2f  BTNS %7.3f  I/PY %7.3f  BTNSF%7.2f\n", ' ',
              c->bpen, c->y10, c->bfpen, 0.0);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cSPIN%8.2f  MG %9.3f  BLPY %7.3f\n", ' ', c->spin, c->gpen, c->y3);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cSHR %8.4f%14c  BTNY %7.3f\n", ' ', c->shr, ' ', c->bypen);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cS %10.4f  SCF %8.4f\n", ' ', c->s, c->scf);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8c--RATING VALUES---------------------------III A-------\n",
              ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cL %10.4f  SC %9.4f  MAF %8.4f  SCA %8.4f\n", ' ', c->l, c->sc,
              c->maf, c->sca);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cB %10.4f  DC %9.4f  DLF %8.4f  DLFA %7.4f\n", ' ', u->b,
              c->dc, c->dlf, c->dlfa);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cD %10.4f  FC %9.4f  LRP %8.4f  CBFA %7.4f\n", ' ', c->d, c->fc,
              c->lrp, c->cbfa);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cCGF %8.4f  CBF %8.4f  MR %9.4f  CGFA %7.4f\n", ' ', c->cgf,
              c->cbf, c->mr, c->cgfa);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cEPF %8.4f  SMF %8.4f  R %10.4f  MRA%c %7.4f\n", ' ', c->epf,
              c->smf, c->r, (c->mra1 < c->mra2) ? '1' : '2', c->mra);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cTPF %8.4f  CSF %8.4f%14c  RA %9.4f\n", ' ', c->tpf, c->csf, ' ',
              c->ra);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cTMF %6.4f -------------------------------------------\n",
              ' ', c->tmf);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cSAIL LIMITS R = %4.1f  JIBS %3d  SPINNAKERS %3d\n", ' ',
              c->ra, jibs, spinnakers);
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%8cMAXIMUM CREW NUMBER LIMIT = %3d  WEIGHT = %6d\n", ' ',
              c->crewno, c->crewwt);
  cptr += ++n;
  lines[line++][1] = cptr;

  if (c->sv > -0.25) {
    n = sprintf(cptr, "%8cWARNING:  ABOVE VALUE OF SV IS UNSUITABLE FOR RACING\n",
                ' ');
    cptr += ++n;
    lines[line++][1] = cptr;

    n = sprintf(cptr, "%8cUNDER  SPEC. REG. CATAGORIES 0,1,2 & 3. (IOR 1203.2)\n",
                ' ');
    cptr += ++n;
    lines[line++][1] = cptr;
  } else {
    n = sprintf(cptr, "\n");
    cptr += ++n;
    lines[line++][1] = cptr;

    n = sprintf(cptr, "\n");
    cptr += ++n;
    lines[line++][1] = cptr;
  }

  n = sprintf(cptr, "\n");
  cptr += ++n;
  lines[line++][1] = cptr;

  strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S", tm);
  n = sprintf(cptr, "%34cGenerated %s\n", ' ', tbuf);

  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "\n");
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%34c\n", ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%34cjh+ior@daria.co.uk\n", ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%34chttps://github.com/stronnag/IORCalc\n", ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%34c*\n", ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%34c*\n", ' ');
  cptr += ++n;
  lines[line++][1] = cptr;

  n = sprintf(cptr, "%34c*\n", ' ');
  lines[line][1] = NULL;

  *(pages+1) = line;
}
