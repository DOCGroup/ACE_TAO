package gjt;

import java.awt.*;

/**
 * A DrawnRectangle that draws an etched border.<p>
 * 
 * Drawn etched in by default, drawing style used by paint() is 
 * controlled by etchedIn() and etchedOut().  Note that 
 * etchedIn() and etchedOut() do not result in anything being 
 * painted, but only set the state for the next call to paint(). 
 * To set the state and paint in one operation, use 
 * paintEtchedIn() and paintEtchedOut().<p>
 * 
 * Although it is permissible to set the thickness of 
 * EtchedRectangles, they tend to loose the etching effect 
 * if thickness is greater than 4.<p>
 *
 * The current state of the rectangle may be obtained by 
 * calling isEtchedIn().
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     DrawnRectangle
 * @see     ThreeDRectangle
 * @see     gjt.test.DrawnRectangleTest
 */
public class EtchedRectangle extends DrawnRectangle {
    protected static Etching _defaultEtching = Etching.IN;
    private Etching etching;

    public EtchedRectangle(Component drawInto) {
        this(drawInto, _defaultEtching, 
             _defaultThickness, 0, 0, 0, 0);
    }
    public EtchedRectangle(Component drawInto, int thickness) {
        this(drawInto, _defaultEtching, thickness, 0, 0, 0, 0);
    }
    public EtchedRectangle(Component drawInto, int x, int y, 
                                               int w, int h) {
        this(drawInto, _defaultEtching, 
             _defaultThickness, x, y, w, h);
    }
    public EtchedRectangle(Component drawInto, int thickness,
                                               int x, int y, 
                                               int w, int h) {
        this(drawInto, _defaultEtching, thickness, x, y, w, h);
    }
    public EtchedRectangle(Component drawInto, Etching etching, 
                           int thickness, int x, int y, 
                           int w, int h) {
        super(drawInto, thickness, x, y, w, h);
        this.etching = etching;
    }
    public void    etchedIn  () { etching = Etching.IN;        }
    public void    etchedOut () { etching = Etching.OUT;       }
    public boolean isEtchedIn() { return etching == Etching.IN;}

    public void paint() {
        if(etching == Etching.IN) paintEtchedIn();
        else                      paintEtchedOut();
    }
    public void paintEtchedIn() {
        Graphics g = drawInto.getGraphics();
        if(g != null)
            paintEtched(g, getLineColor(), brighter());

        etchedIn();
    }
    public void paintEtchedOut() {
        Graphics g = drawInto.getGraphics();
        if(g != null)
            paintEtched(g, brighter(), getLineColor());

        etchedOut();
    }
    public String paramString() { 
        return super.paramString() + "," + etching;
    }
    private void paintEtched(Graphics g, 
                             Color topLeft, 
                             Color bottomRight) {
        int  thickness = getThickness();
        int  w = width  - thickness;
        int  h = height - thickness;

        g.setColor(topLeft);
        for(int i=0; i < thickness/2; ++i) 
            g.drawRect(x+i, y+i, w, h);

        g.setColor(bottomRight);

        for(int i=0; i < thickness/2; ++i) 
            g.drawRect(x+(thickness/2)+i, 
                       y+(thickness/2)+i, w, h);
    }
}
