package gjt;

import java.awt.*;

/**
 * A bargauge which can be filled (wholly or partially) with a 
 * client-specified color.  Fill color is specified at 
 * construction time; both fill color and fill percent may be 
 * set after construction time.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ThreeDRectangle
 * @see     gjt.test.BargaugeTest
 */
public class Bargauge extends Canvas {
    private double          percentFill = 0;
    private ThreeDRectangle border = new ThreeDRectangle(this);
    private Color           fillColor;

    public Bargauge(Color fillColor) {
        setFillColor(fillColor);
    }
    public void setFillColor(Color fillColor) {
        this.fillColor = fillColor;
    }
    public void setFillPercent(double percentage) {
        Assert.notFalse(percentage >= 0 && percentage <= 100);
        percentFill = percentage;
    }
    public void resize(int w, int h) { 
        reshape(location().x, location().y, w, h);
    }
    public void reshape(int x, int y, int w, int h) { 
        super.reshape(x,y,w,h);
        border.resize(w,h); 
    }
    public Dimension minimumSize() { return preferredSize(); }

    public Dimension preferredSize() {
        int w = border.getThickness() * 3;
        return new Dimension(w, w*4);
    }
    public void paint(Graphics g) {
        border.raise();
        border.paint();
        fill();
    }
    public void fill() {
        Graphics g = getGraphics();

        if((g != null) && (percentFill > 0)) {
            Rectangle b       = border.getInnerBounds();
            int       fillw   = b.width; 
            int       fillh   = b.height;

            if(b.width > b.height) fillw *= percentFill/100;
            else                   fillh *= percentFill/100;

            g.setColor(fillColor);
            border.clearInterior();

            if(b.width > b.height) 
                g.fillRect(b.x, b.y, fillw, b.height);
            else                   
                g.fillRect(b.x, b.y + b.height - fillh, 
                           b.width, fillh);
        }
    }
    protected String paramString() {
        Dimension size = size();
        Orientation orient = size.width > size.height ? 
                             Orientation.HORIZONTAL :
                             Orientation.VERTICAL;
        String    str  = "fill percent=" + percentFill + "," + 
                         "orientation="  + orient      + "," +
                         "color"         + fillColor;
        return str;
    }
}
