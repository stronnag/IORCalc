using Gtk;

class IORPrint : Object {
	private PrintSettings? settings;
	private PageSetup? page_setup;
	private Pango.Layout layout;
	private int lbreak[2];
	private string[] lines;
	private Pango.FontDescription fdesc;
	private KeyFile kf;

	public signal void font_changed(string fn);

	public IORPrint(KeyFile _kf) {
		lines = {};
		lbreak = {0,0};
		kf = _kf;
		find_print_settings();
	}

	public bool loadfile(string fn) {
		string contents;
		int pln = -1;
		try {
			FileUtils.get_contents(fn, out contents);
			lines = contents.split("\n");
			for(var i = 0; i < lines.length; i++) {
				if (lines[i][0] == '\f') {
					pln = i;
					lines[i] = lines[i].substring(1);
				}
			}
			lbreak[0] = pln;
			lbreak[1] = lines.length-1;
		} catch (Error e) {
			stderr.printf("Failed to load %s, %s\n", fn, e.message);
		}
		return (pln != -1);
	}

	private string? generate_string(bool landscape) {
		if(landscape) {
			string []llines={};
			for(var i = 0; i < lbreak[0]; i++) {
				string? s2=null;
				string s1 = lines[i];
				var l2 = i+lbreak[0];
				if (l2 < lines.length) {
					s2 = lines[l2];
				}
				if (s2 == null) {
					s2 = "";
				}
				s1 = s1.replace("\r","");
				s2 = s2.replace("\r","");
				string s = "%-70s%s".printf(s1, s2);
				llines += s;
			}
			lines = llines;
			lbreak[0] = 0;
		}
		return string.joinv("\n", lines);
	}

	private void find_print_settings() {
		try {
			settings = new PrintSettings.from_key_file (kf, "iorprint");
		} catch (Error e) {
		}
		try {
			page_setup = new PageSetup.from_key_file (kf, "iorpage");
		} catch (Error e) {
			page_setup = new PageSetup();
		}

		string prfont;
		try {
			prfont = kf.get_string ("iorcalc", "pr-font");
		} catch (Error e) {
			prfont = "Monospace 10";
		}
		fdesc = Pango.FontDescription.from_string (prfont);
	}

	public string? get_print_font() {
		return fdesc.get_family();
	}

	public void do_print(Gtk.Window? w) {
		var printop = new Gtk.PrintOperation ();
		if(settings != null) {
			printop.print_settings = settings;
			var ps = printop.get_default_page_setup();
			if (ps == null) {
				printop.set_default_page_setup(page_setup);
			}
		}
		printop.embed_page_setup = true;
		printop.begin_print.connect((ctxt) => {
				double height = ctxt.get_height() * Pango.SCALE;
				double width = ctxt.get_width() * Pango.SCALE;
				var fsize = fdesc.get_size ();

				if(width > height) {
					string s = generate_string(true);
					for(var i =0 ; i < 10; i++) {
						layout = ctxt.create_pango_layout();
						fdesc.set_size (fsize);
						layout.set_font_description (fdesc);
						layout.set_text(s,-1);
						Pango.Rectangle r1;
						Pango.Rectangle r2;
						layout.get_extents (out r1, out r2);
						double fw = fsize;
						double fh = fsize;
						bool shrink = false;
						if ( r2.width  > width) {
							fw = fsize * width / r2.width;
							shrink = true;
						}
						if ( r2.height > height) {
							fh = fsize * height / r2.height ;
							shrink = true;
						}
						if(shrink == false) {
							break;
						}
						fsize = (int)((fw < fh) ? fw : fh);
					}
					printop.set_n_pages(1);
				} else {
					string s = generate_string(false);
					layout = ctxt.create_pango_layout();
					fdesc.set_size (fsize);
					layout.set_font_description (fdesc);
					layout.set_text(s,-1);
					printop.set_n_pages(2);
				}
			});

		printop.draw_page.connect((ctxt, nr) => {
				var cr = ctxt.get_cairo_context();
				cr.set_source_rgb(0, 0, 0);
				if(lbreak[0] == 0) {
					Pango.cairo_show_layout (cr, layout);
				} else {
					int sl;
					int ll;
					if (nr == 0) {
						sl = 0;
						ll = lbreak[0];
					} else {
						sl = lbreak[0];
						ll = lbreak[1];
					}
					Pango.Rectangle r1;
					Pango.Rectangle r2;
					double sp = 0.0;
					for(var i = sl; i < ll; i++) {
						var lline = layout.get_line(i);
						lline.get_extents(out r1, out r2);
						cr.move_to(r2.x / Pango.SCALE, sp);
						sp += r2.height / Pango.SCALE;
						Pango.cairo_show_layout_line (cr, lline);
					}
				}
				cr.stroke();
			});
		try {
            var res = printop.run (Gtk.PrintOperationAction.PRINT_DIALOG, w);
			if (res == PrintOperationResult.APPLY) {
				settings = printop.print_settings ;
				settings.to_key_file (kf, "iorprint");
				var ps = printop.get_default_page_setup();
				if (ps != null) {
					ps.to_key_file (kf, "iorpage");
				}
			}
        } catch (Error e) {
            warning ("Error printing: %s", e.message);
        }
	}

	public void do_font(Window? w) {
		var window = new Gtk.Window();
		window.set_modal(true);
		window.set_transient_for(w);
		window.title = "Printer Font Selection";
		var fc = new Gtk.FontButton();
		fc.set_font_desc(fdesc);
		fc.set_use_font(true);
		fc.title = "Select printer font";
		fc.set_preview_text("IOR LIKE IT'S 1988 AGAIN - PRINTER FONT");
		fc.set_filter_func((f1,f2) => {
				if(f1.is_monospace()) {
					var desc = f2.describe();
					if (desc.get_style() == Pango.Style.NORMAL &&
						desc.get_weight() == Pango.Weight.NORMAL) {
						return true;
					}
				}
				return false;
			});

		fc.font_set.connect(() => {
				fdesc = fc.get_font_desc();
				kf.set_string ("iorcalc", "pr-font", fdesc.to_string());
				font_changed(fdesc.get_family());
			});

		window.add (fc);
		window.show_all();
	}
}