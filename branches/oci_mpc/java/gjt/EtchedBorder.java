package gjt;

import java.awt.*;

/**
 * An extension of Border that draws an etched border.
 * 
 * Drawn etchedIn by default, drawing style used by paint() is 
 * controlled by etchedIn() and etchedOut().  Note that 
 * etchedIn() and etchedOut() do not result in anything being 
 * painted, but only set the state for the next call to paint().
 * To set the state and paint in one operation, use 
 * paintEtchedIn() and paintEtchedOut().<p>
 *
 * The current state of the border may be obtained by calling 
 * isEtchedIn().<p>
 *
 * @version 1.0, Apr 1 1996 
 * @author  David Geary   
 * @see     Border
 * @see     ThreeDRectangle
 * @see     gjt.test.BorderTest
 */ 
public class EtchedBorder extends Border {
    public EtchedBorder(Component borderMe) {
        this(borderMe, _defaultThickness, _defaultGap); 
    }
    public EtchedBorder(Component borderMe, 
                        int borderThickness) {
        this(borderMe, borderThickness, _defaultGap);
    }
    public EtchedBorder(Component borderMe, 
                        int borderThickness, int gap) {
        super(borderMe, borderThickness, gap);
    }
    public void etchedIn() {
        ((EtchedRectangle)border()).etchedIn();      
    }
    public void etchedOut() {
        ((EtchedRectangle)border()).etchedOut();     
    }
    public void paintEtchedIn() {
        ((EtchedRectangle)border()).paintEtchedIn ();
    }
    public void paintEtchedOut() {
        ((EtchedRectangle)border()).paintEtchedOut();
    }
    public boolean isEtchedIn() { 
        return ((EtchedRectangle)border()).isEtchedIn();
    }
    protected String paramString() {
        return super.paramString() + (EtchedRectangle)border();
    }
    protected DrawnRectangle border() {
        if(border == null)
            border = new EtchedRectangle(this, thickness);
        return border;
    }
}
