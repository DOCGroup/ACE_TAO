package gjt;

import java.awt.*;

/**
 * RowLayout lays out components in a row.  At construction 
 * time, both horizontal orientation and vertical orientation 
 * may be specified, along with the gap to use between 
 * components.<p>
 *
 * Horizontal orientation must be one of the following:
 * <dl>
 * <dd> LEFT 
 * <dd> CENTER  
 * <dd> RIGHT
 * </dl>
 *
 * Vertical orientation must be one of the following:
 * <dl>
 * <dd> TOP
 * <dd> CENTER  
 * <dd> BOTTOM
 * </dl>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ColumnLayout
 * @see     Orientation
 */
public class RowLayout implements LayoutManager {
    static private int _defaultGap = 5;

    private int gap;
    private Orientation verticalOrientation;
    private Orientation horizontalOrientation;

    public RowLayout() {
        this(Orientation.CENTER, 
             Orientation.CENTER, _defaultGap);
    }
    public RowLayout(int gap) {
        this(Orientation.CENTER, Orientation.CENTER, gap);
    }
    public RowLayout(Orientation horizontalOrient, 
                     Orientation verticalOrient) {
        this(horizontalOrient, verticalOrient, _defaultGap);
    }
    public RowLayout(Orientation horizontalOrient, 
                     Orientation verticalOrient, int gap) {
        Assert.notFalse(gap >= 0);
        Assert.notFalse(
            horizontalOrient == Orientation.LEFT   ||
            horizontalOrient == Orientation.CENTER ||
            horizontalOrient == Orientation.RIGHT);
        Assert.notFalse(
            verticalOrient   == Orientation.TOP    ||
            verticalOrient   == Orientation.CENTER ||
            verticalOrient   == Orientation.BOTTOM);

        this.gap                   = gap;
        this.verticalOrientation   = verticalOrient;
        this.horizontalOrientation = horizontalOrient;
    }

    public void addLayoutComponent(String name, Component comp) {
    }
    public void removeLayoutComponent(Component comp) {
    }

    public Dimension preferredLayoutSize(Container target) {
        Insets    insets      = target.insets();
        Dimension dim         = new Dimension(0,0);
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension d;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                d = comp.preferredSize();

                dim.width  += d.width;
                dim.height  = Math.max(d.height, dim.height);

                if(i > 0) dim.width += gap;
            }
        }
        dim.width  += insets.left + insets.right;
        dim.height += insets.top + insets.bottom;

        return dim;
    }
    public Dimension minimumLayoutSize(Container target) {
        Insets    insets      = target.insets();
        Dimension dim         = new Dimension(0,0);
        int       ncomponents = target.countComponents();
        Component comp;
        Dimension d;

        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                d = comp.minimumSize();

                dim.width  += d.width;
                dim.height  = Math.max(d.height, dim.height);

                if(i > 0) dim.width += gap;
            }
        }
        dim.width  += insets.left + insets.right;
        dim.height += insets.top + insets.bottom;

        return dim;
    }
    public void layoutContainer(Container target) {
        Insets    insets      = target.insets();
        int       ncomponents = target.countComponents();
        int       top         = 0;
        int       left        = insets.left;
        Dimension tps         = target.preferredSize();
        Dimension targetSize  = target.size();
        Component comp;
        Dimension ps;

        if(horizontalOrientation == Orientation.CENTER)
            left = left + (targetSize.width/2) - (tps.width/2);
        if(horizontalOrientation == Orientation.RIGHT)
            left = left + targetSize.width - tps.width;
        
        for (int i = 0 ; i < ncomponents ; i++) {
            comp = target.getComponent(i);

            if(comp.isVisible()) {
                ps  = comp.preferredSize();

                if(verticalOrientation == Orientation.CENTER)
                    top = (targetSize.height/2) - (ps.height/2);
                else if(verticalOrientation == Orientation.TOP)
                    top = insets.top;
                else if(
                    verticalOrientation == Orientation.BOTTOM)
                    top = targetSize.height - 
                          ps.height - insets.bottom;

                comp.reshape(left,top,ps.width,ps.height);
                left += ps.width + gap;
            }
        }
    }
}
