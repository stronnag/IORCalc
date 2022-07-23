using Gtk;

public class EList : Object {
	public struct list {
		public int idx;
		public Gtk.Entry e;
	}

	private static list [] elist = {};

	public static new Gtk.Entry? get(int j) {
		foreach(var e in elist) {
			if (j == e.idx)
				return e.e;
		}
		return null;
	}

	public static void add(int j, Gtk.Entry e) {
		elist += list(){idx = j, e=e};
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

public class IORCalc : Gtk.Application {
	Gtk.ApplicationWindow window;
	private Gtk.Notebook nb;
	private void * udata;
	private void * cdata;
	private string? filename;
	private Gtk.TextView textview;
	private Gtk.Button show_cert;
	private Gtk.TextView certview;

	public IORCalc () {
		Object(application_id: "org.stronnag.iorcalc",
			   flags: ApplicationFlags.HANDLES_COMMAND_LINE);

		const OptionEntry[] options = {
			{ "version", 'v', 0, OptionArg.NONE, null, "show version", null},
			{null}
		};
		add_main_option_entries(options);
		handle_local_options.connect(do_handle_local_options);
		activate.connect(handle_activate);

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

    private void handle_activate () {
		udata = null;
		cdata = IORData.allocate_calc_rec();
		window = new Gtk.ApplicationWindow(this);
		add_window (window);
		window.window_position = Gtk.WindowPosition.CENTER;
        window.set_default_size (1024, 800);
		nb = new Gtk.Notebook();

		var header_bar = new Gtk.HeaderBar ();
		header_bar.decoration_layout = "icon:menu,minimize,maximize,close";
		header_bar.set_title ("IOR Rating Calculator");
		header_bar.show_close_button = true;

		window.set_titlebar (header_bar);

		var sbuilder = new Builder.from_resource("/org/stronnag/iorcalc/iorcalc.ui");
        var mm = sbuilder.get_object("menubar") as GLib.MenuModel;

		var fsmenu_button = new Gtk.MenuButton();
        Gtk.Image img = new Gtk.Image.from_icon_name("open-menu-symbolic",
                                                     Gtk.IconSize.BUTTON);
        var childs = fsmenu_button.get_children();
        fsmenu_button.remove(childs.nth_data(0));
        fsmenu_button.add(img);
		var pop = new Gtk.Popover.from_model(fsmenu_button, mm);
        fsmenu_button.set_popover(pop);
        fsmenu_button.set_use_popover(false);
        header_bar.pack_end (fsmenu_button);

        var aq = new GLib.SimpleAction("open",null);
        aq.activate.connect(() => {
				show_cert.sensitive = false;
				var cfn = run_chooser( Gtk.FileChooserAction.OPEN);
				if (cfn != null) {
					filename = cfn;
					ioropen(filename);
				}
			});
        window.add_action(aq);

        aq = new GLib.SimpleAction("save",null);
        aq.activate.connect(() => {
				if(filename != null) {
					IORIO.save_file(filename, udata);
				}
			});
        window.add_action(aq);

        aq = new GLib.SimpleAction("saveas",null);
        aq.activate.connect(() => {
				var cfn = run_chooser( Gtk.FileChooserAction.SAVE);
				if (cfn != null) {
					filename = cfn;
					IORIO.save_file(filename, udata);
				}
			});
        window.add_action(aq);

        aq = new GLib.SimpleAction("calc",null);
        aq.activate.connect(() => {
				show_cert.sensitive = false;
				var s0 = IORData.calc_ior(udata, cdata);
				var s1 = IORData.report(udata, cdata);
				textview.buffer.text = s0;

				if(s1 != null) {
					Gtk.TextIter iter;
					textview.buffer.get_end_iter(out iter);
					textview.buffer.insert(ref iter, "\n-- Warnings\n", -1);
					textview.buffer.insert(ref iter, s1, -1);
				}
				show_cert.sensitive = true;
			});
        window.add_action(aq);

		set_accels_for_action ("win.calc", { "<Primary>c" });
		set_accels_for_action ("win.save", { "<Primary>s" });
		set_accels_for_action ("win.open", { "<Primary>o" });

        aq = new GLib.SimpleAction("quit",null);
        aq.activate.connect(() => {
                window.destroy();
            });

        window.add_action(aq);

		udata = IORIO.read_file(filename);
		populate_grid();

		try {
			var pix =  new Gdk.Pixbuf.from_resource("/org/stronnag/iorcalc/iorcalc.svg");
			window.set_icon(pix);
		} catch (Error e) {
			stderr.printf("failed to set icon %s\n", e.message);
			window.set_icon_name("iorcalc");
		}

		var is_ok = IORData.is_data_valid(udata);
		set_menu_states(is_ok);

		var vbox = new Gtk.Box (Gtk.Orientation.VERTICAL, 0);
		vbox.pack_start (nb,false,false,0);

		textview = new Gtk.TextView();
		textview.monospace = true;
		textview.editable = false;


		var scrolled = new Gtk.ScrolledWindow (null, null);
		scrolled.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
        scrolled.add (textview);
		vbox.pack_start (scrolled, true, true, 0);
		show_cert = new Gtk.Button.with_label("Show Certificate");
		show_cert.sensitive = false;
		show_cert.clicked.connect(() => {
				var fn = Util.mktempname();
				IORData.pcert(udata, cdata, fn, 1);
				certwindow();
				cw_load_file(fn);
			});
		vbox.pack_start (show_cert, false, false, 0);
		window.add (vbox);
        window.show_all();
    }

	private void set_menu_states(bool is_ok) {
//		set_menu_state("save", is_ok);
//		set_menu_state("saveas", is_ok);
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
			var ent = EList.get(i);
			var s = IORData.to_string(udata, i);
			ent.text = s;
		}
	}

	private void set_menu_state(string action, bool state) {
        var ac = window.lookup_action(action) as SimpleAction;
		if(ac != null)
			ac.set_enabled(state);
    }

	private string? run_chooser(Gtk.FileChooserAction action, string fty="json") {
		string fn = null;
		Gtk.FileChooserDialog fc = new Gtk.FileChooserDialog (
			"IOR Data File",
			window, action,
			"_Cancel",
			Gtk.ResponseType.CANCEL,
			(action == Gtk.FileChooserAction.SAVE) ? "_Save" : "_Open",
			Gtk.ResponseType.ACCEPT);

		fc.set_modal(true);
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

		var id = fc.run();
		if (id == Gtk.ResponseType.ACCEPT) {
			fn = fc.get_file().get_path ();
		}
		fc.close();
		fc.destroy();
		return fn;
	}

	private void populate_grid() {
		string tlabs[]={"Names", "Hull", "AFloat/Prop","Rig"};
		Gtk.Label lab;
		Gtk.Entry e;
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
					nb.append_page(grid, new Gtk.Label(tlabs[gid]));
					gid++;
				}
				row = ef.row;
			}
			lab = new Gtk.Label(strdup(ef.prompt));
			lab.justify = Gtk.Justification.LEFT;
			lab.xalign = 0;
			lab.set_padding(2,0);
			if (ef.flag != IORData.EditType.ED_T) {
				grid.attach(lab, col, row);
				col++;
				e = new Gtk.Entry();
				EList.add(i, e);
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
				int j = i;
				e.focus_out_event.connect((ev) => {
						var ent = EList.get(j);
						if (ent != null) {
							var wtext = ent.get_text();
							var utext = IORData.to_string(udata, j);
							if(wtext != utext) {
								IORData.update(wtext, udata, j);
								var t = IORData.to_string(udata, j);
								ent.set_text(t);
								show_cert.sensitive = false;
								var is_ok = IORData.is_data_valid(udata);
								set_menu_state("calc", is_ok);
							}
						}
						return false;
					});

				grid.attach(e, col, row);
				col++;
			} else {
				grid.attach(lab, col, row, 4, 1);
			}
		}
	}


	public void certwindow() {
		var w = new Gtk.Window();
		w.set_title("IOR Certifiate");
		w.set_modal(true);
		w.set_transient_for (window);
		w.set_default_size (1200, 800);

		var vbox = new Gtk.Box (Gtk.Orientation.VERTICAL, 0);
		certview = new Gtk.TextView();
		certview.monospace = true;
		certview.editable = false;

		var scrolled = new Gtk.ScrolledWindow (null, null);
		scrolled.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
		scrolled.add (certview);
		vbox.pack_start (scrolled, true, true, 0);

		Gtk.ButtonBox bbox = new Gtk.ButtonBox (Gtk.Orientation.HORIZONTAL);
		bbox.set_layout (Gtk.ButtonBoxStyle.SPREAD);
		bbox.set_spacing (5);

		var b1 = new Gtk.Button.with_label ("Save to file");
		var b2 = new Gtk.Button.with_label ("Print");
		var b3 = new Gtk.Button.with_label ("Close");


		if(Environment.find_program_in_path("lp") != null) {
			b1.clicked.connect(() => {
					var fn = Util.mktempname();
					IORData.pcert(udata, cdata, fn, 2);
					try {
						string[] args = {"lp", fn};
						Pid pid;
						Process.spawn_async ("/",
											 args,
											 null,
											 SpawnFlags.SEARCH_PATH|SpawnFlags.DO_NOT_REAP_CHILD,
											 null, out pid);
						ChildWatch.add (pid, (pid, status) => {
								Process.close_pid (pid);
								FileUtils.unlink(fn);
							});
					} catch (SpawnError e) {
						FileUtils.unlink(fn);
						print ("Error: %s\n", e.message);
					}
				});
		} else {
			b2.sensitive = false;
		}
		b1.clicked.connect(() => {
				var fn = run_chooser( Gtk.FileChooserAction.SAVE, "txt");
				if (fn != null) {
					IORData.pcert(udata, cdata, fn, 2);
				}
			});

		b3.clicked.connect(() => {
				w.destroy();
			});
		bbox.add (b1);
		bbox.add (b2);
		bbox.add (b3);
		vbox.pack_start (bbox, false, false, 0);
		w.add(vbox);
		w.show_all();
	}

	public void cw_load_file(string fn) {
		if (fn != null) {
			try {
				string buf;
				if(FileUtils.get_contents(fn, out buf)) {
					certview.buffer.text = buf;
				}
			} catch {}
			FileUtils.unlink(fn);
		}
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
        return 0;
    }
}
