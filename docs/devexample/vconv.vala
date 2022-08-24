int main (string?[] args) {
    if(args.length > 1) {
        IORData.IORRec* udata = IORIO.read_file(args[1]);
        var fn = args[1].replace("json","iorbin");
        var fp = FileStream.open(fn,"w");
        fp.write({'i','o','r','8','8',0});
        fp.write((uint8[])udata);
    }
    return 0;
}
