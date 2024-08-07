using Gtk;

public class IORWindow : Gtk.ApplicationWindow {
	private Gtk.Notebook nb;
    IORData.IORRec* udata;
    IORData.CalcRec* cdata;
	private Gtk.TextView textview;
	private Gtk.Button show_cert;
	private Gtk.Button run_calc;
	private IEntry[] elist = {};
	private string? filename;
	private IORSet kf;
	private Gtk.MenuButton fsmenu_button;
    private Gtk.HeaderBar header_bar;

	public IORWindow() {
		udata = null;
		cdata = IORData.allocate_calc_rec();
        set_default_size (800, 720);
		nb = new Gtk.Notebook();

		header_bar = new Gtk.HeaderBar ();
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

				var fd = IChooser.chooser(dir);
				fd.open_multiple.begin (this, null, (o,r) => {
						try {
							var files = fd.open_multiple.end(r);
							  for(var j = 0; j < files.get_n_items(); j++) {
                                var gfile = files.get_item(j) as File;
								if (gfile != null) {
									var fn = gfile.get_path ();
									if (j == 0) {
										var dn = gfile.get_parent();
										if (dn != null) {
											kf.kf.set_string("iorcalc", "in-dir", dn.get_path());
										}
										filename = fn;
										ioropen(filename);
										textview.buffer.text = "";
									} else {
										load_file(fn);
									}
								} else {
									stderr.printf("filedialog returns NULL\n");
								}
							  }
						} catch {}
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
                if(show_cert.sensitive) {
                    set_menu_state("cert", true);
                    set_menu_state("plot", true);
				}
				FileUtils.unlink(tfn);
			});
        add_action(aq);

		aq = new GLib.SimpleAction("cert", null);
        aq.activate.connect(() => {
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
        add_action(aq);

		aq = new GLib.SimpleAction("about", null);
        aq.activate.connect(() => {
				var a = new AboutDialog();
				a.version = IORCALC_VERSION_STRING;
				a.authors = {"Jonathan Hudson <jh+ior@daria.co.uk>"};
				a.copyright = "(c) Jonathan Hudson 1984-2023";
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

		aq = new GLib.SimpleAction("plot",null);
        aq.activate.connect(() => {
                set_menu_state("plot", false);
                var pw = new PlotWindow();
                pw.close_request.connect(() => {
                        set_menu_state("plot", show_cert.sensitive);
                        return false;
                    });
                pw.load(udata, cdata);
                pw.present();
            });
        add_action(aq);

		set_menu_state("plot", false);
		set_menu_state("cert", false);

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

		show_cert = new Gtk.Button.with_label("View Certificate");
		show_cert.set_action_name("win.cert");

		bbox.hexpand = true;
 		run_calc.hexpand = true;
		show_cert.hexpand = true;
		run_calc.halign = Gtk.Align.CENTER;
		show_cert.halign = Gtk.Align.CENTER;

		bbox.prepend(run_calc);
		bbox.append(show_cert);

		vbox.append (scrolled);
		vbox.append (bbox);
		Util.init_css(textview);
		var droptgt = new Gtk.DropTarget(typeof (Gdk.FileList), Gdk.DragAction.COPY);
		droptgt.drop.connect((tgt, value, x, y) => {
				set_target(textview, false);
				if(value.type() == typeof (Gdk.FileList)) {
					var flist = ((Gdk.FileList)value).get_files();
					foreach(var u in flist) {
						var fn = u.get_path();
                        load_file(fn);
					}
				}
				return true;
			});
		droptgt.accept.connect((d) => {
				set_target(textview, true);
				return true;
			});
		droptgt.leave.connect((d) => {
				set_target(textview, false);
			});
		textview.add_controller((EventController)droptgt);
		set_child (vbox);
	}

    private void load_file(string fn) {
        if(valid_file(fn)) {
            if(IORData.is_empty(udata)) {
                ioropen(fn);
            } else {
                var w = new IORWindow();
                this.application.add_window (w);
                w.setup(kf, fn);
                w.run();
            }
        }
    }

	public void set_target(Gtk.Widget w, bool active) {
		w.set_name(active ? "borderattn" : "bordernormal");
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
			string buf;
			if(FileUtils.get_contents(fn, out buf)) {
                if (buf.has_prefix("ior88") ||
                    (buf.contains("\"yacht\" : ") &&
                     buf.contains("\"icert\" : ") &&
                     buf.contains("\"irgy\" : ") &&
                     buf.contains("\"bmax\" : ")))
                res = true;
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
        set_menu_state("plot", act);
        set_menu_state("cert", act);

	}

	public void setup(IORSet _kf, string? _filename) {
		kf = _kf;
		filename = _filename;
	}

	public void run() {
        ioropen(filename, false);
		present();
	}

	public void ioropen(string? fn, bool update=true) {
		udata = IORIO.read_file(fn);
        if(update) {
            update_grid();
        } else {
            populate_grid();
        }
		var is_ok = IORData.is_data_valid(udata);
		set_menu_states(is_ok);
        if(((string)udata.yacht).length > 0) {
            var s = "IORCalc - %s".printf((string)udata.yacht);
            header_bar.set_title_widget (new Gtk.Label(s));
        }
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
		var fd = IChooser.chooser(dir);
		fd.save.begin (this, null, (o,r) => {
				try {
					var fh = fd.save.end(r);
					var fn = fh.get_path ();
					var dn = fh.get_parent();
					if (dn != null) {
						kf.kf.set_string("iorcalc", "out-dir", dn.get_path());
					}
					filename = fn;
					IORIO.save_file(filename, udata, "j");
				} catch {}
			});
	}

	private void set_menu_states(bool is_ok) {
		set_menu_state("calc", is_ok);
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
                            set_menu_state("plot", false);
                            set_menu_state("cert", false);
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
