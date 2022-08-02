using Gtk;

class IORPrint : Object {
	private static PrintSettings? settings;
	private const int FONT_SIZE = 10;
	private  Pango.Layout layout;
	private int lbreak[2];
	string[] lines;

	public IORPrint() {
		lines = {};
		lbreak = {0,0};
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
				var l2 = i+lbreak[0];
				string s1 = lines[i];
				string s2;
				if (l2 < lines.length) {
					s2 = lines[l2];
				}
				if (s2 == null) {
					s2 = "";
				}
				s1 = s1.replace("\r","");
				s2 = s1.replace("\r","");
				string s = "%-70s%s".printf(s1, s2);
				llines += s;
			}
			lines = llines;
			lbreak[0] = 0;
		}
		return string.joinv("\n", lines);
	}

	public void do_print(Gtk.Window? w) {
		var printop = new Gtk.PrintOperation ();
		printop.embed_page_setup = true;
		if(settings != null) {
			printop.print_settings = settings;
		}
		printop.begin_print.connect((ctxt) => {
				double height = ctxt.get_height() * Pango.SCALE;
				double width = ctxt.get_width() * Pango.SCALE;
				var fsize = FONT_SIZE * Pango.SCALE;
				var fdesc = Pango.FontDescription.from_string("Monospace");
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
			 }
        } catch (Error e) {
            warning ("Error printing: %s", e.message);
        }
	}
}