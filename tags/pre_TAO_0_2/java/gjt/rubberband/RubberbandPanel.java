package gjt.rubberband;

import java.awt.*;

/**
 * An extension of Panel which is fitted with a Rubberband.  
 * Handling of mouse events is automatically handled for 
 * rubberbanding.<p>
 *
 * Clients may set or get the Rubberband at any time.<p>
 *
 * @version 1.0, Dec 27 1995
 * @author  David Geary
 * @see     Rubberband
 * @see     gjt.test.RubberbandTest
 */
public class RubberbandPanel extends Panel {
    private Rubberband rubberband;

    public void setRubberband(Rubberband rubberband) {
        this.rubberband = rubberband;
    }
    public Rubberband getRubberband() {
        return rubberband;
    }
    public boolean mouseDown(Event event, int x, int y) {
        rubberband.anchor(new Point(x,y));
        return false;
    }
    public boolean mouseDrag(Event event, int x, int y) {
        rubberband.stretch(new Point(x,y));
        return false;
    }
    public boolean mouseUp(Event event, int x, int y) {
        rubberband.end(new Point(x,y));
        return false;
    }
}
