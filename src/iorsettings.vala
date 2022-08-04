using Gtk;

public class IORSet : Object {
	public KeyFile kf;
	string keyname;

	public IORSet(){}

	construct {
		kf = new KeyFile();
	}

	public void setup_keyfile() {
		try {
			var basedir = Environment.get_user_config_dir();
			var kfdir = Path.build_filename(basedir, "iorcalc");
			DirUtils.create_with_parents(kfdir, 0755);
			keyname = Path.build_filename(kfdir, "ior.rc");
			kf.load_from_file (keyname, KeyFileFlags.KEEP_COMMENTS);
		} catch  (Error e) {
			print("ini: %s\n", e.message);
		}
	}

	public void save_settings() {
		try {
			kf.save_to_file(keyname);
		} catch  (Error e) {
			print("ini: %s\n", e.message);
		}
	}
}
