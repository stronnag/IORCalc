using Gtk;

namespace IChooser {
	Gtk.FileDialog chooser(string? _fn, string fty="json") {
		Gtk.FileDialog fd = new Gtk.FileDialog();
		if (_fn != null) {
			var fl = File.new_for_path (_fn);
			fd.initial_folder = fl;
		}
		fd.title = "IORCalc";
		var ls = new GLib.ListStore(typeof(Gtk.FileFilter));
		var filter = new Gtk.FileFilter();
		if(fty == "json") {
            filter.set_filter_name ("IORCalc files");
			filter.add_pattern ("*.iorbin");
			filter.add_pattern ("*.json");
            ls.append(filter);
            filter = new Gtk.FileFilter ();
			filter.set_filter_name ("JSON files");
			filter.add_pattern ("*.json");
			ls.append(filter);
            filter = new Gtk.FileFilter ();
			filter.set_filter_name ("Binary (raw)");
			filter.add_pattern ("*.iorbin");
        } else {
			filter.set_filter_name ("Text files");
			filter.add_pattern ("*.txt");
		}
		ls.append(filter);
		filter = new Gtk.FileFilter ();
		filter.set_filter_name ("All Files");
		filter.add_pattern ("*");
		ls.append(filter);
		fd.set_filters(ls);
		return fd;
	}
}

namespace Util {
	string mktempname() {
        var t = Environment.get_tmp_dir();
        var ir = new Rand().int_range (0, 0xffffff);
        var s = Path.build_filename (t, ".ior-%d-%08x".printf(Posix.getpid(), ir));
		return s;
	}

	private  void load_provider_string(ref Gtk.CssProvider provider, string str) {
#if CSS_USE_LOAD_DATA
        provider.load_from_data(str.data);
#elif CSS_USE_LOAD_DATA_STR_LEN
        provider.load_from_data(str, -1);
#else
        provider.load_from_string(str);
#endif
	}

	public void init_css() {
		var css = """
.borderattn { border-style: dotted; border-color: @borders; border-width: 4px; }
.bordernormal { border-style: none; }
""";
		var provider = new CssProvider();
		Util.load_provider_string(ref provider, css);
		Gtk.StyleContext.add_provider_for_display(Gdk.Display.get_default(), provider, Gtk.STYLE_PROVIDER_PRIORITY_USER);

	}
}

public class IEntry : Gtk.Entry {
	public int  idx;
	public IEntry(int _idx) {
		idx = _idx;
	}
}
