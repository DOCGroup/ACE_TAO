package gjt;

import java.awt.*;

/**
 * Extension of Border that draws a 3D border.
 * 
 * Drawn raised by default, drawing style used by paint() is 
 * controlled by raise() and inset().  Note that raise() and 
 * inset() do not result in anything being painted, but only set
 * the state for the next call to paint().  To set the state and
 * paint in one operation, use paintRaised() and paintInset().
 * <p>
 *
 * The current state of the border may be obtained by calling 
 * isRaised().<p>
 *
 * @version 1.0, Apr 1 1996 
 * @author  David Geary   
 * @see     Border
 * @see     EtchedRectangle
 * @see     gjt.test.BorderTest
 */ 
public class ThreeDBorder extends Border {
    public ThreeDBorder(Component borderMe) {
        this(borderMe, _defaultThickness, _defaultGap); 
    }
    public ThreeDBorder(Component borderMe, 
                        int borderThickness) {
        this(borderMe, borderThickness, _defaultGap);
    }
    public ThreeDBorder(Component borderMe, 
                        int borderThickness, int gap) {
        super(borderMe, borderThickness, gap);
    }
    public void inset() { ((ThreeDRectangle)border()).inset(); }
    public void raise() { ((ThreeDRectangle)border()).raise(); }

    public void paintRaised() { 
        ((ThreeDRectangle)border()).paintRaised(); 
    }
    public void paintInset() { 
        ((ThreeDRectangle)border()).paintInset (); 
    }
    public boolean isRaised() { 
        return ((ThreeDRectangle)border()).isRaised();
    }
    protected DrawnRectangle border() {
        if(border == null)
            border = new ThreeDRectangle(this, thickness);
        return border;
    }
}
