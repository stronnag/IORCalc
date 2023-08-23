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
            filter.set_filter_name ("IORCalc files");
			filter.add_pattern ("*.iorbin");
			filter.add_pattern ("*.json");
            fc.add_filter (filter);
            filter = new Gtk.FileFilter ();
			filter.set_filter_name ("JSON files");
			filter.add_pattern ("*.json");
            fc.add_filter (filter);
            filter = new Gtk.FileFilter ();
			filter.set_filter_name ("Binary (raw)");
			filter.add_pattern ("*.iorbin");
        } else {
			filter.set_filter_name ("Text files");
			filter.add_pattern ("*.txt");
		}
		fc.add_filter (filter);
		filter = new Gtk.FileFilter ();
		filter.set_filter_name ("All Files");
		filter.add_pattern ("*");
		fc.add_filter (filter);
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

	public void load_provider_string(ref Gtk.CssProvider provider, string str) {
#if CSS_USE_LOAD_DATA
        provider.load_from_data(str.data);
#elif CSS_USE_LOAD_DATA_STR_LEN
        provider.load_from_data(str, -1);
#else
        provider.load_from_string(str);
#endif
	}
}

public class IEntry : Gtk.Entry {
	public int  idx;
	public IEntry(int _idx) {
		idx = _idx;
	}
}
