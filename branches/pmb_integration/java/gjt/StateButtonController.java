package gjt;

import java.awt.Event;

/**
 * A controller for a StateButton, that cycles through a 
 * series of images which reside in the StateButton class.  
 * Each time a mouse up is detected in the StateButton, the 
 * buttons image is set to the next image in the array.
 * 
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     StateButton
 * @see     SpringyImageButtonController
 * @see     gjt.test.StateButtonTest
 */
class StateButtonController extends SpringyImageButtonController {
    public StateButtonController(StateButton button) {
        super(button);
    }
    public boolean mouseUp(Event event, int x, int y) {
        StateButton button = (StateButton)getButton();
        button.setImage(button.nextImage());
        activateButton(event);
        return super.mouseUp(event, x, y);
    }
}
