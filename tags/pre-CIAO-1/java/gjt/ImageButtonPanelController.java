package gjt;

import java.awt.Event;

/**
 * A controller for an ImageButtonPanel, this abstract class 
 * does nothing more than establish the association between an 
 * ImageButton and its controller.<p>
 *
 * ImageButtonControllers must be constructed with an 
 * ImageButtonPanel; the ImageButtonPanels' controller gets set 
 * by the constructor.<p>
 *
 * The ImageButton passed into the constructor must not be null; 
 * this is enforced by an assertion.<p>
 *
 * Methods defined in the MouseController interface are left 
 * for subclasses to implement.<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     MouseController
 * @see     ImageButtonPanel
 * @see     gjt.test.Toolbar
 */
abstract class ImageButtonPanelController implements 
                                          MouseController {
    private ImageButtonPanel panel;

    ImageButtonPanelController(ImageButtonPanel panel) {
        Assert.notNull(panel);
        this.panel = panel;
        panel.setController(this);
    }
    public ImageButtonPanel panel() {
        return panel;
    }
    public boolean mouseEnter(Event event, int x, int y) {
        return false;
    }
    public boolean mouseExit (Event event, int x, int y) {
        return false;
    }
    public boolean mouseMove (Event event, int x, int y) {
        return false;
    }
}
