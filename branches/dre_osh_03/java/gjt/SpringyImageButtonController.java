package gjt;

import java.awt.Event;
import java.awt.Graphics;

/**
 * An ImageButtonController that reacts to mouseDown/mouseUp 
 * events exactly as a java.awt.Button does.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButton
 * @see     ImageButtonController
 * @see     StickyImageButtonController
 */
public class SpringyImageButtonController 
                extends ImageButtonController {
    public SpringyImageButtonController(ImageButton ib) {
        super(ib);
    }
    public boolean mouseDown(Event event, int x, int y) {
        if(event.modifiers == 0) {
            getButton().paintInset();
            armButton(event);
        }
        return false;
    }
    public boolean mouseUp(Event event, int x, int y) {
        if(event.modifiers == 0) { 
            if(getButton().isRaised() == false) {
                getButton().paintRaised();
                activateButton(event);
            }
        }
        return false;
    }
    public boolean mouseDrag(Event event, int x, int y) {
        if(event.modifiers == 0) { 
            if(getButton().isInside(x,y)) {
                if(getButton().isRaised()) {
                    getButton().paintInset();
                    armButton(event);
                }
            }
            else {
                if(getButton().isRaised() == false) {
                    getButton().paintRaised();
                    disarmButton(event);
                }
            }
        }
        return false;
    }
}
