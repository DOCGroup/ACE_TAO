package gjt;

import java.awt.Event;
import java.awt.Graphics;

/**
 * An ImageButtonController that causes its associated 
 * ImageButton to "stick" when activated.  If the ImageButton 
 * is raised it depresses<b>[1]</b> upon a mouse down and stays 
 * down upon a subsequent mouse up event.  The same "sticky" 
 * behaviour occurs when a depressed ImageButton encounters a 
 * mouse down followed by a subsequent mouse up.<p>
 *
 * Note that false is returned from mouse event handlers; 
 * therefore mouse events will be propagated to the 
 * ImageButton's container.  While this is not always 
 * desirable, it was deemed a better default than swallowing 
 * the event here.  Subclasses may, of course, modify this 
 * behavior.<p>
 * 
 * <b>[1]</b> No psychiatric consultation is necessary.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButton
 * @see     ImageButtonController
 * @see     SpringyImageButtonController
 */
public class StickyImageButtonController 
                    extends ImageButtonController {
    private boolean buttonUpOnLastMouseDown = true;

    public StickyImageButtonController(ImageButton ib) {
        super(ib);
    }
    public boolean mouseDown(Event event, int x, int y) {
        ImageButton button = getButton();

        if(event.modifiers == 0) {  
            if(button.isRaised()) button.paintInset();
            else                  button.paintRaised();

            buttonUpOnLastMouseDown = getButton().isRaised();
            armButton(event);
        }
        return false;
    }
    public boolean mouseUp(Event event, int x, int y) {
        activateButton(event);
        return false;
    }
    public boolean mouseDrag(Event event, int x, int y) {
        ImageButton button = getButton();

        if(event.modifiers == 0) {  
            if(button.isInside(x,y)) {
                if(buttonUpOnLastMouseDown) {
                    if(button.isRaised() == false) {
                        button.paintRaised();
                        armButton(event);
                    }
                }
                else {
                    if(button.isRaised()) {
                        button.paintInset();
                        armButton(event);
                    }
                }
            }
            else {
                if(buttonUpOnLastMouseDown) {
                    if(button.isRaised()) {
                        button.paintInset();
                        disarmButton(event);
                    }
                }
                else {
                    if(button.isRaised() == false) {
                        button.paintRaised();
                        disarmButton(event);
                    }
                }
            }
        }
        return false;
    }
}
