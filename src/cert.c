#include "iorext.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define P1LEN 72
#define P2LEN 64

static char *ptype[] = {"FIXED", "FOLDING", "FEATHERING"};
static char *units[] = {"FEET & POUNDS", "METRES & KILOGRAMS"};
static char *instype[] = {"OUT OF APT (FULL SIZE)", "OUT OF APT (SMALL)", "OTHER", "NONE", "OUTBOARD",
                          "EXPOSED SHAFT",          "STRUT DRIVE"};

void ior_cert(ior_rec_t *u, calc_rec_t *c, cert_pages_t *cp) {
  int loop, jibs, spinnakers, line;
  struct tm *tm;
  char tbuf[64];
  char *cptr;

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
  asprintf(&cptr, " IOR AMENDED TO JANUARY 1988       CERTIFICATE NO. %-13dPAGE 1", u->icert);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " ***************************       MEASUREMENTS: %s", units[u->munit]);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " * %-24s*       CLASS    : %-18sSTD: %d", u->yacht, u->class, u->istd);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " * %-24s*       DESIGNER : %s", u->sail, u->designer);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " * RATING %7.2f  FEET    *       BUILDER  : %s", c->ra, u->builder);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " ***************************       RIG      : %s", u->rig);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " I CERTIFY THAT I UNDERSTAND       KEEL     : %s", u->keel);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " MY RESPONSIBILITIES AS            PROPELLER: %s", ptype[u->ipt - 1]);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " COVERED IN THE IOR                INSTALLED: %s", instype[c->ecod - 1]);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " SIGNED :%61s", "------------------------------------");
  cp->page1[line++] = cptr;

  asprintf(&cptr, "%52s", "REVAL AUTHORITY:-");
  cp->page1[line++] = cptr;

  asprintf(&cptr, "%68s", "NOT VALID AFTER: 31 DECEMBER 1988");
  cp->page1[line++] = cptr;

  asprintf(&cptr, "%51s", "ISSUED:");
  cp->page1[line++] = cptr;

  asprintf(&cptr, "      %-42sBY:-", u->owner);
  cp->page1[line++] = cptr;

  for (loop = 0; loop < 3; loop++) {
    asprintf(&cptr, "      %-28s", u->address[loop]);
    cp->page1[line++] = cptr;
  }

  if (!u->ifyr) {
    asprintf(&cptr, " --------WEIGHTS/LOCATIONS FROM STEM------");
    cp->page1[line++] = cptr;

    asprintf(&cptr, "    ANCHORS       BALLAST       BATTERY");
    cp->page1[line++] = cptr;

    for (loop = 0; loop < 3; loop++) {
      asprintf(&cptr, "    %-12s  %-12s  %-12s", u->anchor[loop], u->ballast[loop], u->battery[loop]);
      cp->page1[line++] = cptr;
    }
  } else {
    asprintf(&cptr, " ------------------------------------------");
    cp->page1[line++] = cptr;

    asprintf(&cptr, " MEASUREMENT INVENTORY TO FORM PAGE THREE");
    cp->page1[line++] = cptr;

    asprintf(&cptr, " FLOATATION DATE %3d/%2d/%4d", u->ifda, u->ifmo, u->ifyr);
    cp->page1[line++] = cptr;

    asprintf(&cptr, " SPECIFIC GRAVITY %5.3f", u->sg);
    cp->page1[line++] = cptr;
  }

  asprintf(&cptr, " ---COMMENTS----------------------------------------------"
                  "--DATES-----");
  cp->page1[line++] = cptr;

  asprintf(&cptr, " %61s%3d/%4d", "MOD HULL", u->hmo, u->hyr);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " %-36s%17cSERIES%5d/%4d", u->comment[0], ' ', u->smo, u->syr);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " %-36s%17cRIG%8d/%4d", u->comment[1], ' ', u->irgm, u->irgy);
  cp->page1[line++] = cptr;

  asprintf(&cptr, " %-36s%17cAGE%8d/%4d", u->comment[2], ' ', u->mon, u->yr);
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  --HULL AND APPENDAGES ----  SGFI %7.3f  --------------------------", c->sgfi);
  if (u->ifyr > 1987)
    strcpy(cptr + 54, "W----SPD----PPD-");
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  LOA %8.3f  BMAX %7.3f  FF %9.3f  AW %9.3f  APD %8.3f", u->loa, u->bmax, c->ff, u->aw, u->apd);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (1) %8.3f%7.3f%7.3f", u->wts[0], u->pdef[0][0], u->pdef[1][0]);
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  FGO %8.3f  B %10.3f  FFI %8.3f  BW %9.3f  BPD %8.3f", u->fgo, u->b, c->ffi, u->bw, u->bpd);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (2) %8.3f%7.3f%7.3f", u->wts[1], u->pdef[0][1], u->pdef[1][1]);
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  AGO %8.3f  BWL %8.3f  FFD %8.3f  CW %9.3f  CPD %8.3f", u->ago, u->bwl, c->ffd, u->cw, u->cpd);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (3) %8.3f%7.3f%7.3f", u->wts[2], u->pdef[0][2], u->pdef[1][2]);
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  LBG %8.3f  BF %9.3f  FMD %8.3f  DW %9.3f  DPD %8.3f", c->lbg, u->bf, c->fmd, u->dw, u->dpd);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (4) %8.3f%7.3f%7.3f", u->wts[3], u->pdef[0][3], u->pdef[1][3]);
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  GSDA %7.3f  BFI %8.3f  FBM %8.3f  AWD %8.3f  PL %9.3f", u->gsda, u->bfi, u->fbm, u->awd, u->pl);
  if (u->ifyr > 1987)
    sprintf(cptr + 42, "  (5) %8.3f%7.3f%7.3f", u->wts[4], u->pdef[0][4], u->pdef[1][4]);
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  GLAI %7.3f  BAI %8.3f  FAI %8.3f  BWD %8.3f  SBMAX %6.3f", u->glai, u->bai, c->fai, u->bwd, u->sbmax);
  if (u->ifyr > 1987) {
    sprintf(cptr + 42, "  SWD %8.3f", u->swd);
  }
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  GSDF %7.3f  BA %9.3f  FA %9.3f  CWD %8.3f  SPD %8.3f", u->gsdf, u->ba, c->fa, u->cwd, u->spd);
  if (u->ifyr > 1987) {
    sprintf(cptr + 42, "  PWD %8.3f", u->pwd);
  }
  cp->page1[line++] = cptr;

  cptr = malloc(P1LEN);
  sprintf(cptr, "  FD %9.3f  GD %9.3f  VHAI %7.3f  DWD %8.3f  SDM %8.3f", u->fd, u->gd, u->vhai, u->dwd, u->sdm);
  if (u->ifyr > 1987) {
    sprintf(cptr + 42, "  PL %9.0f", u->pl);
  }
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  CMD %8.3f  Y %10.3f  VHA %8.3f  MAW1 %7.3f  MAW2 %7.3f", u->cmd, u->y, u->vha, u->raw1, u->raw2);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  MD %9.3f  GDFI %7.3f  BHAI %7.3f  MACG1 %6.3f  MACG2 %6.3f", u->md, u->gdfi, u->bhai, u->racg1, u->racg2);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  OMD %8.3f  DM %9.3f  BHA %8.3f  MACL %7.3f  MACO %7.3f", u->omd, c->dm, u->bha, u->macl, u->maco);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  WLH1 %7.3f  DMT %8.3f  PDT %8.3f  PD %9.3f  ESD %8.3f", u->wlh1, u->dmt, u->pdt, c->pd, c->esd);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  TWL1 %7.3f  TWL2 %7.3f  TWL3 %7.3f  TWL4 %7.3f  TWL5 %7.3f", u->twl[0], u->twl[1], u->twl[2], u->twl[3],
           u->twl[4]);
  cp->page1[line++] = cptr;

  switch (c->ecod) {
  case 1:
  case 2:
    asprintf(&cptr, "  EW %9.3f  EWD %8.3f %27c  PRD %8.3f", u->ew, u->ewd, ' ', u->prd);
    cp->page1[line++] = cptr;
    asprintf(&cptr, "  APH %8.3f  APB %8.3f  APT %8.3f %13c  PBW %8.3f", u->aph, u->apb, u->apt, ' ', u->pbw);
    cp->page1[line++] = cptr;
    break;

  case 3:
  case 4:
  case 6:
    asprintf(&cptr, "  EW %9.3f  EWD %8.3f  ESL %8.3f  ESC %8.3f  PRD %8.3f", u->ew, u->ewd, u->esl, u->esc, u->prd);
    cp->page1[line++] = cptr;
    asprintf(&cptr, "  PSD %8.3f  ST1 %8.3f  ST2 %8.3f  ST3 %8.3f  PBW %8.3f", u->psd, u->st1, u->st2, u->st3, u->pbw);
    cp->page1[line++] = cptr;
    break;

  case 5:
    asprintf(&cptr, "  EW %9.3f  EWD %8.3f", u->ew, u->ewd);
    cp->page1[line++] = cptr;
    asprintf(&cptr, "  OUTBOARD MOTOR");
    cp->page1[line++] = cptr;
    break;

  case 7:
    asprintf(&cptr, "  EW %9.3f  EWD %8.3f  EDL %8.3f  EDC %8.3f  PRD %8.3f", u->ew, u->ewd, u->apt, u->apb, u->prd);
    cp->page1[line++] = cptr;

    asprintf(&cptr, "%14c  ST1 %8.3f  ST2 %8.3f  ST3 %8.3f  PBW %8.3f", ' ', u->st1, u->st2, u->st3, u->pbw);
    cp->page1[line++] = cptr;
    break;
  }

  asprintf(&cptr, "  CD %9.3f  WCBA%8.3f  CBBA%8.3f  CBLDA%7.3f  PHD %8.3f", u->cd, u->wcba, u->cbda, u->cblda, u->phd);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  DMC %8.3f  WCBB%8.3f  CBDB%8.3f  CBLDB%7.3f", u->dmc, u->wcbb, u->cbdb, u->cbldb);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  "
                  "------FORESAIL-----------------MAINSAIL-----------------------------");
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  I %10.3f  SPL %8.3f  P %10.3f  BL1 %8.3f  PC %9.3f", c->i, u->spl, u->p, u->bl1, c->pc);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  J %10.3f  SL %9.3f  E %10.3f  BL2 %8.3f  IC %9.3f", u->j, u->sl, u->e, u->bl2, c->ic);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  LPG %8.3f  SMW %8.3f  BAL %8.3f  BL3 %8.3f  JC %9.3f", u->lpg, u->smw, u->bal, u->bl3, c->jc);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  LPIS %7.3f  HBS %8.3f  BD %9.3f  BL4 %8.3f  EC %9.3f", u->lpis, u->hbs, u->bd, u->bl4, c->ec);
  cp->page1[line++] = cptr;

  /*
    if (u->isp > 0.0)
     {
  */
  asprintf(&cptr, "  FSP %8.3f  SPS %8.3f  BAS %8.3f  BL5 %8.3f  MXSL %7.3f", u->fsp, u->sps, u->bas, u->bl5, c->slm);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  FJ %9.3f  SFJ %8.3f  HB %9.3f  BLP %8.3f  MXSMW%7.3f", u->fs, u->sfs, u->hb, u->blp, c->smwm);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  FBI %8.3f  ISP %8.3f  MW %9.3f  MGU %8.3f  MXLPG%7.3f", u->fbi, u->isp, u->mw, u->mgu, c->lp - u->fsp);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  IG %9.3f  TCI %8.3f  GO %9.3f  MGM %8.3f  MXJL %7.3f", u->ig, c->tci, u->go, u->mgm, c->ajlmx);
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  "
                  "--------------------------------------------------------------------");
  cp->page1[line++] = cptr;

  asprintf(&cptr, "  MEASURED ON:- %2d/%2d/%4d BY :- %s", u->idaa, u->idbb, u->idcc, u->measurer);
  cp->page1[line++] = cptr;
  cp->page1[line] = NULL;

  line = 0;

  asprintf(&cptr, "%8cCERTIFICATE NO. %-12d %s", ' ', u->icert, u->yacht);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cPAGE 2%23c%s", ' ', ' ', u->sail);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8c----FREEBOARDS----------------------RM------PROP & CB-", ' ');
  cp->page2[line++] = cptr;

  cptr = malloc(P2LEN);
  sprintf(cptr, "%8cFJS %8.3f  DMS %8.3f  ARM %8.3f  PF %9.3f", ' ', u->fss, u->dms, c->arm, c->pf);

  if (u->ifyr > 1987) {
    sprintf(cptr + 34, "  SSLP%8.4f", c->slp[0]);
  }
  cp->page2[line++] = cptr;

  cptr = malloc(P2LEN);
  sprintf(cptr, "%8cFFS %8.3f  PDS %8.3f  BRM %8.3f  PS %9.3f", ' ', u->ffs, u->pds, c->brm, c->ps);
  if (u->ifyr > 1987) {
    sprintf(cptr + 34, "  PSLP%8.4f", c->slp[1]);
  }
  cp->page2[line++] = cptr;

  cptr = malloc(P2LEN);
  sprintf(cptr, "%8cFFIS %7.3f  ESDS %7.3f  CRM %8.3f  DF %9.3f", ' ', u->ffis, u->esds, c->crm, c->df);
  if (u->ifyr > 1987) {
    sprintf(cptr + 34, "  SCOR%8.4f", c->cor[0]);
  }
  cp->page2[line++] = cptr;

  cptr = malloc(P2LEN);
  sprintf(cptr, "%8cFFDS %7.3f  MDIAS%7.3f  DRM %8.3f  EMF %8.3f", ' ', u->ffds, u->mdias, c->drm, c->emf);
  if (u->ifyr > 1987) {
    sprintf(cptr + 34, "  PCOR%8.4f", c->cor[1]);
  }
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFBIS %7.3f  BBS %8.3f  RM %9.3f  PRDC %7.3f", ' ', u->fbis, u->bbs, c->rm, c->prdc);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFBMS %7.3f  BCS %8.3f  RMC %8.3f  PDC %8.3f", ' ', u->fbms, u->bsc, c->rmc, c->pdc);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFMDS %7.3f  CFFI %7.3f  TR %9.3f  RD %9.3f", ' ', u->fmds, c->cffi, c->tr, c->rd);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFAIS %7.3f  FFM %8.3f  SV %9.3f", ' ', u->fais, u->ffm, c->sv);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFAS %8.3f  FAM %8.3f  W %10.3f", ' ', u->fas, u->fam, c->w);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8c---L/FC-----------------------D/DC---------LIMITS-----", ' ');
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFOC %8.3f  AGSL %7.3f  FDI %8.3f  BL1 %8.3f", ' ', c->foc, c->agsl, c->fdi, c->xbl1);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cAOC %8.3f  APSL %7.3f  FDIC %7.3f  BL2 %8.3f", ' ', c->aoc, c->apsl, c->fdic, c->xbl2);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cAOCC %7.3f  BAPSL%7.3f  CMDI %7.3f  BL3 %8.3f", ' ', c->aocc, c->bapsl, c->cmdi, c->xbl3);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cAOCP %7.3f  APSLC%7.3f  MDI %8.3f  BL4 %8.3f", ' ', c->aocp, c->apslc, c->mdi, c->xbl4);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cAOCG %7.3f  ACG1 %7.3f  OMDI %7.3f  BLP %8.3f", ' ', c->aocg, c->acg1, c->omdi, c->xblp);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cYCOR %7.3f  ACG2 %7.3f  MDIA %7.3f  HB %9.3f", ' ', c->ycor, c->acg2, c->mdia, c->xhb);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFB %9.3f  HGLA %7.3f  DB %9.3f  MGU %8.3f", ' ', c->fb, c->hgla, c->db, c->xmgu);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cFM %9.3f  HGLI %7.3f  DD %9.3f  MGM %8.3f", ' ', c->fm, c->hgli, c->dd, c->xmgm);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cCCAI %7.3f  BDR %8.3f  DSPL %7.0f", ' ', c->ccor[4], c->bdr, c->dspl);
  cp->page2[line++] = cptr;

  if (u->idc[0] || u->idc[1] || u->idc[2] || u->idc[3]) {
    asprintf(&cptr, "%8cCCF %8.3f  CCO %8.3f  CCM %8.3f  CCC %8.3f", ' ', c->ccor[0], c->ccor[1], c->ccor[2], c->ccor[3]);
  } else {
    cptr = calloc(1, 2);
  }
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8c--FORE/MAIN------PENALTY----MIZZEN & PEN----SCH & PEN-", ' ');
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cLP %9.2f  SL %9.3f  RSAY %7.2f  RSAB %7.2f", ' ', c->lp, c->plf, c->rsay, c->rsab);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cRSAF%8.2f  SPS %8.3f  RSAK %7.2f  RSAG %7.2f", ' ', c->rsaf, c->psh, c->rsak, c->rsag);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cSAM %8.2f  HBS %8.3f  YSAC %7.2f", ' ', c->rsama, c->shbs, c->ysac);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cMSAM%8.2f  HB %9.3f  RSAC %7.2f  HBF %8.2f", ' ', c->smin, c->p2, c->rsac, c->fp2);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cRSAM%8.2f  BD %9.3f%14c  BDF %8.2f", ' ', c->rsam, c->p1, ' ', c->fp1);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cRGF %8.4f  BAS %8.3f  HBY %8.3f  BADS %7.2f", ' ', c->rgf, c->p9, c->y2, c->fp9);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cSATC%8.2f  I/P %8.3f  BDY %8.3f  I/PSF%7.2f", ' ', c->satc, c->p10, c->y1, c->fp10);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cRSAT%8.2f  BLP %8.3f  BADY %7.3f  BLPS %7.2f", ' ', c->rsat, c->p3, c->y9, c->fp3);
  cp->page2[line++] = cptr;

  // FIXME last 2 params?
  asprintf(&cptr, "%8cRSAL%8.2f  BTNS %7.3f  I/PY %7.3f  BTNSF%7.2f", ' ', c->bpen, c->y10, c->bfpen, 0.0);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cSPIN%8.2f  MG %9.3f  BLPY %7.3f", ' ', c->spin, c->gpen, c->y3);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cSHR %8.4f%14c  BTNY %7.3f", ' ', c->shr, ' ', c->bypen);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cS %10.4f  SCF %8.4f", ' ', c->s, c->scf);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8c--RATING VALUES---------------------------III A-------", ' ');
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cL %10.4f  SC %9.4f  MAF %8.4f  SCA %8.4f", ' ', c->l, c->sc, c->maf, c->sca);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cB %10.4f  DC %9.4f  DLF %8.4f  DLFA %7.4f", ' ', u->b, c->dc, c->dlf, c->dlfa);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cD %10.4f  FC %9.4f  LRP %8.4f  CBFA %7.4f", ' ', c->d, c->fc, c->lrp, c->cbfa);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cCGF %8.4f  CBF %8.4f  MR %9.4f  CGFA %7.4f", ' ', c->cgf, c->cbf, c->mr, c->cgfa);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cEPF %8.4f  SMF %8.4f  R %10.4f  MRA%c %7.4f", ' ', c->epf, c->smf, c->r, (c->mra1 < c->mra2) ? '1' : '2',
           c->mra);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cTPF %8.4f  CSF %8.4f%14c  RA %9.4f", ' ', c->tpf, c->csf, ' ', c->ra);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cTMF %6.4f -------------------------------------------", ' ', c->tmf);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cSAIL LIMITS R = %4.1f  JIBS %3d  SPINNAKERS %3d", ' ', c->ra, jibs, spinnakers);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%8cMAXIMUM CREW NUMBER LIMIT = %3d  WEIGHT = %6d", ' ', c->crewno, c->crewwt);
  cp->page2[line++] = cptr;

  if (c->sv > -0.25) {
    asprintf(&cptr, "%8cWARNING:  ABOVE VALUE OF SV IS UNSUITABLE FOR RACING", ' ');
    cp->page2[line++] = cptr;

    asprintf(&cptr, "%8cUNDER  SPEC. REG. CATAGORIES 0,1,2 & 3. (IOR 1203.2)", ' ');
    cp->page2[line++] = cptr;
  } else {
    cptr = calloc(1, 2);
    cp->page2[line++] = cptr;
    cptr = calloc(1, 2);
    cp->page2[line++] = cptr;
  }
  cptr = calloc(1, 2);
  cp->page2[line++] = cptr;

  strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S", tm);
  asprintf(&cptr, "%33cGenerated %s", ' ', tbuf);
  cp->page2[line++] = cptr;

  cptr = calloc(1, 2);
  cp->page2[line++] = cptr;

  asprintf(&cptr, "%27chttps://github.com/stronnag/IORCalc", ' ');
  cp->page2[line++] = cptr;

  cp->page2[line] = NULL;
}
