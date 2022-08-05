using Gtk;

public class CertWindow : Gtk.Window {
	private Gtk.TextView certview;
	private IORPrint iorprt;

	public  CertWindow(IORSet kf, void *u, void *c) {
		iorprt = new IORPrint(kf.kf);
		set_title("IOR Certifiate");
		set_modal(true);
		set_default_size (1200, 800);

		var headerbar = new Gtk.HeaderBar();
		this.set_titlebar(headerbar);

		var vbox = new Gtk.Box (Gtk.Orientation.VERTICAL, 0);
		certview = new Gtk.TextView();
		certview.monospace = true;
		certview.editable = false;
		certview.valign = Gtk.Align.FILL;
		certview.vexpand = true;

		var scrolled = new Gtk.ScrolledWindow ();
		scrolled.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
		scrolled.set_child (certview);

		vbox.append (scrolled);

        var dg = new GLib.SimpleActionGroup();
		var sbuilder = new Builder.from_resource("/org/stronnag/iorcalc/iorcalc.ui");
        var menu = sbuilder.get_object("cert-menu") as GLib.MenuModel;
		var fsmenu_button = new Gtk.MenuButton();
		fsmenu_button.icon_name = "open-menu-symbolic";

        var pop = new Gtk.PopoverMenu.from_model(menu);
        fsmenu_button.set_popover(pop);

		headerbar.pack_end(fsmenu_button);
		headerbar.decoration_layout = "icon:menu,maximize,close";

		var evtc = new EventControllerKey ();
		evtc.set_propagation_phase(PropagationPhase.CAPTURE);
		((Gtk.Widget)this).add_controller(evtc);

		var saq = new GLib.SimpleAction("save",null);
        saq.activate.connect(() => {
				var fc = IChooser.chooser(this, null, Gtk.FileChooserAction.SAVE, "txt");
				fc.response.connect((result) => {
						if (result== Gtk.ResponseType.ACCEPT) {
							var fn = fc.get_file().get_path ();
							IORData.pcert(u, c, fn, 2);
						}
						fc.close();
					});
            });
        dg.add_action(saq);

		var paq = new GLib.SimpleAction("print",null);
        paq.activate.connect(() => {
				var fn = Util.mktempname();
				IORData.pcert(u, c, fn, 2);
				if(iorprt.loadfile(fn)) {
					iorprt.do_print(this);
				}
				FileUtils.unlink(fn);
			});
        dg.add_action(paq);

		var faq = new GLib.SimpleAction("font",null);
        faq.activate.connect(() => 	{
				iorprt.do_font(this);
			});
        dg.add_action(faq);

		var qaq = new GLib.SimpleAction("quit",null);
        qaq.activate.connect(() => {
				destroy();
            });
        dg.add_action(qaq);
		this.insert_action_group("cert", dg);

		iorprt.font_changed.connect((s) => {
				if(certview.buffer.text != null) {
					set_cert_text_font(s);
				}
			});

		evtc.key_pressed.connect((kv, kc, mfy) => {
				if((mfy & Gdk.ModifierType.CONTROL_MASK) != 0) {
					switch(kv) {
					case 'p':
						paq.activate(null);
						break;
					case 'q':
					    qaq.activate(null);
						break;
					case 'f':
					    faq.activate(null);
						break;
					case 's':
						saq.activate(null);
						break;
					default:
					    break;
					}
				}
				return false;
			});
		set_child(vbox);
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
		var css = "textview {font-family: \"%s\",monospace;}".printf(fontname);
		var provider = new CssProvider();
		provider.load_from_data(css.data);
		var stylec = certview.get_style_context();
		stylec.add_provider(provider, Gtk.STYLE_PROVIDER_PRIORITY_USER);
	}
}
