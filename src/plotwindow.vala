using Gtk;

// because it plots ....
public class Conspire : Gtk. DrawingArea {
    double []x;
    double []y;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    int ntype;

    const string[] PROSTAT = {"FGS", "FIGS", "FDS", "MDS", "AIGS", "AGS" };
    const string[] PLANSTAT = {"FGS", "FIGS", "BMAX", "AIGS", "AGS" };
    const string[] SECTSTAT = {"BMAX", "B", "BWL", "OMD", "MD", "CMD" };

    public Conspire(int _n) {
        hexpand = true;
        vexpand = true;
        ntype = _n;
        set_draw_func((da, cr, w, h) => {
                plot_points(cr, w, h);
            });
        set_opacity (1.0);
        queue_draw();
    }

    public void loadpoints(IORData.IORRec* u, IORData.CalcRec *c) {
        switch(ntype) {
        case 0:
            IORData.get_plot_profile(u, c, out x, out y);
            break;
        case 1:
            IORData.get_plot_plan(u, c, out x, out y);
            break;

        case 2:
            IORData.get_plot_section(u, c, out x, out y);
            break;
        }
        xmin = 999;
        xmax = -999;
        ymin = 999;
        ymax = -999;
        for(var i = 0; i < x.length; i++) {
            if(x[i] < xmin)
                xmin = x[i];
            if(x[i] > xmax)
                xmax = x[i];
            if(y[i] < ymin)
                ymin = y[i];
            if(y[i] > ymax)
                ymax = y[i];
        }
    }

    private void plot_points(Cairo.Context cr, int w, int h) {
        double xscale = (w-16)/(xmax - xmin);
        double yscale = (h-16)/(ymax - ymin);
        double scale = Math.fmin(xscale, yscale);
        double xoff = 0;
        double yoff = 0;
        double dashed[] = {14.0, 6.0};

        cr.set_font_size (12);
        cr.set_source_rgba (1, 1, 1,1);
        cr.rectangle(0, 0, w, h);
        cr.fill();

        switch (ntype) {
        case 0:
            xoff = w - 8;
            yoff = h - 8;

            cr.set_source_rgba (0, 0, 0.8, 1);
            cr.move_to(0,  yoff - scale*(-ymin));
            cr.line_to(w,  yoff - scale*(-ymin));
            cr.stroke();
            cr.set_dash(dashed, 0);
            cr.set_source_rgba (0.6, 0.6, 0.6, 1);
            cr.set_font_size (10);
            for(var k =1 ; k < x.length/2; k++) {
                cr.move_to(xoff - scale*(x[k]-xmin), yoff + 4);
                cr.line_to(xoff - scale*(x[k]-xmin), yoff -16 - scale*(ymax-ymin));
                cr.stroke();
                cr.move_to(xoff-12 - scale*(x[k]-xmin), yoff - 24- scale*(ymax-ymin));
                cr.show_text(PROSTAT[k-1]);
                cr.stroke();
            }

            break;
        case 1:
            xoff = w - 8;
            yoff = h - 8;
            cr.set_source_rgba (0, 0, 0.8, 1);
            cr.move_to(0,  yoff - scale*(-ymin));
            cr.line_to(w,  yoff - scale*(-ymin));
            cr.stroke();
            cr.set_dash(dashed, 0);
            cr.set_source_rgba (0.6, 0.6, 0.6, 1);
            cr.set_font_size (10);
            for(var k =1 ; k < x.length-1; k++) {
                cr.move_to(xoff - scale*(x[k]-xmin), yoff + 4);
                cr.line_to(xoff - scale*(x[k]-xmin), yoff -16 - scale*(ymax-ymin));
                cr.stroke();
                cr.move_to(xoff -12 - scale*(x[k]-xmin), yoff - 24- scale*(ymax-ymin));
                cr.show_text(PLANSTAT[k-1]);
                cr.stroke();
            }
            break;
        case 2:
            xoff = 8;
            yoff = h - 8;
            cr.set_source_rgba (0, 0, 0.8, 1);
            cr.move_to(0,  yoff - scale*(-ymin));
            cr.line_to(w,  yoff - scale*(-ymin));
            cr.stroke();
            cr.move_to (8, 0);
            cr.line_to (8, h);
            cr.stroke();
            cr.set_dash(dashed, 0);
            cr.set_source_rgba (0.6, 0.6, 0.6, 1);
            for(var k =1 ; k < x.length-1; k++) {
                if (k < 4) { // BMAX - BWL
                    cr.move_to(xoff + scale*(x[k]-xmin), yoff - 8 - scale*(y[k]-ymin));
                    cr.line_to(xoff + scale*(x[k]-xmin), yoff + 40 - scale*(y[k]-ymin));
                    cr.rel_move_to(8, 0);
                } else {
                    cr.move_to(xoff + scale*(x[k]-xmin), yoff + 8 - scale*(y[k]-ymin));
                    cr.line_to(xoff + scale*(x[k]-xmin), yoff -40  - scale*(y[k]-ymin));
                    cr.rel_move_to(-16, -4);
                }
                cr.show_text(SECTSTAT[k-1]);
                cr.stroke();
            }
            break;
        }
        cr.set_dash(null, 0);
        cr.set_source_rgba (0, 0, 0, 1);
        cr.move_to (xoff  - scale*(x[0]-xmin), yoff  - scale*(y[0]-ymin));
        for(var j = 1; j < x.length; j++) {
            double xval;
            if (xoff > w/2) {
                xval = xoff - scale*(x[j]-xmin);
            } else {
                xval = xoff + scale*(x[j]-xmin);
            }
            if(ntype == 2 && j == 4) {
                cr.move_to(xval, yoff - scale*(y[j]-ymin));
            } else {
                cr.line_to(xval, yoff - scale*(y[j]-ymin));
            }
        }
        cr.stroke();
        if(ntype == 2) {
            cr.set_source_rgba (0.8, 0, 0, 1);
            cr.set_dash(dashed, 0);
            cr.move_to (xoff + scale*(x[3]-xmin), yoff  - scale*(y[3]-ymin));
            cr.line_to(xoff + scale*(x[4]-xmin), yoff - scale*(y[4]-ymin));
            cr.stroke();
        }
    }
}

public class PlotWindow : Gtk.Window {
    private Gtk.Notebook nb;
    private Conspire das[3];
    const string TABLABEL[]={"Profile", "Plan", "BMAX/MD Section"};
    public PlotWindow() {
        set_default_size (800, 600);
        nb = new Gtk.Notebook();
        title = "IORCalc Data Plots";
        for(var i = 0; i < TABLABEL.length; i++) {
            das[i] =   new Conspire(i);
            nb.append_page(das[i], new Gtk.Label(TABLABEL[i]));
        }
        set_child (nb);
    }
    public void load(IORData.IORRec* u, IORData.CalcRec *c) {
        for(var i = 0; i < 3; i++) {
            das[i].loadpoints(u, c);
        }
    }
}

#if TEST
int main (string?[] args) {
    if(args.length > 1) {
        Gtk.init ();
        var cdata = IORData.allocate_calc_rec();
        var  udata = IORIO.read_file(args[1]);
        IORData.calc_ior(udata, cdata);
        var pw = new PlotWindow();
        pw.load(udata, cdata);
        pw.present();
        var ml = MainContext.@default();
        while(Gtk.Window.get_toplevels().get_n_items() > 0) {
            ml.iteration(true);
        }
    }
    return 0;
}
#endif
