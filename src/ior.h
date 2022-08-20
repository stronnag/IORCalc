#pragma once

/*
        ior_h   Header file for IOR program modules. This module
                defines fixed CONSTANTS and strings, the i/o record
                and external variables.
*/

typedef struct ior_rec {
  int idaa, idbb, idcc, hmo, hyr, smo, syr, irgm, irgy, mon, yr, munit, istd, ipt, ifda, ifmo, ifyr, cyr, idc[5], icert, mule,
      mpen;

  double bmax, bsc, bbs, b, wlh1, fgo, bf, gsdf, bfi, gdfi, ago, y, gd, ba, gsda, glai, bai, loa, sbmax, sdm, spd, fss, ffs,
      ffis, ffds, fbis, fmds, fbms, fais, fas, dms, esds, pds, fd, omd, md, cmd, dmt, pdt, bhai, bha, vha, vhai, wcba, cbda,
      cblda, wcbb, cbdb, cbldb, cd, raw1, raw2, racg1, racg2, macl, maco, prd, pbw, esl, esc, st1, st2, st3, aph, apt, apb, ew,
      ewd, psd, fs, fbi, fbm, ffm, fam, bwl, pl, sg, aw, awd, apd, bw, bwd, bpd, cw, cwd, cpd, dw, dwd, dpd, bas, ig, isp, mw,
      go, p, sps, spl, j, sfs, fsp, e, bal, bd, hb, blp, mgu, mgm, bl1, bl2, bl3, bl4, bl5, lpg, lpis, smw, sl, hbs, g, h, s1,
      s2, s3, eb, iy, is, bads, py, ey, baly, bdy, hby, blpy, by1, by2, by3, by4, by5, ysf, gy, hy, ysd, ysmg, badx, s6, ra,
      tmf, mdias, dmc, twl[5], phd, wts[5], pdef[2][5], swd, pwd;

  char yacht[24], sail[12], owner[24], measurer[24], address[3][28], comment[3][36], anchor[3][12], battery[3][12],
      ballast[3][12], class[18], designer[18], builder[18], rig[12], keel[12];
} ior_rec_t;

typedef struct ior_calc {
  int ihm, ihy, imo, iyr, ecod, crewwt, crewno, pcount;
  double lbg, sink, trim, atrim, ff, ffi, ffd, fmd, fai, fa, pd, dm, esd, fs, cffi, foc, ccor[5], fdic, fdi, mdi, omdi, cmdi,
      mdia, aocc, aocg, aocp, aoc, apsl, bapsl, apslc, acg1, acg2, agsl, hgla, hgli, lbgc, ycor, l, d, bdr, dlf, fb, fm, fc, db,
      dspl, cbf, rd, cbfa, maf, dd, dc, prdc, pdc, epf, pf, emf, df, ps, em, arm, brm, crm, drm, rm, rmc, tr, cgf, cgfm, cgfc,
      csf, bwl1, sv, tci, i, sph, psh, bad, jc, shbs, plf, tti, ic, lp, smwm, slm, ajlmx, rsaf, ec, rh, rhc, xbl1, xbl2, xbl3,
      xbl4, xbl5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, xhb, bpen, xmgu, xmgm, gpen, rgf, ttp, rsam, satm, hc, smin, rsama,
      satc, satca, rsat, rsata, ebc, ris, efc, fp1, fp2, fp3, fp4, fp5, fp6, fp7, fp8, fp9, fp10, bfpen, psfc, hfc, satf, eyc,
      y1, y2, y3, y4, y5, y6, y7, y8, y9, y10, bypen, pyc, hyc, spin, rsal, rsala, s, shr, sc, rsay, rsac, rsab, rsak, rsag,
      ysac, pc, sa, dlfa, cgfa, scfa, msar, shra, tpf, mr, mra, mra1, mra2, r, rior, ra, rtf, rtfa, w, tmf, xblp, scf, sca, lrp,
      smf, sgfi, slp[2], cor[2];
} calc_rec_t;

#define IOR_REC sizeof(ior_rec_t)

/*
extern int ior_amend(ior_rec_t *);
extern void report(ior_rec_t *u, calc_rec_t *c);
*/
