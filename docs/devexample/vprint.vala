int main (string?[] args) {
    if(args.length > 1) {
        IORData.CalcRec* c = IORData.allocate_calc_rec();
        IORData.IORRec* u = IORData.allocate_ior_rec();
        var fp = FileStream.open(args[1],"r");
        if(fp != null) {
            uint8 hdr[6];
            var n = fp.read(hdr);
            if(n == 6  && (string)hdr == "ior88") {
                fp.read((uint8[])u);
                var s = IORData.calc_ior(u, c);
                print("%s\n", s);
                var fn = args[1].replace("iorbin", "txt");
                IORData.pcert(u, c,fn,1);
            }
        }
    }
    return 0;
}
