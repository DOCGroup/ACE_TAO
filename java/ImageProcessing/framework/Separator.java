package gjt;

import java.awt.*;

/**
 * A separator that is drawn either vertically or horizontally 
 * depending upon how it is laid out.  Can be drawn either 
 * etched-in or etched-out, with varying thicknesses.  Both 
 * thickness and etching are settable at construction time 
 * only.<p>
 *
 * Default thickness is 2 pixels and default etching is 
 * Etching.IN.  Note that thicknesses greater than 4 loose the 
 * etching effect.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     Etching
 * @see     gjt.test.SeparatorTest
 */
public class Separator extends Canvas {
    static private Etching _defaultEtching   = Etching.IN;
    static private int     _defaultThickness = 2;

    private Etching etching;
    private int     thickness;

    public Separator() {
        this(_defaultThickness, _defaultEtching);
    }
    public Separator(int thickness) {
        this(thickness, _defaultEtching);
    }
    public Separator(Etching etching) {
        this(_defaultThickness, etching);
    }
    public Separator(int thickness, Etching etching) {
        this.etching   = etching;
        this.thickness = thickness;
        resize(thickness, thickness);
    }
    public Dimension minimumSize() {
        return preferredSize();
    }
    public Dimension preferredSize() {
        return new Dimension(thickness, thickness);
    }
    public void paint(Graphics g) {
        Dimension size     = size();
        Color brighter = getBackground().brighter().brighter();
        Color darker   = getBackground().darker().darker();

        if(etching == Etching.IN) {
            if(size.width > size.height)
                paintHorizontal(g, size, darker, brighter);
            else
                paintVertical(g, size, darker, brighter);
        }
        else {
            if(size.width > size.height)
                paintHorizontal(g, size, brighter, darker);
            else
                paintVertical(g, size, brighter, darker);
        }
    }
    public String paramString() {
        Dimension   size = size();
        Orientation orient = size.width > size.height ? 
                             Orientation.HORIZONTAL :
                             Orientation.VERTICAL;
        return super.paramString() + "thickness=" + 
               thickness + "," + etching + "," + orient;
    }
    private void paintHorizontal(Graphics g, Dimension size, 
                                 Color top, Color bottom) {
        g.setColor(top);
        g.fillRect(0, (size.height/2) - (thickness/2), 
                       size.width, thickness/2);
        g.setColor(bottom);
        g.fillRect(0, size.height/2, size.width, thickness/2); 
    }
    private void paintVertical(Graphics g, Dimension size, 
                               Color left, Color right) {
        g.setColor(left);
        g.fillRect((size.width/2) - (thickness/2), 
                    0, thickness/2, size.height);
        g.setColor(right);
        g.fillRect(size.width/2, 0, thickness/2, size.height);
    }
}
