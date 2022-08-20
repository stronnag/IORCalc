namespace IORData {
	[Compact]
    [CCode (cheader_filename = "ior.h", cname="ior_rec_t", has_type_id = false)]
    public struct IORRec {
        int idaa;
        int idbb;
        int idcc;
        int hmo;
        int hyr;
        int smo;
        int syr;
        int irgm;
        int irgy;
        int mon;
        int yr;
        int munit;
        int istd;
        int ipt;
        int ifda;
        int ifmo;
        int ifyr;
        int cyr;
        int idc[5];
        int icert;
        int mule;
        int mpen;
        double bmax;
        double bsc;
        double bbs;
        double b;
        double wlh1;
        double fgo;
        double bf;
        double gsdf;
        double bfi;
        double gdfi;
        double ago;
        double y;
        double gd;
        double ba;
        double gsda;
        double glai;
        double bai;
        double loa;
        double sbmax;
        double sdm;
        double spd;
        double fss;
        double ffs;
        double ffis;
        double ffds;
        double fbis;
        double fmds;
        double fbms;
        double fais;
        double fas;
        double dms;
        double esds;
        double pds;
        double fd;
        double omd;
        double md;
        double cmd;
        double dmt;
        double pdt;
        double bhai;
        double bha;
        double vha;
        double vhai;
        double wcba;
        double cbda;
        double cblda;
        double wcbb;
        double cbdb;
        double cbldb;
        double cd;
        double raw1;
        double raw2;
        double racg1;
        double racg2;
        double macl;
        double maco;
        double prd;
        double pbw;
        double esl;
        double esc;
        double st1;
        double st2;
        double st3;
        double aph;
        double apt;
        double apb;
        double ew;
        double ewd;
        double psd;
        double fs;
        double fbi;
        double fbm;
        double ffm;
        double fam;
        double bwl;
        double pl;
        double sg;
        double aw;
        double awd;
        double apd;
        double bw;
        double bwd;
        double bpd;
        double cw;
        double cwd;
        double cpd;
        double dw;
        double dwd;
        double dpd;
        double bas;
        double ig;
        double isp;
        double mw;
        double go;
        double p;
        double sps;
        double spl;
        double j;
        double sfs;
        double fsp;
        double e;
        double bal;
        double bd;
        double hb;
        double blp;
        double mgu;
        double mgm;
        double bl1;
        double bl2;
        double bl3;
        double bl4;
        double bl5;
        double lpg;
        double lpis;
        double smw;
        double sl;
        double hbs;
        double g;
        double h;
        double s1;
        double s2;
        double s3;
        double eb;
        double iy;
        double is;
        double bads;
        double py;
        double ey;
        double baly;
        double bdy;
        double hby;
        double blpy;
        double by1;
        double by2;
        double by3;
        double by4;
        double by5;
        double ysf;
        double gy;
        double hy;
        double ysd;
        double ysmg;
        double badx;
        double s6;
        double ra;
        double tmf;
        double mdias;
        double dmc;
        double twl[5];
        double phd;
        double wts[5];
        double pdef1[5];
        double pdef2[5];
        double swd;
        double pwd;
        char yacht[24];
        char sail[12];
        char owner[24];
        char measurer[24];
        char address1[28];
        char address2[28];
        char address3[28];
        char comment1[36];
        char comment2[36];
        char comment3[36];
        char anchor1[12];
        char anchor2[12];
        char anchor3[12];
        char battery1[12];
        char battery2[12];
        char battery3[12];
        char ballast1[12];
        char ballast2[12];
        char ballast3[12];
        char class[18];
        char designer[18];
        char builder[18];
        char rig[12];
        char keel[12];
    }
    [CCode (    cheader_filename = "ior.h", cname="calc_rec_t", has_type_id = false)]
    public struct CalcRec {
        int ihm;
        int ihy;
        int imo;
        int iyr;
        int ecod;
        int crewwt;
        int crewno;
        int pcount;
        double lbg;
        double sink;
        double trim;
        double atrim;
        double ff;
        double ffi;
        double ffd;
        double fmd;
        double fai;
        double fa;
        double pd;
        double dm;
        double esd;
        double fs;
        double cffi;
        double foc;
        double ccor[5];
        double fdic;
        double fdi;
        double mdi;
        double omdi;
        double cmdi;
        double mdia;
        double aocc;
        double aocg;
        double aocp;
        double aoc;
        double apsl;
        double bapsl;
        double apslc;
        double acg1;
        double acg2;
        double agsl;
        double hgla;
        double hgli;
        double lbgc;
        double ycor;
        double l;
        double d;
        double bdr;
        double dlf;
        double fb;
        double fm;
        double fc;
        double db;
        double dspl;
        double cbf;
        double rd;
        double cbfa;
        double maf;
        double dd;
        double dc;
        double prdc;
        double pdc;
        double epf;
        double pf;
        double emf;
        double df;
        double ps;
        double em;
        double arm;
        double brm;
        double crm;
        double drm;
        double rm;
        double rmc;
        double tr;
        double cgf;
        double cgfm;
        double cgfc;
        double csf;
        double bwl1;
        double sv;
        double tci;
        double i;
        double sph;
        double psh;
        double bad;
        double jc;
        double shbs;
        double plf;
        double tti;
        double ic;
        double lp;
        double smwm;
        double slm;
        double ajlmx;
        double rsaf;
        double ec;
        double rh;
        double rhc;
        double xbl1;
        double xbl2;
        double xbl3;
        double xbl4;
        double xbl5;
        double p1;
        double p2;
        double p3;
        double p4;
        double p5;
        double p6;
        double p7;
        double p8;
        double p9;
        double p10;
        double xhb;
        double bpen;
        double xmgu;
        double xmgm;
        double gpen;
        double rgf;
        double ttp;
        double rsam;
        double satm;
        double hc;
        double smin;
        double rsama;
        double satc;
        double satca;
        double rsat;
        double rsata;
        double ebc;
        double ris;
        double efc;
        double fp1;
        double fp2;
        double fp3;
        double fp4;
        double fp5;
        double fp6;
        double fp7;
        double fp8;
        double fp9;
        double fp10;
        double bfpen;
        double psfc;
        double hfc;
        double satf;
        double eyc;
        double y1;
        double y2;
        double y3;
        double y4;
        double y5;
        double y6;
        double y7;
        double y8;
        double y9;
        double y10;
        double bypen;
        double pyc;
        double hyc;
        double spin;
        double rsal;
        double rsala;
        double s;
        double shr;
        double sc;
        double rsay;
        double rsac;
        double rsab;
        double rsak;
        double rsag;
        double ysac;
        double pc;
        double sa;
        double dlfa;
        double cgfa;
        double scfa;
        double msar;
        double shra;
        double tpf;
        double mr;
        double mra;
        double mra1;
        double mra2;
        double r;
        double rior;
        double ra;
        double rtf;
        double rtfa;
        double w;
        double tmf;
        double xblp;
        double scf;
        double sca;
        double lrp;
        double smf;
        double sgfi;
        double slp[2];
        double cor[2];
    }

	[CCode (cheader_filename = "iorext.h", cname = "edit_field_t", has_type_id = false)]
	struct EditField {
		unowned string prompt;
		unowned string ename;
		size_t offset;
		uint8 len;
		uint8 col;
		uint8 row;
		uint8 flag;
	}

	[CCode (cheader_filename = "iorext.h", cname = "EditType_e", cprefix = "EDIT_TYPE_", has_type_id = false)]
	enum EditType {
        ED_C,
        ED_F,
        ED_I,
        ED_T
	}

	[CCode (cheader_filename = "iorext.h", cname = "get_field")]
	int get(int j, out unowned EditField ef);

	[CCode (cheader_filename = "iorext.h", cname = "get_as_string")]
	string to_string(IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "read_file")]
	IORData.IORRec* read_file(string f);

	[CCode (cheader_filename = "iorext.h", cname = "ior_write_file")]
	void write_file(string f, IORData.IORRec* u);

	[CCode (cheader_filename = "iorext.h", cname = "update_rec")]
	void update(string s, IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "get_int")]
	int get_int(IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "get_double")]
	double get_double(IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "get_string")]
	unowned string get_string (IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "set_int")]
	void set_int(int v, IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "set_double")]
	void set_double(double d, IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "set_string")]
	void set_string (string s, IORData.IORRec* u, int count);

	[CCode (cheader_filename = "iorext.h", cname = "allocate_ior_rec")]
	IORData.IORRec* allocate_ior_rec();

	[CCode (cheader_filename = "iorext.h", cname = "allocate_calc_rec")]
	IORData.CalcRec* allocate_calc_rec();

	[CCode (cheader_filename = "iorext.h", cname = "is_data_valid")]
	bool is_data_valid(IORData.IORRec* u);

	[CCode (cheader_filename = "iorext.h", cname = "find_field_by_name")]
	unowned EditField * find_field_by_name(string name, IORData.IORRec* u);

	[CCode (cheader_filename = "iorext.h", cname = "calc_ior")]
	string calc_ior(IORData.IORRec* u, IORData.CalcRec *c);

	[CCode (cheader_filename = "iorext.h", cname = "report")]
	string report(IORData.IORRec* u, IORData.CalcRec *c);

	[CCode (cheader_filename = "iorext.h", cname = "pcert")]
	void pcert(IORData.IORRec* u, IORData.CalcRec *c, string fn, int opt);

    [CCode (cheader_filename = "iorext.h", cname = "get_plot_profile")]
    public void get_plot_profile(IORData.IORRec* u, IORData.CalcRec *c, [CCode(array_length_cname = "xp_count", array_length_type="int")] out double []_xp, [CCode(array_length_cname = "yp_count", array_length_type="int")] out double []_yp);

    [CCode (cheader_filename = "iorext.h", cname = "get_plot_plan")]
    public void get_plot_plan(IORData.IORRec* u, IORData.CalcRec *c, [CCode(array_length_cname = "xp_count", array_length_type="int")] out double []_xp, [CCode(array_length_cname = "yp_count", array_length_type="int")] out double []_yp);

    [CCode (cheader_filename = "iorext.h", cname = "get_plot_section")]
    public void get_plot_section(IORData.IORRec* u, IORData.CalcRec *c, [CCode(array_length_cname = "xp_count", array_length_type="int")] out double []_xp, [CCode(array_length_cname = "yp_count", array_length_type="int")] out double []_yp);
}

[CCode (cheader_filename = "iorcalc-config.h", cname="IORCALC_VERSION_STRING")]
public const string IORCALC_VERSION_STRING;
