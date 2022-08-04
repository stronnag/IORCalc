using Gtk;
public class CertWindow : Gtk.Window {
	private Gtk.TextView certview;
    private IORPrint iorprt;

	public  CertWindow(IORSet kf, void *u, void *c) {
		iorprt = new IORPrint(kf.kf);
		set_title("IOR Certifiate");
		set_modal(true);
		set_default_size (1200, 800);

		var vbox = new Gtk.Box (Gtk.Orientation.VERTICAL, 0);
		certview = new Gtk.TextView();
		certview.monospace = true;
		certview.editable = false;

		var scrolled = new Gtk.ScrolledWindow (null, null);
		scrolled.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
		scrolled.add (certview);
		vbox.pack_start (scrolled, true, true, 0);

        var dg = new GLib.SimpleActionGroup();
		var sbuilder = new Builder.from_resource("/org/stronnag/iorcalc/iorcalc.ui");
        var menu = sbuilder.get_object("cert-menu") as GLib.MenuModel;
		var fsmenu_button = new Gtk.MenuButton();
		var pop = new Gtk.Popover.from_model(fsmenu_button, menu);
        fsmenu_button.set_popover(pop);
        fsmenu_button.set_use_popover(false);

        var headerbar = new Gtk.HeaderBar();
		headerbar.pack_end(fsmenu_button);
		headerbar.decoration_layout = "icon:menu,maximize,close";
		headerbar.set_title ("IOR Certificate");
        headerbar.set_show_close_button(true);
		Gtk.Image img = new Gtk.Image.from_icon_name("open-menu-symbolic",
                                                     Gtk.IconSize.BUTTON);
        var childs = fsmenu_button.get_children();
        fsmenu_button.remove(childs.nth_data(0));
        fsmenu_button.add(img);

		this.set_titlebar(headerbar);

		var evtc = new EventControllerKey (this);
		evtc.set_propagation_phase(PropagationPhase.CAPTURE);

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

		add(vbox);
		show_all();
	}

	public void load_file(string fn) {
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
}
