namespace IORIO {
	public void save_file(string fn, IORData.IORRec *u, string fmt="j") {
        string ifn = fn;
        if(fmt == "j") {
            if(!fn.has_suffix(".json")) {
                ifn = fn + ".json";
            }
        } else {
            if(!fn.has_suffix(".iorbin")) {
                ifn = fn + ".iorbin";
            }
        }
        var os = FileStream.open(ifn, "w");
        if(os != null) {
            if(fmt == "j") {
                var builder =  new Json.Builder ();
                builder.begin_object ();
                IORData.EditField ef;
                for(var i = 0; ; i++) {
                    var ok = IORData.get(i, out ef);
                    if (ok != 0)
                        break;

                    if(ef.ename != null) {
                        builder.set_member_name (ef.ename);
                        switch(ef.flag) {
                        case IORData.EditType.ED_F:
                            builder.add_double_value(IORData.get_double(u, i));
                            break;
                        case IORData.EditType.ED_I:
                            builder.add_int_value (IORData.get_int(u, i));
                            break;
                        case IORData.EditType.ED_C:
                            builder.add_string_value (IORData.get_string(u, i));
                            break;
                        default:
                            break;
                        }
                    }
                }
                builder.end_object ();
                var gen = new Json.Generator ();
                gen.indent = 1;
                gen.indent_char = ' ';
                gen.set_pretty(true);
                var root = builder.get_root ();
                gen.set_root (root);
                os.puts(gen.to_data (null));
                os.putc('\n');
            } else {
                os.write({'i','o','r','8','8',0});
                os.write((uint8[])u);
            }
        }
	}

	public IORData.IORRec* read_file(string? fn) {
		var u = IORData.allocate_ior_rec();
		if (fn != null) {
            if(fn.has_suffix("json")) {
                try {
                    string js;
                    IORData.EditField ef;
                    if(FileUtils.get_contents(fn, out js)) {
                        var parser = new Json.Parser ();
                        parser.load_from_data (js);
                        var root = parser.get_root ();
                        var obj = root.get_object ();
                        for(var i = 0; ; i++) {
                            var ok = IORData.get(i, out ef);
                            if (ok != 0)
                                break;
                            if (ef.ename != null) {
                                if(obj.has_member(ef.ename)) {
                                    switch(ef.flag) {
                                    case IORData.EditType.ED_F:
                                        var d = obj.get_double_member(ef.ename);
                                        IORData.set_double(d, u, i);
                                        break;
                                    case IORData.EditType.ED_I:
                                        var v = (int)obj.get_int_member(ef.ename);
                                        IORData.set_int(v, u, i);
                                        break;
                                    case IORData.EditType.ED_C:
                                        var s = obj.get_string_member(ef.ename);
                                        IORData.set_string(s, u, i);
                                        break;
                                    default:
                                        break;
                                    }
                                } else {
                                    stderr.printf("Missing JSON key \"%s\"\n", ef.ename);
                                }
                            }
                        }
                    }
                } catch {}
            } else {
                var fp = FileStream.open(fn, "r");
                if (fp != null) {
                    uint8 header[6];
                    var n = fp.read(header);
                    if(n == 6 && (string)header == "ior88") {
                        fp.read((uint8[])u);
                    }
                }
            }
        }
		return u;
	}
}