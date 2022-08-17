using Gtk;

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

#if !OS_windows
#if !OS_freebsd
		var droptgt = new Gtk.DropTarget(typeof (Gdk.FileList), Gdk.DragAction.COPY);
		droptgt.on_drop.connect((tgt, value, x, y) => {
				set_target(textview, false);
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
#else
		// Incomplete definition of Gdk.FileList (no get_files()).
		var droptgt = new Gtk.DropTarget(typeof (string), Gdk.DragAction.COPY);
		droptgt.on_drop.connect((tgt, value, x, y) => {
				set_target(textview, false);
				if(value.type() == typeof (string)) {
					foreach(var u in ((string)value).split( "\r\n")) {
						if (u!= null && u.length > 0) {
							try {
								var fn = Filename.from_uri(u);
								if(valid_file(fn)) {
									var w = new IORWindow();
									this.application.add_window (w);
									w.setup(kf, fn);
									w.run();
								}
							} catch (Error e) {
								stderr.printf("drop: %s %s\n", u, e.message);
							}
						}
					}
				}
				return true;
			});
#endif
		droptgt.accept.connect((d) => {
				set_target(textview, true);
				return true;
			});
		droptgt.leave.connect((d) => {
				set_target(textview, false);
			});
		textview.add_controller((EventController)droptgt);
#endif
		set_child (vbox);
	}


#if !OS_windows
	public void set_target(Gtk.Widget w, bool active) {
		string css;
		if (active) {
			css =  "textview { border-style: dotted; border-color: @borders; border-width: 4px; }";
		} else {
			css =  "textview { border-style: none; }";
		}
		var provider = new CssProvider();
		provider.load_from_data(css.data);
		var stylec = w.get_style_context();
		stylec.add_provider(provider, Gtk.STYLE_PROVIDER_PRIORITY_USER);
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
				if(buf.contains("\"yacht\" : ") &&
				   buf.contains("\"icert\" : ") &&
				   buf.contains("\"irgy\" : ") &&
				   buf.contains("\"bmax\" : ") )
					res = true;
				}
		} catch (Error e) {
			print("validate %s\n", e.message);
		}
		return res;
	}
#endif
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
