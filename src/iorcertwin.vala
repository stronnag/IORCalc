using Gtk;

public class CertWindow : Gtk.Window {
	private Gtk.TextView certview;
	private IORPrint iorprt;

	public  CertWindow(Gtk.Window _w, IORSet kf, IORData.IORRec* u, IORData.CalcRec* c) {
		iorprt = new IORPrint(kf.kf);
		iorprt.font_changed.connect((s) => {
				if(certview.buffer.text != null) {
					set_cert_text_font(s);
				}
			});

        title = "IORCalc Certificate - %s".printf((string)u.yacht);
		set_transient_for(_w);
		set_default_size (1200, 800);

		var headerbar = new Gtk.HeaderBar();
		this.set_titlebar(headerbar);

		certview = new Gtk.TextView();
		certview.monospace = true;
		certview.editable = false;
		certview.valign = Gtk.Align.FILL;
		certview.vexpand = true;

		var scrolled = new Gtk.ScrolledWindow ();
		scrolled.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
		scrolled.set_child (certview);

        var dg = new GLib.SimpleActionGroup();
		var sbuilder = new Builder.from_resource("/org/stronnag/iorcalc/iorcalc.ui");
        var menu = sbuilder.get_object("cert-menu") as GLib.MenuModel;
		var fsmenu_button = new Gtk.MenuButton();
		fsmenu_button.icon_name = "open-menu-symbolic";

        var pop = new Gtk.PopoverMenu.from_model(menu);
        fsmenu_button.set_popover(pop);

		headerbar.pack_end(fsmenu_button);
		headerbar.decoration_layout = "icon:menu,maximize,close";

		var saq = new GLib.SimpleAction("save",null);
        saq.activate.connect(() => {
				var fd = IChooser.chooser(null, "txt");
				/*
                fc.add_choice("PAGING", "Paging",
                              {"2", "1"},
                              {"Two Pages", "Single Page"});
				*/
				fd.save.begin (this, null, (o,r) => {
						try {
							var fh = fd.save.end(r);
							var fn = fh.get_path ();
							//                            var pchoice = fc.get_choice("PAGING");
                            //int pg = (pchoice == "1") ? 1 : 2;
							int pg = 2;
							IORData.pcert(u, c, fn, pg);
						} catch {}
					});
            });
        dg.add_action(saq);

		saq = new GLib.SimpleAction("print",null);
        saq.activate.connect(() => {
				var fn = Util.mktempname();
				IORData.pcert(u, c, fn, 2);
				if(iorprt.loadfile(fn)) {
					iorprt.do_print(this);
				}
				FileUtils.unlink(fn);
			});
        dg.add_action(saq);

		saq = new GLib.SimpleAction("font",null);
        saq.activate.connect(() => 	{
				iorprt.do_font(this);
			});
        dg.add_action(saq);

		saq = new GLib.SimpleAction("close",null);
        saq.activate.connect(() => {
				close();
            });
        dg.add_action(saq);

		this.insert_action_group("cert", dg);
		add_binding_action('p', Gdk.ModifierType.CONTROL_MASK, "cert.print", null);
		add_binding_action('s', Gdk.ModifierType.CONTROL_MASK, "cert.save", null);
		add_binding_action('f', Gdk.ModifierType.CONTROL_MASK, "cert.font", null);
		add_binding_action('q', Gdk.ModifierType.CONTROL_MASK, "cert.close", null);

		set_child(scrolled);
		show();
	}

	public void load_file(string fn) {
		if (fn != null) {
			try {
				string buf;
				if(FileUtils.get_contents(fn, out buf)) {
					string prfont = iorprt.get_print_font();
					var tbuf = certview.buffer;
					tbuf.text = buf;
					set_cert_text_font(prfont);
				}
			} catch (Error e) {
				stderr.printf("load: %s\n", e.message);
			}
			FileUtils.unlink(fn);
		}
	}

	private void set_cert_text_font(string fontname) {
		var css = "#printmono {font-family: \"%s\",monospace;}".printf(fontname);
		var provider = new CssProvider();
		Util.load_provider_string(ref provider, css);
		Gtk.StyleContext.add_provider_for_display (Gdk.Display.get_default(), provider, Gtk.STYLE_PROVIDER_PRIORITY_USER);
		certview.set_name("printmono");
	}
}
