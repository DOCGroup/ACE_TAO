package gjt;

import java.awt.*;

/**
 * A panel containing a single component, around which a border 
 * is drawn.  Of course, the single component may be a 
 * container which may contain other components, so a Border
 * can surround multiple components.<p>
 * 
 * Thickness of the border, and the gap between the Component 
 * and the border are specified at time of construction.
 * Default border thickness is 2 - default gap is 0.<p>
 *
 * Border color may be set via setLineColor(Color).<p>
 *
 * Border employs a DrawnRectangle to paint the border.  Derived 
 * classes are free to override DrawnRectangle border() if they 
 * wish to use an extension of DrawnRectangle for drawing their 
 * border.<p>
 *
 * The following code snippet, from gjt.test.BorderTest creates
 * and AWT Button, and embeds the button in a border.  That
 * border is then embedded in another border.  The AWT Button
 * winds up inside of a cyan border with a pixel width of 7,
 * inside of a black border (pixel width 2):<p>
 *
 * <pre>
 *      private Border makeBorderedAWTButton() {
 *          Button button;
 *          Border cyanBorder, blackBorder;
 *
 *          button = new Button("Button Inside Two Borders");
 *          cyanBorder = new Border(button, 7);
 *          cyanBorder.setLineColor(Color.cyan);
 *
 *          blackBorder = new Border(cyanBorder);
 *
 *          return blackBorder;
 *      }
 *</pre>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     DrawnRectangle
 * @see     ThreeDBorder
 * @see     EtchedBorder
 * @see     gjt.test.BorderTest
 */
public class Border extends Panel {
    protected int            thickness;
    protected int            gap;
    protected DrawnRectangle border;

    protected static int _defaultThickness = 2;
    protected static int _defaultGap       = 0;

    public Border(Component borderMe) { 
        this(borderMe, _defaultThickness, _defaultGap);
    }
    public Border(Component borderMe, int thickness) {
        this(borderMe, thickness, _defaultGap);
    }
    public Border(Component borderMe, int thickness, int gap) {
        this.thickness = thickness;
        this.gap       = gap;

        setLayout(new BorderLayout());
        add("Center", borderMe);
    }
    public Insets insets() {
        return new Insets(thickness+gap, thickness+gap, 
                          thickness+gap, thickness+gap);
    }
    public Rectangle getInnerBounds() { 
        return border().getInnerBounds(); 
    }
    public void setLineColor(Color c) { 
        border().setLineColor(c);         
    }
    public Color getLineColor()        { 
        return border().getLineColor();   
    }
    public void paint(Graphics g) { 
        border().paint();                 
    }
    public void resize(int w, int h) {
        Point location = location();
        reshape(location.x, location.y, w, h);
    }
    public void reshape(int x, int y, int w, int h) {
        super.reshape(x, y, w, h);
        border().resize(w, h);
    }
    protected String paramString() {
        return super.paramString() + ",border=" + 
               border().toString() + ",thickness=" + thickness 
               + ",gap=" + gap;
    }
    protected DrawnRectangle border() {
        if(border == null)
            border = new DrawnRectangle(this, thickness);
        return border;
    }
}
