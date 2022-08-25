#include "iorext.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static edit_field_t editlist[] = {
    {"Yacht:", "yacht", offsetof(ior_rec_t, yacht), 24, 0, 0, EDIT_TYPE_ED_C},
    {"Sail No:", "sail", offsetof(ior_rec_t, sail), 12, 2, 0, EDIT_TYPE_ED_C},
    {"Cert:", "icert", offsetof(ior_rec_t, icert), 5, 4, 0, EDIT_TYPE_ED_I},
    {"Owner:", "owner", offsetof(ior_rec_t, owner), 24, 0, 1, EDIT_TYPE_ED_C},
    {"Class:", "class", offsetof(ior_rec_t, class), 18, 2, 1, EDIT_TYPE_ED_C},
    {"Std No:", "istd", offsetof(ior_rec_t, istd), 3, 4, 1, EDIT_TYPE_ED_I},
    {"Address:", "address0", offsetof(ior_rec_t, address[0]), 28, 0, 2, EDIT_TYPE_ED_C},
    {"Designer:", "designer", offsetof(ior_rec_t, designer), 18, 2, 2, EDIT_TYPE_ED_C},
    {"Age month:", "mon", offsetof(ior_rec_t, mon), 2, 4, 2, EDIT_TYPE_ED_I},
    {"Year", "yr", offsetof(ior_rec_t, yr), 4, 6, 2, EDIT_TYPE_ED_I},
    {" :", "address1", offsetof(ior_rec_t, address[1]), 28, 0, 3, EDIT_TYPE_ED_C},
    {"Builder:", "builder", offsetof(ior_rec_t, builder), 18, 2, 3, EDIT_TYPE_ED_C},
    {"Hull month:", "hmo", offsetof(ior_rec_t, hmo), 2, 4, 3, EDIT_TYPE_ED_I},
    {"Year", "hyr", offsetof(ior_rec_t, hyr), 4, 6, 3, EDIT_TYPE_ED_I},
    {" :", "address2", offsetof(ior_rec_t, address[2]), 28, 0, 4, EDIT_TYPE_ED_C},
    {"Keel:", "keel", offsetof(ior_rec_t, keel), 12, 2, 4, EDIT_TYPE_ED_C},
    {"Rig month:", "irgm", offsetof(ior_rec_t, irgm), 2, 4, 4, EDIT_TYPE_ED_I},
    {"Year", "irgy", offsetof(ior_rec_t, irgy), 4, 6, 4, EDIT_TYPE_ED_I},
    {"Measurer:", "measurer", offsetof(ior_rec_t, measurer), 24, 0, 6, EDIT_TYPE_ED_C},
    {"Rig:", "rig", offsetof(ior_rec_t, rig), 12, 2, 6, EDIT_TYPE_ED_C},
    {"Units:", "munit", offsetof(ior_rec_t, munit), 1, 4, 6, EDIT_TYPE_ED_I},
    {"Measure day:", "idaa", offsetof(ior_rec_t, idaa), 2, 0, 7, EDIT_TYPE_ED_I},
    {"Month", "idbb", offsetof(ior_rec_t, idbb), 2, 2, 7, EDIT_TYPE_ED_I},
    {"Year", "idcc", offsetof(ior_rec_t, idcc), 4, 4, 7, EDIT_TYPE_ED_I},
    {"Series month:", "smo", offsetof(ior_rec_t, smo), 2, 0, 8, EDIT_TYPE_ED_I},
    {"Year", "syr", offsetof(ior_rec_t, syr), 4, 2, 8, EDIT_TYPE_ED_I},
    {"Comment1:", "comment0", offsetof(ior_rec_t, comment[0]), 36, 0, 9, EDIT_TYPE_ED_C},
    {"Anchor1:", "anchor1", offsetof(ior_rec_t, anchor[0]), 12, 2, 9, EDIT_TYPE_ED_C},
    {"Ballast1:", "ballast1", offsetof(ior_rec_t, ballast[0]), 12, 4, 9, EDIT_TYPE_ED_C},
    {"Battery1:", "battery1", offsetof(ior_rec_t, battery[0]), 12, 6, 9, EDIT_TYPE_ED_C},
    {"Comment2:", "comment1", offsetof(ior_rec_t, comment[1]), 36, 0, 10, EDIT_TYPE_ED_C},
    {"Anchor2:", "anchor2", offsetof(ior_rec_t, anchor[1]), 12, 2, 10, EDIT_TYPE_ED_C},
    {"Ballast2:", "ballast2", offsetof(ior_rec_t, ballast[1]), 12, 4, 10, EDIT_TYPE_ED_C},
    {"Battery2:", "battery2", offsetof(ior_rec_t, battery[1]), 12, 4, 10, EDIT_TYPE_ED_C},
    {"Comment3:", "comment2", offsetof(ior_rec_t, comment[2]), 36, 0, 11, EDIT_TYPE_ED_C},
    {"Anchor2:", "anchor2", offsetof(ior_rec_t, anchor[2]), 12, 2, 11, EDIT_TYPE_ED_C},
    {"Ballast3:", "ballast3", offsetof(ior_rec_t, ballast[2]), 12, 4, 11, EDIT_TYPE_ED_C},
    {"Battery3:", "battery3", offsetof(ior_rec_t, battery[2]), 12, 4, 11, EDIT_TYPE_ED_C},
    {"Note: Unit Codes, 0=Imperial, 1=Metric", NULL, 0, 0, 0, 12, EDIT_TYPE_ED_T},

    {"LOA", "loa", offsetof(ior_rec_t, loa), 8, 0, 0, EDIT_TYPE_ED_F},
    {"FGO", "fgo", offsetof(ior_rec_t, fgo), 8, 2, 0, EDIT_TYPE_ED_F},
    {"AGO", "ago", offsetof(ior_rec_t, ago), 8, 4, 0, EDIT_TYPE_ED_F},
    {"GSDA", "gsda", offsetof(ior_rec_t, gsda), 8, 6, 0, EDIT_TYPE_ED_F},
    {"GLAI", "glai", offsetof(ior_rec_t, glai), 8, 8, 0, EDIT_TYPE_ED_F},
    {"GSDF", "gsdf", offsetof(ior_rec_t, gsdf), 8, 0, 1, EDIT_TYPE_ED_F},
    {"FD", "fd", offsetof(ior_rec_t, fd), 8, 2, 1, EDIT_TYPE_ED_F},
    {"CMD", "cmd", offsetof(ior_rec_t, cmd), 8, 4, 1, EDIT_TYPE_ED_F},
    {"MD", "md", offsetof(ior_rec_t, md), 8, 6, 1, EDIT_TYPE_ED_F},
    {"OMD", "omd", offsetof(ior_rec_t, omd), 8, 8, 1, EDIT_TYPE_ED_F},
    {"BMAX", "bmax", offsetof(ior_rec_t, bmax), 8, 0, 2, EDIT_TYPE_ED_F},
    {"B", "b", offsetof(ior_rec_t, b), 8, 2, 2, EDIT_TYPE_ED_F},
    {"BF", "bf", offsetof(ior_rec_t, bf), 8, 4, 2, EDIT_TYPE_ED_F},
    {"BFI", "bfi", offsetof(ior_rec_t, bfi), 8, 6, 2, EDIT_TYPE_ED_F},
    {"BAI", "bai", offsetof(ior_rec_t, bai), 8, 8, 2, EDIT_TYPE_ED_F},
    {"BA", "ba", offsetof(ior_rec_t, ba), 8, 0, 3, EDIT_TYPE_ED_F},
    {"GD", "gd", offsetof(ior_rec_t, gd), 8, 2, 3, EDIT_TYPE_ED_F},
    {"Y", "y", offsetof(ior_rec_t, y), 8, 4, 3, EDIT_TYPE_ED_F},
    {"GDFI", "gdfi", offsetof(ior_rec_t, gdfi), 8, 6, 3, EDIT_TYPE_ED_F},
    {"VHAI", "vhai", offsetof(ior_rec_t, vhai), 8, 8, 3, EDIT_TYPE_ED_F},
    {"VHA", "vha", offsetof(ior_rec_t, vha), 8, 0, 4, EDIT_TYPE_ED_F},
    {"BHAI", "bhai", offsetof(ior_rec_t, bhai), 8, 2, 4, EDIT_TYPE_ED_F},
    {"BHA", "bha", offsetof(ior_rec_t, bha), 8, 4, 4, EDIT_TYPE_ED_F},
    {"DMT", "dmt", offsetof(ior_rec_t, dmt), 8, 6, 4, EDIT_TYPE_ED_F},
    {"PDT", "pdt", offsetof(ior_rec_t, pdt), 8, 8, 4, EDIT_TYPE_ED_F},
    {"SBMAX", "sbmax", offsetof(ior_rec_t, sbmax), 7, 0, 5, EDIT_TYPE_ED_F},
    {"SDM", "sdm", offsetof(ior_rec_t, sdm), 8, 2, 5, EDIT_TYPE_ED_F},
    {"SPD", "spd", offsetof(ior_rec_t, spd), 8, 4, 5, EDIT_TYPE_ED_F},
    {"BBS", "bbs", offsetof(ior_rec_t, bbs), 8, 6, 5, EDIT_TYPE_ED_F},
    {"BSC", "bsc", offsetof(ior_rec_t, bsc), 8, 8, 5, EDIT_TYPE_ED_F},
    {"FJS", "fss", offsetof(ior_rec_t, fss), 8, 0, 6, EDIT_TYPE_ED_F},
    {"FFS", "ffs", offsetof(ior_rec_t, ffs), 8, 2, 6, EDIT_TYPE_ED_F},
    {"FFIS", "ffis", offsetof(ior_rec_t, ffis), 8, 4, 6, EDIT_TYPE_ED_F},
    {"FFDS", "ffds", offsetof(ior_rec_t, ffds), 8, 6, 6, EDIT_TYPE_ED_F},
    {"FBIS", "fbis", offsetof(ior_rec_t, fbis), 8, 8, 6, EDIT_TYPE_ED_F},
    {"FMDS", "fmds", offsetof(ior_rec_t, fmds), 8, 0, 7, EDIT_TYPE_ED_F},
    {"FBMS", "fbms", offsetof(ior_rec_t, fbms), 8, 2, 7, EDIT_TYPE_ED_F},
    {"FAIS", "fais", offsetof(ior_rec_t, fais), 8, 4, 7, EDIT_TYPE_ED_F},
    {"FAS", "fas", offsetof(ior_rec_t, fas), 8, 6, 7, EDIT_TYPE_ED_F},
    {"DMS", "dms", offsetof(ior_rec_t, dms), 8, 8, 7, EDIT_TYPE_ED_F},
    {"PDS", "pds", offsetof(ior_rec_t, pds), 8, 0, 8, EDIT_TYPE_ED_F},
    {"ESDS", "esds", offsetof(ior_rec_t, esds), 8, 2, 8, EDIT_TYPE_ED_F},
    {"WLH1", "wlh1", offsetof(ior_rec_t, wlh1), 8, 4, 8, EDIT_TYPE_ED_F},
    {"TWL1", "twl0", offsetof(ior_rec_t, twl[0]), 8, 0, 9, EDIT_TYPE_ED_F},
    {"TWL2", "twl1", offsetof(ior_rec_t, twl[1]), 8, 2, 9, EDIT_TYPE_ED_F},
    {"TWL3", "twl2", offsetof(ior_rec_t, twl[2]), 8, 4, 9, EDIT_TYPE_ED_F},
    {"TWL4", "twl3", offsetof(ior_rec_t, twl[3]), 8, 6, 9, EDIT_TYPE_ED_F},
    {"TWL5", "twl4", offsetof(ior_rec_t, twl[4]), 8, 8, 9, EDIT_TYPE_ED_F},
    {"MAW1", "raw1", offsetof(ior_rec_t, raw1), 8, 0, 10, EDIT_TYPE_ED_F},
    {"MAW2", "raw2", offsetof(ior_rec_t, raw2), 8, 2, 10, EDIT_TYPE_ED_F},
    {"MACG1", "racg1", offsetof(ior_rec_t, racg1), 7, 4, 10, EDIT_TYPE_ED_F},
    {"MACG2", "racg2", offsetof(ior_rec_t, racg2), 7, 6, 10, EDIT_TYPE_ED_F},
    {"MACL", "macl", offsetof(ior_rec_t, macl), 8, 8, 10, EDIT_TYPE_ED_F},
    {"MACO", "maco", offsetof(ior_rec_t, maco), 8, 0, 11, EDIT_TYPE_ED_F},
    {"WCBA", "wcba", offsetof(ior_rec_t, wcba), 8, 2, 11, EDIT_TYPE_ED_F},
    {"WCBB", "wcbb", offsetof(ior_rec_t, wcbb), 8, 4, 11, EDIT_TYPE_ED_F},
    {"CBDA", "cbda", offsetof(ior_rec_t, cbda), 8, 6, 11, EDIT_TYPE_ED_F},
    {"CBDB", "cbdb", offsetof(ior_rec_t, cbdb), 8, 8, 11, EDIT_TYPE_ED_F},
    {"CBLDA", "cblda", offsetof(ior_rec_t, cblda), 7, 0, 12, EDIT_TYPE_ED_F},
    {"CBLDB", "cbldb", offsetof(ior_rec_t, cbldb), 7, 2, 12, EDIT_TYPE_ED_F},
    {"DMC", "dmc", offsetof(ior_rec_t, dmc), 8, 4, 12, EDIT_TYPE_ED_F},
    {"MDIAS", "mdias", offsetof(ior_rec_t, mdias), 7, 6, 12, EDIT_TYPE_ED_F},
    {"CD", "cd", offsetof(ior_rec_t, cd), 8, 8, 12, EDIT_TYPE_ED_F},
    {"CCFD  [0/n]", "idc0", offsetof(ior_rec_t, idc[0]), 1, 0, 13, EDIT_TYPE_ED_I},
    {"CCOMD [0/n]", "idc1", offsetof(ior_rec_t, idc[1]), 1, 2, 13, EDIT_TYPE_ED_I},
    {"CCMD  [0/n]", "idc2", offsetof(ior_rec_t, idc[2]), 1, 4, 13, EDIT_TYPE_ED_I},
    {"CCCMD [0/n]", "idc3", offsetof(ior_rec_t, idc[3]), 1, 6, 13, EDIT_TYPE_ED_I},
    {"CCAI  [0/n]", "idc4", offsetof(ior_rec_t, idc[4]), 1, 8, 13, EDIT_TYPE_ED_I},

    {"Prop Code", "ipt", offsetof(ior_rec_t, ipt), 1, 0, 0, EDIT_TYPE_ED_I},
    {"PRD", "prd", offsetof(ior_rec_t, prd), 8, 2, 0, EDIT_TYPE_ED_F},
    {"PBW", "pbw", offsetof(ior_rec_t, pbw), 8, 4, 0, EDIT_TYPE_ED_F},
    {"ESL", "esl", offsetof(ior_rec_t, esl), 8, 6, 0, EDIT_TYPE_ED_F},
    {"ESC", "esc", offsetof(ior_rec_t, esc), 8, 8, 0, EDIT_TYPE_ED_F},
    {"ST1", "st1", offsetof(ior_rec_t, st1), 8, 0, 1, EDIT_TYPE_ED_F},
    {"ST2", "st2", offsetof(ior_rec_t, st2), 8, 2, 1, EDIT_TYPE_ED_F},
    {"ST3", "st3", offsetof(ior_rec_t, st3), 8, 4, 1, EDIT_TYPE_ED_F},
    {"APH", "aph", offsetof(ior_rec_t, aph), 8, 6, 1, EDIT_TYPE_ED_F},
    {"APT/EDL", "apt", offsetof(ior_rec_t, apt), 6, 8, 1, EDIT_TYPE_ED_F},
    {"APB/EDC", "apb", offsetof(ior_rec_t, apb), 6, 0, 2, EDIT_TYPE_ED_F},
    {"EW", "ew", offsetof(ior_rec_t, ew), 8, 2, 2, EDIT_TYPE_ED_F},
    {"EWD", "ewd", offsetof(ior_rec_t, ewd), 8, 4, 2, EDIT_TYPE_ED_F},
    {"PSD", "psd", offsetof(ior_rec_t, psd), 8, 6, 2, EDIT_TYPE_ED_F},
    {"PHD", "phd", offsetof(ior_rec_t, phd), 8, 8, 2, EDIT_TYPE_ED_F},
    {"AW", "aw", offsetof(ior_rec_t, aw), 8, 0, 4, EDIT_TYPE_ED_F},
    {"AWD", "awd", offsetof(ior_rec_t, awd), 8, 2, 4, EDIT_TYPE_ED_F},
    {"APD", "apd", offsetof(ior_rec_t, apd), 8, 4, 4, EDIT_TYPE_ED_F},
    {"BW", "bw", offsetof(ior_rec_t, bw), 8, 0, 5, EDIT_TYPE_ED_F},
    {"BWD", "bwd", offsetof(ior_rec_t, bwd), 8, 2, 5, EDIT_TYPE_ED_F},
    {"BPD", "bpd", offsetof(ior_rec_t, bpd), 8, 4, 5, EDIT_TYPE_ED_F},
    {"CW", "cw", offsetof(ior_rec_t, cw), 8, 0, 6, EDIT_TYPE_ED_F},
    {"CWD", "cwd", offsetof(ior_rec_t, cwd), 8, 2, 6, EDIT_TYPE_ED_F},
    {"CPD", "cpd", offsetof(ior_rec_t, cpd), 8, 4, 6, EDIT_TYPE_ED_F},
    {"DW", "dw", offsetof(ior_rec_t, dw), 8, 0, 7, EDIT_TYPE_ED_F},
    {"DWD", "dwd", offsetof(ior_rec_t, dwd), 8, 2, 7, EDIT_TYPE_ED_F},
    {"DPD", "dpd", offsetof(ior_rec_t, dpd), 8, 4, 7, EDIT_TYPE_ED_F},
    {"W(1)", "wts0", offsetof(ior_rec_t, wts[0]), 8, 0, 9, EDIT_TYPE_ED_F},
    {"W(2)", "wts1", offsetof(ior_rec_t, wts[1]), 8, 2, 9, EDIT_TYPE_ED_F},
    {"W(3)", "wts2", offsetof(ior_rec_t, wts[2]), 8, 4, 9, EDIT_TYPE_ED_F},
    {"W(4)", "wts3", offsetof(ior_rec_t, wts[3]), 8, 6, 9, EDIT_TYPE_ED_F},
    {"W(5)", "wts4", offsetof(ior_rec_t, wts[4]), 8, 8, 9, EDIT_TYPE_ED_F},
    {"SD(1)", "pdef00", offsetof(ior_rec_t, pdef[0][0]), 8, 0, 10, EDIT_TYPE_ED_F},
    {"SD(2)", "pdef01", offsetof(ior_rec_t, pdef[0][1]), 8, 2, 10, EDIT_TYPE_ED_F},
    {"SD(3)", "pdef02", offsetof(ior_rec_t, pdef[0][2]), 8, 4, 10, EDIT_TYPE_ED_F},
    {"SD(4)", "pdef03", offsetof(ior_rec_t, pdef[0][3]), 8, 6, 10, EDIT_TYPE_ED_F},
    {"SD(5)", "pdef04", offsetof(ior_rec_t, pdef[0][4]), 8, 8, 10, EDIT_TYPE_ED_F},
    {"PD(1)", "pdef10", offsetof(ior_rec_t, pdef[1][0]), 8, 0, 11, EDIT_TYPE_ED_F},
    {"PD(2)", "pdef11", offsetof(ior_rec_t, pdef[1][1]), 8, 2, 11, EDIT_TYPE_ED_F},
    {"PD(3)", "pdef12", offsetof(ior_rec_t, pdef[1][2]), 8, 4, 11, EDIT_TYPE_ED_F},
    {"PD(4)", "pdef13", offsetof(ior_rec_t, pdef[1][3]), 8, 6, 11, EDIT_TYPE_ED_F},
    {"PD(5)", "pdef14", offsetof(ior_rec_t, pdef[1][4]), 8, 8, 11, EDIT_TYPE_ED_F},
    {"SWD", "swd", offsetof(ior_rec_t, swd), 8, 0, 12, EDIT_TYPE_ED_F},
    {"PWD", "pwd", offsetof(ior_rec_t, pwd), 8, 2, 12, EDIT_TYPE_ED_F},
    {"FF", "ffm", offsetof(ior_rec_t, ffm), 8, 0, 14, EDIT_TYPE_ED_F},
    {"FA", "fam", offsetof(ior_rec_t, fam), 8, 2, 14, EDIT_TYPE_ED_F},
    {"BWL", "bwl", offsetof(ior_rec_t, bwl), 8, 4, 14, EDIT_TYPE_ED_F},
    {"PL", "pl", offsetof(ior_rec_t, pl), 9, 6, 14, EDIT_TYPE_ED_F},
    {"SG", "sg", offsetof(ior_rec_t, sg), 8, 8, 14, EDIT_TYPE_ED_F},
    {"FJ", "fs", offsetof(ior_rec_t, fs), 8, 0, 15, EDIT_TYPE_ED_F},
    {"FBI", "fbi", offsetof(ior_rec_t, fbi), 8, 2, 15, EDIT_TYPE_ED_F},
    {"FBM", "fbm", offsetof(ior_rec_t, fbm), 8, 4, 15, EDIT_TYPE_ED_F},
    {"Float Day:", "ifda", offsetof(ior_rec_t, ifda), 2, 0, 16, EDIT_TYPE_ED_I},
    {"Month:", "ifmo", offsetof(ior_rec_t, ifmo), 2, 2, 16, EDIT_TYPE_ED_I},
    {"Year", "ifyr", offsetof(ior_rec_t, ifyr), 2, 4, 16, EDIT_TYPE_ED_I},

    {"Note: Prop Codes: 0=None, 1=Fixed, 2=Folding, 3=Feathering, 4=Outboard", NULL, 0, 0, 0, 17, EDIT_TYPE_ED_T},

    {"IG", "ig", offsetof(ior_rec_t, ig), 8, 0, 0, EDIT_TYPE_ED_F},
    {"ISP", "isp", offsetof(ior_rec_t, isp), 8, 2, 0, EDIT_TYPE_ED_F},
    {"J ", "j", offsetof(ior_rec_t, j), 8, 4, 0, EDIT_TYPE_ED_F},
    {"SPL", "spl", offsetof(ior_rec_t, spl), 8, 6, 0, EDIT_TYPE_ED_F},
    {"SL", "sl", offsetof(ior_rec_t, sl), 8, 8, 0, EDIT_TYPE_ED_F},
    {"SMW", "smw", offsetof(ior_rec_t, smw), 8, 0, 1, EDIT_TYPE_ED_F},
    {"LPG", "lpg", offsetof(ior_rec_t, lpg), 8, 2, 1, EDIT_TYPE_ED_F},
    {"LPIS", "lpis", offsetof(ior_rec_t, lpis), 8, 4, 1, EDIT_TYPE_ED_F},
    {"HBS", "hbs", offsetof(ior_rec_t, hbs), 8, 6, 1, EDIT_TYPE_ED_F},
    {"FSP", "fsp", offsetof(ior_rec_t, fsp), 8, 8, 1, EDIT_TYPE_ED_F},
    {"SPS", "sps", offsetof(ior_rec_t, sps), 8, 0, 2, EDIT_TYPE_ED_F},
    {"SJS", "sfs", offsetof(ior_rec_t, sfs), 8, 2, 2, EDIT_TYPE_ED_F},
    {"GO", "go", offsetof(ior_rec_t, go), 8, 4, 2, EDIT_TYPE_ED_F},
    {"MW", "mw", offsetof(ior_rec_t, mw), 8, 6, 2, EDIT_TYPE_ED_F},
    {"Rig Factor:", "mpen", offsetof(ior_rec_t, mpen), 1, 8, 2, EDIT_TYPE_ED_I},
    {"P ", "p", offsetof(ior_rec_t, p), 8, 0, 4, EDIT_TYPE_ED_F},
    {"E ", "e", offsetof(ior_rec_t, e), 8, 2, 4, EDIT_TYPE_ED_F},
    {"BAL", "bal", offsetof(ior_rec_t, bal), 8, 4, 4, EDIT_TYPE_ED_F},
    {"BD", "bd", offsetof(ior_rec_t, bd), 8, 6, 4, EDIT_TYPE_ED_F},
    {"BAS", "bas", offsetof(ior_rec_t, bas), 8, 8, 4, EDIT_TYPE_ED_F},
    {"HB", "hb", offsetof(ior_rec_t, hb), 8, 0, 5, EDIT_TYPE_ED_F},
    {"BLP", "blp", offsetof(ior_rec_t, blp), 8, 2, 5, EDIT_TYPE_ED_F},
    {"MGU", "mgu", offsetof(ior_rec_t, mgu), 8, 4, 5, EDIT_TYPE_ED_F},
    {"MGM", "mgm", offsetof(ior_rec_t, mgm), 8, 6, 5, EDIT_TYPE_ED_F},
    {"BL1", "bl1", offsetof(ior_rec_t, bl1), 8, 0, 6, EDIT_TYPE_ED_F},
    {"BL2", "bl2", offsetof(ior_rec_t, bl2), 8, 2, 6, EDIT_TYPE_ED_F},
    {"BL3", "bl3", offsetof(ior_rec_t, bl3), 8, 4, 6, EDIT_TYPE_ED_F},
    {"BL4", "bl4", offsetof(ior_rec_t, bl4), 8, 6, 6, EDIT_TYPE_ED_F},
    {"BL5", "bl5", offsetof(ior_rec_t, bl5), 8, 8, 6, EDIT_TYPE_ED_F},
    {"EB", "eb", offsetof(ior_rec_t, eb), 8, 0, 9, EDIT_TYPE_ED_F},
    {"IY", "iy", offsetof(ior_rec_t, iy), 8, 2, 9, EDIT_TYPE_ED_F},
    {"IS", "is", offsetof(ior_rec_t, is), 8, 4, 9, EDIT_TYPE_ED_F},
    {"BADY/S", "bads", offsetof(ior_rec_t, bads), 6, 6, 9, EDIT_TYPE_ED_F},
    {"PY/PSF", "py", offsetof(ior_rec_t, py), 6, 8, 9, EDIT_TYPE_ED_F},
    {"EY/EF", "ey", offsetof(ior_rec_t, ey), 7, 0, 10, EDIT_TYPE_ED_F},
    {"BALY/F", "baly", offsetof(ior_rec_t, baly), 6, 2, 10, EDIT_TYPE_ED_F},
    {"BDY/F", "bdy", offsetof(ior_rec_t, bdy), 7, 4, 10, EDIT_TYPE_ED_F},
    {"HBY/F", "hby", offsetof(ior_rec_t, hby), 7, 6, 10, EDIT_TYPE_ED_F},
    {"BLPY/F", "blpy", offsetof(ior_rec_t, blpy), 6, 8, 10, EDIT_TYPE_ED_F},
    {"BY1/S1", "by1", offsetof(ior_rec_t, by1), 6, 0, 11, EDIT_TYPE_ED_F},
    {"BY2/S2", "by2", offsetof(ior_rec_t, by2), 6, 2, 11, EDIT_TYPE_ED_F},
    {"BY3/S3", "by3", offsetof(ior_rec_t, by3), 6, 4, 11, EDIT_TYPE_ED_F},
    {"BY4/S4", "by4", offsetof(ior_rec_t, by4), 6, 6, 11, EDIT_TYPE_ED_F},
    {"BY5/S5", "by5", offsetof(ior_rec_t, by5), 6, 8, 11, EDIT_TYPE_ED_F},
    {"YSF/OF", "ysf", offsetof(ior_rec_t, ysf), 6, 0, 12, EDIT_TYPE_ED_F},
    {"YSD/S4", "ysd", offsetof(ior_rec_t, ysd), 6, 2, 12, EDIT_TYPE_ED_F},
    {"YSMG/S5", "ysmg", offsetof(ior_rec_t, ysmg), 6, 4, 12, EDIT_TYPE_ED_F},
    {"S6", "s6", offsetof(ior_rec_t, s6), 8, 6, 12, EDIT_TYPE_ED_F},
    {"BADX", "badx", offsetof(ior_rec_t, badx), 8, 8, 12, EDIT_TYPE_ED_F},
    {"G", "g", offsetof(ior_rec_t, g), 8, 0, 13, EDIT_TYPE_ED_F},
    {"H", "h", offsetof(ior_rec_t, h), 8, 2, 13, EDIT_TYPE_ED_F},
    {"S1", "s1", offsetof(ior_rec_t, s1), 8, 4, 13, EDIT_TYPE_ED_F},
    {"S2", "s2", offsetof(ior_rec_t, s2), 8, 6, 13, EDIT_TYPE_ED_F},
    {"S3", "s3", offsetof(ior_rec_t, s3), 8, 8, 13, EDIT_TYPE_ED_F},
    {"GY", "gy", offsetof(ior_rec_t, gy), 8, 0, 14, EDIT_TYPE_ED_F},
    {"HY", "hy", offsetof(ior_rec_t, hy), 8, 2, 14, EDIT_TYPE_ED_F},
    {"Mule:", "mule", offsetof(ior_rec_t, mule), 2, 4, 14, EDIT_TYPE_ED_I},
    {"Note: Rig Factor: 0=None, 1=LRP, 2=SMF, 3=LRP & SMF", NULL, 0, 0, 0, 15, EDIT_TYPE_ED_T},
    {"Note: Mule code  :  0 None,  1 Mule, 2 Topsail", NULL, 0, 0, 0, 16, EDIT_TYPE_ED_T}};

size_t editlist_length1 = (sizeof(editlist) / sizeof(edit_field_t));

static char *attrs[] = {"yacht", "sail", "yr",   "loa", "fgo", "fd",  "cmd",   "md",
                        "omd", "bmax", "b",    "bf", "ba", "vha", "sbmax",
                        "sdm", "fss", "ffs",  "ffis", "ffds", "fmds", "fas",
                        "dms", "ffm",  "fam", "bwl",   "pl",  "p",   "e",    "bas"};

bool is_data_valid(ior_rec_t *u) {
  edit_field_t *ep = NULL;
  bool ret = true;
  for (int i = 0; i < sizeof(attrs) / sizeof(char *); i++) {
    if ((ep = find_field_by_name(attrs[i], u)) == NULL) {
      ret = false;
    } else {
      switch (ep->flag) {
      case EDIT_TYPE_ED_C:
        if (*(char *)((void*)u + ep->offset) == 0) {
          ret = false;
        }
        break;
      case EDIT_TYPE_ED_I:
        if (*(int *)((void*)u + ep->offset) == 0) {
          ret = false;
        }
        break;
      case EDIT_TYPE_ED_F:
        if (*(double *)((void*)u + ep->offset) == 0) {
          ret = false;
        }
        break;
      case EDIT_TYPE_ED_T:
        return false;
        break;
      }
    }
    if (ret == false) {
      // stderr.printf("Failed to find or validate %s\n", attrs[i]);
      return false;
    }
  }
  return true;
}

edit_field_t *find_field_by_name(char *name, ior_rec_t *u) {
  edit_field_t *r = NULL;
  if (name != NULL && u != NULL) {
    for (int i = 0; i < editlist_length1; i++) {
      if (editlist[i].ename != NULL) {
        if (strcmp(editlist[i].ename, name) == 0) {
          r = editlist + i;
          break;
        }
      }
    }
  }
  return r;
}

int get_field(int j, edit_field_t *ef) {
  if (j >= editlist_length1)
    return -1;
  else {
    *ef = editlist[j];
    return 0;
  }
}

void update_rec(const char *dt, ior_rec_t *u, int count) {
  switch (editlist[count].flag) {
  case EDIT_TYPE_ED_C:
    set_string(dt, u, count);
    break;
  case EDIT_TYPE_ED_F:
    *(double *)((void *)u + editlist[count].offset) = strtod(dt, NULL);
    break;
  case EDIT_TYPE_ED_I:
    *(int *)((void *)u + editlist[count].offset) = strtol(dt, NULL, 10);
    break;
  }
}

char *get_as_string(ior_rec_t *u, int count) {
  char ctext[32];
  char format[8];

  switch (editlist[count].flag) {
  case EDIT_TYPE_ED_C:
    return strdup((char *)((void*)u + editlist[count].offset));
  case EDIT_TYPE_ED_T:
    return strdup(editlist[count].prompt);
  case EDIT_TYPE_ED_F:
    sprintf(format, "%%%d.3f", editlist[count].len);
    double d = *(double *)((void*)u + editlist[count].offset);
    sprintf(ctext, format, d);
    return strdup(ctext);
  case EDIT_TYPE_ED_I:
    sprintf(format, "%%%dd", editlist[count].len);
    sprintf(ctext, format, *(int *)((void*)u + editlist[count].offset));
    return strdup(ctext);
  }
  return NULL; // NOTREACHED
}

int get_int(ior_rec_t *u, int count) { return *(int *)((void*)u + editlist[count].offset); }

double get_double(ior_rec_t *u, int count) { return *(double *)((void*)u + editlist[count].offset); }

char *get_string(ior_rec_t *u, int count) { return (char *)((void*)u + editlist[count].offset); }

void set_int(int v, ior_rec_t *u, int count) { *(int *)((void*)u + editlist[count].offset) = v; }

void set_double(double d, ior_rec_t *u, int count) { *(double *)((void*)u + editlist[count].offset) = d; }

void set_string(const char *s, ior_rec_t *u, int count) { strcpy((void*)u + editlist[count].offset, s); }
