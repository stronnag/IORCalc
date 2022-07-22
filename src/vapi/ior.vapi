[CCode (cheader_filename = "iorext.h")]

namespace IORData {
	[Compact]

	[CCode (cname = "edit_field_t", has_type_id = false)]
	struct EditField {
		unowned string prompt;
		unowned string ename;
		size_t offset;
		uint8 len;
		uint8 col;
		uint8 row;
		uint8 flag;
	}

	[CCode (cname = "EditType_e", cprefix = "EDIT_TYPE_", has_type_id = false)]
	enum EditType {
        ED_C,
        ED_F,
        ED_I,
        ED_T
	}

	[CCode (cname = "get_field")]
	int get(int j, out unowned EditField ef);

	[CCode (cname = "get_as_string")]
	string to_string(void *u, int count);

	[CCode (cname = "read_file")]
	void *read_file(string f);

	[CCode (cname = "ior_write_file")]
	void write_file(string f, void *u);

	[CCode (cname = "update_rec")]
	void update(string s, void *u, int count);

	[CCode (cname = "get_int")]
	int get_int(void *u, int count);

	[CCode (cname = "get_double")]
	double get_double(void *u, int count);

	[CCode (cname = "get_string")]
	unowned string get_string (void *u, int count);

	[CCode (cname = "set_int")]
	void set_int(int v, void *u, int count);

	[CCode (cname = "set_double")]
	void set_double(double d, void *u, int count);

	[CCode (cname = "set_string")]
	void set_string (string s, void *u, int count);

	[CCode (cname = "allocate_ior_rec")]
	void * allocate_ior_rec();

	[CCode (cname = "allocate_calc_rec")]
	void * allocate_calc_rec();

	[CCode (cname = "is_data_valid")]
	bool is_data_valid(void *u);

	[CCode (cname = "find_field_by_name")]
	unowned EditField * find_field_by_name(string name, void *u);

	[CCode (cname = "calc_ior")]
	string calc_ior(void *u, void *c);

	[CCode (cname = "report")]
	string report(void *u, void *c);

	[CCode (cname = "pcert")]
	void pcert(void *u, void *c, string fn, int opt);
}

[CCode (cheader_filename = "iorcalc-config.h", cname="IORCALC_VERSION_STRING")]
public const string IORCALC_VERSION_STRING;
