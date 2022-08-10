using Gtk;

namespace IChooser {
	Gtk.FileChooserDialog chooser(Gtk.Window w, string? _fn,
								   Gtk.FileChooserAction action, string fty="json") {
		Gtk.FileChooserDialog fc = new Gtk.FileChooserDialog (
			"IOR Data File",
			w, action,
			"_Cancel",
			Gtk.ResponseType.CANCEL,
			(action == Gtk.FileChooserAction.SAVE) ? "_Save" : "_Open",
			Gtk.ResponseType.ACCEPT);

		fc.set_modal(true);
		if (_fn != null) {
			try {
				var fl = File.new_for_path (_fn);
				fc.set_current_folder(fl);
			} catch (Error e) {
				stderr.printf(" Chooser %s\n", e.message);
			}
		}
		fc.select_multiple = false;
		var filter = new Gtk.FileFilter ();
		if(fty == "json") {
			filter.set_filter_name ("JSON files");
			filter.add_pattern ("*.json");
		} else {
			filter.set_filter_name ("Text files");
			filter.add_pattern ("*.txt");
		}
		fc.add_filter (filter);
		filter = new Gtk.FileFilter ();
		filter.set_filter_name ("All Files");
		filter.add_pattern ("*");
		fc.add_filter (filter);
		fc.show();
		return fc;
	}
}

namespace Util {
	string mktempname() {
        var t = Environment.get_tmp_dir();
        var ir = new Rand().int_range (0, 0xffffff);
        var s = Path.build_filename (t, ".ior-%d-%08x".printf(Posix.getpid(), ir));
		return s;
	}
}

public class IEntry : Gtk.Entry {
	public int  idx;
	public IEntry(int _idx) {
		idx = _idx;
	}
}

public class IORWindow : Gtk.ApplicationWindow {
	private Gtk.Notebook nb;
	private void * udata;
	private void * cdata;
	private Gtk.TextView textview;
	private Gtk.Button show_cert;
	private Gtk.Button run_calc;
	private IEntry[] elist = {};
	private string? filename;
	private IORSet kf;
	private Gtk.MenuButton fsmenu_button;

	public IORWindow() {
		udata = null;
		cdata = IORData.allocate_calc_rec();
        set_default_size (800, 720);
		nb = new Gtk.Notebook();

		var header_bar = new Gtk.HeaderBar ();
		header_bar.decoration_layout = "icon:menu,minimize,maximize,close";
		header_bar.set_title_widget (new Gtk.Label("IOR Rating Calculator"));
		header_bar.set_show_title_buttons(true);
		set_titlebar (header_bar);

		var sbuilder = new Builder.from_resource("/org/stronnag/iorcalc/iorcalc.ui");
        var mm = sbuilder.get_object("menubar") as GLib.MenuModel;

		fsmenu_button = new Gtk.MenuButton();
		fsmenu_button.icon_name = "open-menu-symbolic";
		var pop = new Gtk.PopoverMenu.from_model(mm);
        fsmenu_button.set_popover(pop);
        header_bar.pack_end (fsmenu_button);

        var aq = new GLib.SimpleAction("open",null);
        aq.activate.connect(() => {
				show_cert.sensitive = false;
				string? dir = null;
				try {
					dir = kf.kf.get_string("iorcalc", "in-dir");
				} catch {};

				var fc = IChooser.chooser(this, dir, Gtk.FileChooserAction.OPEN);
				fc.response.connect((result) => {
						if (result== Gtk.ResponseType.ACCEPT) {
							var fn = fc.get_file().get_path ();
							var dn = Path.get_dirname(fn);
							if (dn != null) {
								kf.kf.set_string("iorcalc", "in-dir", dn);
							}
							filename = fn;
							ioropen(filename);
							textview.buffer.text = "";
						}
						fc.close();
					});
			});
        add_action(aq);

        aq = new GLib.SimpleAction("save",null);
        aq.activate.connect(() => {
				if(filename == null) {
					save_new_file();
				} else {
					IORIO.save_file(filename, udata);
				}
			});
        add_action(aq);

        aq = new GLib.SimpleAction("saveas",null);
        aq.activate.connect(() => {
				save_new_file();
			});
        add_action(aq);

		aq = new GLib.SimpleAction("calc",null);
        aq.activate.connect(() => {
				var tfn = Path.build_filename(Environment.get_tmp_dir(), ".iorcalc-save.json");
				IORIO.save_file(tfn, udata);
				show_cert.sensitive = false;
				var s0 = IORData.calc_ior(udata, cdata);
				var s1 = IORData.report(udata, cdata);
				textview.buffer.text = s0;

				if(s1 != null) {
					Gtk.TextIter iter;
					textview.buffer.get_end_iter(out iter);
					textview.buffer.insert(ref iter, "-- Warnings\n", -1);
					textview.buffer.insert(ref iter, s1, -1);
				}
				show_cert.sensitive = true;
				FileUtils.unlink(tfn);
			});
        add_action(aq);

		aq = new GLib.SimpleAction("about", null);
        aq.activate.connect(() => {
				var a = new AboutDialog();
				a.version = IORCALC_VERSION_STRING;
				a.authors = {"Jonathan Hudson <jh+ior@daria.co.uk>"};
				a.copyright = "(c) Jonathan Hudson 1984-2022";
				a.license_type = License.GPL_3_0;
				a.program_name = "IORCalc";
				a.website = "https://github.com/stronnag/IORCalc";
				a.documenters = {"Jonathan Hudson"};
				a.comments = "Calculate IOR Ratings like it's 1988";
				a.website_label = "IORCalc on Github";
				a.logo = Gdk.Texture.from_resource("/org/stronnag/iorcalc/iorcalc.svg");
				a.show();
			});
        add_action(aq);

		aq = new GLib.SimpleAction("close",null);
        aq.activate.connect(() => {
                close();
            });

        add_action(aq);
		set_icon_name("iorcalc");

		var vbox = new Gtk.Box (Gtk.Orientation.VERTICAL, 0);
		vbox.append (nb);

		textview = new Gtk.TextView();
		textview.monospace = true;
		textview.editable = false;
		textview.vexpand = true;

		var scrolled = new Gtk.ScrolledWindow ();
		scrolled.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
        scrolled.set_child (textview);

		var bbox = new Gtk.Box (Gtk.Orientation.HORIZONTAL,2);
		bbox.set_spacing (5);
		run_calc = new Gtk.Button.with_label("Calculate Rating");
		run_calc.set_action_name("win.calc");

		show_cert = new Gtk.Button.with_label("Show Certificate");
		show_cert.sensitive = false;
		show_cert.clicked.connect(() => {
				var fn = Util.mktempname();
				IORData.pcert(udata, cdata, fn, 1);
				var cw = new CertWindow(this, kf, udata, cdata);
				toggle_cert_actions(false);
				cw.close_request.connect(() => {
						toggle_cert_actions(true);
						return false;
					});
				cw.load_file(fn);
			});

		bbox.hexpand = true;
 		run_calc.hexpand = true;
		show_cert.hexpand = true;
		run_calc.halign = Gtk.Align.CENTER;
		show_cert.halign = Gtk.Align.CENTER;

		bbox.prepend(run_calc);
		bbox.append(show_cert);

		vbox.append (scrolled);
		vbox.append (bbox);

		var droptgt = new Gtk.DropTarget(typeof (Gdk.FileList), Gdk.DragAction.COPY);
		droptgt.on_drop.connect((tgt, value, x, y) => {
				if(value.type() == typeof (Gdk.FileList)) {
					var flist = ((Gdk.FileList)value).get_files();
					foreach(var u in flist) {
						var fn = u.get_path();
						if(valid_file(fn)) {
							var w = new IORWindow();
							this.application.add_window (w);
							w.setup(kf, fn);
							w.run();
						}
					}
				}
				return true;
			});
		textview.add_controller((EventController)droptgt);
		set_child (vbox);
	}

	private bool valid_file(string? uri) {
		bool res = false;
		string fn;
		try {
			if (uri.has_prefix("file://")) {
				fn = Filename.from_uri(uri);
			} else {
				fn = uri;
			}
			uint8 buf[1024]={0};
			var fs = FileStream.open (fn, "r");
			if (fs != null) {
				if(fs.read (buf) > 0) {
					if(((string)buf).contains("\"yacht\" : ")) {
						res = true;
					}
				}
			}
		} catch (Error e) {
			print("validate %s\n", e.message);
		}
		return res;
	}

	private void toggle_cert_actions(bool act) {
		fsmenu_button.sensitive = act;
		nb.sensitive = act;
		run_calc.sensitive = act;
		show_cert.sensitive = act;
	}

	public void setup(IORSet _kf, string? _filename) {
		kf = _kf;
		filename = _filename;
	}

	public void run() {
		udata = IORIO.read_file(filename);
		populate_grid();
		var is_ok = IORData.is_data_valid(udata);
		set_menu_states(is_ok);
		present();
	}

	public IEntry? find_ientry(int i) {
		foreach(var e in elist) {
			if (e.idx == i) {
				return e;
			}
		}
		return null;
	}

	private void save_new_file() {
		string? dir = null;
		try {
			dir = kf.kf.get_string("iorcalc", "out-dir");
		} catch {};
		var fc = IChooser.chooser(this, dir, Gtk.FileChooserAction.SAVE);
		fc.response.connect((result) => {
				if (result== Gtk.ResponseType.ACCEPT) {
					var fn = fc.get_file().get_path ();
					var dn = fc.get_current_folder().get_path();
					if (dn != null) {
						kf.kf.set_string("iorcalc", "out-dir", dn);
					}
					filename = fn;
					IORIO.save_file(filename, udata);
				}
				fc.close();
			});
	}

	private void set_menu_states(bool is_ok) {
		set_menu_state("calc", is_ok);
	}

	public void ioropen(string? fn) {
		udata = IORIO.read_file(fn);
		update_grid();
		var is_ok = IORData.is_data_valid(udata);
		set_menu_states(is_ok);
	}

	private void update_grid() {
		for(var i = 0 ; ; i++) {
			IORData.EditField ef;
			var ok = IORData.get(i, out ef);
			if (ok != 0)
				break;
			if(ef.flag != IORData.EditType.ED_T) {
				var ent = find_ientry(i);
				var s = IORData.to_string(udata, i);
				ent.text = s;
			}
		}
	}

	private void set_menu_state(string action, bool state) {
        var ac = lookup_action(action) as SimpleAction;
		if(ac != null)
			ac.set_enabled(state);
    }

	private void populate_grid() {
		string tlabs[]={"Names", "Hull", "AFloat/Prop","Rig"};
		Gtk.Label lab;
		IEntry e;
		Gtk.Grid grid = null;

		int row = -1;
        int col = 0;
        int gid = 0;

		for(var i = 0 ; ; i++) {
			IORData.EditField ef;
			var ok = IORData.get(i, out ef);
			if (ok != 0)
				break;

			if (ef.row != row) {
				col = 0;
				if(ef.row == 0) {
					grid = new Grid();
					grid.set_column_homogeneous (false);
					grid.set_column_spacing (2);
					nb.append_page(grid, new Gtk.Label(tlabs[gid]));
					gid++;
				}
				row = ef.row;
			}
			lab = new Gtk.Label(strdup(ef.prompt));
			lab.justify = Gtk.Justification.LEFT;
			lab.xalign = 0;
//			lab.set_padding(2,0);
			if (ef.flag != IORData.EditType.ED_T) {
				grid.attach(lab, col, row);
				col++;
				e = new IEntry(i);
				elist += e;
				e.width_chars = ef.len;
				switch(ef.flag) {
				case IORData.EditType.ED_F:
					e.input_purpose = Gtk.InputPurpose.NUMBER;
					break;
				case IORData.EditType.ED_I:
					e.input_purpose = Gtk.InputPurpose.DIGITS;
					break;
				case IORData.EditType.ED_C:
					e.max_length = ef.len-1;
					break;
				default:
					break;
				}
				if (udata != null) {
					var s = IORData.to_string(udata, i);
					e.text = s;
				}
				var evc = new  EventControllerFocus();
				e.add_controller(evc);
				evc.leave.connect(() => {
						var ent = evc.widget as IEntry;
						var wtext = ent.get_text();
						var utext = IORData.to_string(udata, ent.idx);
						if(wtext != utext) {
							IORData.update(wtext, udata, ent.idx);
							var t = IORData.to_string(udata, ent.idx);
							ent.set_text(t);
							show_cert.sensitive = false;
							var is_ok = IORData.is_data_valid(udata);
							set_menu_state("calc", is_ok);
						}
					});

				grid.attach(e, col, row);
				col++;
			} else {
				grid.attach(lab, col, row, 4, 1);
			}
		}
	}
}

public class IORCalc : Gtk.Application {
	private string? filename;
	private IORSet kf;

	public IORCalc () {
		Object(application_id: "org.stronnag.iorcalc",
			   flags: ApplicationFlags.HANDLES_COMMAND_LINE);

		const OptionEntry[] options = {
			{ "version", 'v', 0, OptionArg.NONE, null, "show version", null},
			{null}
		};

		kf = new IORSet();
		kf.setup_keyfile();

		add_main_option_entries(options);
		handle_local_options.connect(do_handle_local_options);
		activate.connect(handle_activate);
		set_actions();
	}

	private void set_actions() {
  		set_accels_for_action ("win.calc", { "<Ctrl>r" });
		set_accels_for_action ("win.save", { "<Ctrl>s" });
		set_accels_for_action ("win.open", { "<Ctrl>o" });
		set_accels_for_action ("win.about", { "F1" });
		set_accels_for_action ("win.close", { "<Ctrl>c" });
		var sac = new SimpleAction ("new-window", null);
		sac.activate.connect (() => {
				create_ior_window();
			});
		add_action(sac);
		set_accels_for_action ("app.new-window", { "<Ctrl>n" });

		sac = new SimpleAction ("quit", null);
		sac.activate.connect (() => {
				quit();
			});
		add_action(sac);
		set_accels_for_action ("app.quit", { "<Ctrl>q" });
	}

    private void handle_activate () {
		create_ior_window(filename);
		filename=null;
    }

	private void create_ior_window(string? fn=null) {
		var window = new IORWindow();
		add_window (window);
		window.setup(kf, fn);
		window.run();
	}

	public void save_settings() {
		if(kf != null) {
			kf.save_settings();
		}
	}

    private int do_handle_local_options(VariantDict o) {
        if (o.contains("version")) {
            stdout.printf("%s\n", IORCALC_VERSION_STRING);
            return 0;
        }
		return -1;
    }

	private int _command_line (ApplicationCommandLine command_line) {
		string[] args = command_line.get_arguments ();
		if(args.length > 1) {
			filename = args[1];
		}
		activate();
		return 0;
	}

	public override int command_line (ApplicationCommandLine command_line) {
		hold ();
		int res = _command_line (command_line);
		release ();
		return res;
	}

	public static int main (string[] args) {
        var ior = new IORCalc ();
        ior.run (args);
		ior.save_settings();
        return 0;
    }
}
