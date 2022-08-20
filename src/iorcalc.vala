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
  		set_accels_for_action ("win.plot", { "<Ctrl>p" });
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
