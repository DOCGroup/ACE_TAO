package gjt;

import java.awt.Event;

/**
 * A controller for an ImageButtonPanel that ensures that only 
 * one ImageButton in its associated ImageButtonPanel is 
 * selected at a time.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     ImageButtonPanelController
 * @see     ImageButton
 * @see     ImageButtonPanel
 * @see     gjt.test.ToolbarTest
 */
class RadioImageButtonPanelController 
                 extends ImageButtonPanelController {
    ImageButton down;

    public RadioImageButtonPanelController(
                ImageButtonPanel panel) {
        super(panel);
    }
    public boolean mouseDown(Event event, int x, int y) {
        ImageButton button;

        if(event.target instanceof ImageButton) {
            button = (ImageButton)event.target;
            if(down == button) return false;

            if(down != null)
                down.paintRaised();

            down = button;
        }
        return false;
    }
    public boolean mouseUp(Event event, int x, int y) {
        return false;
    }
    public boolean mouseDrag(Event event, int x, int y) {
        return false;
    }
}
