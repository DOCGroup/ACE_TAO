package gjt;
import java.awt.Event;

/**
 * A controller for an ImageButton, this abstract class 
 * establishes the association between itself and an ImageButton
 * and delivers events to its ImageButton.<p>
 *
 * ImageButtonControllers must be constructed with an 
 * ImageButton; the ImageButton's controller gets set by 
 * ImageButtonController's constructor.<p>
 *
 * The ImageButton passed into the constructor must not be null; 
 * this is enforced by an assertion.<p>
 *
 * Methods defined in the MouseController interface are left 
 * for subclasses to implement.  ImageButtonController defines
 * mouseMove(), mouseEnter() and mouseExit() as no-ops, so
 * that extensions of ImageButtonController only have to
 * implement mouseDown(), mouseUp() and mouseDrag().  Note
 * that extensions are still free to override mouseMove(),
 * mouseEnter() and mouseExit() if desired.<p>
 *
 * Subclasses should also call the protected XXXButton(Event)
 * methods below, where XXX is either arm, disarm, activate, or
 * deactivate as appropriate.  SpringyImageButtonController is
 * a good example of this (so is StickyImageButtonController,
 * but it is more complicated than it's springy sibling).<p>
 *
 * @version 1.0, Apr 1 1996
 * @author  David Geary
 * @see     MouseController
 * @see     ImageButton
 * @see     ImageButtonEvent
 * @see     SpringyImageButtonController
 * @see     StickyImageButtonController
 * @see     gjt.test.ImageButtonTest
 */
public abstract class ImageButtonController 
         implements MouseController {
    private ImageButton button;

    ImageButtonController(ImageButton button) {
        Assert.notNull(button);
        this.button = button;
        button.setController(this);
    }
    public ImageButton getButton() {
        return button;
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

    protected void armButton(Event event) {
        button.deliverEvent(
            new ImageButtonEvent(button, 
                                 event, 
                                 ImageButtonEvent.ARM));
    }
    protected void disarmButton(Event event) {
        button.deliverEvent(
            new ImageButtonEvent(button, 
                                 event, 
                                 ImageButtonEvent.DISARM));
    }
    protected void activateButton(Event event) {
        button.deliverEvent(
            new ImageButtonEvent(button, 
                                 event, 
                                 ImageButtonEvent.ACTIVATE));
    }
}
